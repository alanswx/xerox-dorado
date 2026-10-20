#!/usr/bin/env python3
"""Paired original/staged disk RTL under the real core clock-gating contract.

Compare all original sequential fields from the frozen native XML, including
the entire buffer at completion. Covers the capture proof's reset/remount/tag/
stall/host-failure workload. Serial write/CRC payload qualification is separate.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import test_disk_cache_capture as base
import generate_disk_cache_phase as gen
from generate_serial_boards import once,raw_receipt,ROOT


def normalized(source):
    declarations=['  reg [11:0] cyl = INITIAL_CYL;\n','  reg [5:0] head = INITIAL_HEAD;\n',
                  '  reg [2:0] host_state = 0;\n','  reg [1:0] wr_state = 0, wr_block = 0;\n','  reg [5:0] wr_bits = 0;\n']
    locations=sorted((source.index(d),d)for d in declarations);bare=source
    for _,d in locations:bare=once(bare,d,'')
    marker='  function automatic [31:0] fault_status';prefix=''.join(declarations)
    result=once(bare,marker,prefix+marker);inverse=once(result,prefix+marker,marker)
    for pos,d in locations:inverse=inverse[:pos]+d+inverse[pos:]
    assert inverse==source
    return result


def fixture(rows):
    top=base.TOP
    start=top.index(' dorado_pack_hps #');end=top.index(');',start)+2
    instance=top[start:end]
    original=once(instance,'dorado_pack_hps #','dorado_pack_hps_reference #')
    candidate=once(instance,' dut(',' candidate(')
    candidate=once(candidate,'.ready(pack_ready)', '.ready(candidate_ready)')
    top=top[:start]+' wire candidate_ready;\n'+original+'\n'+candidate+top[end:]
    checks='  if(candidate_ready!==pack_ready)$fatal(1,"disk phase ready differs");\n'
    for row in rows:
        name=row['path'].split('.')[-1]
        if name=='buffer':continue
        checks+=f'  if(candidate.{name}!==dut.{name})$fatal(1,"disk phase field differs: {name}");\n'
    top=once(top,'  if(dut.proof_captures!=previous_captures)begin',checks+'  if(dut.proof_captures!=previous_captures)begin')
    top=once(top,'  // The PLL/reset guard', '''  for(integer a=0;a<8192;a=a+1)begin dut.buffer[a]=(a*16'h9e37)^16'h584a;candidate.buffer[a]=dut.buffer[a];end
  // The PLL/reset guard''')
    top=once(top,'  $display("DISK CACHE CAPTURE PASS', '''  for(integer a=0;a<8192;a=a+1)if(candidate.buffer[a]!==dut.buffer[a])$fatal(1,"disk phase buffer differs");
  $display("DISK CACHE PHASE PASS''')
    return top


def run(out,mutations):
    out.mkdir(parents=True,exist_ok=False);source=gen.reference();candidate=gen.transform(source);assert gen.inverse(candidate)==source
    report=ROOT/'fpga/mister/reports/2026-10-19-snapshot-serial-chipwidth'
    inventory=raw_receipt(report,'classification/inventory.json');data=json.loads(inventory)
    rows=[r for r in data['candidates']if r['category']=='sequential'and r['path'].startswith('dorado_native_machine.u_media.')]
    assert sum(r['bits']for r in rows)==131816
    top=fixture(rows)
    original=once(source,'module dorado_pack_hps #','module dorado_pack_hps_reference #')
    def instrument(text):
        text=once(text,'  reg valid = 0;','  integer proof_captures=0;\n  reg valid = 0;')
        # Observer only; original and staged capture branches are otherwise exact.
        text=once(text,'          cached_lba <= target_word >> 8;', '          proof_captures<=proof_captures+1;\n          cached_lba <= target_word >> 8;')
        return normalized(text)
    cases=[('candidate',None,None)]
    if mutations:cases += [('wrong_sampling_edge','always @(negedge clk_sys)','always @(posedge clk_sys)'),
                           ('wrong_cyl','cached_cyl <= cache_source_cyl;',"cached_cyl <= 12'b0;"),
                           ('wrong_head','cached_head <= cache_source_head;','cached_head <= cache_source_cyl[5:0];')]
    results=[]
    for label,before,after in cases:
        case=out/label;case.mkdir();text=candidate
        if before:text=once(text,before,after)
        (case/'fixture.sv').write_text(top);(case/'reference.sv').write_text(instrument(original));(case/'candidate.sv').write_text(instrument(text))
        command=['iverilog','-g2012','-s','disk_cache_capture','-o',str(case/'test.vvp'),str(case/'fixture.sv'),str(case/'reference.sv'),str(case/'candidate.sv')]
        (case/'command.json').write_text(json.dumps(command,indent=2)+'\n')
        with(case/'build.log').open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run(['vvp',str(case/'test.vvp')],capture_output=True,text=True,timeout=60)
        text=result.stdout+result.stderr;(case/'test.txt').write_text(text);print(label+': '+text.strip(),flush=True)
        if before:assert result.returncode and 'disk phase field differs'in text,(label,text)
        else:assert not result.returncode and 'DISK CACHE PHASE PASS'in text,text
        results.append(dict(case=label,returncode=result.returncode))
    inputs=out/'executed-inputs';inputs.mkdir();(inputs/'original.sv').write_text(source);(inputs/'candidate.sv').write_text(candidate)
    (inputs/'native-inventory.json').write_bytes(inventory)
    for p in (Path(__file__),Path(base.__file__),Path(gen.__file__)):(inputs/p.name).write_bytes(p.read_bytes())
    (out/'results.json').write_text(json.dumps(dict(status='PASS',scope=__doc__,cases=results,original_fields=len(rows),original_bits=131816,
        new_required_snapshot_bits=18,integrated=False,timing_qualified=False,new_timing_exceptions=0,
        executed_inputs={p.name:hashlib.sha256(p.read_bytes()).hexdigest()for p in inputs.iterdir()},
        limits='Functional prototype; no native integration, new wrapper snapshot schema, matched fit/timing or serial payload regression claim.'),indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--mutations',action='store_true');a=p.parse_args();run(a.output.resolve(),a.mutations)
