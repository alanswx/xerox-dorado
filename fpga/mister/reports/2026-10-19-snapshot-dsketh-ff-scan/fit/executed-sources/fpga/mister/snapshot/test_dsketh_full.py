#!/usr/bin/env python3
"""Actual full DskEth board state/output/RAM-oracle and destructive restore."""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
import generate_dsketh_full as gen
HERE=Path(__file__).resolve().parent


def fixture(root,m):
 ports=[]
 for x in re.finditer(r'^\s+(input|output)\s+wire\s*(?:\[(\d+):(\d+)\]\s*)?(\w+)(?=[,\n])',(root/gen.SOURCE).read_text(),re.M):
  direction,hi,lo,name=x.groups()
  if name!='sys_clk':ports.append(dict(direction=direction,name=name,width=1 if hi is None else abs(int(hi)-int(lo))+1))
 inputs=[p for p in ports if p['direction']=='input'];outputs=[p for p in ports if p['direction']=='output'];ni=sum(p['width']for p in inputs);no=sum(p['width']for p in outputs);nb=m['logical_state_bits']
 rams=[r for r in m['memories']if r['kind']=='cell_F10145A'];nr=len(rams)
 code=[f'''module dsketh_full_fixture(
 input wire host_clk,run,run_ref,input wire [{ni-1}:0] stimulus,
 input wire freeze,held,request,write,input wire [12:0] state_index,input wire [17:0] state_data,
 input wire [9:0] probe_row,output wire frozen,done,error,output wire [17:0] result,
 output wire [{no-1}:0] original_outputs,candidate_outputs,
 output wire [{nb-1}:0] original_state,candidate_state,
 output wire [{nr*4-1}:0] original_probe,candidate_probe,ram_data,
 output wire [{nr*10-1}:0] ram_address,output wire [{nr-1}:0] ram_write,
 output wire advance,ref_advance);
 reg enabled=0,ref_enabled=0;
 always @(negedge host_clk)begin enabled<=run;ref_enabled<=run_ref;end
 wire guest_clk=host_clk & enabled,ref_clk=host_clk & ref_enabled;
 assign advance=enabled;assign ref_advance=ref_enabled;''']
 out_names={p['name']for p in outputs}
 for copy,module,clock in [('original',m['original_module'],'ref_clk'),('candidate',m['module'],'guest_clk')]:
  for p in outputs:code.append('wire '+(''if p['width']==1 else f"[{p['width']-1}:0] ")+copy+'_'+p['name']+';')
  con=['.sys_clk('+clock+')'];offset=0
  for p in inputs:
   value=f'stimulus[{offset}]'if p['width']==1 else f"stimulus[{offset}+:{p['width']}]"
   if p['name']+'__drv'in out_names:value+=' | '+copy+'_'+p['name']+'__drv'
   con.append(f".{p['name']}({value})");offset+=p['width']
  con +=[f".{p['name']}({copy}_{p['name']})"for p in outputs]
  if copy=='candidate':con +=['.state_host_clk(host_clk)','.state_guest_advance(enabled)','.state_freeze(freeze)','.state_guest_held(held)','.state_request(request)','.state_write(write)','.state_index(state_index)','.state_data(state_data)','.state_frozen(frozen)','.state_done(done)','.state_error(error)','.state_result(result)']
  code +=[f' {module} #(.SYSPER(4)) {copy}(',','.join(con),');']
  code.append('assign '+copy+'_outputs={'+','.join(copy+'_'+p['name']for p in reversed(outputs))+'};')
 words=[];offset=0
 def add(word,width,expression):
  nonlocal offset
  words.append(dict(word=word,width=width,offset=offset))
  for copy in ('original','candidate'):code.append(f'assign {copy}_state[{offset}+:{width}]='+expression(copy)+';')
  offset+=width
 for r in m['ff']:add(r['word'],r['width'],lambda c,r=r:'{'+','.join(c+'.'+r['package']+'.'+f['name']for f in reversed(r['fields']))+'}')
 ram_meta=[]
 for i,r in enumerate(rams):
  start=len(words)
  for a in range(16):add(r['word']|a,4,lambda c,r=r,a=a:f"{c}.{r['package']}.mem[{a}]")
  add(r['word']|16,10,lambda c,r=r:'{'+','.join(c+'.'+r['package']+'.'+f['name']for f in reversed(r['write_window']['fields']))+'}')
  for copy in ('original','candidate'):code.append(f'assign {copy}_probe[{i*4}+:4]={copy}.{r["package"]}.mem[probe_row[3:0]];')
  base='original.'+r['package']+'.'
  code +=[f"assign ram_address[{10*i}+:10]={{6'b0,{base}a_lat}};",f'assign ram_data[{4*i}+:4]={base}d_lat;',f'assign ram_write[{i}]={base}p13 && !{base}we_d && {base}ce_lat;']
  ram_meta.append(dict(package=r['index'],words=16,width=4,start_word=start))
 for r in m['memories']:
  if r['kind']=='cell_F10145A':continue
  # Map physical fields independently onto each transport word, retaining
  # partial signed integer slices and every raw edge/setup history bit.
  fields=[];pos=0
  for f in r['fields']:fields.append(dict(name=f['name'],width=f['width'],lsb=pos));pos+=f['width']
  for w in r['transport_words']:
   lo=18*w['index'];hi=lo+w['width'];parts=[]
   for f in fields:
    a=max(lo,f['lsb']);b=min(hi,f['lsb']+f['width'])
    if b<=a:continue
    suffix=''if a==f['lsb']and b-a==f['width']else '['+str(b-f['lsb']-1)+':'+str(a-f['lsb'])+']'
    parts.append(f['name']+suffix)
   assert parts
   add(r['word']|w['index'],w['width'],lambda c,r=r,parts=parts:'{'+','.join(c+'.'+r['package']+'.'+p for p in reversed(parts))+'}')
 add(m['phase']['word'],9,lambda c:'{'+','.join(c+'.'+f['name']for f in reversed(m['phase']['fields']))+'}')
 assert offset==nb,(offset,nb)
 code.append('integer row;initial begin')
 for i,r in enumerate(rams):
  for copy in ('original','candidate'):code.append(f"for(row=0;row<16;row=row+1){copy}.{r['package']}.mem[row]=(row*7+{i*3+1})&15;")
 code +=['end','endmodule']
 header='#pragma once\n'+f'constexpr unsigned INPUTS={ni},OUTPUTS={no},STATE_BITS={nb},WORDS={len(words)},RAMS={nr},INDEX_COUNT=8192;\n'
 pos=0
 for p in inputs:
  header+=f"constexpr unsigned PIN_{p['name']}={pos};\n";pos+=p['width']
 header+='struct StateWord{unsigned word,width,offset;};\nconstexpr StateWord state_words[]={'+','.join('{'+f"{w['word']},{w['width']},{w['offset']}"+'}'for w in words)+'};\n'
 header+='struct Ram{unsigned package,words,width,start_word;};\nconstexpr Ram rams[]={'+','.join('{'+f"{r['package']},{r['words']},{r['width']},{r['start_word']}"+'}'for r in ram_meta)+'};\n'
 return '\n'.join(code)+'\n',header,dict(ports=ports,state_words=words,rams=ram_meta)


def run(root,out,mutations):
 out.mkdir(parents=True,exist_ok=False);board,m,clones=gen.generate(root);top,header,meta=fixture(root,m)
 for n,x in [('manifest.json',m),('fixture-metadata.json',meta)]:(out/n).write_text(json.dumps(x,indent=2)+'\n')
 for p in [Path(__file__),HERE/'test_dsketh_full.cpp',HERE/'generate_dsketh_full.py',HERE/'generate_receive_monitor.py']:(out/(p.stem+'-executed'+p.suffix)).write_bytes(p.read_bytes())
 cases=[('candidate',board,clones,None)]
 if mutations:
  for name,target,before,after,expected in [
   ('ungated_ff','board','.state_advance(state_guest_advance)',".state_advance(1'b1)",'board state differs'),
   ('ungated_phase','board','else if(state_guest_advance)begin','else begin','board state differs'),
   ('running_access','board','state_hold && state_frozen && state_request','state_request','running request accepted'),
   ('invalid_alias','board',"state_package<8'd117 && state_row==0", "state_package<8'd117 && state_row<2",'invalid index accepted'),
   ('lost_timer_high','cell_SN74123_snapshot.sv','0:t1[17:0]<=state_data','0:t1<=state_data','physical write differs'),
   ('lost_crc_high','cell_F9401_snapshot.sv','cwe_pre<=state_data[0]',"cwe_pre<=1'b0",'physical write differs'),
   ('lost_osc_high','cell_K1115A_snapshot.sv','{osc,acc[31:18]}<=state_data[14:0]',"{osc,acc[31:18]}<=15'b0",'physical write differs'),
   ('lost_small_window','cell_F10145A_snapshot.sv','{ce_lat,d_lat,a_lat,we_d} <= state_data;',"{ce_lat,d_lat,a_lat,we_d} <= 10'b0;",'physical write differs'),
   ('ungated_monitor','dorado_receive_monitor_snapshot.sv','!state_hold && state_advance','!state_hold','board state differs'),
  ]:
   old=board if target=='board'else clones[target];assert before in old,(name,before);changed=old.replace(before,after,1);copies=clones.copy()
   if target!='board':copies[target]=changed
   cases.append((name,changed if target=='board'else board,copies,expected))
 results=[]
 for name,text,copies,expected in cases:
  d=out/name;d.mkdir()
  for n,s in {'board.sv':text,'fixture.sv':top,'ports.h':header,**copies}.items():(d/n).write_text(s)
  cmd=['verilator','--cc','--exe','--build','-j','4','--top-module','dsketh_full_fixture','-Wno-fatal','-DDORADO_F10145_LOGIC','--Mdir',str(d/'obj'),'-CFLAGS','-I'+str(d),str(d/'fixture.sv'),str(d/'board.sv'),*[str(d/n)for n in copies],str(root/gen.SOURCE),*[str(root/n)for n in m['original_sources']],str(HERE/'test_dsketh_full.cpp')]
  (d/'command.json').write_text(json.dumps(cmd,indent=2)+'\n')
  with(d/'build.log').open('w')as log:subprocess.run(cmd,cwd=root,stdout=log,stderr=subprocess.STDOUT,check=True)
  p=subprocess.run([str(d/'obj/Vdsketh_full_fixture')],cwd=root,capture_output=True,text=True,timeout=180);text=p.stdout+p.stderr;(d/'test.txt').write_text(text);print(name+': '+text.strip(),flush=True)
  if expected:assert p.returncode and expected in text,(name,text)
  else:assert p.returncode==0 and'FULL DSKETH RESTORE PASS'in text,text
  results.append(dict(case=name,returncode=p.returncode,expected_failure=expected))
 paths=[Path(__file__),HERE/'test_dsketh_full.cpp',HERE/'generate_dsketh_full.py',HERE/'generate_receive_monitor.py',*[HERE/n for n in ('generate_contb_full.py','generate_snapshot_ff.py','generate_processor_ff.py','generate_dsketh_ff.py','generate_jk.py','generate_f10145.py','generate_sn74123.py','generate_crc9401.py','generate_oscillator.py')],root/gen.SOURCE,*[root/n for n in m['original_sources']],*[root/p['file']for p in m['immutable_proms']]]
 (out/'results.json').write_text(json.dumps(dict(status='PASS',scope=m['scope'],cases=results,source_hashes={str(p.resolve()):hashlib.sha256(p.read_bytes()).hexdigest()for p in paths}),indent=2)+'\n')

if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--source-root',type=Path,required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--mutations',action='store_true');a=p.parse_args();run(a.source_root.resolve(),a.output.resolve(),a.mutations)
