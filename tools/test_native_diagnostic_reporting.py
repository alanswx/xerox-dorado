#!/usr/bin/env python3
"""Preserve diagnostic terminal results when the progress reader disappears.

Mock external commands only: this tests runner lifecycle, not RTL or firmware.
An actual failed stage must still fail and retain its exception detail.
"""
import argparse
import json
from pathlib import Path
from unittest.mock import patch

from run_native_cedar_memory import run


def check(out):
    assert not out.exists();out.mkdir(parents=True);results=[]
    for label,failed in [('closed-progress-pipe',False),('failed-stage',True)]:
        p=out/label;p.mkdir();(p/'inputs').mkdir();
        import gzip
        (p/'provenance.json.gz').write_bytes(gzip.compress(json.dumps(dict(inputs={},artifacts={},used_hdl=[],defines=[])).encode(),mtime=0))
        class Command:
            pid=12345
            def __init__(self,args,cwd,stdout,stderr):
                self.code=1 if failed else 0
                if stdout.name.endswith('test.log'):
                    stdout.write('NATIVE_DIAGNOSTIC_STOP reason= HOST_LIMIT host=400000000 guest=0 cpu=d0fff000\n')
            def wait(self):return self.code
        error=None
        with patch('run_native_cedar_memory.subprocess.Popen',Command),patch('builtins.print',side_effect=BrokenPipeError('expired output handle')):
            try:run(p)
            except AssertionError as e:error=repr(e)
        s=json.loads((p/'run-state.json').read_text())
        if failed:
            assert s['status']=='TERMINAL_FAILED'and error and 'AssertionError'in s['error']
        else:
            assert s['status']=='TERMINAL_NATIVE_MEMORY_DIAGNOSTIC'and s['reason']=='HOST_LIMIT'and not error
        results.append(dict(case=label,result=s['status']))
    (out/'results.json').write_text(json.dumps(dict(status='PASS_NATIVE_DIAGNOSTIC_REPORTING_LIFECYCLE',cases=results,native_qualified=False),indent=2)+'\n')
    print('PASS_NATIVE_DIAGNOSTIC_REPORTING_LIFECYCLE')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    check(p.parse_args().output.resolve())
