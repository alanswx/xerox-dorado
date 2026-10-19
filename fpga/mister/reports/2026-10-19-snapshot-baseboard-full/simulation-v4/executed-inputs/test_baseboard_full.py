#!/usr/bin/env python3
"""Complete actual BaseBoard original/candidate continuation and physical restore."""
import argparse,hashlib,json,re,resource,subprocess
from pathlib import Path
import generate_baseboard_full as gen
HERE=Path(__file__).resolve().parent


def fixture(root,m):
 ports=[]
 for x in re.finditer(r'^\s+(input|output)\s+wire\s*(?:\[(\d+):(\d+)\]\s*)?(\w+)(?=[,\n])',(root/gen.SOURCE).read_text(),re.M):
  direction,hi,lo,name=x.groups()
  if name!='sys_clk':ports.append(dict(direction=direction,name=name,width=1 if hi is None else abs(int(hi)-int(lo))+1))
 inputs=[p for p in ports if p['direction']=='input'];outputs=[p for p in ports if p['direction']=='output'];ni=sum(p['width']for p in inputs);no=sum(p['width']for p in outputs)
 code=[f'''module baseboard_full_fixture(
 input wire host_clk,run,run_ref,input wire [{ni-1}:0] stimulus,
 input wire freeze,held,request,write,input wire [16:0] state_index,input wire [17:0] state_data,
 input wire [9:0] probe_row,output wire frozen,done,error,output wire [17:0] result,
 output wire [{no-1}:0] original_outputs,candidate_outputs,
 output wire [40365:0] original_state,candidate_state,
 output wire [31:0] original_probe,candidate_probe,ram_data,ram_pipeline,
 output wire [39:0] ram_address,output wire [3:0] ram_write,
 output wire [37:0] riot_pins[5],output wire advance,ref_advance);
 reg enabled=0,ref_enabled=0;
 always @(negedge host_clk)begin enabled<=run;ref_enabled<=run_ref;end
 wire guest_clk=host_clk & enabled,ref_clk=host_clk & ref_enabled;
 assign advance=enabled;assign ref_advance=ref_enabled;''']
 driven={p['name']for p in outputs}
 for copy,module,clock in [('original',gen.ORIGINAL_MODULE,'ref_clk'),('candidate',gen.MODULE,'guest_clk')]:
  for p in outputs:code.append('wire '+(''if p['width']==1 else f"[{p['width']-1}:0] ")+copy+'_'+p['name']+';')
  con=['.sys_clk('+clock+')'];offset=0
  for p in inputs:
   value=f'stimulus[{offset}]'if p['width']==1 else f"stimulus[{offset}+:{p['width']}]"
   if p['name']+'__drv'in driven:value+=' | '+copy+'_'+p['name']+'__drv'
   con.append(f".{p['name']}({value})");offset+=p['width']
  con +=[f".{p['name']}({copy}_{p['name']})"for p in outputs]
  if copy=='candidate':con +=['.state_host_clk(host_clk)','.state_guest_advance(enabled)','.state_freeze(freeze)','.state_guest_held(held)','.state_request(request)','.state_write(write)','.state_index(state_index)','.state_data(state_data)','.state_frozen(frozen)','.state_done(done)','.state_error(error)','.state_result(result)']
  code +=[f'{module} #(.SYSPER(4)) {copy}(',','.join(con),');']
  code.append('assign '+copy+'_outputs={'+','.join(copy+'_'+p['name']for p in reversed(outputs))+'};')
 words=[];offset=0;ram_meta=[];riot_meta=[]
 def add(word,width,original,candidate):
  nonlocal offset
  words.append(dict(word=word,width=width,offset=offset))
  for copy,expr in [('original',original),('candidate',candidate)]:code.append(f'assign {copy}_state[{offset}+:{width}]='+expr+';')
  offset+=width
 def split_fields(fields,row,copy,base):
  lo=row*18;hi=lo+18;parts=[];pos=0
  for f in fields:
   a=max(lo,pos);b=min(hi,pos+f['width']);pos+=f['width']
   if a>=b:continue
   local=a-(pos-f['width']);top=b-(pos-f['width'])-1
   suffix=''if local==0 and top==f['width']-1 else'['+str(local)+']'if local==top else'['+str(top)+':'+str(local)+']'
   parts.append(copy+'.'+base+'.'+f['name']+suffix)
  return '{'+','.join(reversed(parts))+'}'
 for r in m['packages']:
  p=r['package'];base=r['word'];kind=r['type']
  if kind=='FF':
   for w in r['words']:add(base+w['index'],w['width'],split_fields(r['fields'],w['index'],'original',p),split_fields(r['fields'],w['index'],'candidate',p))
  elif kind=='SRAM':
   k=len(ram_meta);start=len(words);g=r['group'];members=sorted(g['members'],key=lambda x:x['bit'],reverse=True)
   for a in range(1024):add(base+a,8,'{'+','.join(f"original.{x['instance']}.mem[{a}]"for x in members)+'}',f'candidate.{p}.memory.mem[{a}]')
   add(base+1024,8,'{'+','.join(f"original.{x['instance']}.dout_r"for x in members)+'}',f'candidate.{p}.memory.logical_q')
   code.append(f'assign original_probe[{8*k}+:8]={{'+','.join(f"original.{x['instance']}.mem[probe_row]"for x in members)+'};')
   code.append(f'assign candidate_probe[{8*k}+:8]=candidate.{p}.memory.mem[probe_row];')
   ref='original.'+members[0]['instance']
   code.append(f'assign ram_address[{10*k}+:10]={{'+','.join(ref+'.p'+str(pin)for pin in gen.ram.ADDRESS)+'};')
   code.append(f'assign ram_data[{8*k}+:8]={{'+','.join('original.'+x['instance']+'.p15'for x in members)+'};')
   code.append(f'assign ram_write[{k}]=!{ref}.p1 && !{ref}.p14;')
   code.append(f'assign ram_pipeline[{8*k}+:8]={{'+','.join('original.'+x['instance']+'.dout_r'for x in members)+'};')
   ram_meta.append(dict(index=r['index'],start_word=start,width=8,words=1024))
  elif kind=='EPROM':add(base,9,f'{{original.{p}.en_q,original.{p}.d}}',f'{{candidate.{p}.en_q,candidate.{p}.logical_d}}')
  elif kind=='CPU':
   for w in r['schema']['words']:add(base+w['index'],w['width'],f"original.{p}.u_cpu.q[{w['lsb']+w['width']-1}:{w['lsb']}]",f"candidate.{p}.u_cpu.q[{w['lsb']+w['width']-1}:{w['lsb']}]")
  else:
   k=len(riot_meta);start=len(words)
   for a in range(128):add(base+a,8,f'original.{p}.u_riot.riot_ram[{a}]',f'candidate.{p}.u_riot.riot_ram[{a}]')
   fields=r['schema']['child_manifest']['fields']
   for row in range(5):
    original=split_fields(fields,row,'original',p+'.u_riot').replace('.u_riot.old_pa7','.u_riot.riot_stuff.old_pa7')
    add(base+128+row,18 if row<4 else 1,original,split_fields(fields,row,'candidate',p+'.u_riot'))
   add(base+133,1,f'original.{p}.phase_d',f'candidate.{p}.phase_d')
   rbase='original.'+p
   code.append(f'assign riot_pins[{k}]={{'+','.join([rbase+'.p39',rbase+'.pb_in',rbase+'.pa_in',rbase+'.p38',rbase+'.p37',rbase+'.p36',rbase+'.d_in',rbase+'.p35','{'+','.join(rbase+'.p'+str(x)for x in(7,6,5,4,3,2,40))+'}',rbase+'.p34',"1'b0"] )+'};')
   riot_meta.append(dict(index=r['index'],start_word=start))
 assert offset==40366,(offset,len(words));assert len(words)==4905
 # Same deterministic initial SRAM image in original, candidate and oracle.
 # No CPU/RIOT/clock state is initialized or modified by this fixture.
 code.append('integer a;initial begin')
 for k,r in enumerate(x for x in m['packages']if x['type']=='SRAM'):
  for x in r['group']['members']:code.append(f"for(a=0;a<1024;a=a+1)original.{x['instance']}.mem[a]=(((a*73+{k*49})&255)>>{x['bit']})&1;")
  code.append(f'for(a=0;a<1024;a=a+1)candidate.{r["package"]}.memory.mem[a]=(a*73+{k*49})&255;')
 code+=['end','endmodule']
 header='#pragma once\n'+f'constexpr unsigned INPUTS={ni},OUTPUTS={no},STATE_BITS=40366,WORDS=4905,INDEX_COUNT=131072;\n'
 pos=0
 for p in inputs:header+=f"constexpr unsigned PIN_{p['name']}={pos};\n";pos+=p['width']
 header+='struct StateWord{unsigned word,width,offset;};\nconstexpr StateWord state_words[]={'+','.join('{'+f"{w['word']},{w['width']},{w['offset']}"+'}'for w in words)+'};\n'
 header+='constexpr unsigned ram_start[]={'+','.join(str(r['start_word'])for r in ram_meta)+'};\nconstexpr unsigned riot_start[]={'+','.join(str(r['start_word'])for r in riot_meta)+'};\n'
 return '\n'.join(code)+'\n',header,dict(ports=ports,state_words=words,rams=ram_meta,riots=riot_meta)


def run(root,out,mutations):
 out.mkdir(parents=True,exist_ok=False)
 (out/'compiler-environment.json').write_text(json.dumps(dict(stack_limit_bytes=resource.getrlimit(resource.RLIMIT_STACK)[0]),indent=2)+'\n')
 board,m,clones=gen.generate(root);top,header,meta=fixture(root,m)
 inputs=out/'executed-inputs';inputs.mkdir()
 for name in ('test_baseboard_full.py','test_baseboard_full.cpp','generate_baseboard_full.py','riot_oracle.hpp'):(inputs/name).write_bytes((HERE/name).read_bytes())
 (out/'manifest.json').write_text(json.dumps(m,indent=2)+'\n');(out/'fixture-metadata.json').write_text(json.dumps(meta,indent=2)+'\n')
 # The existing named riot_stuff block permits read-only observation.
 # Compile all original sources byte-identically, including the board.
 original_board=inputs/'BaseBd_original.v';original_board.write_bytes((root/gen.SOURCE).read_bytes());originals=[original_board]
 for name in m['original_sources']:
  p=inputs/Path(name).name;raw=(root/name).read_bytes()
  p.write_bytes(raw);originals.append(p)
 for p in gen.eprom.images(root).values():
  target=out/'verilog/proms/eprom'/p.name;target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes(p.read_bytes())
 cases=[('candidate',board,clones,None)]
 for name,body,children,why in cases:
  d=out/name;d.mkdir();paths=[]
  for n,s in children.items():
   raw=s if isinstance(s,bytes)else s.encode()
   p=d/n;p.write_bytes(raw);paths.append(p)
  (d/'BaseBd_snapshot.sv').write_text(body);(d/'fixture.sv').write_text(top);(inputs/'ports.h').write_text(header)
  cmd=['verilator','--cc','--exe','--build','-j','4','--top-module','baseboard_full_fixture','-Wno-fatal','-DDORADO_F10415_M10K',
   '--Mdir',str(d/'obj'),str(d/'fixture.sv'),str(d/'BaseBd_snapshot.sv'),*map(str,paths),*map(str,originals),str(inputs/'test_baseboard_full.cpp'),'-CFLAGS','-I'+str(inputs)]
  (d/'command.json').write_text(json.dumps(cmd,indent=2)+'\n')
  with(d/'build.log').open('w')as f:subprocess.run(cmd,check=True,stdout=f,stderr=subprocess.STDOUT,cwd=out)
  p=subprocess.run([str(d/'obj/Vbaseboard_full_fixture')],capture_output=True,text=True,cwd=out,timeout=300);text=p.stdout+p.stderr;(d/'test.txt').write_text(text);print(name+': '+text.strip(),flush=True)
  if why:assert p.returncode and why in text,(name,text)
  else:assert not p.returncode and'FULL BASEBOARD RESTORE PASS'in text,text
 (out/'results.json').write_text(json.dumps(dict(status='PASS',scope='Full board two-state original/model/save/restore; unchanged original RTL, read-only named PA7 scope observation; not a machine/global snapshot',executed_inputs={p.name:hashlib.sha256(p.read_bytes()).hexdigest()for p in inputs.iterdir()},cases=[x[0]for x in cases]),indent=2)+'\n')


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--frozen-root',type=Path,required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--mutations',action='store_true');a=p.parse_args();run(a.frozen_root.resolve(),a.output.resolve(),a.mutations)
