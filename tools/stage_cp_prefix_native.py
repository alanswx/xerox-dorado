#!/usr/bin/env python3
"""Wire the tested guarded control component into a private native baseline.

Keep57.142857MHz/SYSPER4 for the first whole-machine pause/firmware comparison.
Control readiness gates the same falling-edge-latched machine enable as disk
and storage; preserve storage readiness as a separate signal. Host SDRAM,
network and input service continue. This is source staging only, not native
timing qualification, assembly permission, speed selection or the60ns target.
"""
import argparse
import gzip
import json
from pathlib import Path
import re

from fit_cp_prefix_cache import gate
from prove_cp_prefix_cache import module
from test_r_front_forward_view import sha
from test_r_front_task_event import replace

ROOT=Path(__file__).resolve().parents[1]
BP='src/build/rtl-executive/native16-tx-cable/dorado_native_backplane.v'
MACHINE='src/build/rtl-executive/native16-tx-cable/dorado_native_machine.sv'


def prepare(parent,fit,review,input_run,out,project):
    assert not out.exists()and project.is_absolute()
    old=json.loads((parent/'r-front-control-native.json').read_text())
    assert old['status']=='STAGED_PROCESSOR_CONTROL_NATIVE_EXPERIMENT'
    assert old['frequency_hz']==57142857 and old['sysper']==4 and not old['assembly_permitted']
    for name,h in old['source_hashes'].items():assert sha(parent/name)==h,name
    receipt=json.loads((review/'results.json').read_text())
    assert receipt['status']=='REVIEWED_DIRECT_CP_GUARDED_COMPONENT'
    assert receipt['direct_cp_projection']and receipt['settling_waves']==5
    assert len(receipt['negative_controls'])==8 and len(receipt['formal_proofs'])==5
    assert all(c['owners']['slack_ns']>=0 for c in receipt['fit_corners'])
    retained_fit=review/fit.name
    for name in ['provenance.json','run-state.json']:
        assert sha(retained_fit/name)==sha(fit/name),name
    gates=[]
    for row in receipt['functional_gates']:
        state,checked=gate(Path(row['directory']),row['engine'])
        checked=json.loads(json.dumps(checked))
        assert checked==row
        gates.append(checked)
    f=json.loads((fit/'provenance.json').read_text())
    assert f['direct_cp_projection']and f['settling_waves']==5 and f['unbounded_induction_proven']
    assert f['functional_gates']==gates and f['clock_period_ns']==15 and f['result_bits']==1401
    for name,h in f['files'].items():
        raw=(fit/name).read_bytes()
        if name=='clock_fit.qsf':raw=re.sub(rb'\nset_global_assignment -name LAST_QUARTUS_VERSION "17\.0\.2 Lite Edition"\n?$',b'',raw)
        import hashlib
        assert hashlib.sha256(raw).hexdigest()==h,name
    run=json.loads(gzip.decompress((input_run/'provenance.json.gz').read_bytes()))
    proof=Path(run['input_reset_proof']);synth=Path(run['input_reset_synthesis'])
    p=json.loads((proof/'run-state.json').read_text());s=json.loads((synth/'run-state.json').read_text())
    assert sha(proof/'run-state.json')==run['input_reset_proof_sha256']
    assert sha(synth/'run-state.json')==run['input_reset_synthesis_sha256']
    assert p['status']=='PASS_STOPPED_INPUT_RESET_WITH_NEGATIVE_CONTROLS'and len(p['cases'])==16
    assert s['status']=='PASS_INPUT_ADAPTER_SYNTHESIS'and s['returncode']==0
    synthesis_inputs=json.loads((synth/'inputs.json').read_text())
    adapters={}
    for name in ['dorado_keyboard.v','dorado_terminal.v']:
        key='src/verilog/cells/'+name;source=input_run/'inputs'/key
        assert sha(source)==run['inputs'][key]==p['inputs'][name]==synthesis_inputs[name]
        assert sha(source)==sha(proof/'inputs'/name)==sha(synth/name)
        adapters[key]=source.read_bytes()

    data={}
    for name in old['source_hashes']:
        raw=(parent/name).read_bytes()
        if Path(name).suffix in {'.qsf','.qpf','.qip','.tcl','.sdc'}:
            raw=raw.replace(old['project'].encode(),str(project).encode())
        data[name]=raw
    rtl=re.findall(r'^set_global_assignment -name SYSTEMVERILOG_FILE (\S+)$',(fit/'clock_fit.qsf').read_text(),re.M)
    rtl=[n for n in rtl if n!='top.sv']
    assert len(rtl)==30
    assert {'src/verilog/rtl/'+n for n in rtl}==set(old['extra_RTL_sources'])
    for name in rtl:data['src/verilog/rtl/'+name]=(fit/name).read_bytes()
    data.update(adapters)

    raw=data[BP].decode();before=module(raw,'dorado_native_backplane')
    body=replace(before,'    output wire storage_ready,','    output wire storage_ready,\n    output wire control_ready,')
    body=replace(body,'.physical_state(r_front_q));',
        '.physical_state(r_front_q),.cp_reset(storage_reset),.cp_ready(control_ready));')
    assert raw.count(before)==1
    data[BP]=raw.replace(before,body,1).encode()
    raw=data[MACHINE].decode()
    raw=replace(raw,'    output wire storage_ready,storage_initialized,',
        '    output wire storage_ready,storage_initialized,\n    output wire control_ready,')
    raw=replace(raw,'    .storage_ready(board_ready),','    .storage_ready(board_ready),.control_ready(control_ready),')
    data[MACHINE]=raw.encode()
    core=data['core/Dorado.sv'].decode()
    core=replace(core,'wire storage_ready,storage_initialized;',
        'wire storage_ready,storage_initialized;\nwire control_ready;')
    core=replace(core,'wire machine_enable = ~reset && pack_ready && storage_ready;',
        'wire machine_enable = ~reset && pack_ready && storage_ready && control_ready;')
    core=replace(core,'    .contb_advance(network_step),',
        '    .contb_advance(network_step),.control_ready(control_ready),')
    assert 'always @(negedge clk_sys) network_step <= machine_enable;'in core
    assert '.ena(machine_enable)'in core
    data['core/Dorado.sv']=core.encode()
    for name in ['core/Dorado.sdc','core/sdram_timing.sdc']:
        # Only project path relocation is permitted in existing constraints.
        assert data[name]==(parent/name).read_bytes().replace(old['project'].encode(),str(project).encode())
    out.mkdir(parents=True)
    for name,raw in data.items():
        target=out/name;target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes(raw)
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    changes=[dict(path=name,parent_sha256=old['source_hashes'].get(name),output_sha256=sha(out/name))
             for name in data if old['source_hashes'].get(name)!=sha(out/name)]
    result={**old,'status':'STAGED_PROCESSOR_CONTROL_NATIVE_EXPERIMENT','scope':__doc__,
        'project':str(project),'parent_stage':str(parent),'parent_manifest_sha256':sha(parent/'r-front-control-native.json'),
        'component_review':str(review),'component_review_sha256':sha(review/'results.json'),
        'component_fit':str(fit),'component_full15ns_passed':receipt['component_fit_passed'],'component_owner15ns_passed':True,
        'component_proof_gates':gates,'original_native_pause_clock_rate_unchanged':True,
        'global_control_ready_gate_wired':True,'storage_ready_kept_separate':True,
        'canonical_runtime_guard_implemented':True,'first_step_fallback_implemented':True,
        'direct_CP_register_projection_proven':True,'component_settling_waves':5,
        'input_reset_source_run':str(input_run),'input_reset_provenance_sha256':sha(input_run/'provenance.json.gz'),
        'input_adapter_proof_sha256':sha(proof/'run-state.json'),'input_adapter_synthesis_sha256':sha(synth/'run-state.json'),
        'source_hashes':{name:sha(out/name)for name in data},'changes':changes,
        'native_pause_consumers_qualified':False,'source_pause_contract_qualified':False,
        'frequency_hz':57142857,'sysper':4,'ordinary_host_ticks':8,'original60ns_cadence_qualified':False,
        'native_qualified':False,'physical_timing_qualified':False,'speedup_qualified':False,
        'assembly_permitted':False,'firmware_qualified':False,'hardware_changed':False,'full_goal_active':True}
    (out/'r-front-control-native.json').write_text(json.dumps(result,indent=2)+'\n')
    print('STAGED_GUARDED_CONTROL_NATIVE_BASELINE_ONLY',len(data),'source files',flush=True)


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    for name in ['parent','fit','review','input-run','output','project']:parser.add_argument('--'+name,type=Path,required=True)
    a=parser.parse_args();prepare(a.parent.resolve(),a.fit.resolve(),a.review.resolve(),a.input_run.resolve(),a.output.resolve(),a.project)
