#!/usr/bin/env python3
"""Review native Ready/front synthesis memory without inferring fitted timing.

Compare every unchanged RAM row and inferred memory parameter table against
the qualified MC1668 native build. Require exactly the intended 12-to-4 bank
replacement and the qualified asynchronous 256x9 MLAB contract. Full timing,
physical placement, firmware and speed qualification remain separate gates.
"""
import argparse
from collections import Counter
import gzip
import hashlib
import json
from pathlib import Path
import re

ROOT=Path(__file__).resolve().parents[1]
BASELINE=ROOT/'fpga/mister/reports/2026-10-20-mc1668-native-build'
BANKS={'rm_h','stk_h','rm_l','stk_l'}
PARAMETERS={'WIDTH':'9','WIDTHAD':'8','NUMWORDS':'256',
    'INDATA_REG':'INCLOCK','WRADDRESS_REG':'INCLOCK','WRCONTROL_REG':'INCLOCK',
    'RDADDRESS_REG':'UNREGISTERED','RDCONTROL_REG':'UNREGISTERED',
    'OUTDATA_REG':'UNREGISTERED','RAM_BLOCK_TYPE':'LUTRAM',
    'READ_DURING_WRITE_MODE_MIXED_PORTS':'DONT_CARE','FILE':'UNUSED'}


def sha(raw):return hashlib.sha256(raw).hexdigest()


def raw(path):
    if path.exists():return path.read_bytes()
    return gzip.decompress(Path(str(path)+'.gz').read_bytes())


def ram_table(text,title):
    needle='; '+title;assert needle in text,('missing RAM table',title)
    table=text[text.index(needle):].split('\n\n',1)[0];header=None;rows={}
    for line in table.splitlines():
        f=[v.strip()for v in line.split(';')[1:-1]]
        if f and f[0]=='Name':header=f
        elif header and len(f)==len(header):
            assert f[0]not in rows,('duplicate RAM',f[0])
            rows[f[0]]=dict(zip(header,f))
    assert rows,('empty RAM table',title)
    return rows


def memory_parameters(text):
    headers=list(re.finditer(r'(?m)^;\s*Parameter Settings for (?:User|Inferred) Entity Instance:\s*([^\n;]+)',text))
    rows={}
    for i,m in enumerate(headers):
        node=m[1].strip()
        if '|altdpram:'not in node and '|altsyncram:'not in node:continue
        section=text[m.end():headers[i+1].start()if i+1<len(headers)else len(text)].split('\n\n',1)[0]
        fields={}
        for line in section.splitlines():
            f=[x.strip()for x in line.split(';')[1:-1]]
            if len(f)>=2 and f[0]not in ['', 'Parameter Name']:
                assert f[0]not in fields,('duplicate memory parameter',node,f[0])
                fields[f[0]]=f[1:]
        assert node not in rows and len(fields)>10,('missing/duplicate parameter table',node)
        rows[node]=fields
    assert rows,'No native inferred memory parameters'
    return rows


def selected_old(name):
    return bool(re.search(r'\|Proc[HL]_full_snapshot_experiment:b_Proc[HL]\|cell_(?:MB7071H|F10414)_snapshot:u_(?:h06|i06|j06|k06|i08|j08)\|',name))


def bank_name(name):
    m=re.search(r'\|dorado_r_front_ready_step:r_front_evaluator\|dorado_r_front_step:child\|dorado_r_memory_step:child\|dorado_r_file_ports:files\|altdpram:(rm_h|stk_h|rm_l|stk_l)_rtl_0(?:\||$)',name)
    return m[1]if m else None


def synthesis_memory(mapping,baseline):
    before=ram_table(baseline,'Analysis & Synthesis RAM Summary')
    after=ram_table(mapping,'Analysis & Synthesis RAM Summary')
    removed={n for n in before if selected_old(n)}
    added={n for n in after if bank_name(n)}
    assert len(before)==170 and len(after)==162 and len(removed)==12 and len(added)==4
    assert set(before)-set(after)==removed and set(after)-set(before)==added,'unintended memory addition/removal'
    same=set(before)&set(after)
    assert len(same)==158
    for n in same:assert before[n]==after[n],('unchanged native RAM differs',n,before[n],after[n])
    seen=set()
    for n in added:
        name=bank_name(n);assert name not in seen;seen.add(name)
        assert {k:v for k,v in after[n].items()if k!='Name'}=={
            'Type':'MLAB','Mode':'Simple Dual Port','Port A Depth':'256','Port A Width':'9',
            'Port B Depth':'256','Port B Width':'9','Size':'2304','MIF':'None'},('new RAM geometry',n,after[n])
    assert seen==BANKS
    old_params=memory_parameters(baseline);new_params=memory_parameters(mapping)
    gone={n for n in old_params if selected_old(n)}
    new={n for n in new_params if bank_name(n)}
    assert len(gone)==12 and len(new)==4
    assert set(old_params)-set(new_params)==gone and set(new_params)-set(old_params)==new,'unexpected inferred RAM parameter coverage'
    common=set(old_params)&set(new_params);assert len(common)==153
    for n in common:assert old_params[n]==new_params[n],('unchanged native memory parameters differ',n)
    for n in new:
        for k,v in PARAMETERS.items():assert new_params[n].get(k,[None])[0]==v,('new RAM parameter',n,k,new_params[n].get(k),v)
    totals={}
    for key,expected in [('Total MLAB memory bits',38912),('Total block memory bits',2506566)]:
        values=[int(x.replace(',',''))for x in re.findall(r';\s*'+re.escape(key)+r'\s*;\s*([\d,]+)\s*;',mapping)]
        previous=[int(x.replace(',',''))for x in re.findall(r';\s*'+re.escape(key)+r'\s*;\s*([\d,]+)\s*;',baseline)]
        assert values and previous and set(values)==set(previous)=={expected},('native memory total differs',key,values,previous)
        totals[key]=expected
    return dict(status='PASS_NATIVE_SYNTHESIS_MEMORY_REVIEW',unchanged_RAM_rows=len(same),
        unchanged_inferred_parameter_tables=len(common),removed_physical_banks=12,new_banks=sorted(seen),
        new_bits=9216,new_parameters=PARAMETERS,memory_totals=totals,
        new_rows={bank_name(n):after[n]for n in sorted(added)},
        fitted_memory_qualified=False,timing_qualified=False,hardware_speedup_claimed=False)


def negative_controls(mapping,baseline):
    lines=mapping.splitlines(keepends=True)
    index=next(i for i,l in enumerate(lines)if bank_name(l)and '; MLAB 'in l)
    samples={}
    for name,a,b in [('wrong_type','; MLAB ','; M10K '),('wrong_depth','; 256 ','; 128 '),('extra_memory','; None ','; injected.mif ')]:
        altered=list(lines);assert a in altered[index];altered[index]=altered[index].replace(a,b,1)
        samples[name]=''.join(altered)
    node=next(n for n in memory_parameters(mapping)if bank_name(n))
    start=re.search(r'(?m)^; Parameter Settings for Inferred Entity Instance: '+re.escape(node)+r' ;$',mapping).start()
    for name,key,value in [('registered_read','RDADDRESS_REG','OUTCLOCK'),('new_rdw','READ_DURING_WRITE_MODE_MIXED_PORTS','OLD_DATA')]:
        tail=mapping[start:];pattern=r'(;\s*'+key+r'\s*;\s*)\w+'
        replaced,n=re.subn(pattern,lambda m:m[1]+value,tail,count=1);assert n==1
        samples[name]=mapping[:start]+replaced
    # An unrelated instruction-bank collision guarantee must also be retained.
    old_node=next(n for n in memory_parameters(mapping)if ':b_ContB|'in n and '|altsyncram:'in n)
    start=re.search(r'(?m)^; Parameter Settings for User Entity Instance: '+re.escape(old_node)+r' ;$',mapping).start()
    tail,n=re.subn(r'(;\s*READ_DURING_WRITE_MODE_MIXED_PORTS\s*;\s*)OLD_DATA',r'\g<1>DONT_CARE',mapping[start:],count=1)
    assert n==1;samples['instruction_old_data']=mapping[:start]+tail
    results=[]
    for name,text in samples.items():
        assert text!=mapping
        try:synthesis_memory(text,baseline)
        except AssertionError as error:results.append(dict(case=name,rejected=True,diagnostic=str(error)))
        else:raise AssertionError('Invalid synthesis memory report accepted: '+name)
    return results


def synthesis_warnings(text,before,candidate,component):
    assert 'Analysis & Synthesis was successful'in text and 'Critical Warning'not in text
    record=json.loads(raw(candidate/'r-front-ready-native.json'))
    proof=json.loads(raw(component/'results.json'))
    assert proof['status']=='PASS_R_FRONT_READY_ARRIVAL_CONTRACT'and len(proof['profiles'])==6
    expected=[];evidence={}
    for name,code,statement in [
        ('dorado_r_front_ready_step.sv','10275','   state_next[751:736],clock_events[58:57],,,);'),
        ('dorado_t_parity_step.sv','10027',"   if(clock_events[13]) state_next[535:532]={pins_after[143],state_q[404+4*old_h_address],pins_after[145],1'b0};")]:
        rel='verilog/rtl/'+name;raw_source=raw(candidate/'src'/rel)
        assert sha(raw_source)==proof['inputs'][rel],('warning source differs from qualified component',name)
        assert sha(raw(component/'executed-inputs'/rel))==proof['inputs'][rel]
        lines=raw_source.decode().splitlines();assert lines.count(statement)==1
        line=lines.index(statement)+1
        location=f"{record['project']}/src/{rel}"
        if code=='10275':
            diagnostic=f'Warning (10275): Verilog HDL Module Instantiation warning at {name}({line}): ignored dangling comma in List of Port Connections File: {location} Line: {line}'
            helper=raw(candidate/'src/verilog/rtl/dorado_task_ready_step.sv')
            assert sha(helper)==proof['inputs']['verilog/rtl/dorado_task_ready_step.sv']
            assert b'output wire [1:0] clock_events,clocks_after,'in helper
            assert b'output reg [14:0] is_next_n,ready_after'in helper
            evidence[code]=dict(diagnostic=diagnostic,source_sha256=sha(raw_source),
                reason='Eight-port helper: first five ports connect state/data/event, final three observational outputs are unused. Dangling comma cannot drive capture data; exact source passed both simulators/six profiles and actual native Initial.')
        else:
            diagnostic=f'Warning (10027): Verilog HDL or VHDL warning at the {name}({line}): index expression is not wide enough to address all of the elements in the array File: {location} Line: {line}'
            source=raw_source.decode()
            assert 'input wire [539:0] state_q'in source
            assert 'wire [3:0] old_h_address='in source
            assert source.count('state_q[404+4*old_h_address]')==1
            indices=[404+4*a for a in range(16)]
            assert min(indices)==404 and max(indices)==464 and max(indices)<512 and max(indices)<540
            evidence[code]=dict(diagnostic=diagnostic,source_sha256=sha(raw_source),
                selected_indices=indices,minimum_index_bits=9,array_index_bits_for_all_elements=10,
                reason='Four-bit address selects only404..464 of540 bits. Nine bits cover every reachable index; bit512..539 cannot be selected by this expression. No truncation/out-of-bounds access.')
        expected.append(diagnostic)
    # Existing warnings move with project roots and edited board line numbers.
    # Preserve the baseline reader's category-count contract for those; pin
    # BOTH new warning categories to exact diagnostics and qualified sources.
    increase=Counter(re.findall(r'Warning \((\d+)\)',text))-Counter(re.findall(r'Warning \((\d+)\)',before))
    assert not(set(increase)-{'10036','10027','10275'}),('unreviewed warning increase',increase)
    assert increase['10027']==increase['10275']==1
    selected=lambda s:Counter(l for l in s.splitlines()if re.match(r'Warning \((10027|10275)\)',l))
    assert not selected(before),'Unexpected reviewed warning in baseline'
    assert selected(text)==Counter(expected),'Unreviewed diagnostic in the two new warning categories'
    return dict(increases=dict(increase),reviewed=evidence,
        comparison_scope='Existing categories cannot increase except unused-object10036; both new categories pin exact diagnostics/qualified sources. Existing per-message text changes with board line numbers/project paths.')


def run(report,baseline,out,log,candidate,component):
    current=raw(report);previous=raw(baseline)
    result=synthesis_memory(current.decode(),previous.decode())
    result.update(scope=__doc__,mapping_sha256=sha(current),baseline_mapping_sha256=sha(previous),
                  negative_controls=negative_controls(current.decode(),previous.decode()))
    if log:
        assert candidate and component,'Warning review needs exact candidate and qualified component sources'
        result['warnings']=synthesis_warnings(raw(log).decode(),raw(BASELINE/'map.log').decode(),candidate,component)
        result['map_log_sha256']=sha(raw(log))
    out.write_text(json.dumps(result,indent=2)+'\n')
    print(result['status'],result['unchanged_RAM_rows'],'unchanged RAM rows;',len(result['negative_controls']),'damaged reports rejected')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--mapping',type=Path,required=True)
    p.add_argument('--baseline',type=Path,default=BASELINE/'core/output_files/Dorado.map.rpt')
    p.add_argument('--map-log',type=Path)
    p.add_argument('--candidate',type=Path)
    p.add_argument('--component',type=Path)
    p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();run(a.mapping,a.baseline,a.output,a.map_log,a.candidate,a.component)
