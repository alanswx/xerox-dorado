#!/usr/bin/env python3
"""Compare functional R ports with original 12 SRAMs and two parity generators.

The RAM arrays remain behind physical ports. This gate checks bank/address/data,
byte parity, chip selects/wired OR, half writes and the FPGA advance contract.
It does not qualify address producers, control coupling, SRAM setup/hold or 60ns.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import sys
from generate_r_file_reference import specs

ROOT=Path(__file__).resolve().parents[1]
CANDIDATE='verilog/rtl/dorado_r_file_ports.sv'
TEST='verilog/verilator/tb_r_file_ports.sv'
PROFILES=[('balanced',1000,1000),('late_ram',1000,9900),('late_parity',6600,2000)]
MUTATIONS={
    'lose_advance':[('advance && !write_n[0]','!write_n[0]')],
    'wrong_low_gate':[('advance && !write_n[1]','advance && !write_n[0]')],
    'wrong_high_parity':[('~^write_data[7:0]','^write_data[7:0]')],
    'wrong_high_address':[('rm_h[address[7:0]]','rm_h[address[15:8]]')],
    'lost_stack_write':[('if(!select_n[1]) stk_h','if(1\'b0) stk_h')],
    'wired_and':[('rm_h[address[7:0]]) |','rm_h[address[7:0]]) &')],
    'deselected_output':[('select_n[0] ? 9\'b0 :','select_n[0] ? 9\'b1 :')],
    'wrong_low_byte':[('~^write_data[15:8],write_data[15:8]','~^write_data[15:8],write_data[7:0]')],
}


def run(out,mutations):
    out.mkdir(parents=True,exist_ok=False);inputs=out/'executed-inputs'
    files=[CANDIDATE,TEST,'verilog/verilator/r_file_original_storage.sv',
        'tools/generate_r_file_reference.py','tools/generate_processor_capture_reference.py',
        'tools/generate_control_clock_reference.py','tools/generate_run_control_reference.py',
        'tools/sil_netlist.py','tools/test_r_file_ports.py',
        'verilog/cells/cell_MC10170.v','verilog/cells/cell_MB7071H.v','verilog/cells/cell_F10414.v']
    for s in specs():
        prefix='chm/sil/'+s['bn']+'.dm!1_/'+s['bn'];files.extend([prefix+'.wl',prefix+'.lc'])
    hashes={};actions=[]
    for name in sorted(set(files)):
        raw=(ROOT/name).read_bytes();p=inputs/name;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(raw)
        hashes[name]=hashlib.sha256(raw).hexdigest()
    def execute(command,log,expect=0):
        with log.open('w')as f:code=subprocess.run([str(x)for x in command],cwd=inputs,stdout=f,stderr=subprocess.STDOUT).returncode
        actions.append(dict(command=[str(x)for x in command],returncode=code,log=str(log)))
        if expect==0:assert code==0,(log,code)
        return code
    reference=out/'reference'
    execute([sys.executable,inputs/'tools/generate_r_file_reference.py','--output',reference],out/'generation.log')
    manifest=json.loads((reference/'manifest.json').read_text());assert (manifest['physical_bits'],manifest['packages'],manifest['original_pins'])==(9216,14,244)
    source=(inputs/CANDIDATE).read_text();results=[];baseline=None
    for profile,gate_ps,read_ps in PROFILES:
        d=out/profile;d.mkdir();timed=d/'reference';timed.mkdir()
        for p in reference.glob('*.sv'):(timed/p.name).write_bytes(p.read_bytes())
        raw=(inputs/'verilog/cells/cell_MC10170.v').read_text()
        raw=re.sub(r'\bassign\s+(p\d+)\b',lambda m:'assign #('+str(gate_ps)+') '+m[1],raw)
        (timed/'cell_MC10170.v').write_text('`timescale 1ps/1ps\n'+raw)
        raw=(inputs/'verilog/verilator/r_file_original_storage.sv').read_text().replace('READ_PS=1000','READ_PS='+str(read_ps))
        (timed/'r_file_original_storage.sv').write_text(raw)
        support=[inputs/TEST,*sorted(timed.glob('*.sv')),*sorted(timed.glob('*.v'))]
        cases=[('original',source)]
        if mutations and profile=='balanced':
            for name,edits in MUTATIONS.items():
                raw=source
                for before,after in edits:
                    assert raw.count(before)>=1,(name,before);raw=raw.replace(before,after)
                cases.append((name,raw))
        for name,raw in cases:
            c=d/name;c.mkdir();rtl=c/'dorado_r_file_ports.sv';rtl.write_text(raw)
            execute(['iverilog','-g2012','-s','tb_r_file_ports','-o',c/'test.vvp',rtl,*support],c/'compile.log')
            code=execute(['vvp',c/'test.vvp'],c/'test.log',expect=None)
            text=(c/'test.log').read_text()
            if name=='original':
                assert code==0 and 'R_FILE_PORTS PASS 'in text,(profile,text)
                summary=text.split('R_FILE_PORTS PASS ')[1].splitlines()[0]
                if baseline is None:baseline=summary
                assert summary==baseline
            else:assert code!=0 and 'R file port mismatch'in text,(name,text)
            results.append(dict(profile=profile,case=name,returncode=code,expected_failure=name!='original'))
        c=d/'verilator';c.mkdir()
        execute(['verilator','--binary','--timing','--top-module','tb_r_file_ports','-Wno-fatal',
            '-Wno-MULTIDRIVEN','--Mdir',c/'obj','-o','test',inputs/CANDIDATE,*support],c/'compile.log')
        execute([c/'obj/test'],c/'test.log')
        assert 'R_FILE_PORTS PASS '+baseline in(c/'test.log').read_text()
        print('R_FILE_PORTS',profile,'both simulators:',baseline,flush=True)
    for name,digest in hashes.items():assert hashlib.sha256((inputs/name).read_bytes()).hexdigest()==digest
    record=dict(status='PASS_R_FILE_FUNCTIONAL_PORTS',scope=__doc__,inputs=hashes,actions=actions,
        profiles=PROFILES,cases=results,summary=baseline,reference=manifest,native_integrated=False,
        coupled_control_qualified=False,physical_timing_qualified=False)
    (out/'results.json').write_text(json.dumps(record,indent=2)+'\n')
    print(record['status'],len(MUTATIONS)if mutations else 0,'bad variants rejected')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--mutations',action='store_true');a=p.parse_args();run(a.output.resolve(),a.mutations)
