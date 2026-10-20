#!/usr/bin/env python3
"""Stage Native59 with only the MC1668 normal-operation correction.

Preserve all 498 baseline files and compiled-input identities except the
generic cell and its actually instantiated copy. Keep the existing inactive
state interface/layout; its two obsolete clock-history bits have no influence
on normal operation. No clock/config/Main/network/disk changes.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import stage_native_speed as baseline

ROOT=Path(__file__).resolve().parents[1]
CELL='src/verilog/cells/cell_MC1668.v'
ADAPTER='src/fpga/mister/snapshot_native_processor/cell_MC1668_snapshot.v'


def sha(raw):return hashlib.sha256(raw).hexdigest()


def adapter(original,production):
    # Reuse the exact qualified production processes, not independently
    # transcribed equations. Retain the inactive interface's historical bits.
    assert production.count('always @(posedge sys_clk)')==2
    assert production.count('  reg qa, qb;')==1
    normal=production.split('  reg qa, qb;',1)[1].split('  assign p2',1)[0]
    assert normal.count('always @(posedge sys_clk)')==2
    assert not re.search(r'cka_en|ckb_en|cka_d|ckb_d',normal)
    normal=normal.replace('always @(posedge sys_clk)','')
    prefix='    end else if (state_advance) begin\n'
    suffix='\n    end\n  end\nendmodule'
    assert original.count(prefix)==original.count(suffix)==1
    before,rest=original.split(prefix);_,after=rest.split(suffix)
    result=before+prefix+'      cka_d<=p7; ckb_d<=p9; // inactive-interface compatibility only\n'+normal+suffix+after
    result=production.split('`default_nettype none',1)[0]+'''// Native59 adapter: state administration remains inactive in this candidate.
// Compatibility clock-history slots are retained but never select data capture.
'''+ '`default_nettype none'+result.split('`default_nettype none',1)[1]
    assert result.count('else if (state_advance)')==1
    assert 'if (cka_en)'not in result and 'if (ckb_en)'not in result
    return result


def prepare(out,project):
    files,compiled=baseline.baseline(baseline.REPORTS)
    production=(ROOT/'verilog/cells/cell_MC1668.v').read_bytes()
    fixed=adapter(files[ADAPTER].decode(),production.decode()).encode()
    changes={CELL:production,ADAPTER:fixed}
    assert set(changes)<=set(compiled)
    baseline.stage(out,baseline.REPORTS,project)
    for name,raw in changes.items():(out/name).write_bytes(raw)
    actual={name:sha((out/name).read_bytes())for name in files}
    changed=[name for name,raw in files.items()if (out/name).read_bytes()!=baseline.relocated(name,raw,project)]
    assert set(changed)==set(changes)
    boards=['src/fpga/mister/snapshot_native_processor/ProcH_full_snapshot.sv',
            'src/fpga/mister/snapshot_native_processor/ProcL_full_snapshot.sv',
            'src/fpga/mister/snapshot_native_memc/MemC_full_snapshot.sv']
    uses=sum(len(re.findall(r'\bcell_MC1668_snapshot\s+u_',files[n].decode()))for n in boards)
    assert uses==12
    record=dict(status='STAGED_MC1668_NATIVE59_CANDIDATE',project=str(project),
        baseline_compiled_inputs=compiled,source_hashes=actual,baseline_files=len(files),
        intended_deltas={n:dict(before=sha(files[n]),after=sha(changes[n]))for n in changes},
        physical_instances=uses,state_admin='INACTIVE',frequency_mhz=50,sysper=4,
        hardware_qualified=False,speedup_claimed=False,scope=__doc__)
    (out/'mc1668-candidate.json').write_text(json.dumps(record,indent=2)+'\n')
    (out/'stage_mc1668_native.py').write_bytes(Path(__file__).read_bytes())
    (out/'stage_native_speed.py').write_bytes((ROOT/'tools/stage_native_speed.py').read_bytes())
    print(record['status'],'12 instances; two source deltas; unchanged50MHz/SYSPER4')
    return record


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--project',type=Path,required=True);a=p.parse_args()
    assert a.project.is_absolute();prepare(a.output.resolve(),a.project)
