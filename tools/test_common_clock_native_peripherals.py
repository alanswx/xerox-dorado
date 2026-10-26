#!/usr/bin/env python3
"""Replay real input and host-disk timing fixtures on common-clock variants.

Keep every fixture assertion. Keyboard queue and reset tests keep a virtual
source-edge counter, but drive the decoder/serializer from the host clock
with the same enable. HPS host-service tests run with guest advance zero.
Also check exact key/chord and mouse serial messages with advance enabled.
"""
import argparse
import json
from pathlib import Path
import re
import shutil
import subprocess
import time

from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1]


def run(stage,out):
    assert not out.exists();out.mkdir(parents=True)
    m=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,m['source_hashes'])
    assert m['clock_enable_conversion_staged']
    files={n:stage/n for n in ['src/verilog/cells/dorado_keyboard.v','src/verilog/cells/dorado_terminal.v',
       'src/fpga/mister/dorado_key_queue.sv','src/fpga/mister/dorado_mouse.sv','src/fpga/mister/dorado_pack_hps.sv']}
    assert all(p.exists()for p in files.values())
    # Icarus binds function/continuous-assignment references in declaration
    # order. Keep the frozen RTL intact and hoist only these constant-initialized
    # declarations in a simulation copy; both engines run the same copy.
    disk=files['src/fpga/mister/dorado_pack_hps.sv'];disk_raw=disk.read_text();hoisted=[]
    def declarations(match):
        text=match[0];end=text.index(');')+2;header=text[:end];body=text[end:];rows=[]
        for name in ['cyl','head','host_state','wr_state','wr_bits']:
            pattern=r'^  reg \[[^\n;]+\]\s+[^\n;]*\b'+name+r'\s*=[^\n;]*;\n'
            hits=list(re.finditer(pattern,body,re.M));assert len(hits)==1,(name,hits)
            hit=hits[0];rows.append(hit[0]);body=body[:hit.start()]+body[hit.end():]
        hoisted.append(dict(module=re.search(r'module\s+(\w+)',text)[1],declarations=rows))
        return header+'\n'+''.join(rows)+body
    disk_after,count=re.subn(r'^module dorado_pack_hps(?:_ce)?\b.*?^endmodule',declarations,disk_raw,flags=re.M|re.S)
    assert count==2
    adapter=out/'dorado_pack_hps-declaration-order.sv';adapter.write_text(disk_after)
    files['src/fpga/mister/dorado_pack_hps.sv']=adapter
    original_names=['tb_keyboard_stopped_reset','tb_keyboard_terminal','tb_mouse_terminal','tb_pack_wallclock']
    fixtures={}
    for name in original_names:
        source=ROOT/'verilog/verilator'/(name+'.sv');raw=source.read_text();old=raw;edits=[]
        def change(a,b,count=1):
            nonlocal raw
            assert raw.count(a)==count,(name,a,raw.count(a))
            raw=raw.replace(a,b);edits.append((a,b))
        if name=='tb_keyboard_stopped_reset':
            change('dorado_keyboard k(.sys_clk(machine)','dorado_keyboard_ce k(.guest_advance(enabled),.sys_clk(host)')
            change('dorado_terminal t(.sys_clk(machine)','dorado_terminal_ce t(.guest_advance(enabled),.sys_clk(host)')
        elif name=='tb_keyboard_terminal':
            change('dorado_keyboard k(.sys_clk(sys_clk)',"dorado_keyboard_ce k(.guest_advance(1'b1),.sys_clk(sys_clk)")
            change('dorado_terminal t(.sys_clk(sys_clk)',"dorado_terminal_ce t(.guest_advance(1'b1),.sys_clk(sys_clk)")
        elif name=='tb_mouse_terminal':
            change('dorado_terminal t(.sys_clk(clk)',"dorado_terminal_ce t(.guest_advance(1'b1),.sys_clk(clk)")
        else:
            change('dorado_pack_hps #','dorado_pack_hps_ce #')
            change(".clk_sys(clk),.machine_clk(1'b0)",".guest_advance(1'b0),.clk_sys(clk),.machine_clk(clk)")
        reverse=raw
        for a,b in reversed(edits):reverse=reverse.replace(b,a)
        assert reverse==old and raw.count('$fatal(')==old.count('$fatal(')
        (out/(name+'-original.sv')).write_text(old);p=out/(name+'.sv');p.write_text(raw)
        fixtures[name]=dict(path=p,original_sha256=sha(source),retiming_edits=edits,original_assertions=old.count('$fatal('))
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    state=dict(status='RUNNING',scope=__doc__,stage=str(stage),stage_manifest_sha256=sha(stage/'r-front-control-native.json'),
        staged_source_hashes={n:sha(stage/n)for n in files},simulation_source_hashes={str(p):sha(p)for p in files.values()},
        Icarus_declaration_order_adapter=dict(original_sha256=sha(disk),adapted_sha256=sha(adapter),hoisted=hoisted),
        fixtures={n:{k:(str(v)if k=='path'else v)for k,v in f.items()}for n,f in fixtures.items()},
        started=time.time(),actions=[],cases=[],hardware_changed=False,firmware_or_full_machine_qualified=False)
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    def execute(label,cmd):
        with(out/(label+'.log')).open('x')as log:
            p=subprocess.Popen(cmd,cwd=stage/'src',stdout=log,stderr=log)
            action=dict(label=label,pid=p.pid,command=cmd,started=time.time());state['actions'].append(action);save();code=p.wait()
        action.update(returncode=code,finished=time.time());save();return code
    good=True
    for engine in ['icarus','verilator']:
        for case,top in [('reset-low','tb_keyboard_stopped_reset'),('reset-high','tb_keyboard_stopped_reset'),
                         ('keyboard','tb_keyboard_terminal'),('mouse','tb_mouse_terminal'),('host-disk','tb_pack_wallclock')]:
            label=engine+'-'+case;sources=[str(p)for p in files.values()]+[str(fixtures[top]['path'])]
            defines=['-DDORADO_RESET_START_HIGH']if case=='reset-high'else[]
            if engine=='icarus':
                binary=out/(label+'.vvp');cmd=['iverilog','-g2012','-s',top,*defines,'-o',str(binary),*sources];test=['vvp',str(binary)]
            else:
                obj=out/('obj-'+label);cmd=['verilator','--binary','--timing','--top-module',top,*defines,'-j','2','-Wno-fatal','--Mdir',str(obj),'-o','test',*sources];test=[str(obj/'test')]
            code=execute(label+'-compile',cmd)
            if code==0:code=execute(label+'-test',test)
            log=(out/(label+'-test.log')).read_text()if(out/(label+'-test.log')).exists()else''
            expected=3 if case=='host-disk'else 1
            passed=code==0 and log.count('PASS')==expected
            state['cases'].append(dict(label=label,returncode=code,passed=passed,expected_PASS_markers=expected))
            good=good and passed;save()
    state.update(status='PASS_ACTUAL_COMMON_CLOCK_NATIVE_INPUT_AND_HOST_DISK'if good else'TERMINAL_COMMON_CLOCK_PERIPHERALS_NOT_ESTABLISHED',
        finished=time.time(),files={p.name:sha(p)for p in out.iterdir()if p.is_file()and p.name!='run-state.json'and p.suffix!='.vvp'})
    save();print(state['status'],flush=True);return 0 if good else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['stage','output']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.stage.resolve(),a.output.resolve()))
