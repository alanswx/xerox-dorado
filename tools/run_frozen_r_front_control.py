#!/usr/bin/env python3
"""Run a saved experimental CLI/generator without editing a live matrix's tools.

Keep their original repository __file__ locations for source lookup. The
snapshot's explicit hashes are checked before executing these local tools.
All HDL/reference inputs are subsequently frozen by the normal test driver.
"""
import argparse
import hashlib
import json
from pathlib import Path
import sys
import types

ROOT=Path(__file__).resolve().parents[1]


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--snapshot',type=Path,required=True)
    a,remaining=p.parse_known_args();snapshot=a.snapshot.resolve()
    record=json.loads((snapshot/'restore-record.json').read_text())
    for n,r in record['files'].items():assert hashlib.sha256((snapshot/n).read_bytes()).hexdigest()==r['experimental_sha256'],n
    sys.path.insert(0,str(ROOT/'tools'))
    name='generate_r_front_control_candidate';module=types.ModuleType(name);module.__file__=str(ROOT/'tools'/(name+'.py'))
    exec(compile((snapshot/(name+'.py')).read_text(),module.__file__,'exec'),module.__dict__)
    sys.modules[name]=module
    path=str(ROOT/'tools/test_r_front_control.py');sys.argv=[path,*remaining]
    exec(compile((snapshot/'test_r_front_control.py').read_text(),path,'exec'),dict(__name__='__main__',__file__=path))
