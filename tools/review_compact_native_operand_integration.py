#!/usr/bin/env python3
"""Retain the selected compact descriptor, full replay gates and new native fit."""
import argparse
import json
from pathlib import Path
import shutil
import subprocess
import time

from replay_r_read_buffer import outcomes
from review_baseboard_clock_consumer import verify
from review_native_alu_capture import retain
from review_r_front_capture_followup import append_archive
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1];BUILD=ROOT/'build/rtl-executive'
TOOLS=[Path(__file__).name,'select_compact_native_operand_buffer.py','stage_compact_native_operand_buffer.py',
 'lint_native_clock_enables.py','lint_r_front_control_native.py','start_native_phase_guard_timing.py',
 'review_native_cp_read_timing.py','qualify_native_operand_buffer.py','review_common_clock_native_followup.py']


def review(out):
    assert not out.exists();out.mkdir(parents=True)
    stage=BUILD/'native-compact-operand-stage-v1-20261025';m=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,m['source_hashes'])
    assert m['compact_operand_tag_qualified']and m['operand_view_tag_support_bits']==465 and len(m['source_hashes'])==578
    parent=Path(m['parent_stage']);p=json.loads((parent/'r-front-control-native.json').read_text());verify(parent,p['source_hashes'])
    helper='src/verilog/rtl/dorado_native_operand_buffer.sv'
    differences={n for n,h in p['source_hashes'].items()if Path(n).suffix in ['.v','.sv']and m['source_hashes'][n]!=h}
    assert differences=={helper}and m['incremental_functional_HDL_changes']==[helper]
    selection=BUILD/'native-compact-operand-selection-v1-20261025';s=json.loads((selection/'results.json').read_text())
    assert s['status']=='SELECTED_QUALIFIED_COMPACT_NATIVE_OPERAND_DESCRIPTOR'
    assert s['selected_sha256']==sha(ROOT/'verilog/rtl/dorado_native_operand_buffer.sv')==m['source_hashes'][helper]
    assert s['before_sha256']==p['source_hashes'][helper]
    replays=[]
    for engine in ['icarus','verilator']:
        for mode in ['full','words']:
            d=BUILD/f'native-operand-binary-mask-{mode}-{engine}-v1-20261025';r=json.loads((d/'run-state.json').read_text());verify(d,r['files'])
            assert r['status']=='PASS_PREPARED_OPERAND_COMPLETE_NATIVE_CALLER'
            assert r['operand_proof_state_sha256']==m['operand_buffer_proof_state_sha256']
            assert r['operand_prepare_edges']==(2353 if mode=='full'else 3 if engine=='icarus'else 8)
            assert outcomes((d/'test.log').read_text())==outcomes((Path(r['source'])/'test.log').read_text())
            replays.append(dict(engine=engine,mode=mode,preparation_edges=r['operand_prepare_edges'],state_sha256=sha(d/'run-state.json')))
    lint=BUILD/'native-compact-operand-lint-v1-20261025';l=json.loads((lint/'run-state.json').read_text());verify(lint,l['files'])
    assert l['status']=='ELABORATED_NATIVE_COMMON_CLOCK_ENABLE_EXPERIMENT'and all(a['returncode']==0 for a in l['actions'])
    assert l['stage_manifest_sha256']==sha(stage/'r-front-control-native.json')and 'dorado_native_machine_ce #'in(lint/'tb_native_cedar.sv').read_text()
    audit=BUILD/'native-compact-operand-audit-v1-20261025';a=json.loads((audit/'results.json').read_text())
    assert a['status']=='PASS_NATIVE_COMMON_CLOCK_ENABLE_STRUCTURAL_AUDIT'and a['stage_manifest_sha256']==sha(stage/'r-front-control-native.json')
    assert a['process_counts']['removed_guards']==1823 and a['process_counts']['HOST_CLOCK_RETAINS_EXISTING_POLICY']==1975
    launch=BUILD/'native-compact-operand-launch-v1-20261025';la=json.loads((launch/'launch-observation.json').read_text())
    assert la['builder_pid']==743068 and la['manifest_sha256']==sha(stage/'r-front-control-native.json')and not la['assembly_permitted']
    query='''from pathlib import Path
import hashlib,json,subprocess,time
p=Path(PROJECT);m=json.loads((p/'r-front-control-native.json').read_text())
assert len(m['source_hashes'])==578 and all(hashlib.sha256((p/n).read_bytes()).hexdigest()==h for n,h in m['source_hashes'].items())
s=json.loads((p/'run-state.json').read_text());assert s['builder_pid']==743068 and not s['assembly_permitted']
ps=[]
if s['status']=='RUNNING':
 for pid in [s['builder_pid'],s['stages'][-1]['pid']]:
  r=subprocess.run(['ps','-p',str(pid),'-o','pid=,etime=,time=,stat=,args='],capture_output=True,text=True);ps.append({'pid':pid,'returncode':r.returncode,'process':r.stdout.strip()})
 assert any(r['returncode']==0 for r in ps)
print(json.dumps({'observed':time.time(),'run_state':s,'processes':ps,'source_files_verified':len(m['source_hashes'])}))
'''.replace('PROJECT',repr(m['project']))
    (out/'same-compact-native-query.py').write_text(query)
    remote=subprocess.run(['ssh','-o','BatchMode=yes','-o','ConnectTimeout=10','misterubuntu','python3','-'],input=query,text=True,capture_output=True)
    (out/'same-compact-native-query.stderr').write_text(remote.stderr);assert remote.returncode==0,remote.stderr
    fit=json.loads(remote.stdout);(out/'same-compact-native-observation.json').write_text(json.dumps(fit,indent=2)+'\n')
    for d in [selection,stage,lint,audit,launch,*[BUILD/f'native-operand-binary-mask-{mode}-icarus-v1-20261025'for mode in ['full','words']]]:
        retain(d,out/'retained'/d.name)
    # Preserve the interrupted observation attempt: its expected-live peer had
    # actually completed successfully, so no process was restarted.
    earlier=BUILD/'native-compact-operand-words-retained-v1-20261025'
    if earlier.exists():retain(earlier,out/'retained'/earlier.name)
    result=dict(status='REVIEWED_SELECTED_COMPACT_NATIVE_OPERAND_INTEGRATION',stage_manifest_sha256=sha(stage/'r-front-control-native.json'),
        selected_helper_sha256=s['selected_sha256'],saved_tag_bits=465,previous_saved_tag_bits=909,logical_tag_bits=1264,
        actual_native_view_bits_proved=844,assumptions=0,XZ_checks=11895,all_four_replays=replays,
        changed_HDL_sources=[helper],nominal_ordinary_ns=120,target_ordinary_ns=60,
        actual_common_clock_lint_and_ancestry_pass=True,same_new_native_fit=fit,
        firmware_or_physical_timing_or_hardware_speed_qualified=False,FPGA_or_protected_media_changed=False,full_goal_active=True,
        next_action='Observe SAME743068 and native38212. Collect all terminal fit/corner/SDRAM reports; if congestion remains, prove/factor duplicate publication gating behind the actual global grant without losing readiness conditions. Then original60ns phases and full hardware qualification.')
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--archive',type=Path)
    a=p.parse_args();r=review(a.output.resolve())
    if a.archive:append_archive(a.archive.resolve(),a.output.resolve(),[],label='native-compact-operand-integration',extra_tools=TOOLS)
    print(r['status'],flush=True)
