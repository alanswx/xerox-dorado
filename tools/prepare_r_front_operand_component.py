#!/usr/bin/env python3
"""Replay a frozen passing original-image component with exact operand ports.

Only output-only board ports/connections and passive alias checks are added.
The original-ROM/packet/pack setup and guest execution remain unchanged.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path
import shutil

from stage_r_front_operand_monitor import BP,PROCESSOR,once

ROOT=Path(__file__).resolve().parents[1]


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def prepare(parent,native,out,initial_reference=None):
    assert not out.exists()
    state=json.loads((parent/'run-state.json').read_text())
    assert state['status']=='TERMINAL_PILOT_COMPONENT_DIAGNOSTIC'
    assert all(a['returncode']==0 for a in state['actions'])
    record=json.loads(gzip.decompress((parent/'provenance.json.gz').read_bytes()))
    for n,h in record['inputs'].items():assert sha(parent/'executed-inputs'/n)==h,n
    stage=json.loads((native/'r-front-ready-native.json').read_text())
    assert stage['passive_operand_monitor']and stage['state_admin']=='INACTIVE'
    for n,h in stage['source_hashes'].items():assert sha(native/n)==h,n
    out.mkdir(parents=True);inputs=out/'executed-inputs'
    shutil.copytree(parent/'executed-inputs',inputs)
    changed=[]
    for which in ['ProcH','ProcL']:
        name=PROCESSOR.format(which)[4:]
        (inputs/name).write_bytes((native/('src/'+name)).read_bytes());changed.append(name)
        record['native_visible_r_sources'][name]['resulting_sha256']=sha(inputs/name)
    top=inputs/'verilog/generated/dorado_full.v';text=top.read_text()
    for which in ['ProcH','ProcL']:
        old=f'{which}_full_snapshot_experiment b_{which} ('
        text=once(text,old,old+f'\n    .operand_debug(operand_debug_{which}),')
    old='ContA_full_snapshot_experiment b_ContA ('
    text=once(text,old,old+'\n    .processor_control(processor_control),\n    .instruction_signals(instruction_signals),')
    bp=(native/BP).read_text()
    marker='  wire [55:0] operand_debug_ProcH,operand_debug_ProcL;'
    logic=marker+bp.split(marker,1)[1].split('endmodule',1)[0]
    declarations='  wire [127:0] operand_debug;\n  wire [14:0] processor_control;\n  wire [4:0] instruction_signals;\n'
    text=once(text,'  // Physical state only;',declarations+'  // Physical state only;')
    # The generated file also contains an unused convenience wrapper. Add
    # observation only to its first, actual dorado_full module.
    assert text.count('endmodule')==2
    text=text.replace('endmodule',logic+'endmodule',1);top.write_text(text)
    changed.append(str(top.relative_to(inputs)))
    bench=inputs/'verilog/verilator/tb_exec.sv'
    vectors=[]
    for prefix in ['T','alua','alub','Md','R','alu']:
        vectors.append('{'+','.join(f'm.b_Proc{"H"if i<8 else"L"}.{prefix}_{i:02d}'for i in range(16))+'}')
    phases=['RbBypassDly','Clock1_p_Bb','PreSHCP_p_B','Clock1_p_Ac','SHCP_p_C','TbSelMd']
    tail=['m.processor_control[12:0]','m.instruction_signals',
          'm.b_ProcH.ResEqZero_p_','m.b_ProcH.ResLtZero_p_']
    tail += [f'm.b_{which}.{n}'for which in ['ProcH','ProcL']for n in phases]
    expected='{'+','.join(vectors+tail)+'}'
    monitor='''
  // Passive pin-level comparison of the exact staged output-only wiring.
  longint operand_monitor_checks=0;
  always @(negedge sys_clk)if(paritysources_arm && (n_cyc2%8)==3)begin
    operand_monitor_checks++;
    if(m.operand_debug !== EXPECTED)
      $fatal(1,"OPERAND_MONITOR_PIN_ALIAS_MISMATCH sample=%0d",n_cyc2);
  end
  final begin
    if(operand_monitor_checks<100000)$fatal(1,"Insufficient operand monitor coverage");
    $display("OPERAND_MONITOR_PIN_ALIAS_PASS checks=%0d",operand_monitor_checks);
  end
'''.replace('EXPECTED',expected)
    bench.write_text(once(bench.read_text(),'endmodule',monitor+'\nendmodule'))
    changed.append(str(bench.relative_to(inputs)))
    for name in [Path(__file__).name,'stage_r_front_operand_monitor.py']:
        shutil.copy2(ROOT/'tools'/name,inputs/name)
    record['commands']=[[str(arg).replace(str(parent),str(out))for arg in command]for command in record['commands']]
    # The existing test runner reads this small file for its bounded limit.
    shutil.copy2(parent/'cycle-limit.txt',out/'cycle-limit.txt')
    if initial_reference:
        reference=json.loads(gzip.decompress((initial_reference/'provenance.json.gz').read_bytes()))
        state=json.loads((initial_reference/'run-state.json').read_text())
        assert state['status']=='TERMINAL_RECEIVE_FILTER_PROBE'and reference['probe_only']
        assert all(a['returncode']==0 for a in state['actions'])
        for key in ['external_inputs','native_io_inputs','native_memory_inputs','native_ifu_inputs']:
            assert reference[key]==record[key],key
        record['commands'][1]=[str(arg).replace(str(initial_reference),str(out))for arg in reference['commands'][1]]
        for key in ['probe_only','handoff_only','handoff_trace']:
            record[key]=reference[key]
        record['initial_reference_parent']=str(initial_reference)
        shutil.copy2(initial_reference/'cycle-limit.txt',out/'cycle-limit.txt')
    record.update(operand_monitor_native_manifest_sha256=sha(native/'r-front-ready-native.json'),
                  operand_monitor_parent=str(parent),operand_monitor_parent_provenance_sha256=sha(parent/'provenance.json.gz'),
                  changed_component_inputs=changed)
    record['inputs']={str(p.relative_to(inputs)):sha(p)for p in inputs.rglob('*')if p.is_file()}
    (out/'provenance.json.gz').write_bytes(gzip.compress((json.dumps(record,indent=2)+'\n').encode(),mtime=0))
    print('PREPARED_OPERAND_PORT_ORIGINAL_IMAGE_COMPONENT',len(record['inputs']),'inputs; unchanged guest fixture')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['parent','native','output']:p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--initial-reference',type=Path,help='Reuse a terminal Initial/map probe runtime with these exact new board sources')
    a=p.parse_args();prepare(a.parent.resolve(),a.native.resolve(),a.output.resolve(),a.initial_reference.resolve()if a.initial_reference else None)
