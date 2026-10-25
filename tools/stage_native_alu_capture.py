#!/usr/bin/env python3
"""Connect the qualified ALU/flag transfer to actual native producers passively.

Export physical package Q (not snapshot host-history payloads), all original
flag boundary pins and additional ALU inputs. Retain the existing native flag
owners and every machine/storage/network enable. Capture observation history
only at the existing common owner commit. This is the native phase-observation
step before ownership migration, not a faster or firmware-qualified core.
"""
import argparse
import gzip
import json
from pathlib import Path
import re
import shutil

from generate_r_front_cone import PIN_Q
from native_branch_flag_sources import bindings,operand_producers
from review_processor_branch_composition import checked
from stage_r_front_ready_native import native_net
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1]
BP='src/build/rtl-executive/native16-tx-cable/dorado_native_backplane.v'


def prepare(native,reference,qualified,out,project):
    assert not out.exists();out.mkdir(parents=True)
    provenance=json.loads(gzip.decompress((native/'provenance.json.gz').read_bytes()))
    assert provenance['experimental_j19_ordered_epochs']and not provenance['paired_mode_enabled']
    for n,h in provenance['inputs'].items():assert sha(native/'inputs'/n)==h,n
    parent=Path(provenance['stage'])/provenance['stage_manifest_name']
    assert sha(parent)==provenance['stage_manifest_sha256']
    previous=json.loads(parent.read_text());old_project=previous['project']
    q=checked(qualified)
    assert q['status']=='PASS_ALU_PROJECTION_NATIVE_LOWERING'and q['native_flag_capture_interface']
    assert not q.get('mutation'),'A mutation experiment cannot qualify native wiring'
    m=json.loads((reference/'manifest.json').read_text())
    assert m['physical_bits']==1198 and m['boundary_pins']==432
    binding=operand_producers(native,m);flags=bindings(native)
    output={n:(native/'inputs'/n).read_bytes()for n in provenance['inputs']}
    for n,raw in output.items():
        if Path(n).suffix in {'.qsf','.qip','.qpf','.sdc','.tcl'}:
            output[n]=raw.replace(old_project.encode(),str(project).encode())
    paths={name:f'src/fpga/mister/snapshot_native_processor/{name}_full_snapshot.sv'for name in ['ProcH','ProcL']}
    boards={name:output[path].decode()for name,path in paths.items()}
    physical={name:{}for name in boards}
    kinds={name:{}for name in boards}
    for pkg,(offset,width)in {'e13':(844,64),'f17':(908,4),'g11':(912,6),'h11':(918,6)}.items():
        physical['ProcH'][pkg]=(offset,width);kinds['ProcH'][pkg]=flags['packages'][pkg]['kind']
    for pkg,(offset,width)in m['new_low_packages'].items():
        physical['ProcL'][pkg]=(offset-408+924,width)
        kinds['ProcL'][pkg]=m['boards']['ProcL']['packages'][pkg]
    for row in m['operand_state_layout'].values():
        name,pkg=row['board'],row['package']
        physical[name][pkg]=(row['global_offset'],row['bits']);kinds[name][pkg]=row['kind']
    assigned=set();exports={}
    for name,text in boards.items():
        marker='    input wire [843:0] r_front_q,';assert text.count(marker)==1
        text=text.replace(marker,'    output wire [1197:844] alu_physical,\n'
            '    output wire [431:401] alu_inputs,output wire [37:0] alu_flag_pins,\n'+marker,1)
        lines=[];local=set();memory_exports=[]
        for pkg,(offset,width)in physical[name].items():
            kind=kinds[name][pkg]
            pattern=r'^  cell_'+kind+r'_snapshot(?: #\([^\n]+\))? u_'+pkg+r' \(.*?^  \);'
            matches=re.findall(pattern,text,re.M|re.S);assert len(matches)==1,(name,pkg)
            call=matches[0];actual={int(pin):net for pin,net in re.findall(r'\.p(\d+)\((\w+)\)',call)}
            if kind=='F10145A':
                wire='alu_memory_'+pkg
                lines.append(' wire [63:0] '+wire+';')
                lines.append(f' assign alu_physical[{offset}+:64]={wire};')
                pos=call.index('(\n')+2
                call=call[:pos]+'    .physical_memory('+wire+'),\n'+call[pos:]
                memory_exports.append(pkg)
                local.update(range(offset,offset+width))
            else:
                assert PIN_Q[kind][0]==width
                for pin,expression in PIN_Q[kind][1].items():
                    bit=re.fullmatch(r'state_q\[(\d+)\]',expression)
                    if not bit:continue
                    absolute=offset+int(bit[1]);assert absolute not in local
                    local.add(absolute)
                    if pin in actual:lines.append(f' assign alu_physical[{absolute}]={actual[pin]};')
                    else:
                        # Preserve an originally open output by connecting
                        # that exact physical pin, not by inventing its value.
                        pos=call.index('(\n')+2
                        call=call[:pos]+f'    .p{pin}(alu_physical[{absolute}]),\n'+call[pos:]
            text=text.replace(matches[0],call,1)
        assert not assigned&local;assigned|=local
        for bit in set(range(844,1198))-local:lines.append(f' assign alu_physical[{bit}]=1\'b0;')
        input_expressions={}
        # The twelve former independent ALU/TIOA slots remain connected to
        # their actual producers even when optimized out of the new view.
        extras=json.loads((reference/'base/manifest.json').read_text())['extra_producer_pins']
        if name=='ProcH':
            for net,pin in extras.items():input_expressions[pin]=native_net(boards[name],net)
        for token,row in binding['additional_projection_inputs'].items():
            producer=row['producers'][0]
            if producer['board']==name:input_expressions[row['pin']]=producer['native_net']
        for bit in range(401,432):lines.append(f' assign alu_inputs[{bit}]='+input_expressions.get(bit,"1'b0")+';')
        if name=='ProcH':
            pin_index=json.loads((reference/'base/branch/manifest.json').read_text())['pin_index']
            for net,pin in pin_index.items():lines.append(f' assign alu_flag_pins[{pin}]='+native_net(boards[name],net)+';')
        else:lines.append(' assign alu_flag_pins=38\'b0;')
        assert text.count('endmodule')==1;text=text.replace('endmodule','\n'.join(lines)+'\nendmodule',1)
        output[paths[name]]=text.encode()
        exports[name]=dict(physical_packages=physical[name],physical_bits=sorted(local),memory_packages=memory_exports,
                           input_expressions=input_expressions)
    assert assigned==set(range(844,1198))
    cells=[n for n in provenance['used_hdl']if Path(n).stem=='cell_F10145A_snapshot'];assert len(cells)==1
    cell=cells[0];text=output[cell].decode()
    marker='    output reg [9:0] state_result,';assert text.count(marker)==1
    text=text.replace(marker,marker+'\n    output wire [63:0] physical_memory,',1)
    assert text.count('endmodule')==1
    text=text.replace('endmodule','\n'.join(f' assign physical_memory[{4*i}+:4]=mem[{i}];'for i in range(16))+'\nendmodule',1)
    output[cell]=text.encode()
    backplane=output[BP].decode()
    for name in boards:
        marker=f'Proc{name[4:]}_full_snapshot_experiment b_{name} (';assert backplane.count(marker)==1
        connection=f'\n    .alu_physical(alu_physical_{name}),.alu_inputs(alu_inputs_{name}),.alu_flag_pins(alu_flag_pins_{name}),'
        backplane=backplane.replace(marker,marker+connection,1)
    probe='''
  // Passive actual-native phase observation. No probe output drives a
  // machine pin, architectural owner, readiness signal or clock enable.
  wire [1197:844] alu_physical_ProcH,alu_physical_ProcL;
  wire [431:401] alu_inputs_ProcH,alu_inputs_ProcL;
  wire [37:0] alu_flag_pins_ProcH,alu_flag_pins_ProcL;
  wire [1197:0] alu_physical_live={alu_physical_ProcH|alu_physical_ProcL,r_front_q};
  wire [431:0] alu_pins_live={alu_inputs_ProcH|alu_inputs_ProcL,
    r_front_live[400:145],r_front_parity,r_front_data,r_front_live[126:0]};
  reg alu_observation_valid=0;
  reg [1197:0] alu_physical_before;
  reg [37:0] alu_flag_pins_before;
  reg alu_source_before,alu_flag_clock_before;
  wire [79:0] alu_flags_predicted;
  wire [3:0] alu_flag_events;
  wire [37:0] alu_flag_pins_early;
  wire alu_flag_unsettled;
  always @(posedge storage_clk)if(r_capture_commit)begin
    alu_observation_valid<=1;
    alu_physical_before<=alu_physical_live;
    alu_flag_pins_before<=alu_flag_pins_ProcH;
    alu_source_before<=CLK_cb_p_;
    alu_flag_clock_before<=CLK_ph_p_;
  end
  dorado_processor_flag_capture alu_phase_observation(
    .physical_before(alu_physical_before),.producer_pins_after(alu_pins_live),
    .source_clock_before(alu_source_before),.flag_clock_before(alu_flag_clock_before),
    .flag_clock_after(CLK_ph_p_),.flag_pins_before(alu_flag_pins_before),
    .flag_pins_after(alu_flag_pins_ProcH),.flags_next(alu_flags_predicted),
    .clock_events(alu_flag_events),.unsettled(alu_flag_unsettled),.flag_pins_early(alu_flag_pins_early));
'''
    assert backplane.count('endmodule')==2;backplane=backplane.replace('endmodule',probe+'\nendmodule',1)
    output[BP]=backplane.encode()
    added={
        'src/verilog/rtl/dorado_processor_flag_capture.sv':(ROOT/'verilog/rtl/dorado_processor_flag_capture.sv').read_bytes(),
        'src/verilog/rtl/dorado_branch_condition_frame_step.sv':(ROOT/'verilog/rtl/dorado_branch_condition_frame_step.sv').read_bytes(),
        'src/verilog/rtl/dorado_alu_projection.sv':(qualified/'lowered/projection.sv').read_bytes(),
        'src/verilog/rtl/dorado_branch_condition_cone.sv':(qualified/'replay/reference/base/branch/cone.sv').read_bytes()}
    output.update(added)
    harness='harness/tb_native_cedar.sv';text=output[harness].decode()
    monitor='''
  // Read-only native source records keep the new observation path live.
  // No expected flag value is imposed on the legacy oversampled machine.
  integer alu_probe_records=0;
  reg alu_probe_commit_seen=0,alu_probe_was_valid=0;
  reg [1238:0] alu_probe_saved;
  wire [1238:0] alu_probe_history={dut.m.alu_observation_valid,dut.m.alu_physical_before,
    dut.m.alu_flag_pins_before,dut.m.alu_source_before,dut.m.alu_flag_clock_before};
  always @(posedge host_clk)alu_probe_commit_seen<=dut.m.r_capture_commit;
  always @(negedge host_clk)begin
    if(alu_probe_was_valid && !alu_probe_commit_seen && alu_probe_history!==alu_probe_saved)
      $fatal(1,"NATIVE_ALU observation history changed outside common commit host=%0d",host_ticks);
    alu_probe_saved=alu_probe_history;alu_probe_was_valid=dut.m.alu_observation_valid;
    if(!reset && step && cia!=12'hfff && dut.m.alu_observation_valid && alu_probe_records<128)begin
      $display("NATIVE_ALU_PHASE_RECORD index=%0d host=%0d guest=%0d source=%b/%b flag_clock=%b/%b before=%h after=%h pins=%h before_flags=%h after_flags=%h early=%h predicted=%h events=%h unsettled=%b",
        alu_probe_records,host_ticks,guest_ticks,dut.m.alu_source_before,dut.m.CLK_cb_p_,
        dut.m.alu_flag_clock_before,dut.m.CLK_ph_p_,dut.m.alu_physical_before,dut.m.alu_physical_live,
        dut.m.alu_pins_live,dut.m.alu_flag_pins_before,dut.m.alu_flag_pins_ProcH,
        dut.m.alu_flag_pins_early,dut.m.alu_flags_predicted,dut.m.alu_flag_events,dut.m.alu_flag_unsettled);
      alu_probe_records++;
    end
  end
'''
    assert text.count('endmodule')==1
    output[harness]=text.replace('endmodule',monitor+'\nendmodule',1).encode()
    qsf=output['core/Dorado.qsf'].decode()
    for n in sorted(set(provenance['used_hdl'])|set(added)):
        path=str(project/n)
        if path not in qsf:qsf+='\nset_global_assignment -name SYSTEMVERILOG_FILE '+path+'\n'
    output['core/Dorado.qsf']=qsf.encode()
    for n,raw in output.items():
        path=out/n;path.parent.mkdir(parents=True,exist_ok=True);path.write_bytes(raw)
    record={**previous,'status':'STAGED_PROCESSOR_CONTROL_NATIVE_EXPERIMENT','scope':__doc__,
        'project':str(project),'source_hashes':{n:sha(out/n)for n in output},
        'extra_RTL_sources':list(set(previous['extra_RTL_sources'])|set(added)),
        'passive_actual_native_ALU_capture_connected':True,'new_flag_owner_selected':False,
        'native_observation_harness':harness,'observation_records':128,
        'qualified_projection_gate_sha256':sha(qualified/'run-state.json'),'original_inputs_or_limits_changed':False,
        'physical_state_bits':1198,'new_physical_producer_bits':354,'physical_exports':exports,
        'additional_projection_bindings':binding['additional_projection_inputs'],
        'observation_history_commit':'existing r_capture_commit only',
        'architectural_state_clock_or_enable_changed':False,'no_new_timing_exceptions':True,
        'assembly_permitted':False,'hardware_changed':False,'firmware_qualified':False,
        'native_source_phase_contract_qualified':False,'full60ns_event_batch_connected':False,'full_goal_active':True}
    (out/'r-front-control-native.json').write_text(json.dumps(record,indent=2)+'\n')
    (out/'native-alu-observation.sv.txt').write_text(probe)
    print('STAGED_PASSIVE_NATIVE_ALU_CAPTURE',len(output),'inputs;1198 physical bits; existing owners/enables unchanged')
    return record


def prepare_run(stage,native,out,limit):
    assert not out.exists()and 1000000<=limit<=1000000000
    record=json.loads((stage/'r-front-control-native.json').read_text())
    assert record['passive_actual_native_ALU_capture_connected']and not record['new_flag_owner_selected']
    for n,h in record['source_hashes'].items():assert sha(stage/n)==h,n
    parent=json.loads(gzip.decompress((native/'provenance.json.gz').read_bytes()))
    out.mkdir(parents=True);shutil.copytree(stage,out/'inputs')
    (out/'host-cycle-limit.txt').write_text(str(limit)+'\n')
    window=parent.get('window_initial_control')or'0 2\n';(out/'window-control.txt').write_text(window)
    used=list(parent['used_hdl'])
    for n in sorted(record['extra_RTL_sources']):
        if n not in used:used.append(n)
    host=(out/'inputs/harness/host.cpp').read_text()
    assert 'fopen(medium,"rb")'in host and 'fwrite('not in host and 'ofstream'not in host
    provenance={**parent,'status':'PREPARED_PASSIVE_NATIVE_ALU_SOURCE_DIAGNOSTIC','scope':__doc__,
        'stage':str(stage),'stage_manifest_name':'r-front-control-native.json',
        'stage_manifest_sha256':sha(stage/'r-front-control-native.json'),
        'inputs':{str(p.relative_to(out/'inputs')):sha(p)for p in(out/'inputs').rglob('*')if p.is_file()},
        'used_hdl':used,'window_control_path':str(out/'window-control.txt'),'window_initial_control':window,
        'host_limit':limit,'baseline_inputs_or_limits_changed':False,'original_cold_firmware_boot_claimed':False,
        'bounded_original_firmware_run':False,'hardware_changed':False,'firmware_qualified':False,
        'native_alu_source_observer':True,'new_flag_owner_selected':False}
    (out/'provenance.json.gz').write_bytes(gzip.compress((json.dumps(provenance,indent=2)+'\n').encode(),mtime=0))
    print(provenance['status'],len(used),'HDL files; own fixed HOST limit',limit)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--prepare-run',action='store_true');p.add_argument('--limit',type=int,default=600000000)
    for name in ['native','output']:p.add_argument('--'+name,type=Path,required=True)
    for name in ['reference','qualified','project','stage']:p.add_argument('--'+name,type=Path)
    a=p.parse_args()
    if a.prepare_run:
        assert a.stage;prepare_run(a.stage.resolve(),a.native.resolve(),a.output.resolve(),a.limit)
    else:
        assert a.reference and a.qualified and a.project
        prepare(a.native.resolve(),a.reference.resolve(),a.qualified.resolve(),a.output.resolve(),a.project)
