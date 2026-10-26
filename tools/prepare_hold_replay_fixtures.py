#!/usr/bin/env python3
"""Find two actual producer contexts for replaying recorded Hold386 values.

These are synthetic component fixtures, not native machine states or timing
measurements. They let the unchanged trace drive its original raw Hold values
while exercising the real132-bit prepared-Hold component and full caller.
"""
import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess

from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha


def prepare(source,out,yosys):
    assert not out.exists();out.mkdir(parents=True)
    r=json.loads((source/'results.json').read_text());verify(source,r['files'])
    assert r['status']=='PREPARED_SOURCE_BOUND_NATIVE_HOLD_FUNCTION'and r['context_bits']==132
    for p in source.iterdir():
        if p.suffix in ['.sv','.v']:shutil.copy2(p,out/p.name)
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    fixtures={};actions=[]
    for value in [0,1]:
        script=f'''read_verilog -sv {' '.join('/work/'+p.name for p in sorted(out.iterdir())if p.suffix in ['.sv','.v'])}
hierarchy -check -top dorado_memc_hold_function
proc
flatten
opt
check -assert
sat -set hold_value {value} -show-ports -dump_json /work/fixture-{value}.json
'''
        (out/f'fixture-{value}.ys').write_text(script);env=os.environ.copy();env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache');env['YOWASP_MOUNT']='/work='+str(out)
        cmd=[str(yosys),'-Q','-T','-s',f'/work/fixture-{value}.ys']
        with(out/f'fixture-{value}.log').open('x')as log:code=subprocess.run(cmd,stdout=log,stderr=log,env=env).returncode
        assert code==0
        c=json.loads((out/f'fixture-{value}.json').read_text())
        s={v['name']:v['data'][0]if'data'in v else v['wave'][0]for v in c['signal']}
        assert len(s['descriptor'])==132 and set(s['descriptor'])<={'0','1'}and s['hold_value']==str(value)
        fixtures[str(value)]=f"{int(s['descriptor'],2):033x}"
        actions.append(dict(value=value,returncode=code,log_sha256=sha(out/f'fixture-{value}.log'),counterexample_sha256=sha(out/f'fixture-{value}.json')))
    result=dict(status='PREPARED_SYNTHETIC_SOURCE_HOLD_REPLAY_FIXTURES',scope=__doc__,source_function_receipt_sha256=sha(source/'results.json'),
                fixtures_hex=fixtures,actions=actions,native_machine_state_or_timing_claimed=False,
                files={p.name:sha(p)for p in out.iterdir()if p.is_file()})
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],fixtures,flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['source','output','yosys']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();prepare(a.source.resolve(),a.output.resolve(),a.yosys.resolve())
