#!/usr/bin/env python3
"""Frozen Serial73 hold-domain control experiment, with actual native wiring.

Extract the original owner/group/coordinator instances and final hold/service
equations, retaining all control logic. Nineteen small scan leaves stand in for
physical board payloads: this checks the native hold producer, not board/RAM
payload equivalence, state access, fitted register duplication or timing.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
from generate_serial_boards import ROOT,raw_receipt,once,rename,masked_comments
import generate_native_ready_prediction as native

HERE=Path(__file__).resolve().parent
REPORT='2026-10-20-snapshot-native-ready-prediction-rejected'
PREFIX='src/fpga/mister/snapshot_serial/'


def instance(text,module):
    matches=list(re.finditer(r'\b'+module+r'\s+(\w+)\s*\(.*?\);',masked_comments(text),re.S))
    assert len(matches)==1,module
    match=matches[0];return text[match.start():match.end()]


def frozen():
    report=ROOT/'fpga/mister/reports'/REPORT
    record=json.loads(raw_receipt(report,'terminal-fit/ready-prediction-integration.json'))
    sources={}
    for name,digest in record['source_hashes'].items():
        raw=raw_receipt(report,'terminal-fit/'+name)
        assert hashlib.sha256(raw).hexdigest()==digest,name
        sources[name]=raw.decode()
    assert len(sources)==662
    return sources,record


def fixture(sources):
    core=sources['core/Dorado.sv'];wrapper=sources[native.WRAPPER];backplane=sources[native.BACKPLANE]
    assert core.count('always @(negedge clk_sys) network_step <= machine_enable;')==1
    assert core.count('wire machine_enable = ~reset && pack_ready && storage_initialized && snapshot_frontends_cached && !board_state_hold;')==1
    assert backplane.count('.state_guest_advance(contb_advance)')==19
    assert backplane.count('.state_global_hold(snapshot_consumers_hold)')==19
    assert backplane.count('.state_freeze(snapshot_consumers_hold)')==19
    assert backplane.count('.state_guest_held(snapshot_guest_held)')==19
    assert wrapper.count('.snapshot_consumers_hold(snapshot_hold)')==1
    assert wrapper.count('.storage_clk(clk_host)')==1
    assert core.count('.contb_advance(network_step)')==1
    assert core.count('.clk_host(clk_sys)')>=1
    checked=[]
    for name in [n for n in sources if n.startswith(PREFIX)and(n.endswith('__board.sv')or n.endswith('MSA__msa_board.sv'))]:
        text=sources[name];m=re.search(r'wire state_hold\s*=([^;]+);',text);assert m,name
        expr=re.sub(r'\s+','',m[1]);assert expr in(
            'state_global_hold||((state_freeze&&state_guest_held)||state_scan_hold_required)',
            'state_global_hold||(state_freeze&&state_guest_held)||state_scan_hold_required'),name
        scan=re.search(r'\bu_ff_scan\s*\((.*?)\);',text,re.S)[1]
        assert '.freeze(state_freeze && !state_global_abort)'in scan
        assert '.guest_held(state_guest_held)'in scan and '.hold_required(state_scan_hold_required)'in scan
        checked.append(name)
    assert len(checked)==12
    # Copy the actual final hold cone. The backplane owner output is wired
    # to snapshot_owner_hold in the native wrapper, not final snapshot_hold.
    network=rename(instance(core,'dorado_network_pause_barrier'),dict(
        clk_sys='clk',board_state_owner_hold='snapshot_owner_hold',
        board_state_guest_held='snapshot_guest_held',board_state_fault='snapshot_fault',
        CAPACITY_TRANSPORT_RESET='snapshot_transport_reset',board_state_hold='snapshot_hold'))
    disk=rename(instance(wrapper,'dorado_disk_pause_barrier'),dict(clk_host='clk'))
    memory=rename(instance(wrapper,'dorado_memory_pause_barrier'),dict(clk_host='clk'))
    owner=rename(instance(backplane,'dorado_board_state_fabric_msa'),dict(storage_clk='clk',
        snapshot_hold='snapshot_owner_hold',snapshot_fault='snapshot_owner_fault',
        snapshot_services_held='snapshot_services_status'))
    group=rename(instance(backplane,'dorado_msa_state_group_chipwidth'),dict(storage_clk='clk'))
    equations=[]
    for signal in ('snapshot_services_status','snapshot_fault'):
        equations.append(re.search(r'assign '+signal+r'\s*=.*?;',wrapper,re.S)[0])
    blocks='\n'.join([network,disk,memory,owner,group,*equations])
    top=once((HERE/'test_native_phase_domain.sv').read_text(),' // ACTUAL_NATIVE_CONTROL',blocks)
    selected=['board_state_fabric_msa.sv','msa_state_group.sv','memory_pause_barrier.sv',
              'network_pause_barrier.sv','disk_pause_barrier.sv']
    assets={n:sources[PREFIX+n]for n in selected}
    controller=sources[PREFIX+'MSA__ff_scan_controller.sv']
    # Every scan hold/fault contribution is a positive-edge register. All
    # owner/group/coordinator sequential blocks use that same edge as well.
    clocks={}
    for n,text in {**assets,'ff_scan_controller.sv':controller}.items():
        events=re.findall(r'\balways\s*@\s*\(([^)]+)\)',masked_comments(text))
        assert events and all(e.strip()=='posedge clk'for e in events),(n,events)
        clocks[n]=events
    for n,text in sources.items():
        if n.startswith(PREFIX)and n.endswith('__ff_scan_controller.sv'):
            normalized=re.sub(r'module dorado_serial_\w+__dorado_ff_scan_controller\b',
                'module dorado_ff_scan_controller',text)
            assert normalized==controller,n
    assets.update({'ff_scan_controller.sv':controller,'board_phase_enable.sv':(HERE/'board_phase_enable.sv').read_text(),
                   'fixture.sv':top})
    return assets,dict(boards=19,board_equations_checked=checked,clock_events=clocks,
        argument=['Final hold depends only on positive-edge owner/group/scan/coordinator state.',
                  'Those values remain stable from the permission falling edge until the next capturing edge.',
                  'A definite enabled permission requires final hold zero; every child hold/fault is included fail-closed.',
                  'The scan controller can assert scan_step only when freeze and guest_held are both definite one; board hold is then one.'],
        limits='Control cone with nineteen 38-bit scan leaf models; complete native physical payload/SDRAM integration, access and timing remain open.')


def run(out):
    out.mkdir(parents=True,exist_ok=False);sources,record=frozen();assets,review=fixture(sources)
    inputs=out/'executed-inputs';inputs.mkdir()
    for n,text in assets.items():(inputs/n).write_text(text)
    for name in ('core/Dorado.sv',native.WRAPPER,native.BACKPLANE,*review['board_equations_checked']):
        p=out/'native-source'/name;p.parent.mkdir(parents=True,exist_ok=True);p.write_text(sources[name])
    for p in (Path(__file__),HERE/'test_native_phase_domain.sv'):(inputs/p.name).write_bytes(p.read_bytes())
    cases=[('candidate',None,None,None,None),
        ('wrong_phase','board_phase_enable.sv','always @(negedge clk)local_step',
         'always @(posedge clk)local_step','native local phase differs'),
        ('missing_hold_gate','fixture.sv','copies(.clk(clk),.permit(machine_enable)',
         'copies(.clk(clk),.permit(~reset && pack_ready && storage_initialized && snapshot_frontends_cached)',
         'native local phase differs'),
        ('late_combinational_hold','fixture.sv','assign snapshot_service_hold=snapshot_network_hold;',
         'assign snapshot_service_hold=snapshot_network_hold || late_hold;',
         'native hold changed after falling edge')]
    results=[]
    for label,target,before,after,expected in cases:
        case=out/label;case.mkdir();paths={n:inputs/n for n in assets}
        if target:
            paths[target]=case/target;paths[target].write_text(once(assets[target],before,after))
        cmd=['iverilog','-g2012','-s','tb','-o',str(case/'test.vvp'),*map(str,paths.values())]
        (case/'commands.json').write_text(json.dumps([cmd,['vvp',str(case/'test.vvp')]],indent=2)+'\n')
        with(case/'build.log').open('w')as log:subprocess.run(cmd,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run(['vvp',str(case/'test.vvp')],capture_output=True,text=True,timeout=120)
        text=result.stdout+result.stderr;(case/'test.txt').write_text(text);print(label+': '+text[:1600].strip(),flush=True)
        if expected:assert result.returncode and expected in text,text
        else:assert not result.returncode and'NATIVE PHASE DOMAIN PASS'in text,text
        results.append(dict(case=label,returncode=result.returncode,expected_failure=expected))
        if not expected:
            # Fresh simulation is a genuine RBF initialization. The original
            # Core ties coordinator reseed low: a parser/system reset must not
            # be misrepresented as recovery from its sticky quarantine.
            random=subprocess.run(['vvp',str(case/'test.vvp'),'+random'],capture_output=True,text=True,timeout=120)
            text=random.stdout+random.stderr;(case/'random-test.txt').write_text(text);print(label+' random: '+text.strip(),flush=True)
            assert not random.returncode and 'NATIVE PHASE DOMAIN PASS'in text,text
            results.append(dict(case='fresh_random',returncode=random.returncode))
    (out/'results.json').write_text(json.dumps(dict(status='PASS_NATIVE_CONTROL_DOMAIN',scope=__doc__,review=review,cases=results,
        verified_native_sources=record['source_hashes'],compiled_inputs={p.name:hashlib.sha256(p.read_bytes()).hexdigest()for p in inputs.iterdir()}),indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args();run(a.output.resolve())
