#!/usr/bin/env python3
"""Pilot the 844-Q processor/control/memory-Hold composition.

Reuse the independent full state/RAM/clock/functional-port checker. The old
directed tests drive raw ASEL/BSEL/LC, which are now internal, so this pilot
instead supplies real ContB instruction inputs and the shared CP bus. It is
not a replacement qualification for the old directed tests or native timing.
"""
import argparse
import json
from pathlib import Path
import sys
from unittest.mock import patch

import test_r_front_l17 as legacy
from generate_r_front_control_reference import generate as reference_generate
from generate_r_front_control_candidate import generate as candidate_generate
from rewrite_rbase_window import rewrite as rbase_rewrite
from rewrite_r_front_input_cones import rewrite_control,rewrite_memory

PROFILES={**legacy.visible.masked.commands.parent.PROFILES,
          'early_refresh':(1750,400,1250,None,1000),
          'late_phase_only':(1750,400,2000,None,1000)}


def bench_transform(raw,arrival,trace_check=None,actual_directed=False,zero_owned=False):
    for old,new in [('[797:0]','[843:0]'),('[325:0]','[400:0]'),('rp[325:145]','rp[400:145]'),
                    ('[68:0]','[99:0]'),('0:68','0:99'),('i<69','i<100'),('i<798','i<844'),
                    ('dorado_r_front_l17_step dut','dorado_r_front_control_step dut'),
                    ('r_front_local_reference original','r_front_control_reference original')]:raw=raw.replace(old,new)
    old='q,cb,ca,pb,pa,n,events';assert raw.count(old)==1
    raw=raw.replace(old,f"q,cb,ca,pb,pa,1'b{arrival},n,events")
    old='conta_next,shared_mismatch);';assert raw.count(old)==1
    raw=raw.replace(old,'conta_next,shared_mismatch,control_flags);')
    old='ref_fa_n,,ref_ready_flags);';assert raw.count(old)==1
    raw=raw.replace(old,'ref_fa_n,,ref_ready_flags,ref_control_flags);')
    old=' wire unsettled,shared_mismatch;';assert raw.count(old)==1
    raw=raw.replace(old,' wire [93:0] control_flags,ref_control_flags;\n'+old)
    # The extra flags must be sampled with all other candidate observations;
    # the predicted epoch still belongs to q until the final commit below.
    old='reg [201:0] expected_source;';assert raw.count(old)==1
    raw=raw.replace(old,'reg [93:0] expected_control_flags;'+old)
    old='expected_source=source_pins;expected_fa=fa_n;';assert raw.count(old)==1
    raw=raw.replace(old,old+'expected_control_flags=control_flags;')
    old='     if(ready_flags!==ref_ready_flags)';assert raw.count(old)==1
    raw=raw.replace(old,'     if(ref_control_flags!==expected_control_flags)$fatal(1,"Joint control functional flags mismatch check=%0d",checks);\n'+old)
    # These are neutral external IM/parity/memory-data levels, not forced Qs.
    old='SEED=(315\'h1<<311)';assert raw.count(old)==1
    raw=raw.replace(old,"SEED=(401'h1<<(326+7))|(401'h1<<(326+8))|(401'h1<<(326+46))|(401'h1<<(326+60))|(401'h1<<(326+61))|(315'h1<<311)")
    old="   fixture[797:792]=6'b111111;";assert raw.count(old)==1
    raw=raw.replace(old,"   fixture[843:798]=0;fixture[843:838]=6'b001101;\n"+old)
    old='   load=1;#100;load=0;#100;load=1;#100;load=0;#100;';assert raw.count(old)==1
    raw=raw.replace(old,'   #100;'+old.strip())
    if zero_owned:
        old='ca=next_root;pa=pins;';assert raw.count(old)==1
        raw=raw.replace(old,old+'pa[144:127]=0;')
        old='pb=pins;#100;';assert raw.count(old)==1
        raw=raw.replace(old,'pb=pins;pb[144:127]=0;#100;')
    if trace_check is not None:
        marker=' integer late_mir_before;';assert raw.count(marker)==1
        trace=f''' always @(original.l.g15.p13 or original.l.g15.p12 or original.l.g15.p11 or
  original.l.g15.p4 or original.l.g15.p5 or original.l.g15.p10 or original.l.g15.p9 or
  original.l.g15.p7 or original.l.g15.p6 or original.l.g14.state_q or reference[785:764] or reference[837:798])
  if(monitor && checks>={trace_check-1} && checks<={trace_check})
   $display("JOINT_RBASE_ARRIVAL t=%0.3f check=%0d root=%b we_n=%b addr=%h data=%h latch_clk=%b latch_sel=%b latch_Q=%h MIR=%h CB=%h word1=%h predicted_word1=%h pre_data=%h pre_WE=%b",
    $realtime,checks,root,original.l.g15.p13,{{original.l.g15.p10,original.l.g15.p9,original.l.g15.p7,original.l.g15.p6}},
    {{original.l.g15.p12,original.l.g15.p11,original.l.g15.p4,original.l.g15.p5}},original.l.g14.p7,original.l.g14.p9,original.l.g14.state_q,
    reference[785:764],reference[837:798],reference[647:644],n[647:644],dut.child.child.pre_data,dut.child.child.after_clocks[46]);
'''
        raw=raw.replace(marker,trace+marker)
    directed=''
    if actual_directed:
        start=raw.index('   // Store TaskingOff through actual MIR before maintenance task selection.')
        last=raw.index('   $display("R_FRONT_LOCAL_DIRECTED RBase=32 StkP=256',start)
        end=raw.index('   p=SEED;event_step(1,p);',last)+len('   p=SEED;event_step(1,p);')
        directed=raw[start:end]
        # These fields used to be driven directly at processor package pins.
        # Supply the corresponding real ContB dASEL/dBSEL/dLC inputs instead;
        # actual MIR clocks and CP maintenance still determine when they land.
        directed=directed.replace('p=SEED;',"p=SEED;p[326+33:326+28]=6'b110001;p[326+36:326+34]=0;")
        directed=directed.replace('   // Store TaskingOff', '   // Actual-source directed: store TaskingOff',1)
    # Preserve the complete checker, storage initialization, physical fixture
    # and startup. Replace only raw-boundary-dependent stimulus/expectations.
    start=raw.index('   // Clear Ready and select emulator task through real CP transactions.')
    raw=raw[:start]+r'''
   $display("R_FRONT_CONTROL_PILOT physical=844 clocks=100 ram=9216 old_directed_cases_not_qualified=1");
   // Drive LC through the actual ContB instruction inputs. Keep all three
   // memory Hold data sources released long enough for their real captures.
   p[326+60]=1;p[326+61]=1;p[326+62]=0;
   p[253:246]=0;p[261:254]=8'h01;p[326+33:326+28]=6'b110001;
   for(integer lc=0;lc<8;lc++)begin
     if(!cb)event_step(1,p);
     p[326+34]=lc[2];p[326+35]=lc[1];p[326+36]=lc[0];
     event_step(1,p);for(integer n=0;n<64;n++)event_step(!cb,p);
   end
   $display("R_FRONT_CONTROL_LC actual_instruction_inputs=8 writes=%0d reads=%0d",writes,reads);
   if(writes<10||reads<100)$fatal(1,"Joint processor actual LC instruction coverage incomplete");
ACTUAL_DIRECTED_INSERT
   for(integer trial=0;trial<iterations;trial++)begin
     if(!cb)event_step(1,p);
     for(integer word_no=0;word_no<8;word_no++)begin
       rng=random_next(rng);
       for(integer bit_no=0;bit_no<32;bit_no++)
         if(word_no*32+bit_no<236)p[word_no*32+bit_no]=rng[bit_no];
     end
     p[8:0]=9'h178|(9'(rng[1])<<1);p[75]=0;p[108]=0;p[74:71]=0;
     rng=random_next(rng);p[253:246]=rng[7:0];
     rng=random_next(rng);p[261:254]=rng[7:0];
     rng=random_next(rng);p[326+45:326+28]=rng[17:0];
     p[263]=trial[0];p[310]=trial[1];p[311]=trial[2];p[313]=trial[3];
     p[326+60]=!trial[0];p[326+61]=!trial[1];p[326+62]=trial[2];
     event_step(1,p);for(integer n=0;n<8;n++)event_step(!cb,p);
   end
   if(!cb)event_step(1,p);
   for(integer bank=0;bank<2;bank++)for(integer data=0;data<128;data++)begin
     cp_write(bank,data);event_step(0,p);event_step(1,p);
   end
   for(integer channel=0;channel<100;channel++)
     $display("R_FRONT_CONTROL_CLOCK channel=%0d edges=%0d",channel,totals[channel]);
   $display("R_FRONT_LOCAL_PROBE PASS checks=%0d writes=%0d reads=%0d epochs=%0d",checks,writes,reads,capture_epoch_changes);
   $finish;
 end
endmodule
`default_nettype wire
'''
    raw=raw.replace('ACTUAL_DIRECTED_INSERT',directed)
    return raw


def run(a):
    parent=legacy.visible.masked.commands.parent;original_run=parent.run
    mutation=a.mutation
    candidate=a.output.resolve().parent/(a.output.name+'-control-candidate.sv');metadata=candidate_generate(candidate,mutation)
    input_cones=rewrite_control(candidate)if a.partition_input_cones else None
    reference=json.loads((a.reference/'manifest.json').read_text())
    sources=set()
    for b in reference['boards']:
        bn=b['board']
        for ext in ['wl','lc']:sources.add('chm/sil/'+bn+'.dm!1_/'+bn+'.'+ext)
        sources.update(b['combinational_sources'])
    gate,ff,phase,parc,read=PROFILES[a.profile];arrival=int(phase<gate)
    original_adapters=parent.adapters_generate
    def adapters(root,out):
        record=original_adapters(root,out)
        if a.rbase_close:
            change=rbase_rewrite(out/'dorado_local_r_front_step.sv')
            for row in record['files']:
                if row['output']=='dorado_local_r_front_step.sv':
                    assert row['output_sha256']==change['parent_sha256']
                    row.update(parent_adapter_sha256=row['output_sha256'],output_sha256=change['output_sha256'],rbase_window_rewrite=change)
            record['rbase_window_rewrite']=change
            (out/'manifest.json').write_text(json.dumps(record,indent=2)+'\n')
        if a.partition_input_cones:
            change=rewrite_memory(out/'dorado_local_r_memory_step.sv',out/'dorado_local_r_port_preview.sv')
            for row in record['files']:
                if row['output']=='dorado_local_r_memory_step.sv':
                    assert row['output_sha256']==change['parent_sha256']
                    row.update(parent_adapter_sha256=row['output_sha256'],output_sha256=change['output_sha256'],input_cone_rewrite=change)
            record['input_cone_rewrite']=change
            (out/'manifest.json').write_text(json.dumps(record,indent=2)+'\n')
        return record
    def integrated(*args,**kwargs):
        original_bench=parent.bench
        def bench(*bargs,**bkwargs):return bench_transform(original_bench(*bargs,**bkwargs),arrival,a.trace_check,a.actual_directed,a.zero_owned_r_inputs)
        kwargs['extra_sources']=sorted(set(kwargs.get('extra_sources',()))|sources)
        if a.rbase_close:kwargs['extra_sources'].append('verilog/rtl/dorado_rbase_window_step.sv')
        with patch.object(parent,'generate',reference_generate),patch.object(parent,'CANDIDATE',str(candidate.relative_to(parent.ROOT))),patch.object(parent,'bench',bench):
            return original_run(*args,**kwargs)
    a.mutation=None
    with patch.object(parent,'run',integrated),patch.object(parent,'adapters_generate',adapters),patch.dict(parent.PROFILES,PROFILES):rc=legacy.run(a)
    p=a.output.resolve()/'run-state.json';r=json.loads(p.read_text())
    r.update(experiment='joint_processor_control_hold_pilot',physical_bits=844,ram_bits=9216,clock_channels=100,
             joint_candidate=metadata,joint_mutation=mutation,rbase_close=a.rbase_close,actual_RBase_StkP_directed=a.actual_directed,owned_r_inputs_zeroed=a.zero_owned_r_inputs,refresh_arrival=arrival,trace_check=a.trace_check,old_raw_boundary_directed_cases_qualified=False,
             native_integrated=False,physical_timing_qualified=False)
    r['partitioned_input_cones']=input_cones
    p.write_text(json.dumps(r,indent=2)+'\n');return rc


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--base',type=Path,required=True)
    p.add_argument('--reference',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--engine',choices=['icarus','verilator'],default='icarus')
    p.add_argument('--profile',choices=PROFILES,default='balanced');p.add_argument('--mutation',choices=['new_fields'])
    p.add_argument('--trace-check',type=int)
    p.add_argument('--rbase-close',action='store_true')
    p.add_argument('--actual-directed',action='store_true')
    p.add_argument('--zero-owned-r-inputs',action='store_true')
    p.add_argument('--partition-input-cones',action='store_true')
    p.add_argument('--iterations',type=int,default=256)
    sys.exit(run(p.parse_args()))
