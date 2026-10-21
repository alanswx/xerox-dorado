#!/usr/bin/env python3
"""Reject corrupt native timing/source/physical-state evidence.

Use file-only symlink overlays of an immutable terminal receipt. Mutated
files are separately created after unlinking any overlay link; never write
through to original evidence. No Quartus, FPGA or guest changes.
"""
import argparse
import json
from pathlib import Path
import re

import review_r_front_ready_fit as reader


def overlay(source,target):
    target.mkdir(exist_ok=False)
    for p in source.rglob('*'):
        q=target/p.relative_to(source)
        if p.is_dir():q.mkdir(exist_ok=True)
        elif p.is_file():q.symlink_to(p.resolve())


def replace(root,name,text):
    p=root/name
    if p.is_symlink():p.unlink()
    p.parent.mkdir(parents=True,exist_ok=True);p.write_text(text)


def row_change(text,title,needle,column,value):
    start=text.index('; '+title);end=text.index('\n\n',start);table=text[start:end];header=None
    for line in table.splitlines():
        fields=[x.strip()for x in line.split(';')[1:-1]]
        if fields and fields[0]in ['Name','Compilation Hierarchy Node']:header=fields;continue
        if not header or len(fields)!=len(header)or needle not in fields[0]:continue
        at=header.index(column);fields[at]=value(fields[at])if callable(value)else value
        replacement='; '+' ; '.join(fields)+' ;'
        assert text.count(line)==1
        return text.replace(line,replacement,1)
    raise AssertionError('Missing damaged-evidence anchor '+needle)


def run(source,out,candidate,baseline,initial,component,initial_map_control=None):
    def check(root):return reader.review(root,candidate,baseline,initial,component,initial_map_control)
    positive=check(source)
    assert positive['status']=='PASS_NATIVE_R_FRONT_READY_PREASSEMBLY_REVIEW'
    out.mkdir(exist_ok=False)
    modifications={}
    hold='core/native59-hold-MIN_fast_1100mv_-40c.rpt'
    text=reader.raw(source/hold).decode()
    modifications['negative_hold']=(hold,re.sub(r'Worst case slack is [-+\d.]+','Worst case slack is -0.100',text,count=1))
    modifications['violated_hold']=(hold,text.replace('(0 violated)','(1 violated)',1))
    fitted=reader.raw(source/'core/output_files/Dorado.fit.rpt').decode('latin-1')
    modifications['processor_ram_type']=('core/output_files/Dorado.fit.rpt',row_change(fitted,'Fitter RAM Summary','altdpram:rm_h_rtl_0','Type','M10K block'))
    modifications['registered_processor_read']=('core/output_files/Dorado.fit.rpt',row_change(fitted,'Fitter RAM Summary','altdpram:rm_h_rtl_0','Port B Input Registers','yes'))
    instruction=a=reader.ram_table(fitted,'Fitter RAM Summary')
    cache_name=next(n for n in instruction if ':b_MemD|'in n and 'snapshot_bank_3|'in n)
    instruction_name=next(n for n in instruction if ':b_ContB|'in n and 'snapshot_bank_3|'in n)
    modifications['cache_old_data']=('core/output_files/Dorado.fit.rpt',row_change(fitted,'Fitter RAM Summary',cache_name,'Mixed Width RDW Mode','Dont care'))
    modifications['instruction_physical_old_data']=('core/output_files/Dorado.fit.rpt',row_change(fitted,'Fitter RAM Summary',instruction_name,'Mixed Width RDW Mode','Dont care'))
    mapping=reader.raw(source/'core/output_files/Dorado.map.rpt').decode()
    modifications['disk_state_count']=('core/output_files/Dorado.map.rpt',row_change(mapping,'Analysis & Synthesis Resource Utilization by Entity','cell_F10000_snapshot:u_f09','Dedicated Logic Registers',lambda v:str(int(re.match(r'\d+',v)[0])+1)+' ('+str(int(re.match(r'\d+',v)[0])+1)+')'))
    central=reader.raw(source/'central-crossings.log').decode()
    modifications['central_coverage']=('central-crossings.log',re.sub(r'^R_FRONT_READY_BOUNDARY [^\n]+\n','',central,count=1,flags=re.M))
    canonical=positive.get('native_visible_R_registers',0)==16
    if canonical:
        name='core/r-front-ready-native-r-mapbuf-hold-MIN_fast_1100mv_-40c.rpt'
        text=reader.raw(source/name).decode()
        modifications['native_R_mapbuf_hold']=(name,re.sub(r'Worst case slack is [-+\d.]+','Worst case slack is -0.100',text,count=1))
        modifications['native_R_register_coverage']=('central-crossings.log',re.sub(r'^R_FRONT_READY_NATIVE_R [^\n]+\n','',central,count=1,flags=re.M))
        modifications['native_R_bit_identity']=('central-crossings.log',re.sub(
            r'(^R_FRONT_READY_R_REGISTER [^\n]+\|q\[)[0-3](\])',r'\g<1>4\2',central,count=1,flags=re.M))
    if positive.get('host_video_register_mapping'):
        text=reader.raw(source/'video-registers.log').decode()
        modifications['video_pipeline_register_coverage']=('video-registers.log',re.sub(
            r'^VIDEO_REGISTER ascal ascal:ascal\|o_dev\[11\]\n','',text,count=1,flags=re.M))
    if positive.get('additional_hold_uncertainty_ps'):
        name='core/r-front-ready-internal-hold-MIN_fast_1100mv_-40c.rpt'
        text=reader.raw(source/name).decode()
        modifications['hold_margin_not_applied']=(name,re.sub(
            r'(^;[^;\n]+;\s*)0\.100(\s*;[^\n]*;\s*clock uncertainty\s*;)',r'\g<1>0.000\2',text,count=1,flags=re.M))
    results=[]
    for name,(member,text)in modifications.items():
        assert text!=reader.raw(source/member).decode('latin-1'if member.endswith('fit.rpt')else'utf-8'),name
        target=out/name;overlay(source,target);replace(target,member,text)
        try:check(target)
        except (AssertionError,FileNotFoundError)as error:results.append(dict(case=name,rejected=True,diagnostic=str(error)))
        else:raise AssertionError('Corrupt native evidence accepted '+name)
    missing=[('missing_existing_hold',hold),('missing_central_hold','core/r-front-ready-internal-hold-MIN_fast_1100mv_-40c.rpt')]
    if canonical:missing.append(('missing_native_R_mapbuf_hold','core/r-front-ready-native-r-mapbuf-hold-MIN_fast_1100mv_-40c.rpt'))
    for name,member in missing:
        target=out/name;overlay(source,target)
        p=target/member
        if not p.exists():p=Path(str(p)+'.gz')
        assert p.is_symlink();p.unlink()
        try:check(target)
        except (AssertionError,FileNotFoundError)as error:results.append(dict(case=name,rejected=True,diagnostic=str(error)))
        else:raise AssertionError('Missing native evidence accepted '+name)
    target=out/'rehashed_constraint';overlay(source,target)
    member='core/native_timing.sdc';text=reader.raw(source/member).decode()+'\nset_false_path -from [all_registers]\n'
    replace(target,member,text)
    record=json.loads(reader.raw(source/'r-front-ready-native.json'));record['source_hashes'][member]=reader.sha(text.encode())
    replace(target,'r-front-ready-native.json',json.dumps(record,indent=2)+'\n')
    try:check(target)
    except AssertionError as error:results.append(dict(case='rehashed_constraint',rejected=True,diagnostic=str(error)))
    else:raise AssertionError('Rehashed timing exception accepted')
    (out/'results.json').write_text(json.dumps(dict(status='PASS_NATIVE_READY_FIT_READER_CONTROLS',positive_status=positive['status'],cases=results),indent=2)+'\n')
    print('Native full-fit evidence:',len(results),'damaged variants rejected')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for name in ['source','output','candidate','baseline','initial','component']:p.add_argument('--'+name,type=Path,required=True)
    p.add_argument('--initial-map-control',type=Path)
    a=p.parse_args();run(a.source.resolve(),a.output.resolve(),a.candidate.resolve(),a.baseline.resolve(),a.initial.resolve(),a.component.resolve(),a.initial_map_control)
