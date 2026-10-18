"""Compile deliberately invalid monitors; real board tests must reject them."""
from pathlib import Path
import json
import subprocess

root = Path.cwd()
out = root/'build/rtl-executive/instruction-progress/negative-controls'
out.mkdir(parents=True, exist_ok=True)
source = (root/'fpga/mister/dorado_instruction_monitor.sv').read_text()
results = []
for name, old, new, case, args in [
    ('count-held', 'if(repeat_current)', "if(1'b0)", 'held',
     ['+arm_pc=258', '+expect_holds=1']),
    ('half-board-clocks', 'if(start_cycle && !start_d)',
     'if(clock2 && !clock_d && !clocks[0])', 'read-tpc', ['+extended=1']),
]:
    assert source.count(old) == 1
    rtl = out/(name+'.sv')
    rtl.write_text(source.replace(old, new))
    obj = out/('obj_'+name)
    log_path = out/(name+'.txt')
    with log_path.open('w') as log:
        subprocess.run(['make', '-C', 'verilog/verilator',
                        'instruction-progress-build', 'SYSPER=4',
                        'PROGRESS_MONITOR_RTL='+str(rtl),
                        'PROGRESS_TEST_OBJ='+str(obj)],
                       cwd=root, stdout=log, stderr=subprocess.STDOUT, check=True)
        result = subprocess.run([str(obj/'progress-test'), '+start=256',
                                 '+preload=verilog/verilator/fixtures/'
                                 f'instruction-progress/{case}.txt',
                                 '+cycles=100000', '+nostuck', *args],
                                cwd=root, stdout=log, stderr=subprocess.STDOUT)
    log = log_path.read_text()
    assert result.returncode != 0 and 'Verilog $stop' in log and 'architectural increments100' in log
    results.append(dict(control=name, compiled=True, rejected_at_runtime=True,
                        diagnostic=next(line for line in log.splitlines()
                                        if 'architectural increments100' in line)))
    print(name, 'runtime rejected', flush=True)
(out/'results.json').write_text(json.dumps(results, indent=2)+'\n')
