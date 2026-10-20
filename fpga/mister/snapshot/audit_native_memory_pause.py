#!/usr/bin/env python3
"""Independent inverse/source/pin review of real native memory-pause wiring.

Use immutable Serial65 and the executed eight-board physical-memory receipt.
Do not import the integration generator. Retain all original clocks, normal
readiness and architectural pins; foreign service ACKs remain explicit inputs.
"""
import argparse
from functools import lru_cache
import hashlib
import json
from pathlib import Path
from generate_serial_boards import ROOT,raw_receipt
import audit_ready_path as prior

BP='src/build/rtl-executive/native16-tx-cable/dorado_native_backplane.v'
WRAPPER='src/build/rtl-executive/native16-tx-cable/dorado_native_machine.sv'
PREFIX='src/fpga/mister/snapshot_serial/'


def sha(raw):return hashlib.sha256(raw).hexdigest()


def once(text,before,after):
    if text.count(before)!=1:raise ValueError('unexpected inverse anchor: '+before[:100])
    return text.replace(before,after,1)


def inverse_core(text):
    text=once(text,'wire snapshot_frontends_ready;\n','wire snapshot_frontends_ready;\nassign CAPACITY_MEMORY_DRAINED=snapshot_frontends_ready;\n')
    text=once(text,'wire [4:0] board_state_services_held;\n','')
    text=once(text,'.local_held(board_state_guest_held),.services_held(board_state_services_held)',
        '.local_held(board_state_guest_held),.services_held(CAPACITY_SERVICES_HELD)')
    text=once(text,'.snapshot_memory_drained(CAPACITY_MEMORY_DRAINED),\n    .snapshot_frontends_ready(snapshot_frontends_ready),\n    .snapshot_services_status(board_state_services_held),',
        '.snapshot_memory_drained(snapshot_frontends_ready),')
    text=once(text,".snapshot_system_reset(1'b0),",'.snapshot_system_reset(~pll_locked),')
    text=once(text,".clk(clk_sys),.system_reset(1'b0),.transport_reset(CAPACITY_TRANSPORT_RESET),.bus(board_state_ext_bus)",
        '.clk(clk_sys),.system_reset(~pll_locked),.transport_reset(CAPACITY_TRANSPORT_RESET),.bus(board_state_ext_bus)')
    return once(text,'// Snapshot control initializes on complete RBF configuration only.\n// PLL loss and menu/framework/parser resets cannot thaw partially imported state.',
        '// SYSTEM reset is PLL/RBF lifecycle; menu/framework/parser resets cannot thaw state.')


def inverse_backplane(text):
    text=once(text,'\n    input wire snapshot_consumers_hold,snapshot_frontend_hold,','')
    for needle,replacement in [('.state_global_hold(snapshot_consumers_hold)','.state_global_hold(snapshot_hold)'),
        ('.state_freeze(snapshot_consumers_hold)','.state_freeze(snapshot_hold)')]:
        if text.count(needle)!=19:raise ValueError('missing global consumer hold')
        text=text.replace(needle,replacement)
    needle='    .state_frontend_hold(snapshot_frontend_hold),\n'
    if text.count(needle)!=8:raise ValueError('missing frontend member hold')
    return text.replace(needle,'')


WRAPPER_BLOCK='''  // Real memory readiness tightens external service bit0; no acknowledgment
  // is tied high. The same vector reaches the owner and public status.
  assign snapshot_services_status={snapshot_services_held[4:1],
      (snapshot_services_held[0] && snapshot_memory_drained)};
  assign snapshot_fault=snapshot_owner_fault || snapshot_memory_fault;
  dorado_memory_pause_barrier u_memory_pause(
    .clk(clk_host),.machine_reseed(snapshot_system_reset),.owner_hold(snapshot_owner_hold),
    .guest_held(snapshot_guest_held),
    .abort(snapshot_owner_fault || (snapshot_transport_reset && snapshot_hold)),
    .frontends_drained(snapshot_frontends_ready),
    .arbiter_idle(storage_initialized && snapshot_arbiter_idle),
    .raw_service_idle(storage_initialized && snapshot_raw_idle),
    .frontends_frozen(snapshot_frozen[11]),.guest_hold(snapshot_hold),
    .frontend_hold(snapshot_frontend_hold),.ready(snapshot_memory_drained),.fault(snapshot_memory_fault));
'''


def inverse_wrapper(text):
    text=once(text,'\n    output wire snapshot_frontends_ready,\n    output wire [4:0] snapshot_services_status,','')
    text=once(text,'  wire snapshot_owner_hold,snapshot_owner_fault;\n  wire snapshot_frontend_hold,snapshot_memory_fault;\n  wire snapshot_arbiter_idle,snapshot_raw_idle;\n','')
    text=once(text,'.reset(reset || !storage_initialized),.snapshot_idle(snapshot_arbiter_idle),','.reset(reset || !storage_initialized),')
    text=once(text,'.reset(reset),.snapshot_idle(snapshot_raw_idle),','.reset(reset),')
    text=once(text,WRAPPER_BLOCK,'')
    text=once(text,'    .snapshot_consumers_hold(snapshot_hold),.snapshot_frontend_hold(snapshot_frontend_hold),\n','')
    for before,after in [('.snapshot_services_held(snapshot_services_status)','.snapshot_services_held(snapshot_services_held)'),
        ('.snapshot_hold(snapshot_owner_hold)','.snapshot_hold(snapshot_hold)'),
        ('.snapshot_fault(snapshot_owner_fault)','.snapshot_fault(snapshot_fault)'),
        ('.snapshot_memory_drained(snapshot_frontends_ready)','.snapshot_memory_drained(snapshot_memory_drained)')]:
        text=once(text,before,after)
    return text


@lru_cache(maxsize=1)
def parents(reports):
    report=reports/'2026-10-19-snapshot-serial-ready'
    parent=json.loads(raw_receipt(report,'terminal-fit/ready-path-integration.json'))
    files={n:raw_receipt(report,'terminal-fit/'+n)for n in parent['source_hashes']}
    if len(files)!=659:raise ValueError('wrong parent count')
    for n,b in files.items():
        if sha(b)!=parent['source_hashes'][n]:raise ValueError('damaged parent '+n)
    return parent,files


def audit(project,reports,overrides=None):
    overrides=overrides or {}
    def read(n):
        raw=overrides.get(n,(project/n).read_bytes());return raw.encode()if isinstance(raw,str)else raw
    record=json.loads(read('memory-pause-integration.json'));parent,files=parents(reports)
    if record['verified_parent_inputs']!=parent['source_hashes']:raise ValueError('parent hash set differs')
    if not(record['capacity_only']and not record['assembly_permitted']and record['real_memory_pause']and not record['real_pause_barriers']):raise ValueError('scope differs')
    qualified=reports/'2026-10-20-snapshot-eight-msa-sdram-pause'
    proof=json.loads(raw_receipt(qualified,'qualified/results.json'))
    if proof['status']!='PASS'or len(proof['cases'])<4:raise ValueError('unqualified actual memory circuit')
    replacements={PREFIX+'MSA__msa_board.sv':'msa_board.sv','src/fpga/mister/dorado_storage_arb.sv':'dorado_storage_arb.sv',
        'src/fpga/mister/dorado_storage_sdram.sv':'dorado_storage_sdram.sv',PREFIX+'memory_pause_barrier.sv':'memory_pause_barrier.sv'}
    for n,source in replacements.items():
        raw=raw_receipt(qualified,'qualified/executed-inputs/'+source)
        if sha(raw)!=proof['executed_inputs'][source]or read(n)!=raw:raise ValueError('unqualified memory asset: '+n)
    if record['qualified_inputs']!=proof['executed_inputs']:raise ValueError('qualified input set differs')
    allowed=set(files)|{PREFIX+'memory_pause_barrier.sv'}
    if set(record['source_hashes'])!=allowed:raise ValueError('actual source set differs')
    inverse={};changed=[]
    for name,before in files.items():
        expected=before
        if Path(name).suffix in ('.qsf','.qpf','.qip','.sdc','.tcl'):expected=before.decode().replace(parent['project'],record['project']).encode()
        raw=read(name);restored=raw
        if name in replacements:restored=expected
        elif name=='core/Dorado.sv':restored=inverse_core(raw.decode()).encode()
        elif name==BP:restored=inverse_backplane(raw.decode()).encode()
        elif name==WRAPPER:restored=inverse_wrapper(raw.decode()).encode()
        elif name=='core/Dorado.qsf':
            policy=f'\nset_global_assignment -name SYSTEMVERILOG_FILE {record["project"]}/{PREFIX}memory_pause_barrier.sv\n'
            restored=once(raw.decode(),policy,'').encode()
        elif name==PREFIX+'assets.json':
            actual=json.loads(raw);old=json.loads(before)
            for asset in ('MSA__msa_board.sv','memory_pause_barrier.sv'):
                if actual['assets'][asset]!=sha(read(PREFIX+asset)):raise ValueError('asset hash differs')
            del actual['assets']['memory_pause_barrier.sv'];actual['assets']['MSA__msa_board.sv']=old['assets']['MSA__msa_board.sv']
            actual['limits']=old['limits']
            if actual!=old:raise ValueError('unapproved asset metadata')
            restored=expected
        if restored!=expected:raise ValueError('unapproved normal/pin/clock/constraint edit: '+name)
        if raw!=expected:changed.append(name)
        if record['source_hashes'].get(name)!=sha(raw):raise ValueError('actual source hash differs: '+name)
        inverse[name]=restored
    new=PREFIX+'memory_pause_barrier.sv'
    if record['source_hashes'][new]!=sha(read(new)):raise ValueError('new source hash differs')
    # Replay the old independent native pin/clock review on the inverse view.
    rolled=json.loads(json.dumps(parent));rolled.update(project=record['project'],source_hashes={n:sha(b)for n,b in inverse.items()})
    inverse['ready-path-integration.json']=json.dumps(rolled).encode()
    chip=json.loads((project/'chipwidth-integration.json').read_text())
    chip.update(project=record['project'],source_hashes={n:sha(b)for n,b in inverse.items()if n in files})
    inverse['chipwidth-integration.json']=json.dumps(chip).encode()
    old=prior.audit(project,reports,inverse)
    return dict(status='PASS_NATIVE_MEMORY_PAUSE_SOURCE_ONLY',scope=__doc__,verified_parent_inputs=659,actual_inputs=660,
        original_pin_connections=old['original_pin_connections'],schema_id=old['schema_id'],changed_parent_inputs=changed,
        new_state_bits=2,new_ports=0,new_clocks=0,constraints_unchanged=True,real_memory_pause=True,real_pause_barriers=False,
        quarantine_reseed='RBF configuration only; PLL and parser reset cannot clear snapshot control.',
        limits='Other services virtual; wrapper/raw-memory access, full snapshots, timing and hardware remain required.')


def controls(project,reports):
    cases=[('lost_frontend_hold',BP,'.state_frontend_hold(snapshot_frontend_hold)',".state_frontend_hold(1'b0)"),
        ('lost_consumer_hold',BP,'.state_global_hold(snapshot_consumers_hold)',".state_global_hold(1'b0)"),
        ('fake_arbiter_idle',WRAPPER,'.arbiter_idle(storage_initialized && snapshot_arbiter_idle)',".arbiter_idle(1'b1)"),
        ('fake_raw_idle',WRAPPER,'.raw_service_idle(storage_initialized && snapshot_raw_idle)',".raw_service_idle(1'b1)"),
        ('ready_feedback_loop',WRAPPER,'.abort(snapshot_owner_fault || (snapshot_transport_reset && snapshot_hold))','.abort(snapshot_scan_abort)'),
        ('lost_external_permit',WRAPPER,'snapshot_services_held[0] && snapshot_memory_drained','snapshot_memory_drained'),
        ('wrong_status','core/Dorado.sv','.services_held(board_state_services_held)','.services_held(CAPACITY_SERVICES_HELD)'),
        ('pll_reseeds','core/Dorado.sv',".snapshot_system_reset(1'b0)",'.snapshot_system_reset(~pll_locked)'),
        ('lost_init','core/Dorado.sv',' && storage_initialized && snapshot_frontends_ready',' && snapshot_frontends_ready'),
        ('relaxed_clock','core/Dorado.sdc','derive_pll_clocks','create_clock -period 1000 [get_ports FAKE_CLOCK]\nderive_pll_clocks')]
    results=[]
    for label,name,before,after in cases:
        raw=(project/name).read_text()
        if before not in raw:raise ValueError('missing control anchor: '+label)
        raw=raw.replace(before,after,1);record=json.loads((project/'memory-pause-integration.json').read_text());record['source_hashes'][name]=sha(raw.encode())
        try:audit(project,reports,{name:raw,'memory-pause-integration.json':json.dumps(record)})
        except(ValueError,AssertionError)as e:results.append(dict(case=label,rejected=True,diagnostic=str(e),metadata_rehashed=True))
        else:raise ValueError('damaged integration accepted: '+label)
    return results


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--project',type=Path,required=True);p.add_argument('--reports',type=Path,default=ROOT/'fpga/mister/reports');p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    result=audit(a.project.resolve(),a.reports.resolve());result['failure_controls']=controls(a.project.resolve(),a.reports.resolve())
    a.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
