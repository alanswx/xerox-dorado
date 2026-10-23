#!/usr/bin/env python3
"""Stage the 844-Q processor/control slice privately in frozen 5c65 wrappers.

Use the existing native same-host-edge refresh contract established by exact
cell tests. This is conservative57.142857MHz/SYSPER4 integration, not the
future60ns partition, original analog arrival qualification or permission to
assemble/program a core. Keep original visible-R latches as sole owners.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path
import re

from fit_r_front import quartus_cone
from inventory_r_front_control_native import PATHS
from stage_r_front_ready_native import BACKPLANE,replace_package,native_net
from review_r_front_control import read_run

ROOT=Path(__file__).resolve().parents[1]


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def prepare(native_run,reference,qualified,inventory,refresh_proof,out,project,cut_owned_r_inputs=False,split_live_pins=False):
    assert not out.exists()and project.is_absolute()
    run_source=json.loads(gzip.decompress((native_run/'provenance.json.gz').read_bytes()))
    native=native_run/'inputs';stage=Path(run_source['stage']);meta_path=stage/run_source['stage_manifest_name']
    assert sha(meta_path)==run_source['stage_manifest_sha256']
    baseline=json.loads(meta_path.read_text());assert baseline['state_admin']=='INACTIVE'and baseline['sysper']==4
    assert baseline['frequency_hz']==57142857
    for n,h in baseline['source_hashes'].items():assert sha(native/n)==h,(n,'Changed baseline')
    inv=json.loads((inventory/'results.json').read_text());assert inv['status']=='PASS_PROCESSOR_CONTROL_NATIVE_INVENTORY'
    for n,h in inv['inputs'].items():assert sha(inventory/'inputs'/n)==h
    assert all(sha(native/n)==h for n,h in inv['native_board_identities'].items())
    proof=json.loads((refresh_proof/'results.json').read_text())
    assert proof['status']=='PASS_FROZEN_NATIVE_REFRESH_ORDER'and proof['native_refresh_before_run_capture']==0
    assert not proof['assume_early']and not proof['future60ns_partition_qualified']
    for n,h in proof['inputs'].items():assert sha(native/n)==h and sha(refresh_proof/'inputs'/n)==h
    gate,log=read_run(qualified)
    assert gate['status']=='PASS_R_FRONT_LOCAL_PROBE'and gate['rbase_close']and gate['actual_RBase_StkP_directed']
    assert gate['summary']=='checks=10121 writes=242 reads=4166 epochs=391'
    manifest=json.loads((reference/'manifest.json').read_text());assert manifest==gate['reference']
    if cut_owned_r_inputs:
        memory=(qualified/'adapters/dorado_local_r_memory_step.sv').read_text()
        for side in ['before','after']:
            assert f'wire [201:0] source_{side}={{pins_{side}[201:145],file_read_parity,file_read_data,pins_{side}[126:0]}};'in memory
        assert gate.get('owned_r_inputs_zeroed'), 'Use the complete comparison with owned input slots cut'
    boards={b:(native/path).read_text()for b,path in PATHS.items()};producers={}
    for row in manifest['boards']:
        name=row['board'].split('-Rev-')[0]
        for net,expression in row['boundary_bindings'].items():
            m=re.fullmatch(r'pins\[(\d+)\]',expression)
            if not m:continue
            pin=int(m[1])+(326 if name in ['ContB','MemC']else 0)
            assert pin<401
            producers.setdefault(pin,[]).append(dict(board=name,net=net,expression=native_net(boards[name],net)))
    retained=[r for r in inv['packages']if r['board']in ['ProcH','ProcL']and r['package']in ['g06','g07']]
    assert len(retained)==4
    for board,text in boards.items():
        marker='    input wire state_host_clk';assert text.count(marker)==1
        ports='''    input wire [843:0] r_front_q,
    input wire [15:0] r_front_data,input wire [1:0] r_front_parity,
    output wire [400:0] r_front_pins,output wire r_front_alias_ok,
'''
        if board in ['ProcH','ProcL']:ports+='    output wire [7:0] r_front_visible_r,\n'
        if split_live_pins:ports=ports.replace('r_front_pins,','r_front_pins /* verilator split_var */,')
        text=text.replace(marker,ports+marker,1)
        for row in inv['packages']+inv['RAM_packages']:
            if row['board']==board and row not in retained:
                text=replace_package(text,row).replace('Qualified central storage replaces','Experimental central storage replaces')
        lines=[];aliases=[]
        for pin in range(401):
            sources=[p for p in producers.get(pin,[])if p['board']==board]
            value=sources[0]['expression']if sources else"1'b0"
            exported="1'b0"if cut_owned_r_inputs and 127<=pin<=144 else value
            # These18 slots are supplied inside the owning RAM evaluator.
            # Cut at the board export too, before packed-bus regularization;
            # the actual ddR/parity wires and all physical consumers stay live.
            lines.append(f'  assign r_front_pins[{pin}]={exported};')
            aliases += [f'({value}=={p["expression"]})'for p in sources[1:]]
        lines.append('  assign r_front_alias_ok='+(' & '.join(aliases)if aliases else"1'b1")+';')
        if board in ['ProcH','ProcL']:
            start=0 if board=='ProcH'else 8
            lines += [f'  assign r_front_visible_r[{i}]=R_{start+i:02d};'for i in range(8)]
        assert text.count('endmodule')==1;boards[board]=text.replace('endmodule','\n'.join(lines)+'\nendmodule')
    bp=(native/BACKPLANE).read_text()
    for board in PATHS:
        marker=f'{board}_full_snapshot_experiment b_{board} (';assert bp.count(marker)==1
        ports=f'\n    .r_front_q(r_front_q),.r_front_data(r_front_data),.r_front_parity(r_front_parity),.r_front_pins(r_front_pins_{board}),.r_front_alias_ok(r_front_alias_{board}),'
        if board in ['ProcH','ProcL']:ports+=f'.r_front_visible_r(r_front_visible_r_{board}),'
        bp=bp.replace(marker,marker+ports,1)
    logic='''
  // Experimental physical state; original visible-R latches remain owners.
  wire [843:0] r_front_q;reg [571:0] r_front_lower_q;reg [255:0] r_front_upper_q;
  wire [7:0] r_front_visible_r_ProcH,r_front_visible_r_ProcL;
  assign r_front_q={r_front_upper_q,r_front_visible_r_ProcL,r_front_visible_r_ProcH,r_front_lower_q};
  reg r_front_clock_before;reg [400:0] r_front_before;
  wire [843:0] r_front_next;wire [400:0] r_front_live;
  wire [15:0] r_front_data;wire [1:0] r_front_parity;
  wire [99:0] r_front_events,r_front_repeated;wire r_front_unsettled;
  wire [93:0] r_front_control_flags;wire r_front_shared_mismatch;
  wire r_front_alias_ok;
  always @(posedge storage_clk)if(contb_advance)begin
    r_front_lower_q<=r_front_next[571:0];r_front_upper_q<=r_front_next[843:588];
    r_front_before<=r_front_live;r_front_clock_before<=CLK_ca_p_;
  end
'''
    for board in PATHS:logic+=f'  wire [400:0] r_front_pins_{board};wire r_front_alias_{board};\n'
    aliases=['r_front_alias_'+b for b in PATHS]
    aliases += [f'(CLK_ca_p_==CLK_{suffix}_p_)'for suffix in ['ph','pl','cb','mc']]
    for pin in range(401):
        sources=producers.get(pin,[]);value=f'r_front_pins_{sources[0]["board"]}[{pin}]'if sources else"1'b0"
        if cut_owned_r_inputs and 127<=pin<=144:value="1'b0"
        logic+=f'  assign r_front_live[{pin}]={value};\n'
        first=f'r_front_pins_{sources[0]["board"]}[{pin}]'if sources else"1'b0"
        aliases += [f'(r_front_pins_{p["board"]}[{pin}]=={first})'for p in sources[1:]]
    logic+='  assign r_front_alias_ok='+' & '.join(aliases)+';\n'
    logic+='''  // Frozen native cells sample l06 and l03 on one host edge; old Set
  // is present at capture. This is not an original-analog/future60ns claim.
  dorado_r_front_control_step r_front_evaluator(
    .host_clk(storage_clk),.advance(contb_advance),.state_q(r_front_q),
    .clock_before(r_front_clock_before),.clock_after(CLK_ca_p_),
    .pins_before(r_front_before),.pins_after(r_front_live),.refresh_before_run_capture(1'b0),
    .state_next(r_front_next),.clock_events(r_front_events),.repeated_events(r_front_repeated),
    .unsettled(r_front_unsettled),.file_read_data(r_front_data),.file_read_parity(r_front_parity),
    .shared_state_mismatch(r_front_shared_mismatch),.control_flags(r_front_control_flags));
'''
    if split_live_pins:
        old='wire [400:0] r_front_live;';assert logic.count(old)==1
        logic=logic.replace(old,'wire [400:0] r_front_live /* verilator split_var */;')
        for board in PATHS:
            old=f'wire [400:0] r_front_pins_{board};';assert logic.count(old)==1
            logic=logic.replace(old,f'wire [400:0] r_front_pins_{board} /* verilator split_var */;')
    assert bp.count('endmodule')==2 and 'module dorado_native_backplane_machine'in bp
    bp=bp.replace('endmodule',logic+'\nendmodule',1)
    out.mkdir(parents=True);outputs={}
    for n in baseline['source_hashes']:
        raw=(native/n).read_bytes()
        if Path(n).suffix in {'.qsf','.qip','.qpf','.sdc','.tcl'}:raw=raw.replace(baseline['project'].encode(),str(project).encode())
        outputs[n]=raw
    outputs.update({PATHS[b]:s.encode()for b,s in boards.items()});outputs[BACKPLANE]=bp.encode()
    added={}
    for n in gate['inputs']:
        if n.startswith('verilog/rtl/'):added['src/'+n]=(qualified/'inputs'/n).read_bytes()
    for row in gate['adapters']['files']:added['src/verilog/rtl/'+row['output']]=(qualified/'adapters'/row['output']).read_bytes()
    added['src/verilog/rtl/dorado_r_front_control_step.sv']=(qualified/'candidate.sv').read_bytes()
    added['src/verilog/rtl/dorado_r_front_cone.sv']=quartus_cone((qualified/'dorado_r_front_cone.sv').read_text()).encode()
    split_annotations=[]
    if split_live_pins:
        name='src/verilog/rtl/dorado_r_front_control_step.sv'
        text=added[name].decode()
        for side in ['before','after']:
            old=f'control_{side}=pins_{side}[397:0]';new=f'control_{side} /* verilator split_var */=pins_{side}[397:0]'
            assert text.count(old)==1
            text=text.replace(old,new,1);split_annotations.append(f'control_{side}')
        added[name]=text.encode()
    outputs.update(added)
    outputs['core/Dorado.qsf']+=b'\n# Experimental844-Q processor/control integration; no assembly qualification\n'
    for n in sorted(added):outputs['core/Dorado.qsf']+=f'set_global_assignment -name SYSTEMVERILOG_FILE {project}/{n}\n'.encode()
    for n,raw in outputs.items():
        p=out/n;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(raw)
    record=dict(status='STAGED_PROCESSOR_CONTROL_NATIVE_EXPERIMENT',scope=__doc__,project=str(project),
        source_hashes={n:sha(out/n)for n in outputs},baseline_source_hashes=baseline['source_hashes'],
        modified_board_sources=list(PATHS.values()),extra_RTL_sources=list(added),native_inventory_sha256=sha(inventory/'results.json'),
        component_gate_sha256=sha(qualified/'run-state.json'),refresh_proof_sha256=sha(refresh_proof/'results.json'),
        frequency_hz=57142857,sysper=4,physical_Q_bits=844,evaluator_owned_Q_bits=828,native_visible_R_bits=16,RAM_bits=9216,
        replaced_storage_packages=len(inv['packages'])-4,replaced_RAM_packages=12,live_producers=producers,
        unused_input_positions=[p for p in range(401)if p not in producers],state_admin='INACTIVE',
        owned_read_input_placeholders=list(range(127,145))if cut_owned_r_inputs else[],physical_RAM_read_signals_tied_off=False,
        partitioned_component_input_cones=gate.get('partitioned_input_cones'),verilator_split_live_pins=split_live_pins,
        verilator_component_split_annotations=split_annotations,
        initialization_or_guest_state_added=False,snapshot_abi_supported=False,physical_timing_qualified=False,
        original_analog_order_qualified=False,future60ns_partition_qualified=False,firmware_qualified=False,
        assembly_permitted=False,hardware_changed=False)
    (out/'r-front-control-native.json').write_text(json.dumps(record,indent=2)+'\n')
    (out/'staged-native-logic.sv.txt').write_text(logic)
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    print(record['status'],record['replaced_storage_packages'],'Q packages/12RAM packages;844Qs,16canonical R bits;unchanged57MHz/SYSPER4')
    return record


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for name in ['native-run','reference','qualified','inventory','refresh-proof','output','project']:p.add_argument('--'+name,type=Path,required=True)
    p.add_argument('--cut-owned-r-inputs',action='store_true')
    p.add_argument('--split-live-pins',action='store_true')
    a=p.parse_args();prepare(a.native_run.resolve(),a.reference.resolve(),a.qualified.resolve(),a.inventory.resolve(),a.refresh_proof.resolve(),a.output.resolve(),a.project,a.cut_owned_r_inputs,a.split_live_pins)
