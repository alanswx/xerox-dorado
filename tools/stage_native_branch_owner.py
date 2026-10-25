#!/usr/bin/env python3
"""Replace only four native ProcH flag storage cells with their common owner.

Bind every actual package clock, data and administration pin from the frozen
board. Preserve all other board bytes and existing registered clock buffers.
No reset, firmware, machine enable, SDRAM or timing-constraint change. This
private native stage remains an unqualified cadence/firmware experiment.
"""
import argparse
import json
from pathlib import Path
import re
import shutil

from qualify_native_branch_owner import FILES
from review_baseboard_clock_consumer import verify
from review_processor_branch_composition import checked
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1]
BOARD='src/fpga/mister/snapshot_native_processor/ProcH_full_snapshot.sv'
KINDS={'e13':'F10145A','f17':'MC10173','g11':'MC10176','h11':'MC10176'}


def transform(raw):
    blocks={};pins={};connections={}
    for pkg,kind in KINDS.items():
        matches=re.findall(r'^  cell_'+kind+r'_snapshot u_'+pkg+r' \(.*?^  \); // '+kind,raw,re.M|re.S)
        assert len(matches)==1,(pkg,len(matches));blocks[pkg]=matches[0]
        pins[pkg]={int(p):n for p,n in re.findall(r'\.p(\d+)\((\w+)\)',matches[0])}
        connections[pkg]=dict(re.findall(r'\.(state_\w+)\(([^()]*)\)',matches[0]))
        assert connections[pkg]['state_host_clk']=='state_host_clk'
        assert connections[pkg]['state_advance']=='state_guest_advance'
    assert 3 not in pins['e13']and 8 not in pins['e13']
    assert pins['f17'][7]=='Clock1_p_Cb'and pins['g11'][9]==pins['h11'][9]=='Clock1_p_Da'
    for pkg in ['f17','g11','h11']:
        assert connections[pkg]['state_hold']=='state_hold'and connections[pkg]['state_data']=='state_data'
    e=connections['e13'];assert e['state_freeze']=='state_freeze'and e['state_guest_held']=='state_guest_held'
    assert e['state_write']=='state_write'and e['state_index']=='state_row[4:0]'and e['state_data']=='state_data[9:0]'
    def cat(pkg,numbers):return '{'+','.join(pins[pkg][p]for p in numbers)+'}'
    data='{'+','.join([cat('e13',[12,11,4,5]),cat('e13',[10,9,7,6]),
        pins['f17'][9]+' ? '+cat('f17',[11,13,4,6])+':'+cat('f17',[10,12,3,5]),
        cat('h11',[12,11,10,7,6,5]),cat('g11',[12,11,10,7,6,5])])+'}'
    clocks='{'+','.join([pins['e13'][13],pins['f17'][7],pins['h11'][9],pins['g11'][9]])+'}'
    new=f'''  // One owner for the EXISTING native104-bit flag/history frontier.
  // Actual buffered package clocks remain; no raw-root reconstruction.
  wire [103:0] native_branch_physical;
  wire [3:0] native_branch_read;
  wire [3:0] native_branch_clocks={clocks};
  wire [23:0] native_branch_data={data};
  dorado_native_branch_owner native_branch_owner(
    .clk(state_host_clk),.advance(state_guest_advance),
    .clocks(native_branch_clocks),.data(native_branch_data),
    .ff_hold(state_hold),.ff_write({{{connections['h11']['state_write']},{connections['g11']['state_write']},{connections['f17']['state_write']}}}),
    .ff_data(state_data),.file_freeze(state_freeze),.file_guest_held(state_guest_held),
    .file_request({e['state_request']}),.file_write(state_write),
    .file_index(state_row[4:0]),.file_data(state_data[9:0]),
    .physical_state(native_branch_physical),.file_read(native_branch_read),
    .f17_result({connections['f17']['state_result']}),
    .g11_result({connections['g11']['state_result']}),.h11_result({connections['h11']['state_result']}),
    .file_frozen(),.file_done(),.file_error(),.file_result({e['state_result']}));
  assign alu_memory_e13=native_branch_physical[63:0];
'''
    for bit,pin in enumerate([2,1,15,14]):new+=f'  assign {pins["e13"][pin]}=native_branch_read[{bit}];\n'
    for pkg,offset,outs in [('f17',64,[1,2,15,14]),('g11',68,[2,3,4,13,14,15]),('h11',74,[2,3,4,13,14,15])]:
        for bit,pin in enumerate(outs):new+=f'  assign {pins[pkg][pin]}=native_branch_physical[{offset+bit}];\n'
    result=raw
    for pkg,block in blocks.items():result=result.replace(block,new if pkg=='e13'else'  // '+pkg+' storage belongs to native_branch_owner.',1)
    restored=result.replace(new,blocks['e13'],1)
    for pkg in ['f17','g11','h11']:restored=restored.replace('  // '+pkg+' storage belongs to native_branch_owner.',blocks[pkg],1)
    assert restored==raw
    return result,dict(original_blocks=blocks,actual_native_package_pins=pins,
        original_administration_connections=connections,actual_clock_expression=clocks,actual_data_expression=data,
        all_other_board_bytes_unchanged=True,existing_clock_buffer_registers_unchanged=True,
        no_guest_reset_or_initialization_added=True)


def prepare(parent,proof,out,project,interface_checks):
    assert not out.exists()and project.parent==Path('/home/alans/mister')
    p=json.loads((parent/'r-front-control-native.json').read_text());verify(parent,p['source_hashes'])
    assert not p['new_flag_owner_selected']and p['passive_actual_native_ALU_capture_connected']and not p['assembly_permitted']
    q=json.loads((proof/'run-state.json').read_text());verify(proof,q['files'])
    assert q['status']=='PASS_ACTUAL_NATIVE_BRANCH_COMMON_OWNER'and q['compared_outputs']==175 and q['assumptions']==0
    for a in q['actions']:
        d=proof/a['label'];verify(d,a['files']);assert a['passed']and a['returncode']==0 and sha(d/'proof.log')==a['log_sha256']
    for engine in ['icarus','verilator']:
        d=interface_checks[engine];r=checked(d)
        assert r['engine']==engine and r['status']=='PASS_ACTUAL_NATIVE_BRANCH_OWNER_INTERFACE'and r['proof_state_sha256']==sha(proof/'run-state.json')
    for n in FILES:assert sha(ROOT/n)==sha(proof/Path(n).name)
    after,binding=transform((parent/BOARD).read_text())
    output={n:(parent/n).read_bytes()for n in p['source_hashes']};output[BOARD]=after.encode()
    added={'src/'+n:(ROOT/n).read_bytes()for n in FILES};output.update(added)
    for n,raw in output.items():
        if Path(n).suffix in ['.qsf','.qip','.qpf','.sdc','.tcl']:output[n]=raw.replace(p['project'].encode(),str(project).encode())
    qsf=output['core/Dorado.qsf'].decode()
    for n in added:qsf+='\nset_global_assignment -name SYSTEMVERILOG_FILE '+str(project/n)+'\n'
    output['core/Dorado.qsf']=qsf.encode();out.mkdir(parents=True)
    for n,raw in output.items():
        dest=out/n;dest.parent.mkdir(parents=True,exist_ok=True);dest.write_bytes(raw)
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    record={**p,'status':'STAGED_PROCESSOR_CONTROL_NATIVE_EXPERIMENT','scope':__doc__,
        'project':str(project),'parent_project':p['project'],'parent_stage':str(parent),
        'parent_manifest_sha256':sha(parent/'r-front-control-native.json'),
        'source_hashes':{n:sha(out/n)for n in output},'extra_RTL_sources':sorted(set(p['extra_RTL_sources'])|set(added)),
        'native_branch_owner_proof':str(proof),'native_branch_owner_proof_state_sha256':sha(proof/'run-state.json'),
        'interface_checks':{engine:dict(directory=str(d),state_sha256=sha(d/'run-state.json'))for engine,d in interface_checks.items()},
        'actual_native_flag_owner_binding':binding,'new_flag_owner_selected':True,
        'native_common_flag_owner_selected_only_in_private_stage':True,'native_flag_owner_state_bits':104,
        'existing_PRECAP_and_write_window_protocols_retained':True,
        'baseline_clocks_timers_enables_media_and_constraints_unchanged':True,
        'native_source_phase_contract_qualified':False,'full60ns_event_batch_connected':False,
        'assembly_permitted':False,'hardware_changed':False,'firmware_qualified':False,'full_goal_active':True}
    (out/'r-front-control-native.json').write_text(json.dumps(record,indent=2)+'\n')
    print('STAGED_ACTUAL_NATIVE_BRANCH_COMMON_OWNER',len(output),'inputs; existing104-bit frontier; no timing or firmware qualification')
    return record


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['parent','proof','output','project']:p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--icarus-check',type=Path,required=True);p.add_argument('--verilator-check',type=Path,required=True)
    a=p.parse_args();prepare(a.parent.resolve(),a.proof.resolve(),a.output.resolve(),a.project,
        {'icarus':a.icarus_check.resolve(),'verilator':a.verilator_check.resolve()})
