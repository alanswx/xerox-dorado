#!/usr/bin/env python3
"""Exercise original-ROM CPU fixtures through the native host/advance boundary.

Keep every CPU/helper/ROM byte. Only replace the fixture's continuous clock
connection with the production host-clock plus advance wiring and a synthetic
127-host-tick profile (113 active,14 paused). This is not an SDRAM-controller,
native BaseBoard, original-firmware Cedar boot or speed qualification.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path
import re
import shutil


def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def once(text,old,new):
    assert text.count(old)==1,old
    return text.replace(old,new)


def prepare(parent,out):
    assert not out.exists()
    pp=parent/'provenance.json.gz';record=json.loads(gzip.decompress(pp.read_bytes()))
    state=json.loads((parent/'run-state.json').read_text())
    assert state['status']!='RUNNING'and all(a['returncode']==0 for a in state['actions'])
    assert record['probe_only']
    out.mkdir(parents=True);inputs=out/'executed-inputs'
    for n,h in record['inputs'].items():
        original=parent/'executed-inputs'/n;assert sha(original)==h,n
        p=inputs/n;p.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(original,p)
    name='verilog/generated/dorado_full.v';top=inputs/name;s=top.read_text()
    s=once(s,'    input  wire sys_clk,','    input wire storage_clk,contb_advance,\n    input  wire sys_clk,')
    alias="  wire storage_clk=sys_clk;wire contb_advance=1'b1;\n"
    assert s.count(alias)in [0,1]
    s=s.replace(alias,'')
    s,count=re.subn(r"\.state_host_clk\(sys_clk\),\s*\.state_guest_advance\(1'b1\)",
                    '.state_host_clk(storage_clk),.state_guest_advance(contb_advance)',s)
    assert count>=8,count
    top.write_text(s)
    name='verilog/verilator/tb_exec.sv';bench=inputs/name;s=bench.read_text()
    old="  reg sys_clk = 1'b0;\n  always #1 sys_clk = ~sys_clk;"
    clock='''  reg host_clk=0;
  always #1 host_clk=~host_clk;
  reg native_advance=1;
  longint native_host_ticks=0,native_paused_ticks=0;
  wire sys_clk=host_clk && native_advance;
  always @(negedge host_clk)begin
    native_host_ticks<=native_host_ticks+1;
    if(!native_advance)native_paused_ticks<=native_paused_ticks+1;
    native_advance<=(native_host_ticks%127)<113;
  end
  final $display("HOST_PAUSE_PROBE host_ticks=%0d paused_ticks=%0d",native_host_ticks,native_paused_ticks);'''
    s=once(s,old,clock)
    anchor='  dorado_full #(.SYSPER(SYSPER)) m ('
    s=once(s,anchor,anchor+'\n    .storage_clk(host_clk),.contb_advance(native_advance),')
    bench.write_text(s)
    shutil.copy2(parent/'cycle-limit.txt',out/'cycle-limit.txt')
    original_inputs=dict(record['inputs'])
    record['commands']=[[str(a).replace(str(parent),str(out))for a in c]for c in record['commands']]
    record['inputs']={n:sha(inputs/n)for n in original_inputs}
    changed={n for n,h in original_inputs.items()if record['inputs'][n]!=h}
    assert changed=={'verilog/generated/dorado_full.v','verilog/verilator/tb_exec.sv'}
    record.update(native_host_pause_probe=True,native_host_pause_parent=str(parent),
        native_host_pause_parent_provenance_sha256=sha(pp),native_host_pause_changed_inputs=sorted(changed),
        native_host_pause_snapshot_board_bindings=count,
        native_host_pause_profile=dict(period=127,active=113,paused=14),
        actual_SDRAM_controller_tested=False)
    (out/'provenance.json.gz').write_bytes(gzip.compress((json.dumps(record,indent=2)+'\n').encode(),mtime=0))
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    print('PREPARED_NATIVE_HOST_PAUSE_PROBE',out,count,'native board bindings; CPU sources unchanged')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--parent',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();prepare(a.parent.resolve(),a.output.resolve())
