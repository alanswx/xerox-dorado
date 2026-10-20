#!/usr/bin/env python3
"""Couple a complete MSA and pause coordinator to the actual arbiter/SDRAM.

Use real idle outputs and physical XSDS chip models, keeping refresh live.
Original frozen MSA RTL observes the same raw peer for bit/state comparison.
No host code completes a guest instruction or services raw memory requests.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import resource
import subprocess
import generate_msa_late_freeze as gen
import test_msa_late_freeze as component

HERE=Path(__file__).resolve().parent


def run(out,mutations):
    resource.setrlimit(resource.RLIMIT_CORE,(0,0))
    out.mkdir(parents=True,exist_ok=False);inputs=out/'executed-inputs';inputs.mkdir()
    board,manifest,helpers=gen.generate();fixture,_,metadata=component.fixture(manifest)
    header=fixture[fixture.index('(\n')+2:fixture.index(');')]
    declarations=[]
    for line in re.split(r',\s*(?=(?:input|output)\s+wire\b)',header.strip()):
        line=line.strip().rstrip(',')
        assert line.startswith(('input wire ','output wire ')),line
        declarations.append(line.replace('input wire ','logic ',1).replace('output wire ','logic ',1)+';')
    mask=sum(1<<p for p in metadata['canonical_bit'][:866])
    bench=(HERE/'test_msa_sdram_pause.sv').read_text().replace('@DECLARATIONS@','\n    '.join(declarations))
    bench=bench.replace('@FF_MASK@',format(mask,'x')).replace('@CANONICAL_BITS@',','.join(map(str,metadata['canonical_bit'])))
    originals=gen.parent.base.original_cells()
    sources={'msa_board.sv':board,**helpers,'fixture.sv':fixture,'tb.sv':bench,
        'memory_pause_barrier.sv':(HERE/'memory_pause_barrier.sv').read_text()}
    for name in ('dorado_storage_arb.sv','dorado_storage_sdram.sv'):
        sources[name]=(gen.parent.base.ROOT/'fpga/mister'/name).read_text()
    sources['msa_ddram.v']=gen.parent.base.original(gen.parent.base.SOURCE).decode()
    sources['dorado_storage_group.sv']=gen.parent.compact.original.original_source().decode()
    sources.update({n:r.decode()for n,r in originals.items()})
    sources['xsds_128mbyte_sdram_model.sv']=(gen.parent.base.ROOT/'verilog/verilator/third_party/xsds/xsds_128mbyte_sdram_model.sv').read_text()
    for n,s in sources.items():(inputs/n).write_text(s)
    for p in(Path(__file__),HERE/'test_msa_sdram_pause.sv'):(inputs/p.name).write_bytes(p.read_bytes())
    cases=[('candidate',None,None,None)]
    if mutations:cases +=[
        ('early_frontend_hold','msa_board.sv','wire frontend_hold=state_frontend_hold || state_scan_hold_required;',
         'wire frontend_hold=state_global_hold || state_scan_hold_required;'),
        ('stopped_refresh','dorado_storage_sdram.sv','IDLE:if(refresh_due)begin','IDLE:if(refresh_due && req_valid)begin'),
        ('nonsticky_hold','memory_pause_barrier.sv','assign frontend_hold=phase==FROZEN || phase==QUARANTINE;',
         'assign frontend_hold=(phase==FROZEN && frontends_drained) || phase==QUARANTINE;'),
        ('early_guest_release','memory_pause_barrier.sv','assign guest_hold=owner_hold || phase!=RUN;',
         'assign guest_hold=owner_hold;')]
    results=[]
    for label,target,before,after in cases:
        case=out/label;case.mkdir();paths={n:inputs/n for n in sources}
        if target:
            assert sources[target].count(before)==1
            paths[target]=case/target;paths[target].write_text(sources[target].replace(before,after,1))
        command=['verilator','--binary','--timing','--assert','-j','4','--top-module','tb','-Wno-fatal',
            '--Mdir',str(case/'obj'),'-o','test',*map(str,paths.values())]
        (case/'commands.json').write_text(json.dumps([command,[str(case/'obj/test')]],indent=2)+'\n')
        with(case/'build.log').open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run([str(case/'obj/test')],capture_output=True,text=True,timeout=180)
        text=result.stdout+result.stderr;(case/'test.txt').write_text(text);print(label+': '+text[:800],flush=True)
        if target:
            diagnostics=('physical SDRAM refresh stopped','tRAS(max) violation')if label=='stopped_refresh'else(
                'ordinary MSA raw controls differ','ordinary physical MSA state differs',
                'rotation lost real raw service hold','guest hold released before frontend thaw')
            assert result.returncode and any(s in text for s in diagnostics),text
        else:assert not result.returncode and 'MSA SDRAM PAUSE PASS'in text,text
        results.append(dict(case=label,returncode=result.returncode))
    (out/'results.json').write_text(json.dumps(dict(status='PASS',scope=__doc__,
        cases=results,
        executed_inputs={n:hashlib.sha256((inputs/n).read_bytes()).hexdigest()for n in sources},
        limits='One actual MSA/16 raw groups, real SDRAM pin model and service idle wiring. Eight-board/native integration, full wrapper/ARM state, fit/timing and complete FPGA restore remain pending.'),indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--mutations',action='store_true');a=p.parse_args();run(a.output.resolve(),a.mutations)
