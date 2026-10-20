#!/usr/bin/env python3
"""Drain pending raw work under FF hold on one actual complete MSA board.

Compare normal traffic/physical state to original frozen RTL, then stop guest
edges with accepted work pending, drain under FF hold and serially read the
entire 1538-bit state without changing it. Backend queues are a test oracle,
not a claim of native arbiter/SDRAM service qualification.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import generate_msa_late_freeze as gen
import test_msa_chipwidth as base
from generate_serial_boards import raw_receipt

HERE=Path(__file__).resolve().parent


CPP=r'''
static bool force_raw_busy=false,force_arb_busy=false;
static unsigned pause_count=0,pause_reads=0,pause_writes=0;
static void ff_same(const State &s){
 const auto now=physical();
 for(unsigned b=0;b<866;++b){unsigned p=canonical_bit[b];
  check(bit(now[p/32],p%32)==bit(s[p/32],p%32),"FF changed while raw work drained");}
}
static void drain_pauses(){
 for(unsigned pass=0;pass<8;++pass){
  bool pending=false;
  for(unsigned n=0;n<4000&&!pending;++n){
   step();
   for(unsigned host=0;host<40&&!pending;++host){
    cycle(false,true);
    for(auto &b:backend)pending|=b.pending&&(b.write==bool(pass&1));
    if(d.storage_ready&&d.ref_storage_ready)break;
   }
  }
  check(pending,"no accepted work at pause");
  for(auto &b:backend)if(b.pending){if(b.write)++pause_writes;else ++pause_reads;}
  const auto ff=physical();const auto before_guest=guest_edges;
  d.request=d.write=0;d.advance=0;d.global_hold=d.freeze=d.held=d.drain_mode=1;
  force_raw_busy=force_arb_busy=true;
  for(unsigned n=0;n<13;++n){cycle(false,true);ff_same(ff);
   check(!d.pause_frontend_hold&&!d.pause_ready&&!d.ready,"freeze before raw/arbiter idle");}
  force_arb_busy=false;
  for(unsigned n=0;n<7;++n){cycle(false,true);ff_same(ff);
   check(!d.pause_frontend_hold&&!d.pause_ready,"freeze before raw service idle");}
  force_arb_busy=true;force_raw_busy=false;
  for(unsigned n=0;n<7;++n){cycle(false,true);ff_same(ff);
   check(!d.pause_frontend_hold&&!d.pause_ready,"freeze before arbiter idle");}
  force_arb_busy=false;
  unsigned wait=0;
  while(!d.pause_frontend_hold){cycle(false,true);ff_same(ff);
   check(++wait<2000,"accepted response deadlocked behind snapshot hold");}
  check(guest_edges==before_guest,"guest advanced during drain");
  for(auto &b:backend)check(!b.pending,"raw operation lost at freeze");
  cycle(false,false);cycle(false,false);
  check(d.pause_ready&&d.ready&&d.frozen&&!d.pause_fault,"late freeze did not acknowledge");
  const auto saved=physical();
  for(unsigned i=0;i<SCAN_WORDS;++i){
   check(access(i,false)==word(saved,i),"late freeze read word differs");
   same(saved,"late freeze read rotation changed state");
   check(d.pause_ready&&d.pause_frontend_hold,"serial rotation released frontend hold");
  }
  d.request=d.write=d.global_hold=d.freeze=d.held=0;
  d.eval();check(d.pause_guest_hold,"guest released before frontend thaw");
  cycle(false,true);d.drain_mode=0;d.eval();compare();
  check(!d.pause_guest_hold&&!d.pause_frontend_hold,"normal pause release retained hold");
  for(unsigned n=0;n<100;++n)step();settle();++pause_count;
 }
 check(pause_reads&&pause_writes,"pending read/write pause coverage missing");
 std::printf("LATE FREEZE DRAIN PASS pauses=%u pending_reads=%u pending_writes=%u\n",pause_count,pause_reads,pause_writes);
}
'''


def fixture(manifest):
    top,header,metadata=base.fixture(manifest)
    top=gen.parent.base.once(top,' input wire clk,ref_clk,advance,',
        ' input wire drain_mode,arbiter_idle,raw_service_idle,\n input wire clk,ref_clk,advance,')
    top=gen.parent.base.once(top,' output wire [17:0] result,',
        ' output wire pause_ready,pause_frontend_hold,pause_guest_hold,pause_fault,\n output wire [17:0] result,')
    top=gen.parent.base.once(top,'.state_global_hold(global_hold),',
        '.state_frontend_hold(drain_mode ? pause_frontend_hold : (global_hold || (freeze && held))),\n.state_global_hold(drain_mode ? pause_guest_hold : global_hold),')
    code='''dorado_memory_pause_barrier pause_barrier(
 .clk(clk),.machine_reseed(system_reset),.owner_hold(drain_mode && global_hold),
 .guest_held(held && !advance),.abort(global_abort),.frontends_drained(drained),
 .arbiter_idle(arbiter_idle),.raw_service_idle(raw_service_idle),
 .frontends_frozen(frozen),.guest_hold(pause_guest_hold),.frontend_hold(pause_frontend_hold),
 .ready(pause_ready),.fault(pause_fault));
'''
    return gen.parent.base.once(top,'endmodule',code+'endmodule'),header,metadata


def run(out,mutations):
    out.mkdir(parents=True,exist_ok=False);inputs=out/'executed-inputs';inputs.mkdir()
    board,manifest,helpers=gen.generate();top,header,metadata=fixture(manifest)
    cpp=raw_receipt(gen.parent.base.ROOT/'fpga/mister/reports/2026-10-19-snapshot-msa-chipwidth',
        'msa-v1/executed-inputs/test_msa_chipwidth.cpp').decode()
    cpp=gen.parent.base.once(cpp,'static void prepare(){',
        'static bool force_raw_busy=false,force_arb_busy=false;\nstatic void prepare(){')
    cpp=gen.parent.base.once(cpp,' d.eval();\n}\nstatic void cycle(',
        ' bool idle=true;for(auto &b:backend)idle&=!b.pending;\n d.arbiter_idle=idle&&!force_arb_busy;d.raw_service_idle=idle&&!force_raw_busy;\n d.eval();\n}\nstatic void cycle(')
    cpp=gen.parent.base.once(cpp,' check(d.storage_ready==d.ref_storage_ready',
        ' if(d.drain_mode&&d.pause_frontend_hold)return;\n check(d.storage_ready==d.ref_storage_ready')
    cpp=gen.parent.base.once(cpp,' const bool shifting=d.scan_step;',
        ' if(d.drain_mode&&d.scan_step)check(d.pause_frontend_hold,"serial rotation released frontend hold");\n const bool shifting=d.scan_step;')
    extra=CPP.replace('static bool force_raw_busy=false,force_arb_busy=false;\n','')
    cpp=gen.parent.base.once(cpp,'int main(int argc,char**argv){',extra+'\nint main(int argc,char**argv){')
    cpp=gen.parent.base.once(cpp,'  const auto saved=reference();',
        '  drain_pauses();\n  const auto saved=reference();')
    cpp=cpp.replace('CHIPWIDTH MSA PASS','LATE FREEZE MSA PASS')
    originals=gen.parent.base.original_cells()
    (inputs/'test_msa_late_freeze.cpp').write_text(cpp)
    (inputs/'msa_ddram.v').write_bytes(gen.parent.base.original(gen.parent.base.SOURCE))
    (inputs/'dorado_storage_group.sv').write_bytes(gen.parent.compact.original.original_source())
    for name,raw in originals.items():(inputs/name).write_bytes(raw)
    for p in (Path(__file__),Path(gen.__file__),HERE/'memory_pause_barrier.sv'):(inputs/p.name).write_bytes(p.read_bytes())
    assets={'msa_board.sv':board,**helpers,'memory_pause_barrier.sv':(HERE/'memory_pause_barrier.sv').read_text()}
    cases=[('candidate',None,None,None)]
    if mutations:cases +=[
        ('early_frontend_hold','msa_board.sv','wire frontend_hold=state_frontend_hold || state_scan_hold_required;',
         'wire frontend_hold=state_hold;'),
        ('ignored_raw_drain','memory_pause_barrier.sv',"(raw_service_idle===1'b1);","1'b1;"),
        ('ignored_arbiter_drain','memory_pause_barrier.sv',"(arbiter_idle===1'b1) && (raw_service_idle===1'b1);","(raw_service_idle===1'b1);"),
        ('nonsticky_frontend_hold','memory_pause_barrier.sv','assign frontend_hold=phase==FROZEN || phase==QUARANTINE;',
         'assign frontend_hold=(phase==FROZEN && frontends_drained) || phase==QUARANTINE;')]
    results=[]
    for label,target,before,after in cases:
        case=out/label;case.mkdir();sources=dict(assets)
        if target:assert sources[target].count(before)==1,(label,before);sources[target]=sources[target].replace(before,after,1)
        for name,source in sources.items():(case/name).write_text(source)
        (case/'fixture.sv').write_text(top);(case/'parameters.h').write_text(header)
        command=['verilator','--cc','--exe','--build','-j','4','--top-module','msa_board_fixture','-Wno-fatal','--assert',
            '--Mdir',str(case/'obj'),'-CFLAGS','-I'+str(case),str(case/'fixture.sv'),*[str(case/n)for n in sources],
            str(inputs/'msa_ddram.v'),str(inputs/'dorado_storage_group.sv'),*[str(inputs/n)for n in originals],str(inputs/'test_msa_late_freeze.cpp')]
        (case/'command.json').write_text(json.dumps(command,indent=2)+'\n')
        with(case/'build.log').open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run([str(case/'obj/Vmsa_board_fixture')],capture_output=True,text=True,timeout=180)
        text=result.stdout+result.stderr;(case/'test.txt').write_text(text);print(label+': '+text.strip(),flush=True)
        if target:assert result.returncode and any(s in text for s in ('ordinary raw controls differ','ordinary physical state differs','freeze before','held raw request/response leak','serial rotation released frontend hold')),text
        else:assert not result.returncode and 'LATE FREEZE DRAIN PASS'in text and 'LATE FREEZE MSA PASS'in text,text
        results.append(dict(case=label,returncode=result.returncode))
    for name,data in [('manifest',manifest),('fixture-metadata',metadata),('results',dict(status='PASS',scope=__doc__,cases=results,
        executed_inputs={p.name:hashlib.sha256(p.read_bytes()).hexdigest()for p in inputs.iterdir()},
        limits='One complete MSA with oracle backend queues; real native arbiter/SDRAM drain, wrapper state access, fit and complete machine snapshot remain pending.'))]:
        (out/(name+'.json')).write_text(json.dumps(data,indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--mutations',action='store_true');a=p.parse_args();run(a.output.resolve(),a.mutations)
