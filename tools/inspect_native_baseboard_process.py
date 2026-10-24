#!/usr/bin/env python3
"""Read native simulation CPU nodes at an evaluation boundary using LLDB.

Only debugger breakpoints/reads are used; no guest values, files, bounds or
clocks are modified. Detaching resumes the same simulation. The generated
header supplies actual field offsets, avoiding guessed object layouts.
"""
import argparse
import json
from pathlib import Path
import re
import subprocess
import time

from test_r_front_forward_view import sha


def run(source, out, pid, low_ram=False, riot_ram=False, keyboard=False):
    assert not out.exists()
    state = json.loads((source/'run-state.json').read_text())
    test = state['actions'][-1]
    assert test['stage'] == 'test' and test['pid'] == pid and 'returncode' not in test
    obj = source/'obj'
    binary = obj/'native-cedar'
    actual = subprocess.check_output(['ps','-p',str(pid),'-o','command='],text=True).strip()
    assert actual.startswith(str(binary)+' '), actual
    out.mkdir(parents=True)
    header = obj/'Vtb_native_cedar___024root.h'
    prefix = 'tb_native_cedar__DOT__'
    bb = prefix+'dut__DOT__m__DOT__b_BaseBd__DOT__'
    fields = {'host_ticks':(prefix+'host_ticks',8),'guest_ticks':(prefix+'guest_ticks',8),
              'boot_samples':(prefix+'boot_samples',4),'step':(prefix+'step',1),
              'nodes_q':(bb+'u_f61__DOT__u_cpu__DOT__q',216),
              'nodes_no':(bb+'u_f61__DOT__u_cpu__DOT__no',216),
              'phi_inverted':(bb+'u_g08__DOT__qa',1),
              'phase':(bb+'MCClk',1),'reset_n':(bb+'u_j08__DOT__qa',1),
              'watchdog_counter':(bb+'u_g21__DOT__cnt',4),
              'watchdog_phase_a':(bb+'u_g22__DOT__qa',1),
              'watchdog_phase_b':(bb+'u_g22__DOT__qb',1)}
    for bit in range(8): fields[f'MCD{bit}']=(bb+f'MCD_{bit}',1)
    if keyboard:
        for name,size in [('ps2_key',2),('machine_key',2),
                          ('keys__DOT__count',2),('keys__DOT__head',1),
                          ('keys__DOT__tail',1),('keys__DOT__last_event',2)]:
            fields['keyboard_'+name]=(prefix+name,size)
        for index in range(4):
            fields[f'keyboard_word{index}']=(prefix+f'dut__DOT__kw{index}',2)
            fields[f'terminal_prev{index}']=(prefix+f'dut__DOT__u_terminal__DOT__prev{index}',2)
        for name,size in [('prev_strobe',1)]:
            fields['keyboard_'+name]=(prefix+'dut__DOT__u_keyboard__DOT__'+name,size)
        for name,size in [('kbd_pending',1),('msg_bit',1),('msg_word',1),('msg',4)]:
            fields['terminal_'+name]=(prefix+'dut__DOT__u_terminal__DOT__'+name,size)
    ram_pins={}
    board=source/'inputs/src/build/rtl-executive/native16-tx-cable/BaseBd_nominal.v'
    riot_banks={}
    if riot_ram:
        text=board.read_text()
        for pkg,body in re.findall(r'^  cell_MCS6532 u_(\w+) \(\n(.*?)^  \);',text,re.M|re.S):
            pins=dict(re.findall(r'\.p(\d+)\(([^()]*)\)',body))
            match=re.fullmatch(r'Ram([0-4])_p_',pins['37']);assert match
            assert pins['36']=='MCA_10'
            index=int(match[1]);riot_banks[pkg]=index
            fields['riot_RAM_'+str(index)]=(bb+f'u_{pkg}__DOT__u_riot__DOT__riot_ram',128)
            for name,size in [('timer',1),('irq_en',1)]:
                fields['riot_'+str(index)+'_'+name]=(bb+f'u_{pkg}__DOT__u_riot__DOT__'+name,size)
        assert set(riot_banks.values())==set(range(5))
        m=re.search(r'^  cell_SN74LS138 u_g14 \(\n(.*?)^  \);',text,re.M|re.S);assert m
        pins=dict(re.findall(r'\.p(\d+)\(([^()]*)\)',m[1]))
        assert [pins[str(n)]for n in [1,2,3]]==['MCA_07','MCA_08','MCA_09']
        assert [pins[str(n)]for n in [15,14,13,12,11]]==['Ram'+str(i)+'_p_'for i in range(5)]
        # The generated EPROM files store each byte bit-reversed because
        # the board crosses O0..O7 onto MCD7..MCD0. Bind the actual operands
        # before interpreting either RAM word as a software timer.
        eprom=source/'inputs/src/verilog/proms/eprom/BaseBd-c61.mem'
        encoded=bytes(int(line,16)for line in eprom.read_text().splitlines()if line and not line.startswith('//'))
        program=bytes(int(f'{v:08b}'[::-1],2)for v in encoded)
        expected=bytes.fromhex('85 9f 84 a0 a0 09 8a 18 65 9f 85 9f a9 00 65 a0 85 a0 88 10 f1 20 92 f6 a5 53 c5 a0 30 f7 d0 06 a5 52 c5 9f 90 ef 60')
        assert program[0x4c0:0x4c0+len(expected)]==expected,'Delay operands differ from the compiled ROM'
        delay_rom=dict(eprom=str(eprom.relative_to(source)),sha256=sha(eprom),
                       byte_order='Each stored byte bit-reversed by original board data wiring',
                       logical_address=0xf4c0,decoded_bytes=expected.hex())
    if low_ram:
        text=board.read_text()
        instances={pkg:dict(re.findall(r'\.p(\d+)\(([^()]*)\)',body))
                   for pkg,body in re.findall(r'^  cell_i2125 u_(\w+) \(\n(.*?)^  \);',text,re.M|re.S)}
        ram_pins={pkg:p for pkg,p in instances.items()if p['1']=='RS0_p_'}
        assert len(ram_pins)==8
        assert {p['15']for p in ram_pins.values()}=={'MCD_'+str(i)for i in range(8)}
        for pkg,pins in ram_pins.items():
            assert [pins[str(p)]for p in [2,3,4,5,6,9,10,11,12,13]]==['RA'+str(i)for i in range(10)]
            fields['ram_'+pins['15']]=(bb+f'u_{pkg}__DOT__mem',1024)
        # Original RAM addresses are complemented by f13/g13; bind every
        # inverter connection rather than guessing the physical array index.
        for pkg,pairs in [('f13',[(9,1,2),(8,3,4),(7,5,6),(6,9,8),(5,11,10),(4,13,12)]),
                          ('g13',[(3,1,2),(2,3,4),(1,5,6),(0,9,8)])]:
            m=re.search(r'^  cell_SN74LS04 u_'+pkg+r' \(\n(.*?)^  \);',text,re.M|re.S);assert m
            pins=dict(re.findall(r'\.p(\d+)\(([^()]*)\)',m[1]))
            for bit,ip,op in pairs:assert pins[str(ip)]==f'MCA_{bit:02d}' and pins[str(op)]=='RA'+str(bit)
    assert all(name in header.read_text() for name, size in fields.values())
    layout = out/'layout.cpp'
    layout.write_text('#include <cstdio>\n#include <cstddef>\n#include "Vtb_native_cedar___024root.h"\n'
        'int main(){\n'+''.join(f'printf("{key} %zu {size}\\n",offsetof(Vtb_native_cedar___024root,{name}));\n'
                              for key,(name,size) in fields.items())+'}\n')
    include = subprocess.check_output(['verilator','--getenv','VERILATOR_ROOT'],text=True).strip()+'/include'
    # The model header is compiled as data-layout information only.
    cc = ['c++','-std=c++20','-I'+str(obj),'-I'+include,'-I'+include+'/vltstd',str(layout),'-o',str(out/'layout')]
    with (out/'layout-compile.log').open('w') as log:
        subprocess.run(cc,stdout=log,stderr=subprocess.STDOUT,check=True)
    offsets = {}
    for line in subprocess.check_output([str(out/'layout')],text=True).splitlines():
        key, offset, size = line.split();offsets[key]=dict(offset=int(offset),size=int(size))
    (out/'layout.json').write_text(json.dumps(offsets,indent=2)+'\n')
    script = out/'read.py'
    script.write_text('import json,lldb,time\n'
        f'layout={offsets!r}\n'
        'p=lldb.debugger.GetSelectedTarget().GetProcess()\n'
        'f=p.GetSelectedThread().GetFrameAtIndex(0)\n'
        'base=f.FindRegister("x0").GetValueAsUnsigned()\n'
        'result={"time":time.time(),"base":base,"stop_frame":str(f),"fields":{}}\n'
        'for k,v in layout.items():\n'
        ' e=lldb.SBError();raw=p.ReadMemory(base+v["offset"],v["size"],e)\n'
        ' if not e.Success(): raise RuntimeError(str(e))\n'
        ' result["fields"][k]=raw.hex()\n'
        f'open({str(out/"memory.json")!r},"w").write(json.dumps(result,indent=2)+"\\n")\n')
    commands = out/'commands.lldb'
    commands.write_text('settings set target.skip-prologue false\n'
        'breakpoint set --name Vtb_native_cedar___024root___eval_act --one-shot true\n'
        'continue\n'+f'command script import {script}\n'+'process detach\nquit\n')
    with (out/'lldb.log').open('w') as log:
        rc = subprocess.run(['lldb','-b','-p',str(pid),'-s',str(commands)],stdout=log,stderr=subprocess.STDOUT).returncode
    assert rc == 0 and (out/'memory.json').exists(), (rc,'Inspect lldb.log; ensure process detached')
    result = json.loads((out/'memory.json').read_text())
    decoded = {}
    node_lists = {'x':[1216,98,1,1648,85,589,448,777],
                  'a':[737,1234,978,162,727,858,1136,1653],
                  'pc':[1139,1022,655,1359,900,622,377,1611,1670,292,502,584,948,49,1551,205],
                  'ab':[268,451,1340,211,435,736,887,1493,230,148,1443,399,1237,349,672,195],
                  'db':[1005,82,945,650,1393,175,1591,1349],
                  'sync':[539],'rw':[1156]}
    for view in ['nodes_q','nodes_no']:
        bits = int.from_bytes(bytes.fromhex(result['fields'][view]),'little')
        decoded[view] = {k:sum(((bits>>node)&1)<<i for i,node in enumerate(nodes)) for k,nodes in node_lists.items()}
    if low_ram:
        image=bytes(sum((bytes.fromhex(result['fields']['ram_MCD_'+str(bit)])[address^1023]&1)<<bit
                        for bit in range(8))for address in range(1024))
        (out/'baseboard-extra-ram-rs0.bin').write_bytes(image)
        result['extra_ram']=dict(byte_count=1024,physical_index='address_low10 XOR 1023',
                                bank='RS0',pins=ram_pins,board_sha256=sha(board),
                                image_sha256=sha(out/'baseboard-extra-ram-rs0.bin'),
                                address_interpretation='Extra RAM bank RS0; not the RIOT RAM containing zero page, stack or ROM timer variables.')
    if riot_ram:
        image=b''.join(bytes.fromhex(result['fields']['riot_RAM_'+str(i)])for i in range(5))
        assert len(image)==640
        (out/'baseboard-riot-ram.bin').write_bytes(image)
        result['riot_ram']=dict(byte_count=640,bank_bytes=128,package_banks=riot_banks,
                               physical_address='g14 decodes MCA07..09; each RIOT indexes MCA00..06',
                               board_sha256=sha(board),image_sha256=sha(out/'baseboard-riot-ram.bin'),
                               timer100ms_word=image[0x52]|image[0x53]<<8,
                               EndOfDelay_word=image[0x9f]|image[0xa0]<<8,
                               compiled_delay_operands=delay_rom,
                               address_interpretation='Timer/EndOfDelay addresses match exact compiled ROM Delay operands; no guest writes.')
    result.update(decoded_nodes=decoded,counters={k:int.from_bytes(bytes.fromhex(v),'little')
                  for k,v in result['fields'].items() if not k.startswith(('nodes_','ram_','riot_RAM_'))},
                  source=str(source),pid=pid,source_state_sha256=sha(source/'run-state.json'),
                  binary_sha256=sha(binary),header_sha256=sha(header),layout=offsets,
                  read_only_guest_inspection=True)
    (out/'result.json').write_text(json.dumps(result,indent=2)+'\n')
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    print(json.dumps(dict(counters=result['counters'],nodes=result['decoded_nodes'],
                         extra_ram=result.get('extra_ram'),riot_ram=result.get('riot_ram'))),flush=True)


if __name__ == '__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True)
    p.add_argument('--pid',type=int,required=True)
    p.add_argument('--low-ram',action='store_true',help='Legacy name: read extra SRAM bank RS0, not the RIOT zero page')
    p.add_argument('--riot-ram',action='store_true',help='Read the five original 128-byte RIOT RAM banks containing zero page and stack')
    p.add_argument('--keyboard',action='store_true',help='Read actual queued PS/2 event, keyboard matrix and terminal serialization state')
    a=p.parse_args();run(a.source.resolve(),a.output.resolve(),a.pid,a.low_ram,a.riot_ram,a.keyboard)
