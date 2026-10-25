#!/usr/bin/env python3
"""Review the coupled original operand/ALU capture and bind its native Q pins.

The1198-bit original reference and80-bit flag transfer pass both engines.
This establishes the tested digital phase composition, not arbitrary physical
arrival timing or native60ns integration. Preserve the earlier missing-low-byte
zero-reduction receipts as superseded evidence, without rewriting them.
"""
import argparse
import json
from pathlib import Path
import re
import subprocess
import time

from generate_processor_alu_reference import generate,BUILD,ROOT
from native_branch_flag_sources import operand_producers
from review_late_hold_and_input_reset import retain
from review_processor_branch_composition import checked
from review_r_front_capture_followup import append_archive
from test_external_r_packet import sha
from test_processor_alu_composition import with_write_suffix

TOOLS=[Path(__file__).name,'generate_processor_alu_reference.py','generate_processor_alu_projection.py',
       'test_processor_alu_composition.py','native_branch_flag_sources.py','review_native_cp_read_timing.py']


def review(out):
    assert not out.exists();out.mkdir(parents=True)
    good=[BUILD/'processor-operand-wired-zero-icarus-v1-20261025',
          BUILD/'processor-operand-wired-zero-verilator-v2-20261025']
    states=[checked(d)for d in good]
    assert states[0]['outcomes']==states[1]['outcomes']
    original=(BUILD/'processor-raw-input-recording-v1-20261025/raw-inputs.hex').read_text().splitlines()
    expected,selectors=with_write_suffix(original)
    for d,s in zip(good,states):
        assert s['status']=='PASS_ORIGINAL_ALU_PROCESSOR_COMPOSITION'and s['returncode']==0
        assert s['optimized_flag_transfer_checked']and s['explicit_pre_root_ALU_projection']
        assert s['supported_operand_registers_integrated']and s['normal_MIR_ALUF_write_suffix']
        assert s['reference']['physical_bits']==1198 and s['reference']['clock_channels']==147
        assert s['reference']['boundary_pins']==432 and s['records']==15470
        assert (d/'raw-inputs.hex').read_text().splitlines()==expected
        assert [int(x,16)for x in(d/'selectors.hex').read_text().splitlines()]==selectors
        assert set(map(tuple,s['reference']['global_drivers']["aluOut=0'"]))=={
            (name,'d08',pin)for name in ['ProcH','ProcL']for pin in [2,15]}
        log=(d/'test.log').read_text()
        clocks=dict(re.findall(r'^ALU_COMPOSITION_CLOCK channel=(\d+) edges=(\d+)$',log,re.M))
        assert len(clocks)==147 and all(int(v)>0 for v in clocks.values())
        assert clocks['105']==clocks['106']=='64'
        assert 'ALU_FUNCTION_WRITE_COVERAGE addresses=ffff address_changes=0 data_changes=0'in log
        tb=(d/'tb.sv').read_text()
        assert 'if((^a)===1\'bx || (^b)===1\'bx || (^ac)===1\'bx || (^bc)===1\'bx)'in tb
        assert 'projection_q=a;projection_root=root;'in tb
        assert 'before_pins,projected_pins,full.branch_pins,next_q,events,unsettled)'in tb
    # Current generator metadata adds explicit physical offsets, but the
    # tested gate wiring must remain byte-for-byte identical in both engines.
    current=out/'current-reference';m=generate(current,close_operands=True)
    for p in sorted((current/'timed').iterdir()):
        if p.suffix in ['.sv','.v']:
            assert all(sha(p)==sha(d/'reference/timed'/p.name)for d in good),p.name
    native=BUILD/'native-cp-read-j19-boot-v1-20261025'
    binding=operand_producers(native,m)
    assert binding['physical_bits']==134 and len(binding['packages'])==33
    (out/'native-operand-Q-binding.json').write_text(json.dumps(binding,indent=2)+'\n')
    negatives=[]
    for engine in ['icarus','verilator']:
        for name,mutation,pattern,expected_diff in [
            ('zero','drop-low-zero',r'^ALU_COMPOSITION_OBSERVABLE_DIFF event=(\d+) bit=(\d+) full=([01]) split=([01])$',
             [('7940','486','1','0')]),
            ('late','late-projection',r'^ALU_TRANSFER_DIFF event=(\d+) bit=(\d+) predicted=([01]) original=([01])$',
             [('15','73','1','0')])]:
            d=BUILD/f'processor-operand-{name}-negative-{engine}-v1-20261025';s=checked(d)
            assert s['returncode']!=0 and s['mutation']==mutation
            differences=re.findall(pattern,(d/'test.log').read_text(),re.M);assert differences==expected_diff
            negatives.append(dict(directory=d.name,mutation=mutation,differences=differences))
            retain(d,out/d.name)
    # The earlier claim that both zero-detector boards were closed was wrong.
    # Verify the exact omission, and preserve those historical receipts.
    old=BUILD/'processor-alu-mir-writes-icarus-v2-20261025/reference/manifest.json'
    old_reference=json.loads(old.read_text())
    assert old_reference['global_drivers']["aluOut=0'"]==[['ProcH','d08',2],['ProcH','d08',15]]
    timing=BUILD/'native-cp-read-full15-timing-review-v1-20261025'
    fit=json.loads((timing/'results.json').read_text())
    assert fit['status']=='REVIEWED_FULL_CORE_15NS_TIMING_REJECTION'
    assert fit['worst_setup_path']['slack_ns']==-11.669 and not fit['assembly_permitted']
    observations=[]
    for d in [native,BUILD/'control-short-native-jumper-boot-v2-20261024']:
        s=json.loads((d/'run-state.json').read_text());action=s['actions'][-1]
        assert s['status']=='RUNNING'and'returncode'not in action
        row=subprocess.check_output(['ps','-p',str(action['pid']),'-o','pid=,etime=,time=,stat=,command='],text=True).strip()
        assert str(d)in row
        assert int((d/'host-cycle-limit.txt').read_text())==5000000000
        progress=[x for x in(d/'test.log').read_text().splitlines()if x.startswith('NATIVE_PROGRESS')][-1]
        observations.append(dict(directory=d.name,pid=action['pid'],ps=row,progress=progress,observed=time.time()))
    assert sha(BUILD/'cedar-work-native-hints.pack')=='f3bce2a8566b3bac3dd793b2016910c71b0c8d7b061b9d48c14261665802df51'
    for d in good:retain(d,out/d.name)
    # Include constructor rejections and unqualified intermediate passes;
    # no failed/partial experiment is relabelled as a native qualification.
    for pattern in ['processor-operand-composition-*-20261025','processor-operand-projection-*-20261025',
                    'processor-operand-frontier-inventory-*-20261025']:
        for d in sorted(BUILD.glob(pattern)):retain(d,out/d.name)
    retain(timing,out/timing.name)
    result=dict(status='REVIEWED_ORIGINAL_OPERAND_ALU_FLAG_CAPTURE_AND_NATIVE_BINDING',scope=__doc__,
        comparison_summary=states[0]['outcomes'][-1],all147_clock_counts=states[0]['outcomes'][:-1],
        actual_native_operand_binding=binding,negative_controls=negatives,
        low_byte_zero_omission_corrected=True,superseded_reference_sha256=sha(old),
        prior1064_and_partial1198_receipts_do_not_qualify_full_16bit_zero=True,
        full_core_15ns_timing=fit,native_boot_observations=observations,
        old_physical_indices_1063_to_0_retained=True,new_physical_bits=134,
        original_input_prefix_byte_identical=True,complete_function_RAM_write_windows_checked=True,
        controlled_digital_composition_qualified=True,arbitrary_source_arrivals_or_unknown_inputs_qualified=False,
        native_common_owner_or_full60ns_batch_integrated=False,physical_timing_qualified=False,
        firmware_or_speed_qualified=False,hardware_changed=False,full_goal_active=True,
        next_action='Integrate the native operand/ALU/flag producer phases into the common chronological capture; remove the deep iterative settle dependency without weakening storage/device barriers, then refit and benchmark')
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--archive',type=Path);a=p.parse_args();r=review(a.output.resolve())
    if a.archive:append_archive(a.archive.resolve(),a.output.resolve(),[],label='processor-operand-capture',extra_tools=TOOLS)
    print(r['status'])
