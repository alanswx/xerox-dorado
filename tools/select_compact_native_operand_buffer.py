#!/usr/bin/env python3
"""Select the465-bit descriptor only after actual-function and replay gates."""
import argparse
import json
from pathlib import Path
import re
import shutil

from replay_r_read_buffer import outcomes
from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1];BUILD=ROOT/'build/rtl-executive'


def select(out):
    assert not out.exists()
    proof=BUILD/'native-operand-binary-mask-proof-v1-20261025';p=json.loads((proof/'run-state.json').read_text());verify(proof,p['files'])
    assert p['status']=='PASS_ACTUAL_NATIVE_PREPARED_OPERAND_VIEW'and p['tag_support_bits']==465 and p['assumptions']==0
    for a in p['actions']:
        assert a['passed']and a['returncode']==0;verify(proof/a['label'],a['files'])
        assert sha(proof/a['label']/'proof.log')==a['log_sha256']
    candidate=BUILD/'native-operand-binary-mask-candidate-v1-20261025/dorado_native_operand_buffer.sv'
    target=ROOT/'verilog/rtl/dorado_native_operand_buffer.sv'
    assert sha(candidate)==p['helper_sha256']and sha(target)==p['original_helper_sha256']
    pattern=r"OPERAND_TAG_SUPPORT=1264'h[0-9a-f]+;"
    assert re.sub(pattern,'MASK',candidate.read_text())==re.sub(pattern,'MASK',target.read_text())
    replays=[]
    for engine in ['icarus','verilator']:
        for mode in ['full','words']:
            d=BUILD/f'native-operand-binary-mask-{mode}-{engine}-v1-20261025';s=json.loads((d/'run-state.json').read_text());verify(d,s['files'])
            assert s['status']=='PASS_PREPARED_OPERAND_COMPLETE_NATIVE_CALLER'
            assert s['operand_proof_state_sha256']==sha(proof/'run-state.json')and s['buffer_helper_sha256']==sha(candidate)
            assert s['operand_prepare_edges']==(2353 if mode=='full'else 8 if engine=='verilator'else 3)
            assert outcomes((d/'test.log').read_text())==outcomes((Path(s['source'])/'test.log').read_text())
            replays.append(dict(engine=engine,mode=mode,state_sha256=sha(d/'run-state.json'),preparation_edges=s['operand_prepare_edges']))
    for name,status in [('native-operand-binary-mask-xz-v1-20261025','PASS_NATIVE_OPERAND_UNKNOWN_FALLBACK'),
                        ('native-operand-binary-mask-xz-negative-v1-20261025','PASS_NATIVE_OPERAND_UNKNOWN_BYPASS_COUNTEREXAMPLE')]:
        d=BUILD/name;s=json.loads((d/'run-state.json').read_text());verify(d,s['files'])
        assert s['status']==status and s['helper_sha256']==sha(candidate)
        if not s['broken_unknown_fallback']:assert 'checks=11895 unknown=11892 outputs=2969 physical=844'in(d/'test.log').read_text()
    out.mkdir(parents=True);shutil.copy2(target,out/'before.sv');shutil.copy2(candidate,out/'selected.sv');shutil.copy2(Path(__file__),out/Path(__file__).name)
    shutil.copy2(candidate,target)
    result=dict(status='SELECTED_QUALIFIED_COMPACT_NATIVE_OPERAND_DESCRIPTOR',before_sha256=sha(out/'before.sv'),selected_sha256=sha(target),
        proof_state_sha256=sha(proof/'run-state.json'),support_bits_before=909,support_bits_after=465,replays=replays,
        all844_native_view_bits_proved=True,assumptions=0,XZ_checks=11895,guest_owners_or_RAM_or_capture_edges_changed=False,
        new_native_fit_or_hardware_speed_or60ns_qualified=False,FPGA_or_media_changed=False)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();select(a.output.resolve())
