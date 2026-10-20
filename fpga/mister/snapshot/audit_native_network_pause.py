#!/usr/bin/env python3
"""Independent inverse review of Serial71 network/memory pause wiring.

Replay the prior native memory/pin audit after reversing only qualified service
wiring. No clocks, periods, original pins, memory pipeline or area policy edits.
Physical DDR visibility and complete machine restore remain unqualified.
"""
import argparse
from functools import lru_cache
import hashlib
import json
from pathlib import Path
from generate_serial_boards import ROOT,raw_receipt
import audit_native_memory_pause as memory

WRAPPER='src/build/rtl-executive/native16-tx-cable/dorado_native_machine.sv'
PREFIX='src/fpga/mister/snapshot_serial/'


def sha(raw):return hashlib.sha256(raw).hexdigest()


def check(ok,why):
    if not ok:raise ValueError(why)


def once(text,before,after):
    check(text.count(before)==1,'unexpected inverse anchor: '+before[:100]);return text.replace(before,after,1)


def inverse_core(text):
    text=once(text,'dorado_ethernet_ddr #(.SNAPSHOT_PAUSE(1)) u_network_ddr (','dorado_ethernet_ddr u_network_ddr (')
    text=once(text,'\n    .snapshot_hold(board_state_hold),.snapshot_idle(snapshot_network_idle),','')
    text=once(text,'dorado_ethernet_uio #(.MAX_WORDS(1024),.SNAPSHOT_PAUSE(1)',
        'dorado_ethernet_uio #(.MAX_WORDS(1024)')
    text=once(text,'\n    .snapshot_hold(snapshot_network_parser_hold),.snapshot_idle(snapshot_network_parser_idle),\n    .snapshot_held(snapshot_network_parser_held),','')
    start=text.index('// Network permission includes the actual engine/parser/settling boundary.\n')
    stop=text.index('assign board_state_guest_held=board_state_hold && !network_step;',start)
    block=text[start:stop]
    # Check each independently named wire/connection, and then compare the
    # complete block to the frozen reviewed source receipt below. No broad
    # deletion can silently approve an inserted ordinary-logic assignment.
    expected='''// Network permission includes the actual engine/parser/settling boundary.
// The external permit still owns physical DDR visibility and the ARM lease.
wire board_state_owner_hold,snapshot_network_hold,snapshot_network_parser_hold;
wire snapshot_network_idle,snapshot_network_parser_idle,snapshot_network_parser_held;
wire snapshot_network_drained,snapshot_network_fault;
wire [4:0] snapshot_service_permits={CAPACITY_SERVICES_HELD[4:3],
    (CAPACITY_SERVICES_HELD[2] && snapshot_network_drained),CAPACITY_SERVICES_HELD[1:0]};
dorado_network_pause_barrier u_network_pause(
    .clk(clk_sys),.machine_reseed(1'b0),.owner_hold(board_state_owner_hold),
    .guest_held(board_state_guest_held),
    .abort(board_state_fault || (CAPACITY_TRANSPORT_RESET && board_state_hold)),
    .device_reset(~pll_locked),.device_fault((|net_ddr_fault) || (|net_ddr_control_errors)),
    .engine_idle(snapshot_network_idle),.parser_idle(snapshot_network_parser_idle),
    .parser_held(snapshot_network_parser_held),.hold(snapshot_network_hold),
    .parser_hold(snapshot_network_parser_hold),.ready(snapshot_network_drained),.fault(snapshot_network_fault));
'''
    check(block==expected,'network permission/controller wiring differs')
    text=once(text,block,'')
    return once(text,'.snapshot_services_held(snapshot_service_permits),\n    .snapshot_service_hold(snapshot_network_hold),.snapshot_service_fault(snapshot_network_fault),\n    .snapshot_owner_holding(board_state_owner_hold),',
        '.snapshot_services_held(CAPACITY_SERVICES_HELD),')


def inverse_wrapper(text):
    text=once(text,'\n    input wire snapshot_service_hold,snapshot_service_fault,\n    output wire snapshot_owner_holding,','')
    text=once(text,'  assign snapshot_owner_holding=snapshot_owner_hold;\n  assign snapshot_fault=snapshot_owner_fault || snapshot_memory_fault || snapshot_service_fault;',
        '  assign snapshot_fault=snapshot_owner_fault || snapshot_memory_fault;')
    text=once(text,'.owner_hold(snapshot_owner_hold || snapshot_service_hold),','.owner_hold(snapshot_owner_hold),')
    return once(text,'.abort(snapshot_owner_fault || snapshot_service_fault || (snapshot_transport_reset && snapshot_hold)),',
        '.abort(snapshot_owner_fault || (snapshot_transport_reset && snapshot_hold)),')


@lru_cache(maxsize=1)
def parent(reports):
    report=reports/'2026-10-20-snapshot-native-memory-pause-rejected'
    record=json.loads(raw_receipt(report,'terminal-fit/memory-pause-integration.json'))
    files={n:raw_receipt(report,'terminal-fit/'+n)for n in record['source_hashes']}
    check(len(files)==660,'parent count differs')
    for name,raw in files.items():check(sha(raw)==record['source_hashes'][name],'damaged parent '+name)
    return record,files


def audit(project,reports,overrides=None):
    overrides=overrides or{}
    def read(name):
        raw=overrides.get(name,(project/name).read_bytes());return raw.encode()if isinstance(raw,str)else raw
    record=json.loads(read('network-pause-integration.json'));old,files=parent(reports)
    check(record['verified_parent_inputs']==old['source_hashes'],'parent hashes differ')
    check(record['capacity_only']and not record['assembly_permitted']and not record['real_pause_barriers'],'capacity-only scope differs')
    check(record['real_memory_pause']and record['real_network_pause'],'missing real service boundary')
    check(record['schema_id']=='af0b647cb43a3a62'and record['new_top_state_bits']==6,'schema/state differs')
    check(record['new_wrapper_state_bits']==record['new_clocks']==record['new_external_ports']==0,'unexpected ordinary topology change')
    qualified=reports/'2026-10-20-snapshot-network-barrier'
    proof=json.loads(raw_receipt(qualified,'wire/qualification.json'))
    control=json.loads(raw_receipt(qualified,'fourstate/results.json'))
    check(proof['status']=='PASS'and proof['real_network_pause_barrier']and proof['state_review']['observed_bits']==34344 and len(proof['cases'])==8,'unqualified actual packet circuit')
    check(control['status']=='PASS'and len(control['cases'])==8,'unqualified four-state/memory release')
    check(record['qualified_inputs']==proof['inputs'],'qualified input set differs')
    assets={'src/fpga/mister/dorado_ethernet_ddr.sv':'dorado_ethernet_ddr.sv',
        'src/fpga/mister/dorado_ethernet_uio.sv':'dorado_ethernet_uio.sv',PREFIX+'network_pause_barrier.sv':'network_pause_barrier.sv'}
    for name,source in assets.items():
        raw=raw_receipt(qualified,'wire/executed-inputs/'+source)
        check(read(name)==raw and sha(raw)==proof['inputs'][source],'unqualified network asset: '+name)
    check(proof['inputs']['network_pause_barrier.sv']==control['inputs']['network_pause_barrier.sv'],'controller source differs across gates')
    check(set(record['source_hashes'])==set(files)|{PREFIX+'network_pause_barrier.sv'},'source set differs')
    inverse={};changed=[]
    for name,before in files.items():
        expected=before
        if Path(name).suffix in('.qsf','.qpf','.qip','.sdc','.tcl'):expected=before.decode().replace(old['project'],record['project']).encode()
        raw=read(name);restored=raw
        if name in assets:restored=expected
        elif name=='core/Dorado.sv':restored=inverse_core(raw.decode()).encode()
        elif name==WRAPPER:restored=inverse_wrapper(raw.decode()).encode()
        elif name=='core/Dorado.qsf':
            suffix=f'\nset_global_assignment -name SYSTEMVERILOG_FILE {record["project"]}/{PREFIX}network_pause_barrier.sv\n'
            restored=once(raw.decode(),suffix,'').encode()
        check(restored==expected,'unapproved normal/pin/clock/constraint edit: '+name)
        check(record['source_hashes'][name]==sha(raw),'actual source hash differs: '+name)
        inverse[name]=restored
        if raw!=expected:changed.append(name)
    check(record['source_hashes'][PREFIX+'network_pause_barrier.sv']==sha(read(PREFIX+'network_pause_barrier.sv')),'new source hash differs')
    upstream=dict(old);upstream.update(project=record['project'],source_hashes={n:sha(raw)for n,raw in inverse.items()})
    inverse['memory-pause-integration.json']=json.dumps(upstream).encode()
    prior=memory.audit(project,reports,inverse)
    return dict(status='PASS_NATIVE_NETWORK_PAUSE_SOURCE_ONLY',scope=__doc__,verified_parent_inputs=660,actual_inputs=661,
        original_pin_connections=prior['original_pin_connections'],schema_id=prior['schema_id'],changed_parent_inputs=changed,
        new_top_state_bits=6,new_wrapper_state_bits=0,new_clocks=0,new_external_ports=0,constraints_unchanged=True,
        real_memory_pause=True,real_network_pause=True,real_pause_barriers=False,
        limits='Engine/parser/settling and retained memory hold only. Physical DDR fence, Main/disk/input/video coordination, all wrapper state access, full snapshots, legal timing and hardware qualification remain required.')


def controls(project,reports):
    cases=[('engine_hold_missing','core/Dorado.sv','.snapshot_hold(board_state_hold)',".snapshot_hold(1'b0)"),
        ('parser_hold_wrong','core/Dorado.sv','.snapshot_hold(snapshot_network_parser_hold)','.snapshot_hold(board_state_owner_hold)'),
        ('raw_owner_feedback','core/Dorado.sv','.owner_hold(board_state_owner_hold)','.owner_hold(board_state_hold)'),
        ('external_permit_missing','core/Dorado.sv','CAPACITY_SERVICES_HELD[2] && snapshot_network_drained','snapshot_network_drained'),
        ('ready_faked','core/Dorado.sv','.ready(snapshot_network_drained)',".ready()"),
        ('pll_reseed','core/Dorado.sv',".machine_reseed(1'b0)",'.machine_reseed(~pll_locked)'),
        ('memory_release_early',WRAPPER,'snapshot_owner_hold || snapshot_service_hold','snapshot_owner_hold'),
        ('fault_not_retained',WRAPPER,'snapshot_owner_fault || snapshot_service_fault || (snapshot_transport_reset && snapshot_hold)','snapshot_owner_fault || (snapshot_transport_reset && snapshot_hold)'),
        ('parser_held_unconnected','core/Dorado.sv','.snapshot_held(snapshot_network_parser_held)','.snapshot_held()'),
        ('barrier_modified',PREFIX+'network_pause_barrier.sv','assign hold=requested || phase!=RUN;','assign hold=requested;'),
        ('ordinary_enable_modified','core/Dorado.sv',' && storage_initialized && snapshot_frontends_ready',' && snapshot_frontends_ready'),
        ('timing_relaxed','core/Dorado.sdc','derive_pll_clocks','create_clock -period 1000 [get_ports FAKE_CLOCK]\nderive_pll_clocks')]
    results=[]
    for label,name,before,after in cases:
        raw=(project/name).read_text();check(before in raw,'missing mutation anchor: '+label);damaged=raw.replace(before,after,1)
        record=json.loads((project/'network-pause-integration.json').read_text());record['source_hashes'][name]=sha(damaged.encode())
        try:audit(project,reports,{name:damaged,'network-pause-integration.json':json.dumps(record)})
        except(ValueError,AssertionError)as e:results.append(dict(case=label,rejected=True,diagnostic=str(e),metadata_rehashed=True))
        else:raise ValueError('damaged integration accepted: '+label)
    return results


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--project',type=Path,required=True)
    p.add_argument('--reports',type=Path,default=ROOT/'fpga/mister/reports');p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    result=audit(a.project.resolve(),a.reports.resolve());result['failure_controls']=controls(a.project.resolve(),a.reports.resolve())
    a.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
