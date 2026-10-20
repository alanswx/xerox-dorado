#!/usr/bin/env python3
"""Extract the original coupled T-data latches, HoldDly and four SRAM slices.

The Pdata/dMD buses and unused g18 D pins are explicit boundaries. No candidate
clock, mux, address or write equations are used to construct this reference.
"""
import argparse
import json
from pathlib import Path

from generate_processor_capture_reference import graph
from generate_task_capture_reference import specs as task_specs, generate as generate_tasks
from sil_netlist import load_board

ROOT = Path(__file__).resolve().parents[1]


def specs():
    result = task_specs()
    result[0]['module'] = 't_file_conta'
    result[0]['flag_taps'] += ['Freeze']
    for low, spec in zip([False, True], result[1:]):
        spec['module'] = 't_file_procl' if low else 't_file_proch'
        spec['seq'] = {**spec['seq'], 'g18': (20, 6), 'k01': (26, 4),
                       'k02': (30, 4), 'l03': (34, 64), 'l04': (98, 64),
                       'j03':(162,4),'j04':(166,4),'i03':(170,4),'i04':(174,4)}
        spec['comb'] = sorted(set(spec['comb'] + ['i05', 'j05'] + ([] if low else ['h17','g21'])))
        bindings = spec['bindings']
        bindings.pop('HoldDly', None)
        bindings['Holda'] = 'pins[68]'
        bindings['TisIFdata'] = 'pins[117]'
        # High p5 is the OR of the original FF076 decode and Freeze through
        # h17. High p7 is unconnected. Preserve both real wired-OR drivers.
        extra = ['ProcL27.sil+10', 'ProcL27.sil+8', 'ProcL27.sil+9',
                 'ProcL24.sil+14', 'ProcL20.sil+7'] if low else [
                 None, 'ProcH24.sil+4', 'ALUCarry', "Overflow'"]
        for index, name in enumerate(extra):
            if name is not None:
                bindings[name] = f'pins[{112+index if low else 108+index}]'
        if not low:
            bindings.update({'Freeze': 'freeze', 'MBBypass': "1'b0", 'DMuxData': "1'b0",
                             'ProcH25.sil+18': "1'b0", 'ProcH25.sil+1': "1'b0",
                             'SignIfuData':'pins[118]','RbBypassDly':"1'b0",'RisIFdata':"1'b0","R.Perr'":"1'b0","T.Perr'":"1'b0"})
        else:
            for bit in range(8):
                bindings[f'IfuData.{bit}']=f'pins[{119+bit}]'
        for bit in range(8):
            number = bit + (8 if low else 0)
            bindings[f'Pdata.{number:02}'] = f'pins[{76+number}]'
            bindings[f'dMD.{number:02}'] = f'pins[{92+number}]'
        spec['clock_taps'] += ["Clock1'Ba", "TbWrite'a", "Clock0'Db" if low else "Clock0'Da"]
        spec['flag_taps'] += ["PreSHCP'B",'TbBypass']
    return result


def generate(root, out, inspect=False):
    if inspect:
        for spec in specs():
            external = graph(root, out=None, inspect=True, **spec)
            print(spec['bn'], 'unbound', sorted(set(external) - set(spec['bindings'])),
                  'extra', sorted(set(spec['bindings']) - set(external)))
        return
    out.mkdir(parents=True, exist_ok=False)
    # A previously unused output may become a consumed wired-OR net as the
    # slice grows. Never silently drop an original driver's contribution.
    for spec in specs():
        board = load_board(str(root / 'chm/sil' / (spec['bn'] + '.dm!1_') / (spec['bn'] + '.wl')))
        chosen = set(spec['comb']) | set(spec['seq'])
        for name, net in board.nets.items():
            drivers = [p['pkg'] for p in net['pins'] if p['dir'] == 'out']
            consumed = any(p['pkg'] in chosen and p['dir'] == 'in' for p in net['pins'])
            if consumed and any(p in chosen for p in drivers):
                assert all(p in chosen for p in drivers), (spec['bn'], name, drivers)
    records = [graph(root, out=out, **s) for s in specs()]
    for spec in specs():
        path = out / (spec['module'] + '.sv')
        text = path.read_text()
        assert text.count('input wire [27:0] pins') == 1
        text = text.replace('input wire [27:0] pins',
                            'input wire [126:0] pins,input wire [3:0] next_raw,input wire priority_n,freeze')
        path.write_text(text)
    audit = generate_tasks(root, out / 'task-source-audit')
    body = '''`timescale 1ns/1ps
module t_file_reference(input wire fixture_load,input wire [403:0] fixture_state,
 input wire base,input wire [126:0] pins,output wire [403:0] state_q,
 output wire [24:0] clocks,output wire [8:0] flags,output wire [3:0] next_raw,
 output wire [1:0] write_n,latch_n,output wire [1:0] hold_clocks,t_latch_n,bypass);
 wire [2:0] bases;
 cell_MC10210 base_fanout(.p7(base),.p11(base),.p5(1'b0),.p6(1'b0),.p9(1'b0),.p10(1'b0),
   .p2(bases[2]),.p12(bases[1]),.p13(bases[0]));
 wire [12:0] cc;wire [8:0] ch,cl;wire [13:0] fc;wire [4:0] fh,fl;
 wire en_h=fc[3],en_l=fc[4],start_a=fc[5],priority_n=fc[10];
 wire freeze=fc[13];
 wire [177:0] sh,sl,ih,il;
 assign ih[19:0]={fixture_state[83:80],fixture_state[47:32]};
 assign il[19:0]={fixture_state[87:84],fixture_state[63:48]};
 assign ih[33:20]={fixture_state[107:100],fixture_state[93:88]};
 assign il[33:20]={fixture_state[115:108],fixture_state[99:94]};
 assign {state_q[83:80],state_q[47:32]}=sh[19:0];
 assign {state_q[87:84],state_q[63:48]}=sl[19:0];
 assign {state_q[107:100],state_q[93:88]}=sh[33:20];
 assign {state_q[115:108],state_q[99:94]}=sl[33:20];
 assign ih[177:162]={fixture_state[395:388],fixture_state[379:372]};
 assign il[177:162]={fixture_state[403:396],fixture_state[387:380]};
 assign {state_q[395:388],state_q[379:372]}=sh[177:162];
 assign {state_q[403:396],state_q[387:380]}=sl[177:162];
 for(genvar i=0;i<16;i=i+1) begin: map_t_file
   assign ih[34+4*i+:4]=fixture_state[116+16*i+:4];
   assign ih[98+4*i+:4]=fixture_state[120+16*i+:4];
   assign il[34+4*i+:4]=fixture_state[124+16*i+:4];
   assign il[98+4*i+:4]=fixture_state[128+16*i+:4];
   assign state_q[116+16*i+:4]=sh[34+4*i+:4];
   assign state_q[120+16*i+:4]=sh[98+4*i+:4];
   assign state_q[124+16*i+:4]=sl[34+4*i+:4];
   assign state_q[128+16*i+:4]=sl[98+4*i+:4];
 end
 t_file_conta c(fixture_load,{fixture_state[79:64],fixture_state[31:0]},bases[2],1'b0,1'b0,
   pins,next_raw,priority_n,freeze,{state_q[79:64],state_q[31:0]},cc,fc);
 t_file_proch h(fixture_load,ih,bases[1],start_a,en_h,pins,next_raw,priority_n,freeze,sh,ch,fh);
 t_file_procl l(fixture_load,il,bases[0],start_a,en_l,pins,next_raw,priority_n,freeze,sl,cl,fl);
 assign next_raw=fc[9:6];
 assign clocks={cl[5],ch[5],cc[12],cc[11:9],cl[4:0],ch[4:0],cc[8:0]};
 assign flags={fl[2:0],fh[2:0],fc[2:0]};
 assign t_latch_n={fl[3],fh[3]};assign bypass={fl[4],fh[4]};
 assign latch_n={cl[6],ch[6]};assign write_n={cl[7],ch[7]};assign hold_clocks={cl[8],ch[8]};
endmodule
'''
    (out / 't_file_reference.sv').write_text(body)
    manifest = dict(boards=records, physical_bits=sum(r['physical_bits'] for r in records),
                    packages=sum(len(r['packages']) for r in records),
                    original_pins=sum(r['original_pins'] for r in records), source_audit=audit,
                    boundary='Pdata/dMD, unused g18 D, and inherited task/producer inputs')
    (out / 'manifest.json').write_text(json.dumps(manifest, indent=2) + '\n')
    print('T_FILE_REFERENCE', manifest['physical_bits'], 'bits', manifest['packages'],
          'packages', manifest['original_pins'], 'pins')
    return manifest


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, default=ROOT)
    parser.add_argument('--output', type=Path)
    parser.add_argument('--inspect', action='store_true')
    args = parser.parse_args()
    if not args.inspect and not args.output:
        parser.error('Specify --output or --inspect')
    generate(args.root.resolve(), args.output.resolve() if args.output else None, args.inspect)
