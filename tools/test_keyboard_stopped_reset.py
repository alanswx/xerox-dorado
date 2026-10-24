#!/usr/bin/env python3
"""Check native input reset with no machine-clock edges in both simulators.

Retain negative controls that separately remove each asynchronous reset.
Also run the existing queue and keyboard/terminal serial tests unchanged.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import time

ROOT=Path(__file__).resolve().parents[1]


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def run(out):
    assert not out.exists()
    out.mkdir(parents=True)
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    sources={
        'dorado_keyboard.v':ROOT/'verilog/cells/dorado_keyboard.v',
        'dorado_terminal.v':ROOT/'verilog/cells/dorado_terminal.v',
        'dorado_key_queue.sv':ROOT/'fpga/mister/dorado_key_queue.sv',
        **{name+'.sv':ROOT/'verilog/verilator'/(name+'.sv') for name in
           ['tb_keyboard_stopped_reset','tb_key_queue','tb_keyboard_terminal']}}
    frozen=out/'inputs';frozen.mkdir()
    for name,source in sources.items():
        (frozen/name).write_bytes(source.read_bytes())
    state=dict(status='RUNNING',scope=__doc__,inputs={name:sha(frozen/name)for name in sources},
               started=time.time(),actions=[],cases=[])
    path=out/'run-state.json'
    def save():
        path.write_text(json.dumps(state,indent=2)+'\n')
    def execute(command,label):
        with (out/(label+'.log')).open('x')as log:
            process=subprocess.Popen(command,stdout=log,stderr=subprocess.STDOUT)
            action=dict(label=label,command=command,pid=process.pid,started=time.time())
            state['actions'].append(action);save()
            action.update(returncode=process.wait(),finished=time.time());save()
            return action['returncode']
    save()
    try:
        for engine in ['icarus','verilator']:
            for mode in ['corrected','corrected_reset_high','missing_powerup_init',
                         'keyboard_sync_reset','terminal_sync_reset','both_sync_reset',
                         'existing_queue','existing_terminal']:
                case=out/(engine+'-'+mode);case.mkdir()
                top={'existing_queue':'tb_key_queue','existing_terminal':'tb_keyboard_terminal'}.get(mode,'tb_keyboard_stopped_reset')
                hdl=[]
                for name in ['dorado_keyboard.v','dorado_terminal.v','dorado_key_queue.sv',top+'.sv']:
                    text=(frozen/name).read_text()
                    if mode=='missing_powerup_init' and name in ['dorado_keyboard.v','dorado_terminal.v']:
                        names=(r'key_word[0-3]|prev_strobe'if name=='dorado_keyboard.v'else
                               r'mouse_taken|mouse_sent_d[xy]|prev[0-4]|kbd_pending|msg_bit|msg_word|msg_type|msg_body|msg')
                        text,count=re.subn(r'\b('+names+r")\s*=\s*\d+'s?[hbd][0-9a-fA-F]+",r'\1',text)
                        assert count==(5 if name=='dorado_keyboard.v'else 14),(name,count)
                    if ((name=='dorado_keyboard.v' and mode in ['keyboard_sync_reset','both_sync_reset'])or
                        (name=='dorado_terminal.v' and mode in ['terminal_sync_reset','both_sync_reset'])):
                        old='always @(posedge sys_clk or posedge reset)'
                        assert text.count(old)==1
                        text=text.replace(old,'always @(posedge sys_clk)')
                    target=case/name;target.write_text(text);hdl.append(str(target))
                binary=case/'test'
                if engine=='icarus':
                    command=['iverilog','-g2012','-s',top,'-o',str(binary),*hdl]
                    test=['vvp',str(binary)]
                else:
                    command=['verilator','--binary','--timing','--top-module',top,'-Wno-fatal',
                             '--Mdir',str(case/'obj'),'-o',str(binary),'-j','2',*hdl]
                    test=[str(binary)]
                if mode=='corrected_reset_high':command.insert(1,'-DDORADO_RESET_START_HIGH')
                label=engine+'-'+mode
                assert execute(command,label+'-compile')==0,label
                code=execute(test,label+'-test')
                log=(out/(label+'-test.log')).read_text()
                expected_failure='sync_reset'in mode or mode=='missing_powerup_init'
                if expected_failure:
                    expected='stopped reset terminal'if mode=='terminal_sync_reset'else'stopped reset matrix'
                    assert code!=0 and expected in log,(label,code,log)
                else:
                    assert code==0 and 'PASS'in log,(label,code,log)
                state['cases'].append(dict(engine=engine,mode=mode,expected_rejection=expected_failure,
                                           test_returncode=code,log_sha256=sha(out/(label+'-test.log'))));save()
        state.update(status='PASS_STOPPED_INPUT_RESET_WITH_NEGATIVE_CONTROLS',finished=time.time(),
                     native_boot_qualified=False,physical_timing_qualified=False)
        save();print(state['status'],len(state['cases']),'cases',flush=True)
    except BaseException as error:
        state.update(status='TERMINAL_FAILED',error=repr(error),finished=time.time());save();raise


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args();run(args.output.resolve())
