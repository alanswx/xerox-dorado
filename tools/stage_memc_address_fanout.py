#!/usr/bin/env python3
"""Stage physical replication of the measured MemC cache-address latch fanout.

The full native fit's worst path starts at an Aad latch driving322 loads,
through asynchronous cache tags/conditional Hold into the stack write path.
Limit only the sixteen original address-latch fanouts, preserving all RTL,
capture edges, SRAM ports, clocks and timing constraints. This prepares a
diagnostic fit, not an accepted native selection or deployment.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re

ROOT=Path(__file__).resolve().parents[1]
MEMC='src/fpga/mister/snapshot_native_memc/MemC_full_snapshot.sv'


def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def prepare(parent,fit,out,project,physical_duplication=False):
    assert not out.exists()and project.is_absolute()
    original=json.loads((parent/'r-front-control-native.json').read_text())
    p=json.loads((fit/'r-front-control-native.json').read_text())
    assert p==original and p['frequency_hz']==57142857 and p['sysper']==4
    assert p['balanced_task_priority_proven']and p['owned_caller_slice15ns_passed']
    assert not p['assembly_permitted']
    s=json.loads((fit/'run-state.json').read_text())
    assert s['status']=='TERMINAL_TIMING_OBSERVED_REVIEW_REQUIRED'and all(a['returncode']==0 for a in s['stages'])
    assert json.loads((fit/'physical-report/run-state.json').read_text())['status']=='TERMINAL_PHYSICAL_NATIVE_REPORTS_REVIEW_REQUIRED'
    for name,digest in p['source_hashes'].items():
        assert sha(parent/name)==sha(fit/name)==digest,name
    report=fit/'core/native59-setup-7_slow_1100mv_100c.rpt'
    raw=report.read_text();assert 'Worst case slack is -1.314'in raw
    first=raw[raw.index('Path #1:'):raw.index('Path #2:')]
    assert 'cell_MC1668_snapshot:u_a15|qa'in first and 'stk_h_rtl_0'in first
    assert re.search(r';\s*322\s*;[^\n]*u_a15\|qa\|q\s*;',first)
    memc=(parent/MEMC).read_text();assignments=[]
    packages={'a14':('Aad_2_p_a','Aad_3_p_a'),'a15':('Aad_4a','Aad_5a'),
              'b14':('Aad_0_p_a','Aad_0_p_b'),'b15':('Aad_6a','Aad_7a'),
              'c14':('Aad_1_p_a','Aad_1_p_b'),'c15':('Aad_4b','Aad_5b'),
              'd14':('Aad_2_p_b','Aad_3_p_b'),'d15':('Aad_6b','Aad_7b')}
    for package,outputs in packages.items():
        body=re.search(r'cell_MC1668_snapshot u_'+package+r' \(.*?\n  \);',memc,re.S)[0]
        pins=dict(re.findall(r'\.p(\d+)\((\w+)\)',body))
        for field,pin,net in [('qa','2',outputs[0]),('qb','15',outputs[1])]:
            assert pins[pin]==net,(package,pin,pins[pin])
            target=f'emu|u_native|m|b_MemC|u_{package}|{field}'
            assignments.extend([f'set_instance_assignment -name MAX_FANOUT 32 -to "{target}"',
                                f'set_instance_assignment -name IGNORE_MAX_FANOUT_ASSIGNMENTS OFF -to "{target}"'])
    policy='\n# Physical copies of the existing sixteen Aad owners; no new logical stage.\n'+'\n'.join(assignments)+'\n'
    out.mkdir(parents=True);data={}
    for name in p['source_hashes']:
        raw=(parent/name).read_bytes()
        if Path(name).suffix in ['.qsf','.qpf','.qip','.tcl','.sdc']:
            raw=raw.replace(p['project'].encode(),str(project).encode())
        if name=='core/Dorado.qsf':
            if physical_duplication:
                old=b'set_global_assignment -name PHYSICAL_SYNTHESIS_REGISTER_DUPLICATION OFF'
                assert raw.count(old)==1
                raw=raw.replace(old,old[:-3]+b'ON',1)
            raw+=policy.encode()
        data[name]=raw;target=out/name;target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes(raw)
    changed=[name for name in data if data[name].replace(str(project).encode(),p['project'].encode())!=(parent/name).read_bytes()]
    assert changed==['core/Dorado.qsf']
    for name in ['core/Dorado.sdc','core/sdram_timing.sdc']:
        assert data[name]==(parent/name).read_bytes().replace(p['project'].encode(),str(project).encode())
    record={**original,'project':str(project),'parent_stage':str(parent),
            'parent_manifest_sha256':sha(parent/'r-front-control-native.json'),
            'scope':__doc__,'memc_address_fanout_experiment':dict(limit=32,source_registers=16,assignments=assignments,
                physical_register_duplication_enabled=physical_duplication,
                failing_fit=str(fit),failing_path_sha256=sha(report),old_setup_ns=-1.314,old_source_fanout=322),
            'source_hashes':{name:sha(out/name)for name in data},
            'only_QSF_physical_assignments_changed_after_relocation':True,
            'HDL_or_clock_changes':False,'physical_timing_qualified':False,
            'speedup_qualified':False,'assembly_permitted':False,'hardware_changed':False}
    (out/'r-front-control-native.json').write_text(json.dumps(record,indent=2)+'\n')
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    print('PREPARED_NATIVE_MEMC_ADDRESS_FANOUT_DIAGNOSTIC',len(data),'source files',flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for name in ['parent','fit','output','project']:p.add_argument('--'+name,type=Path,required=True)
    p.add_argument('--physical-duplication',action='store_true',
                   help='Allow the fitter to duplicate existing registers, without adding a logical stage')
    a=p.parse_args();prepare(a.parent.resolve(),a.fit.resolve(),a.output.resolve(),a.project,a.physical_duplication)
