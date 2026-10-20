#!/usr/bin/env python3
"""Stage qualified same-falling-edge advance copies in the full native source.

This driver has no Quartus/assembly/deployment mode. Preserve the existing
global gate, every unrelated source and all physical board state. Nineteen
new implementation bits require complete native state access and routed review.
"""
import argparse
import hashlib
import json
from pathlib import Path
from generate_serial_boards import ROOT,raw_receipt,once
from generate_native_ready_prediction import WRAPPER,BACKPLANE,PREFIX

PARENT='2026-10-20-snapshot-native-ready-prediction-rejected'
QUALIFIED='2026-10-20-snapshot-phase-distribution'
PHASE=PREFIX+'board_phase_enable.sv'
BLOCK='''  // Same falling-edge permission as Core's global gate; no extra stage.
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


def sha(raw):return hashlib.sha256(raw).hexdigest()


def qualified(reports):
    report=reports/QUALIFIED
    proof=json.loads(raw_receipt(report,'proof/complete-verification.json'))
    replay=json.loads(raw_receipt(report,'proof/extracted-replay.json'))
    assert proof['status']=='PASS_PHASE_BOARDS_NATIVE_CONTROL_AND_SDRAM_PROVENANCE'
    assert proof['boards_checked']==11 and proof['board_checks']==8399378 and proof['native_source_count']==662
    assert replay['status']=='PASS_EXTRACTED_PHASE_REPLAY'and len(replay['cases'])==19
    assert proof['eight_msa']['status']=='PASS_EIGHT_MSA_PHASE'and len(proof['eight_msa']['cases'])==3
    assets={};members={}
    for board in proof['boards']:
        name=board['board'];tag='boards-memc'if name=='MemC'else'boards-baseboard'if name=='BaseBd'else'boards-rest'
        names=[name+'__board.sv']+[name+'__cell_'+r['kind']+'_snapshot.v'for r in board['families']]
        for path in names:
            member=tag+'/'+name+'/inputs/'+path;raw=raw_receipt(report,member)
            assert sha(raw)==board['source_hashes'][path]
            assets[PREFIX+path]=raw;members[PREFIX+path]=member
    for dest,source in [('MSA__msa_board.sv','msa_board.sv'),('MSA__cell_MC10176.sv','cell_MC10176.sv'),
                        ('board_phase_enable.sv','board_phase_enable.sv')]:
        member='eight-msa/executed-inputs/'+source;raw=raw_receipt(report,member)
        assert sha(raw)==proof['eight_msa']['compiled_inputs'][source]
        assets[PREFIX+dest]=raw;members[PREFIX+dest]=member
    return assets,members,proof,replay


def core(text):
    return once(text,'.contb_advance(network_step),',
        '.contb_advance(network_step),\n    .contb_permit(machine_enable),')


def wrapper(text):
    text=once(text,'    input wire contb_advance,','    input wire contb_advance,contb_permit,')
    return once(text,'.contb_advance(contb_advance),',
        '.contb_advance(contb_advance),\n    .contb_permit(contb_permit),')


def backplane(text):
    text=once(text,'    input wire contb_advance,','    input wire contb_advance,contb_permit,')
    text=once(text,'  wire [11:0] snapshot_board_request',BLOCK+'  wire [11:0] snapshot_board_request')
    assert text.count('.state_guest_advance(contb_advance)')==19
    for index in range(19):text=text.replace('.state_guest_advance(contb_advance)',
        f'.state_guest_advance(snapshot_board_advance[{index}])',1)
    return text


def stage(out,reports,destination):
    parent=json.loads(raw_receipt(reports/PARENT,'terminal-fit/ready-prediction-integration.json'))
    assets,members,proof,replay=qualified(reports)
    old=parent['project'];new=str(destination or out);out.mkdir(parents=True,exist_ok=False);hashes={}
    for name,digest in parent['source_hashes'].items():
        raw=raw_receipt(reports/PARENT,'terminal-fit/'+name);assert sha(raw)==digest,name
        if Path(name).suffix in('.qsf','.qpf','.qip','.sdc','.tcl'):raw=raw.decode().replace(old,new).encode()
        if name in assets:raw=assets[name]
        elif name=='core/Dorado.sv':raw=core(raw.decode()).encode()
        elif name==WRAPPER:raw=wrapper(raw.decode()).encode()
        elif name==BACKPLANE:raw=backplane(raw.decode()).encode()
        p=out/name;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(raw);hashes[name]=sha(raw)
    (out/PHASE).write_bytes(assets[PHASE]);hashes[PHASE]=sha(assets[PHASE])
    qsf=out/'core/Dorado.qsf';text=qsf.read_text()
    text+=f'\nset_global_assignment -name SYSTEMVERILOG_FILE {new}/{PHASE}\n'
    qsf.write_text(text);hashes['core/Dorado.qsf']=sha(qsf.read_bytes())
    path=PREFIX+'assets.json';catalog=json.loads((out/path).read_text())
    for name in assets:catalog['assets'][Path(name).name]=hashes[name]
    catalog['phase_distribution']=dict(qualified_report=QUALIFIED,phase_bits=19,edge='negedge storage_clk',
        original_global_gate_preserved=True,board_only_schema_unchanged=True)
    (out/path).write_text(json.dumps(catalog,indent=2)+'\n');hashes[path]=sha((out/path).read_bytes())
    assert len(hashes)==663
    for name in('chipwidth-integration.json','ready-path-integration.json','memory-pause-integration.json',
                'network-pause-integration.json','disk-pause-integration.json','ready-prediction-integration.json'):
        (out/name).write_bytes(raw_receipt(reports/PARENT,'terminal-fit/'+name))
    record=dict(scope=__doc__,project=new,parent_project=old,parent_report=PARENT,qualified_report=QUALIFIED,
        verified_parent_inputs=parent['source_hashes'],source_hashes=hashes,qualified_asset_members=members,
        qualified_proof_sha256=sha(raw_receipt(reports/QUALIFIED,'proof/complete-verification.json')),
        qualified_replay_sha256=sha(raw_receipt(reports/QUALIFIED,'proof/extracted-replay.json')),
        new_required_wrapper_bits=19,new_clocks=0,new_external_ports=0,schema_id='af0b647cb43a3a62',
        quartus_permitted=False,assembly_permitted=False,full_snapshots=False,
        builder_sha256=sha(Path(__file__).read_bytes()),
        limits='Source-only native integration. Added-state access, actual Main controls, remaining RAM/DMadr timing paths, full native capacity/timing and FPGA qualification remain open.')
    (out/'phase-distribution-integration.json').write_text(json.dumps(record,indent=2)+'\n')
    print('Staged native phase distribution:',len(hashes),'inputs;19 required bits; no Quartus/assembly/deployment',flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--stage',type=Path,required=True)
    p.add_argument('--destination',type=Path);p.add_argument('--reports',type=Path,default=ROOT/'fpga/mister/reports')
    a=p.parse_args();stage(a.stage.resolve(),a.reports.resolve(),a.destination)
