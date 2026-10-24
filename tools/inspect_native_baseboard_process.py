#!/usr/bin/env python3
"""Read native simulation CPU nodes at an evaluation boundary using LLDB.

Only debugger breakpoints/reads are used; no guest values, files, bounds or
clocks are modified. Detaching resumes the same simulation. The generated
header supplies actual field offsets, avoiding guessed object layouts.
"""
import argparse
import json
from pathlib import Path
import subprocess
import time

from test_r_front_forward_view import sha


def run(source, out, pid):
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
    result.update(decoded_nodes=decoded,counters={k:int.from_bytes(bytes.fromhex(v),'little')
                  for k,v in result['fields'].items() if not k.startswith('nodes_')},
                  source=str(source),pid=pid,source_state_sha256=sha(source/'run-state.json'),
                  binary_sha256=sha(binary),header_sha256=sha(header),layout=offsets,
                  read_only_guest_inspection=True)
    (out/'result.json').write_text(json.dumps(result,indent=2)+'\n')
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    print(json.dumps(dict(counters=result['counters'],nodes=result['decoded_nodes'])),flush=True)


if __name__ == '__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True)
    p.add_argument('--pid',type=int,required=True)
    a=p.parse_args();run(a.source.resolve(),a.output.resolve(),a.pid)
