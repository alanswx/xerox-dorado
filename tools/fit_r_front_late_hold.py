#!/usr/bin/env python3
"""Budget late Hold selection with the same 15 ns / 1400-result shell.

Require both complete all-owner original-reference comparisons and an
independent-original rejection of an inverted output selection. Forecasts
have no RAM or sequential storage. Keep every result and the original timing
constraints; report the late input->owner cone separately from other results.
This is an isolated component budget, not a native fit/phase/speed permit.
"""
import argparse
import json
from pathlib import Path
import re

import fit_control_clock as fixture
from fit_r_front import quartus_cone
from probe_r_front_late_hold import candidate,bench,pure_sources
from report_r_front_late_hold_fit import TCL as LATE_HOLD_REPORTS
from test_r_front_forward_view import sha

ROOT=Path(__file__).resolve().parents[1]


def gate(d,engine):
    r=json.loads((d/'run-state.json').read_text());assert r['engine']==engine
    assert r['status']=='PASS_LATE_HOLD_COFACTOR' and not r['mutation'] and r['all_owner_bits_selected']
    assert r['actions'][-1]['returncode']==0 and r['summary']=='checks=14884 writes=238 reads=6928 epochs=392'
    assert r['forecasts_sequential_storage_bits']==r['forecasts_RAM_instances']==r['extra_guest_cycles']==0
    assert r['physical_SRAM_banks']==4
    source=Path(r['parent']);assert sha(source/'run-state.json')==r['parent_state_sha256']
    assert (d/'candidate.sv').read_text()==candidate((source/'candidate.sv').read_text(),False,True)
    assert (d/'inputs/tb_r_front_local.sv').read_text()==bench((source/'inputs/tb_r_front_local.sv').read_text())
    assert (d/'late_hold_forecasts.sv').read_text()==pure_sources(source)
    assert sha(d/'late_hold_forecasts.sv')==r['pure_forecasts_sha256']
    for row in r['source_files']:assert sha(d/row['path'])==row['output_sha256']
    assert sum(row['changed']for row in r['source_files'])==2
    old=(source/'inputs/tb_r_front_local.sv').read_text();new=(d/'inputs/tb_r_front_local.sv').read_text()
    for statement in re.findall(r'\$fatal\([^;]+;',old):assert statement in new,statement
    clocks=re.findall(r'^R_FRONT_CONTROL_CLOCK channel=(\d+) edges=(\d+)$',(d/'test.log').read_text(),re.M)
    assert len(clocks)==100
    assert clocks==re.findall(r'^R_FRONT_CONTROL_CLOCK channel=(\d+) edges=(\d+)$',(source/'test.log').read_text(),re.M)
    coverage=dict(re.findall(r'(\w+)=(\w+)',r['cofactor_coverage']))
    assert int(coverage['checks'])==29768 and int(coverage['state_distinct'])>0 and coverage['seen']=='11'
    return r,dict(engine=engine,directory=str(d),state_sha256=sha(d/'run-state.json'),
                  candidate_sha256=sha(d/'candidate.sv'),pure_forecasts_sha256=sha(d/'late_hold_forecasts.sv'),
                  summary=r['summary'],coverage=coverage,clocks=clocks)


def prepare(parent,runs,negative,out):
    assert not out.exists() and len(runs)==2
    selected=[];sources=[]
    for engine,d in zip(['icarus','verilator'],runs):
        r,g=gate(d,engine);selected.append(g);sources.append(r)
    assert selected[0]['candidate_sha256']==selected[1]['candidate_sha256']
    assert selected[0]['pure_forecasts_sha256']==selected[1]['pure_forecasts_sha256']
    assert selected[0]['coverage']==selected[1]['coverage'] and selected[0]['clocks']==selected[1]['clocks']
    neg=json.loads((negative/'run-state.json').read_text());assert neg['mutation'] and neg['all_owner_bits_selected']
    assert neg['status']=='LATE_HOLD_COFACTOR_FAILED' and neg['actions'][0]['returncode']==0 and neg['actions'][-1]['returncode']!=0
    nlog=(negative/'test.log').read_text()
    assert 'R front state mismatch'in nlog,'Require the independent original-state rejection, not only the forecast self-check'
    old=json.loads((parent/'provenance.json').read_text());assert old['component']=='r_front_control_physical_forward'
    for n,h in old['files'].items():assert sha(parent/n)==h,n
    assert (parent/'clock.sdc').read_text()==fixture.SDC
    rtl={};d=runs[0]
    for row in sources[0]['source_files']:
        n=row['path'];p=d/n
        if p.name=='candidate.sv':name='dorado_r_front_control_step.sv'
        elif n.startswith(('adapters/','inputs/verilog/rtl/')) or p.name in [
             'dorado_r_front_cone.sv','dorado_r_read_window_step.sv','dorado_r_flag_window_step.sv']:name=p.name
        else:continue
        assert name not in rtl,name
        rtl[name]=quartus_cone(p.read_text())if name=='dorado_r_front_cone.sv'else p.read_text()
    rtl['late_hold_forecasts.sv']=(d/'late_hold_forecasts.sv').read_text()
    assert len(rtl)==31,len(rtl)
    # Reuse the exact previously proved Quartus17 generate syntax. The old
    # parser rejects an inline genvar declaration even in a .sv source.
    name='dorado_r_read_window_step.sv';original=rtl[name];lowered=(parent/name).read_text()
    before=' generate for(genvar byte_no=0;';after=' genvar byte_no;\n generate for(byte_no=0;'
    assert original.count(before)==1 and lowered.count(after)==1
    assert lowered.replace(after,before,1)==original
    syntax=old['quartus_genvar_syntax']
    assert syntax['inverse_byte_identical'] and len(syntax['comparisons'])==2
    import hashlib
    assert syntax['original_sha256']==hashlib.sha256(original.encode()).hexdigest()
    assert syntax['lowered_sha256']==sha(parent/name)
    for proof in syntax['comparisons']:
        assert all(a['returncode']==0 for a in proof['actions'])
        log=parent/proof['actions'][-1]['log']
        assert sha(log)==old['files'][log.name]
        assert 'R_READ_WINDOW PASS checks=2304 closing_halves=1024 unclocked_updates=768 closed_changes=768'in log.read_text()
    rtl[name]=lowered
    out.mkdir(parents=True)
    qsf=fixture.QSF.replace('set_global_assignment -name SYSTEMVERILOG_FILE dorado_control_clock_step.sv\n',
        ''.join('set_global_assignment -name SYSTEMVERILOG_FILE '+n+'\n'for n in rtl))
    audit=LATE_HOLD_REPORTS.replace('late-hold-flat-v5-reports','late-hold-reports')
    files=dict(rtl,**{'top.sv':(parent/'top.sv').read_text(),'clock.sdc':fixture.SDC,'clock_fit.qsf':qsf,
                     'clock_fit.qpf':'PROJECT_REVISION = "clock_fit"\n','audit.tcl':audit})
    for n,raw in files.items():(out/n).write_text(raw)
    for proof in syntax['comparisons']:
        for action in proof['actions']:
            (out/action['log']).write_bytes((parent/action['log']).read_bytes())
    (out/'read-window-syntax-proof.json').write_text(json.dumps(syntax,indent=2)+'\n')
    for n in ['fit_control_clock.py','fit_r_front.py','probe_r_front_late_hold.py',Path(__file__).name]:
        (out/n).write_bytes((ROOT/'tools'/n).read_bytes())
    r=dict(scope=__doc__,component='r_front_late_hold_cofactor',clock_period_ns=15,
           physical_Q_bits=844,ram_bits=9216,input_staging_bits=1650,launch_bits=1650,result_bits=1400,
           extra_pipeline_registers=0,extra_guest_cycles=0,forecasts_RAM_instances=0,physical_SRAM_banks=4,
           original_input_output_shell_unchanged=sha(out/'top.sv')==sha(parent/'top.sv'),
           quartus_genvar_syntax=syntax,
           functional_gates=selected,negative_directory=str(negative),negative_state_sha256=sha(negative/'run-state.json'),
           files={p.name:sha(p)for p in out.iterdir()if p.is_file()},
           assembly_permitted=False,native_qualified=False,physical_timing_qualified=False,speedup_qualified=False)
    (out/'provenance.json').write_text(json.dumps(r,indent=2)+'\n')
    print('PREPARED_LATE_HOLD_COMPONENT_15NS_BUDGET',flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for name in ['parent','icarus','verilator','negative','output']:p.add_argument('--'+name,type=Path,required=True)
    a=p.parse_args();prepare(a.parent.resolve(),[a.icarus.resolve(),a.verilator.resolve()],a.negative.resolve(),a.output.resolve())
