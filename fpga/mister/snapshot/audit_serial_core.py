#!/usr/bin/env python3
"""Independent nineteen-board serial integration audit against frozen Native59.

Checks every original board pin/parameter, all admin bindings, hierarchy,
virtual capacity contracts, source selection and allowed timing-policy edits.
Not a capacity/timing result, native barrier or complete snapshot claim.
"""
import argparse
from collections import Counter
import copy
import hashlib
import json
from pathlib import Path
import re
import shlex
from audit_enabled_core import ports,compact

BP='src/build/rtl-executive/native16-tx-cable/dorado_native_backplane.v'
MACHINE='src/build/rtl-executive/native16-tx-cable/dorado_native_machine.sv'
WIDTHS=[('ContA',12),('ContB',15),('ProcH',16),('ProcL',16),('IFU',18),('MemC',16),('MemD',20),('MemX',24),('DskEth',13),('DispY',19),('BaseBd',17)]
MEMBERS=['b_msa','b_msaOdd','b_msa1','b_msa1Odd','b_msa2','b_msa2Odd','b_msa3','b_msa3Odd']
SIGNALS=('system_reset','transport_reset','guest_held','services_held','hold_update','hold_value','request','write','board','index','data',
         'hold','scan_abort','command_hold','control_error','ready','done','error','result','frozen','fault','memory_drained')


def check(ok,why):
    if not ok:raise ValueError(why)


def header(text,name):
    # A parameter list may nest parentheses, but cannot cross a preceding
    # instance's terminating semicolon. An unrestricted .*? can start at the
    # enclosing module header and consume multiple instances.
    found=re.search(r'\b(\w+)(\s*(?:#\([^;]*?\))?\s+)'+re.escape(name)+r'\s*\(',text,re.S)
    check(found is not None,'missing instance header: '+name)
    return found[1],compact(found[2])


def erase_instance(text,module,name):
    match=re.search(r'\b'+re.escape(module)+r'\s*(?:#\([^;]*?\))?\s+'+re.escape(name)+r'\s*\(',text,re.S)
    check(match is not None,'missing approved instance: '+name)
    end=text.index(');',match.end())+2
    return text[:match.start()]+text[end:]


def ordinary_inverse(baseline,read):
    strip_ports=lambda s:re.sub(r'^\s*(?:input|output) wire [^\n]*snapshot_[^\n]*\n','',s,flags=re.M)
    strip_state=lambda s:re.sub(r'\.state_\w+\([^()]*\)\s*,?','',s)
    strip_connections=lambda s:re.sub(r'\.snapshot_\w+\([^()]*\)\s*,?','',s)
    bp=read(BP)
    begin=bp.index('  wire [11:0] snapshot_board_request,')
    end=bp.index(');',bp.index('dorado_msa_state_group u_snapshot_msa',begin))+2
    fabric=bp[begin:end]
    for module,name in [('dorado_board_state_fabric_msa','u_snapshot_fabric'),('dorado_msa_state_group','u_snapshot_msa')]:
        check(header(bp,name)==(module,''),'owner/router module differs')
        fabric=erase_instance(fabric,module,name)
    allowed='''wire [11:0] snapshot_board_request,snapshot_board_done,snapshot_board_error,snapshot_board_hold,snapshot_board_fault;
wire [215:0] snapshot_board_result;
wire [7:0] snapshot_msa_request,snapshot_msa_frozen,snapshot_msa_done,snapshot_msa_error,snapshot_msa_hold,snapshot_msa_fault,snapshot_msa_drained;
wire [143:0] snapshot_msa_result;wire [23:0] snapshot_msa_index;
wire snapshot_msa_write;wire [17:0] snapshot_msa_data;
assign snapshot_memory_drained=&snapshot_msa_drained;'''
    check(compact(fabric)==compact(allowed),'unapproved native fabric logic/declaration')
    bp=strip_state(strip_ports(bp[:begin]+bp[end:]))
    for name,_ in WIDTHS:
        old='BaseBd_m_Rev_m_Am_nominal'if name=='BaseBd'else name+'_full_snapshot_experiment'
        bp=re.sub(r'\bdorado_serial_'+name+r'\b',old,bp)
    bp=re.sub(r'\bdorado_serial_MSA\b','msa_ddram',bp)
    check(compact(bp)==compact(strip_state((baseline/BP).read_text())),'ordinary backplane equations differ')
    check(compact(strip_connections(strip_ports(read(MACHINE))))==compact((baseline/MACHINE).read_text()),'ordinary wrapper equations differ')
    core=read('core/Dorado.sv')
    begin=core.index('wire [35:0] ethernet_ext_bus,board_state_ext_bus,dorado_ext_bus;')
    last="assign dorado_ext_bus[15:0]=ethernet_ext_bus[32]?ethernet_ext_bus[15:0]:board_state_ext_bus[32]?board_state_ext_bus[15:0]:16'b0;"
    end=core.index(last,begin)+len(last)
    allowed='''wire [35:0] ethernet_ext_bus,board_state_ext_bus,dorado_ext_bus;
wire board_state_hold,board_state_scan_abort,board_state_command_hold,board_state_control_error;
wire board_state_guest_held,board_state_hold_update,board_state_hold_value,board_state_request,board_state_write;
wire [3:0] board_state_board;wire [23:0] board_state_index;wire [17:0] board_state_data,board_state_result;
wire board_state_ready,board_state_done,board_state_fault;wire [2:0] board_state_error;wire [11:0] board_state_frozen;
assign ethernet_ext_bus[35:33]=dorado_ext_bus[35:33];assign ethernet_ext_bus[31:16]=dorado_ext_bus[31:16];
assign board_state_ext_bus[35:33]=dorado_ext_bus[35:33];assign board_state_ext_bus[31:16]=dorado_ext_bus[31:16];
assign dorado_ext_bus[32]=ethernet_ext_bus[32]|board_state_ext_bus[32];
'''+last
    check(compact(core[begin:end])==compact(allowed),'unapproved SPI ownership/declaration')
    core=core[:begin]+'wire [35:0] ethernet_ext_bus;'+core[end:]
    begin=core.index('// CAPACITY EXPERIMENT ONLY: service acknowledgments are virtual inputs.')
    end=core.index('dorado_native_machine #(.SYSPER(MACHINE_SYSPER)) u_native (',begin)
    frontend=erase_instance(core[begin:end],'dorado_board_state_uio_msa','u_board_state_uio')
    check(compact(frontend)=='assignboard_state_guest_held=board_state_hold&&!network_step;','unapproved UIO frontend logic')
    core=core[:begin]+core[end:]
    core=strip_connections(core).replace('.EXT_BUS(dorado_ext_bus)','.EXT_BUS(ethernet_ext_bus)').replace(' && !board_state_hold','')
    strip_capacity=lambda s:re.sub(r'^\s*(?:input|output) wire [^\n]*CAPACITY_[^\n]*\n','',s,flags=re.M)
    check(compact(strip_capacity(core))==compact((baseline/'core/Dorado.sv').read_text()),'ordinary outer-core equations differ')
    top=strip_capacity(read('core/sys/sys_top.v'))
    top=re.sub(r'\.CAPACITY_\w+\([^()]*\)\s*,?','',top)
    check(compact(top)==compact((baseline/'core/sys/sys_top.v').read_text()),'ordinary framework equations differ')


def audit(baseline,project,overrides=None):
    overrides=overrides or {}
    read=lambda p:overrides.get(p,(project/p).read_text())
    record=json.loads((project/'serial-integration.json').read_text())
    check(record['boards']==19 and record['slots']==12 and record['physical_board_bits']==1966264,'board inventory differs')
    check(record['capacity_only']and not record['assembly_permitted']and not record['real_pause_barriers'],'capacity scope differs')
    for name,digest in record['verified_native59_sources'].items():
        check(hashlib.sha256((baseline/name).read_bytes()).hexdigest()==digest,'baseline receipt differs: '+name)
        if name not in(BP,MACHINE):check((project/name).read_bytes()==(baseline/name).read_bytes(),'unrelated native source differs: '+name)
    assets=json.loads((project/'src/fpga/mister/snapshot_serial/assets.json').read_text())
    for name,digest in assets['assets'].items():check(hashlib.sha256((project/'src/fpga/mister/snapshot_serial'/name).read_bytes()).hexdigest()==digest,'qualified asset differs: '+name)
    bp=read(BP);old_bp=(baseline/BP).read_text();normal_count=0
    for identity,(name,width)in enumerate(WIDTHS):
        inst='b_'+name;actual=ports(bp,inst);old=ports(old_bp,inst)
        normal=lambda pairs:[(p,v)for p,v in pairs if not p.startswith('state_')]
        check(normal(actual)==normal(old),'original pins differ: '+name);normal_count+=len(normal(old))
        check(header(bp,inst)==('dorado_serial_'+name,header(old_bp,inst)[1]),'module/parameter differs: '+name)
        expected=dict(state_host_clk='storage_clk',state_guest_advance='contb_advance',state_freeze='snapshot_hold',
            state_guest_held='snapshot_guest_held',state_global_hold='snapshot_hold',state_global_abort='snapshot_scan_abort',
            state_system_reset='snapshot_system_reset',state_request=f'snapshot_board_request[{identity}]',state_write='snapshot_write',
            state_index=f'snapshot_index[{width-1}:0]',state_data='snapshot_data',state_frozen=f'snapshot_frozen[{identity}]',
            state_done=f'snapshot_board_done[{identity}]',state_error=f'snapshot_board_error[{identity}]',
            state_result=f'snapshot_board_result[{identity*18}+:18]',state_scan_hold_required=f'snapshot_board_hold[{identity}]',state_scan_fault=f'snapshot_board_fault[{identity}]')
        state=[(p,v)for p,v in actual if p.startswith('state_')]
        check(len(state)==len(expected)and dict(state)==expected,'main-board state binding differs: '+name)
    for unit,name in enumerate(MEMBERS):
        actual=ports(bp,name);old=ports(old_bp,name)
        check([(p,v)for p,v in actual if not p.startswith('state_')]==old,'MSA original pins differ: '+name);normal_count+=len(old)
        check(header(bp,name)==('dorado_serial_MSA',header(old_bp,name)[1]),'MSA parameter differs: '+name)
        expected=dict(state_guest_advance='contb_advance',state_freeze='snapshot_hold',state_guest_held='snapshot_guest_held',
            state_global_hold='snapshot_hold',state_global_abort='snapshot_scan_abort',state_system_reset='snapshot_system_reset',
            state_request=f'snapshot_msa_request[{unit}]',state_write='snapshot_msa_write',state_index='snapshot_msa_index',state_data='snapshot_msa_data',state_ready='',
            state_frozen=f'snapshot_msa_frozen[{unit}]',state_done=f'snapshot_msa_done[{unit}]',state_error=f'snapshot_msa_error[{unit}]',
            state_scan_hold_required=f'snapshot_msa_hold[{unit}]',state_scan_fault=f'snapshot_msa_fault[{unit}]',state_memory_drained=f'snapshot_msa_drained[{unit}]',
            state_result=f'snapshot_msa_result[{unit*18}+:18]')
        state=[(p,v)for p,v in actual if p.startswith('state_')];check(len(state)==len(expected)and dict(state)==expected,'MSA state binding differs: '+name)
    fabric=dict(ports(bp,'u_snapshot_fabric'))
    expected={p:'snapshot_'+p for p in('system_reset','transport_reset','hold_update','hold_value','hold','scan_abort','command_hold','control_error','request','write','board','index','data','ready','done','error','result','fault')}
    expected.update(clk='storage_clk',local_held='snapshot_guest_held',services_held='snapshot_services_held',board_frozen='snapshot_frozen',board_done='snapshot_board_done',
        board_error='snapshot_board_error',board_hold_required='snapshot_board_hold',board_fault='snapshot_board_fault',board_result='snapshot_board_result',board_request='snapshot_board_request')
    check(fabric==expected,'owner binding differs')
    group=dict(ports(bp,'u_snapshot_msa'))
    expected=dict(clk='storage_clk',system_reset='snapshot_system_reset',transport_reset='snapshot_transport_reset',scan_abort='snapshot_scan_abort',request='snapshot_board_request[11]',
        write='snapshot_write',index='snapshot_index',data='snapshot_data',unit_frozen='snapshot_msa_frozen',unit_done='snapshot_msa_done',unit_error='snapshot_msa_error',
        unit_hold_required='snapshot_msa_hold',unit_fault='snapshot_msa_fault',unit_result='snapshot_msa_result',frozen='snapshot_frozen[11]',hold_required='snapshot_board_hold[11]',
        fault='snapshot_board_fault[11]',done='snapshot_board_done[11]',error='snapshot_board_error[11]',result='snapshot_board_result[198+:18]',
        unit_request='snapshot_msa_request',unit_index='snapshot_msa_index',unit_write='snapshot_msa_write',unit_data='snapshot_msa_data')
    check(group==expected,'MSA router binding differs')
    check(len(ports(bp,'u_snapshot_fabric'))==len(fabric)and len(ports(bp,'u_snapshot_msa'))==len(group),'duplicate owner/router port')
    for path,instance,prefix in[(MACHINE,'m','snapshot_'),('core/Dorado.sv','u_native','board_state_')]:
        actual=ports(read(path),instance);old=ports((baseline/path).read_text(),instance)
        check([(p,v)for p,v in actual if not p.startswith('snapshot_')]==old,'normal wrapper binding differs: '+instance)
        expected={'snapshot_'+n:prefix+n for n in SIGNALS}
        if instance=='u_native':expected.update(snapshot_system_reset='~pll_locked',snapshot_transport_reset='CAPACITY_TRANSPORT_RESET',snapshot_services_held='CAPACITY_SERVICES_HELD',snapshot_memory_drained='CAPACITY_MEMORY_DRAINED')
        added=[(p,v)for p,v in actual if p.startswith('snapshot_')]
        check(len(added)==len(expected)and dict(added)==expected,'snapshot wrapper binding differs: '+instance)
    core=read('core/Dorado.sv');old=(baseline/'core/Dorado.sv').read_text()
    for name in('u_network_uio','u_network_ddr'):check(ports(core,name)==ports(old,name),'Ethernet binding differs: '+name)
    uio=dict(ports(core,'u_board_state_uio'));check(uio['system_reset']=='~pll_locked'and uio['transport_reset']=='CAPACITY_TRANSPORT_RESET','lifecycle/parser reset conflated')
    check(uio['services_held']=='CAPACITY_SERVICES_HELD','service acknowledgment not virtual input')
    check(header(core,'u_board_state_uio')==('dorado_board_state_uio_msa',"#(.SCHEMA_ID(64'hb80a00977d7e7992),.RESPONSE_LIMIT(4096))"),'UIO schema/deadline differs')
    expected={p:'board_state_'+p for p in ('hold_update','hold_value','hold','command_hold','control_error','request','write','board','index','data','ready','done','error','result','fault')}
    expected.update(clk='clk_sys',system_reset='~pll_locked',transport_reset='CAPACITY_TRANSPORT_RESET',bus='board_state_ext_bus',local_held='board_state_guest_held',
                    services_held='CAPACITY_SERVICES_HELD',board_frozen='board_state_frozen',busy='',complete='',txn_sequence='',control_errors='')
    check(uio==expected and len(ports(core,'u_board_state_uio'))==len(expected),'UIO binding differs')
    ordinary_inverse(baseline,read)
    for required in('wiremachine_enable=~reset&&pack_ready&&storage_ready&&!board_state_hold;',
                     'assignboard_state_guest_held=board_state_hold&&!network_step;',
                     'always@(negedgeclk_sys)network_step<=machine_enable;',
                     'assigndorado_ext_bus[32]=ethernet_ext_bus[32]|board_state_ext_bus[32];',
                     "assigndorado_ext_bus[15:0]=ethernet_ext_bus[32]?ethernet_ext_bus[15:0]:board_state_ext_bus[32]?board_state_ext_bus[15:0]:16'b0;"):
        check(compact(core).count(required)==1,'clock or SPI ownership differs')
    actual=ports(read('core/sys/sys_top.v'),'emu');old=ports((baseline/'core/sys/sys_top.v').read_text(),'emu')
    check([(p,v)for p,v in actual if not p.startswith('CAPACITY_')]==old,'framework emu pins differ')
    check(dict((p,v)for p,v in actual if p.startswith('CAPACITY_'))==dict(CAPACITY_SERVICES_HELD='CAPACITY_SERVICES_HELD',CAPACITY_TRANSPORT_RESET='CAPACITY_TRANSPORT_RESET',CAPACITY_MEMORY_DRAINED='CAPACITY_MEMORY_DRAINED'),'virtual contracts not propagated to top')
    qsf=read('core/Dorado.qsf');check('GENERATE_RBF_FILE OFF'in qsf and 'GENERATE_RBF_FILE ON'not in qsf,'assembly enabled')
    # Retain every original QSF assignment (including all macros and fitter
    # settings), except retired parallel adapters and the explicit RBF switch.
    def assignments(source,root,old=False):
        result=[]
        for line in source.splitlines():
            words=shlex.split(line,comments=True)
            if not words:continue
            words=[w.replace(str(root),'<PROJECT>').replace('/home/alans/mister/Dorado_Native59_20261019','<PROJECT>')for w in words]
            if old and words[:3]in (['set_global_assignment','-name','VERILOG_FILE'],['set_global_assignment','-name','SYSTEMVERILOG_FILE'])and '/snapshot_native'in words[3]:continue
            if not old:
                if words[:3]==['set_global_assignment','-name','SYSTEMVERILOG_FILE']and '/snapshot_serial/'in words[3]:continue
                if words==['set_global_assignment','-name','SDC_FILE','capacity_timing.sdc']:continue
                if words[:4]==['set_instance_assignment','-name','VIRTUAL_PIN','ON']and words[-1].startswith('CAPACITY_'):continue
                if words==['set_global_assignment','-name','GENERATE_RBF_FILE','OFF']:words[-1]='ON'
            result.append(words)
        return result
    check(assignments(qsf,project)==assignments((baseline/'core/Dorado.qsf').read_text(),baseline,True),'unapproved source/macro/fitter assignment differs')
    for name in('CAPACITY_SERVICES_HELD[*]','CAPACITY_TRANSPORT_RESET','CAPACITY_MEMORY_DRAINED'):
        check(qsf.count('set_instance_assignment -name VIRTUAL_PIN ON -to '+name+'\n')==1,'virtual capacity port missing')
    check(not re.search(r'^set_global_assignment -name (?:SYSTEMVERILOG_FILE|VERILOG_FILE) .*?/snapshot_native',qsf,re.M),'legacy snapshot source still selected')
    check(qsf.count('VERILOG_MACRO "DORADO_STORAGE_INPUT_DELAY=2"')==1,'storage-input timing policy differs')
    selected=re.findall(r'^set_global_assignment -name SYSTEMVERILOG_FILE (.+snapshot_serial/[^\n]+)',qsf,re.M)
    wanted={str(project/'src/fpga/mister/snapshot_serial'/n)for n in assets['assets']if n.endswith(('.v','.sv'))}
    counts=Counter(str((project/'core'/n).resolve())for n in selected)
    check(set(counts)==wanted and set(counts.values())in ({1},{2}),'qualified HDL selection missing/duplicate')
    # Quartus17 saves one complete relative alias set for new absolute files.
    # It must add no different source and cannot selectively repeat a file.
    aliases={'../src/fpga/mister/snapshot_serial/'+Path(n).name for n in wanted}
    check(set(selected)==(wanted|aliases if set(counts.values())=={2}else wanted),'unapproved Quartus source alias')
    for name in('Dorado.sdc','sdram_timing.sdc','checkpoint_timing.sdc'):
        original=(baseline/'core'/name).read_text().replace(str(baseline),str(project)).replace('/home/alans/mister/Dorado_Native59_20261019',str(project))
        check(read('core/'+name)==original,'real clock/IO constraint differs: '+name)
    capacity='''# Only the added virtual capacity contract ports; all real clocks/IO unchanged.
set capacity_clock [get_clocks {emu|pll|pll_inst|altera_pll_i|general[0].gpll~PLL_OUTPUT_COUNTER|divclk}]
if {[get_collection_size $capacity_clock]!=1} {error "Expected native 50MHz host clock"}
set capacity_inputs [get_ports {CAPACITY_SERVICES_HELD[*] CAPACITY_TRANSPORT_RESET}]
if {[get_collection_size $capacity_inputs]!=6} {error "Expected six virtual capacity inputs"}
set_input_delay -clock $capacity_clock -max 2.000 $capacity_inputs
set_input_delay -clock $capacity_clock -min 0.000 $capacity_inputs
set_output_delay -clock $capacity_clock -max 2.000 [get_ports CAPACITY_MEMORY_DRAINED]
set_output_delay -clock $capacity_clock -min 0.000 [get_ports CAPACITY_MEMORY_DRAINED]
'''
    check(read('core/capacity_timing.sdc')==capacity,'virtual timing constraints differ')
    timing=read('core/native_timing.sdc');cut=timing.index('\n# Serial-mode extension of the existing own-source HOLD proof:')
    timing=timing[:cut].replace('dorado_serial_MSA:b_%s','msa_ddram:b_%s').replace('dorado_serial_MSA__dorado_storage_group_snapshot:u_storage_${group}','dorado_storage_group:u_storage_${group}')
    timing=timing.replace('dorado_serial_MSA__cell_MK4096P_addr_msa_snapshot:u_dram_addr_${group}','cell_MK4096P_addr:u_dram_addr_${group}')
    check(timing==(baseline/'core/native_timing.sdc').read_text(),'new hold exception or altered endpoint policy')
    rebase=lambda s:s.replace(str(baseline),str(project)).replace('/home/alans/mister/Dorado_Native59_20261019',str(project))
    corners=rebase((baseline/'core/device-corners.tcl').read_text()).replace('native59-','serial61-')
    check(read('core/device-corners.tcl')==corners,'operating condition script differs')
    crossing=read('core/audit-adapter-crossings.tcl')
    begin=crossing.index(' foreach {pattern label} {\n')+len(' foreach {pattern label} {\n');end=crossing.index(' } {\n',begin)
    rows=''.join('  {*|dorado_serial_'+name+':b_'+name+'|*} '+name.lower()+'\n'for name,_ in WIDTHS)
    rows+=''.join('  {*|dorado_serial_MSA:'+name+'|*} '+name[2:].lower()+'\n'for name in MEMBERS)
    check(crossing[begin:end]==rows,'not all nineteen crossings covered')
    original=rebase((baseline/'core/audit-adapter-crossings.tcl').read_text()).replace('native59-','serial61-')
    old_begin=original.index(' foreach {pattern label} {\n')+len(' foreach {pattern label} {\n');old_end=original.index(' } {\n',old_begin)
    crossing=crossing[:begin]+original[old_begin:old_end]+crossing[end:]
    crossing=crossing.replace('# Reporting only: nineteen boards, unchanged clock periods; named serial-mode hold proof and virtual IO are in SDC.',
                              '# Reporting only: unchanged SDC, no new timing exceptions.')
    check(crossing==original,'crossing reporting procedure differs')
    special={'Dorado.sv','Dorado.qsf','sys/sys_top.v','device-corners.tcl','audit-adapter-crossings.tcl',
             'Dorado.sdc','sdram_timing.sdc','checkpoint_timing.sdc','native_timing.sdc'}
    unchanged_core=0
    for p in (baseline/'core').rglob('*'):
        name=p.relative_to(baseline/'core')
        if not p.is_file()or str(name)in special or set(name.parts)&{'db','incremental_db','output_files'}or p.suffix in ('.rpt','.summary','.log')or p.name=='c5_pin_model_dump.txt':continue
        if p.suffix in ('.qsf','.qip','.sdc','.tcl','.qpf'):check(read('core/'+str(name))==rebase(p.read_text()),'unrelated core file differs: '+str(name))
        else:check(overrides.get('core/'+str(name),(project/'core'/name).read_bytes())==p.read_bytes(),'unrelated core file differs: '+str(name))
        unchanged_core+=1
    return dict(status='PASS_SOURCE_ONLY',scope=__doc__,boards=19,slots=12,original_pin_connections=normal_count,
        physical_board_bits=1966264,baseline_sources_verified=len(record['verified_native59_sources']),unchanged_core_files=unchanged_core,capacity_only=True,
        native_barriers_implemented=False,assembly_permitted=False,limits='Source integration only; full elaboration/fit/timing/hardware restore remain separate')


def controls(baseline,project,reviewer=None):
    reviewer=reviewer or audit
    cases=[('index',BP,'snapshot_index[14:0]','snapshot_index[13:0]'),
        ('msa_alias',BP,'.state_request(snapshot_msa_request[7])','.state_request(snapshot_msa_request[0])'),
        ('global_hold',BP,'.state_global_hold(snapshot_hold)',".state_global_hold(1'b0)"),
        ('guest_advance',BP,'.state_guest_advance(contb_advance)',".state_guest_advance(1'b1)"),
        ('processor_param',BP,'dorado_serial_DskEth #(.SYSPER(SYSPER))','dorado_serial_DskEth #(.SYSPER(16))'),
        ('wrapper',MACHINE,'.snapshot_scan_abort(snapshot_scan_abort)',".snapshot_scan_abort(1'b0)"),
        ('reset','core/Dorado.sv','.system_reset(~pll_locked)','.system_reset(reset)'),
        ('service_high','core/Dorado.sv','.services_held(CAPACITY_SERVICES_HELD)',".services_held(5'h1f)"),
        ('normal_keyboard','core/Dorado.sv','.ps2_key(native_key)','.ps2_key(ps2_key)'),
        ('top_ack','core/sys/sys_top.v','.CAPACITY_SERVICES_HELD(CAPACITY_SERVICES_HELD)',".CAPACITY_SERVICES_HELD(5'h1f)"),
        ('rbf','core/Dorado.qsf','GENERATE_RBF_FILE OFF','GENERATE_RBF_FILE ON'),
        ('storage_mode','core/Dorado.qsf','DORADO_STORAGE_INPUT_DELAY=2','DORADO_STORAGE_INPUT_DELAY=0'),
        ('host_clock','core/Dorado.qsf','DORADO_HOST_CLOCK_HZ=50000000','DORADO_HOST_CLOCK_HZ=100000000'),
        ('uio_schema','core/Dorado.sv',"SCHEMA_ID(64'hb80a00977d7e7992)","SCHEMA_ID(64'hb80a00977d7e7993)"),
        ('uio_deadline','core/Dorado.sv','RESPONSE_LIMIT(4096)','RESPONSE_LIMIT(2048)'),
        ('backplane_equation',BP,'endmodule',"assign contb_advance=1'b1;\nendmodule"),
        ('fabric_extra',BP,'  assign snapshot_memory_drained=',"  assign contb_advance=1'b1;\n  assign snapshot_memory_drained="),
        ('wrapper_equation',MACHINE,'endmodule',"assign sys_clk=1'b0;\nendmodule"),
        ('core_equation','core/Dorado.sv','endmodule',"assign clk_sys=1'b0;\nendmodule"),
        ('virtual_false_path','core/capacity_timing.sdc','set_input_delay -clock','set_false_path -from [get_registers *]\nset_input_delay -clock'),
        ('crossing_missing','core/audit-adapter-crossings.tcl','dorado_serial_MSA:b_msa3Odd','dorado_serial_MSA:b_msa2Odd'),
        ('corner_missing','core/device-corners.tcl','foreach mode {setup hold recovery removal}','foreach mode {setup}'),
        ('hold_cut','core/native_timing.sdc','set_false_path -hold -from $data_source','set_false_path -setup -from $data_source')]
    results=[]
    for name,path,before,after in cases:
        source=(project/path).read_text();check(before in source,'missing negative-control target: '+name)
        try:reviewer(baseline,project,{path:source.replace(before,after,1)})
        except ValueError as error:results.append(dict(case=name,rejected=True,diagnostic=str(error)))
        else:raise AssertionError('damaged native integration accepted: '+name)
    return results


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('baseline',type=Path);p.add_argument('project',type=Path);p.add_argument('--output',type=Path,required=True);p.add_argument('--negative-controls',action='store_true');a=p.parse_args()
    result=audit(a.baseline.resolve(),a.project.resolve())
    if a.negative_controls:result['failure_controls']=controls(a.baseline.resolve(),a.project.resolve())
    a.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(dict(status=result['status'],boards=19,pins=result['original_pin_connections'],failure_controls=len(result.get('failure_controls',[])))))
