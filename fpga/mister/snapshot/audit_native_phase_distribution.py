#!/usr/bin/env python3
"""Independently reverse native phase edits and replay the frozen source audit.

Allow only exact archived qualified payload assets and nineteen same-edge
permission copies. Preserve physical pins, global clock/enable/live guards,
raw drain, hold/quarantine logic, placement, periods and all unrelated bytes.
"""
import argparse
from functools import lru_cache
import hashlib
import json
from pathlib import Path
from generate_serial_boards import ROOT,raw_receipt
import audit_native_ready_prediction as prior

WRAPPER=prior.WRAPPER;BACKPLANE=prior.BACKPLANE;PREFIX=prior.PREFIX
PHASE=PREFIX+'board_phase_enable.sv'
PARENT='2026-10-20-snapshot-native-ready-prediction-rejected'
QUALIFIED='2026-10-20-snapshot-phase-distribution'


def sha(raw):return hashlib.sha256(raw).hexdigest()


def check(ok,why):
    if not ok:raise ValueError(why)


def once(text,before,after):
    check(text.count(before)==1,'inverse anchor differs: '+before[:90]);return text.replace(before,after,1)


def inverse_backplane(text):
    text=once(text,'    input wire contb_advance,contb_permit,','    input wire contb_advance,')
    block='''  // Same falling-edge permission as Core's global gate; no extra stage.
  // These nineteen registers are required wrapper state, outside board scans.
  wire [18:0] snapshot_board_advance;
  dorado_board_phase_enable #(.BOARDS(19)) u_board_phase(
    .clk(storage_clk),.permit(contb_permit),.advance(snapshot_board_advance));
  // synthesis translate_off
  always @(posedge storage_clk)begin
    if(snapshot_board_advance!=={19{contb_advance}})
      $fatal(1,"native board advance differs from original global phase");
  end
  // synthesis translate_on
'''
    text=once(text,block,'')
    for index in range(19):text=once(text,f'.state_guest_advance(snapshot_board_advance[{index}])',
        '.state_guest_advance(contb_advance)')
    return text


def inverse_helper(text):
    first=text.index('  generate if(PHASE_QUALIFIED)begin:phase_payload\n')
    last=text.index('  end endgenerate\n',first)+len('  end endgenerate\n')
    start=text.index('  end else begin:original_payload\n',first)+len('  end else begin:original_payload\n')
    old=text[start:last-len('  end endgenerate\n')];check(old.endswith('\n'),'missing original branch terminator')
    restored=text[:first]+old[:-1]+text[last:]
    return once(restored,'#(parameter PRECAP = 1, parameter PHASE_QUALIFIED = 0)',
        '#(parameter PRECAP = 1)')


@lru_cache(maxsize=1)
def references(reports):
    r=reports/PARENT;record=json.loads(raw_receipt(r,'terminal-fit/ready-prediction-integration.json'))
    files={n:raw_receipt(r,'terminal-fit/'+n)for n in record['source_hashes']}
    check(len(files)==662,'parent source set differs')
    for n,raw in files.items():check(sha(raw)==record['source_hashes'][n],'parent hash differs: '+n)
    r=reports/QUALIFIED;proof=json.loads(raw_receipt(r,'proof/complete-verification.json'))
    replay=json.loads(raw_receipt(r,'proof/extracted-replay.json'))
    check(proof['status']=='PASS_PHASE_BOARDS_NATIVE_CONTROL_AND_SDRAM_PROVENANCE','component provenance unqualified')
    check(proof['boards_checked']==11 and proof['board_checks']==8399378,'whole-board scope differs')
    check(replay['status']=='PASS_EXTRACTED_PHASE_REPLAY'and len(replay['cases'])==19,'extracted replay missing')
    assets={};members={}
    for board in proof['boards']:
        name=board['board'];root=('boards-memc'if name=='MemC'else'boards-baseboard'if name=='BaseBd'else'boards-rest')+'/'+name+'/inputs/'
        paths=[name+'__board.sv']+[name+'__cell_'+v['kind']+'_snapshot.v'for v in board['families']]
        for n in paths:
            raw=raw_receipt(r,root+n);check(sha(raw)==board['source_hashes'][n],'qualified board digest differs')
            assets[PREFIX+n]=raw;members[PREFIX+n]=root+n
    for target,source in [('MSA__msa_board.sv','msa_board.sv'),('MSA__cell_MC10176.sv','cell_MC10176.sv'),
                          ('board_phase_enable.sv','board_phase_enable.sv')]:
        member='eight-msa/executed-inputs/'+source;raw=raw_receipt(r,member)
        check(sha(raw)==proof['eight_msa']['compiled_inputs'][source],'qualified MSA digest differs')
        assets[PREFIX+target]=raw;members[PREFIX+target]=member
    return record,files,assets,members


def audit(project,reports,overrides=None):
    overrides=overrides or{}
    def read(name):
        raw=overrides[name]if name in overrides else(project/name).read_bytes()
        return raw.encode()if isinstance(raw,str)else raw
    record=json.loads(read('phase-distribution-integration.json'));old,files,assets,members=references(reports)
    check(record['verified_parent_inputs']==old['source_hashes'],'verified parent inputs differ')
    check(set(record['source_hashes'])==set(files)|{PHASE},'actual input set differs')
    check(record['qualified_asset_members']==members,'qualified asset bindings differ')
    check(not record['quartus_permitted']and not record['assembly_permitted']and not record['full_snapshots'],'source-only scope differs')
    check(record['new_required_wrapper_bits']==19 and record['new_clocks']==record['new_external_ports']==0,'new state/clock/port count differs')
    check(record['schema_id']=='af0b647cb43a3a62','board-only schema changed')
    for key,member in [('qualified_proof_sha256','proof/complete-verification.json'),('qualified_replay_sha256','proof/extracted-replay.json')]:
        check(record[key]==sha(raw_receipt(reports/QUALIFIED,member)),'qualification record differs')
    check(read(PHASE)==assets[PHASE],'phase registers are not the qualified source')
    inverse={};changed=[]
    for name,old_raw in files.items():
        expected=old_raw
        if Path(name).suffix in('.qsf','.qpf','.qip','.sdc','.tcl'):expected=old_raw.decode().replace(old['project'],record['project']).encode()
        raw=read(name);restored=raw
        if name in assets:
            check(raw==assets[name],'qualified payload bytes differ: '+name)
            if name.endswith('__board.sv')or name.endswith('MSA__msa_board.sv'):
                restored=raw.decode().replace(',.PHASE_QUALIFIED(1)','').replace(' #(.PHASE_QUALIFIED(1))','').encode()
            else:restored=inverse_helper(raw.decode()).encode()
        elif name=='core/Dorado.sv':restored=once(raw.decode(),'\n    .contb_permit(machine_enable),','').encode()
        elif name==WRAPPER:
            text=once(raw.decode(),'    input wire contb_advance,contb_permit,','    input wire contb_advance,')
            restored=once(text,'\n    .contb_permit(contb_permit),','').encode()
        elif name==BACKPLANE:restored=inverse_backplane(raw.decode()).encode()
        elif name=='core/Dorado.qsf':
            restored=once(raw.decode(),f'\nset_global_assignment -name SYSTEMVERILOG_FILE {record["project"]}/{PHASE}\n','').encode()
        elif name==PREFIX+'assets.json':
            actual=json.loads(raw);previous=json.loads(old_raw)
            check(actual.pop('phase_distribution')==dict(qualified_report=QUALIFIED,phase_bits=19,edge='negedge storage_clk',
                original_global_gate_preserved=True,board_only_schema_unchanged=True),'phase asset metadata differs')
            for path in assets:
                member=Path(path).name;check(actual['assets'][member]==sha(read(path)),'asset hash differs')
                if member in previous['assets']:actual['assets'][member]=previous['assets'][member]
                else:del actual['assets'][member]
            check(actual==previous,'unapproved asset metadata edit');restored=expected
        check(restored==expected,'unapproved physical/source/clock/guard edit: '+name)
        check(sha(raw)==record['source_hashes'][name],'actual source hash differs: '+name)
        inverse[name]=restored
        if raw!=expected:changed.append(name)
    check(sha(read(PHASE))==record['source_hashes'][PHASE],'phase file hash differs')
    rolled=dict(old,project=record['project'],source_hashes={n:sha(raw)for n,raw in inverse.items()})
    inverse['ready-prediction-integration.json']=json.dumps(rolled).encode()
    upstream=prior.audit(project,reports,inverse)
    return dict(status='PASS_NATIVE_PHASE_DISTRIBUTION_SOURCE',scope=__doc__,source_hashes=record['source_hashes'],
        verified_parent_inputs=662,actual_inputs=663,changed_parent_inputs=changed,
        original_pin_connections=upstream['original_pin_connections'],schema_id=upstream['schema_id'],
        new_required_wrapper_bits=19,new_clocks=0,new_external_ports=0,constraints_unchanged=True,
        ordinary_advance_phase_unchanged=True,raw_drain_unchanged=True,qualified_payload_instances=479,
        limits='Exact full-native source integration. State access, Main controls, remaining RAM/DMadr paths, routed timing and hardware qualification remain open.')


def controls(project,reports):
    cases=[('wrong_phase_file',PHASE,'always @(negedge clk)local_step','always @(posedge clk)local_step'),
        ('merged_phase_file',PHASE,'(* preserve, dont_merge *)','(* preserve *)'),
        ('missing_board',BACKPLANE,'.state_guest_advance(snapshot_board_advance[18])','.state_guest_advance(contb_advance)'),
        ('wrong_board',BACKPLANE,'.state_guest_advance(snapshot_board_advance[1])','.state_guest_advance(snapshot_board_advance[0])'),
        ('wrong_clock',BACKPLANE,'.clk(storage_clk),.permit(contb_permit)','.clk(sys_clk),.permit(contb_permit)'),
        ('pipelined_permission','core/Dorado.sv','.contb_permit(machine_enable)','.contb_permit(network_step)'),
        ('wrong_wrapper_permission',WRAPPER,'.contb_permit(contb_permit)','.contb_permit(contb_advance)'),
        ('lost_live_hold','core/Dorado.sv','snapshot_frontends_cached && !board_state_hold;','snapshot_frontends_cached;'),
        ('lost_payload_optin',PREFIX+'MemC__board.sv',',.PHASE_QUALIFIED(1)',''),
        ('lost_precap',PREFIX+'MemC__board.sv','.PRECAP(0)','.PRECAP(1)'),
        ('changed_payload',PREFIX+'MSA__cell_MC10176.sv','else if(state_advance)begin',"else if(1'b1)begin"),
        ('changed_ram',PREFIX+'MSA__storage_group.sv','waiting && rsp_valid,req_write','waiting && rsp_valid,write_pending'),
        ('timing_relaxed','core/Dorado.sdc','derive_pll_clocks','create_clock -period 1000 [get_ports FAKE_CLOCK]\nderive_pll_clocks')]
    result=[]
    for label,name,before,after in cases:
        original=(project/name).read_text();check(before in original,'missing control anchor: '+label)
        changed=original.replace(before,after,1);record=json.loads((project/'phase-distribution-integration.json').read_text())
        record['source_hashes'][name]=sha(changed.encode())
        try:audit(project,reports,{name:changed,'phase-distribution-integration.json':json.dumps(record)})
        except(ValueError,AssertionError,IndexError)as error:result.append(dict(case=label,rejected=True,diagnostic=str(error)))
        else:raise AssertionError('damaged native phase source accepted: '+label)
    return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('project',type=Path)
    p.add_argument('--reports',type=Path,default=ROOT/'fpga/mister/reports');p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    result=audit(a.project.resolve(),a.reports.resolve());result['failure_controls']=controls(a.project.resolve(),a.reports.resolve())
    a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(result,indent=2)+'\n')
    print(result['status'],result['actual_inputs'],'inputs;',len(result['failure_controls']),'broken controls rejected')
