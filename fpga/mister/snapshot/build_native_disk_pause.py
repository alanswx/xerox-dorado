#!/usr/bin/env python3
"""Stage Serial72 disk/memory/network pause and independent local ACK status.

Source qualification only. Serial71 timing is rejected; this driver cannot run
Quartus or assemble/deploy. Qualify a structural advance change before any fit.
"""
import argparse
import hashlib
import json
from pathlib import Path
from generate_serial_boards import ROOT,raw_receipt
import generate_native_disk_pause as change

PARENT='2026-10-20-snapshot-native-network-pause-rejected'
QUALIFIED='2026-10-20-snapshot-disk-services-barrier'


def sha(raw):return hashlib.sha256(raw).hexdigest()


def stage(out,reports,destination):
    parent_report=reports/PARENT;qualified=reports/QUALIFIED
    parent=json.loads(raw_receipt(parent_report,'terminal-fit/network-pause-integration.json'))
    state=json.loads(raw_receipt(parent_report,'terminal-fit/terminal-state.json'))
    assert not state['builder_live']and not state['project_processes']and not state['prepared_inputs_changed']
    assert json.loads(raw_receipt(parent_report,'qualification/proof/timing-audit.json'))['status']=='REJECTED_TIMING'
    payload=json.loads(raw_receipt(qualified,'payload/qualification.json'))
    control=json.loads(raw_receipt(qualified,'coordinator/results.json'))
    status=json.loads(raw_receipt(qualified,'status/qualification.json'))
    assert payload['status']=='PASS'and payload['real_pause_coordinators']and payload['observed_bits']==131823 and len(payload['cases'])==5
    assert control['status']=='PASS'and control['new_required_disk_coordinator_bits']==3 and len(control['cases'])==8
    assert status['status']=='PASS'and status['new_required_top_bits']==5 and len(status['cases'])==6 and len(status['client_failure_controls'])==9
    new=str(destination or out);out.mkdir(parents=True,exist_ok=False);hashes={}
    assets={'src/fpga/mister/dorado_pack_hps.sv':('payload/executed-inputs/medium.sv',payload['inputs']['medium.sv']),
        change.PREFIX+'board_state_uio_msa.sv':('status/executed-inputs/candidate.sv',status['inputs']['candidate.sv'])}
    for name,digest in parent['source_hashes'].items():
        raw=raw_receipt(parent_report,'terminal-fit/'+name);assert sha(raw)==digest,name
        if Path(name).suffix in('.qsf','.qpf','.qip','.sdc','.tcl'):raw=raw.decode().replace(parent['project'],new).encode()
        if name in assets:
            path,expected=assets[name];raw=raw_receipt(qualified,path);assert sha(raw)==expected
        elif name=='core/Dorado.sv':raw=change.core(raw.decode()).encode()
        elif name==change.WRAPPER:raw=change.wrapper(raw.decode()).encode()
        elif name=='core/Dorado.qsf':raw+=f'\nset_global_assignment -name SYSTEMVERILOG_FILE {new}/{change.PREFIX}disk_pause_barrier.sv\n'.encode()
        p=out/name;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(raw);hashes[name]=sha(raw)
    assert len(hashes)==661
    raw=raw_receipt(qualified,'payload/executed-inputs/disk_pause_barrier.sv')
    assert sha(raw)==payload['inputs']['disk_pause_barrier.sv']==control['inputs']['disk_pause_barrier.sv']
    name=change.PREFIX+'disk_pause_barrier.sv';(out/name).write_bytes(raw);hashes[name]=sha(raw)
    name=change.PREFIX+'assets.json';record=json.loads((out/name).read_text())
    record['assets']['board_state_uio_msa.sv']=hashes[change.PREFIX+'board_state_uio_msa.sv']
    (out/name).write_text(json.dumps(record,indent=2)+'\n');hashes[name]=sha((out/name).read_bytes())
    for name in('chipwidth-integration.json','ready-path-integration.json','memory-pause-integration.json','network-pause-integration.json'):
        (out/name).write_bytes(raw_receipt(parent_report,'terminal-fit/'+name))
    record=dict(scope=__doc__,project=new,parent_project=parent['project'],parent_report=PARENT,qualified_report=QUALIFIED,
        verified_parent_inputs=parent['source_hashes'],source_hashes=hashes,
        qualified_payload_inputs=payload['inputs'],qualified_control_inputs=control['inputs'],qualified_status_inputs=status['inputs'],
        capacity_only=True,assembly_permitted=False,quartus_permitted=False,real_memory_pause=True,real_network_pause=True,real_disk_pause=True,
        real_pause_barriers=False,new_wrapper_state_bits=5,new_top_state_bits=5,existing_network_top_bits=6,local_status_mask=7,
        new_clocks=0,new_external_ports=0,foreign_service_inputs=5,schema_id='af0b647cb43a3a62',
        builder_sha256=sha(Path(__file__).read_bytes()),generator_sha256=sha(Path(change.__file__).read_bytes()),
        limits='Raw memory/disk/network adapter ACKs only; host permissions remain required. Input/video ACKs, physical DDR fence, Main coordinator, full state access/restore and legal timing remain open.')
    (out/'disk-pause-integration.json').write_text(json.dumps(record,indent=2)+'\n')
    print('Staged Serial72 source only:',len(hashes),'inputs;',new,flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--stage',type=Path,required=True);p.add_argument('--destination',type=Path)
    p.add_argument('--reports',type=Path,default=ROOT/'fpga/mister/reports');a=p.parse_args();stage(a.stage.resolve(),a.reports.resolve(),a.destination)
