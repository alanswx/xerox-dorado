#!/usr/bin/env python3
"""Exercise deployment guards with temporary synthetic metadata; no hardware."""
import copy
import json
from pathlib import Path
import tempfile
import xml.etree.ElementTree as ET

from deploy_r_front_ready_native import reviewed_image,rewrite_mgl,sha


def fixture(path,change=None):
    path.mkdir()
    (path/'Dorado.rbf').write_bytes(b'TEST ONLY: NOT AN FPGA IMAGE\n')
    record=dict(source_hashes={'test-only-source':'not-a-production-source'},
        native_visible_R_bits=16,evaluator_owned_Q_bits=736,internal_predicted_R_duplicate=False)
    q=dict(status='PASS_NATIVE_R_FRONT_READY_PREASSEMBLY_REVIEW',state_admin='INACTIVE',
        frequency_mhz=50,sysper=4,minimum_slack_ns=.1,timing_checks=[0]*31,
        detailed_paths=[0]*196,central_paths=[0]*160,initial_component_enabled_samples=611683,
        firmware_qualified=False,hardware_speedup_claimed=False,native_visible_R_registers=16,
        source_hashes=copy.deepcopy(record['source_hashes']))
    names=['native_R_mapbuf_hold','native_R_register_coverage','missing_native_R_mapbuf_hold','native_R_bit_identity']
    names += ['test-only-control-'+str(i)for i in range(11)]
    controls=dict(status='PASS_NATIVE_READY_FIT_READER_CONTROLS',cases=[dict(case=n,rejected=True)for n in names])
    proof=dict(status='PASS_REPAIRED_NATIVE_COMPONENT_IMAGE_HANDOFF',exact_staged_native_sources_verified=True,
        canonical_native_R=True,original_IM_words_verified=3600,original_IFUM_entries_verified=256,
        Cedar_entry_executed=True,final_RM_map_dump_identical=True,inputs={})
    if change:change(q,record,controls,proof)
    for name,data in [('qualification.json',q),('r-front-ready-native.json',record),('fit-review-controls.json',controls)]:
        (path/name).write_text(json.dumps(data))
    if q.get('passive_operand_monitor'):
        (path/'operand-source-review.json').write_text(json.dumps(q['operand_monitor_source_review']))
    if not proof['inputs']:proof['inputs']={'test-only/r-front-ready-native.json':sha(path/'r-front-ready-native.json')}
    (path/'component-image-handoff.json').write_text(json.dumps(proof))
    a=dict(status='TERMINAL_ASSEMBLED_REVIEWED_NATIVE_CANDIDATE',returncode=0,
        qualification_sha256=sha(path/'qualification.json'),controls_sha256=sha(path/'fit-review-controls.json'),
        rbf_sha256=sha(path/'Dorado.rbf'))
    (path/'assembly-state.json').write_text(json.dumps(a))


def run():
    rejected=[]
    with tempfile.TemporaryDirectory(prefix='dorado-loader-guards-')as scratch:
        root=Path(scratch)
        fixture(root/'positive');assert reviewed_image(root/'positive',True)==sha(root/'positive/Dorado.rbf')
        def current_revision(q,r,c,p):
            q.update(host_video_register_mapping={'status':'PASS_UNCHANGED_HOST_VIDEO_REGISTER_MAPPING'},
                     additional_hold_uncertainty_ps=100)
            r['additional_hold_uncertainty_ps']=100
            c['cases'] += [dict(case=n,rejected=True)for n in
                           ['video_pipeline_register_coverage','hold_margin_not_applied']]
        fixture(root/'video-margin-positive',current_revision)
        assert reviewed_image(root/'video-margin-positive',True)==sha(root/'video-margin-positive/Dorado.rbf')
        def operand_revision(q,r,c,p):
            current_revision(q,r,c,p)
            q.update(passive_operand_monitor=True,operand_monitor_source_review={
                'status':'PASS_OUTPUT_ONLY_OPERAND_MONITOR_SOURCE','compiled_top':'core/Dorado.sv'},
                operand_monitor_compiled_sources={str(i):{}for i in range(5)})
            c['cases'].append(dict(case='operand_source_proof',rejected=True))
            c['cases'].append(dict(case='operand_compiled_source',rejected=True))
            p.update(passive_operand_monitor=True,operand_pin_alias_checks={'image':6875000,'map':1750000},
                     operand_readout_negative={'status':'PASS_SWAPPED_READOUT_REJECTED'})
        fixture(root/'operand-positive',operand_revision)
        assert reviewed_image(root/'operand-positive',True)==sha(root/'operand-positive/Dorado.rbf')
        for name,damage in [
            ('wrong_compiled_operand_top',lambda q,r,c,p:q['operand_monitor_source_review'].update(compiled_top='src/fpga/mister/Dorado.sv')),
            ('missing_operand_control',lambda q,r,c,p:c['cases'][-1].update(case='other-control')),
            ('missing_operand_alias_gate',lambda q,r,c,p:p.update(operand_pin_alias_checks={})),
            ('unverified_operand_negative',lambda q,r,c,p:p['operand_readout_negative'].update(status='NOT_CHECKED'))]:
            def change(q,r,c,p,damage=damage):
                operand_revision(q,r,c,p);damage(q,r,c,p)
            fixture(root/name,change)
            try:reviewed_image(root/name,True)
            except AssertionError:rejected.append(name)
            else:raise AssertionError('Unsafe operand metadata accepted: '+name)
        for name,missing in [('missing_video_control','video_pipeline_register_coverage'),
                             ('missing_margin_control','hold_margin_not_applied')]:
            def change(q,r,c,p,missing=missing):
                current_revision(q,r,c,p)
                next(row for row in c['cases']if row['case']==missing)['case']='other-control'
            fixture(root/name,change)
            try:reviewed_image(root/name,True)
            except AssertionError:rejected.append(name)
            else:raise AssertionError('Missing deployment control accepted: '+missing)
        cases={
            'missing_R_timing':lambda q,r,c,p:q.update(central_paths=[0]*136),
            'zero_slack':lambda q,r,c,p:q.update(minimum_slack_ns=0),
            'duplicate_R_state':lambda q,r,c,p:r.update(internal_predicted_R_duplicate=True),
            'wrong_R_owner_width':lambda q,r,c,p:r.update(evaluator_owned_Q_bits=752),
            'wrong_image_source':lambda q,r,c,p:p.update(inputs={'other/r-front-ready-native.json':'different'}),
            'entry_not_executed':lambda q,r,c,p:p.update(Cedar_entry_executed=False),
            'missing_named_R_control':lambda q,r,c,p:c['cases'][0].update(case='other-control'),
            'duplicate_control':lambda q,r,c,p:c['cases'][-1].update(case=c['cases'][-2]['case']),
        }
        for name,change in cases.items():
            fixture(root/name,change)
            try:reviewed_image(root/name,True)
            except AssertionError:rejected.append(name)
            else:raise AssertionError('Unsafe metadata accepted: '+name)
        for name,file in [('stale_RBF','Dorado.rbf'),('changed_controls','fit-review-controls.json')]:
            fixture(root/name)
            with(root/name/file).open('ab')as stream:stream.write(b' ')
            try:reviewed_image(root/name,True)
            except AssertionError:rejected.append(name)
            else:raise AssertionError('Changed artifact accepted: '+name)
    old='CedarRFrontReady_Boot2_20261021';new='CedarRVisible4_20261021';launcher='DoradoRVis4_20261021'
    text=f'''<mistergamedescription><rbf>_Computer/DoradoRTL_{old}/Dorado</rbf>
<setname same_dir="1">DoradoRFrontB2_20261021</setname>
<file delay="2" type="s" index="0" path="/media/fat/games/DoradoRTL/{old}/cedar-work.pack" /></mistergamedescription>'''
    result=ET.fromstring(rewrite_mgl(text,old,new,launcher))
    assert result.find('setname').text==launcher and result.find('setname').get('same_dir')=='1'
    assert result.find('rbf').text==f'_Computer/DoradoRTL_{new}/Dorado'
    assert result.find('file').get('path')==f'/media/fat/games/DoradoRTL/{new}/cedar-work.pack'
    assert result.find('file').get('index')=='0'
    for bad in ['missing setname',text+text]:
        try:rewrite_mgl(bad,old,new,launcher)
        except AssertionError:rejected.append('invalid_MGL')
        else:raise AssertionError('Ambiguous/missing launcher accepted')
    return dict(status='PASS_CANONICAL_R_LOADER_GUARDS',rejected_cases=rejected,
                launcher_paths_and_attributes_verified=True,synthetic_metadata_only=True,hardware_changed=False)


if __name__=='__main__':print(json.dumps(run(),indent=2))
