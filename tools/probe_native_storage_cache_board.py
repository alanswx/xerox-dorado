#!/usr/bin/env python3
"""Compare native4/16-line backing buffers against the original144-chip MSA.

Exact qualified9e group/address/arbiter/backend/cells; one original board,
16 groups, all serial/ECC outputs and contents checked. Not full128-group
native firmware, physical fit/timing or measured FPGA instruction speed.
"""
import argparse,gzip,hashlib,json,os,shutil,subprocess,time
from pathlib import Path
from sil_storage_ddram import storage64_frontend
ROOT=Path(__file__).resolve().parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def run(stage,qualification,out):
 assert not out.exists(),'Observe the existing recorded run'
 q=json.loads(qualification.read_text())
 assert q['status']=='PASS_NATIVE_PACK_WINDOW_PREASSEMBLY_REVIEW'
 assert q['frequency_hz']==57142857 and q['sysper']==4
 out.mkdir(parents=True);inputs=out/'inputs';inputs.mkdir()
 names={
  'msa_ddram.v':'src/build/rtl-executive/native16-tx-cable/msa_ddram.v',
  'msa14.v':'src/verilog/generated/msa.v',
  **{n:'src/fpga/mister/'+n for n in ['dorado_storage_group.sv','dorado_storage_arb.sv',
    'dorado_storage_sdram.sv','dorado_storage_sdram_burst.sv','dorado_storage_addr_preview.sv']},
 }
 for n in q['source_hashes']:
  if n.startswith('src/verilog/cells/'):
   names['cells/'+Path(n).name]=n
 for name,relative in names.items():
  source=stage/relative;assert sha(source)==q['source_hashes'][relative],relative
  target=inputs/name;target.parent.mkdir(exist_ok=True);shutil.copyfile(source,target)
 (inputs/'msa.v').write_text(storage64_frontend((inputs/'msa14.v').read_text()))
 for n,source in {
  'original-tb.sv':'verilog/verilator/tb_storage_board.sv',
  'original-fixture.sv':'verilog/verilator/fixtures/xsds_storage.sv',
  'xsds_model.sv':'verilog/verilator/third_party/xsds/xsds_128mbyte_sdram_model.sv',
  'sil_storage_ddram.py':'tools/sil_storage_ddram.py',
  Path(__file__).name:'tools/'+Path(__file__).name,
 }.items():shutil.copyfile(ROOT/source,inputs/n)
 tb=(inputs/'original-tb.sv').read_text()
 replacements={
  '    initial if(external_memory.u_storage_0.DEFER_READS!=DEFER_READS)\n        $fatal(1,"testbench/generator deferred-read options differ");\n':'',
  '    always #10 clk=~clk; //50MHz host, opposite-phase SDRAM clock':'    always #(500000000.0/57142857) clk=~clk; //qualified native host rate',
  '.storage_clk(clk),.storage_reset(reset),':'.storage_clk(clk),.storage_reset(reset),.storage_step(run),',
 }
 for old,new in replacements.items():assert tb.count(old)==1;tb=tb.replace(old,new)
 (inputs/'tb.sv').write_text(tb)
 fixture=(inputs/'original-fixture.sv').read_text()
 old='dorado_storage_sdram #(.ADDR_BITS(ADDR_BITS))'
 assert fixture.count(old)==1
 hashes={str(p.relative_to(inputs)):sha(p) for p in inputs.rglob('*') if p.is_file()}
 provenance=dict(scope=__doc__,qualification_sha256=sha(qualification),
   qualified_inputs={r:q['source_hashes'][r] for r in names.values()},inputs=hashes,
   oracle_power_on='Fixture defines unwritten cells as zero, not a hardware assumption',
   original_chip_count=144,group_count=16,address_bits=16,host_clock_hz=57142857,
   FPGA_or_running_core_changed=False)
 (out/'provenance.json.gz').write_bytes(gzip.compress((json.dumps(provenance,indent=2)+'\n').encode(),mtime=0))
 state=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),actions=[],results={})
 def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
 def execute(command,label):
  with (out/(label+'.log')).open('x')as f:
   p=subprocess.Popen(command,cwd=inputs,stdout=f,stderr=subprocess.STDOUT)
   row=dict(stage=label,pid=p.pid,command=command,started=time.time());state['actions'].append(row);save()
   code=p.wait();row.update(returncode=code,finished=time.time());save();assert code==0,(label,code)
 save()
 try:
  for lines in [4,16]:
   label='lines'+str(lines);obj=out/('obj-'+label)
   f=inputs/(label+'-fixture.sv')
   f.write_text(fixture.replace(old,'dorado_storage_sdram_burst #(.ADDR_BITS(ADDR_BITS),.CLOCK_HZ(57142857),.CACHE_LINES('+str(lines)+'))'))
   state.setdefault('fixture_hashes',{})[f.name]=sha(f);save()
   execute(['verilator','--binary','--timing','--autoflush','-j','4',
    '--top-module','tb_storage_board','-GADDR_BITS=16','-GPREFETCH_NEXT=1','-GINTERLEAVE_GROUPS=1','-DSTORAGE_USE_SDRAM',
    '-Wno-fatal','-Wno-TIMESCALEMOD','-Wno-WIDTHTRUNC','-Wno-WIDTHEXPAND','-Wno-REALCVT',
    '-I'+str(inputs/'cells'),'--Mdir',str(obj),'-o','board-test',
    *[str(inputs/n)for n in ['msa_ddram.v','msa.v','dorado_storage_group.sv','dorado_storage_arb.sv',
    'dorado_storage_sdram.sv','dorado_storage_sdram_burst.sv','dorado_storage_addr_preview.sv','xsds_model.sv','tb.sv',f.name]]],label+'-compile')
   execute([str(obj/'board-test'),'+raw_trace='+str(out/(label+'-raw.csv'))],label+'-test')
   log=(out/(label+'-test.log')).read_text();assert 'STORAGE-BOARD PASS:'in log
   state['results'][label]=next(s for s in log.splitlines()if 'STORAGE-BOARD PASS:'in s)
   print(label,state['results'][label],flush=True);save()
  assert all(sha(inputs/n)==h for n,h in hashes.items())
  state.update(status='PASS_NATIVE_CACHE_CAPACITY_ORIGINAL_MSA_COMPONENT',finished=time.time(),frozen_inputs_unchanged=True);save()
 except BaseException:
  state.update(status='TERMINAL_FAILED',finished=time.time());save();raise
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__)
 for n in ['stage','qualification','output']:p.add_argument('--'+n,type=Path,required=True)
 a=p.parse_args();run(a.stage.resolve(),a.qualification.resolve(),a.output.resolve())
