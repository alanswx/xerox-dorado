#!/usr/bin/env python3
"""Independent inverse review of disk drain, retained hold and raw ACK status.

Reconstruct immutable Serial71, then replay its original native pin/clock/source
review. No ordinary clock, readiness, board pin or timing-policy edit is allowed.
"""
import argparse
from functools import lru_cache
import hashlib
import json
from pathlib import Path
from generate_serial_boards import ROOT,raw_receipt
import audit_native_network_pause as network

WRAPPER='src/build/rtl-executive/native16-tx-cable/dorado_native_machine.sv'
PREFIX='src/fpga/mister/snapshot_serial/'


def sha(raw):return hashlib.sha256(raw).hexdigest()


def check(ok,why):
    if not ok:raise ValueError(why)


def once(text,before,after):
    check(text.count(before)==1,'unexpected inverse anchor: '+before[:100]);return text.replace(before,after,1)


def inverse_core(text):
    text=once(text,'wire board_state_disk_drained;\n','')
    text=once(text,".SCHEMA_ID(64'haf0b647cb43a3a62),.LOCAL_SERVICES_MASK(5'b00111),.RESPONSE_LIMIT(4096)",
        ".SCHEMA_ID(64'haf0b647cb43a3a62),.RESPONSE_LIMIT(4096)")
    text=once(text,"\n .local_services_held({2'b00,snapshot_network_drained,board_state_disk_drained,CAPACITY_MEMORY_DRAINED}),",'')
    return once(text,'\n    .snapshot_disk_drained(board_state_disk_drained),','')


def inverse_wrapper(text):
    text=once(text,'\n    output wire snapshot_disk_drained,','')
    text=once(text,'  wire snapshot_disk_hold,snapshot_disk_request,snapshot_disk_idle,snapshot_disk_held,snapshot_disk_fault;\n','')
    text=once(text,'dorado_pack_hps #(.SNAPSHOT_PAUSE(1),.SECTORS_PER_TRACK(29),.LABEL_WORDS(10))',
        'dorado_pack_hps #(.SECTORS_PER_TRACK(29),.LABEL_WORDS(10))')
    text=once(text,'\n    .snapshot_hold(snapshot_disk_request),.snapshot_guest_held(snapshot_guest_held),\n    .snapshot_idle(snapshot_disk_idle),.snapshot_held(snapshot_disk_held),','')
    text=once(text,'  assign snapshot_services_status={snapshot_services_held[4:2],\n      (snapshot_services_held[1] && snapshot_disk_drained),\n      (snapshot_services_held[0] && snapshot_memory_drained)};',
        '  assign snapshot_services_status={snapshot_services_held[4:1],\n      (snapshot_services_held[0] && snapshot_memory_drained)};')
    text=once(text,'snapshot_fault=snapshot_owner_fault || snapshot_memory_fault || snapshot_service_fault || snapshot_disk_fault;',
        'snapshot_fault=snapshot_owner_fault || snapshot_memory_fault || snapshot_service_fault;')
    text=once(text,'.owner_hold(snapshot_owner_hold || snapshot_service_hold || snapshot_disk_hold),',
        '.owner_hold(snapshot_owner_hold || snapshot_service_hold),')
    text=once(text,'.abort(snapshot_owner_fault || snapshot_service_fault || snapshot_disk_fault || (snapshot_transport_reset && snapshot_hold)),',
        '.abort(snapshot_owner_fault || snapshot_service_fault || (snapshot_transport_reset && snapshot_hold)),')
    block='''  // Real disk HPS drain is visible before the ARM image permission. The
  // independent hold keeps all consumers frozen through host-latch release.
  dorado_disk_pause_barrier u_disk_pause(
    .clk(clk_host),.machine_reseed(snapshot_system_reset),.owner_hold(snapshot_owner_hold),
    .guest_held(snapshot_guest_held),
    .abort(snapshot_owner_fault || snapshot_memory_fault || snapshot_service_fault ||
        (snapshot_transport_reset && snapshot_hold)),
    .device_reset(reset),.device_fault(pack_fault || pack_mounted),
    .device_idle(snapshot_disk_idle),.device_held(snapshot_disk_held),
    .hold(snapshot_disk_hold),.device_hold(snapshot_disk_request),
    .ready(snapshot_disk_drained),.fault(snapshot_disk_fault));
'''
    return once(text,block,'')


@lru_cache(maxsize=1)
def parent(reports):
    report=reports/'2026-10-20-snapshot-native-network-pause-rejected'
    record=json.loads(raw_receipt(report,'terminal-fit/network-pause-integration.json'))
    files={n:raw_receipt(report,'terminal-fit/'+n)for n in record['source_hashes']}
    check(len(files)==661,'parent input count differs')
    for n,raw in files.items():check(sha(raw)==record['source_hashes'][n],'damaged parent '+n)
    return record,files


def audit(project,reports,overrides=None):
    overrides=overrides or{}
    def read(n):
        raw=overrides.get(n,(project/n).read_bytes());return raw.encode()if isinstance(raw,str)else raw
    record=json.loads(read('disk-pause-integration.json'));old,files=parent(reports)
    check(record['verified_parent_inputs']==old['source_hashes'],'parent hashes differ')
    check(record['capacity_only']and not record['assembly_permitted']and not record['quartus_permitted']and not record['real_pause_barriers'],'source-only scope differs')
    check(record['real_memory_pause']and record['real_network_pause']and record['real_disk_pause'],'missing actual device boundaries')
    check((record['new_wrapper_state_bits'],record['new_top_state_bits'],record['existing_network_top_bits'],record['local_status_mask'])==(5,5,6,7),'new state/status scope differs')
    check(record['new_clocks']==record['new_external_ports']==0 and record['schema_id']=='af0b647cb43a3a62','ordinary topology/schema differs')
    report=reports/'2026-10-20-snapshot-disk-services-barrier'
    payload=json.loads(raw_receipt(report,'payload/qualification.json'))
    control=json.loads(raw_receipt(report,'coordinator/results.json'))
    status=json.loads(raw_receipt(report,'status/qualification.json'))
    check(payload['status']=='PASS'and payload['real_pause_coordinators']and payload['observed_bits']==131823 and len(payload['cases'])==5,'unqualified real disk circuit')
    check(control['status']=='PASS'and len(control['cases'])==8 and control['new_required_disk_coordinator_bits']==3,'unqualified controller release/unknowns')
    check(status['status']=='PASS'and len(status['cases'])==6 and status['new_required_top_bits']==5 and len(status['client_failure_controls'])==9,'unqualified local ACK protocol')
    for key,proof in(('qualified_payload_inputs',payload),('qualified_control_inputs',control),('qualified_status_inputs',status)):
        check(record[key]==proof['inputs'],'qualified input set differs: '+key)
    assets={'src/fpga/mister/dorado_pack_hps.sv':('payload/executed-inputs/medium.sv',payload['inputs']['medium.sv']),
        PREFIX+'board_state_uio_msa.sv':('status/executed-inputs/candidate.sv',status['inputs']['candidate.sv']),
        PREFIX+'disk_pause_barrier.sv':('payload/executed-inputs/disk_pause_barrier.sv',payload['inputs']['disk_pause_barrier.sv'])}
    for n,(path,digest)in assets.items():
        raw=raw_receipt(report,path);check(read(n)==raw and sha(raw)==digest,'unqualified device asset: '+n)
    check(payload['inputs']['disk_pause_barrier.sv']==control['inputs']['disk_pause_barrier.sv'],'disk controllers differ across fixtures')
    check(set(record['source_hashes'])==set(files)|{PREFIX+'disk_pause_barrier.sv'},'actual source set differs')
    inverse={};changed=[]
    for name,before in files.items():
        expected=before
        if Path(name).suffix in('.qsf','.qpf','.qip','.sdc','.tcl'):expected=before.decode().replace(old['project'],record['project']).encode()
        raw=read(name);restored=raw
        if name in assets:restored=expected
        elif name=='core/Dorado.sv':restored=inverse_core(raw.decode()).encode()
        elif name==WRAPPER:restored=inverse_wrapper(raw.decode()).encode()
        elif name=='core/Dorado.qsf':
            suffix=f'\nset_global_assignment -name SYSTEMVERILOG_FILE {record["project"]}/{PREFIX}disk_pause_barrier.sv\n'
            restored=once(raw.decode(),suffix,'').encode()
        elif name==PREFIX+'assets.json':
            actual=json.loads(raw);original=json.loads(before)
            check(actual['assets']['board_state_uio_msa.sv']==sha(read(PREFIX+'board_state_uio_msa.sv')),'UIO asset digest differs')
            actual['assets']['board_state_uio_msa.sv']=original['assets']['board_state_uio_msa.sv']
            check(actual==original,'unapproved asset metadata change');restored=expected
        check(restored==expected,'unapproved ordinary/pin/clock/constraint edit: '+name)
        check(record['source_hashes'][name]==sha(raw),'actual source hash differs: '+name)
        inverse[name]=restored
        if raw!=expected:changed.append(name)
    check(record['source_hashes'][PREFIX+'disk_pause_barrier.sv']==sha(read(PREFIX+'disk_pause_barrier.sv')),'new source digest differs')
    rolled=dict(old);rolled.update(project=record['project'],source_hashes={n:sha(raw)for n,raw in inverse.items()})
    inverse['network-pause-integration.json']=json.dumps(rolled).encode()
    upstream=network.audit(project,reports,inverse)
    return dict(status='PASS_NATIVE_DISK_PAUSE_SOURCE_ONLY',scope=__doc__,verified_parent_inputs=661,actual_inputs=662,
        original_pin_connections=upstream['original_pin_connections'],schema_id=upstream['schema_id'],changed_parent_inputs=changed,
        new_wrapper_state_bits=5,new_top_state_bits=5,existing_network_top_bits=6,new_clocks=0,new_external_ports=0,
        local_status_mask=7,constraints_unchanged=True,real_memory_pause=True,real_network_pause=True,real_disk_pause=True,
        real_pause_barriers=False,quartus_permitted=False,
        limits='Source-only real memory/network/disk boundaries and raw local ACK status. Host permissions, input/video, DDR visibility, Main coordinator, complete state access/restore and legal timing remain required.')


def controls(project,reports):
    cases=[('disk_not_enabled',WRAPPER,'.SNAPSHOT_PAUSE(1),.SECTORS_PER_TRACK(29)','.SNAPSHOT_PAUSE(0),.SECTORS_PER_TRACK(29)'),
        ('wrong_disk_owner',WRAPPER,'.owner_hold(snapshot_owner_hold),','.owner_hold(snapshot_hold),'),
        ('disk_latch_unobserved',WRAPPER,'.device_held(snapshot_disk_held)',".device_held(1'b0)"),
        ('memory_released_early',WRAPPER,'snapshot_owner_hold || snapshot_service_hold || snapshot_disk_hold','snapshot_owner_hold || snapshot_service_hold'),
        ('missing_disk_permission',WRAPPER,'snapshot_services_held[1] && snapshot_disk_drained','snapshot_disk_drained'),
        ('wrong_local_disk_ack','core/Dorado.sv','snapshot_network_drained,board_state_disk_drained,CAPACITY_MEMORY_DRAINED','snapshot_network_drained,board_state_services_held[1],CAPACITY_MEMORY_DRAINED'),
        ('pretend_input_video_ack','core/Dorado.sv',"{2'b00,snapshot_network_drained", "{2'b11,snapshot_network_drained"),
        ('wrong_raw_status_mask','core/Dorado.sv',"LOCAL_SERVICES_MASK(5'b00111)","LOCAL_SERVICES_MASK(5'b11111)"),
        ('reset_reseeds_disk',WRAPPER,'.machine_reseed(snapshot_system_reset),.owner_hold(snapshot_owner_hold),','.machine_reseed(reset),.owner_hold(snapshot_owner_hold),'),
        ('disk_fault_not_retained',WRAPPER,'snapshot_owner_fault || snapshot_service_fault || snapshot_disk_fault || (snapshot_transport_reset && snapshot_hold)','snapshot_owner_fault || snapshot_service_fault || (snapshot_transport_reset && snapshot_hold)'),
        ('disk_medium_changed','src/fpga/mister/dorado_pack_hps.sv','SECTORS_PER_TRACK = 29','SECTORS_PER_TRACK = 28'),
        ('status_source_changed',PREFIX+'board_state_uio_msa.sv','local_services_snapshot<=local_services_held & LOCAL_SERVICES_MASK','local_services_snapshot<=services_held & LOCAL_SERVICES_MASK'),
        ('ordinary_enable_changed','core/Dorado.sv',' && storage_initialized && snapshot_frontends_ready',' && snapshot_frontends_ready'),
        ('timing_relaxed','core/Dorado.sdc','derive_pll_clocks','create_clock -period 1000 [get_ports FAKE_CLOCK]\nderive_pll_clocks')]
    result=[]
    for label,name,before,after in cases:
        raw=(project/name).read_text();check(before in raw,'missing mutation anchor '+label);damaged=raw.replace(before,after,1)
        record=json.loads((project/'disk-pause-integration.json').read_text());record['source_hashes'][name]=sha(damaged.encode())
        try:audit(project,reports,{name:damaged,'disk-pause-integration.json':json.dumps(record)})
        except(ValueError,AssertionError)as e:result.append(dict(case=label,rejected=True,diagnostic=str(e),metadata_rehashed=True))
        else:raise ValueError('damaged disk integration accepted: '+label)
    return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--project',type=Path,required=True)
    p.add_argument('--reports',type=Path,default=ROOT/'fpga/mister/reports');p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    result=audit(a.project.resolve(),a.reports.resolve());result['failure_controls']=controls(a.project.resolve(),a.reports.resolve())
    a.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
