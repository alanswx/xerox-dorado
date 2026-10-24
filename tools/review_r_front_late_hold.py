#!/usr/bin/env python3
"""Preserve late Hold selection proofs, limitations and real ROM-delay progress.

Only the component transfer is under test. No new clock, native fit, FPGA
image, machine cadence or Cedar performance claim follows from this receipt.
Keep earlier failed/limited attempts and the rejected CP timing profile.
"""
import argparse
import gzip
import json
from pathlib import Path
import re

from fit_r_front_late_hold import gate
from probe_r_front_late_hold import candidate,pure_sources
from review_r_front_capture_followup import append_archive
from review_r_front_cp_subevents import observation
from test_r_front_forward_view import sha

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'


def read(p):return json.loads(p.read_text())


def hierarchy(d,r):
    mods={}
    paths=[d/v['path']for v in r['source_files']if Path(v['path']).name!='tb_r_front_local.sv'
           and not v['path'].startswith('timed/')]+[d/'late_hold_forecasts.sv']
    for f in paths:
        for name,body in re.findall(r'^module\s+(\w+)\b(.*?)^endmodule',f.read_text(),re.M|re.S):
            assert name not in mods,name
            mods[name]=body
    children={}
    for name,body in mods.items():
        children[name]=[(t,n)for t,n in re.findall(r'^\s*(\w+)\s+(?:#\s*\([^;]*?\)\s*)?(\w+)\s*\(',body,re.M)if t in mods]
    def walk(name,path):
        result=[(name,path)]
        for t,n in children[name]:result+=walk(t,path+'/'+n)
        return result
    all_instances=walk('dorado_r_front_control_step','dut')
    files=[p for n,p in all_instances if n=='dorado_r_file_ports']
    assert files==['dut/child/child/child/files']
    forecasts=walk('dorado_late_hold_forecast_step','forecast')
    assert all(n!='dorado_r_file_ports'for n,p in forecasts)
    for n,p in forecasts:
        assert not re.search(r'\b(?:always\s*@\s*\(\s*(?:posedge|negedge)|initial\b)',mods[n]),n
    return dict(reachable_instances=len(all_instances),physical_R_file_owners=files,
                forecast_instances=len(forecasts),forecast_sequential_processes=0,forecast_RAM_owners=0)


def review(out):
    assert not out.exists();out.mkdir(parents=True);dirs=[];gates=[];structures=[];negatives=[]
    for engine in ['icarus','verilator']:
        d=BUILD/f'r-front-late-hold-all-{engine}-v3-20261023';r,g=gate(d,engine)
        gates.append(g);structures.append(hierarchy(d,r));dirs.append(d)
        neg=BUILD/f'r-front-late-hold-all-inverted-{engine}-v3-20261023';n=read(neg/'run-state.json')
        assert n['status']=='LATE_HOLD_COFACTOR_FAILED' and n['mutation'] and n['all_owner_bits_selected']
        assert n['actions'][0]['returncode']==0 and n['actions'][-1]['returncode']!=0
        assert (neg/'candidate.sv').read_text()==candidate((Path(n['parent'])/'candidate.sv').read_text(),True,True)
        log=(neg/'test.log').read_text()
        assert 'R_FRONT_DIFF bit=840'in log and 'R front state mismatch check=15'in log
        assert sha(neg/'late_hold_forecasts.sv')==g['pure_forecasts_sha256']
        negatives.append(dict(engine=engine,directory=neg.name,state_sha256=sha(neg/'run-state.json'),
                              independent_original_rejection='check15 / physical Q840'));dirs.append(neg)
    assert gates[0]['candidate_sha256']==gates[1]['candidate_sha256']
    assert gates[0]['pure_forecasts_sha256']==gates[1]['pure_forecasts_sha256']
    assert gates[0]['coverage']==gates[1]['coverage'] and gates[0]['clocks']==gates[1]['clocks']
    history=[]
    for name in ['r-front-late-hold-icarus-v1-20261023','r-front-late-hold-icarus-v2-20261023',
                 'r-front-late-hold-verilator-v1-20261023','r-front-late-hold-inverted-verilator-v2-20261023']:
        d=BUILD/name;r=read(d/'run-state.json');assert r['status']!='RUNNING'
        assert all('returncode'in a for a in r['actions'])
        if name=='r-front-late-hold-icarus-v1-20261023':
            assert r['actions'][0]['returncode']!=0 and 'Unable to bind wire/reg/memory `checks\''in(d/'compile.log').read_text()
        elif 'inverted'in name:
            assert r['status']=='LATE_HOLD_COFACTOR_FAILED'
            assert 'Late Hold selected full-state forecast mismatch check=15'in(d/'test.log').read_text()
        else:
            assert r['status']=='PASS_LATE_HOLD_COFACTOR' and 'distinct=0 bits=00'in r['cofactor_coverage']
        history.append(dict(directory=name,status=r['status'],state_sha256=sha(d/'run-state.json')));dirs.append(d)
    # Read-only observations establish actual Delay progress, not a CPU rate.
    inspections=[]
    for version in [2,3]:
        d=BUILD/f'native-cedar-queue-live-delay-riot-v{version}-20261023';r=read(d/'result.json')
        assert r['pid']==29618 and r['read_only_guest_inspection']
        assert 'Process 29618 detached'in(d/'lldb.log').read_text()
        image=(d/'baseboard-riot-ram.bin').read_bytes();assert len(image)==640
        assert sha(d/'baseboard-riot-ram.bin')==r['riot_ram']['image_sha256']
        assert image[0x9f]|image[0xa0]<<8==30
        inspections.append(dict(directory=d.name,guest_ticks=r['counters']['guest_ticks'],
                                timer=image[0x52]|image[0x53]<<8,target=30,
                                result_sha256=sha(d/'result.json'),binary_sha256=r['binary_sha256'],
                                header_sha256=r['header_sha256']));dirs.append(d)
    assert inspections[0]['binary_sha256']==inspections[1]['binary_sha256']
    assert [r['timer']for r in inspections]==[25,27]
    assert inspections[1]['guest_ticks']>inspections[0]['guest_ticks']
    latest=read(dirs[-1]/'result.json');rom=latest['riot_ram']['compiled_delay_operands']
    native=BUILD/'native-cedar-queue-skip-wait-v1-20261023'
    eprom=native/rom['eprom'];assert sha(eprom)==rom['sha256']
    (out/'BaseBd-c61.mem').write_bytes(eprom.read_bytes())
    wrong=BUILD/'native-cedar-queue-live-delay-ram-v1-20261023'
    wrong_r=read(wrong/'result.json');assert wrong_r['low_ram']['bank']=='RS0'
    assert wrong_r['low_ram']['timer100ms_word']==wrong_r['low_ram']['EndOfDelay_word']==0
    dirs+=[wrong,BUILD/'native-cedar-queue-live-read-v2-20261023']
    # Bind the candidate's selected signal to the real cache/ref-hold source.
    board=native/'inputs/src/fpga/mister/snapshot_native_memc/MemC_full_snapshot.sv'
    provenance=json.loads(gzip.decompress((native/'provenance.json.gz').read_bytes()))
    n=str(board.relative_to(native/'inputs'));assert sha(board)==provenance['inputs'][n]
    text=board.read_text();m=re.search(r'^  cell_MC10231_snapshot u_e22 \(\n(.*?)^  \);',text,re.M|re.S);assert m
    pins=dict(re.findall(r'\.p(\d+)\(([^()]*)\)',m[1]));assert pins['7']=='MemC15_sil_pl_2' and pins['2']=='RefHold_p_'
    (out/'native-MemC.sv').write_bytes(board.read_bytes())
    rejected=BUILD/'r-front-cp-subevents-matrix-v1-20261023/fast_gates-icarus'
    assert read(rejected/'run-state.json')['status']=='CP_BEFORE_ROOT_SUBEVENTS_FAILED'
    assert 'R front state mismatch check=12839'in(rejected/'test.log').read_text()
    observations=[]
    for kind in ['5c65','queue']:
        d=BUILD/f'native-cedar-{kind}-skip-wait-v1-20261023';obs=out/('observed-native-'+kind)
        observations.append(observation(d,obs));assert int((d/'host-cycle-limit.txt').read_text())==5000000000
        for n in ['provenance.json.gz','host-cycle-limit.txt','window-control.txt']:(obs/n).write_bytes((d/n).read_bytes())
    result=dict(status='REVIEWED_LATE_HOLD_COMPONENT_SELECTION_AND_ROM_DELAY_PROGRESS',scope=__doc__,
                component_gates=gates,physical_hierarchies=structures,negative_controls=negatives,
                history=history,extra_guest_cycles=0,extra_pipeline_registers=0,
                eight_flag_derivative_coverage=0,selected_full_state_distinct_checks=2709,
                observed_full_state_distinct_bits=[840],full_domain_derivative_proven=False,
                eight_flag_influence_or_speedup_claimed=False,
                live_delay_observations=inspections,compiled_delay_operands=rom,
                rejected_RAM_interpretation=dict(directory=wrong.name,
                    reason='RS0 is EXTRA SRAM, not RIOT zero page. Its zero values say nothing about Timer100ms or EndOfDelay.'),
                no_inspected_guest_state_changed=True,original_native_runs_restarted=False,
                native_observations=observations,CP_profile_rejection_retained=True,
                assembly_permitted=False,hardware_changed=False,native_qualified=False,speedup_qualified=False)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result,dirs


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--output',type=Path,required=True);p.add_argument('--archive',type=Path,required=True)
    a=p.parse_args();r,dirs=review(a.output.resolve())
    append_archive(a.archive.resolve(),a.output.resolve(),dirs,label='late-hold-selection',extra_tools=[
        'probe_r_front_late_hold.py','fit_r_front_late_hold.py','inspect_native_baseboard_process.py',Path(__file__).name])
    print(r['status'],flush=True)
