#!/usr/bin/env python3
"""Fresh complete MSA inventory plus required clock phase and fixture state.

The external clock-control phase is explicitly accounted for, not silently
excluded. Hardware capture/reconstruction and a native barrier remain pending.
"""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import subprocess
import xml.etree.ElementTree as ET
import generate_msa_clocked as gen
import test_msa_clocked as test
import classify_msa_board as base
from classify_serial_boards import read_inventory,alias_controls
from sil_state_inventory import destinations

EXTRA={'msa_board_fixture.gate.gate_open':(1,'CLOCK CONTROL PHASE REQUIRED'),
       'msa_board_fixture.actual_advance':(1,'FIXTURE LATCHED GUEST ENABLE'),
       'msa_board_fixture.ff_edges':(32,'FIXTURE EDGE MONITOR')}


def edges(normalized):
    tree=ET.fromstring(normalized);modules={m.get('name'):m for m in tree.findall('.//module')}
    cells={c.get('hier'):modules[c.get('submodname')]for c in tree.findall('.//cells//cell')}
    result={}
    for path in EXTRA:
        owner,variable=path.rsplit('.',1);module=cells[owner];writers=[]
        for process in module.findall('./always'):
            written=set()
            for a in process.iter():
                if a.tag in ('assign','assigndly'):written|=destinations(a[1])
            if variable in written:
                writers.extend((s.get('edgeType'),s.find('./varref').get('name'))for s in process.findall('./sentree/senitem'))
        result[path]=writers
    return result


def classify(data,manifest):
    expected=dict(EXTRA);extra=[]
    for row in data['candidates']:
        if row['path']not in EXTRA:continue
        wanted=expected.pop(row['path'],None);assert wanted is not None,'duplicate clock/fixture field'
        width,scope=wanted
        assert (row['category'],row['width'],row['bits'],row['unpacked_dimensions'])==('sequential',width,width,[]),'clock/fixture geometry differs'
        extra.append(dict(path=row['path'],bits=width,scope=scope))
    assert not expected,'missing clock/fixture state: '+str(expected)
    wanted_edges={'msa_board_fixture.gate.gate_open':[('NEG','clk')],
                  'msa_board_fixture.actual_advance':[('NEG','clk')],
                  'msa_board_fixture.ff_edges':[('POS','ff_clock')]}
    # JSON round trips lists/tuples, so compare canonical JSON structures.
    assert json.loads(json.dumps(data['clock_edges']))==json.loads(json.dumps(wanted_edges)),'clock sampling phase differs'
    report=dict(data);report['candidates']=[r for r in data['candidates']if r['path']not in EXTRA]
    board=base.classify(report,manifest);decisions=board['decisions']+extra
    assert sum(r['bits']for r in decisions)==data['candidate_bits']['sequential']==5497
    return dict(status='PASS_BOARD_AND_REQUIRED_CLOCK_PHASE',scope=__doc__,physical_bits_per_side=2690,private_board_bits=83,
                required_clock_phase_bits=1,fixture_bits=33,sequential_exclusions=[],decisions=decisions,
                clock_edges=data['clock_edges'],xml_sha256=data['xml_sha256'],normalized_xml_sha256=data['normalized_xml_sha256'],
                resolved_module_aliases=data['resolved_module_aliases'])


def controls(data,manifest):
    report=dict(data);report['candidates']=[r for r in data['candidates']if r['path']not in EXTRA]
    results=base.controls(report,manifest)
    for case in ('clock_phase_missing','clock_phase_width','guest_enable_missing','monitor_missing','wrong_clock_edge'):
        broken=copy.deepcopy(data);rows=broken['candidates'];target=next(r for r in rows if r['path']=='msa_board_fixture.gate.gate_open')
        if case=='clock_phase_missing':rows.remove(target)
        elif case=='clock_phase_width':target['width']=2
        elif case=='guest_enable_missing':rows.remove(next(r for r in rows if r['path']=='msa_board_fixture.actual_advance'))
        elif case=='monitor_missing':rows.remove(next(r for r in rows if r['path']=='msa_board_fixture.ff_edges'))
        else:broken['clock_edges']['msa_board_fixture.gate.gate_open']=[('POS','clk')]
        try:classify(broken,manifest)
        except AssertionError as error:results.append(dict(case=case,rejected=True,diagnostic=str(error)))
        else:raise AssertionError('damaged clock inventory accepted: '+case)
    return results


def run(tests,out):
    result=json.loads((tests/'results.json').read_text());assert result['status']=='PASS'
    board,manifest,helpers=gen.generate();top,_,_=test.fixture(manifest);assets={'msa_board.sv':board,**helpers,'ff_clock_model.sv':(test.HERE/'ff_clock_falling_model.sv').read_text(),'fixture.sv':top}
    assert json.loads(json.dumps(manifest))==json.loads((tests/'manifest.json').read_text())
    for name,source in assets.items():assert (tests/'candidate'/name).read_text()==source,'tested source differs: '+name
    for name,digest in result['executed_inputs'].items():assert hashlib.sha256((tests/'executed-inputs'/name).read_bytes()).hexdigest()==digest,'executed input differs'
    out.mkdir(parents=True,exist_ok=False)
    sources=[p for p in json.loads((tests/'candidate/command.json').read_text())if p.endswith(('.v','.sv'))]
    command=['verilator','--xml-only','--assert','--top-module','msa_board_fixture','-Wno-fatal','--xml-output',str(out/'state.xml'),*sources]
    (out/'command.json').write_text(json.dumps(command,indent=2)+'\n')
    with(out/'elaborate.log').open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
    raw=(out/'state.xml').read_bytes();data,normalized=read_inventory(raw);data['clock_edges']=edges(normalized)
    (out/'state-normalized.xml').write_bytes(normalized);r=classify(data,manifest);r['failure_controls']=controls(data,manifest)+alias_controls(raw)
    for name,value in [('inventory',data),('classification',r),('manifest',manifest)]:(out/(name+'.json')).write_text(json.dumps(value,indent=2)+'\n')
    print(json.dumps(dict(status=r['status'],physical_bits_per_side=2690,private_board_bits=83,clock_phase_bits=1,fixture_bits=33,failure_controls=len(r['failure_controls']))),flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('tests',type=Path);p.add_argument('--output',type=Path,required=True);a=p.parse_args();run(a.tests.resolve(),a.output.resolve())
