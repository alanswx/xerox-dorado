#!/usr/bin/env python3
"""Reject damaged isolated clock-fit evidence, including rehashed input changes."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import shutil
from audit_control_clock_fit import audit


def run(source, qualified, output, allow_rejected_baseline=False):
    baseline=audit(source,qualified,False)
    assert baseline['status']==('REJECTED_COMPONENT_TIMING'if allow_rejected_baseline else'PASS_COMPONENT_TIMING')
    output.mkdir(parents=True,exist_ok=False)
    cases=[]
    def changed_input(directory,name,edit):
        p=directory/name;p.write_text(edit(p.read_text()))
        digest=hashlib.sha256(p.read_bytes()).hexdigest()
        for file,key in [('provenance.json','files'),('collected-input-check.json','actual')]:
            p=directory/file;q=json.loads(p.read_text());q[key][name]=digest
            p.write_text(json.dumps(q,indent=2)+'\n')
    names=['wrong_dimensions','relaxed_period_rehashed','changed_evaluator_rehashed',
           'changed_dependency_rehashed','missing_corner','missing_evaluator','negative_setup','negative_evaluator']
    component=json.loads((source/'provenance.json').read_text()).get('component')
    if component=='task_producers':names+=['changed_cp_dependency_rehashed']
    if component=='mir_ready':names+=['changed_cp_dependency_rehashed','changed_mir_dependency_rehashed','changed_fixture_wiring_rehashed']
    if component=='conta_mir':
        names.remove('changed_dependency_rehashed');names+=['changed_fixture_wiring_rehashed']
    if component in ('task_ready_capture','r_front_ready','r_front_mir','r_front_mir_mask','r_front_stack_adder','r_front_visible'):names+=['changed_ready_dependency_rehashed']
    if component in ('r_front_ready','r_front_mir','r_front_mir_mask','r_front_stack_adder','r_front_visible'):names+=['changed_front_dependency_rehashed','changed_fixture_wiring_rehashed']
    if component=='r_front_mir':names+=['changed_conta_dependency_rehashed']
    if component in ('r_front_mir_mask','r_front_stack_adder','r_front_visible'):names+=['changed_clock_adapter_rehashed','changed_project_sources_rehashed']
    if component in ('r_storage','r_memory','r_front','r_front_functional','r_front_ready','r_front_mir','r_front_mir_mask','r_front_stack_adder','r_front_visible'):
        names+=['missing_ram_bank','wrong_ram_width','wrong_ram_type','wrong_ram_init',
                'wrong_ram_bits','wrong_ram_output_register','wrong_ram_collision_mode']
    if component in ('r_memory','r_front','r_front_functional','r_front_ready','r_front_mir','r_front_mir_mask','r_front_stack_adder','r_front_visible'):
        names+=['wrong_ram_address_register']
    if component in ('r_front','r_front_functional','r_front_ready','r_front_mir','r_front_mir_mask','r_front_stack_adder','r_front_visible'):
        names+=['changed_generated_cone_rehashed']
    if allow_rejected_baseline:
        # A negative timing number cannot demonstrate a new rejection when
        # the source is already timing-rejected. Only malformed-evidence
        # controls remain, and each must raise rather than return REJECTED.
        names.remove('negative_setup');names.remove('negative_evaluator')
    for name in names:
        d=output/name;shutil.copytree(source,d)
        if name=='wrong_dimensions':
            p=d/'provenance.json';q=json.loads(p.read_text());q['launch_bits']-=1;p.write_text(json.dumps(q))
        elif name=='relaxed_period_rehashed':
            changed_input(d,'clock.sdc',lambda s:s.replace('15.000','20.000'))
        elif name.startswith('changed_'):
            evaluator=json.loads((d/'provenance.json').read_text())['component']
            target='dorado_'+evaluator+'_step.sv' if name.startswith('changed_evaluator') else 'dorado_task_capture_step.sv'
            if evaluator in ('r_front','r_front_functional')and name.startswith('changed_evaluator'):target='dorado_r_front_step.sv'
            if evaluator in ('r_front_mir_mask','r_front_stack_adder','r_front_visible'):
                if name.startswith('changed_evaluator'):target='dorado_r_front_commands_step.sv'
                if name=='changed_dependency_rehashed':target='dorado_local_task_capture_step.sv'
                if name=='changed_clock_adapter_rehashed':target='dorado_local_r_port_preview.sv'
            if evaluator=='task_producers'and name=='changed_dependency_rehashed':target='dorado_task_ready_step.sv'
            if evaluator=='task_producers'and name=='changed_cp_dependency_rehashed':target='dorado_cp_control_step.sv'
            if evaluator=='conta_mir'and name=='changed_fixture_wiring_rehashed':target='top.sv'
            if evaluator=='mir_ready':
                if name=='changed_dependency_rehashed':target='dorado_task_ready_step.sv'
                if name=='changed_cp_dependency_rehashed':target='dorado_cp_control_step.sv'
                if name=='changed_mir_dependency_rehashed':target='dorado_mir_capture_step.sv'
                if name=='changed_fixture_wiring_rehashed':target='top.sv'
            if evaluator in ('task_ready_capture','r_front_ready','r_front_mir','r_front_mir_mask','r_front_stack_adder','r_front_visible')and name=='changed_ready_dependency_rehashed':target='dorado_task_ready_step.sv'
            if evaluator in ('r_front_ready','r_front_mir')and name=='changed_front_dependency_rehashed':target='dorado_r_front_step.sv'
            if evaluator in ('r_front_ready','r_front_mir')and name=='changed_fixture_wiring_rehashed':target='top.sv'
            if evaluator in ('r_front_mir_mask','r_front_stack_adder','r_front_visible')and name=='changed_front_dependency_rehashed':target='dorado_local_r_front_step.sv'
            if evaluator in ('r_front_mir_mask','r_front_stack_adder','r_front_visible')and name=='changed_fixture_wiring_rehashed':target='top.sv'
            if evaluator=='r_front_mir'and name=='changed_conta_dependency_rehashed':target='dorado_conta_mir_step.sv'
            if name=='changed_generated_cone_rehashed':target='dorado_r_front_cone.sv'
            if name=='changed_fixture_wiring_rehashed':
                zero="792'b0"if evaluator in ('r_front_mir_mask','r_front_stack_adder','r_front_visible')else"786'b0"if evaluator=='r_front_mir'else"98'b0"if evaluator=='conta_mir'else"50'b0"if evaluator=='mir_ready'else"752'b0"
                changed_input(d,target,lambda s:s.replace('result<={next_q,',"result<={"+zero+",",1))
            elif name=='changed_project_sources_rehashed':
                changed_input(d,'clock_fit.qsf',lambda s:s+'\nset_global_assignment -name SYSTEMVERILOG_FILE unqualified_source.sv\n')
            else:
                changed_input(d,target,lambda s:s+'\n// changed qualified source\n')
        elif name=='missing_corner':
            (d/'clock-hold-MIN_fast_1100mv_-40c.rpt').unlink()
        elif name=='missing_evaluator':
            (d/'clock-evaluator-7_slow_1100mv_100c.rpt').unlink()
        elif '_ram_'in name:
            p=d/'clock_fit.map.rpt';text=p.read_text();lines=text.splitlines()
            index=next(i for i,line in enumerate(lines)if 'ALTDPRAM_INSTANCE'in line and '; MLAB'in line)
            if name=='missing_ram_bank':lines.pop(index)
            elif name in ('wrong_ram_width','wrong_ram_type','wrong_ram_init'):
                fields=lines[index].split(';')
                if name=='wrong_ram_width':fields[5]=' 8 '
                elif name=='wrong_ram_type':fields[2]=' M10K '
                else:fields[-2]=' nonzero.mif '
                lines[index]=';'.join(fields)
            elif name=='wrong_ram_bits':
                lines=[re.sub(r'(Total MLAB memory bits\s*;\s*)9216',r'\g<1>9217',line)for line in lines]
            else:
                key={'wrong_ram_output_register':'OUTDATA_REG','wrong_ram_address_register':'RDADDRESS_REG',
                     'wrong_ram_collision_mode':'READ_DURING_WRITE_MODE_MIXED_PORTS'}[name]
                replacement='OLD_DATA'if name=='wrong_ram_collision_mode'else'CLOCK0'
                text='\n'.join(lines)+'\n'
                text,count=re.subn(r'(;\s*'+key+r'\s*;\s*)[^;]+',lambda m:m[1]+replacement+' ',text,count=1)
                assert count==1;lines=text.splitlines()
            p.write_text('\n'.join(lines)+'\n')
        else:
            mode='evaluator' if name=='negative_evaluator' else 'setup'
            p=d/('clock-'+mode+'-7_slow_1100mv_100c.rpt');s=p.read_text()
            s,n=re.subn(r'(Worst case slack is )-?\d+\.\d+',r'\g<1>-0.001',s,flags=re.I);assert n==1
            s,n=re.subn(r'(;\s*Slack\s*;\s*)-?\d+\.\d+',r'\g<1>-0.001',s);assert n>=1
            p.write_text(s)
        try:
            result=audit(d,qualified,False)
        except (AssertionError,FileNotFoundError) as error:
            cases.append(dict(case=name,rejected=True,diagnostic=str(error)))
        else:
            assert not allow_rejected_baseline,(name,'malformed rejected-fit evidence was accepted structurally')
            assert result['status']=='REJECTED_COMPONENT_TIMING',(name,'damaged evidence accepted')
            cases.append(dict(case=name,rejected=True,diagnostic=result['status']))
    record=dict(status='PASS',cases=cases)
    if allow_rejected_baseline:record.update(status='PASS_REJECTED_FIT_READER_CONTROLS',baseline=baseline['status'],timing_negative_controls_omitted=True)
    (output/'results.json').write_text(json.dumps(record,indent=2)+'\n')
    print('FIT_READER_CONTROLS PASS',len(cases),'damaged/rehashed cases rejected')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path)
    p.add_argument('--qualified',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--allow-rejected-baseline',action='store_true')
    a=p.parse_args();run(a.directory.resolve(),a.qualified.resolve(),a.output.resolve(),a.allow_rejected_baseline)
