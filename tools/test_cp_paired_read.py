#!/usr/bin/env python3
"""Check the combined caller's single fallback against the original processor.

Retain every original event/state/RAM/clock assertion. The same corrected pure
and cached top modules must be byte-identical to the qualified CP fixture.
Replace its cached invocation/read caller with the real combined packet owner.
Pair selection is OFF in this fixture; do not claim paired qualification here.
The actual16 visible-R cell hookup remains external to its fixture model.
"""
import argparse
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import time

from generate_cp_paired_read import processor,caller
from generate_native_r_capture import generate as native_cells,expose,expose_caller
from prove_cp_prefix_cache import module
from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1]


def bench(raw,out,payload_probe=False,native_visible=False):
    original=raw;edits=[]
    def change(a,b):
        nonlocal raw
        assert raw.count(a)==1,a
        raw=raw.replace(a,b,1);edits.append((a,b))
    start=raw.index(' dorado_r_front_control_step dut(')
    end=raw.index(');',start)+2
    old=raw[start:end]
    start_owner=raw.index(' dorado_cp_read_caller owner(')
    end_owner=raw.index(');',start_owner)+2
    old_owner=raw[start_owner:end_owner]
    pack='n,events,repeated,unsettled,wn,ln,tln,bp,ok,fault,rr,rv,rf,rf_n,address,select_n,forwarded,rwrite,port_address,port_select,raw_data,raw_parity,source_pins,fa_n,ready_flags,conta_next,shared_mismatch,control_flags,q,file_write_data'
    addition=''' wire [2469:0] first_result,last_result;
 wire [1:0] packet_forwarded;
 reg payload_ready=1;
 assign {PACK}=first_result;
 assign pb=owner.r_front_before;assign cb=owner.r_front_clock_before;
 assign cp_ready=owner.packet.entry_ready;
 assign external_file_read=owner.before_data;assign stage_capture=owner.capture_data;
 assign interior_address=owner.interior_address;assign interior_select=owner.interior_select;
 assign physical_address=owner.physical_address;assign physical_select=owner.physical_select;
 // Test-only adversarial producer request. The hardware commit guard must
 // protect RAM even if a write enable is asserted while the read is pending.
 always @*begin
  if(untrusted_write)force owner.memory_write_n=2'b00;
  else release owner.memory_write_n;
 end
'''.replace('PACK',pack)
    change(old,addition)
    change(old_owner,''' dorado_r_front_owned_packet owner(host_clk,advance,1'b0,ca,1'b1,cp_reset,pa,visible_h,visible_l,
  first_result,last_result,owner_ready,packet_forwarded,stored_q,read_accept,payload_ready,
  interior_needed,stage_active,stage_select,stage_valid,stage_busy,stage_tag_match,owner_commit);''')
    if native_visible:
        change('reg [7:0] visible_h=0,visible_l=0;', '''wire [7:0] visible_h,visible_l;
 wire [35:0] native_R_frame;wire [15:0] native_visible;
 dorado_native_r_capture visible_cells(host_clk,owner_commit,native_R_frame,native_visible);
 assign visible_h=native_visible[7:0];assign visible_l=native_visible[15:8];''')
        change('stage_busy,stage_tag_match,owner_commit);','stage_busy,stage_tag_match,owner_commit,native_R_frame);')
        change(''' always @(posedge host_clk)if(owner_commit)begin
  visible_h<=n[579:572];visible_l<=n[587:580];
 end''',' // The four original native cells, not a predicted-result register, own visible R.')
        change('visible_h=fixture[579:572];visible_l=fixture[587:580];', '''visible_cells.high.u_g06.q=fixture[575:572];visible_cells.high.u_g07.q=fixture[579:576];
   visible_cells.low.u_g06.q=fixture[583:580];visible_cells.low.u_g07.q=fixture[587:584];''')
    if '#1; // Settle the held-wire view' not in raw:
        marker='visible_h=fixture[579:572];visible_l=fixture[587:580];'
        change(marker,marker+'\n   #1; // Settle the held-wire view before recording the initial fixture.')
    if payload_probe:
        marker='     end else if(owner_ready!==cp_ready)$fatal(1,"READ_OWNER delayed an ordinary event");'
        change(marker,marker+'''
     if(checks%41==0 || interior_needed)begin: check_payload_gate
      reg [843:0] saved_owner;reg [400:0] saved_before;reg saved_root;
      reg [9215:0] saved_memory;
      saved_owner=stored_q;saved_before=pb;saved_root=cb;saved_memory=memory_image();
      payload_ready=0;advance=1;#100;
      repeat(3)begin
       if(owner_ready || owner_commit)$fatal(1,"PAYLOAD_GATE accepted invalid payload");
       host_clk=1;#100;host_clk=0;#100;
       if(stored_q!==saved_owner || pb!==saved_before || cb!==saved_root || memory_image()!==saved_memory)
        $fatal(1,"PAYLOAD_GATE invalid payload changed owner/context/RAM");
      end
      payload_ready=1;#100;
      if(owner_ready!==cp_ready)$fatal(1,"PAYLOAD_GATE did not release a complete transaction");
      $display("PAYLOAD_GATE_PROBE check=%0d staged=%b",checks,interior_needed);
     end
''')
    # Only the hierarchical observer root changes. All comparisons and
    # reference expressions stay, including the original capture probes.
    names=sorted(set(re.findall(r'dut\.\w+',raw)),key=len,reverse=True)
    for name in names:
        suffix=name.split('.',1)[1]
        target={'cp_cache_hit':'owner.packet.entry_hit','cp_fast':'owner.packet.entry_fast'}.get(suffix,'owner.packet.single.'+suffix)
        before=raw;raw=re.sub(r'\b'+re.escape(name)+r'\b',target,raw)
        assert raw!=before
        edits.append((before,raw))
    # Redirect only output recordings; never reopen a qualified source trace.
    for name in re.findall(r'\$fopen\("([^\"]+)"',raw):
        change('$fopen("'+name+'"','$fopen("'+str(out/Path(name).name)+'"')
    restored=raw
    for a,b in reversed(edits):
        assert restored.count(b)==1
        restored=restored.replace(b,a,1)
    assert restored==original
    return raw


def run(source,out,engine,payload_probe=False,mutation=None,native_visible=False,native_mutation=None):
    assert not out.exists()
    s=json.loads((source/'run-state.json').read_text())
    assert s['status']=='PASS_ORIGINAL_FULL_PROCESSOR_CP_SOURCE' and s['synthesized_read_owner']
    verify(source,s['files'])
    for row in s['source_files']:
        assert sha(source/row['path'])==row['output_sha256']
    base=Path(s['source']);parent=json.loads((base/'run-state.json').read_text())
    assert parent['status']=='PASS_OWNED_FULL_PROCESSOR_PACKETS'
    verify(base,parent['files'])
    cp=ROOT/'build/rtl-executive/native-cp-source-v1-20261024'
    m=json.loads((cp/'manifest.json').read_text());verify(cp,m['files'])
    assert sha(cp/'manifest.json')==s['CP_manifest_sha256']
    fn=(cp/'functions.sv').read_text()
    actual=processor((base/'candidate.sv').read_text(),fn)
    for name in ['dorado_r_front_control_step','dorado_r_front_pure_step']:
        assert module(actual,name)==module((source/'candidate.sv').read_text(),name)
    owned=caller((base/'caller.sv').read_text(),fn)
    native_record=None
    if native_visible:
        actual=expose(actual);owned=expose_caller(owned)
        bridge,cell,native_record=native_cells(ROOT)
        if native_mutation=='invert-select':
            old='wire RbBypassDly=select_b;';new='wire RbBypassDly=!select_b;'
            assert bridge.count(old)==2;bridge=bridge.replace(old,new)
        else:assert native_mutation is None
    else:assert native_mutation is None
    if mutation=='ignore-payload':
        assert payload_probe
        old='packet_ready && read_ready && payload_ready;';new='packet_ready && read_ready;'
        assert owned.count(old)==1;owned=owned.replace(old,new,1)
    elif mutation=='early-ram':
        old='files(storage_clk,owner_commit,';new='files(storage_clk,contb_advance,'
        assert owned.count(old)==1;owned=owned.replace(old,new,1)
    else:
        assert mutation is None
    out.mkdir(parents=True);paths=[];rows=[]
    for row in s['source_files']:
        name=row['path']
        if name=='dorado_cp_read_caller.sv':
            continue
        dest=out/name;dest.parent.mkdir(parents=True,exist_ok=True)
        text=(source/name).read_text()
        if name=='candidate.sv':text=actual
        elif name=='inputs/tb_r_front_local.sv':text=bench(text,out,payload_probe,native_visible)
        dest.write_text(text);paths.append(dest)
        rows.append(dict(path=name,parent_sha256=sha(source/name),output_sha256=sha(dest)))
    dest=out/'caller.sv';dest.write_text(owned);paths.append(dest)
    rows.append(dict(path='caller.sv',output_sha256=sha(dest)))
    dest=out/'forward.sv';shutil.copy2(base/'forward.sv',dest);paths.append(dest)
    rows.append(dict(path='forward.sv',output_sha256=sha(dest)))
    if native_visible:
        for name,text in [('native_R.sv',bridge),('native_MC10173.v',cell)]:
            dest=out/name;dest.write_text(text);paths.append(dest);rows.append(dict(path=name,output_sha256=sha(dest)))
        (out/'native_R_manifest.json').write_text(json.dumps(native_record,indent=2)+'\n')
    for name in [Path(__file__).name,'generate_cp_paired_read.py','generate_native_r_capture.py']:
        shutil.copy2(Path(__file__).with_name(name),out/name)
    state=dict(status='RUNNING',scope=__doc__,engine=engine,source=str(source),
               source_state_sha256=sha(source/'run-state.json'),started=time.time(),driver_pid=os.getpid(),
               source_files=rows,files={str(p.relative_to(out)):sha(p) for p in out.rglob('*') if p.is_file()},
               actions=[],single_fallback_fixture=True,ordinary_paired_path_tested=False,
               payload_gate_probe=payload_probe,mutation=mutation,whole_machine_payload_validity_still_upstream=True,
               actual_native_visible_R_cells=native_visible,native_cell_mutation=native_mutation,
               external_visible_R_predicted_result_register=not native_visible,
               qualified_pure_and_cached_evaluators_byte_identical=True,
               all_original_event_assertions_retained=True,physical_Q_bits=844,physical_RAM_bits=9216,clock_channels=100,
               native_integrated=False,physical_timing_qualified=False,hardware_changed=False)
    def save():
        (out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    commands=([['iverilog','-g2012','-s','tb_r_front_local','-o',str(out/'test.vvp'),*map(str,paths)],
               ['vvp','-i',str(out/'test.vvp')]] if engine=='icarus' else
              [['verilator','--binary','--timing','-j','2','-Wno-fatal','--top-module','tb_r_front_local',
                '--Mdir',str(out/'obj'),'-o','test',*map(str,paths)],[str(out/'obj/test')]])
    save();code=1
    for label,cmd in zip(['compile','test'],commands):
        with (out/(label+'.log')).open('x') as log:
            child=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT)
            row=dict(label=label,command=cmd,pid=child.pid,started=time.time())
            state['actions'].append(row);save();code=child.wait()
        row.update(returncode=code,finished=time.time(),log_sha256=sha(out/(label+'.log')));save()
        if code:break
    log=(out/'test.log').read_text() if (out/'test.log').exists() else ''
    if native_mutation:
        passed=state['actions'][0]['returncode']==0 and code!=0 and 'READ_OWNER common publication mismatch'in log
    elif mutation:
        marker='PAYLOAD_GATE accepted invalid payload' if mutation=='ignore-payload' else 'CP unready request reached the original SRAM owner'
        passed=state['actions'][0]['returncode']==0 and code!=0 and marker in log
    else:
        passed=code==0 and any(marker in log for marker in ['FULL_PROCESSOR_CP_BLOCK_PASS','FULL_PROCESSOR_CP_COLLISION_PASS','FULL_PROCESSOR_CP_SOURCE_PASS'])
    if passed and not mutation and not native_mutation:
        assert len(re.findall(r'^FULL_CP_CLOCK channel=\d+ edges=\d+$',log,re.M))==100
        assert sha(out/'transactions.hex')==sha(source/'transactions.hex')
        state['full_trace_byte_identical_to_original_checked_fixture']=True
        if payload_probe:
            state['payload_probes']=re.findall(r'^PAYLOAD_GATE_PROBE check=(\d+) staged=([01])$',log,re.M)
            assert any(row[1]=='1'for row in state['payload_probes'])
    state.update(status=('PASS_COMBINED_CP_PACKET_COUNTEREXAMPLE' if mutation or native_mutation else 'PASS_COMBINED_CP_PACKET_SINGLE_FALLBACK')
                 if passed else 'TERMINAL_COMBINED_CP_PACKET_NOT_ESTABLISHED',finished=time.time())
    save();print(state['status'],flush=True)
    return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--engine',choices=['icarus','verilator'],required=True)
    p.add_argument('--payload-probe',action='store_true');p.add_argument('--mutation',choices=['ignore-payload','early-ram'])
    p.add_argument('--native-visible',action='store_true');p.add_argument('--native-mutation',choices=['invert-select'])
    a=p.parse_args();raise SystemExit(run(a.source.resolve(),a.output.resolve(),a.engine,a.payload_probe,a.mutation,a.native_visible,a.native_mutation))
