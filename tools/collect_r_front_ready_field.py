#!/usr/bin/env python3
"""Collect bounded live prefixes from the Ready/front test on MiSTer14.

Run on MiSTer14. This reads files and process identities only, does not copy
an active disk pack, and does not claim a coherent machine snapshot or a
completed boot. Prefixes and their original hashes are retained losslessly.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path
import time

from deploy_mc1668_native import MAIN, SERVER, sha,matches
from deploy_r_front_ready_native import RBF,reviewed_image


def collect(out, boot2=False,canonical=False):
    assert not(boot2 and canonical)
    name=('CedarRVisible4_20261021'if canonical else
          'CedarRFrontReady_Boot2_20261021'if boot2 else'CedarRFrontReady_20261021')
    base=Path('/media/fat/games/DoradoRTL')/name
    deployment=json.loads((base/'deployment.json').read_text())
    main_pid,server_pid=((deployment['main_pid'],deployment['server_pid'])if canonical else
                         (16808,16807)if boot2 else(15708,15703))
    assert deployment['main_pid']==main_pid and deployment['server_pid']==server_pid
    assert sha('/proc/%d/exe'%main_pid)==MAIN and sha('/proc/%d/exe'%server_pid)==SERVER
    if canonical:
        resume=json.loads((base/'reboot-resume.json').read_text())
        assert matches(resume['main']['exe'],resume['main']['argv'])==[main_pid]
        assert matches(resume['server']['exe'],resume['server']['argv'])==[server_pid]
    core=Path('/media/fat/_Computer')/('DoradoRTL_'+name)/'Dorado.rbf'
    expected_rbf=deployment['rbf_sha256']if canonical else RBF
    assert sha(core)==expected_rbf
    if canonical:
        # The deployed volume stores the review metadata; the actual image
        # remains in its core directory. Validate both without touching either.
        from tempfile import TemporaryDirectory
        with TemporaryDirectory(prefix='dorado-field-review-')as scratch:
            staging=Path(scratch)
            for item in ['qualification.json','assembly-state.json','fit-review-controls.json',
                         'r-front-ready-native.json','component-image-handoff.json']:
                (staging/item).symlink_to(base/item)
            (staging/'Dorado.rbf').symlink_to(core)
            assert reviewed_image(staging,True)==expected_rbf
    assert '/dev/mem' in Path('/proc/%d/maps'%server_pid).read_text()
    out.mkdir(parents=True,exist_ok=False)
    record=dict(scope=__doc__,started=time.time(),main_pid=main_pid,server_pid=server_pid,
                main_sha256=MAIN,server_sha256=SERVER,rbf_sha256=expected_rbf,
                host_clock_hz=50000000,sysper=4,state_admin='INACTIVE',files=[],
                executed_tools={p.name:sha(p) for p in [Path(__file__),
                    Path(__file__).with_name('deploy_mc1668_native.py'),
                    Path(__file__).with_name('deploy_r_front_ready_native.py')]})
    sources=[base/n for n in ('deployment.json','preparation.json','reboot-resume.json')]
    sources += [Path(deployment['main_log']),Path(deployment['disk_trace']),base/'server.log']
    sources += [base.parent/('CedarRFrontReady_Boot2_20261021/preservation-before-r-visible4.json'if canonical else
                            'CedarRFrontReady_20261021/preservation-before-rfront-ready-boot2.json'if boot2 else
                            'CedarMC1668_20261020/preservation-before-rfront-ready.json')]
    if boot2 or canonical:
        old=base.parent/('CedarRFrontReady_Boot2_20261021'if canonical else'CedarRFrontReady_20261021')
        proof=json.loads(sources[-1].read_text())
        suffix='r-visible4'if canonical else'rfront-ready-boot2'
        trace=old/('disk-through-'+suffix+'-load.jsonl')
        assert sha(trace)==proof['disk_trace_sha256']
        sources += [trace,old/('main-through-'+suffix+'-load.log')]
    if canonical:
        sources += [base/n for n in ['qualification.json','assembly-state.json','fit-review-controls.json',
                                    'r-front-ready-native.json','component-image-handoff.json']]
    for source in sources:
        with source.open('rb') as stream:
            limit=source.stat().st_size
            data=stream.read(limit)
        assert len(data)==limit
        # Do not manufacture an incomplete final record in a live text prefix.
        if not data.endswith(b'\n'):
            data=data[:data.rfind(b'\n')+1]
        target=out/(source.name+'.gz')
        target.write_bytes(gzip.compress(data,mtime=0))
        assert gzip.decompress(target.read_bytes())==data
        record['files'].append(dict(source=str(source),file=target.name,
                                    prefix_bytes=len(data),sha256=hashlib.sha256(data).hexdigest(),
                                    stored_sha256=sha(target)))
    record.update(finished=time.time(),desktop_qualified=False,hardware_speedup_claimed=False,
                  note='Live prefixes, not closed logs. Disk file excludes still-buffered RAM records.')
    (out/'collection.json').write_text(json.dumps(record,indent=2)+'\n')
    print(json.dumps(record,indent=2))


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--output',type=Path,required=True)
    p.add_argument('--boot2',action='store_true')
    p.add_argument('--canonical-r',action='store_true')
    a=p.parse_args();collect(a.output,a.boot2,a.canonical_r)
