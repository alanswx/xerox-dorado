#!/usr/bin/env python3
"""Convert the inventoried native guest-clock consumers in a private source set.

Only processes clocked by native.sys_clk receive a synchronous advance guard.
Host-clock processes and original transfer expressions remain byte-identical.
Actual instantiated ancestry routes the enable; variants keep the original
modules available for independent comparisons. Async keyboard/terminal reset
branches remain outside the advance guard. No phase/divider change is made.
This stages an experiment, not a functional, timing or firmware qualification.
"""
import argparse
from collections import defaultdict
import json
from pathlib import Path
import re
import shutil
import xml.etree.ElementTree as ET

from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1]
TOKEN=re.compile(r'//[^\n]*|/\*[\s\S]*?\*/|"(?:\\.|[^"\\])*"|[A-Za-z_$][\w$]*|\S')
READY_AUDIT='''project_open Dorado
create_timing_netlist -model slow
set corners {}
foreach_in_collection op [get_available_operating_conditions] {lappend corners [get_operating_conditions_info $op -name]}
delete_timing_netlist
if {[llength $corners]!=4} {error "Expected four operating conditions"}
foreach name $corners {
 create_timing_netlist
 set_operating_conditions $name
 read_sdc
 update_timing_netlist
 set predicted [get_registers {*|msa_ddram_ce:b_*|ready_q*}]
 set native [get_registers {*|dorado_native_machine_ce:u_native|*}]
 if {[get_collection_size $predicted]<8 || [get_collection_size $native]<1} {error "Missing native ready/common-clock state"}
 puts "NATIVE_READY_COMMON_CLOCK_CORNER $name [get_collection_size $predicted] [get_collection_size $native]"
 foreach_in_collection r $predicted {puts "NATIVE_READY_REGISTER [get_node_info -name $r]"}
 foreach mode {setup hold} {
  report_timing -$mode -to $predicted -npaths 256 -detail full_path -file native-ready-predict-$mode-$name.rpt
  report_timing -$mode -from $predicted -to $native -npaths 256 -detail full_path -file native-ready-gate-$mode-$name.rpt
  report_timing -$mode -from $native -to $predicted -npaths 256 -detail full_path -file native-ready-step-$mode-$name.rpt
 }
 delete_timing_netlist
}
project_close
'''


def tokens(raw):
    return [(m.group(),m.start(),m.end())for m in TOKEN.finditer(raw)if not m.group().startswith(('//','/*'))]


def close(ts,start,left='(',right=')'):
    assert ts[start][0]==left
    depth=0
    for n in range(start,len(ts)):
        if ts[n][0]==left:depth+=1
        if ts[n][0]==right:depth-=1
        if not depth:return n
    raise AssertionError(('unclosed',ts[start]))


def prepare(stage,audit,xml_source,out,project,global_advance=False):
    assert not out.exists()and project.parent==Path('/home/alans/mister')
    parent=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,parent['source_hashes'])
    inventory=json.loads((audit/'results.json').read_text())
    assert inventory['status']=='INVENTORIED_ACTUAL_NATIVE_CLOCK_ENABLE_SINKS'
    assert inventory['native_XML_sha256']==sha(xml_source/'native.xml')
    assert not inventory['ambiguous_clock_port_bindings']
    assert not inventory['process_class_counts'].get('DERIVED_CLOCK_REVIEW_REQUIRED',0)
    assert all(u['domain']=='HOST'for u in inventory['clock_as_data_uses'])
    x=json.loads((xml_source/'run-state.json').read_text());verify(xml_source,x['files'])
    assert x['stage_manifest_sha256']==sha(stage/'r-front-control-native.json')
    xml=ET.parse(xml_source/'native.xml').getroot()
    modules={m.get('name'):m for m in xml.findall('.//module')}
    filenames={f.get('id'):f.get('filename')for f in xml.findall('./files/file')}
    by_path={i['path']:i for i in inventory['instances']}
    root=inventory['native_root_path']
    selected_paths=set()
    for process in inventory['processes']:
        if process['classification']!='VIRTUAL_CLOCK_ENABLE_REQUIRED':continue
        path=process['path']
        while True:
            assert path in by_path,path
            selected_paths.add(path)
            if path==root:break
            path=path.rsplit('.',1)[0]
    selected_types={by_path[p]['original_module']for p in selected_paths}
    assert 'dorado_native_machine'in selected_types and 'chip_6502'in selected_types and 'M6532'in selected_types
    # Every instance of each converted source type must have the same policy.
    assert all(i['path']in selected_paths for i in inventory['instances']if i['original_module']in selected_types)
    source_bytes={n:(stage/n).read_bytes()for n in parent['source_hashes']}
    raw={n:v.decode()for n,v in source_bytes.items()if Path(n).suffix in ['.v','.sv']}
    ts={n:tokens(s)for n,s in raw.items()}
    edits=defaultdict(dict);records=[]
    def add(n,start,end,value,reason):
        key=(start,end)
        if key in edits[n]:assert edits[n][key][0]==value,(n,key,value,edits[n][key])
        else:edits[n][key]=(value,reason)
    def locate(loc):
        f,line,column,*unused=loc.split(',');path=Path(filenames[f])
        name=str(path.relative_to(stage));assert name in raw,name
        text=raw[name];offset=sum(len(v)for v in text.splitlines(keepends=True)[:int(line)-1])+int(column)-1
        return name,offset
    seen_modules=set()
    for module in modules.values():
        original=module.get('origName')
        if original not in selected_types:continue
        n,offset=locate(module.get('loc'));tt=ts[n]
        key=(n,original)
        if key in seen_modules:continue
        seen_modules.add(key)
        starts=[k for k,t in enumerate(tt)if t[0]=='module'and k+1<len(tt)and tt[k+1][0]==original]
        assert len(starts)==1,(n,original,starts)
        k=starts[0]+1;add(n,tt[k][1],tt[k][2],original+'_ce','module variant')
        k+=1
        if tt[k][0]=='#':k=close(tt,k+1)+1
        assert tt[k][0]=='('
        if original!='dorado_native_machine':add(n,tt[k][2],tt[k][2],'\n    input wire guest_advance,','advance input')
    process_locations={}
    for process in inventory['processes']:
        if process['classification']!='VIRTUAL_CLOCK_ENABLE_REQUIRED':continue
        key=process['loc']
        policy=tuple((e['edge'],e['domain'],e['inverted'])for e in process['events'])
        if key in process_locations:assert process_locations[key]==policy;continue
        process_locations[key]=policy
        n,offset=locate(key);tt=ts[n];starts=[k for k,t in enumerate(tt)if t[1]==offset]
        assert len(starts)==1,(n,key,raw[n][offset:offset+50])
        k=starts[0];assert tt[k][0]in ['always','always_ff']
        assert tt[k+1][0]=='@'and tt[k+2][0]=='('
        event_end=close(tt,k+2)
        assert all(e['edge']=='POS'and not e['inverted']for e in process['events']if e['domain']=='VIRTUAL'),process
        enable='contb_advance'if process['original_module']=='dorado_native_machine'else'guest_advance'
        if all(e['domain']=='VIRTUAL'for e in process['events']):
            add(n,tt[event_end][2],tt[event_end][2],' if ('+enable+')','virtual clock process guard')
        else:
            # The only actual mixed sensitivities are positive async reset.
            assert process['original_module']in ['dorado_keyboard','dorado_terminal']
            assert [t[0]for t in tt[k+3:event_end]]==['posedge','sys_clk','or','posedge','reset']
            body=event_end+1
            assert [t[0]for t in tt[body:body+5]]==['begin','if','(','reset',')']
            depth=0;reset_end=None
            for q in range(body+5,len(tt)):
                if tt[q][0]=='begin':depth+=1
                if tt[q][0]=='end':depth-=1
                if depth==0:reset_end=q;break
            assert tt[reset_end+1][0]=='else'and tt[reset_end+2][0]=='begin'
            add(n,tt[reset_end+1][2],tt[reset_end+1][2],' if ('+enable+')','async reset preserved; normal branch guard')
    # Connect every converted child through its real, source-bound instance.
    seen_instances=set()
    for instance in inventory['instances']:
        module=modules[instance['module']]
        for child in module.findall('.//instance'):
            original=modules[child.get('defName')].get('origName')
            if original not in selected_types:continue
            assert instance['path']in selected_paths,(instance,child.attrib)
            n,offset=locate(child.get('loc'));key=(n,offset)
            if key in seen_instances:continue
            seen_instances.add(key);tt=ts[n]
            match=[k for k,t in enumerate(tt)if t[1]==offset]
            assert len(match)==1,(n,child.attrib,raw[n][offset:offset+60])
            k=match[0]
            # Verilator loc identifies the instance name, after type/#params.
            assert tt[k][0]==child.get('origName'),(n,tt[k],child.attrib)
            q=k-1
            if tt[q][0]==')':
                depth=1;q-=1
                while depth:
                    if tt[q][0]==')':depth+=1
                    if tt[q][0]=='(':depth-=1
                    q-=1
                assert tt[q][0]=='#';q-=1
            assert tt[q][0]==original,(n,tt[q],original)
            add(n,tt[q][1],tt[q][2],original+'_ce','child variant')
            assert tt[k+1][0]=='(',(n,tt[k:k+5])
            enable='contb_advance'if instance['original_module']=='dorado_native_machine'else'guest_advance'
            value='.'+'guest_advance('+enable+'),'if tt[k+2][0]=='.'else enable+','
            add(n,tt[k+1][2],tt[k+1][2],value,'child advance connection')
    # The shell now samples readiness on the rising edge with every guest
    # owner, instead of preparing a gated clock on the preceding falling edge.
    for n in ['core/Dorado.sv','src/fpga/mister/Dorado.sv']:
        s=raw[n]
        substitutions=[
          ('reg network_step = 0;\nalways @(negedge clk_sys) network_step <= machine_enable;',
           'wire network_step = machine_enable;'),
          ('always @(posedge machine_clk)if(boot_samples<29\'d240000000)',
           'always @(posedge machine_clk)if(machine_enable && boot_samples<29\'d240000000)'),
          ('dorado_native_machine #(.SYSPER(MACHINE_SYSPER)) u_native (',
           'dorado_native_machine_ce #(.SYSPER(MACHINE_SYSPER)) u_native (')]
        for old,new in substitutions:
            assert s.count(old)==1,(n,old)
            start=s.index(old);add(n,start,start+len(old),new,'shell synchronous machine advance')
        pattern=r'altclkctrl #\(\.clock_type\("Global Clock"\), \.ena_register_mode\("falling edge"\),\n\s*\.intended_device_family\("Cyclone V"\)\) machine_clock\n\s*\(\.inclk\(\{3\'b000, clk_sys\}\), \.clkselect\(2\'b00\), \.ena\(machine_enable\),\n\s*\.outclk\(machine_clk\)\);'
        matches=list(re.finditer(pattern,s));assert len(matches)==1,n
        m=matches[0];add(n,m.start(),m.end(),'assign machine_clk = clk_sys;','common rising-edge clock')
    output=dict(source_bytes)
    for n,changes in edits.items():
        s=raw[n];previous=len(s)+1
        for (start,end),(value,reason)in sorted(changes.items(),reverse=True):
            assert end<=previous,(n,start,end,previous)
            s=s[:start]+value+s[end:];previous=start
            records.append(dict(source=n,start=start,end=end,before=raw[n][start:end],after=value,reason=reason))
        if n not in ['core/Dorado.sv','src/fpga/mister/Dorado.sv']:
            # Keep original declarations as well: the source file can contain
            # unused diagnostic wrappers that still reference those modules.
            # Append only the selected variants, never duplicate helper logic.
            tt=tokens(s);variants=[]
            for k,t in enumerate(tt):
                if t[0]!='module' or k+1>=len(tt):continue
                name=tt[k+1][0]
                if not name.endswith('_ce')or name[:-3]not in selected_types:continue
                end=next(q for q in range(k+2,len(tt))if tt[q][0]=='endmodule')
                variants.append(s[t[1]:tt[end][2]])
            assert variants,n
            original_tokens=ts[n];ends=[]
            for k,t in enumerate(original_tokens):
                if t[0]=='module'and original_tokens[k+1][0]in selected_types:
                    end=next(q for q in range(k+2,len(original_tokens))if original_tokens[q][0]=='endmodule')
                    ends.append(original_tokens[end][2])
            assert ends,n
            # Stay before vendor macro cleanup (`undef NODE_*/NUM_NODES).
            insertion=max(ends)
            s=raw[n][:insertion]+'\n\n'+'\n\n'.join(variants)+'\n'+raw[n][insertion:]
        output[n]=s.encode()
    for n,b in output.items():
        if Path(n).suffix in ['.qsf','.qip','.qpf','.sdc','.tcl']:
            b=b.replace(parent['project'].encode(),str(project).encode())
            if n.endswith(('.sdc','.tcl')):
                # Preserve exactly the old constraints after type renaming.
                for original in sorted(selected_types,key=len,reverse=True):
                    b=b.replace((original+':').encode(),(original+'_ce:').encode())
            output[n]=b
    assert 'Missing native ready/step state'in output['core/audit-storage-ready.tcl'].decode()
    output['core/audit-storage-ready.tcl']=READY_AUDIT.encode()
    if global_advance:
        output['core/Dorado.qsf']+=b'\n# Route the synchronous advance control over a dedicated global network.\nset_instance_assignment -name GLOBAL_SIGNAL ON -to "emu|network_step"\n'
    out.mkdir(parents=True)
    for n,b in output.items():
        p=out/n;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(b)
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    shutil.copy2(ROOT/'tools/build_r_front_ready_native.py',out/'build_r_front_ready_native.py')
    result={**parent,'scope':__doc__,'project':str(project),'parent_stage':str(stage),
        'parent_project':parent['project'],'parent_manifest_sha256':sha(stage/'r-front-control-native.json'),
        'clock_enable_inventory_sha256':sha(audit/'results.json'),'clock_enable_XML_sha256':sha(xml_source/'native.xml'),
        'clock_enable_conversion_staged':True,'clock_enable_conversion_qualified':False,
        'physical_global_advance_assignment':global_advance,
        'physical_control_routing_reference':'https://docs.altera.com/r/docs/683323/18.1/quartus-prime-standard-edition-user-guide-design-recommendations/use-global-clock-network-resources',
        'converted_module_types':sorted(selected_types),'converted_virtual_process_instances':inventory['process_class_counts']['VIRTUAL_CLOCK_ENABLE_REQUIRED'],
        'converted_unique_processes':len(process_locations),'host_clock_processes_unchanged':True,
        'async_reset_branches_unchanged':True,'negative_guest_clock_processes':0,
        'source_hashes':{n:sha(out/n)for n in output},'clock_enable_edits':records,
        'incremental_functional_HDL_changes':sorted(edits),
        'functional_HDL_changes':sorted(set(parent['functional_HDL_changes'])|set(edits)),
        'assembly_permitted':False,'physical_timing_qualified':False,'firmware_qualified':False,
        'hardware_speedup_qualified':False,'full_goal_active':True}
    (out/'r-front-control-native.json').write_text(json.dumps(result,indent=2)+'\n')
    print('STAGED_NATIVE_CLOCK_ENABLE_CONVERSION',len(selected_types),'types',len(process_locations),'source processes',flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['stage','audit','xml-source','output','project']:p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--global-advance',action='store_true')
    a=p.parse_args();prepare(a.stage.resolve(),a.audit.resolve(),a.xml_source.resolve(),a.output.resolve(),a.project,a.global_advance)
