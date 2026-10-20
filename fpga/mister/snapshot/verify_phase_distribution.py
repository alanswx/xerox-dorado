#!/usr/bin/env python3
"""Match executed complete-board/control/MSA inputs to the current phase generator.

This closes component source provenance. It does not replace a native inventory,
Main integration, routed timing, full snapshot access or FPGA qualification.
"""
import argparse
import json
from pathlib import Path
import re
import test_phase_boards as boards
import test_native_phase_domain as domain
import test_eight_msa_phase as msa


def verify(board_roots,control,eight_msa,out):
    found={};records=[]
    for root in board_roots:
        for case in root.iterdir():
            if not case.is_dir()or not(case/'test.txt').is_file():continue
            assert case.name not in found,case.name
            found[case.name]=case
    assert set(found)=={s[0]for s in boards.base.gen.fmt.SPECS}
    for name,case in found.items():
        generated,record,original=boards.base.gen.generate(boards.base.gen.ROOT/'fpga/mister/reports',name)
        generated,families=boards.adapt(generated,record);top,header,metadata=boards.fixture(record,original)
        expected={**original['sources'],**generated,'fixture.sv':top.encode(),
            'parameters.h':header.encode(),'driver.cpp':boards.driver().encode(),
            'board_phase_enable.sv':(boards.HERE/'board_phase_enable.sv').read_bytes()}
        hashes={}
        for n,raw in expected.items():
            p=case/'inputs'/n;assert p.read_bytes()==raw,(name,n)
            hashes[n]=boards.base.gen.sha(raw)
        text=(case/'test.txt').read_text()
        match=re.search(r'PHASE WHOLE BOARD PASS host=(\d+) checks=(\d+) ordinary=20000 forced_rows=32 abort=32',text)
        assert match and 'Fatal'not in text,(name,text)
        assert json.loads((case/'metadata.json').read_text())==dict(**metadata,qualified_families=families)
        command=json.loads((case/'command.json').read_text());assert '--assert'in command
        records.append(dict(board=name,source_hashes=hashes,host_edges=int(match[1]),checks=int(match[2]),
            families=families,physical_comparison=metadata))
    source,record=domain.frozen();assets,review=domain.fixture(source)
    for n,text in assets.items():assert (control/'executed-inputs'/n).read_text()==text,n
    tested=json.loads((control/'results.json').read_text())
    assert tested['status']=='PASS_NATIVE_CONTROL_DOMAIN'and tested['review']==review
    assert tested['verified_native_sources']==record['source_hashes']
    assert len(tested['cases'])==5 and all(c['returncode']!=0 if c.get('expected_failure')else c['returncode']==0 for c in tested['cases'])
    for file,quarantine in [('test.txt',1),('random-test.txt',0)]:
        text=(control/'candidate'/file).read_text()
        assert 'NATIVE PHASE DOMAIN PASS'in text and f'partial_import_quarantine={quarantine}'in text
    sources,metadata,count=msa.assets()
    for n,text in sources.items():assert (eight_msa/'executed-inputs'/n).read_text()==text,n
    tested_msa=json.loads((eight_msa/'results.json').read_text())
    assert tested_msa['status']=='PASS_EIGHT_MSA_PHASE'and tested_msa['metadata']==metadata
    assert tested_msa['helper_instances']==count*8 and tested_msa['physical_bits']==12304
    assert tested_msa['phase_bits']==19 and tested_msa['readiness_bits']==8
    assert len(tested_msa['cases'])==3
    for case in tested_msa['cases']:
        log=(eight_msa/case['case']/'test.txt').read_text()
        if case['expected_failure']:
            assert case['returncode']!=0 and case['expected_failure']in log
        else:
            assert case['returncode']==0 and all(s in log for s in ('EIGHT MSA QUARANTINE PASS',
                'EIGHT MSA READY PREDICTION PASS','EIGHT MSA SDRAM PAUSE PASS'))
    result=dict(status='PASS_PHASE_BOARDS_NATIVE_CONTROL_AND_SDRAM_PROVENANCE',scope=__doc__,boards=records,
        boards_checked=len(records),board_checks=sum(r['checks']for r in records),
        board_host_edges=sum(r['host_edges']for r in records),native_review=review,
        native_source_count=len(record['source_hashes']),
        eight_msa=tested_msa,
        limits='Separate complete-board, native-control-cone and eight-MSA/SDRAM qualifications. Full-native integration, RAM-family optimization, nineteen-bit state access, actual Main controls and routed timing remain open.')
    out.parent.mkdir(parents=True,exist_ok=True);out.write_text(json.dumps(result,indent=2)+'\n')
    print(result['status'],result['boards_checked'],'boards;',result['board_checks'],'checks')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--boards',type=Path,nargs='+',required=True)
    p.add_argument('--control',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--eight-msa',type=Path,required=True);a=p.parse_args()
    verify([v.resolve()for v in a.boards],a.control.resolve(),a.eight_msa.resolve(),a.output.resolve())
