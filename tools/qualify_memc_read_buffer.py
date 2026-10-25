#!/usr/bin/env python3
"""Qualify a tagged read register on the actual native MemC tag SRAM cell.

Keep all original memory/write/admin state and transfers byte-identical. Only
guest read outputs use the auxiliary register; the caller must wait for its
address match. Administrative reads retain the original asynchronous port.
No native integration, original60ns schedule or timing pass follows alone.
"""
import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1]
CELL='src/fpga/mister/snapshot_native_processor/cell_MB7071H_snapshot.sv'
RTL='verilog/rtl/dorado_memc_read_buffer.sv'
EXTRA='''  // Auxiliary read pipeline only. The original RAM is the sole owner.
  reg read_valid=1'b0;
  reg [7:0] read_address_q;
  reg [3:0] read_data_q;
  always @(posedge state_host_clk) begin
    read_valid<=1'b1;
    read_address_q<=state_address;
    // Own write on this edge becomes visible after this edge, like the RAM.
    read_data_q<=(state_restore || state_guest_write) ? state_write_data:state_read;
  end
  assign read_ready=state_hold || (read_valid && read_address_q==state_address);
'''


def transform(raw):
    original=raw
    a='module cell_MB7071H_snapshot (';b='module dorado_memc_read_buffer ('
    c='output wire p23// b_OUT';d='output wire p23,output wire read_ready// b_OUT; auxiliary address match'
    e='  wire [3:0] q = state_read;';f=EXTRA+'  wire [3:0] q = state_hold ? state_read:read_data_q;'
    for old,new in [(a,b),(c,d),(e,f)]:
        assert raw.count(old)==1,old
        raw=raw.replace(old,new,1)
    assert raw.replace(f,e,1).replace(d,c,1).replace(b,a,1)==original
    return raw


def harness(raw):
    # Yosys rejects the original combined initialized ANSI output declaration.
    # Lower to identical explicit initial assignments in the proof only.
    for old,new in [("output reg state_frozen = 1'b0,",'output reg state_frozen,'),
                    ("output reg state_done = 1'b0, state_error = 1'b0,",'output reg state_done, state_error,')]:
        assert raw.count(old)==1;raw=raw.replace(old,new,1)
    raw=raw.replace('  wire [7:0] a   =',"  initial begin state_frozen=0;state_done=0;state_error=0;end\n  wire [7:0] a   =",1)
    a='output wire p23,output wire read_ready// b_OUT; auxiliary address match'
    b='output wire p23,output wire read_ready,output wire safe_data// b_OUT; proof only'
    assert raw.count(a)==1
    checks='''
  // Read-only proof observer at the saved tag: not another production port.
  assign safe_data=!read_ready || ({p20,p21,p22,p23}==(sel ? state_read:4'b0));
  always @*begin
    if(read_valid)assert(read_data_q==mem[read_address_q]);
    assert(safe_data);
  end
'''
    return raw.replace(a,b,1).replace('endmodule',checks+'endmodule',1)


def run(stage,out,yosys):
    assert not out.exists();out.mkdir(parents=True)
    m=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,m['source_hashes'])
    original=(stage/CELL).read_text();candidate=(ROOT/RTL).read_text()
    assert candidate==transform(original)
    (out/'original.sv').write_text(original);(out/'candidate.sv').write_text(candidate)
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    state=dict(status='RUNNING',started=time.time(),scope=__doc__,source=str(stage),
        stage_manifest_sha256=sha(stage/'r-front-control-native.json'),source_cell_sha256=sha(stage/CELL),
        candidate_sha256=sha(ROOT/RTL),initial_RAM_bits_unconstrained=1024,
        original_RAM_and_all_admin_write_updates_unchanged=True,assumptions=0,
        files={q.name:sha(q)for q in out.iterdir()},actions=[])
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    mutations=[('contracts',None),
        ('missing-write-forward',('(state_restore || state_guest_write) ? state_write_data:state_read','state_read')),
        ('ignore-address-tag',('read_address_q==state_address',"1'b1")),
        ('wrong-guest-data',('state_hold ? state_read:read_data_q','state_hold ? state_read:(read_data_q ^ 4\'h1)'))]
    save()
    for label,mutation in mutations:
        d=out/label;d.mkdir();actual=candidate
        if mutation:
            a,b=mutation;assert actual.count(a)==1;actual=actual.replace(a,b,1)
        (d/'cell.sv').write_text(harness(actual))
        positive=mutation is None
        sat='sat -timeout 120 -seq 3 -tempinduct -verify -prove-asserts'if positive else'sat -timeout 120 -seq 5 -prove safe_data 1 -falsify -show-ports -dump_json /work/counterexample.json'
        (d/'proof.ys').write_text('''read_verilog -formal -sv /work/cell.sv
hierarchy -check -top dorado_memc_read_buffer
proc
flatten
async2sync
chformal -lower
memory_map
opt
check -assert
'''+sat+'\n')
        env=os.environ.copy();env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache');env['YOWASP_MOUNT']='/work='+str(d)
        cmd=[str(yosys),'-Q','-T','-s','/work/proof.ys']
        with(d/'proof.log').open('x')as log:
            p=subprocess.Popen(cmd,stdout=log,stderr=log,env=env)
            a=dict(label=label,positive=positive,pid=p.pid,started=time.time(),command=cmd,files={q.name:sha(q)for q in d.iterdir()if q.name!='proof.log'})
            state['actions'].append(a);save();code=p.wait()
        marker='Induction step proven: SUCCESS!'if positive else'model found: FAIL!'
        a.update(returncode=code,passed=code==0 and marker in(d/'proof.log').read_text(),finished=time.time(),log_sha256=sha(d/'proof.log'));save()
        print(label,a['passed'],flush=True)
        if not a['passed']:break
    passed=len(state['actions'])==4 and all(a['passed']for a in state['actions'])
    state.update(status='PASS_ACTUAL_MEMC_TAGGED_SRAM_READ'if passed else'TERMINAL_MEMC_TAGGED_READ_NOT_ESTABLISHED',finished=time.time(),
        native_or_firmware_or_speed_qualified=False,original60ns_schedule_qualified=False)
    save();print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['stage','output','yosys']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.stage.resolve(),a.output.resolve(),a.yosys.resolve()))
