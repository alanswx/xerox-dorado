#!/usr/bin/env python3
"""Freeze a fully checked phase-preserving Hold experiment in the15ns shell.

Require both complete original-graph comparisons and direct RTL equivalence
of all363 atomic outputs. Retain the entire original1400-result fixture/SDC,
single four-bank SRAM owner and proven Quartus17 read-window syntax. This is
an isolated component timing experiment; no native build or image is allowed.
"""
import argparse
import json
from pathlib import Path
import re

import fit_control_clock as fixture
from fit_r_front import quartus_cone
from probe_memory_mir_mask import candidate,bench
from prove_memory_mir_rtl import atomic
from report_r_front_late_hold_fit import TCL
from test_r_front_forward_view import sha

ROOT=Path(__file__).resolve().parents[1]


def gate(run,engine):
    r=json.loads((run/'run-state.json').read_text())
    assert r['status']=='PASS_MEMORY_MIR_MASK_ORIGINAL_COMPARISON'and r['engine']==engine and r['mutation']is None
    assert r['actions'][-1]['returncode']==0 and r['all100_original_clock_counts_match']
    assert r['summary']=='checks=14884 writes=238 reads=6928 epochs=392'
    parent=Path(r['parent']);assert sha(parent/'run-state.json')==r['parent_state_sha256']
    assert (run/'candidate.sv').read_text()==candidate((parent/'candidate.sv').read_text(),None,r['settling_waves'],r['all_hold_clocks'])
    assert (run/'inputs/tb_r_front_local.sv').read_text()==bench((parent/'inputs/tb_r_front_local.sv').read_text())
    for row in r['source_files']:assert sha(run/row['path'])==row['output_sha256']
    for statement in re.findall(r'\$fatal\([^;]+;',(parent/'inputs/tb_r_front_local.sv').read_text()):
        assert statement in(run/'inputs/tb_r_front_local.sv').read_text()
    clocks=re.findall(r'^R_FRONT_CONTROL_CLOCK channel=(\d+) edges=(\d+)$',(run/'test.log').read_text(),re.M)
    assert len(clocks)==100 and clocks==re.findall(r'^R_FRONT_CONTROL_CLOCK channel=(\d+) edges=(\d+)$',(parent/'test.log').read_text(),re.M)
    coverage=dict((k,int(v))for k,v in re.findall(r'(\w+)=(\d+)',r['mask_coverage']))
    assert coverage['high_distinct']>0
    return r,dict(engine=engine,directory=str(run),state_sha256=sha(run/'run-state.json'),
        candidate_sha256=sha(run/'candidate.sv'),summary=r['summary'],coverage=coverage,clocks=clocks)


def prepare(parent,runs,formal,out):
    assert not out.exists()
    checked=[gate(run,engine)for run,engine in zip(runs,['icarus','verilator'])]
    states=[v[0]for v in checked];gates=[v[1]for v in checked]
    assert gates[0]['candidate_sha256']==gates[1]['candidate_sha256']
    assert gates[0]['coverage']==gates[1]['coverage']and gates[0]['clocks']==gates[1]['clocks']
    proof=json.loads((formal/'run-state.json').read_text())
    assert proof['status']=='PASS_ALL_BINARY_CONTROL_MIR_EQUIVALENCE'and proof['returncode']==0 and proof['proof_succeeded']
    assert proof['state_and_input_bits']==955 and proof['compared_output_bits']==363 and not proof['assumptions_added']
    assert states[0]['settling_waves']==states[1]['settling_waves']==proof.get('candidate_atomic_waves',24)
    for name,h in proof['source_files'].items():assert sha(formal/name)==h,name
    assert 'SAT proof finished - no model found: SUCCESS!'in(formal/'proof.log').read_text()
    assert (formal/'candidate.sv').read_text()==atomic((runs[0]/'candidate.sv').read_text(),'candidate_atomic')+'\n'
    assert (formal/'baseline.sv').read_text()==atomic((Path(states[0]['parent'])/'candidate.sv').read_text(),'original_atomic')+'\n'
    fixture_proof=json.loads((parent/'provenance.json').read_text())
    for name,h in fixture_proof['files'].items():assert sha(parent/name)==h,name
    assert (parent/'clock.sdc').read_text()==fixture.SDC
    rtl={};run=runs[0]
    for row in states[0]['source_files']:
        name=row['path'];path=run/name
        if path.name=='candidate.sv':target='dorado_r_front_control_step.sv'
        elif name.startswith(('adapters/','inputs/verilog/rtl/'))or path.name in [
            'dorado_r_front_cone.sv','dorado_r_read_window_step.sv','dorado_r_flag_window_step.sv']:target=path.name
        else:continue
        assert target not in rtl,target
        rtl[target]=quartus_cone(path.read_text())if target=='dorado_r_front_cone.sv'else path.read_text()
    assert len(rtl)==30,len(rtl)
    name='dorado_r_read_window_step.sv';original=rtl[name];lowered=(parent/name).read_text()
    before=' generate for(genvar byte_no=0;';after=' genvar byte_no;\n generate for(byte_no=0;'
    assert original.count(before)==1 and lowered.replace(after,before,1)==original
    syntax=fixture_proof['quartus_genvar_syntax'];assert syntax['inverse_byte_identical']
    assert syntax['lowered_sha256']==sha(parent/name)
    rtl[name]=lowered
    qsf=fixture.QSF.replace('set_global_assignment -name SYSTEMVERILOG_FILE dorado_control_clock_step.sv\n',
        ''.join('set_global_assignment -name SYSTEMVERILOG_FILE '+name+'\n'for name in rtl))
    files=dict(rtl,**{'top.sv':(parent/'top.sv').read_text(),'clock.sdc':fixture.SDC,'clock_fit.qsf':qsf,
                     'clock_fit.qpf':'PROJECT_REVISION = "clock_fit"\n',
                     'audit.tcl':TCL.replace('late-hold-flat-v5-reports','memory-hold-reports')})
    out.mkdir(parents=True)
    for name,text in files.items():(out/name).write_text(text)
    for case in syntax['comparisons']:
        assert all(a['returncode']==0 for a in case['actions'])
        for action in case['actions']:
            log=parent/action['log'];assert sha(log)==fixture_proof['files'][log.name]
            (out/log.name).write_bytes(log.read_bytes())
    for name in [Path(__file__).name,'probe_memory_mir_mask.py','fit_control_clock.py','fit_r_front.py','report_r_front_late_hold_fit.py']:
        (out/name).write_bytes((ROOT/'tools'/name).read_bytes())
    result=dict(status='PREPARED_CHECKED_MEMORY_HOLD_PHASE_FIT',scope=__doc__,component='memory_hold_phase_mask',
        clock_period_ns=15,physical_Q_bits=844,ram_bits=9216,physical_SRAM_banks=4,
        input_staging_bits=1650,launch_bits=1650,result_bits=1400,
        extra_pipeline_registers=0,extra_guest_cycles=0,original_shell_sha256=sha(parent/'top.sv'),
        candidate_settling_waves=states[0]['settling_waves'],all_hold_clocks=states[0]['all_hold_clocks'],
        functional_gates=gates,formal_directory=str(formal),formal_state_sha256=sha(formal/'run-state.json'),
        formal_state_input_bits=955,formal_output_bits=363,formal_added_assumptions=False,
        quartus_genvar_syntax=syntax,files={path.name:sha(path)for path in out.iterdir()if path.is_file()},
        native_qualified=False,physical_timing_qualified=False,speedup_qualified=False,assembly_permitted=False)
    (out/'provenance.json').write_text(json.dumps(result,indent=2)+'\n')
    print(result['status'],flush=True)


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    for name in ['parent','icarus','verilator','formal','output']:parser.add_argument('--'+name,type=Path,required=True)
    args=parser.parse_args();prepare(args.parent.resolve(),[args.icarus.resolve(),args.verilator.resolve()],args.formal.resolve(),args.output.resolve())
