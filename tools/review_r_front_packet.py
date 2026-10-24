#!/usr/bin/env python3
"""Preserve original-oracle single-commit packets, corruptions and full fit.

Validate every trace input/owner link and all original per-bank memory updates,
not only a passing marker. Keep both full subevent results and the exact source
conversion failures. Scope timing to this processor/one-SRAM component. Actual
clock-source/input epochs, peripherals, native60ns and all full-goal gates stay
open; the FPGA remains reserved until the user releases it.
"""
import argparse
import json
from pathlib import Path
import re

from review_baseboard_clock_consumer import collection,CORNERS
from review_exact_pixel_phase import retain_text
from review_late_hold_and_input_reset import first_path
from review_r_front_capture_followup import append_archive
from test_r_front_forward_view import sha
from test_r_front_packet import packet
from trace_r_front_transactions import port_map,layout

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'
OFFSETS=[(0,1024,4096),(2048,3072,4352),(4608,5632,8704),(6656,7680,8960)]


def read(p):return json.loads(p.read_text())


def verify(d,files):
    for n,h in files.items():assert sha(d/n)==h,(d,n)


def trace_audit(trace):
    s=read(trace/'run-state.json');assert s['status']=='PASS_FULL_ORIGINAL_PROCESSOR_TRANSACTION_TRACE' and s['records']==14884
    assert s['original_assertions_and_references_retained'] and all(v['returncode']==0 for v in s['actions'])
    assert sha(trace/'transactions.hex')==s['trace_sha256'] and sha(trace/'initial.hex')==s['initial_sha256']
    for row in s['source_files']:assert sha(trace/row['path'])==row['output_sha256']
    parent=Path(s['source']);assert sha(parent/'run-state.json')==s['source_state_sha256']
    # The source fixture remains in order, with only passive insertions.
    lines=iter((trace/'inputs/tb_r_front_local.sv').read_text().splitlines())
    assert all(any(x==y for y in lines) for x in (parent/'inputs/tb_r_front_local.sv').read_text().splitlines())
    value=int((trace/'initial.hex').read_text(),16);memory=value&((1<<9216)-1);owner=value>>9216
    last_pins=None;last_root=None;writes=0;rows=[]
    fields=s['result_layout']
    def field(result,name):
        f=fields[name];return (result>>f['low'])&((1<<f['bits'])-1)
    for index,line in enumerate((trace/'transactions.hex').read_text().splitlines()):
        v=int(line,16);after_memory=v&((1<<9216)-1);v>>=9216;result=v&((1<<2470)-1);v>>=2470
        pa=v&((1<<401)-1);v>>=401;pb=v&((1<<401)-1);v>>=401;ca=v&1;v>>=1;cb=v&1;old_owner=v>>1
        assert old_owner==owner,(index,'owner chain')
        if last_root is not None:assert cb==last_root and pb==last_pins,(index,'pin/root chain')
        addr=field(result,'port_address');select=field(result,'port_select_n');wr=field(result,'r_write_n');data=field(result,'file_write_data')
        words=[]
        for bank,(lo,hi,parity) in enumerate(OFFSETS):
            a=(addr>>(8*bank))&255
            words.append(((memory>>(lo+4*a))&15)|(((memory>>(hi+4*a))&15)<<4)|(((memory>>(parity+a))&1)<<8))
        h=(0 if select&1 else words[0])|(0 if select&2 else words[1]);l=(0 if select&4 else words[2])|(0 if select&8 else words[3])
        assert field(result,'file_read_data')==((l&255)<<8)|(h&255),(index,'read before write data')
        assert field(result,'file_read_parity')==((l>>8)<<1)|(h>>8),(index,'read before write parity')
        for bank,(lo,hi,parity) in enumerate(OFFSETS):
            half=bank//2
            if wr&(1<<half) or select&(1<<bank):continue
            a=(addr>>(8*bank))&255;byte=(data>>(8*half))&255;odd=1^(byte.bit_count()&1)
            mask=(15<<(lo+4*a))|(15<<(hi+4*a))|(1<<(parity+a))
            memory=(memory&~mask)|((byte&15)<<(lo+4*a))|((byte>>4)<<(hi+4*a))|(odd<<(parity+a));writes+=1
        assert memory==after_memory,(index,'full9216-bit write transition')
        owner=field(result,'held_next');last_pins=pa;last_root=ca
        rows.append((cb,ca,pb,pa,wr))
    candidates=paired_writes=0;index=0
    while index<len(rows):
        a=rows[index];b=rows[index+1] if index+1<len(rows) else None
        if b and a[:2]==(1,0) and b[:2]==(0,1) and a[2]==a[3]==b[2]==b[3]:
            candidates+=1;paired_writes+=a[4]!=3;index+=2
        else:index+=1
    assert len(rows)==14884 and candidates==4461 and paired_writes==119 and writes==238
    return dict(records=len(rows),eligible_pairs=candidates,paired_write_events=paired_writes,bank_writes=writes,
        all_input_owner_chains_verified=True,all_read_before_write_words_verified=True,all9216_RAM_bits_per_record_verified=True)


def review(out):
    assert not out.exists();dirs=[];receipts=[]
    trace=BUILD/'processor-original-trace-v2-20261024';audit=trace_audit(trace);dirs.append(trace)
    failed=BUILD/'processor-original-trace-v1-20261024';f=read(failed/'run-state.json')
    assert f['status']=='TERMINAL_PROCESSOR_TRACE_NOT_ESTABLISHED' and f['actions'][0]['returncode']!=0
    assert 'Exceeded limit of 8192 bits' in (failed/'compile.log').read_text();dirs.append(failed)
    expected='transactions=14884 commits=10423 pairs=4461 candidates=4461 fallbacks=0 paired_writes=119 forwarded_bytes=238 Qs=844 RAM=9216 results=2470 clocks=100'
    originals=[]
    for name in ['processor-packet-icarus-v1-20261024','processor-packet-verilator-v1-20261024','processor-packet-verilator-v2-20261024']:
        d=BUILD/name;s=read(d/'run-state.json');assert s['status']=='PASS_FULL_PROCESSOR_PACKET_SINGLE_COMMIT' and s['summary']==expected
        assert all(v['returncode']==0 for v in s['actions']) and s['trace_state_sha256']==sha(trace/'run-state.json');verify(d,s['files'])
        for row in s['source_files']:assert sha(d/row['path'])==row['output_sha256']
        assert (d/'packet.sv').read_text()==packet((d/'candidate.sv').read_text())
        log=(d/'test.log').read_text();assert sha(d/'test.log')==s['test_log_sha256']
        counts=re.findall(r'^PACKET_CLOCK channel=(\d+) edges=(\d+)$',log,re.M);assert len(counts)==100
        originals.append(counts)
        if s.get('geometry_checks'):assert 'PACKET_GEOMETRY_REJECTIONS cases=403' in log
        dirs.append(d)
    assert originals[0]==originals[1]==originals[2]
    for mutation,check,message in [('early-write',203,'PACKET hold changed memory/owner'),('drop-forward',203,'PACKET second result mismatch'),('stale-intermediate',16,'PACKET second result mismatch')]:
        d=BUILD/f'processor-packet-{mutation}-icarus-v1-20261024';s=read(d/'run-state.json')
        assert s['status']=='PASS_FULL_PACKET_COUNTEREXAMPLE' and s['actions'][0]['returncode']==0 and s['actions'][-1]['returncode']!=0
        assert f'{message} record={check}' in (d/'test.log').read_text();verify(d,s['files'])
        assert (d/'packet.sv').read_text()==packet((d/'candidate.sv').read_text(),mutation);dirs.append(d)
    proof=BUILD/'processor-forward-proof-v1-20261024';p=read(proof/'run-state.json')
    assert p['status']=='PASS_ORIGINAL_BANK_WRITE_FORWARDING' and len(p['cases'])==3 and p['compared_read_bits']==18
    assert p['source_sha256']==sha(ROOT/'verilog/rtl/dorado_r_pair_read_forward.sv')
    assert sha(proof/'original-file-ports.sv')==p['original_RAM_source_sha256']==sha(BUILD/'processor-packet-verilator-v2-20261024/inputs/verilog/rtl/dorado_r_file_ports.sv')
    for row in p['cases']:
        d=proof/row['variant'];verify(d,row['files']);assert row['passed'] and row['returncode']==0 and sha(d/'proof.log')==row['log_sha256']
    dirs.append(proof)
    for version in [1,2]:
        d=BUILD/f'processor-packet-stage-v{version}-20261024';s=read(d/'provenance.json');verify(d,s['files'])
        assert s['result_bits']==4943 and s['input_bits']==1651 and not s['assembly_permitted'];dirs.append(d)
    bad,receipt,record=collection('processor-packet-terminal-v1-20261024')
    assert len(record['members'])==46 and read(bad/'run-state.json')['status']=='TERMINAL_FAILED'
    assert 'genvar' in (bad/'map.log').read_text() and 'near text: "for"' in (bad/'map.log').read_text()
    dirs.append(bad);receipts.append(receipt)
    fit,receipt,record=collection('processor-packet-terminal-v2-20261024');s=read(fit/'run-state.json')
    assert s['status'] in ['TERMINAL_REVIEW_REQUIRED','TERMINAL_FAILED'];dirs.append(fit);receipts.append(receipt)
    # Independently bind the collected synthesis inputs to both immutable stages.
    for version,terminal in [(1,bad),(2,fit)]:
        stage=BUILD/f'processor-packet-stage-v{version}-20261024';p=read(stage/'provenance.json')
        assert read(terminal/'provenance.json')==p
        annotation=b'\nset_global_assignment -name LAST_QUARTUS_VERSION "17.0.2 Lite Edition"'
        actual={}
        for name,digest in p['files'].items():
            data=(terminal/name).read_bytes();actual[name]=sha(terminal/name)
            if actual[name]!=digest:
                assert name=='clock_fit.qsf' and data.endswith(annotation)
                assert data[:-len(annotation)]==(stage/name).read_bytes()
        assert read(terminal/'collected-input-check.json')['actual']==actual
    corners=[];physical_pass=False;area={}
    if s['status']=='TERMINAL_REVIEW_REQUIRED':
        assert all(v['returncode']==0 for v in s['stages'])
        assert re.findall(r'PROCESSOR_PACKET_OUTPUT_BITS (\d+)',(fit/'corners.log').read_text())==['4943']*4
        for corner in CORNERS:
            row=dict(corner=corner,setup=first_path(fit/f'clock-setup-{corner}.rpt'),hold=first_path(fit/f'clock-hold-{corner}.rpt'))
            ucp=re.findall(r'; (?:Illegal Clocks|Unconstrained [^;]+)\s*;\s*(\d+)\s*;\s*(\d*)\s*;', (fit/f'clock-ucp-{corner}.rpt').read_text())
            assert len(ucp)==6 and all(a=='0' and b in ['','0'] for a,b in ucp)
            mpw=re.search(r'Worst case slack is (-?[0-9.]+)',(fit/f'clock-mpw-{corner}.rpt').read_text())
            assert mpw;row['minimum_pulse_width_ns']=float(mpw[1])
            for mode in ['recovery','removal']:
                text=(fit/f'clock-{mode}-{corner}.rpt').read_text()
                assert 'Nothing to report.' in text
            row['evaluator']=first_path(fit/f'clock-evaluator-{corner}.rpt')
            corners.append(row)
        physical_pass=all(v[k]['slack_ns']>=0 for v in corners for k in ['setup','hold']) and all(v['minimum_pulse_width_ns']>=0 for v in corners)
        summary=(fit/'clock_fit.fit.summary').read_text()
        area=dict(ALMs=int(re.search(r'Logic utilization \(in ALMs\) : ([0-9,]+)',summary)[1].replace(',','')),
            registers=int(re.search(r'Total registers : ([0-9,]+)',summary)[1].replace(',','')),
            MLAB_memory_bits=int(re.search(r'Total MLAB memory bits\s*;\s*([0-9,]+)',(fit/'clock_fit.fit.rpt').read_text(encoding='latin1'))[1].replace(',','')))
        assert area==dict(ALMs=8665,registers=7189,MLAB_memory_bits=9216)
    result=dict(status='REVIEWED_FULL_PROCESSOR_SINGLE_COMMIT_PACKETS',trace_audit=audit,
        transactions=14884,commits=10423,paired_packets=4461,paired_writes=119,forwarded_bytes=238,
        all844_Qs_9216_RAM_2470_results_100_clocks_verified=True,both_simulators_pass=True,
        invalid_geometry_rejections=403,corruptions_rejected_at_records=[203,203,16],
        forwarding_bits_proven=18,all_bank_selections_and_word_values_arbitrary=True,
        complete_component_15ns_pass=physical_pass,component_corners=corners,component_fit_state=s['status'],component_area=area,
        earlier_display_width_and_Quartus_parser_failures_retained=True,
        source_and_CP_device_timestamps_qualified=False,whole_native_timing_or_real60ns_qualified=False,
        hardware_speed_qualified=False,hardware_changed=False,hardware_requires_user_release=True,full_goal_active=True)
    out.mkdir(parents=True)
    for d in dirs:retain_text(d,out/d.name)
    for p in receipts:(out/p.name).write_bytes(p.read_bytes())
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--archive',type=Path,required=True)
    a=p.parse_args();r=review(a.output.resolve());append_archive(a.archive.resolve(),a.output.resolve(),[],label='processor-packet',extra_tools=[
        'trace_r_front_transactions.py','test_r_front_packet.py','prove_r_pair_forward.py','fit_r_front_packet.py',Path(__file__).name]);print(r['status'],flush=True)
