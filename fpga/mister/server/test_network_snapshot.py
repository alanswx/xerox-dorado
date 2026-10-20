#!/usr/bin/env python3
"""Actual RTL/C-peer snapshot pause, all original state and independent DDR oracle.

Host-side ring PAUSE is real. The wire-buffer peer and external DDR memory are
independent models. This is not physical DDR write visibility, whole-machine
service ordering, state import or a Cedar FPGA snapshot qualification.
"""
import argparse
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import subprocess
import sys
import xml.etree.ElementTree as ET

HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[2]
sys.path.insert(0,str(ROOT/'fpga/mister/snapshot'))
from generate_serial_boards import raw_receipt,once
spec=importlib.util.spec_from_file_location('state_inventory',ROOT/'tools/sil_state_inventory.py')
inventory=importlib.util.module_from_spec(spec);spec.loader.exec_module(inventory)

BOUNDARY='''    // Snapshot continuation retains the epoch, ring indices and timer phase.
    // Finish the current frame/accepted DDR operation before holding IDLE.
    // STOP/reset/changed grants still revoke ownership through the original
    // drained-stop path; a pause is never a substitute for lifecycle STOP.
    // The caller separately pauses the daemon and guest wire, and must still
    // establish physical DDR-fabric write visibility before memory capture.
    wire snapshot_boundary=(SNAPSHOT_PAUSE!=0) && snapshot_hold && !reset &&
        !stop_latched && fault==0 &&
        ((state==IDLE && enable && epoch==active_epoch) ||
         (state==STOPPED && !enable));
    assign snapshot_idle=snapshot_boundary && !tx_ack && !rx_wr && !rx_commit;
'''


def sha(raw):return hashlib.sha256(raw).hexdigest()


def inverse(text):
    for before in('    parameter integer SNAPSHOT_PAUSE=0,\n',
                  '    input wire snapshot_hold,\n    output wire snapshot_idle,\n',
                  '\n'+BOUNDARY,'        if(!snapshot_boundary)begin\n'):
        text=once(text,before,'')
    return once(text,'        endcase\n        end\n','        endcase\n')


def state(xml):
    data=inventory.inventory(xml);tree=ET.fromstring(xml)
    modules=tree.findall('.//module');assert len(modules)==1
    module=modules[0];rows={};tasks=[]
    for row in data['candidates']:
        name=row['name']
        if row['category']=='subroutine_review':
            assert name in('ddram_addr','ddram_be','ddram_wdata','return_state'),'unreviewed task state'
            writers=[t.get('name')for t in module.findall('./task')if any(name in inventory.destinations(a[1])for a in t.findall('.//assigndly'))]
            called={t.get('name')for p in module.findall('./always')if any(s.get('edgeType')=='POS'for s in p.findall('./sentree/senitem'))for t in p.findall('.//taskref')}
            assert writers and set(writers)<=called,'unreviewed task caller'
            tasks.append(dict(name=name,bits=row['bits'],tasks=writers))
        elif row['category']!='sequential':continue
        assert not row['unpacked_dimensions']
        if name.startswith('@'):
            # Retain the clocked packing temporary conservatively too. The
            # named static block makes its actual bits observable in the test.
            assert name.endswith('.updated')and row['bits']==64
            name='pack_word.updated'
        assert name not in rows;rows[name]=(row['width'],row['bits'])
    assert len(rows)==29 and sum(bits for _,bits in rows.values())==677
    assert {r['name']for r in tasks}=={'ddram_addr','ddram_be','ddram_wdata','return_state'}
    return rows,dict(required_bits=677,required_fields=29,task_written_bits=109,task_destinations=tasks,sequential_exclusions=[])


def run(out,mutations):
    out.mkdir(parents=True,exist_ok=False);inputs=out/'executed-inputs';inputs.mkdir()
    source=(ROOT/'fpga/mister/dorado_ethernet_ddr.sv').read_text()
    old=raw_receipt(ROOT/'fpga/mister/reports/2026-10-20-snapshot-native-memory-pause-rejected','terminal-fit/src/fpga/mister/dorado_ethernet_ddr.sv')
    assert inverse(source).encode()==old,'unapproved normal RTL edit'
    (inputs/'original.sv').write_bytes(old);(inputs/'candidate.sv').write_text(source)
    descriptions=[]
    for label,extra in(('original',[]),('candidate',['-GSNAPSHOT_PAUSE=1'])):
        cmd=['verilator','--xml-only','--top-module','dorado_ethernet_ddr','-Wno-fatal',*extra,
             '--xml-output',str(out/(label+'.xml')),str(inputs/(label+'.sv'))]
        (out/(label+'-xml-command.json')).write_text(json.dumps(cmd,indent=2)+'\n')
        with(out/(label+'-xml.log')).open('w')as log:subprocess.run(cmd,stdout=log,stderr=subprocess.STDOUT,check=True)
        descriptions.append(state((out/(label+'.xml')).read_bytes()))
    assert descriptions[0][0]==descriptions[1][0],'original state geometry changed'
    rows,description=descriptions[1];bits=description['required_bits'];words=(bits+31)//32
    for name in('dorado_ddr_ring.c','dorado_ddr_ring.h','test_network_snapshot.cpp'):
        (inputs/name).write_bytes((HERE/name).read_bytes())
    legacy=(HERE/'test_ddr_ring_rtl.cpp').read_text()
    legacy=once(legacy,'top.tx_valid=0;top.tx_words=0;','top.tx_valid=0;top.tx_words=0;top.snapshot_hold=0;')
    legacy=once(legacy,'round_trip();stop_reset();mid_frame_stop();malformed();','round_trip();stop_reset();mid_frame_stop();malformed();return 0;')
    (inputs/'legacy_test.cpp').write_text(legacy)
    (inputs/'original-legacy-test.cpp').write_bytes((HERE/'test_ddr_ring_rtl.cpp').read_bytes())
    for path in(Path(__file__),ROOT/'tools/sil_state_inventory.py'):(inputs/path.name).write_bytes(path.read_bytes())
    host=out/'host.o'
    cc=[os.environ.get('CC','cc'),'-std=c11','-O2','-Wall','-Wextra','-Werror','-I'+str(inputs),'-c',str(inputs/'dorado_ddr_ring.c'),'-o',str(host)]
    (out/'c-command.json').write_text(json.dumps(cc,indent=2)+'\n')
    subprocess.run(cc,check=True,capture_output=True)
    cases=[('candidate-wrap',source,0xfffffff0,None),('candidate-zero',source,0,None)]
    if mutations:
        for label,before,after,diagnostic in(
            ('early_idle','assign snapshot_idle=snapshot_boundary && !tx_ack && !rx_wr && !rx_commit;',
             'assign snapshot_idle=(SNAPSHOT_PAUSE!=0) && snapshot_hold && !reset;','pause acknowledged an accepted DDR read'),
            ('changing_timer','        if(!snapshot_boundary)begin\n',
             "        if(snapshot_boundary && poll_count!=0)poll_count<=poll_count-1'b1;\n        if(!snapshot_boundary)begin\n",'paused engine state changed'),
            ('stale_epoch','state==IDLE && enable && epoch==active_epoch','state==IDLE && enable','pause hid lifecycle change'),
            ('pulse_tail','assign snapshot_idle=snapshot_boundary && !tx_ack && !rx_wr && !rx_commit;',
             'assign snapshot_idle=snapshot_boundary;','paused engine state changed')):
            cases.append((label,once(source,before,after),0xfffffff0,diagnostic))
    results=[]
    for label,text,initial,diagnostic in cases:
        case=out/label;case.mkdir()
        instrumented=once(text,'    output wire stopped,',f'    output wire [{bits-1}:0] observed_state,\n    output wire stopped,')
        instrumented=once(instrumented,'endmodule','    assign observed_state={'+','.join(sorted(rows))+'};\nendmodule')
        (case/'candidate.sv').write_text(text);(case/'observed.sv').write_text(instrumented)
        cmd=['verilator','--cc','--exe','--build','-j','4','--top-module','dorado_ethernet_ddr','-Wno-fatal','-Wno-PROCASSINIT',
             '-GPOLL_CYCLES=4','-GSNAPSHOT_PAUSE=1',f'-GINITIAL_COUNT={initial}','--Mdir',str(case/'obj'),str(case/'observed.sv'),str(inputs/'test_network_snapshot.cpp'),
             '-CFLAGS',f'-std=c++17 -I{inputs} -DDDR_INITIAL_COUNT={initial}u -DDDR_PAUSE_STATE_WORDS={words} -DDDR_PAUSE_STATE_BITS={bits}',
             '-LDFLAGS',str(host)]
        (case/'command.json').write_text(json.dumps(cmd,indent=2)+'\n')
        with(case/'build.log').open('w')as log:subprocess.run(cmd,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run([str(case/'obj/Vdorado_ethernet_ddr')],capture_output=True,text=True,timeout=30)
        text=result.stdout+result.stderr;(case/'test.log').write_text(text)
        if diagnostic:assert result.returncode==23 and diagnostic in text,(label,result.returncode,text)
        else:assert result.returncode==0 and'NETWORK SNAPSHOT PASS'in text and'DDR RTL FAULT PASS'in text,text
        print(label+': '+(diagnostic if diagnostic else text.splitlines()[-1]),flush=True)
        results.append(dict(case=label,returncode=result.returncode,expected_failure=diagnostic,source_sha256=sha((case/'candidate.sv').read_bytes()),log_sha256=sha((case/'test.log').read_bytes())))
    (out/'qualification.json').write_text(json.dumps(dict(status='PASS',scope=__doc__,state_review=description,cases=results,
        original_sha256=sha(old),candidate_sha256=sha(source.encode()),new_state_bits=0,
        inputs={p.name:sha(p.read_bytes())for p in inputs.iterdir()},
        limits='Isolated engine/C endpoint. Production enables no snapshot pause yet. Physical DDR visibility, guest wire pause, Main coordination, full export/import and FPGA restore remain open.'),indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--mutations',action='store_true');a=p.parse_args();run(a.output.resolve(),a.mutations)
