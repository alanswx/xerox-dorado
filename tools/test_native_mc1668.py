#!/usr/bin/env python3
"""Qualify the MC1668 copy actually used by all twelve Native59 positions.

Normal operation only: the existing state-admin inputs are tied inactive.
Check the manufacturer table and paused machine advances in both simulators.
This is not full-core firmware, timing or enabled snapshot qualification.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import stage_native_speed as baseline
from stage_mc1668_native import ADAPTER,CELL

ROOT=Path(__file__).resolve().parents[1]


def run(candidate,out):
    record=json.loads((candidate/'mc1668-candidate.json').read_text())
    assert record['state_admin']=='INACTIVE'and record['physical_instances']==12
    for name,digest in record['source_hashes'].items():assert hashlib.sha256((candidate/name).read_bytes()).hexdigest()==digest,name
    original,_=baseline.baseline(baseline.REPORTS)
    out.mkdir(parents=True,exist_ok=False);inputs=out/'executed-inputs';inputs.mkdir()
    source=(candidate/ADAPTER).read_text()
    test=(ROOT/'verilog/verilator/tb_mc1668_latch.sv').read_text()
    test=test.replace('reg sys_clk=0;','reg sys_clk=0,advance=1;\n integer paused_checks=0;')
    test=test.replace('cell_MC1668 dut(.sys_clk(sys_clk),','''cell_MC1668_snapshot dut(.sys_clk(sys_clk),.state_host_clk(sys_clk),
   .state_advance(advance),.state_hold(1'b0),.state_write(1'b0),.state_data(18'b0),''')
    needle='   a=next_a;b=next_b;'
    assert test.count(needle)==1
    test=test.replace(needle,needle+'''
   if(checks>0) begin
     advance=0;
     repeat(3) begin
       @(posedge sys_clk);#1;
       if({qb,qa}!=={expected_b,expected_a}) $fatal(1,"native MC1668 changed while machine advance paused");
       paused_checks=paused_checks+1;
     end
     @(negedge sys_clk);advance=1;
   end''')
    test=test.replace('   $display("MC1668_LATCH PASS', '''   if(paused_checks<1000)$fatal(1,"native MC1668 pause coverage missing");
   $display("NATIVE_MC1668_PAUSED checks=%0d",paused_checks);
   $display("MC1668_LATCH PASS''')
    (inputs/'tb_mc1668_latch.sv').write_text(test)
    (inputs/'cell_MC1668_snapshot.v').write_text(source)
    (inputs/'cell_MC1668.v').write_bytes((candidate/CELL).read_bytes())
    (inputs/'legacy_MC1668_snapshot.v').write_bytes(original[ADAPTER])
    (inputs/'test_native_mc1668.py').write_bytes(Path(__file__).read_bytes())
    actions=[]
    def execute(command,log):
        with log.open('w')as stream:
            code=subprocess.run([str(x)for x in command],cwd=inputs,stdout=stream,stderr=subprocess.STDOUT).returncode
        actions.append(dict(command=[str(x)for x in command],returncode=code,log=str(log)));return code
    variants={'original':source,'legacy_edge_only':original[ADAPTER].decode(),
              'ignore_machine_pause':source.replace('end else if (state_advance) begin','end else begin'),
              'shared_half_clock':source.replace('if (p9)', 'if (p7)')}
    summary=None
    for name,raw in variants.items():
        case=out/name;case.mkdir();rtl=case/'cell_MC1668_snapshot.v';rtl.write_text(raw)
        assert execute(['iverilog','-g2012','-s','tb_mc1668_latch','-o',case/'test.vvp',rtl,inputs/'tb_mc1668_latch.sv'],case/'compile.log')==0
        code=execute(['vvp',case/'test.vvp'],case/'test.log');text=(case/'test.log').read_text()
        if name=='original':
            assert code==0 and 'MC1668_LATCH PASS'in text,text
            summary=[line for line in text.splitlines()if 'PASS'in line or 'PAUSED'in line]
        else:assert code and ('MC1668 truth table mismatch'in text or 'changed while machine advance paused'in text),(name,text)
    v=out/'verilator';v.mkdir()
    assert execute(['verilator','--binary','--timing','--top-module','tb_mc1668_latch','-Wno-fatal','--Mdir',v/'obj','-o','test',inputs/'cell_MC1668_snapshot.v',inputs/'tb_mc1668_latch.sv'],v/'compile.log')==0
    assert execute([v/'obj/test'],v/'test.log')==0
    assert all(line in(v/'test.log').read_text()for line in summary)
    result=dict(status='PASS_NATIVE_MC1668_NORMAL_OPERATION',summary=summary,
        inputs={p.name:hashlib.sha256(p.read_bytes()).hexdigest()for p in inputs.iterdir()},
        candidate=record['intended_deltas'],actions=actions,physical_instances=12,
        state_admin='INACTIVE',firmware_qualified=False,scope=__doc__)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n')
    print(result['status'],summary,'3 defective variants rejected')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--candidate',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True);a=p.parse_args();run(a.candidate.resolve(),a.output.resolve())
