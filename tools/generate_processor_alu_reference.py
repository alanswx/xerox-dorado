#!/usr/bin/env python3
"""Close the real16-bit ALU/function-memory path in the original processor.

Preserve the existing924-state layout, append both whole function RAM packages
and both six-bit registers. Optionally close the supported operand registers,
appending their physical state without changing existing1063:0 indices.
Backplane nets resolve all selected board contributions before feeding either
board. This is an original-cell reference, not a native optimized replacement.
"""
import argparse
import json
from pathlib import Path
import re
import shutil

from generate_processor_branch_reference import generate as branch_generate,ROOT,BUILD
from generate_processor_capture_reference import graph
from generate_r_front_cone import PIN_Q
from sil_netlist import load_board
from sil_to_verilog import vname
from test_external_r_packet import sha

NATIVE=BUILD/'native-cp-read-j19-boot-v1-20261025/inputs'
BP='src/build/rtl-executive/native16-tx-cable/dorado_native_backplane.v'
SEQUENTIAL=set(PIN_Q)|{'MC10175','MB7071H','F10414','F10016'}
NEW_LOW={'e12':(408,6),'e13':(414,64),'e14':(478,64),'e17':(542,6)}


def original_boards():
    m=json.loads((BUILD/'r-front-control-native-inventory-v1-20261023/inputs/reference/manifest.json').read_text())
    return {r['board'].split('-Rev-')[0]:r for r in m['boards']if r['board'].startswith(('ProcH','ProcL'))}


def closure(boards,specs,close_operands=False):
    selected={n:set(s['seq'])|set(s['comb'])for n,s in specs.items()}
    initial={n:set(s)for n,s in selected.items()};added={n:set()for n in boards}
    queue=[('ProcH',n)for n in ['SignedCarry','aluCout',"aluOut=0'",'alu.00']]+[('ProcL','aluM')]
    for pkg in NEW_LOW:
        queue += [('ProcL',n)for n,r in boards['ProcL'].nets.items()if any(p['pkg']==pkg and p['dir']=='in'for p in r['pins'])]
    # Include every wired contribution to a net consumed by this graph,
    # including contributions on a package not otherwise in the ALU cone.
    for name,board in boards.items():
        queue += [(name,n)for n,r in board.nets.items()
                  if any(p['dir']=='out'and p['pkg']in selected[name]for p in r['pins'])
                  and any(p['dir']=='in'and p['pkg']in selected[name]for p in r['pins'])]
    seen=set();stopped=set()
    while queue:
        name,net=queue.pop()
        if(name,net)in seen:continue
        seen.add((name,net));row=boards[name].nets[net]
        if net.startswith(('GND','VCC','VDD','Vdd')):continue
        drivers=[p for p in row['pins']if p['dir']=='out']
        if close_operands and net=='DMuxData':
            # This diagnostic bus includes nine other boards. Native ProcH
            # explicitly reads the bus without its own contribution. Keep
            # those two *different* external read boundaries; do not invent
            # a closed H/L-only wired bus or a self-feedback path.
            stopped.update((name,net,p['pkg'],boards[name].packages[p['pkg']]['type'])for p in drivers)
            continue
        if row.get('backplane'):
            other='ProcL'if name=='ProcH'else'ProcH';r=boards[other].nets.get(net,{})
            if r.get('backplane')and any(p['dir']=='out'for p in r.get('pins',[])):
                queue.append((other,net))
        for driver in drivers:
            pkg=driver['pkg'];kind=boards[name].packages[pkg]['type']
            if pkg in selected[name]:continue
            # The temperature sensor is an analog machine input, not a gate
            # whose state or transfer can be inferred from this digital graph.
            if (kind in SEQUENTIAL and (not close_operands or kind not in PIN_Q)) or kind in {'SG10139','LM3911+20K'}:
                stopped.add((name,net,pkg,kind));continue
            selected[name].add(pkg);added[name].add(pkg)
            queue += [(name,n)for n,r in boards[name].nets.items()if any(p['pkg']==pkg and p['dir']=='in'for p in r['pins'])]
    operand_packages={}
    for name,spec in specs.items():
        sequential=sorted(p for p in added[name]if boards[name].packages[p]['type']in PIN_Q)
        operand_packages[name]={}
        end=max(offset+width for offset,width in spec['seq'].values())
        for pkg in sequential:
            kind=boards[name].packages[pkg]['type'];width=PIN_Q[kind][0]
            spec['seq'][pkg]=(end,width);operand_packages[name][pkg]=(end,width);end+=width
            taps={'MC10231':['ca','cb'],'SE10231':['ca','cb'],'MC10141':['p4'],
                  'MC10173':['p7'],'MC10176':['p9'],'MC1668':['p9'],
                  'F10000':['clk'],'F10145A':['p13']}[kind]
            spec['clock_taps'] += ['@'+pkg+'.'+tap for tap in taps]
        spec['comb']=sorted((set(spec['comb'])|added[name])-set(sequential))
    # A local wired net must never silently omit an unselected driver.
    for name,board in boards.items():
        for net,row in board.nets.items():
            drivers={p['pkg']for p in row['pins']if p['dir']=='out'}
            used=any(p['dir']=='in'and p['pkg']in selected[name]for p in row['pins'])
            if used and drivers&selected[name]:assert drivers<=selected[name],(name,net,drivers-selected[name])
    return dict(added_combinational={n:sorted(s)for n,s in added.items()},
                remaining_operand_storage_boundaries=sorted(stopped),
                closed_operand_packages=operand_packages,
                original_packages={n:sorted(s)for n,s in initial.items()})


def generate(out,close_operands=False):
    assert not out.exists();out.mkdir(parents=True)
    base=branch_generate(out/'base');timed=out/'timed';shutil.copytree(out/'base/timed',timed)
    old=original_boards();h=base['original_high_board'];low=old['ProcL']
    specs={
        'ProcH':dict(bn=h['board'],module='processor_alu_proch',comb=sorted(set(h['packages'])-set(h['sequential'])),
                     seq=dict(h['sequential']),bindings={},clock_taps=list(h['clock_taps']),flag_taps=h['flag_taps']),
        'ProcL':dict(bn=low['board'],module='processor_alu_procl',comb=sorted(set(low['packages'])-set(low['sequential'])),
                     seq={**low['sequential'],**NEW_LOW},bindings={},clock_taps=low['clock_taps']+['@e12.p9','@e13.p13','@e14.p13','@e17.p9'],flag_taps=low['flag_taps'])}
    boards={n:load_board(str(ROOT/'chm/sil'/(r['board']+'.dm!1_')/(r['board']+'.wl')))for n,r in old.items()}
    support=closure(boards,specs,close_operands)
    high_bits=max(a+b for a,b in specs['ProcH']['seq'].values())
    low_bits=max(a+b for a,b in specs['ProcL']['seq'].values())
    physical_bits=1064+(high_bits-360)+(low_bits-548)
    high_clocks=len(specs['ProcH']['clock_taps']);low_clocks=len(specs['ProcL']['clock_taps'])
    clock_channels=108+(high_clocks-19)+(low_clocks-41)
    selected={n:set(s['seq'])|set(s['comb'])for n,s in specs.items()}
    # Global nets retain all board drivers, including the two zero-detector
    # contributions and ProcL's e17/f61 wired carry input.
    global_drivers={}
    for name,board in boards.items():
        for net,row in board.nets.items():
            drivers=[p for p in row['pins']if p['dir']=='out'and p['pkg']in selected[name]]
            if row.get('backplane')and drivers:global_drivers.setdefault(net,[]).extend((name,p['pkg'],p['pin'])for p in drivers)
    assert set(global_drivers["aluOut=0'"])=={(n,'d08',p)for n in ['ProcH','ProcL']for p in [2,15]}
    # ALUF is already registered in the original common ContB MIR owner.
    # Do not replace those four real producers with independent test pins.
    control_nodes={net:wire for wire,net in re.findall(r'^ wire (\w+); // ([^\n]+)$',(timed/'contb_mir_reference.sv').read_text(),re.M)}
    selector_sources={}
    for bit,(pkg,pin)in enumerate([('j24',15),('j24',2),('k24',15),('k24',2)]):
        net=f'ALUF.{bit}';assert net not in global_drivers
        global_drivers[net]=[('ContB',pkg,pin)];selector_sources[net]='m.b.'+control_nodes[net]
    globals_index={n:i for i,n in enumerate(sorted(global_drivers))}
    old_bindings={'ProcH':h['boundary_bindings'],'ProcL':low['boundary_bindings']}
    extras={};next_pin=413
    records={};nodes={};driver_expressions={};port_checks=[]
    for name,spec in specs.items():
        external=graph(ROOT,out=None,inspect=True,**spec)
        bindings={}
        for net in external:
            if net in globals_index and boards[name].nets[net].get('backplane'):
                bindings[net]=f'bridge[{globals_index[net]}]'
            elif net in old_bindings[name]:bindings[net]=old_bindings[name][net]
            else:
                token=net if boards[name].nets[net].get('backplane')and net!='DMuxData'else name+'.'+net
                if token not in extras:extras[token]=next_pin;next_pin+=1
                bindings[net]=f'pins[{extras[token]}]'
        spec['bindings']=bindings
        records[name]=graph(ROOT,out=timed,**spec)
        path=timed/(spec['module']+'.sv');raw=path.read_text()
        nodes[name]={net:wire for wire,net in re.findall(r'^ wire (\w+); // ([^\n]+)$',raw,re.M)}
        for net,index in globals_index.items():
            if net not in nodes[name]or not boards[name].nets[net].get('backplane'):continue
            wire=nodes[name][net];pattern=r'^ assign '+wire+r'=([^;]+);$'
            matches=re.findall(pattern,raw,re.M);assert len(matches)==1,(name,net)
            if any(b==name for b,p,k in global_drivers[net]):
                driver_expressions[name,net]=matches[0]
                raw=re.sub(pattern,f' assign {wire}=bridge[{index}];',raw,count=1,flags=re.M)
        # Preserve the native per-instance parity polarity, especially d13's
        # SignedCarry XOR. The generic cell's compatibility default is1.
        native=(NATIVE/f'src/fpga/mister/snapshot_native_processor/{name}_full_snapshot.sv').read_text()
        for pkg in spec['comb']:
            kind=boards[name].packages[pkg]['type']
            if kind=='MC10170'and re.search(r'cell_MC10170 #\(\.LEGACY_B_INVERT\(0\)\) u_'+pkg+r' \(',native):
                marker='cell_MC10170 '+pkg+'(';assert raw.count(marker)==1
                raw=raw.replace(marker,'cell_MC10170 #(.LEGACY_B_INVERT(0)) '+pkg+'(',1)
        for pkg in spec['seq']:
            if boards[name].packages[pkg]['type']!='F10000':continue
            instance=re.findall(r' original_F10000 '+pkg+r'\([^\n]+',raw)
            assert len(instance)==1
            if ".p4(1'b0)"not in instance[0]:
                # g16's narrower oracle rejects an asserted p4 because its
                # physical p4 is open. l16/l18 have TWO wired clock inputs;
                # use the existing parallel/OR-clock oracle, retaining its
                # serial/reset/parallel-mode assertions.
                raw=raw.replace(instance[0],instance[0].replace('original_F10000 ','original_F10000_gated ',1),1)
        path.write_text(raw)
    for name,spec in specs.items():
        path=timed/(spec['module']+'.sv');raw=path.read_text()
        before='input wire [27:0] pins';after=f'input wire [{next_pin-1}:0] pins,input wire [{len(globals_index)-1}:0] bridge,input wire [3:0] next_raw,input wire priority_n,freeze,rb_forward,rb_forward_n'
        assert raw.count(before)==1;path.write_text(raw.replace(before,after,1))
    # Check selected cross-board connector identities against the actual
    # native backplane's global names, not just coincident local net names.
    bp=(NATIVE/BP).read_text()
    for bit in range(4):assert f'assign ALUF_{bit} = ALUF_{bit}__ContB;'in bp
    for net in globals_index:
        name=vname(net)
        for board in specs:
            if net not in boards[board].nets or not boards[board].nets[net].get('backplane'):continue
            module=re.findall(r'  '+board+r'_full_snapshot_experiment b_'+board+r' \(.*?\n  \);',bp,re.S)
            assert len(module)==1,board
            if re.search(r'\.'+re.escape(name)+r'\(',module[0]):
                assert '.'+name+'('+name+')'in module[0],(board,net)
                port_checks.append([board,net,name])
    top=(out/'base/timed/processor_branch_reference.sv').read_text()
    replacements=[('module processor_branch_reference','module processor_alu_reference'),('[923:0]',f'[{physical_bits-1}:0]'),('[412:0]',f'[{next_pin-1}:0]'),
        ('[103:0] clocks',f'[{clock_channels-1}:0] clocks'),('wire [36:0] cl',f'wire [{low_clocks-1}:0] cl'),('wire [407:0] sl,il',f'wire [{low_bits-1}:0] sl,il'),
        ('processor_branch_proch h(','processor_alu_proch h('),('r_front_ready_procl l(','processor_alu_procl l('),
        ('assign clocks={ch[18:15],','assign clocks={cl[40:37],ch[18:15],'),
        ('output wire [3:0] branch_flags);','output wire [3:0] branch_flags,output wire [15:0] alu_word,output wire [5:0] alu_function,output wire [400:0] legacy_pins);')]
    for before,after in replacements:assert before in top,before;top=top.replace(before,after)
    if close_operands:
        assert high_bits>360 and low_bits>548
        top=top.replace('wire [359:0] sh,ih',f'wire [{high_bits-1}:0] sh,ih',1)
        marker='wire [18:0] ch';assert top.count(marker)==1
        top=top.replace(marker,f'wire [{high_clocks-1}:0] ch',1)
        marker='assign clocks={cl[40:37],ch[18:15],'
        assert top.count(marker)==1
        top=top.replace(marker,f'assign clocks={{cl[{low_clocks-1}:41],ch[{high_clocks-1}:19],cl[40:37],ch[18:15],',1)
    for before,after in [('cpu_pins,next_raw','cpu_pins,bridge,next_raw'),('cpu_pins[245:0],next_raw','cpu_pins,bridge,next_raw')]:
        assert top.count(before)==1,(before,top.count(before));top=top.replace(before,after,1)
    declaration=f' wire [{len(globals_index)-1}:0] bridge;\n'
    marker=' wire [2:0] bases;';assert top.count(marker)==1
    top=top.replace(marker,declaration+marker,1)
    extra=''
    for net,index in globals_index.items():
        terms=[selector_sources[net]]if net in selector_sources else[]
        for name in specs:
            expr=driver_expressions.get((name,net))
            if expr:
                scope='h'if name=='ProcH'else'l'
                terms.append(re.sub(r'\bn\d+_\w+_\d+\b',lambda m:scope+'.'+m[0],expr))
        assert terms,net
        extra+=f' assign bridge[{index}]='+ ' | '.join('('+x+')'for x in terms)+';\n'
    extra+=' assign il[547:408]=fixture_state[1063:924];\n assign state_q[1063:924]=sl[547:408];\n'
    if close_operands:
        high_extra=high_bits-360
        extra+=f' assign ih[{high_bits-1}:360]=fixture_state[{1063+high_extra}:1064];\n'
        extra+=f' assign state_q[{1063+high_extra}:1064]=sh[{high_bits-1}:360];\n'
        extra+=f' assign il[{low_bits-1}:548]=fixture_state[{physical_bits-1}:{1064+high_extra}];\n'
        extra+=f' assign state_q[{physical_bits-1}:{1064+high_extra}]=sl[{low_bits-1}:548];\n'
    for bit in range(16):
        board='ProcH'if bit<8 else'ProcL';scope='h'if bit<8 else'l'
        extra+=f' assign alu_word[{15-bit}]={scope}.{nodes[board][f"alu.{bit:02d}"]};\n'
    extra+=' assign alu_function=sl[413:408];\n'
    # Feed an independent old844 circuit from the newly closed physical
    # boundaries. Its own registers and SRAM must still compare bit-for-bit.
    legacy={}
    for name in specs:
        for net,expression in old_bindings[name].items():
            match=re.fullmatch(r'pins\[(\d+)\]',expression)
            if match and net in nodes[name]:
                pin=int(match[1]);scope='h'if name=='ProcH'else'l'
                drivers=boards[name].nets[net]['pins']
                closed=any(p['dir']=='out'and p['pkg']in selected[name]for p in drivers)or(net in globals_index and boards[name].nets[net].get('backplane'))
                if closed:legacy.setdefault(pin,[]).append(scope+'.'+nodes[name][net])
    for pin in range(401):extra+=f' assign legacy_pins[{pin}]='+legacy.get(pin,[f'pins[{pin}]'])[0]+';\n'
    assert top.count('endmodule')==1;top=top.replace('endmodule',extra+'endmodule',1)
    (timed/'processor_alu_reference.sv').write_text(top)
    # Independently owned80-bit flag family, with the same complete ALU/core
    # gates around it. Only the four flag packages become read-only views in
    # this second core; separate original primitives own/update their state.
    split=(timed/'processor_alu_proch.sv').read_text()
    split=split.replace('module processor_alu_proch','module processor_alu_proch_split',1)
    marker='input wire [3:0] next_raw';assert split.count(marker)==1
    split=split.replace(marker,'input wire [79:0] saved_q,'+marker,1)
    for pkg,(start,width)in {'e13':(0,64),'f17':(64,4),'g11':(68,6),'h11':(74,6)}.items():
        kind=boards['ProcH'].packages[pkg]['type']
        line=re.findall(r' original_'+kind+' '+pkg+r'\([^\n]+',split);assert len(line)==1,pkg
        changed=line[0].replace('original_','branch_sampled_',1)
        field=f'.fixture_state(fixture_state[{280+start}+:{width}])'
        assert changed.count(field)==1;changed=changed.replace(field,f'.fixture_state(saved_q[{start}+:{width}])',1)
        split=split.replace(line[0],changed,1)
    (timed/'processor_alu_proch_split.sv').write_text(split)
    cone=(out/'base/branch/cone.sv').read_text()
    sampled=[]
    for kind in ['F10145A','MC10173','MC10176']:
        module=re.findall(r'module branch_sampled_'+kind+r'\(.*?endmodule',cone,re.S)
        assert len(module)==1;sampled.append(module[0])
    (timed/'processor_alu_flag_views.sv').write_text('\n'.join(sampled)+'\n')
    partition=top.replace('module processor_alu_reference','module processor_alu_partition',1)
    marker='processor_alu_proch h(';assert partition.count(marker)==1
    partition=partition.replace(marker,'processor_alu_proch_split h(',1)
    marker='cpu_pins,bridge,next_raw';assert partition.count(marker)==2
    partition=partition.replace(marker,'cpu_pins,bridge,saved_q,next_raw',1)
    partition=partition.replace('cl[40:37],ch[18:15],','cl[40:37],saved_clocks,',1)
    declaration=' wire [79:0] saved_q;wire [3:0] saved_clocks;\n'
    partition=partition.replace(' wire [2:0] bases;',declaration+' wire [2:0] bases;',1)
    extra=''' branch_condition_reference saved(.fixture_load(fixture_load),.fixture_state(fixture_state[923:844]),
  .base(processor_roots[1]),.start_a(1'b0),.en(1'b0),.pins(branch_pins),
  .state_q(saved_q),.clocks(saved_clocks));
'''
    partition=partition.replace('endmodule',extra+'endmodule',1)
    (timed/'processor_alu_partition.sv').write_text(partition)
    comb_kinds={boards[n].packages[p]['type']for n,s in specs.items()for p in s['comb']}
    for kind in sorted(comb_kinds):
        path=timed/('cell_'+kind+'.v')
        if not path.exists():
            raw=(ROOT/'verilog/cells'/path.name).read_text()
            raw=re.sub(r'(?m)^(\s*assign\s+)',r'\g<1>#(1000) ',raw)
            path.write_text('`timescale 1ps/1ps\n'+raw)
    operand_layout={}
    for name,packages in support['closed_operand_packages'].items():
        for pkg,(offset,width)in packages.items():
            start = 1064 + offset - 360 if name == 'ProcH' else 1064 + high_bits - 360 + offset - 548
            operand_layout[name+'.'+pkg]=dict(board=name,package=pkg,kind=boards[name].packages[pkg]['type'],
                local_offset=offset,global_offset=start,bits=width)
    result=dict(scope=__doc__,physical_bits=physical_bits,clock_channels=clock_channels,bulk_SRAM_bits=9216,
        original_indices_retained='923:0',new_low_indices='1063:924',new_low_packages=NEW_LOW,
        boundary_pins=next_pin,extra_boundary_pins=extras,global_drivers=global_drivers,
        native_global_port_checks=port_checks,support=support,boards=records,
        actual_instruction_ALUF_pins=[326+37+i for i in range(4)],
        actual_instruction_ALUF_sources=selector_sources,
        old_state_source_overrides=legacy,source_native_backplane_sha256=sha(NATIVE/BP),
        original_ALU_function_memory_and_operand_muxes_integrated=True,
        remaining_operand_registers_and_shifter_ROMs_are_boundaries=True,
        supported_operand_registers_integrated=close_operands,
        operand_state_layout=operand_layout,
        optimized_capture_qualified=False,native_qualified=False,physical_timing_qualified=False)
    (out/'manifest.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--operands',action='store_true')
    a=p.parse_args();m=generate(a.output.resolve(),a.operands);print('ORIGINAL_ALU_REFERENCE',m['physical_bits'],m['boundary_pins'])
