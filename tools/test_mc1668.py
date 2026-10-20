#!/usr/bin/env python3
"""Check MC1668 against Motorola's level-sensitive truth table in both simulators.

Production cell samples at the fabric clock; this checks that sampled truth
table, not native board cadence or analog timing. Closed S=R=1 is undefined.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess

ROOT=Path(__file__).resolve().parents[1]


def run(out):
    out.mkdir(parents=True,exist_ok=False)
    names=['verilog/cells/cell_MC1668.v','verilog/verilator/tb_mc1668_latch.sv',
           'DoradoDocs/datasheets/MC1668.pdf','tools/test_mc1668.py']
    inputs=out/'executed-inputs';hashes={}
    for name in names:
        p=inputs/name;p.parent.mkdir(parents=True,exist_ok=True);raw=(ROOT/name).read_bytes();p.write_bytes(raw)
        hashes[name]=hashlib.sha256(raw).hexdigest()
    source=(inputs/names[0]).read_text();test=inputs/names[1];actions=[]
    def execute(command,log):
        with log.open('w')as stream:
            code=subprocess.run([str(x)for x in command],stdout=stream,stderr=subprocess.STDOUT,cwd=inputs).returncode
        actions.append(dict(command=[str(x)for x in command],returncode=code,log=str(log)))
        return code
    legacy=source.replace('  reg qa, qb;',
        '  reg qa,qb,ca_d=0,cb_d=0;\n  always @(posedge sys_clk) begin ca_d<=p7;cb_d<=p9;end')
    legacy=legacy.replace('if (p7)          qa <= p6;', 'if (p7 && !ca_d) qa <= p6;')
    legacy=legacy.replace('if (p9)          qb <= p11;', 'if (p9 && !cb_d) qb <= p11;')
    variants={'original':source,'edge_only':legacy,
              'closed_clock_transparency':source.replace('if (p7)', 'if (!p7)'),
              'set_dominates_open':source.replace('if (p7)          qa <= p6;', "if (p7)          qa <= p5 ? 1'b1 : p6;"),
              'shared_half_clock':source.replace('if (p9)', 'if (p7)')}
    baseline=None;rows=[]
    for name,raw in variants.items():
        assert name=='original'or raw!=source,name
        case=out/name;case.mkdir();rtl=case/'cell_MC1668.v';rtl.write_text(raw)
        assert execute(['iverilog','-g2012','-s','tb_mc1668_latch','-o',case/'test.vvp',rtl,test],case/'compile.log')==0
        code=execute(['vvp',case/'test.vvp'],case/'test.log');text=(case/'test.log').read_text()
        if name=='original':
            assert code==0 and 'MC1668_LATCH PASS'in text,text
            baseline=text.split('MC1668_LATCH PASS ')[1].splitlines()[0]
        else:assert code and 'MC1668 truth table mismatch'in text,(name,text)
        rows.append(dict(case=name,returncode=code))
    v=out/'verilator';v.mkdir()
    assert execute(['verilator','--binary','--timing','--top-module','tb_mc1668_latch','-Wno-fatal','--Mdir',v/'obj','-o','test',inputs/names[0],test],v/'compile.log')==0
    assert execute([v/'obj/test'],v/'test.log')==0
    assert 'MC1668_LATCH PASS '+baseline in(v/'test.log').read_text()
    result=dict(status='PASS_MC1668_SAMPLED_LATCH_TRUTH_TABLE',inputs=hashes,summary=baseline,
                cases=rows,actions=actions,native_board_qualified=False,scope=__doc__)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n')
    print(result['status'],baseline,'4 bad variants rejected')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    run(a.output.resolve())
