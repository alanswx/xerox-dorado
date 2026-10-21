#!/usr/bin/env python3
"""Add output-only operand visibility to a qualified native R-front stage.

No CPU storage/control, clock, memory, protocol, Main or daemon change.
Status15 enables the diagnostic; status14:11 selects one of four32-bit pages.
The existing command70 snapshots that page together with the normal CPU PC.
Raw operand snapshots are not an instruction-consumption trace or a snapshot.
"""
import argparse
import hashlib
import json
from pathlib import Path

ROOT=Path(__file__).resolve().parents[1]
BP='src/build/rtl-executive/native16-tx-cable/dorado_native_backplane.v'
WRAPPER='src/build/rtl-executive/native16-tx-cable/dorado_native_machine.sv'
TOP='core/Dorado.sv'
PROCESSOR='src/fpga/mister/snapshot_native_processor/{}_full_snapshot.sv'


def sha(raw):return hashlib.sha256(raw).hexdigest()


def once(text,old,new):
    assert text.count(old)==1,old
    return text.replace(old,new)


def board(text,which):
    first=0 if which=='ProcH'else 8
    fields=[]
    phases=['RbBypassDly','Clock1_p_Bb','PreSHCP_p_B','Clock1_p_Ac','SHCP_p_C','TbSelMd']
    flags=['ResEqZero_p_','ResLtZero_p_']if which=='ProcH'else["1'b0","1'b0"]
    fields.append('{'+','.join(flags+phases)+'}')
    for prefix in ['T','alua','alub','Md','R','alu']:
        values=[f'{prefix}_{i:02d}'for i in range(first,first+8)]
        assert all(v in text for v in values)
        fields.append('{'+','.join(values)+'}')
    assert all(v in text for v in phases)
    declaration='    output wire [55:0] operand_debug,\n'
    text=once(text,'    output wire [7:0] r_front_visible_r,',declaration+'    output wire [7:0] r_front_visible_r,')
    assignment='  // Read-only physical operand/phase outputs; no new drivers of core nets.\n  assign operand_debug={'+','.join(fields)+'};\n'
    text=once(text,'endmodule',assignment+'endmodule')
    return text,[(declaration,''),(assignment,'')]


def backplane(text):
    changes=[]
    old='    output wire [31:0] processor_debug,'
    declaration='    output wire [127:0] operand_debug,\n'
    assert text.count(old)==2
    # Only the first module is instantiated by the native FPGA wrapper.
    text=text.replace(old,declaration+old,1);changes.append((declaration,''))
    for which in ['ProcH','ProcL']:
        old=f'{which}_full_snapshot_experiment b_{which} ('
        new=old+f'\n    .operand_debug(operand_debug_{which}),'
        text=once(text,old,new);changes.append((new,old))
    logic='  wire [55:0] operand_debug_ProcH,operand_debug_ProcL;\n'
    for i,name in enumerate(['t','a','b','md','r','alu']):
        hi=47-8*i;lo=hi-7
        logic+=f'  wire [15:0] operand_{name}={{operand_debug_ProcH[{hi}:{lo}],operand_debug_ProcL[{hi}:{lo}]}};\n'
    logic+='''  assign operand_debug={operand_t,operand_a,operand_b,operand_md,operand_r,operand_alu,
    processor_control[12:0],instruction_signals,operand_debug_ProcH[55:48],operand_debug_ProcL[53:48]};
'''
    text=text.replace('endmodule',logic+'endmodule',1);changes.append((logic,''))
    return text,changes


def wrapper(text):
    declaration='    output wire [127:0] operand_debug,\n'
    text=once(text,'    output wire [31:0] baseboard_debug,processor_debug,',declaration+'    output wire [31:0] baseboard_debug,processor_debug,')
    old='  dorado_native_backplane #(.SYSPER(SYSPER)) m ('
    new=old+'\n    .operand_debug(operand_debug),'
    return once(text,old,new),[(declaration,''),(new,old)]


def top(text):
    logic='''// Read-only operand monitor. The guest never reads this selector or payload.
`ifdef DORADO_OPERAND_MONITOR
wire [127:0] native_operand_debug;
wire operand_monitor_active=status[15] && !pack_fault;
wire [31:0] operand_word=status[14:11]<4
    ? native_operand_debug[127-32*status[14:11]-:32] : 32'hd0d0bad0;
`else
wire operand_monitor_active=1'b0;
wire [31:0] operand_word=32'd0;
`endif
'''
    needle='dorado_ethernet_uio #(.MAX_WORDS(1024)'
    text=once(text,needle,logic+needle);changes=[(logic,'')]
    for old,new in [
        ('speed_monitor_active ? count_debug :','operand_monitor_active ? operand_word : speed_monitor_active ? count_debug :'),
        ('speed_monitor_active ? count_reference : processor_debug','operand_monitor_active ? processor_debug : speed_monitor_active ? count_reference : processor_debug')]:
        assert text.count(old)==4,old
        text=text.replace(old,new);changes.append((new,old))
    old='dorado_native_machine #(.SYSPER(MACHINE_SYSPER)) u_native ('
    addition='\n`ifdef DORADO_OPERAND_MONITOR\n    .operand_debug(native_operand_debug),\n`endif'
    text=once(text,old,old+addition);changes.append((old+addition,old))
    return text,changes


def make_outputs(parent,project):
    record=json.loads((parent/'r-front-ready-native.json').read_text())
    proof=json.loads((parent/'qualification.json').read_text())
    assert proof['status']=='PASS_NATIVE_R_FRONT_READY_PREASSEMBLY_REVIEW'
    assert proof['source_hashes']==record['source_hashes']
    assert record['native_visible_R_bits']==16 and record['evaluator_owned_Q_bits']==736
    assert record['frequency_mhz']==50 and record['sysper']==4 and record['state_admin']=='INACTIVE'
    outputs={}
    for name,key in record['source_hashes'].items():
        raw=(parent/name).read_bytes();assert sha(raw)==key,name
        if Path(name).suffix in ['.qsf','.qip','.qpf','.sdc','.tcl']:
            raw=raw.replace(record['project'].encode(),str(project).encode())
        outputs[name]=raw
    original=dict(outputs);undo={}
    assert b'set_global_assignment -name SYSTEMVERILOG_FILE Dorado.sv\n'in outputs['core/Dorado.qsf'], 'Unexpected compiled emu source assignment'
    for which in ['ProcH','ProcL']:
        name=PROCESSOR.format(which);text,changes=board(outputs[name].decode(),which)
        outputs[name]=text.encode();undo[name]=changes
    for name,change in [(BP,backplane),(WRAPPER,wrapper),(TOP,top)]:
        text,changes=change(outputs[name].decode());outputs[name]=text.encode();undo[name]=changes
    tail=b'\n# Read-only operands through existing command70; no Main/protocol change.\nset_global_assignment -name VERILOG_MACRO DORADO_OPERAND_MONITOR\n'
    outputs['core/Dorado.qsf']+=tail
    for name,changes in undo.items():
        text=outputs[name].decode()
        for added,previous in reversed(changes):
            assert added in text,(name,added)
            text=text.replace(added,previous)
        assert text.encode()==original[name],('Observer changes core logic',name)
    assert outputs['core/Dorado.qsf'][:-len(tail)]==original['core/Dorado.qsf']
    changed={n for n in outputs if outputs[n]!=original[n]}
    assert changed==set(undo)|{'core/Dorado.qsf'}
    return record,outputs,changed


def stage(parent,out,project):
    assert project.is_absolute()and not out.exists()
    record,outputs,changed=make_outputs(parent,project)
    out.mkdir(parents=True)
    for name,raw in outputs.items():
        p=out/name;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(raw)
    record.update(project=str(project),source_hashes={n:sha(raw)for n,raw in outputs.items()},
        passive_operand_monitor=True,operand_monitor_parent_manifest_sha256=sha((parent/'r-front-ready-native.json').read_bytes()))
    (out/'r-front-ready-native.json').write_text(json.dumps(record,indent=2)+'\n')
    report=dict(status='PASS_OUTPUT_ONLY_OPERAND_MONITOR_SOURCE',parent=str(parent),
        compiled_top=TOP,
        parent_manifest_sha256=record['operand_monitor_parent_manifest_sha256'],changed=sorted(changed),
        core_logic_identical_after_removing_exact_output_only_additions=True,
        guest_inputs_state_clocks_memory_protocol_changed=False,new_sequential_logic=False,
        Main_changed=False,daemon_changed=False,firmware_qualified=False,timing_qualified=False,
        pages={'0':'T[31:16],ALUA[15:0]','1':'ALUB[31:16],Md[15:0]',
               '2':'R[31:16],ALU[15:0]','3':'Control13,Instruction5,Hflags8,Lphases6'},
        input_hashes={n:sha((parent/n).read_bytes())for n in changed},
        output_hashes={n:sha(outputs[n])for n in changed})
    (out/'operand-source-review.json').write_text(json.dumps(report,indent=2)+'\n')
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    print(report['status'],len(changed),'files; no new storage/protocol/Main/clock logic')


def verify(parent,candidate):
    """Reconstruct every compiled byte from the independently qualified parent."""
    actual=json.loads((candidate/'r-front-ready-native.json').read_text())
    record,outputs,changed=make_outputs(parent,Path(actual['project']))
    record.update(project=actual['project'],source_hashes={n:sha(raw)for n,raw in outputs.items()},
        passive_operand_monitor=True,operand_monitor_parent_manifest_sha256=sha((parent/'r-front-ready-native.json').read_bytes()))
    assert actual==record,'Operand candidate metadata does not reconstruct from its qualified parent'
    for n,raw in outputs.items():assert(candidate/n).read_bytes()==raw,('Operand observer changes qualified source',n)
    proof=json.loads((candidate/'operand-source-review.json').read_text())
    assert proof['status']=='PASS_OUTPUT_ONLY_OPERAND_MONITOR_SOURCE'
    assert proof['compiled_top']==TOP=='core/Dorado.sv'
    assert proof['parent_manifest_sha256']==record['operand_monitor_parent_manifest_sha256']
    assert proof['changed']==sorted(changed)and proof['core_logic_identical_after_removing_exact_output_only_additions']
    assert all(proof[k]is False for k in ['guest_inputs_state_clocks_memory_protocol_changed','new_sequential_logic','Main_changed','daemon_changed'])
    assert proof['input_hashes']=={n:sha((parent/n).read_bytes())for n in changed}
    assert proof['output_hashes']=={n:sha(outputs[n])for n in changed}
    return proof


def compiled_sources(mapping,record,proof):
    """Bind every changed HDL file to Quartus's used-in-netlist inventory."""
    title='; Analysis & Synthesis Source Files Read'
    assert title in mapping
    table=mapping[mapping.index(title):].split('\n\n',1)[0]
    rows={}
    for line in table.splitlines():
        fields=[v.strip()for v in line.split(';')[1:-1]]
        if len(fields)==5 and fields[1]in ['yes','no']:
            assert fields[3]not in rows,('Duplicate compiled source row',fields[3])
            rows[fields[3]]=fields
    result={}
    assert proof['compiled_top']==TOP=='core/Dorado.sv'
    for name in proof['changed']:
        if name.endswith('.qsf'):continue
        absolute=record['project']+'/'+name
        assert absolute in rows,('Observer HDL was not compiled',name)
        row=rows[absolute]
        assert row[1]=='yes'and row[2]in ['User Verilog HDL File','User SystemVerilog HDL File'],row
        result[name]=dict(absolute_path=absolute,used_in_netlist=True,sha256=record['source_hashes'][name])
    assert set(result)=={TOP,BP,WRAPPER,PROCESSOR.format('ProcH'),PROCESSOR.format('ProcL')}
    return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for name in ['parent','output','project']:p.add_argument('--'+name,type=Path,required=True)
    a=p.parse_args();stage(a.parent.resolve(),a.output.resolve(),a.project)
