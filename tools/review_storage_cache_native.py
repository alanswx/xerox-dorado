#!/usr/bin/env python3
"""Review native16-line backing-buffer source, fit and passive hit observation.

Require regenerated503-source recipe, all332 inherited timing categories,
unchanged RAM behavior, exact expanded buffer state and24 counter crossings.
This is preassembly review; actual firmware and pinned performance are separate.
"""
import argparse,contextlib,hashlib,io,json,re,tempfile
from collections import Counter
from pathlib import Path
from stage_storage_cache_native import stage,WRAPPER
from review_sdram_burst_native import memory_comparison,readers
from review_pack_window_native import sample_paths
from audit_native_conta import ucp
ROOT=Path(__file__).resolve().parents[1]
def sha(b):return hashlib.sha256(b).hexdigest()
def canonical(names):return {re.sub(r'~DUPLICATE(?:_\d+)?$','',n)for n in names}
def review(root,recipe,counter):
 evidence={}
 def data(n):
  b=(root/n).read_bytes();evidence[n]=sha(b);return b
 def text(n):return data(n).decode('latin1')
 def js(n):return json.loads(data(n))
 r=js('storage-cache-native.json');args=json.loads(recipe.read_text());args={k:Path(v).resolve()for k,v in args.items()}
 assert set(args)=={'parent','qualification','capacity','controls','board'}
 assert r['frequency_hz']==57142857 and r['sysper']==4 and r['state_admin']=='INACTIVE'
 assert r['raw_buffer_lines']==16 and r['raw_buffer_bytes']==256 and r['modified_RTL_sources']==[WRAPPER]
 assert not r['assembly_permitted']and not r['firmware_qualified']and not r['hardware_speedup_qualified']
 actual={n:sha(data(n))for n in r['source_hashes']};assert actual==r['source_hashes']and len(actual)==503
 assert data('stage_storage_cache_native.py')==(ROOT/'tools/stage_storage_cache_native.py').read_bytes()
 with tempfile.TemporaryDirectory(prefix='dorado-cache-review-')as tmp:
  target=Path(tmp)/'stage'
  with contextlib.redirect_stdout(io.StringIO()):stage(**args,out=target,project=Path(r['project']))
  assert json.loads((target/'storage-cache-native.json').read_text())==r,'Recipe differs'
  assert {n:sha((target/n).read_bytes())for n in actual}==actual
 parent=args['parent'];q=json.loads(args['qualification'].read_text())
 build=js('run-state.json');reports=js('storage-ready-reports-v1/run-state.json');collection=js('collected-ready-input-check.json')
 assert build['status']=='TERMINAL_TIMING_OBSERVED_REVIEW_REQUIRED'
 assert [s['stage']for s in build['stages']]==['map','fit','sta','clocks','corners','control-crossings','contb-crossings','adapter-crossings','storage-ready']
 assert reports['status']=='TERMINAL_STORAGE_READY_REPORTS_REVIEW_REQUIRED'
 assert [s['stage']for s in reports['actions']]==['ready','backend','inventory']
 assert all(s['returncode']==0 for s in build['stages']+reports['actions'])
 pids={build['builder_pid'],reports['driver_pid']}|{s['pid']for s in build['stages']+reports['actions']}
 assert set(collection['absent_pids'])==pids and collection['actual']==actual
 assert sha(data('build_r_front_ready_native.py'))==build['builder_sha256']
 for n,h in reports['report_inputs'].items():assert sha(data('storage-ready-reports-v1/'+n))==h
 mapping=text('core/output_files/Dorado.map.rpt');fitting=text('core/output_files/Dorado.fit.rpt')
 memory={}
 for label,n,fit in [('mapping','Dorado.map.rpt',False),('fitting','Dorado.fit.rpt',True)]:
  memory[label]=memory_comparison((parent/'core/output_files'/n).read_text(encoding='latin1'),text('core/output_files/'+n),fit,True)
 memory['cache']=readers.native_cache_physical(fitting,js('src/fpga/mister/snapshot_native_memd/assets.json')['manifests']['MemD'],'independent40')
 summary=re.findall(r'Type\s*:\s*([^\n]+)\nSlack\s*:\s*([-+\d.]+)',text('core/output_files/Dorado.sta.summary'))
 assert len(summary)==31 and all(float(v)>0 for _,v in summary)
 paths=[];coverage={};clock='emu|pll|pll_inst|altera_pll_i|general[0].gpll~PLL_OUTPUT_COUNTER|divclk'
 names=[x['report'].removesuffix('.gz')for x in q['detailed_paths']];assert len(names)==len(set(names))==332
 for n in names:
  body=text(n);paths.append(readers.timed(body,n))
  if n.startswith('core/storage-sample-'):
   parts=n.split('-');sample_paths(body,parts[2],parts[3],clock)
 for corner in sorted(readers.CORNERS):
  n='core/native59-ucp-'+corner+'.rpt';coverage[corner]=ucp(text(n));assert coverage[corner]==ucp((parent/n).read_text())
 clocks=text('core/native-clocks.rpt')
 for name in [clock,'dorado_sdram_clk']:
  rows=[s for s in clocks.splitlines()if s.startswith('; '+name+' ')]
  assert len(rows)==1 and re.search(r'; Generated\s*; 17\.500\s*;',rows[0])
 for key,value,count in [('HOST_CLOCK_HZ',57142857,1),('HOST_TIMEOUT_CLOCKS',228571428,1),('HOST_ADDR_SAMPLE',1,128),('DIRTY_BLOCK_WINDOW',1,1),('SNAPSHOT_PAUSE',0,1),('CACHE_LINES',16,1)]:
  found=re.findall(r';\s*'+key+r'\s*;\s*(\d+)\s*;',mapping);assert len(found)==count and all(int(v)==value for v in found),key
 command={n:a+b+c for n,a,b,c in re.findall(r'; command\.(\w+)\s*; ([01])\s*; ([01])\s*; ([01])\s*;',mapping)}
 assert command==q['command_encoding']
 used=set();table=mapping[mapping.index('; Analysis & Synthesis Source Files Read'):].split('\n\n',1)[0]
 for line in table.splitlines():
  f=[s.strip()for s in line.split(';')[1:-1]]
  if len(f)==5 and f[1]=='yes':used.add(f[3].replace(r['project']+'/',''))
 assert WRAPPER in used
 ready=text('storage-ready-reports-v1/ready.log');prior_ready=(parent/'storage-ready-reports-v1/ready.log').read_text()
 for label in ['STORAGE_SAMPLE_REGISTER','NATIVE_READY_REGISTER','NATIVE_STEP_REGISTER']:
  assert Counter(re.findall(r'^'+label+r' (.+)$',ready,re.M))==Counter(re.findall(r'^'+label+r' (.+)$',prior_ready,re.M)),label
 counts=re.findall(r'^STORAGE_SAMPLE_CORNER (\S+) (\d+) (\d+)$',ready,re.M)
 assert len(counts)==4 and{c for c,_,_ in counts}==readers.CORNERS and all(int(n)==2048 and int(m)>=2048 for _,n,m in counts)
 backend=text('storage-ready-reports-v1/backend.log');old_backend=(parent/'storage-ready-reports-v1/backend.log').read_text()
 physical=canonical(re.findall(r'^BURST_REGISTER (.+)$',backend,re.M));old_physical=canonical(re.findall(r'^BURST_REGISTER (.+)$',old_backend,re.M))
 buffer={};scope='emu:emu|dorado_native_machine:u_native|dorado_storage_sdram_burst:u_sdram|'
 for label in ['line_words','line_tag']:
  expr=re.compile(re.escape(scope+label)+r'\[(\d+)\]\[(\d+)\]$')
  old_bits={int(m[2])for n in old_physical if(m:=expr.fullmatch(n))and int(m[1])==0}
  expected={scope+label+f'[{line}][{bit}]'for line in range(16)for bit in old_bits}
  found={n for n in physical if expr.fullmatch(n)}
  assert found==expected and len(old_bits)==(96 if label=='line_words'else 20),label
  buffer[label]=dict(bits=len(found),bits_per_line=len(old_bits))
  for bit in old_bits:
   if label=='line_words':assert bit%16<12 and 0<=bit<128
  if label=='line_tag':assert old_bits==set(range(20))
 valid={n for n in physical if re.fullmatch(re.escape(scope)+r'line_valid\[\d+\]',n)}
 assert valid=={scope+f'line_valid[{i}]'for i in range(16)}
 for label in ['trace','observer','selftest','response']:
  inv=text('storage-ready-reports-v1/inventory.log');old_inv=(parent/'storage-ready-reports-v1/inventory.log').read_text()
  pattern=r'^SELFTEST_FIT_REGISTER '+label+r' (.+)$';assert canonical(re.findall(pattern,inv,re.M))==canonical(re.findall(pattern,old_inv,re.M)),label
 warnings={}
 def normalized(row):
  code,message=row;message=re.sub(r'/home/alans/mister/Dorado_\w+_202610\d+','PROJECT',message)
  message=re.sub(r'(at [^ ()]+)\(\d+\)',r'\1(LINE)',message)
  return code,re.sub(r' Line: \d+',' Line: LINE',message)
 for n in ['map.log','fit.log','sta.log']:
  body=text(n);assert 'Critical Warning'not in body and not re.search(r'^Error',body,re.M)
  before=Counter(map(normalized,re.findall(r'^Warning \((\d+)\): (.+)$',(parent/n).read_text(),re.M)))
  after=Counter(map(normalized,re.findall(r'^Warning \((\d+)\): (.+)$',body,re.M)))
  removed,added=before-after,after-before
  if n=='map.log':
   template='Verilog HDL assignment warning at dorado_storage_sdram_burst.sv(LINE): truncated value with size 32 to match size of target ({width}) File: PROJECT/src/fpga/mister/dorado_storage_sdram_burst.sv Line: LINE'
   assert removed==Counter({('10230',template.format(width=2)):1})and added==Counter({('10230',template.format(width=4)):1})
   src=text('src/fpga/mister/dorado_storage_sdram_burst.sv')
   # The unchanged FIFO wrap expression is promoted by its integer zero.
   # Values are0 or fill_line+1 below CACHE_LINES, fitting the new4-bit index.
   assert "next_line<=fill_line==CACHE_LINES-1 ? 0 : fill_line+1'b1;"in src and 'CACHE_LINES>16'in src
  else:assert not removed and not added,n
  warnings[n]=dict(removed=list(removed.items()),added=list(added.items()))
 for n in ['clocks','corners','control-crossings','contb-crossings','adapter-crossings','storage-ready','storage-ready-reports-v1/ready']:
  assert not re.search(r'^(?:Critical )?Warning|^Error',text(n+'.log'),re.M),n
 for n,count in [('backend',4),('inventory',1)]:
  found=re.findall(r'^Warning \((\d+)\): (.+)$',text('storage-ready-reports-v1/'+n+'.log'),re.M)
  assert len(found)==count and all(c=='332174'and'burst_observation* could not be matched with a register'in m for c,m in found)
 fit_summary=text('core/output_files/Dorado.fit.summary');assert'Device : 5CSEBA6U23I7'in fit_summary and'Timing Models : Final'in fit_summary
 resources={}
 for label,key in [('Logic utilization (in ALMs)','alms'),('Total registers','registers'),('Total RAM Blocks','ram_blocks'),('Total DSP Blocks','dsp_blocks')]:resources[key]=int(re.search(re.escape(label)+r'\s*:\s*([\d,]+)',fit_summary)[1].replace(',',''))
 assert resources['ram_blocks']==332 and resources['dsp_blocks']==35
 # This follow-up is a separate read-only query, not a second fit.
 counter_state=json.loads((counter/'run-state.json').read_text());assert counter_state['status']=='TERMINAL_AUDIT_COMPLETED'and counter_state['returncode']==0
 assert counter_state['source_manifest_sha256']==sha(data('storage-cache-native.json'))
 assert sha((counter/'audit.tcl').read_bytes())==counter_state['script_sha256']==sha((ROOT/'tools/audit_storage_cache_native.tcl').read_bytes())
 log=(counter/'audit.log').read_text();assert not re.search(r'^(?:Critical )?Warning|^Error',log,re.M)
 hit_counts=re.findall(r'^CACHE_HIT_COUNTER_COUNT (\S+) (\d+)$',log,re.M);assert len(hit_counts)==4 and{c for c,_ in hit_counts}==readers.CORNERS
 wanted={f'emu:emu|dorado_speed_monitor:u_speed_monitor|causes.count[11][{i}]'for i in range(32)}
 for corner in sorted(readers.CORNERS):
  found=re.findall(r'^CACHE_HIT_COUNTER_REGISTER '+re.escape(corner)+r' (.+)$',log,re.M);assert canonical(found)==wanted
  for label in ['counter','event','initialized']:
   for mode in ['setup','hold']:
    n=f'cache-hit-{label}-{mode}-{corner}.rpt';paths.append(readers.timed((counter/n).read_text(),n))
 counter_evidence={n.name:sha(n.read_bytes())for n in counter.iterdir()if n.is_file()}
 assert len(paths)==356
 return dict(status='PASS_NATIVE_STORAGE_CACHE_PREASSEMBLY_REVIEW',scope=__doc__,source_hashes=actual,reviewed_evidence_hashes=evidence,counter_evidence_hashes=counter_evidence,recipe_inputs={k:str(v)for k,v in args.items()},recipe_config_sha256=sha(recipe.read_bytes()),frequency_hz=57142857,sysper=4,state_admin='INACTIVE',raw_buffer_lines=16,timing_checks=summary,detailed_paths=paths,coverage=coverage,memory=memory,buffer_registers=buffer,hit_counter_bits=32,warnings=warnings,resources=resources,command_encoding=command,minimum_setup_ns=min(x['slack_ns']for x in paths if'-setup-'in x['report']),minimum_hold_ns=min(x['slack_ns']for x in paths if'-hold-'in x['report']),assembly_permitted=False,firmware_qualified=False,hardware_speedup_qualified=False)
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__)
 for n in ['root','recipe','counter','output']:p.add_argument('--'+n,type=Path,required=True)
 a=p.parse_args();result=review(a.root.resolve(),a.recipe.resolve(),a.counter.resolve());assert not a.output.exists();a.output.write_text(json.dumps(result,indent=2)+'\n');print(result['status'],len(result['detailed_paths']),result['minimum_setup_ns'],result['minimum_hold_ns'])
