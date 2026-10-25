#!/usr/bin/env python3
"""Read real native ALU phase records without changing the running machine.

Compare the passive prediction with the original cells' observed80 physical
flag bits, retaining exact source records and uncovered clock/package cases.
Endpoint clock transitions are observations, not proof of every internal pulse
or of arbitrary physical source arrivals. Never extend/restart the source run.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path
import re
import subprocess
import time

from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1]
PACKAGES={'e13_task_file':(0,64),'f17_visible_flags':(64,4),'g11':(68,6),'h11':(74,6)}
WIDTHS={'before':1198,'after':1198,'pins':432,'before_flags':38,'after_flags':38,
        'early':38,'predicted':80,'events':4}


def analyze(run,out):
    assert not out.exists();out.mkdir(parents=True)
    state=json.loads((run/'run-state.json').read_text())
    provenance=json.loads(gzip.decompress((run/'provenance.json.gz').read_bytes()))
    verify(run/'inputs',provenance['inputs'])
    assert len(provenance['used_hdl'])==198 and not provenance['new_flag_owner_selected']
    assert int((run/'host-cycle-limit.txt').read_text())==600000000
    observation=dict(observed=time.time(),state=state,provenance_sha256=sha(run/'provenance.json.gz'))
    if state['status']=='RUNNING':
        action=state['actions'][-1];assert action['stage']=='test'and'returncode'not in action
        ps=subprocess.run(['ps','-p',str(action['pid']),'-o','pid=,etime=,time=,stat=,command='],capture_output=True,text=True)
        if str(run)not in ps.stdout:
            again=json.loads((run/'run-state.json').read_text())
            assert again['status']!='RUNNING',(ps.stdout,ps.stderr)
            state=again;observation['state']=again
        else:observation.update(verified_live=True,pid=action['pid'],process=ps.stdout.strip())
    # Retain a complete-line prefix. Later appends cannot change its meaning.
    raw=(run/'test.log').read_bytes();prefix=raw[:raw.rfind(b'\n')+1]
    lines=prefix.decode().splitlines();records=[];source=[]
    for line in lines:
        if not line.startswith('NATIVE_ALU_PHASE_RECORD '):continue
        fields=dict(re.findall(r'(\w+)=([^ ]+)',line))
        assert set(fields)=={'index','host','guest','source','flag_clock',*WIDTHS,'unsettled'},fields
        row={n:int(fields[n])for n in ['index','host','guest']}
        assert row['index']==len(records)
        for n,width in WIDTHS.items():
            assert re.fullmatch('[0-9a-fA-F]+',fields[n]),(n,fields[n])
            value=int(fields[n],16);assert value<1<<width,(n,value)
            row[n]=value
        for n in ['source','flag_clock']:
            assert re.fullmatch('[01]/[01]',fields[n]),(n,fields[n]);row[n]=fields[n]
        assert fields['unsettled']in ['0','1'];row['unsettled']=int(fields['unsettled'])
        if records:assert row['host']>records[-1]['host']and row['guest']>=records[-1]['guest']
        records.append(row);source.append(line)
    assert len(records)<=128
    (out/'native-phase-records.log').write_text('\n'.join(source)+('\n'if source else''))
    (out/'run-state-at-observation.json').write_text(json.dumps(state,indent=2)+'\n')
    (out/'provenance.json.gz').write_bytes((run/'provenance.json.gz').read_bytes())
    progress=next((line for line in reversed(lines)if line.startswith('NATIVE_PROGRESS ')),None)
    observation.update(log_prefix_bytes=len(prefix),log_prefix_sha256=hashlib.sha256(prefix).hexdigest(),last_progress=progress)
    (out/'process-observation.json').write_text(json.dumps(observation,indent=2)+'\n')
    transitions={n:{key:0 for key in ['0/0','0/1','1/0','1/1']}for n in ['source','flag_clock']}
    events=[0]*4;changed={n:0 for n in PACKAGES};pin_changes=[0]*38
    mismatch=[];continuity=[];unsettled=[];actual_changes=predicted_changes=0
    mask=(1<<80)-1
    for index,row in enumerate(records):
        before=(row['before']>>844)&mask;actual=(row['after']>>844)&mask;predicted=row['predicted']
        diff=actual^predicted
        for n in transitions:transitions[n][row[n]]+=1
        for k in range(4):events[k]+=bool(row['events']>>k&1)
        for k in range(38):pin_changes[k]+=bool((row['early']^row['after_flags'])>>k&1)
        for n,(offset,width)in PACKAGES.items():changed[n]+=bool(((before^actual)>>offset)&((1<<width)-1))
        actual_changes+=before!=actual;predicted_changes+=before!=predicted
        if row['unsettled']:unsettled.append(index)
        if index and records[index-1]['after']!=row['before']:
            continuity.append(dict(index=index,host_gap=row['host']-records[index-1]['host'],
                                   guest_gap=row['guest']-records[index-1]['guest']))
        if diff:
            mismatch.append(dict(index=index,host=row['host'],guest=row['guest'],source=row['source'],flag_clock=row['flag_clock'],
                physical_before=f'{before:020x}',physical_after=f'{actual:020x}',predicted=f'{predicted:020x}',
                differing_bits=[k for k in range(80)if diff>>k&1],
                differing_packages={n:[k for k in range(width)if diff>>(offset+k)&1]
                                    for n,(offset,width)in PACKAGES.items()if(diff>>offset)&((1<<width)-1)}))
    if not records:status='NO_NATIVE_ALU_PHASE_RECORDS_YET'if observation.get('verified_live')else'TERMINAL_NATIVE_ALU_PHASE_RECORDS_MISSING'
    else:status='OBSERVED_NATIVE_ALU_PHASE_PREFIX_MISMATCH'if mismatch else'OBSERVED_NATIVE_ALU_PHASE_PREFIX_MATCH'
    result=dict(status=status,scope=__doc__,run=str(run),observation=observation,
        expected_records=128,observed_records=len(records),all128_records_observed=len(records)==128,
        actual_original_cell_flag_states_compared=True,physical_flag_bits=80,physical_flag_offset=844,
        mismatches=mismatch,matching_records=len(records)-len(mismatch),unsettled_records=unsettled,
        source_and_flag_root_transitions=transitions,candidate_event_counts=events,
        uncovered_candidate_clock_channels=[k for k,n in enumerate(events)if not n],
        actual_changed_records=actual_changes,predicted_changed_records=predicted_changes,
        original_package_change_counts=changed,unexercised_original_packages=[n for n,count in changed.items()if not count],
        early_vs_late_flag_pin_change_counts=pin_changes,noncontinuous_physical_frames=continuity,
        native_source_arrival_contract_qualified=False,all_internal_clock_pulses_observed=False,
        native_flag_owner_selected=False,firmware_or_FPGA_timing_or_speed_qualified=False,
        source_inputs_or_limit_changed=False,hardware_or_media_changed=False,full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n')
    print(status,'records',len(records),'mismatches',len(mismatch),flush=True);return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['run','output']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();analyze(a.run.resolve(),a.output.resolve())
