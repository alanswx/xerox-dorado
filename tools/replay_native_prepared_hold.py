#!/usr/bin/env python3
"""Replay complete recorded programs with the actual prepared-Hold/caller seam.

Preserve every old reference/assertion/result and native caller/control bytes.
Two SAT-selected synthetic memory contexts reproduce recorded pin386 exactly;
these fixtures test host preparation history, not real MemC timing or speed.
The real native132-value cut has its separate source/induction/full-core tests.
"""
import argparse
import json
from pathlib import Path
import re
import shutil
import subprocess
import time

from replay_r_read_buffer import outcomes
from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha

PREPARE='''
 integer hold_prepare_edges=0,hold_published0=0,hold_published1=0;
 task automatic prepare_hold_decision;
  integer edges;reg [9215:0] saved_memory;reg [843:0] saved_owner;
  reg [400:0] saved_pins;reg saved_root;
  begin
   edges=0;saved_memory=memory_image();saved_owner=held_q;
   saved_pins=dut.r_front_before;saved_root=dut.r_front_clock_before;
   while(hold_ready!==1'b1 && edges<4)begin
    advance=0;#1;host_clk=1;#2;host_clk=0;#2;edges++;
    if(memory_image()!==saved_memory || held_q!==saved_owner ||
       dut.r_front_before!==saved_pins || dut.r_front_clock_before!==saved_root)
     $fatal(1,"HOLD_REPLAY preparation published architectural state record=%0d",idx);
   end
   if(hold_ready!==1'b1 || hold_value!==pa[386])$fatal(1,"HOLD_REPLAY wrong usable decision record=%0d",idx);
   hold_prepare_edges+=edges;
  end
 endtask
 always @(posedge host_clk)begin
  if(!reset && hold_known && fixture_hold!==pa[386])$fatal(1,"HOLD_REPLAY fixture does not reproduce recorded raw pin");
  if(owner_commit)begin
   if(hold_ready!==1'b1 || hold_value!==pa[386])$fatal(1,"HOLD_REPLAY stale published decision");
   if(hold_value)hold_published1++;else hold_published0++;
  end
 end
 final $display("HOLD_REPLAY_PREPARATION edges=%0d published0=%0d published1=%0d",hold_prepare_edges,hold_published0,hold_published1);
'''


def bench(raw,source,out,fixtures):
    result=raw.replace(str(source),str(out));edits=[]
    def change(a,b):
        nonlocal result
        assert result.count(a)==1,a
        result=result.replace(a,b,1);edits.append((a,b))
    anchor=' reg [400:0] pb=0,pa=0;'
    extra=f'''
 // Synthetic producer contexts reproduce the exact recorded Hold386 value.
 // They exercise caller/host history; they do not model native memory timing.
 wire [131:0] fixture_context=pa[386] ? 132'h{fixtures['1']}:132'h{fixtures['0']};
 wire fixture_hold,hold_ready,hold_value,hold_known;
 wire [400:0] hold_pa={{pa[400:387],hold_value,pa[385:0]}};
 dorado_memc_hold_function fixture_producer(fixture_context,fixture_hold);
 dorado_memc_hold_buffer hold_frontier(host_clk,reset,fixture_context,pa[386],hold_ready,hold_value,,,,hold_known);
'''
    change(anchor,anchor+extra)
    a='dorado_r_front_owned_packet dut(host_clk,advance,use_pair,ca,refresh,reset,pa,'
    change(a,a.replace('reset,pa,','reset,hold_pa,'))
    change("  read_accept,1'b1,read_needed,",'  read_accept,hold_ready,read_needed,')
    change(' task automatic prepare_entry;',PREPARE+' task automatic prepare_entry;')
    a='   prepare_physical_read;prepare_operand_view;\n  end'
    change(a,'   prepare_physical_read;prepare_operand_view;prepare_hold_decision;\n   prepare_physical_read;prepare_operand_view;\n  end')
    a='#2;prepare_physical_read;prepare_operand_view;#2;'
    change(a,'#2;prepare_physical_read;prepare_operand_view;prepare_hold_decision;prepare_physical_read;prepare_operand_view;#2;')
    restored=result
    for a,b in reversed(edits):restored=restored.replace(b,a,1)
    assert restored.replace(str(out),str(source))==raw
    assert result.count('$fatal')==raw.count('$fatal')+PREPARE.count('$fatal')
    return result


def run(source,stage,fixtures,out,engine):
    assert not out.exists()
    s=json.loads((source/'run-state.json').read_text());verify(source,s['files'])
    assert s['status']=='PASS_PREPARED_OPERAND_COMPLETE_NATIVE_CALLER'and s['engine']==engine
    f=json.loads((fixtures/'results.json').read_text());verify(fixtures,f['files'])
    assert f['status']=='PREPARED_SYNTHETIC_SOURCE_HOLD_REPLAY_FIXTURES'
    m=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,m['source_hashes'])
    assert m['prepared_MemC_Hold_staged']and not m['assembly_permitted']
    caller=(stage/'src/verilog/rtl/caller.sv').read_text().replace(',output wire [400:0] native_before_pins,output wire native_before_root','',1)
    caller=caller.replace(' assign native_before_pins=r_front_before;assign native_before_root=r_front_clock_before;\n','',1)
    assert caller==(source/'caller.sv').read_text()
    assert(stage/'src/verilog/rtl/dorado_r_front_control_step.sv').read_text()==(source/'candidate.sv').read_text()
    assert s['buffer_helper_sha256']==sha(stage/'src/verilog/rtl/dorado_native_operand_buffer.sv')
    out.mkdir(parents=True);paths=[];rows=[];bank={}
    for row in s['source_files']:
        n=row['path'];assert sha(source/n)==row['output_sha256']
        raw=(source/n).read_text();text=bench(raw,source,out,f['fixtures_hex'])if n=='tb.sv'else raw
        q=out/n;q.parent.mkdir(parents=True,exist_ok=True);q.write_text(text);paths.append(q)
        rows.append(dict(path=n,parent_sha256=row['output_sha256'],output_sha256=sha(q)))
        for match in re.finditer(r'^module\s+(\w+)\b.*?^endmodule',text,re.M|re.S):
            assert match[1]not in bank;bank[match[1]]=match[0]
    # Keep the independently delayed original oracle intact. Its gate modules
    # share names with production cells; namespace only the added fixture bank.
    renames={match[1]:'hold_fixture_'+match[1]for p in fixtures.iterdir()if p.suffix=='.v'
             for match in re.finditer(r'^module\s+(\w+)\b',p.read_text(),re.M)}
    assert all(n not in bank for n in renames.values())
    pattern=r'\b(?:'+'|'.join(sorted(renames))+r')\b'
    inverse={v:k for k,v in renames.items()}
    for p in fixtures.iterdir():
        if p.suffix not in ['.sv','.v']:continue
        original=p.read_text();text=re.sub(pattern,lambda m:renames[m[0]],original)
        assert re.sub(r'\b(?:'+'|'.join(sorted(inverse))+r')\b',lambda m:inverse[m[0]],text)==original
        modules=list(re.finditer(r'^module\s+(\w+)\b.*?^endmodule',text,re.M|re.S));assert modules
        present=[v[1]in bank for v in modules]
        if any(present):
            assert all(present)and all(bank[v[1]]==v[0]for v in modules),p
            continue
        n='hold-fixture/'+p.name;q=out/n;q.parent.mkdir(exist_ok=True);q.write_text(text);paths.append(q)
        rows.append(dict(path=n,parent_sha256=None,output_sha256=sha(q)))
    for n in s['files']:
        if n.startswith('executed-tools/'):continue
        q=out/n
        if q.exists():continue
        q.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(source/n,q)
    q=out/'executed-tools'/Path(__file__).name;q.parent.mkdir(exist_ok=True);shutil.copy2(Path(__file__),q)
    state=dict(status='RUNNING',scope=__doc__,started=time.time(),engine=engine,source=str(source),
               source_state_sha256=sha(source/'run-state.json'),stage_manifest_sha256=sha(stage/'r-front-control-native.json'),
               fixtures_receipt_sha256=sha(fixtures/'results.json'),synthetic_producer_contexts=True,
               fixture_primitive_renames=renames,original_delayed_oracle_modules_unchanged=True,
               original_assertions_reference_and_recorded_inputs_retained=True,
               source_files=rows,actions=[],files={str(q.relative_to(out)):sha(q)for q in out.rglob('*')if q.is_file()})
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    commands=([['iverilog','-g2012','-s','tb_r_front_packet','-o',str(out/'test.vvp'),*map(str,paths)],['vvp','-i',str(out/'test.vvp')]]if engine=='icarus'else
              [['verilator','--binary','--timing','--top-module','tb_r_front_packet','-Wno-fatal','-j','2','--Mdir',str(out/'obj'),'-o','test',*map(str,paths)],[str(out/'obj/test')]])
    for label,cmd in zip(['compile','test'],commands):
        with(out/(label+'.log')).open('x')as log:
            child=subprocess.Popen(cmd,stdout=log,stderr=log);a=dict(label=label,pid=child.pid,command=cmd,started=time.time());state['actions'].append(a);save();code=child.wait()
        a.update(returncode=code,finished=time.time(),log_sha256=sha(out/(label+'.log')));save()
        if code:break
    log=(out/'test.log').read_text()if(out/'test.log').exists()else''
    metrics=re.findall(r'^HOLD_REPLAY_PREPARATION edges=(\d+) published0=(\d+) published1=(\d+)$',log,re.M)
    same=outcomes(log)==outcomes((source/'test.log').read_text())
    commits=re.findall(r'^FULL_PROCESSOR_PACKET_PASS .*? commits=(\d+) ',(source/'test.log').read_text(),re.M)
    assert len(commits)==1
    # A recorded trace may contain only one Hold value (the words program
    # contains only1). Require every expected publication, not invented inputs.
    passed=code==0 and same and len(metrics)==1 and sum(int(n)for n in metrics[0][1:])==int(commits[0])
    state.update(status='PASS_PREPARED_HOLD_COMPLETE_CALLER_REPLAY'if passed else'TERMINAL_PREPARED_HOLD_REPLAY_NOT_ESTABLISHED',
                 returncode=code,finished=time.time(),original_results_match=same,outcomes=outcomes(log),metrics=metrics,
                 native_producer_timing_boot_or_speed_qualified=False)
    save();print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['source','stage','fixtures','output']:p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--engine',choices=['icarus','verilator'],required=True)
    a=p.parse_args();raise SystemExit(run(a.source.resolve(),a.stage.resolve(),a.fixtures.resolve(),a.output.resolve(),a.engine))
