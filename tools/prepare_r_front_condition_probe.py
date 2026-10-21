#!/usr/bin/env python3
"""Add passive branch-condition observations to a terminal original-ROM fixture.

All existing executed inputs remain byte-identical except the testbench's
appended readout. No CPU inputs, clocks, ROMs, runtime limits or guest writes
change. The bounded Initial/map window is not a Cedar desktop qualification.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path
import shutil


def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()


MONITOR='''
  // Observe retained conditions as well as raw ALU outputs after settling.
  always @(negedge sys_clk)
    if(boot_diag_settled_from>=0 && n_cyc2>=boot_diag_settled_from &&
       n_cyc2<boot_diag_settled_from+6000)
      $display("BOOT_DIAG_CONDITION sample=%0d CIA=%h task=%h T=%h A=%h B=%h ALU=%h Flags=%b%b%b%b HFlagClock=%b HTClock=%b LTClock=%b Root=%b",
        n_cyc2,ring_cia,pct_ctask,brc_t,brc_alua,brc_alub,brc_alu,
        m.ResEqZero_p_,m.ResLtZero_p_,m.Overflow_p_,m.ALUCarry,
        m.b_ProcH.Clock1_p_Cb,m.b_ProcH.SHCP_p_C,m.b_ProcL.SHCP_p_C,m.CLK_ca_p_);
'''


def prepare(parent,out):
    assert not out.exists()
    provenance=parent/'provenance.json.gz'
    record=json.loads(gzip.decompress(provenance.read_bytes()))
    state=json.loads((parent/'run-state.json').read_text())
    assert state['status']!='RUNNING' and all(a['returncode']==0 for a in state['actions'])
    assert record['probe_only']
    for n,h in record['inputs'].items():assert sha(parent/'executed-inputs'/n)==h,n
    out.mkdir(parents=True);inputs=out/'executed-inputs'
    # Freeze only declared inputs, excluding outputs left by the parent run.
    for name in record['inputs']:
        p=inputs/name;p.parent.mkdir(parents=True,exist_ok=True)
        shutil.copy2(parent/'executed-inputs'/name,p)
    name='verilog/verilator/tb_exec.sv';bench=inputs/name;text=bench.read_text()
    assert text.count('endmodule')==1 and 'BOOT_DIAG_CONDITION 'not in text
    bench.write_text(text.replace('endmodule',MONITOR+'\nendmodule'))
    assert bench.read_text().replace(MONITOR+'\n','')==text
    shutil.copy2(parent/'cycle-limit.txt',out/'cycle-limit.txt')
    record['commands']=[[str(arg).replace(str(parent),str(out))for arg in command]for command in record['commands']]
    # The bench runs in its own executed-inputs cwd; its private +frame output
    # cannot touch the protected repository Executive artifact.
    record.update(condition_probe_parent=str(parent),condition_probe_parent_provenance_sha256=sha(provenance),
                  condition_probe_only=True,condition_probe_changed_inputs=[name])
    record['inputs']={str(p.relative_to(inputs)):sha(p)for p in inputs.rglob('*')if p.is_file()}
    changed={n for n,h in json.loads(gzip.decompress(provenance.read_bytes()))['inputs'].items()
             if record['inputs'][n]!=h}
    assert changed=={name}
    (out/'provenance.json.gz').write_bytes(gzip.compress((json.dumps(record,indent=2)+'\n').encode(),mtime=0))
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    print('PREPARED_PASSIVE_CONDITION_PROBE',out,len(record['inputs']),'inputs')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--parent',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();prepare(a.parent.resolve(),a.output.resolve())
