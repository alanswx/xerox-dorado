#!/usr/bin/env python3
"""Readiness equivalence under the actual eight-MSA SPI owner and quarantine.

Guest clocks remain stopped; normal traffic is covered separately. Other board
endpoints and external service acknowledgments are the inherited explicit stubs.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import test_msa_chipwidth_transport as base
import generate_ready_path as change


def fixture(catalog):
    top,vectors=base.fixture(catalog);once=base.base.once
    declarations=' wire [7:0] member_ready,member_drained,member_hold;\n integer ready_checks=0,running_checks=0,held_checks=0;\n'
    top=once(top,' wire msa_frozen,msa_done,msa_error,msa_hold,msa_fault;',declarations+' wire msa_frozen,msa_done,msa_error,msa_hold,msa_fault;')
    top=once(top,'.storage_ready()', '.storage_ready(member_ready[u])')
    top=once(top,'.state_memory_drained()', '.state_memory_drained(member_drained[u])')
    top=once(top,'assign msa_physical[u]=','assign member_hold[u]=member.state_hold;\nassign msa_physical[u]=')
    old,new=change.expressions()
    old=old.replace('~reset',"1'b1").replace('pack_ready',"1'b1").replace('storage_ready','(&member_ready)').replace('board_state_hold','hold')
    new=new.replace('~reset',"1'b1").replace('pack_ready',"1'b1").replace('storage_initialized',"1'b1").replace('snapshot_frontends_ready','(&member_drained)').replace('board_state_hold','hold')
    body=f''' wire old_gate={old};
 wire new_gate={new};
 task check_readiness;begin
  ready_checks=ready_checks+1;
  if(old_gate!==new_gate)$fatal(1,"eight-board CPU readiness differs");
  if(hold===1'b0)begin
   if(member_hold!==8'b0 || (msa_unit_hold|msa_inject_hold)!==8'b0 ||
      (msa_unit_fault|msa_inject_fault)!==8'b0)$fatal(1,"global hold omitted a member");
   if(old_gate===1'b1)running_checks=running_checks+1;
  end
  if(hold===1'b1)begin
   held_checks=held_checks+1;
   if(new_gate!==1'b0)$fatal(1,"held guest enabled");
  end
 end endtask
'''
    top=once(top,' task tick;',body+' task tick;')
    top=once(top,'clk=0;#2;clk=1;#2;ticks=ticks+1;', 'clk=0;#2;check_readiness();clk=1;#2;check_readiness();ticks=ticks+1;')
    # Make the initial unheld interval observably ready. This is test setup,
    # before any saved state or transaction; subsequent SPI traffic is unchanged.
    seed=''
    for u in range(8):
        for i in range(16):
            seed+=f'  members[{u}].member.u_storage_{i}.cache_valid=1;\n'
    assert '  cold_reset();\n' in top
    top=top.replace('  cold_reset();\n', '  cold_reset();\n'+seed+'  idle(2);\n',1)
    top=once(top,'  $display("CHIPWIDTH MSA TRANSPORT PASS', '''  if(running_checks<1 || held_checks<100)$fatal(1,"readiness coverage missing");
  $display("READY PATH OWNER PASS checks=%0d ready=%0d held=%0d",ready_checks,running_checks,held_checks);
  $display("CHIPWIDTH MSA TRANSPORT PASS''')
    return top,vectors


def run(out,ethernet,mutations):
    out.mkdir(parents=True,exist_ok=False);inputs=out/'executed-inputs';inputs.mkdir()
    catalog=base.fmt.build_catalog(base.msa.base.ROOT/'fpga/mister/reports');top,vectors=fixture(catalog)
    board,manifest,helpers=base.msa.generate()
    # The chip-width generator already inherits the qualified normalization.
    assets={'msa_board.sv':board,**helpers,'msa_state_group_chipwidth.sv':base.group_source(),
            'board_state_fabric_msa.sv':base.fabric_source(),'board_state_uio_msa.sv':(base.HERE/'board_state_uio_msa.sv').read_text()}
    originals=base.msa.base.original_cells();originals.update({'msa_ddram.v':base.msa.base.original(base.msa.base.SOURCE),
        'dorado_storage_group.sv':base.msa.compact.original.original_source(),ethernet.name:ethernet.read_bytes()})
    for name,raw in originals.items():(inputs/name).write_bytes(raw)
    for p in (Path(__file__),Path(change.__file__),Path(base.__file__),Path(base.base.__file__),base.HERE/'test_board_state_serial.sv'):
        (inputs/p.name).write_bytes(p.read_bytes())
    cases=[('candidate',None,None,None,None)]
    if mutations:cases += [
        ('ignore_member_hold','msa_state_group_chipwidth.sv',"children_holding=(unit_hold_required!==8'b0)","children_holding=1'b0",'global hold omitted a member'),
        ('unknown_member_hold','msa_state_group_chipwidth.sv',"children_holding=(unit_hold_required!==8'b0)",'children_holding=|unit_hold_required','unknown MSA hold did not fail closed'),
        ('lost_global_guard','fixture.sv',new_guard(),"(&member_drained)",'eight-board CPU readiness differs'),
    ]
    results=[]
    for label,target,before,after,why in cases:
        case=out/label;case.mkdir();compiled=[]
        for name,body in {'fixture.sv':top,**assets}.items():
            if name==target:body=base.base.once(body,before,after)
            path=case/name;path.write_text(body);compiled.append(str(path))
        command=['iverilog','-g2012','-s','msa_transport_fixture','-o',str(case/'test.vvp'),*compiled,*[str(inputs/n)for n in originals]]
        (case/'command.json').write_text(json.dumps(command,indent=2)+'\n')
        with(case/'build.log').open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run(['vvp',str(case/'test.vvp')],capture_output=True,text=True,timeout=180)
        text=result.stdout+result.stderr;(case/'test.txt').write_text(text);print(label+': '+text.strip(),flush=True)
        if why:assert result.returncode and why in text,(label,text)
        else:assert not result.returncode and 'READY PATH OWNER PASS'in text,text
        results.append(dict(case=label,returncode=result.returncode,expected_failure=why))
    for n,v in [('catalog',catalog),('manifest',manifest),('results',dict(status='PASS',scope=__doc__,cases=results,
        executed_inputs={p.name:hashlib.sha256(p.read_bytes()).hexdigest()for p in inputs.iterdir()}))]:
        (out/(n+'.json')).write_text(json.dumps(v,indent=2)+'\n')


def new_guard():
    return '(&member_drained) && !hold'


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--ethernet',type=Path,required=True);p.add_argument('--mutations',action='store_true')
    a=p.parse_args();run(a.output.resolve(),a.ethernet.resolve(),a.mutations)
