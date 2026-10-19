"""Reproduce the nested ContB coverage proof from the archived native inventory.

Run from any directory with an optional inventory.json[.gz] argument. This is
coverage, not machine-wide state classification or a physical snapshot test.
"""
from pathlib import Path
import gzip
import json
import sys

ROOT=Path(__file__).resolve().parents[4]
sys.path.insert(0,str(ROOT/'fpga/mister/snapshot'))
import classify_contb_full as classifier
import generate_contb_full as generator

path=Path(sys.argv[1])if len(sys.argv)>1 else Path(__file__).with_name('inventory.json.gz')
data=gzip.decompress(path.read_bytes())if path.suffix=='.gz'else path.read_bytes()
report=json.loads(data)
prefix='dorado_native_machine.m.b_ContB.'
rows=[dict(row,path='ContB_m_Rev_m_Cd.'+row['path'][len(prefix):])
      for row in report['candidates']if row['path'].startswith(prefix)]
assert len(rows)==1572,len(rows)
_,manifest,_=generator.generate()
proof=classifier.classify(dict(report,candidates=rows),manifest)
assert proof['logical_state_bits']==147809
print(json.dumps({k:v for k,v in proof.items()if k!='decisions'},indent=2))
