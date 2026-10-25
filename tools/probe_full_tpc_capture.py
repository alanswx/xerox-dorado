#!/usr/bin/env python3
"""Exercise normal memory Block data followed by a source-closed CP Block reset.

Use the entire checked original processor graph, common caller and original
four visible-R cells. Drive normal memory MIR Block and optional RSTK inputs;
load StkP through the original program's CP/task/MIR sequence. Optional timing
profiles change only the independent reference's explicit delays. Do not import
guest state, alter original cells or drop any event assertion. This is a directed
timing-profile experiment, not native arrival qualification.
"""
import argparse
import json
from pathlib import Path
import re
import shutil
import subprocess
import time

from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha
from generate_j19_capture import rewrite as j19_capture
from prove_cp_prefix_cache import module

PROFILES={'inherited':(1750,400,1250),'aperture':(3200,1500,5000)}


def timing_source(name,raw,profile):
    """Change only explicit independent-reference delays, not DUT behavior."""
    if profile=='inherited' or not name.startswith('timed/'):
        return raw
    gate,ff,phase=PROFILES[profile]
    if name.startswith('timed/cell_'):
        values=set(re.findall(r'assign\s+#\((\d+)\)',raw))
        if values:
            assert values=={'1750'},(name,values)
            raw=re.sub(r'(assign\s+#\()1750(\))',r'\g<1>'+str(gate)+r'\2',raw)
    elif name=='timed/run_control_original_ff.sv':
        original_phase=module(raw,'original_SE10231')
        rest=raw.replace(original_phase,'PHASE_SOURCE_SENTINEL',1)
        assert set(re.findall(r'<=#\((\d+)\)',rest))=={'400'}
        assert set(re.findall(r'<=#\((\d+)\)',original_phase))=={'1250'}
        rest=rest.replace('<=#(400)','<=#('+str(ff)+')')
        original_phase=original_phase.replace('<=#(1250)','<=#('+str(phase)+')')
        raw=rest.replace('PHASE_SOURCE_SENTINEL',original_phase,1)
    else:
        raw=raw.replace('<=#(400)','<=#('+str(ff)+')')
        raw=raw.replace('DELAY_PS=400','DELAY_PS='+str(ff))
    return raw


def run(source, out, engine, late_block=False, trace_arrivals=False, j19_epochs=False, rstk=None, j19_data_mutation=False, stack_pointer=None, profile='inherited'):
    assert not out.exists()
    state = json.loads((source/'run-state.json').read_text())
    assert state['status']=='PASS_COMBINED_CP_PACKET_SINGLE_FALLBACK'
    assert state['actual_native_visible_R_cells']
    verify(source,state['files'])
    out.mkdir(parents=True)
    rows=[]; paths=[]
    for row in state['source_files']:
        name=row['path']; old=source/name
        assert sha(old)==row['output_sha256']
        raw=old.read_text()
        if name=='inputs/tb_r_front_local.sv':
            marker='monitor=1;p=SEED;p[19:0]=20\'h178;'
            assert raw.count(marker)==1
            # conta_pins maps p[281:246] to MIR[35:0]; data[17]=MIR[15].
            # Use a normal input producer, never a fixture/owner patch.
            raw=raw.replace(marker,marker+'p[261]=1;',1)
            marker='   if(!observed_cp_sets[17])$fatal(1,"FULL_CP sBLOCK not observed");'
            assert raw.count(marker)==1
            extra='''   // Keep the complete original staged-read case above, then continue
   // the SAME history through ordinary phases into a second CP/root crossing.
   repeat(2)begin event_step(0,p);event_step(1,p);end
   event_step(0,p);
   p[292]=1;p[291:289]=~3'(4);p[288:282]=7'(16>>1);p[340]=0;p[344]=0;
   event_step(0,p);p[292]=0;event_step(1,p);p[292]=1;event_step(1,p);
'''
            if rstk is not None:
                assert 0<=rstk<16
                marker_extra='   repeat(2)begin event_step(0,p);event_step(1,p);end'
                extra=extra.replace(marker_extra,
                    ''.join(f"   p[{172+bit}]=1'b{(rstk>>(3-bit))&1};\n"for bit in range(4))+marker_extra,1)
            if stack_pointer is not None:
                assert 0<=stack_pointer<256
                prep='''   // Exact normal CP/task/MIR sequence used by the original
   // full program's256-value StkP<- gate; never patch an owner in flight.
   if(!cb)event_step(1,p);
   p[326+33:326+28]=6'b110001;p[326+36:326+34]=0;
   p[8:0]=9'h178;p[293]=0;p[294]=0;p[205]=1;p[254]=1;p[204:202]=3'b110;
   event_step(1,p);
   cp_write(0,40);cp_write(1,64);cp_write(0,8);cp_write(1,0);cp_write(0,0);
   for(integer i=0;i<32;i++)event_step(!cb,p);
   event_step(1,p);p[253:246]=im_ff(8'h62);event_step(1,p);
   for(integer i=0;i<32;i++)event_step(!cb,p);
   event_step(1,p);p[253:246]=0;event_step(1,p);
   for(integer i=0;i<32;i++)event_step(!cb,p);
   p[263]=1;p[253:246]=im_ff(8'o144);
   for(integer bit_no=0;bit_no<8;bit_no++)p[227+bit_no]=(STACK_VALUE>>(7-bit_no))&1;
   event_step(1,p);for(integer i=0;i<16;i++)event_step(!cb,p);
   if({q[710],q[711],q[712],q[713],q[714],q[715],q[717],q[716]}!==8'(STACK_VALUE))
    $fatal(1,"J19 ordinary stack-pointer load failed");
   $display("J19_STACK_INPUT value=%0d check=%0d",STACK_VALUE,checks);
   event_step(1,p);p[253:246]=0;p[261]=1;event_step(1,p);
'''.replace('STACK_VALUE',str(stack_pointer))
                extra=prep+extra
            raw=raw.replace(marker,extra+marker,1)
            for path in re.findall(r'\$fopen\("([^\"]+)"',raw):
                raw=raw.replace('$fopen("'+path+'"','$fopen("'+str(out/Path(path).name)+'"',1)
            marker='     if(pure_held_next!==n)'
            assert raw.count(marker)==1
            raw=raw.replace(marker,'''     if(checks>=185)
      $display("TPC_FULL_CAPTURE check=%0d before_Block_n=%b predicted_Block_n=%b original_Block_n=%b before_TPCBypass=%b predicted_TPCBypass=%b original_TPCBypass=%b root=%b/%b",checks,stored_q[781],n[781],reference[781],stored_q[74],n[74],reference[74],cb,ca);
'''+marker,1)
            if trace_arrivals:
                marker='     for(integer i=0;i<100;i=i+1) begin\n       if(counts[i]-previous[i]!=int\'(predicted[i]))'
                assert raw.count(marker)==1
                diagnostic='''     if(checks>=195)begin
      for(integer i=0;i<100;i++)if(counts[i]-previous[i]!=int'(predicted[i]))
       $display("TPC_CLOCK_DIFF check=%0d channel=%0d actual=%0d expected=%0d",checks,i,counts[i]-previous[i],predicted[i]);
      for(integer i=0;i<844;i++)if(reference[i]!==expected[i])
       $display("TPC_STATE_DIFF check=%0d bit=%0d predicted=%b actual=%b",checks,i,expected[i],reference[i]);
     end
'''
                raw=raw.replace(marker,diagnostic+marker,1)
                monitor=''' always @(original.l.j19.ca or original.l.j19.cb or original.l.j19.p7 or original.l.j19.p10 or original.l.j19.state_q or original.c.f15.ca or original.c.f15.p7 or original.c.f15.state_q)
  if(monitor && checks>=195)
   $display("TPC_ORIGINAL_ARRIVAL t=%0.3f check=%0d root=%b f15_C=%b D=%b Q=%h j19_C=%b%b D=%b%b Q=%h",$realtime,checks,root,original.c.f15.ca,original.c.f15.p7,original.c.f15.state_q,original.l.j19.cb,original.l.j19.ca,original.l.j19.p10,original.l.j19.p7,original.l.j19.state_q);
'''
                assert raw.count('endmodule')==1
                raw=raw.replace('endmodule',monitor+'endmodule',1)
        elif name=='candidate.sv':
            assert 'r[111:76]=p[281:246];' in raw
            assert 'data[19:16]={m[14],m[15],m[17],m[16]};' in raw
            if late_block:
                # Shared consumers use the generated capture copy. Retain the
                # generic copy's same setting too; changing only unused legacy
                # code cannot calibrate a negative control.
                for family in ['control','capture']:
                    before='module dorado_'+family+'_hold_atomic_step #(parameter integer WAVES=24,parameter integer CP_TPC_BLOCK_EARLY=1)('
                    assert raw.count(before)==1
                    raw=raw.replace(before,before.replace('CP_TPC_BLOCK_EARLY=1','CP_TPC_BLOCK_EARLY=0'),1)
        elif name=='adapters/dorado_local_r_front_step.sv' and j19_epochs:
            raw=j19_capture(raw)
            if j19_data_mutation:
                for bit in range(2):
                    use_pre='(j19_legacy_edges[0] | j19_root_edges[0])'if bit==0 else'j19_legacy_edges[1]'
                    original=f'if(captures[{10+bit}]) front_next[{98+bit}]={use_pre} ? pre_data[{42+bit}]:before_front_data[{42+bit}];'
                    assert raw.count(original)==1
                    if j19_data_mutation in [True,'one']:
                        actual=f"if(captures[{10+bit}]) front_next[{98+bit}]=1'b1;"
                    elif j19_data_mutation=='late-underflow'and bit==1:
                        actual=original.replace('j19_legacy_edges[1]','(j19_legacy_edges[1] | j19_root_edges[1])',1)
                    elif j19_data_mutation=='old-overflow'and bit==0:
                        actual=original.replace('(j19_legacy_edges[0] | j19_root_edges[0])','j19_legacy_edges[0]',1)
                    else:continue
                    raw=raw.replace(original,actual,1)
        raw=timing_source(name,raw,profile)
        target=out/name;target.parent.mkdir(parents=True,exist_ok=True);target.write_text(raw)
        paths.append(str(target));rows.append(dict(path=name,parent_sha256=sha(old),output_sha256=sha(target)))
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    shutil.copy2(Path(__file__).with_name('generate_j19_capture.py'),out/'generate_j19_capture.py')
    record=dict(status='RUNNING',scope=__doc__,source=str(source),source_state_sha256=sha(source/'run-state.json'),
                engine=engine,late_block=late_block,normal_memory_MIR_Block_input=261,
                original_staged_read_case_retained_first=True,additional_ordinary_root_pairs=2,
                passive_original_arrival_probe=trace_arrivals,
                experimental_j19_ordered_epochs=j19_epochs,
                tail_RSTK_value=rstk,j19_data_mutation=j19_data_mutation,
                ordinary_stack_pointer_input=stack_pointer,
                compiled_digital_profile_ps=dict(zip(['gate','ordinary_FF','SE10231'],PROFILES[profile])),
                timing_profile_is_not_part_characterization=True,
                original_per_event_assertions_preserved=True,actual_native_R_cells=True,
                source_files=rows,files={str(p.relative_to(out)):sha(p)for p in out.rglob('*')if p.is_file()},
                actions=[],started=time.time(),native_timing_qualified=False)
    def save():(out/'run-state.json').write_text(json.dumps(record,indent=2)+'\n')
    commands=([['iverilog','-g2012','-s','tb_r_front_local','-o',str(out/'test.vvp'),*paths],
               ['vvp','-i',str(out/'test.vvp')]]if engine=='icarus'else
              [['verilator','--binary','--timing','-j','2','-Wno-fatal','--top-module','tb_r_front_local',
                '--Mdir',str(out/'obj'),'-o','test',*paths],[str(out/'obj/test')]])
    save();code=1
    for label,command in zip(['compile','test'],commands):
        with(out/(label+'.log')).open('x')as log:
            child=subprocess.Popen(command,stdout=log,stderr=subprocess.STDOUT)
            row=dict(label=label,pid=child.pid,command=command,started=time.time());record['actions'].append(row);save()
            code=child.wait()
        row.update(returncode=code,finished=time.time(),log_sha256=sha(out/(label+'.log')));save()
        if code:break
    log=(out/'test.log').read_text()if(out/'test.log').exists()else''
    record.update(status='TERMINAL_FULL_TPC_CAPTURE_OBSERVED',returncode=code,finished=time.time(),
                  original_program_passed=code==0 and 'FULL_PROCESSOR_CP_BLOCK_PASS' in log,
                  observations=re.findall(r'^TPC_FULL_CAPTURE .*$',log,re.M))
    save();print(record['status'],code,flush=True)
    return code


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    for name in ['source','output']:parser.add_argument('--'+name,type=Path,required=True)
    parser.add_argument('--engine',choices=['icarus','verilator'],required=True)
    parser.add_argument('--late-block',action='store_true')
    parser.add_argument('--trace-arrivals',action='store_true')
    parser.add_argument('--j19-epochs',action='store_true')
    parser.add_argument('--rstk',type=int,choices=range(16))
    parser.add_argument('--j19-data-mutation',nargs='?',const='one',choices=['one','late-underflow','old-overflow'])
    parser.add_argument('--stack-pointer',type=int,choices=range(256))
    parser.add_argument('--profile',choices=PROFILES,default='inherited')
    args=parser.parse_args()
    assert not args.j19_data_mutation or args.j19_epochs
    raise SystemExit(run(args.source.resolve(),args.output.resolve(),args.engine,args.late_block,args.trace_arrivals,args.j19_epochs,args.rstk,args.j19_data_mutation,args.stack_pointer,args.profile))
