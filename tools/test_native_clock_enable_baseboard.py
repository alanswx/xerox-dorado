#!/usr/bin/env python3
"""Compare original-ROM BaseBoard gated-clock and common-clock implementations.

Run the original 6502 reset clear loop and MidasSetup under pseudorandom host
pauses. Compare every sequential scalar/vector after every host edge, all
BaseBoard outputs, and every written RAM cell before successful completion.
Host-only services, the other boards and full timing require separate gates.
"""
import argparse
import json
from pathlib import Path
import re
import shutil
import subprocess
import time
import xml.etree.ElementTree as ET

from diagnose_native_baseboard_reset import fixture,BOARD
from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha


def run(stage,inventory,xml_source,baseline_lint,out,negative=False):
    assert not out.exists();out.mkdir(parents=True)
    manifest=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,manifest['source_hashes'])
    assert manifest['clock_enable_conversion_staged']and manifest['sysper']==4
    inv=json.loads((inventory/'results.json').read_text());assert sha(inventory/'results.json')==manifest['clock_enable_inventory_sha256']
    xml=ET.parse(xml_source/'native.xml').getroot();assert sha(xml_source/'native.xml')==manifest['clock_enable_XML_sha256']
    modules={m.get('name'):m for m in xml.findall('.//module')}
    types={t.get('id'):t for t in xml.findall('.//typetable/*')}
    instances={i['path']:i for i in inv['instances']}
    roots=[p for p,i in instances.items()if i['original_module']=='BaseBd_m_Rev_m_Am_nominal'];assert len(roots)==1
    root=roots[0]
    raw=(Path(manifest['parent_stage'])/BOARD).read_text()
    bench,environment=fixture(raw,4,True,True,False,False,False)
    bench=bench.replace('always #8.75 host_clk=~host_clk;','always #7.5 host_clk=~host_clk;')
    header=raw[:raw.index(');')]
    ports=re.findall(r'^\s*(input|output)\s+(?:wire\s+)?(\[[^]]+\]\s*)?(\w+)\s*[,)]?',header,re.M)
    outputs={n for d,w,n in ports if d=='output'}
    wires='\n'.join(f'wire {w or ""}ce_{n};'for d,w,n in ports if d=='output')
    connections=[".guest_advance(1'b1)"if negative else'.guest_advance(step)']
    for direction,width,name in ports:
        if direction=='output':value='ce_'+name
        elif name=='sys_clk':value='host_clk'
        else:
            value=environment[name]
            if value in outputs:value='ce_'+value
        connections.append(f'.{name}({value})')
    candidate=wires+'\nBaseBd_m_Rev_m_Am_nominal_ce #(.SYSPER(4)) candidate(\n'+',\n'.join(connections)+'\n);\n'
    state_checks=[];ram_checks=[];seen=set()
    def bounds(value):
        match=re.fullmatch(r"\d+'s?h([0-9a-fA-F]+)",value);assert match,value
        return int(match[1],16)
    for process in inv['processes']:
        if not(process['path']==root or process['path'].startswith(root+'.')):continue
        m=modules[process['module']];parents={c:p for p in m.iter()for c in p}
        for variable in process['written_variables']:
            key=(process['path'],variable)
            if key in seen:continue
            seen.add(key)
            variables=[v for v in m.findall('.//var')if v.get('name')==variable];assert len(variables)==1,key
            var=variables[0];scope=[];cursor=var
            while cursor in parents:
                cursor=parents[cursor]
                if cursor.tag in ['begin','genblock']and cursor.get('name'):scope.insert(0,cursor.get('name'))
            suffix=process['path'][len(root):]+'.'+'.'.join(scope+[variable])
            dtype=types[var.get('dtype_id')]
            if dtype.tag=='unpackarraydtype':
                dim=[bounds(c.get('name'))for c in dtype.findall('range/const')];assert len(dim)==2
                ram_checks.append(f'for(integer k={min(dim)};k<={max(dim)};k++) if(bb{suffix}[k]!==candidate{suffix}[k]) $fatal(1,"CLOCK_ENABLE_RAM_MISMATCH {suffix} word=%0d",k);')
            else:
                assert dtype.tag in ['basicdtype','packarraydtype'],(key,dtype.tag)
                state_checks.append(f'if(bb{suffix}!==candidate{suffix}) $fatal(1,"CLOCK_ENABLE_STATE_MISMATCH {suffix} host=%0d step=%b",host_checks,step);')
    output_checks=[f'if({n}!==ce_{n}) $fatal(1,"CLOCK_ENABLE_OUTPUT_MISMATCH {n} host=%0d",host_checks);'for n in sorted(outputs)]
    checks='\n'.join(state_checks+output_checks)
    extra=f'''
{candidate}
longint unsigned host_checks=0,paused_checks=0,active_checks=0;
always @(negedge host_clk)begin
 #1;host_checks++;
 if(step)active_checks++;else paused_checks++;
 {checks}
end
task automatic differential_finish;
 begin
  #1;
  {checks}
  {chr(10).join(ram_checks)}
  if(!midas_returned || active_checks<1000 || paused_checks<1000)$fatal(1,"CLOCK_ENABLE insufficient original-ROM coverage");
  $display("CLOCK_ENABLE_BASEBOARD_PASS host=%0d active=%0d paused=%0d state_vectors={len(state_checks)} outputs={len(outputs)} RAM_arrays={len(ram_checks)}",host_checks,active_checks,paused_checks);
  $finish;
 end
endtask
'''
    bench=bench.replace('$finish;','differential_finish;')
    assert bench.count('\nendmodule')==1;bench=bench.replace('\nendmodule',extra+'\nendmodule',1)
    tb=out/'tb_native_baseboard_reset.sv';tb.write_text(bench)
    lint=json.loads((baseline_lint/'run-state.json').read_text());assert lint['stage_manifest_sha256']==manifest['parent_manifest_sha256']
    command=[s.replace(manifest['parent_stage'],str(stage))for s in lint['command']]
    sources=[s for s in command if s.endswith(('.v','.sv'))and not s.endswith(('tb_native_cedar.sv','xsds_model.sv'))]
    options=[s for s in command if s.startswith(('-D','-I'))]
    cmd=['verilator','--binary','--timing','--top-module','tb_native_baseboard_reset','-j','2','-Wno-fatal','-Wno-PINMISSING','-Wno-TIMESCALEMOD',
        *options,'--Mdir',str(out/'obj'),'-o','test',*sources,str(tb)]
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    state=dict(status='RUNNING',scope=__doc__,stage=str(stage),stage_manifest_sha256=sha(stage/'r-front-control-native.json'),
        baseline_inventory_sha256=sha(inventory/'results.json'),started=time.time(),actions=[],negative_missing_enable=negative,
        compared_state_vectors=len(state_checks),compared_outputs=len(outputs),compared_RAM_arrays=len(ram_checks),
        original_firmware_and_reset_loop_checks_retained=True,hardware_changed=False,full_machine_qualified=False)
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    for label,command in [('compile',cmd),('test',[str(out/'obj/test')])]:
        with(out/(label+'.log')).open('x')as log:
            p=subprocess.Popen(command,cwd=stage/'src',stdout=log,stderr=log)
            action=dict(label=label,pid=p.pid,command=command,started=time.time());state['actions'].append(action);save();code=p.wait()
        action.update(returncode=code,finished=time.time());save()
        if code:break
    log=(out/'test.log').read_text()if(out/'test.log').exists()else''
    passed=(code!=0 and 'CLOCK_ENABLE_STATE_MISMATCH'in log)if negative else(code==0 and 'CLOCK_ENABLE_BASEBOARD_PASS'in log)
    state.update(status=('PASS_MISSING_CLOCK_ENABLE_COUNTEREXAMPLE'if negative else'PASS_ORIGINAL_ROM_BASEBOARD_COMMON_CLOCK_EQUIVALENCE')if passed else'TERMINAL_BASEBOARD_CLOCK_ENABLE_NOT_ESTABLISHED',
        returncode=code,finished=time.time(),files={p.name:sha(p)for p in out.iterdir()if p.is_file()and p.name!='run-state.json'})
    save();print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['stage','inventory','xml-source','baseline-lint','output']:p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--negative',action='store_true')
    a=p.parse_args();raise SystemExit(run(a.stage.resolve(),a.inventory.resolve(),a.xml_source.resolve(),a.baseline_lint.resolve(),a.output.resolve(),a.negative))
