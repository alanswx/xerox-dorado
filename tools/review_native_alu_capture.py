#!/usr/bin/env python3
"""Review native-facing ALU capture, exact lowering and passive native wiring.

Keep the original native owners/enables authoritative. The new source trace is
a fixed-limit diagnostic; running is not a firmware or capture-contract pass.
"""
import argparse
import gzip
import json
from pathlib import Path
import re
import shutil
import subprocess
import time

from review_baseboard_clock_consumer import verify
from review_processor_branch_composition import checked
from review_r_front_capture_followup import append_archive
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1];BUILD=ROOT/'build/rtl-executive'
TOOLS=[Path(__file__).name,'lower_processor_alu_projection.py','test_alu_projection_lowering.py',
       'stage_native_alu_capture.py','lint_r_front_control_native.py','probe_native_alu_root_dependency.py']


def retain(source,dest):
    for path in source.rglob('*'):
        rel=path.relative_to(source)
        if not path.is_file()or any(part=='obj'or part.startswith('obj_')for part in rel.parts):continue
        if path.suffix in ['.o','.a','.vvp','.rbf','.sof','.pack','.gch']or path.name in ['test','layout','symbols']:continue
        raw=path.read_bytes();assert not raw.startswith((b'\x7fELF',b'\xcf\xfa\xed\xfe')),path
        target=dest/rel;target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes(raw)


def review(out):
    assert not out.exists();out.mkdir(parents=True)
    good=[BUILD/f'native-alu-capture-{e}-v1-20261025'for e in ['icarus','verilator']]
    summaries=[]
    for d in good:
        s=checked(d)
        assert s['status']=='PASS_ALU_PROJECTION_NATIVE_LOWERING'and s['returncode']==0
        assert s['native_flag_capture_interface']and s['original_reference_and_checks_unchanged']
        assert s['source_root_and_derived_flag_clock_separate']
        assert s['input_bits']==1632 and s['output_bits']==2287
        assert 'cases=1024 input_bits=1632 output_bits=2287'in(d/'miter-test.log').read_text()
        log=(d/'replay-test.log').read_text()
        summary=re.findall(r'^ORIGINAL_ALU_COMPOSITION_PASS .*$',log,re.M);assert len(summary)==1
        assert 'events=15470 states=1198 RAM=9216 clocks=147 observations=964'in summary[0]
        summaries.append(summary[0])
        parent=Path(s['source'])
        assert sha(d/'replay/raw-inputs.hex')==sha(parent/'raw-inputs.hex')
        assert sha(d/'replay/initial.hex')==sha(parent/'initial.hex')
        assert sha(d/'replay/selectors.hex')==sha(parent/'selectors.hex')
        assert sha(d/'replay/branch-step.sv')==sha(parent/'branch-step.sv')
        assert sha(d/'replay/dorado_processor_flag_capture.sv')==sha(ROOT/'verilog/rtl/dorado_processor_flag_capture.sv')
        tb=(d/'replay/tb.sv').read_text()
        assert '.physical_before({projection_q[1197:924],tq,projection_q[843:0]})'in tb
        assert '.source_clock_before(projection_root)'in tb and'.flag_clock_before(before_root)'in tb
        retain(d,out/d.name)
    assert summaries[0]==summaries[1]
    # Both variants compare against the identical raw circuit. The original
    # constructor assigned export numbers in hash-dependent tie order; retain
    # both tested artifacts. Future generation sorts equal-length names too.
    assert sha(Path(checked(good[0])['source'])/'projection/projection.sv')==sha(Path(checked(good[1])['source'])/'projection/projection.sv')
    stable=[BUILD/f'native-alu-capture-stable-{e}-v1-20261025'for e in ['icarus','verilator']]
    for d in stable:
        s=checked(d)
        assert s['status']=='PASS_ALU_PROJECTION_NATIVE_LOWERING'and not s.get('mutation')
        assert 'cases=1024 input_bits=1632 output_bits=2287'in(d/'miter-test.log').read_text()
        assert re.findall(r'^ORIGINAL_ALU_COMPOSITION_PASS .*$',(d/'replay-test.log').read_text(),re.M)==[summaries[0]]
        assert sha(d/'replay/branch-step.sv')==sha(ROOT/'verilog/rtl/dorado_branch_condition_frame_step.sv')
        assert "capture.q'"not in(d/'replay-compile.log').read_text()
        retain(d,out/d.name)
    assert sha(stable[0]/'lowered/projection.sv')==sha(stable[1]/'lowered/projection.sv')
    trial=BUILD/'native-alu-late-root-negative-icarus-v1-20261025';t=checked(trial)
    assert t['mutation']=='late-source-root'and t['returncode']==0
    assert re.findall(r'^ORIGINAL_ALU_COMPOSITION_PASS .*$',(trial/'replay-test.log').read_text(),re.M)==[summaries[0]]
    retain(trial,out/trial.name)
    proof=BUILD/'native-alu-root-dependency-v1-20261025';pstate=checked(proof)
    assert pstate['status']=='PROVED_EARLY_FLAG_DATA_SOURCE_ROOT_INDEPENDENT'
    assert pstate['input_bits']==1632 and pstate['compared_data_bits']==16 and pstate['assumptions']==0
    assert 'no model found: SUCCESS!'in(proof/'check.log').read_text()
    retain(proof,out/proof.name)
    stage=BUILD/'native-alu-observation-stage-v3-20261025'
    native=BUILD/'native-cp-read-j19-boot-v1-20261025'
    r=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,r['source_hashes'])
    assert len(r['source_hashes'])==578 and r['frequency_hz']==57142857 and r['sysper']==4
    assert r['passive_actual_native_ALU_capture_connected']and not r['new_flag_owner_selected']
    assert not r['assembly_permitted']and not r['architectural_state_clock_or_enable_changed']
    p=json.loads(gzip.decompress((native/'provenance.json.gz').read_bytes()));verify(native/'inputs',p['inputs'])
    for name in ['ProcH','ProcL']:
        path=f'src/fpga/mister/snapshot_native_processor/{name}_full_snapshot.sv'
        text=(stage/path).read_text()
        header='    output wire [1197:844] alu_physical,\n    output wire [431:401] alu_inputs,output wire [37:0] alu_flag_pins,\n'
        assert text.count(header)==1;text=text.replace(header,'',1)
        text=re.sub(r'^    \.(?:physical_memory|p\d+)\((?:alu_memory_\w+|alu_physical\[\d+\])\),\n','',text,flags=re.M)
        text=''.join(line for line in text.splitlines(keepends=True)if not re.match(
            r' (?:wire \[63:0\] alu_memory_|assign alu_(?:physical|inputs|flag_pins))',line))
        # Remove only the extra separator inserted before the original end.
        old=(native/'inputs'/path).read_text()
        assert text.replace('\n\nendmodule','\nendmodule')==old.replace('\n\nendmodule','\nendmodule'),name
    cell=next(n for n in p['used_hdl']if Path(n).stem=='cell_F10145A_snapshot')
    text=(stage/cell).read_text().replace('\n    output wire [63:0] physical_memory,','',1)
    text=re.sub(r'^ assign physical_memory\[\d+\+:4\]=mem\[\d+\];\n','',text,flags=re.M)
    assert text==(native/'inputs'/cell).read_text()
    bp='src/build/rtl-executive/native16-tx-cable/dorado_native_backplane.v'
    text=(stage/bp).read_text();probe=(stage/'native-alu-observation.sv.txt').read_text()
    assert text.count(probe)==1;text=text.replace(probe+'\nendmodule','endmodule',1)
    for name in ['ProcH','ProcL']:
        line=f'\n    .alu_physical(alu_physical_{name}),.alu_inputs(alu_inputs_{name}),.alu_flag_pins(alu_flag_pins_{name}),'
        assert text.count(line)==1;text=text.replace(line,'',1)
    assert text==(native/'inputs'/bp).read_text()
    text=(stage/'harness/tb_native_cedar.sv').read_text()
    text,count=re.subn(r'\n  // Read-only native source records keep the new observation path live\..*?\n  end\n\nendmodule','endmodule',text,count=1,flags=re.S)
    assert count==1 and text==(native/'inputs/harness/tb_native_cedar.sv').read_text()
    assert 'always @(posedge storage_clk)if(r_capture_commit)'in probe
    modified={n for n,h in p['inputs'].items()if sha(stage/n)!=h}
    allowed={bp,cell,'harness/tb_native_cedar.sv',*[f'src/fpga/mister/snapshot_native_processor/{n}_full_snapshot.sv'for n in ['ProcH','ProcL']]}
    for n in modified-allowed:
        assert Path(n).suffix in ['.qsf','.qip','.qpf','.sdc','.tcl'],n
        if n.endswith('.sdc'):
            # No timing exception was added, including the existing memory
            # own-address constraints. Only frozen project roots relocate.
            old=json.loads((Path(p['stage'])/p['stage_manifest_name']).read_text())['project']
            assert (stage/n).read_text()==(native/'inputs'/n).read_text().replace(old,r['project']),n
    assert sha(stage/'core/Dorado.sv')==sha(native/'inputs/core/Dorado.sv')
    frame=ROOT/'verilog/rtl/dorado_branch_condition_frame_step.sv'
    oldframe=good[0]/'replay/branch-step.sv'
    tokens=lambda text:re.sub(r'\s+','',re.sub(r'/\*.*?\*/','',text,flags=re.S))
    assert tokens(frame.read_text())==tokens(oldframe.read_text())
    lint=BUILD/'native-alu-observation-lint-v3-20261025';l=json.loads((lint/'run-state.json').read_text())
    assert l['status']=='PASS_NATIVE_PROCESSOR_CONTROL_ELABORATION'and l['structural_warnings_visible']
    assert l['stage_manifest_sha256']==sha(stage/'r-front-control-native.json')
    log=(lint/'lint.log').read_text();warnings=len(re.findall(r'^%Warning-UNOPTFLAT:',log,re.M))
    assert warnings==9 and "capture.q'"not in log
    # The native run is represented by verified process/immutable-input
    # observations, never a terminal correctness claim while it is live.
    run=BUILD/'native-alu-source-boot-prefix-v1-20261025'
    run_provenance=json.loads(gzip.decompress((run/'provenance.json.gz').read_bytes()))
    verify(run/'inputs',run_provenance['inputs'])
    assert len(run_provenance['used_hdl'])==198 and not run_provenance['new_flag_owner_selected']
    assert int((run/'host-cycle-limit.txt').read_text())==600000000
    s=json.loads((run/'run-state.json').read_text());action=s['actions'][-1]
    assert s['status']=='RUNNING'and action['stage']=='test'and'returncode'not in action
    ps=subprocess.check_output(['ps','-p',str(action['pid']),'-o','pid=,etime=,time=,stat=,command='],text=True).strip()
    assert str(run)in ps
    progress=[x for x in(run/'test.log').read_text().splitlines()if x.startswith('NATIVE_PROGRESS')][-1]
    live=dict(directory=run.name,pid=action['pid'],ps=ps,observed=time.time(),progress=progress,
              original_fixed_HOST_limit=600000000,phase_records_completed=len(re.findall(r'^NATIVE_ALU_PHASE_RECORD',(run/'test.log').read_text(),re.M)))
    (out/'native-run-provenance.json.gz').write_bytes((run/'provenance.json.gz').read_bytes())
    (out/'native-run-state-at-observation.json').write_bytes((run/'run-state.json').read_bytes())
    retain(stage,out/stage.name);retain(lint,out/lint.name)
    # Preserve failed syntax lowering and earlier passive stage/lint variants.
    for name in ['native-alu-projection-lowering-icarus-v1-20261025','native-alu-projection-lowering-icarus-v2-20261025',
                 'native-alu-projection-lowered-v1-20261025',
                 'native-alu-observation-stage-v1-20261025','native-alu-observation-stage-v2-20261025',
                 'native-alu-observation-lint-v1-20261025','native-alu-observation-lint-v2-20261025']:
        d=BUILD/name;retain(d,out/name)
    for path in [frame,ROOT/'verilog/rtl/dorado_processor_flag_capture.sv']:
        target=out/'current-RTL'/path.name;target.parent.mkdir(exist_ok=True);shutil.copy2(path,target)
    assert sha(BUILD/'cedar-work-native-hints.pack')=='f3bce2a8566b3bac3dd793b2016910c71b0c8d7b061b9d48c14261665802df51'
    result=dict(status='REVIEWED_NATIVE_ALU_CAPTURE_INTERFACE_AND_PASSIVE_PRODUCER_WIRING',scope=__doc__,
        both_engines_summary=summaries[0],all2287_projection_outputs_checked=True,arbitrary_binary_cases=1024,input_bits=1632,
        stable_export_numbering_same_source_in_both_engines=True,
        late_source_root_mutation_rejected=False,
        all_binary_early_data_root_independence_proved=True,
        root_independence_input_bits=1632,root_independence_data_bits=16,root_independence_assumptions=0,
        late_source_root_trial_scope='The16 early capture-data bits are formally independent of raw root level with physical Q held; pre-event registered state and physical arrival/clock contracts remain required',
        native_stage_source_files=578,native_structural_UNOPTFLAT_warnings=warnings,
        original_owner_clock_enable_and_cell_update_code_preserved=True,probe_history_uses_common_commit=True,
        native_run=live,physical_native_phase_contract_qualified=False,flag_owner_selected=False,
        native60ns_batch_integrated=False,firmware_or_speed_qualified=False,hardware_changed=False,
        full_goal_active=True,next_action='Follow SAME native source diagnostic and analyze actual phase records; migrate flags/operand producers into the common chronological owner and shorten the measured settle path, retaining all storage/device barriers')
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--archive',type=Path);a=p.parse_args();r=review(a.output.resolve())
    if a.archive:append_archive(a.archive.resolve(),a.output.resolve(),[],label='native-alu-capture',extra_tools=TOOLS)
    print(r['status'])
