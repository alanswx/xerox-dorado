#!/usr/bin/env python3
"""Probe actual Control-A MIR/CP producers against the original processor graph.

Compare all 786 physical Qs, 9216 RAM bits, 67 clock channels and processor
outputs after each event. This composition experiment is not native or physical
timing qualification. A failure is retained as evidence, never a passing gate.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import sys
import time

from generate_r_front_mir_reference import generate
from stage_r_operand_timing import stage
from test_task_ready import ORACLE

ROOT = Path(__file__).resolve().parents[1]
CANDIDATE = 'verilog/rtl/dorado_r_front_mir_step.sv'
PROFILES = {
    'balanced': (1000, 1000, 1000, None, 1000),
    'fast_gates': (500, 1000, 500, None, 1000),
    'later_control': (1000, 2000, 1000, None, 1000),
    # The PARC values apply only to the inherited parity arcs, not all gates.
    'parc_parity_min': (1000, 1000, 1000, 'min', 1000),
    'parc_parity_max': (1000, 1000, 1000, 'max', 1000),
    'later_r_memory': (1000, 1000, 1000, None, 9900),
}


def sha(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()


def bench_source(parent, iterations, producer_random=False, cp=False, front_directed=False, low_cp=False):
    raw = parent.read_text()
    raw = raw[:raw.index('   // Notify-only wakeups')]
    for a, b in [
        ('tb_r_front_ready', 'tb_r_front_mir'),
        ('dorado_r_front_ready_step', 'dorado_r_front_mir_step'),
        ('r_front_ready_reference', 'r_front_mir_reference'),
        ('[245:0]', '[309:0]'), ('rp[245:145]', 'rp[309:145]'),
        ('[751:0]', '[785:0]'), ('[58:0]', '[66:0]'),
        ('0:58', '0:66'), ('i<59', 'i<67'), ('i<752', 'i<786'),
        ('dut.child.child.files', 'dut.child.child.child.files'),
        ('integer iterations=2048', f'integer iterations={iterations}'),
    ]:
        raw = raw.replace(a, b)
    raw = raw.replace('wire unsettled;', 'wire unsettled,shared_mismatch;wire [97:0] conta_next;')
    raw = raw.replace('source_pins,fa_n,ready_flags);', 'source_pins,fa_n,ready_flags,conta_next,shared_mismatch);')
    # Raw data, original maintenance data, CP bus idle, distinct ASEL.0\'a.
    raw = raw.replace("(246'h3<<240)", "(310'h3<<240)|(310'h1<<263)|(310'h7<<289)|(310'h1<<292)|(310'h1<<293)|(310'h1<<294)")
    raw = raw.replace('q=fixture;monitor=1;', 'q=fixture;monitor=1;')
    raw = raw.replace('integer checks=0,adr_moves', 'integer adr_moves')
    raw = raw.replace(' integer counts[0:66]', '''
 integer checks=0;
 always @(posedge original.h.l05.p9) if(monitor && checks>=65 && checks<100)
   $display("R_FRONT_MIR_ARRIVAL t=%0.3f kind=H_l05 D=%b MIR=%h",$realtime,
    {original.h.l05.p5,original.h.l05.p6,original.h.l05.p7,original.h.l05.p10},reference[785:764]);
 always @(posedge original.l.i15.p9) if(monitor && checks>=65 && checks<100)
   $display("R_FRONT_MIR_ARRIVAL t=%0.3f kind=L_i15 D=%b MIR=%h",$realtime,
    {original.l.i15.p5,original.l.i15.p6,original.l.i15.p7,original.l.i15.p10},reference[785:764]);
 always @(reference[785:764]) if(monitor && checks>=65 && checks<100)
   $display("R_FRONT_MIR_ARRIVAL t=%0.3f kind=MIR Q=%h",$realtime,reference[785:764]);
 integer counts[0:66]''')
    # CP/MIR replace the obsolete external CP/FF/Ready control stimuli.
    raw = raw.replace('   p[242]=1;event_step(1,p);p[242]=0;event_step(1,p);', '')
    raw = raw.replace('if(unsettled||repeated) $fatal(1,"R front solver unsettled/repeated");', '''
     if(unsettled||repeated)begin
       $display("R_FRONT_MIR_SOLVER check=%0d shared=%b old=%h next=%h ca_next=%h events=%h repeated=%h",checks,shared_mismatch,q,n,conta_next,events,repeated);
       $fatal(1,"R front solver unsettled/repeated");
     end''')
    stimulus = r'''
   // IM data settles while the root masks the capture gates, then both
   // root edges exercise actual MIR->processor and MIR->Ready arrivals.
   p[293]=0;event_step(1,p);
   for(integer trial=0;trial<iterations;trial=trial+1)begin
     if(!cb)event_step(1,p);
     rng=random_next(rng);p[253:246]=rng[7:0];
     rng=random_next(rng);p[261:254]=rng[7:0];
     p[263]=trial[0];p[294]=trial[1];
     event_step(1,p);event_step(0,p);event_step(1,p);
   end
   for(integer channel=0;channel<67;channel=channel+1)
     $display("R_FRONT_MIR_CLOCK channel=%0d edges=%0d",channel,totals[channel]);
   $display("R_FRONT_MIR_CHANGED front=%h Ready=%h",front_changed,ready_changed);
   $display("R_FRONT_MIR_PROBE PASS checks=%0d writes=%0d reads=%0d epochs=%0d",checks,writes,reads,capture_epoch_changes);
   $finish;
 end
endmodule
`default_nettype wire
'''
    if producer_random:
        marker='     rng=random_next(rng);p[253:246]=rng[7:0];'
        extra=r'''
     for(integer word_no=0;word_no<8;word_no=word_no+1)begin
       rng=random_next(rng);
       for(integer bit_no=0;bit_no<32;bit_no=bit_no+1)
         if(word_no*32+bit_no<236)p[word_no*32+bit_no]=rng[bit_no];
     end
     p[8:0]=9'h178|(9'(rng[1])<<1);p[75]=0;p[108]=0;
     // rCT is CP-owned. Use no asynchronous task set during this producer run.
     p[74:71]=0;
'''
        stimulus=stimulus.replace(marker,extra+marker)
        stimulus=stimulus.replace('   for(integer channel=0;', '''
   if(writes<10||reads<100||capture_epoch_changes<10)
     $fatal(1,"R front MIR producer coverage incomplete");
   for(integer channel=0;''')
    if cp:
        marker='   for(integer channel=0;'
        extra=r'''
   // Real CP bus release captures; no direct maintenance-Q stimulus.
   if(!cb)event_step(1,p);
   for(integer bank=0;bank<2;bank=bank+1)
     for(integer data=0;data<128;data=data+1)begin
       p[292]=1;p[291:289]=~3'(bank);p[288:282]=7'(data);event_step(1,p);
       p[292]=0;event_step(1,p);p[292]=1;event_step(1,p);
       event_step(0,p);event_step(1,p);
     end
   if(totals[65]<128||totals[66]<128)$fatal(1,"R front MIR CP capture coverage incomplete");
'''
        stimulus=stimulus.replace(marker,extra+marker)
    if low_cp:
        marker='   for(integer channel=0;'
        extra=r'''
   // CP is asynchronous to the root. Exercise bus releases with root LOW.
   event_step(0,p);
   for(integer bank=0;bank<2;bank=bank+1)
     for(integer data=0;data<128;data=data+1)begin
       p[292]=1;p[291:289]=~3'(bank);p[288:282]=7'(data);event_step(0,p);
       p[292]=0;event_step(0,p);p[292]=1;event_step(0,p);
       event_step(1,p);event_step(0,p);
     end
'''
        stimulus=stimulus.replace(marker,extra+marker)
    if front_directed:
        helpers=r'''
 function automatic [7:0] im_ff(input [7:0] ff);
   im_ff={ff[0],ff[1],ff[2],ff[3],ff[4],ff[5],ff[6],ff[7]};
 endfunction
 task automatic cp_write(input integer bank,data);
   begin
     if(!cb)event_step(1,p);
     p[292]=1;p[291:289]=~3'(bank);p[288:282]=7'(data);event_step(1,p);
     p[292]=0;event_step(1,p);p[292]=1;event_step(1,p);
   end
 endtask
'''
        raw=raw.replace(' reg [309:0] p;', ' reg [309:0] p;'+helpers)
        directed=r'''
   // Same RBase/StkP commands as the qualified front test, delivered through
   // actual MIR captures and CP bus transactions rather than decoded FF pins.
   if(!cb)event_step(1,p);
   cp_write(1,0);cp_write(0,0);
   for(integer pass_no=0;pass_no<2;pass_no=pass_no+1)
     for(integer task_no=0;task_no<16;task_no=task_no+1)begin
       p=SEED;p[8:0]=9'h178;p[65]=1;p[293]=0;
       cp_write(0,40);p[74:71]=0;event_step(1,p);
       // CPOut.4 drives NoDispatch; CPOut.6 drives rCT (j02 physical pins).
       cp_write(0,8);p[74:71]=4'(task_no);event_step(1,p);
       p[74:71]=0;event_step(1,p);cp_write(0,0);
       p[204:202]=3'b110;p[205]=1;p[254]=1;
       p[253:246]=im_ff(8'h80|(pass_no ? 8'h0f : 8'h00));
       event_step(1,p);
       for(integer edge_no=0;edge_no<32;edge_no=edge_no+1)event_step(!cb,p);
       if(task_no==15 && q[703:640] !== (pass_no ? 64'b0 : ~64'b0))
         $fatal(1,"MIR-directed RBase fill mismatch pass=%0d actual=%h",pass_no,q[703:640]);
     end
   p=SEED;p[8:0]=9'h178;p[65]=1;p[293]=0;
   cp_write(0,40);p[74:71]=0;event_step(1,p);cp_write(0,8);cp_write(0,0);
   p[204:202]=3'b110;p[205]=1;p[254]=1;p[263]=1;p[253:246]=im_ff(8'o144);
   for(integer value=0;value<256;value=value+1)begin
     for(integer bit_no=0;bit_no<8;bit_no=bit_no+1)p[227+bit_no]=(value>>(7-bit_no))&1;
     event_step(1,p);
     for(integer edge_no=0;edge_no<16;edge_no=edge_no+1)event_step(!cb,p);
     if({q[710],q[711],q[712],q[713],q[714],q[715],q[717],q[716]} !== 8'(value))
       $fatal(1,"MIR-directed StkP load mismatch value=%h actual=%b",value,{q[710],q[711],q[712],q[713],q[714],q[715],q[717],q[716]});
   end
   $display("R_FRONT_MIR_DIRECTED RBase=32 StkP=256 changed=%h",front_changed);
   p=SEED;event_step(1,p);
'''
        stimulus=directed+stimulus
    return raw+stimulus


def run(base, out, iterations, post_mir, engine, profile, producer_random, cp, front_directed, low_cp):
    assert not out.exists()
    out.mkdir(parents=True)
    parent = json.loads((base/'results.json').read_text())
    assert parent['status'] == 'PASS_R_FRONT_READY_ARRIVAL_CONTRACT'
    inputs = out/'inputs'
    names = set(parent['inputs']) - {'tb_r_front_ready.sv'}
    names.update([CANDIDATE, 'verilog/rtl/dorado_conta_mir_step.sv'])
    for module in list(sys.modules.values()):
        name = getattr(module, '__file__', None)
        if name and Path(name).resolve().parent == ROOT/'tools':
            names.add(str(Path(name).resolve().relative_to(ROOT)))
    hashes = {}
    for name in sorted(names):
        source = ROOT/name
        # Qualified RTL and original circuit identities must remain unchanged.
        if name in parent['inputs'] and name.startswith(('verilog/', 'chm/')):
            assert sha(source) == parent['inputs'][name], name
        p = inputs/name
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_bytes(source.read_bytes())
        hashes[name] = sha(p)
    tb = inputs/'tb_r_front_mir.sv'
    tb.write_text(bench_source(base/'executed-inputs/tb_r_front_ready.sv', iterations, producer_random, cp, front_directed, low_cp))
    hashes[tb.name] = sha(tb)
    state = dict(status='RUNNING', pid=os.getpid(), started=time.time(), actions=[],
                 post_mir_pins=post_mir, engine=engine, profile=profile,
                 random_external_producers=producer_random, CP_bus_sweep=cp,
                 directed_RBase_StkP=front_directed,LOW_root_CP=low_cp,native_integrated=False)
    def save():
        (out/'run-state.json').write_text(json.dumps(state, indent=2)+'\n')
    def execute(cmd, log):
        with log.open('w') as f:
            rc = subprocess.run(list(map(str, cmd)), cwd=inputs, stdout=f, stderr=subprocess.STDOUT).returncode
        state['actions'].append(dict(command=list(map(str,cmd)), returncode=rc, log=str(log)))
        save()
        return rc
    save()
    ref = out/'reference'
    manifest = generate(inputs, ref)
    # Preserve the independently generated qualified processor cone and RAM.
    cone = out/'dorado_r_front_cone.sv'
    cone.write_bytes((base/cone.name).read_bytes())
    assert sha(cone) == parent['inherited_cone_sha256']
    timed = out/'timed'
    gates,ffs,phases,parc,read_ps=PROFILES[profile]
    stage(inputs, ref, timed, gates,ffs,phases,parc)
    for p in (base/'ram-reference').glob('*.sv'):
        (timed/p.name).write_bytes(p.read_bytes())
    for name in ['r_file_original_storage.sv', 'r_front_original_storage.sv']:
        raw=(inputs/'verilog/verilator'/name).read_text()
        if name=='r_front_original_storage.sv':raw=raw.replace('DELAY_PS=1000',f'DELAY_PS={ffs}')
        else:raw=raw.replace('READ_PS=1000',f'READ_PS={read_ps}')
        (timed/name).write_text(raw)
    (timed/'ready_ff.sv').write_text(ORACLE.replace('module original_F10000', 'module original_F10000_ready').replace('DELAY_PS=1000',f'DELAY_PS={ffs}'))
    candidate = out/'candidate.sv'
    candidate.write_text((inputs/CANDIDATE).read_text().replace('POST_MIR_PINS=2', f'POST_MIR_PINS={post_mir}'))
    rtl = [inputs/n for n in hashes if n.startswith('verilog/rtl/') and n != CANDIDATE]
    support = [candidate, tb, cone, *rtl, *sorted(timed.glob('*.sv')), *sorted(timed.glob('*.v'))]
    defines=['-DPARC_PARITY_TIMING']if parc else[]
    if engine == 'icarus':
        command = ['iverilog', '-g2012', '-s', 'tb_r_front_mir', *defines, '-o', out/'test.vvp', *support]
    else:
        command = ['verilator', '--binary', '--timing', '--top-module', 'tb_r_front_mir', '-Wno-fatal',
                   *defines, '--Mdir', out/'obj', '-o', 'test', *support]
    rc = execute(command, out/'compile.log')
    if rc == 0:
        rc = execute(['vvp', '-i', out/'test.vvp'] if engine == 'icarus' else [out/'obj/test'], out/'test.log')
    for name, digest in hashes.items():
        assert sha(inputs/name) == digest, name
    log=(out/'test.log').read_text() if (out/'test.log').exists() else ''
    summary=re.search(r'^R_FRONT_MIR_PROBE PASS ([^\n]+)$',log,re.M)
    passed=rc==0 and summary is not None
    state.update(status='PASS_COMPOSITION_PROBE' if passed else 'COMPOSITION_PROBE_FAILED',
                 finished=time.time(), inputs=hashes, reference=manifest,
                 summary=summary[1] if summary else None,
                 timed_sources={p.name:sha(p)for p in timed.iterdir()if p.suffix in ['.v','.sv']},
                 inherited_cone_sha256=sha(cone),executed_candidate_sha256=sha(candidate),
                 delay_profile=dict(gates_ps=gates,ffs_ps=ffs,phases_ps=phases,
                                    parc_parity_control=parc,R_read_ps=read_ps),
                 physical_timing_qualified=False, full_arrival_contract_qualified=False)
    save()
    print(state['status'], out, flush=True)
    return 0 if passed else (rc or 1)


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--base', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True)
    p.add_argument('--iterations', type=int, default=64)
    p.add_argument('--post-mir', type=int, choices=[0, 1, 2], default=2)
    p.add_argument('--engine', choices=['icarus', 'verilator'], default='icarus')
    p.add_argument('--profile', choices=PROFILES, default='balanced')
    p.add_argument('--producer-random', action='store_true')
    p.add_argument('--cp', action='store_true')
    p.add_argument('--front-directed', action='store_true')
    p.add_argument('--low-cp', action='store_true')
    a = p.parse_args()
    sys.exit(run(a.base.resolve(), a.output.resolve(), a.iterations, a.post_mir, a.engine, a.profile, a.producer_random, a.cp, a.front_directed, a.low_cp))
