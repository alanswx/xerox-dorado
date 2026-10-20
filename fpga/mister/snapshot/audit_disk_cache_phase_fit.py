#!/usr/bin/env python3
"""Independent matched disk source/fixture/capacity and all-corner timing review.

Check actual selected bytes, every original medium port, dedicated clock gate,
50MHz and full I/O budgets. Candidate timing must pass all four conditions.
Original timing failures, if any, are recorded. No new timing exceptions.
"""
import argparse
import ast
import copy
import hashlib
import json
from pathlib import Path
import re
import shlex
import audit_disk_cache_phase as source
from audit_enabled_core import ports,compact
from audit_serial_core import header,erase_instance
from audit_storage_factored_fit import CORNERS
from audit_serial_chipwidth_timing import clocks
from generate_serial_boards import ROOT,raw_receipt

INPUTS={n:1 for n in 'clk_sys machine_clk reset attached readonly remounted cyltag_n headtag_n drivetag_n conttag_n selected data_adv write_data_p write_data_m sd_ack sd_buff_wr'.split()}
INPUTS.update(tagbus_n=12,pulse=16,sd_buff_addr=13,sd_buff_dout=16)
OUTPUTS={n:1 for n in 'data_bit write_gate endofcyl_n ready dirty fault sd_rd sd_wr'.split()}
OUTPUTS.update(host_phase=3,fault_debug=32,fault_extra=32,sd_lba=32,sd_blk_cnt=6,sd_buff_din=16,reads=32,writes=32)
SDC='''create_clock -name host_clock -period 20.000 [get_ports clk_sys]
derive_clock_uncertainty
set_input_delay -clock host_clock -max 2.000 [remove_from_collection [all_inputs] [get_ports clk_sys]]
set_input_delay -clock host_clock -min 0.000 [remove_from_collection [all_inputs] [get_ports clk_sys]]
set_output_delay -clock host_clock -max 2.000 [all_outputs]
set_output_delay -clock host_clock -min 0.000 [all_outputs]
'''


def sha(raw):return hashlib.sha256(raw).hexdigest()


def fixture(top,qsf,sdc):
    assert sdc==SDC,'unapproved clock/delay/exception'
    connections=ports(top,'u_medium')
    expected={n:('guest_clock'if n=='machine_clk'else'clk_sys'if n=='clk_sys'else n+('_launch'if n in INPUTS else'_raw'))for n in INPUTS|OUTPUTS}
    assert len(connections)==36 and dict(connections)==expected,'original medium port mapping differs'
    assert header(top,'u_medium')==('dorado_pack_hps','#(.LABEL_WORDS(10),.HOST_CLOCK_HZ(50000000))'),'native medium parameters differ'
    assert header(top,'machine_clock')==('altclkctrl','#(.clock_type("GlobalClock"),.ena_register_mode("fallingedge"),.intended_device_family("CycloneV"))'),'dedicated clock contract differs'
    assert dict(ports(top,'machine_clock'))=={'inclk':"{3'b000,clk_sys}",'clkselect':"2'b00",'ena':'!reset_launch&&ready_raw&&extra_ready_launch','outclk':'guest_clock'},'clock bindings differ'
    bare=erase_instance(erase_instance(top,'dorado_pack_hps','u_medium'),'altclkctrl','machine_clock')
    declarations=re.findall(r'(input|output)\s+wire\s*(\[\d+:0\])?\s*(\w+)',bare)
    actual={name:(direction,int(width[1:].split(':')[0])+1 if width else 1)for direction,width,name in declarations}
    wanted={n:('input',w)for n,w in INPUTS.items()if n!='machine_clk'}|{n:('output',w)for n,w in OUTPUTS.items()}|{'extra_ready':('input',1)}
    assert len(declarations)==36 and actual==wanted,'fixture IO geometry differs'
    bare=re.sub(r'\b(?:input|output)\s+wire\s*(?:\[\d+:0\])?\s*\w+','',bare)
    bare=compact(bare)
    for name,(direction,width)in wanted.items():
        if name=='clk_sys':continue
        shape=f'[{width-1}:0]'if width>1 else''
        if direction=='input':
            allowed=f'reg{shape}{name}_stage,{name}_launch;always@(negedgeclk_sys){name}_stage<={name};always@(posedgeclk_sys){name}_launch<={name}_stage;'
        else:
            allowed=f'wire{shape}{name}_raw;reg{shape}{name}_capture;always@(posedgeclk_sys){name}_capture<={name}_raw;assign{name}={name}_capture;'
        assert bare.count(allowed)==1,'registered fixture binding differs: '+name
        bare=bare.replace(allowed,'',1)
    assert compact(bare)==('modulesnapshot_fit('+','*35+');wireguest_clock;endmodule'),'extra fixture logic or state'
    globals={};virtual=[];other=[];sources=[]
    for line in qsf.splitlines():
        row=shlex.split(line,comments=True)
        if not row:continue
        if row[:3]==['set_global_assignment','-name','LAST_QUARTUS_VERSION']:
            assert row[3]=='17.0.2 Lite Edition';continue
        if row[:1]==['set_global_assignment']:
            if row[2]=='SYSTEMVERILOG_FILE':
                assert len(row)==4;sources.append(row[3]);continue
            assert len(row)==4 and row[1]=='-name'and row[2]not in globals,'duplicate/invalid global policy'
            globals[row[2]]=row[3]
        elif row[:4]==['set_instance_assignment','-name','VIRTUAL_PIN','ON']:
            assert row[4]=='-to'and len(row)==6;virtual.append(row[5])
        else:other.append(row)
    assert globals=={'FAMILY':'Cyclone V','DEVICE':'5CSEBA6U23I7','TOP_LEVEL_ENTITY':'snapshot_fit','MIN_CORE_JUNCTION_TEMP':'-40',
                     'MAX_CORE_JUNCTION_TEMP':'100','TIMEQUEST_MULTICORNER_ANALYSIS':'OFF','OPTIMIZATION_MODE':'HIGH PERFORMANCE EFFORT',
                     'OPTIMIZE_HOLD_TIMING':'ALL_PATHS','NUM_PARALLEL_PROCESSORS':'4','GENERATE_RBF_FILE':'OFF',
                     'SDC_FILE':'bank.sdc'},'fitter/source policy differs'
    assert sources==['top.sv','medium.sv'],'selected sources differ'
    assert other==[['set_location_assignment','PIN_V11','-to','clk_sys'],['set_instance_assignment','-name','IO_STANDARD','3.3-V LVTTL','-to','clk_sys']],'physical pin policy differs'
    expected_virtual={n+('[*]'if w>1 else'')for n,(_,w)in wanted.items()if n!='clk_sys'}
    assert len(virtual)==len(expected_virtual)and set(virtual)==expected_virtual,'missing/duplicate virtual IO'


def timing(read,label):
    records=[];clock_rows=[]
    for corner in CORNERS:
        for kind in ('setup','hold','mpw'):
            text=read(f'{label}/bank-{kind}-{corner}.rpt')
            m=re.search(r'Found (\d+) (?:\w+ )*\((\d+) violated\)\.\s+Worst case slack is\s+([-+\d.]+)',text)
            assert m and int(m[1])>0,'missing timing report'
            records.append(dict(corner=corner,kind=kind,paths=int(m[1]),violations=int(m[2]),slack_ns=float(m[3])))
        for kind in ('recovery','removal'):assert 'Nothing to report.'in read(f'{label}/bank-{kind}-{corner}.rpt'),'unreviewed recovery/removal'
        text=read(f'{label}/bank-ucp-{corner}.rpt')
        for prop in ('Illegal Clocks','Unconstrained Clocks','Unconstrained Input Ports','Unconstrained Input Port Paths','Unconstrained Output Ports','Unconstrained Output Port Paths'):
            m=re.search(r';\s*'+prop+r'\s*;\s*(\d+)\s*;\s*(\d+)\s*;',text)
            assert m and m.groups()==('0','0'),'unconstrained component paths'
        current=clocks(read(f'{label}/bank-clocks-{corner}.rpt'))
        assert len(current)==1 and current[0]['Clock Name']=='host_clock'and current[0]['Type']=='Base'and current[0]['Period']=='20.000','clock period/scope differs'
        clock_rows.append(current)
    return dict(records=records,clocks=clock_rows,minimum_slack_ns=min(t['slack_ns']for t in records),
                violations=[t for t in records if t['violations']or t['slack_ns']<0])


def audit(root,reports,overrides=None):
    overrides=overrides or {};raw=lambda n:overrides[n]if n in overrides else(root/n).read_bytes();read=lambda n:raw(n).decode('latin1')
    record=json.loads(read('provenance.json'))
    assert not record['assembly_permitted']and record['virtual_pin_bits']==265 and record['registered_embedded_io']
    assert 'Matched disk phase terminal;'in read('driver.log'),'run not terminal'
    for n,d in record['compiled_files'].items():
        if not n.endswith('.qsf'):assert sha(raw(n))==d,'prepared input changed: '+n
    assert raw('original/medium.sv')==source.reference(reports),'original medium differs'
    source_review=source.audit(raw('candidate/medium.sv'),reports)
    parent=raw_receipt(reports/'2026-10-19-snapshot-ready-path','tools/fpga/mister/snapshot/fit_f10415.py').decode()
    audit_tcl=next(ast.literal_eval(n.value)for n in ast.parse(parent).body if isinstance(n,ast.Assign)and any(isinstance(t,ast.Name)and t.id=='AUDIT'for t in n.targets))
    results={}
    for label in ('original','candidate'):
        fixture(read(label+'/top.sv'),read(label+'/snapshot_fit.qsf'),read(label+'/bank.sdc'))
        assert read(label+'/audit.tcl')==record['expected_audit']==audit_tcl,'corner collection differs'
        assert 'Fitter Status : Successful'in read(label+'/snapshot_fit.fit.summary')
        assert 'Analysis & Synthesis Status : Successful'in read(label+'/snapshot_fit.map.summary')
        summary=read(label+'/snapshot_fit.fit.summary')
        for prop in ('Device : 5CSEBA6U23I7','Total virtual pins : 265','Total block memory bits : 131,072 /','Total RAM Blocks : 16 /','Total DSP Blocks : 2 /','Total PLLs : 0 /'):assert prop in summary,'resource/fixture coverage differs: '+prop
        m=re.search(r'Logic utilization \(in ALMs\) : ([\d,]+) / 41,910',summary);r=re.search(r'Total registers : (\d+)',summary);assert m and r
        results[label]=dict(alms=int(m[1].replace(',','')),registers=int(r[1]),timing=timing(read,label))
    assert not results['candidate']['timing']['violations'],'candidate timing fails'
    return dict(status='PASS_MATCHED_DISK_PHASE_COMPONENT',scope=__doc__,cases=results,
                additional_alms=results['candidate']['alms']-results['original']['alms'],new_required_wrapper_snapshot_bits=18,
                source_review=source_review,new_timing_exceptions=0,hardware_changed=False,
                limits='Component source/physical timing only; no native integration, complete wrapper snapshot access or speed claim.')


def controls(root,reports):
    cases=[('loose_clock','candidate/bank.sdc','20.000','40.000'),
           ('changed_gate','candidate/top.sv','!reset_launch && ready_raw && extra_ready_launch','!reset_launch && extra_ready_launch'),
           ('aliased_input','candidate/top.sv','.cyltag_n(cyltag_n_launch)','.cyltag_n(headtag_n_launch)'),
           ('removed_buffer','candidate/snapshot_fit.fit.summary','131,072 /','0 /'),
           ('bad_hold','candidate/bank-hold-7_slow_1100mv_100c.rpt','(0 violated)','(1 violated)'),
           ('different_clock','candidate/bank-clocks-7_slow_1100mv_-40c.rpt','20.000','40.000'),
           ('new_exception','candidate/audit.tcl','read_sdc','read_sdc\n set_false_path -from [all_registers]')]
    out=[]
    for name,path,before,after in cases:
        raw=(root/path).read_bytes();assert before.encode()in raw,name
        damaged=raw.replace(before.encode(),after.encode(),1);record=copy.deepcopy(json.loads((root/'provenance.json').read_text()))
        if path in record['compiled_files']:record['compiled_files'][path]=sha(damaged)
        if path.endswith('bank.sdc'):record['expected_sdc']=damaged.decode()
        elif path.endswith('top.sv'):record['expected_top']=damaged.decode()
        elif path.endswith('audit.tcl'):record['expected_audit']=damaged.decode()
        try:audit(root,reports,{path:damaged,'provenance.json':json.dumps(record).encode()})
        except (AssertionError,ValueError)as e:out.append(dict(case=name,rejected=True,reason=str(e),damaged_metadata_rehashed=True))
        else:raise AssertionError('damaged component receipt accepted: '+name)
    return out


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path);p.add_argument('--reports',type=Path,default=ROOT/'fpga/mister/reports');p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    result=audit(a.directory,a.reports.resolve());result['failure_controls']=controls(a.directory,a.reports.resolve())
    a.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
