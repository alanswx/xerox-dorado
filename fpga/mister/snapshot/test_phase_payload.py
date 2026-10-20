#!/usr/bin/env python3
"""Real FF scan controller, nineteen phase copies and frozen original FF oracles.

Both PRECAP variants of MC10176/MC10231 exercise four-state physical inputs,
destructive scan/restore/resume and partial-import quarantine. Default-off
copies additionally accept arbitrary raw control combinations. Deliberately
unqualified hold/write timing MUST reject the optional producer contract.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import generate_phase_payload as gen

HERE=Path(__file__).resolve().parent


def fixture():
    code=[]
    for g in range(4):
        kind='MC10176'if g<2 else'MC10231';width=gen.KINDS[kind];offset=(0,13,26,32)[g]
        outputs=(2,3,4,13,14,15)if g<2 else(2,3,14,15)
        allpins=(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16)
        for label,module,phase,advance,hold,write,data,target in[
            ('reference','reference_'+kind,None,'global_step','global_hold','scan_step',f"18'(reference_next[{offset}+:{width}])",'reference_bank'),
            ('candidate','dorado_serial_MemC__cell_'+kind+'_snapshot',1,f'local_step[{g}]','global_hold','scan_step',f"18'(next_bank[{offset}+:{width}])",'bank'),
            ('default_reference','reference_'+kind,None,'default_advance','default_hold','default_write','default_data','default_reference_bank'),
            ('default_candidate','dorado_serial_MemC__cell_'+kind+'_snapshot',0,'default_advance','default_hold','default_write','default_data','default_bank')]:
            name=label+'_'+str(g);con=[f'.state_host_clk(clk)',f'.sys_clk(clk)',f'.state_advance({advance})',
                f'.state_hold({hold})',f'.state_write({write})',f'.state_data({data})',f'.state_result({name}_state)']
            if label=='candidate'and g==0:
                con[4]='.state_write(scan_step)'
                # The unheld-write negative tests the asserted producer contract.
                con[4]='.state_write(scan_step\n`ifdef UNHELD_WRITE\n || (edges==5)\n`endif\n)'
            code.append(f'wire [17:0] {name}_state;')
            for pin in allpins:con.append(f'.p{pin}()'if pin in outputs else f'.p{pin}(pins[{pin-1}])')
            params=f'.PRECAP({g%2})'+(''if phase is None else f',.PHASE_QUALIFIED({phase})')
            code.append(f'{module} #({params}) {name}('+','.join(con)+');')
            code.append(f'assign {target}[{offset}+:{width}]={name}_state[{width-1}:0];')
    return gen.once((HERE/'test_phase_payload.sv').read_text(),' // INSTANCES','\n'.join(code))


def run(out):
    out.mkdir(parents=True,exist_ok=False);inputs=out/'executed-inputs';inputs.mkdir()
    sources,manifest=gen.generate();top=fixture()
    originals={kind+'.v':gen.once(gen.reference(kind),'module dorado_serial_MemC__cell_'+kind+'_snapshot ',
        'module reference_'+kind+' ')for kind in gen.KINDS}
    assets={**sources,**originals,'board_phase_enable.sv':(HERE/'board_phase_enable.sv').read_text(),
        'ff_scan_controller.sv':(HERE/'ff_scan_controller.sv').read_text(),'fixture.sv':top}
    for n,s in assets.items():(inputs/n).write_text(s)
    for p in(Path(__file__),Path(gen.__file__),HERE/'test_phase_payload.sv'):(inputs/p.name).write_bytes(p.read_bytes())
    cases=[('candidate',None,None,None,None,()),
        ('wrong_phase','board_phase_enable.sv','always @(negedge clk)local_step','always @(posedge clk)local_step','local board advance phase differs',()),
        ('lost_hold_permission','fixture.sv','.permit(permit)','.permit(desired)','local board advance phase differs',()),
        ('lost_payload_enable','MC10231.sv','else if(state_advance)begin',"else if(1'b1)begin",'phase payload physical state differs',()),
        ('wrong_precapture','MC10176.sv','if (ck_en) q <= PRECAP ? d_pre : {p12, p11, p10, p7, p6, p5};',
            'if (ck_en) q <= {p12, p11, p10, p7, p6, p5};','phase payload physical state differs',()),
        ('late_hold_contract',None,None,None,'phase payload advance/hold contract violated',('-DLATE_HOLD',)),
        ('unheld_write_contract',None,None,None,'phase payload scan/hold contract violated',('-DUNHELD_WRITE',))]
    records=[]
    for label,target,before,after,expected,defines in cases:
        case=out/label;case.mkdir();paths={n:inputs/n for n in assets}
        if target:
            source=assets[target];assert before in source
            # Mutate only the optional payload branch; keep the original
            # default-off branch and every independent oracle intact.
            paths[target]=case/target;paths[target].write_text(source.replace(before,after,1))
        cmd=['iverilog','-g2012','-s','tb',*defines,'-o',str(case/'test.vvp'),*map(str,paths.values())]
        (case/'commands.json').write_text(json.dumps([cmd,['vvp',str(case/'test.vvp')]],indent=2)+'\n')
        with(case/'build.log').open('w')as log:subprocess.run(cmd,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run(['vvp',str(case/'test.vvp')],capture_output=True,text=True,timeout=120)
        text=result.stdout+result.stderr;(case/'test.log').write_text(text);print(label+': '+text.strip(),flush=True)
        if expected:assert result.returncode and expected in text,text
        else:assert not result.returncode and'PHASE PAYLOAD PASS'in text,text
        records.append(dict(case=label,returncode=result.returncode,expected_failure=expected))
    (out/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
    (out/'results.json').write_text(json.dumps(dict(status='PASS_COMPONENT_CONTRACT_ONLY',scope=__doc__,cases=records,
        physical_bits=38,required_distribution_bits=19,additional_payload_bits=0,
        inputs={p.name:hashlib.sha256(p.read_bytes()).hexdigest()for p in inputs.iterdir()},
        limits='Synthetic synchronous hold producer with actual scan controller and original FF payload oracles. Native source-domain/phase proof, full boards/SDRAM, RAM families, inventory/access and routed timing remain required.'),indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args();run(a.output.resolve())
