#!/usr/bin/env python3
"""Replay original CP-coherent traffic through the actual combined caller.

Every original2470-bit subevent,844-bit owner,9216-bit SRAM image and100 clock
counts is checked. Geometry mutations cover376 independent source bits plus
two roots: dependent CP aliases/decoders are regenerated from the real13 wires.
Preserve the write/forwarding/reset/hold coverage requirements. This is not
native cell/clock integration or physical timing qualification.
"""
import argparse
import json
from pathlib import Path
import re
import shutil
import subprocess
import time

from generate_cp_paired_read import processor,caller
from generate_native_r_capture import generate as native_cells,expose,expose_caller
from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'


def bench(raw,trace,functions,indices,cp_words=False,initial_path=None,native_visible=False):
    original=raw;edits=[]
    def change(a,b):
        nonlocal raw
        assert raw.count(a)==1,a
        raw=raw.replace(a,b,1);edits.append((a,b))
    if native_visible:
        change('reg [15:0] visible_r=0;', '''wire [15:0] visible_r;wire [35:0] native_R_frame;wire owner_commit;
 dorado_native_r_capture visible_cells(host_clk,owner_commit,native_R_frame,visible_r);
 integer native_first_only=0,native_first_only_changed=0,native_last_open=0;
''')
        marker=' wire [843:0] last_held_next=last_result[2469:1626];'
        change(marker,marker+'''
 always @(posedge host_clk)if(owner_commit && use_pair)begin
  for(integer half=0;half<2;half++)begin
   if(!dut.packet.first_native_R[34+half] && dut.packet.second_native_R[34+half])begin
    native_first_only++;
    if(last_held_next[572+8*half+:8]!==visible_r[8*half+:8])native_first_only_changed++;
   end
   if(!dut.packet.second_native_R[34+half])native_last_open++;
  end
 end
''')
        change(' always @(posedge host_clk)if(advance && ready)visible_r<=last_held_next[587:572];',
               ' // The original four native MC10173 cells own the16 visible-R bits.')
        change('visible_r=initial_owner[587:572];', '''visible_cells.high.u_g06.q=initial_owner[575:572];visible_cells.high.u_g07.q=initial_owner[579:576];
  visible_cells.low.u_g06.q=initial_owner[583:580];visible_cells.low.u_g07.q=initial_owner[587:584];''')
    if cp_words:
        change('localparam integer COUNT=14884;','localparam integer COUNT=12481;')
        change('if(pairs<1000 || paired_writes==0 || forwarded_bytes==0)',
               'if(pairs<1 || paired_writes!=0 || forwarded_bytes!=0 || staged_reads==0)')
        change('if(take==2 && dut.packet.first_r_write_n!=2\'b11 && geometry_rejections==0)',
               'if(take==2 && geometry_rejections==0)')
    addition=functions+''' reg read_accept=0;
 wire read_needed,stage_active,stage_reading,stage_valid,stage_busy,stage_tag_match,owner_commit;
 integer staged_reads=0;
 function automatic integer geometry_bit(input integer index);
  case(index)
 GEOMETRY
   default:geometry_bit=-1;
  endcase
 endfunction
'''.replace('GEOMETRY','\n'.join(f'   {i}:geometry_bit={v};' for i,v in enumerate(indices)))
    if native_visible:
        addition=addition.replace('stage_busy,stage_tag_match,owner_commit;','stage_busy,stage_tag_match;')
    marker=' dorado_r_front_owned_packet dut('
    change(marker,addition+marker)
    marker='first_result,last_result,ready,forwarded,held_q);'
    change(marker,'''first_result,last_result,ready,forwarded,held_q,
  read_accept,1'b1,read_needed,stage_active,stage_reading,stage_valid,stage_busy,stage_tag_match,owner_commit'''+
           (',native_R_frame'if native_visible else'')+');')
    # Move only actual SRAM hierarchy, preserving every original memory bit.
    before=raw;raw=raw.replace('dut.packet.files.','dut.files.')
    assert before!=raw;edits.append((before,raw))
    # This independent source-domain check makes the changed geometry test
    # explicit; no original result or memory comparison is masked.
    marker='   cb=rb;ca=eligible ? 1\'b1:ra;pb=before_pins;pa=after_pins;use_pair=eligible;take=1;#2;prepare_entry;#2;'
    change(marker,'''   if(cp_source_pins(before_pins,cp_bus(before_pins))!==before_pins ||
      cp_source_pins(after_pins,cp_bus(after_pins))!==after_pins)
    $fatal(1,"CP_PACKET source trace is not physically coherent record=%0d",idx);
'''+marker)
    marker='   if(!ready)$fatal(1,"PACKET unready single record=%0d",idx);'
    change(marker,'''   if(!use_pair && read_needed)begin: service_original_read
    reg [843:0] saved_owner;reg [9215:0] saved_memory;reg [400:0] saved_pins;reg saved_root;
    integer waits;
    saved_owner=held_q;saved_memory=memory_image();saved_pins=dut.r_front_before;saved_root=dut.r_front_clock_before;
    waits=0;advance=1;
    while(!ready && waits<8)begin
     read_accept=stage_reading && waits>=3;
     #1;host_clk=1;#2;host_clk=0;#2;read_accept=0;waits++;
     if(held_q!==saved_owner || memory_image()!==saved_memory || dut.r_front_before!==saved_pins || dut.r_front_clock_before!==saved_root)
      $fatal(1,"CP_PACKET read preparation published prematurely record=%0d",idx);
    end
    advance=0;
    if(!ready || !stage_valid)$fatal(1,"CP_PACKET staged single not ready record=%0d",idx);
    staged_reads++;
   end
'''+marker)
    change('for(integer k=0;k<403;k++)begin','for(integer k=0;k<378;k++)begin')
    old='if(k<401)pa[k]=!pa[k];else if(k==401)begin cb=0;dut.r_front_clock_before=0;end else ca=0;'
    change(old,'''if(k<376)begin
      pa[geometry_bit(k)]=!pa[geometry_bit(k)];pa=cp_source_pins(pa,cp_bus(pa));
     end else if(k==376)begin cb=0;dut.r_front_clock_before=0;end else ca=0;''')
    change('// k401 deliberately violates','// k376 deliberately violates')
    change('if(geometry_rejections!=403)','if(geometry_rejections!=378)')
    change('PACKET_GEOMETRY_REJECTIONS cases=%0d','CP_PACKET_GEOMETRY_REJECTIONS cases=%0d')
    # Source locations are read-only. Use the independently checked coherent
    # trace and its correct initial record, rather than the old raw-pin trace.
    for suffix in ['transactions.hex','initial.hex']:
        old=re.search(r'\$readmemh\("([^\"]*'+suffix+r')"',raw)[1]
        target=initial_path if suffix=='initial.hex' and initial_path else trace/suffix
        change('$readmemh("'+old+'"','$readmemh("'+str(target)+'"')
    marker='  $finish;'
    extra='  $display("CP_PACKET_STAGED_READS count=%0d",staged_reads);\n'
    if native_visible:
        extra+='''  $display("NATIVE_R_PACKET_COVERAGE first_only=%0d first_only_changed=%0d last_open=%0d",native_first_only,native_first_only_changed,native_last_open);
'''
    change(marker,extra+marker)
    restored=raw
    for a,b in reversed(edits):
        assert restored.count(b)==1
        restored=restored.replace(b,a,1)
    assert restored==original
    return raw


def run(trace,out,engine,mutation,native_visible=False,native_mutation=None):
    assert not out.exists()
    t=json.loads((trace/'run-state.json').read_text());verify(trace,t['files'])
    cp_words=t['status']=='PASS_ORIGINAL_FULL_PROCESSOR_CP_SOURCE'
    assert t['status']=='PASS_ORIGINAL_FULL_PROGRAM_WITH_CP_SOURCE_CLOSURE' or cp_words
    if cp_words:
        assert not t['reproduction_only'] and not t['root_CP_collision_program'] and t['ram_sweep_cases']==0
        assert mutation is None,'write/forwarding mutations require the real paired-write fixture'
    for row in t['source_files']:
        assert sha(trace/row['path'])==row['output_sha256']
    records=(trace/'transactions.hex').read_text().splitlines();assert len(records)==(12481 if cp_words else 14884)
    initial=int((trace/'initial.hex').read_text().strip(),16)
    actual_first_owner=int(records[0],16)>>12490
    recovered=(actual_first_owner<<9216)|(initial&((1<<9216)-1))
    if cp_words:
        cold=BUILD/f'cp-interior-stage-all-{t["engine"]}-v2-20261025/initial.hex'
        assert f'{recovered:02515x}\n'==cold.read_text()
    else:
        assert initial==recovered
    base=Path(t['source']);p=json.loads((base/'run-state.json').read_text());verify(base,p['files'])
    cp=BUILD/'native-cp-source-v1-20261024';m=json.loads((cp/'manifest.json').read_text());verify(cp,m['files'])
    assert sha(cp/'manifest.json')==t['CP_manifest_sha256']
    fn=(cp/'functions.sv').read_text();candidate=processor((base/'candidate.sv').read_text(),fn)
    owned=caller((base/'caller.sv').read_text(),fn)
    native_record=None
    if native_visible:
        candidate=expose(candidate);owned=expose_caller(owned)
        bridge,cell,native_record=native_cells(ROOT)
        if native_mutation=='final-clock':
            old='result[34+half]=first[34+half] & last[34+half];';new='result[34+half]=last[34+half];'
            assert candidate.count(old)==1;candidate=candidate.replace(old,new,1)
        elif native_mutation=='invert-select':
            old='wire RbBypassDly=select_b;';new='wire RbBypassDly=!select_b;'
            assert bridge.count(old)==2;bridge=bridge.replace(old,new)
        else:assert native_mutation is None
    else:assert native_mutation is None
    if mutation=='drop-forward':
        old='.external_file_read(later_read)';new='.external_file_read(original_read)'
        assert candidate.count(old)==1;candidate=candidate.replace(old,new,1)
    elif mutation=='early-ram':
        old='files(storage_clk,owner_commit,';new='files(storage_clk,contb_advance,'
        assert owned.count(old)==1;owned=owned.replace(old,new,1)
    else:assert mutation is None
    # Retain one source bit per physical CP wire. All other aliases and set
    # decoders are dependent; mutate the real source then regenerate them.
    groups={}
    for row in m['direct_aliases']:
        groups.setdefault(row['net'],[]).append(row['index'])
    primary={min(v) for v in groups.values()};assert len(primary)==13
    dependent={v for a in groups.values() for v in a}-primary
    dependent|=set(range(264,282))
    indices=[v for v in range(401)if v not in dependent];assert len(indices)==376
    recovered_path=out/'initial-recovered.hex' if cp_words else None
    tb=bench((base/'tb.sv').read_text(),trace,fn,indices,cp_words,recovered_path,native_visible)
    out.mkdir(parents=True);paths=[];rows=[]
    if cp_words:
        recovered_path.write_text(f'{recovered:02515x}\n')
    for row in t['source_files']:
        name=row['path']
        if name=='dorado_cp_read_caller.sv':continue
        if Path(name).name in ['dorado_r_interior_read_stage.sv','dorado_r_interior_read_tagged_stage.sv']:continue
        dest=out/name;dest.parent.mkdir(parents=True,exist_ok=True)
        text=(trace/name).read_text()
        if name=='candidate.sv':text=candidate
        elif name=='inputs/tb_r_front_local.sv':continue
        dest.write_text(text);paths.append(dest);rows.append(dict(path=name,output_sha256=sha(dest)))
    for name,text in [('caller.sv',owned),('tb.sv',tb),('forward.sv',(base/'forward.sv').read_text())]:
        dest=out/name;dest.write_text(text);paths.append(dest);rows.append(dict(path=name,output_sha256=sha(dest)))
    # The balanced source uses the original unstaged owner, so its source set
    # has no staged-read modules. Freeze the actual qualified auxiliary RTL.
    for name in ['dorado_r_interior_read_stage.sv','dorado_r_interior_read_tagged_stage.sv']:
        dest=out/name;shutil.copy2(ROOT/'verilog/rtl'/name,dest);paths.append(dest)
        rows.append(dict(path=name,output_sha256=sha(dest)))
    if native_visible:
        for name,text in [('native_R.sv',bridge),('native_MC10173.v',cell)]:
            dest=out/name;dest.write_text(text);paths.append(dest);rows.append(dict(path=name,output_sha256=sha(dest)))
        (out/'native_R_manifest.json').write_text(json.dumps(native_record,indent=2)+'\n')
    for name in [Path(__file__).name,'generate_cp_paired_read.py','generate_native_r_capture.py']:
        shutil.copy2(Path(__file__).with_name(name),out/name)
    state=dict(status='RUNNING',scope=__doc__,source=str(trace),source_state_sha256=sha(trace/'run-state.json'),
               source_trace_sha256=sha(trace/'transactions.hex'),source_initial_sha256=sha(trace/'initial.hex'),
               engine=engine,mutation=mutation,source_files=rows,files={str(p.relative_to(out)):sha(p)for p in out.rglob('*')if p.is_file()},
               CP_word_pair_and_read_fixture=cp_words,initial_owner_recovered_from_actual_first_event=cp_words,
               actual_native_visible_R_cells=native_visible,native_cell_mutation=native_mutation,
               external_visible_R_predicted_result_register=not native_visible,
               geometry_independent_input_bits=376,geometry_cases=378,dependent_CP_aliases_regenerated=True,
               original_per_event_result_owner_RAM_and_clock_comparisons_retained=True,
               native_integrated=False,physical_timing_qualified=False,actions=[],started=time.time())
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    commands=([['iverilog','-g2012','-s','tb_r_front_packet','-o',str(out/'test.vvp'),*map(str,paths)],['vvp','-i',str(out/'test.vvp')]] if engine=='icarus' else
              [['verilator','--binary','--timing','-j','2','-Wno-fatal','--top-module','tb_r_front_packet','--Mdir',str(out/'obj'),'-o','test',*map(str,paths)],[str(out/'obj/test')]])
    save();code=1
    for label,cmd in zip(['compile','test'],commands):
        with (out/(label+'.log')).open('x')as log:
            child=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT);a=dict(label=label,command=cmd,pid=child.pid,started=time.time());state['actions'].append(a);save();code=child.wait()
        a.update(returncode=code,finished=time.time(),log_sha256=sha(out/(label+'.log')));save()
        if code:break
    text=(out/'test.log').read_text()if(out/'test.log').exists()else''
    if native_mutation:
        passed=state['actions'][0]['returncode']==0 and code!=0 and 'PACKET committed owner mismatch'in text
    elif mutation:
        marker='PACKET second result mismatch'if mutation=='drop-forward'else'PACKET invalid geometry committed'
        passed=state['actions'][0]['returncode']==0 and code!=0 and marker in text
    else:
        summary=re.search(r'^FULL_PROCESSOR_PACKET_PASS (.+)$',text,re.M)
        passed=code==0 and summary is not None and 'CP_PACKET_GEOMETRY_REJECTIONS cases=378'in text
        if passed:
            state.update(summary=summary[1],cache_coverage=re.search(r'^CACHED_PACKET_COVERAGE (.+)$',text,re.M)[1],
                         staged_reads=int(re.search(r'^CP_PACKET_STAGED_READS count=(\d+)$',text,re.M)[1]))
            assert len(re.findall(r'^PACKET_CLOCK channel=\d+ edges=\d+$',text,re.M))==100
            if native_visible:
                m=re.search(r'^NATIVE_R_PACKET_COVERAGE first_only=(\d+) first_only_changed=(\d+) last_open=(\d+)$',text,re.M)
                assert m;state['native_R_coverage']=dict(zip(['first_only','first_only_changed','last_open'],map(int,m.groups())))
    state.update(status=('PASS_COMBINED_CP_PACKET_COUNTEREXAMPLE'if mutation or native_mutation else'PASS_COMBINED_CP_COHERENT_PACKETS')if passed else'TERMINAL_COMBINED_CP_PACKET_REPLAY_NOT_ESTABLISHED',finished=time.time())
    save();print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--trace',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--engine',choices=['icarus','verilator'],required=True);p.add_argument('--mutation',choices=['drop-forward','early-ram'])
    p.add_argument('--native-visible',action='store_true');p.add_argument('--native-mutation',choices=['final-clock','invert-select'])
    a=p.parse_args();raise SystemExit(run(a.trace.resolve(),a.output.resolve(),a.engine,a.mutation,a.native_visible,a.native_mutation))
