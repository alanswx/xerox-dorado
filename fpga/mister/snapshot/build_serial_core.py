#!/usr/bin/env python3
"""Stage the nineteen qualified boards in a Native59-derived capacity experiment.

The real shared SPI owner/router and all physical board paths are connected.
Foreign-service acknowledgments are explicit VIRTUAL primary inputs, never
tied high or presented as native barriers. No assembly/deployment is run.
Wrapper/raw-memory/input/ARM state and actual pause barriers remain required.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import shutil
import subprocess
import generate_serial_boards as boards
import serial_state_format_msa as fmt
from generate_serial_boards import raw_receipt

ROOT=Path(__file__).resolve().parents[3]
PARENT='/home/alans/mister/Dorado_Native59_20261019'
BP='build/rtl-executive/native16-tx-cable/dorado_native_backplane.v'
MACHINE='build/rtl-executive/native16-tx-cable/dorado_native_machine.sv'
TARGET=Path('fpga/mister/snapshot_serial')
SIGNALS={
 'system_reset':('input',1),'transport_reset':('input',1),'guest_held':('input',1),'services_held':('input',5),
 'hold_update':('input',1),'hold_value':('input',1),'request':('input',1),'write':('input',1),
 'board':('input',4),'index':('input',24),'data':('input',18),
 'hold':('output',1),'scan_abort':('output',1),'command_hold':('output',1),'control_error':('output',1),
 'ready':('output',1),'done':('output',1),'error':('output',3),'result':('output',18),
 'frozen':('output',12),'fault':('output',1),'memory_drained':('output',1),
}
PORTS=''.join(f'    {direction} wire '+(''if width==1 else f'[{width-1}:0] ')+f'snapshot_{name},\n'for name,(direction,width)in SIGNALS.items())
CONNECTIONS=''.join(f'    .snapshot_{name}(snapshot_{name}),\n'for name in SIGNALS)


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def once(s,a,b):
    if s.count(a)!=1:raise ValueError('unreviewed source shape: '+a[:100])
    return s.replace(a,b,1)


def prepare(out,reports):
    out.mkdir(parents=True,exist_ok=False)
    main_root=reports/'2026-10-19-snapshot-serial-board-global'
    catalog_root=reports/'2026-10-19-snapshot-msa-transport'
    original=json.loads(raw_receipt(main_root,'generation-v3/assets.json'));assets={}
    for board in original['boards']:
        for name,digest in board['generated_sources'].items():
            raw=raw_receipt(main_root,'generation-v3/'+name);assert boards.sha(raw)==digest
            assets[name]=raw
    msa_names=['msa_board.sv','cell_MC10141.sv','cell_MC10176.sv','cell_F10016.sv','cell_SN74166.sv',
               'cell_SN74S174.sv','cell_MK4096P_addr.sv','storage_group.sv','ff_scan_controller.sv']
    for name in msa_names:assets['MSA__'+name]=raw_receipt(catalog_root,'simulation-v5/candidate/'+name)
    for name in('board_state_fabric_msa.sv','board_state_uio_msa.sv','msa_state_group.sv'):
        assets[name]=raw_receipt(catalog_root,'simulation-v5/candidate/'+name)
    catalog=json.loads(raw_receipt(catalog_root,'simulation-v5/catalog.json'));fmt.validate_catalog(catalog)
    assert catalog==fmt.build_catalog(reports)
    for name,raw in assets.items():(out/name).write_bytes(raw)
    (out/'catalog.json').write_text(json.dumps(catalog,indent=2)+'\n')
    record=dict(scope=__doc__,main_boards=original['boards'],msa_members=list(fmt.native.INSTANCES),
                board_bits=catalog['board_bits'],schema_id=catalog['schema_id'],catalog_sha256=catalog['catalog_sha256'],
                assets={n:sha(out/n)for n in assets},builder_sha256=sha(Path(__file__)),
                limits='Virtual acknowledged-service contracts, no native machine snapshot/barrier or deployable image')
    (out/'assets.json').write_text(json.dumps(record,indent=2)+'\n')
    print('Prepared qualified19-board sources;',record['board_bits'],'physical bits',flush=True)


def instance(text,module,name):
    match=re.search(r'\b'+re.escape(module)+r'(\s*(?:#\(.*?\))?\s+)'+re.escape(name)+r'\s*\((.*?)\);',text,re.S)
    if not match:raise ValueError('native instance missing: '+name)
    return match


def rewire_backplane(text,record):
    text=once(text,'    input wire contb_advance,','    input wire contb_advance,\n'+PORTS)
    for ident,item in enumerate(record['main_boards']):
        name=item['board'];old='BaseBd_m_Rev_m_Am_nominal'if name=='BaseBd'else name+'_full_snapshot_experiment'
        match=instance(text,old,'b_'+name);normal=re.sub(r'\.state_\w+\([^()]*\)\s*,?', '',match[2])
        assert '.state_'not in normal
        width=fmt.old.SPECS[ident][1]
        values=dict(state_host_clk='storage_clk',state_guest_advance='contb_advance',state_freeze='snapshot_hold',
            state_guest_held='snapshot_guest_held',state_global_hold='snapshot_hold',state_global_abort='snapshot_scan_abort',
            state_system_reset='snapshot_system_reset',state_request=f'snapshot_board_request[{ident}]',state_write='snapshot_write',
            state_index=f'snapshot_index[{width-1}:0]',state_data='snapshot_data',state_frozen=f'snapshot_frozen[{ident}]',
            state_done=f'snapshot_board_done[{ident}]',state_error=f'snapshot_board_error[{ident}]',
            state_result=f'snapshot_board_result[{ident*18}+:18]',state_scan_hold_required=f'snapshot_board_hold[{ident}]',
            state_scan_fault=f'snapshot_board_fault[{ident}]')
        con=''.join(f'    .{p}({v}),\n'for p,v in values.items())
        replacement=item['module']+match[1]+'b_'+name+' (\n'+con+normal+');'
        text=text[:match.start()]+replacement+text[match.end():]
    for unit,name in enumerate(record['msa_members']):
        match=instance(text,'msa_ddram',name)
        values=dict(state_guest_advance='contb_advance',state_freeze='snapshot_hold',state_guest_held='snapshot_guest_held',
            state_global_hold='snapshot_hold',state_global_abort='snapshot_scan_abort',state_system_reset='snapshot_system_reset',
            state_request=f'snapshot_msa_request[{unit}]',state_write='snapshot_msa_write',state_index='snapshot_msa_index',
            state_data='snapshot_msa_data',state_ready='',state_frozen=f'snapshot_msa_frozen[{unit}]',
            state_done=f'snapshot_msa_done[{unit}]',state_error=f'snapshot_msa_error[{unit}]',
            state_scan_hold_required=f'snapshot_msa_hold[{unit}]',state_scan_fault=f'snapshot_msa_fault[{unit}]',
            state_memory_drained=f'snapshot_msa_drained[{unit}]',state_result=f'snapshot_msa_result[{unit*18}+:18]')
        con=''.join(f'    .{p}({v}),\n'for p,v in values.items())
        text=text[:match.start()]+'dorado_serial_MSA'+match[1]+name+' (\n'+con+match[2]+');'+text[match.end():]
    fabric='''
  wire [11:0] snapshot_board_request,snapshot_board_done,snapshot_board_error,snapshot_board_hold,snapshot_board_fault;
  wire [215:0] snapshot_board_result;
  wire [7:0] snapshot_msa_request,snapshot_msa_frozen,snapshot_msa_done,snapshot_msa_error,snapshot_msa_hold,snapshot_msa_fault,snapshot_msa_drained;
  wire [143:0] snapshot_msa_result;wire [23:0] snapshot_msa_index;
  wire snapshot_msa_write;wire [17:0] snapshot_msa_data;
  assign snapshot_memory_drained=&snapshot_msa_drained;
  dorado_board_state_fabric_msa u_snapshot_fabric(
   .clk(storage_clk),.system_reset(snapshot_system_reset),.transport_reset(snapshot_transport_reset),
   .hold_update(snapshot_hold_update),.hold_value(snapshot_hold_value),.local_held(snapshot_guest_held),.services_held(snapshot_services_held),
   .hold(snapshot_hold),.scan_abort(snapshot_scan_abort),.command_hold(snapshot_command_hold),.control_error(snapshot_control_error),
   .request(snapshot_request),.write(snapshot_write),.board(snapshot_board),.index(snapshot_index),.data(snapshot_data),
   .ready(snapshot_ready),.done(snapshot_done),.error(snapshot_error),.result(snapshot_result),.fault(snapshot_fault),
   .board_frozen(snapshot_frozen),.board_done(snapshot_board_done),.board_error(snapshot_board_error),
   .board_hold_required(snapshot_board_hold),.board_fault(snapshot_board_fault),.board_result(snapshot_board_result),.board_request(snapshot_board_request));
  dorado_msa_state_group u_snapshot_msa(
   .clk(storage_clk),.system_reset(snapshot_system_reset),.transport_reset(snapshot_transport_reset),.scan_abort(snapshot_scan_abort),
   .request(snapshot_board_request[11]),.write(snapshot_write),.index(snapshot_index),.data(snapshot_data),
   .unit_frozen(snapshot_msa_frozen),.unit_done(snapshot_msa_done),.unit_error(snapshot_msa_error),
   .unit_hold_required(snapshot_msa_hold),.unit_fault(snapshot_msa_fault),.unit_result(snapshot_msa_result),
   .frozen(snapshot_frozen[11]),.hold_required(snapshot_board_hold[11]),.fault(snapshot_board_fault[11]),
   .done(snapshot_board_done[11]),.error(snapshot_board_error[11]),.result(snapshot_board_result[198+:18]),
   .unit_request(snapshot_msa_request),.unit_index(snapshot_msa_index),.unit_write(snapshot_msa_write),.unit_data(snapshot_msa_data));
'''
    end=text.index(');',text.index('module dorado_native_backplane'))+2
    return text[:end]+fabric+text[end:]


def rewire_wrapper(text):
    text=once(text,'    input wire contb_advance,','    input wire contb_advance,\n'+PORTS)
    return once(text,'  dorado_native_backplane #(.SYSPER(SYSPER)) m (\n',
                     '  dorado_native_backplane #(.SYSPER(SYSPER)) m (\n'+CONNECTIONS)


def rewire_core(text,schema):
    text=once(text,'\t`include "sys/emu_ports.vh"','''    input wire [4:0] CAPACITY_SERVICES_HELD,
    input wire CAPACITY_TRANSPORT_RESET,
    output wire CAPACITY_MEMORY_DRAINED,
\t`include "sys/emu_ports.vh"''')
    declarations='''wire [35:0] ethernet_ext_bus,board_state_ext_bus,dorado_ext_bus;
wire board_state_hold,board_state_scan_abort,board_state_command_hold,board_state_control_error;
wire board_state_guest_held,board_state_hold_update,board_state_hold_value,board_state_request,board_state_write;
wire [3:0] board_state_board;wire [23:0] board_state_index;wire [17:0] board_state_data,board_state_result;
wire board_state_ready,board_state_done,board_state_fault;wire [2:0] board_state_error;wire [11:0] board_state_frozen;
assign ethernet_ext_bus[35:33]=dorado_ext_bus[35:33];assign ethernet_ext_bus[31:16]=dorado_ext_bus[31:16];
assign board_state_ext_bus[35:33]=dorado_ext_bus[35:33];assign board_state_ext_bus[31:16]=dorado_ext_bus[31:16];
assign dorado_ext_bus[32]=ethernet_ext_bus[32]|board_state_ext_bus[32];
assign dorado_ext_bus[15:0]=ethernet_ext_bus[32]?ethernet_ext_bus[15:0]:board_state_ext_bus[32]?board_state_ext_bus[15:0]:16'b0;'''
    text=once(text,'wire [35:0] ethernet_ext_bus;',declarations)
    text=once(text,'.EXT_BUS(ethernet_ext_bus)','.EXT_BUS(dorado_ext_bus)')
    text=once(text,'wire machine_enable = ~reset && pack_ready && storage_ready;',
                   'wire machine_enable = ~reset && pack_ready && storage_ready && !board_state_hold;')
    frontend=f'''// CAPACITY EXPERIMENT ONLY: service acknowledgments are virtual inputs.
// SYSTEM reset is PLL/RBF lifecycle; menu/framework/parser resets cannot thaw state.
assign board_state_guest_held=board_state_hold && !network_step;
dorado_board_state_uio_msa #(.SCHEMA_ID(64'h{schema}),.RESPONSE_LIMIT(4096)) u_board_state_uio(
 .clk(clk_sys),.system_reset(~pll_locked),.transport_reset(CAPACITY_TRANSPORT_RESET),.bus(board_state_ext_bus),
 .hold_update(board_state_hold_update),.hold_value(board_state_hold_value),.hold(board_state_hold),.command_hold(board_state_command_hold),
 .local_held(board_state_guest_held),.services_held(CAPACITY_SERVICES_HELD),.control_error(board_state_control_error),.board_frozen(board_state_frozen),
 .request(board_state_request),.write(board_state_write),.board(board_state_board),.index(board_state_index),.data(board_state_data),
 .ready(board_state_ready),.done(board_state_done),.error(board_state_error),.result(board_state_result),.fault(board_state_fault),
 .busy(),.complete(),.txn_sequence(),.control_errors());
'''
    values={name:'board_state_'+name for name in SIGNALS}
    values.update(system_reset='~pll_locked',transport_reset='CAPACITY_TRANSPORT_RESET',services_held='CAPACITY_SERVICES_HELD',memory_drained='CAPACITY_MEMORY_DRAINED')
    con=''.join(f'    .snapshot_{p}({v}),\n'for p,v in values.items())
    return once(text,'dorado_native_machine #(.SYSPER(MACHINE_SYSPER)) u_native (\n',
                     frontend+'dorado_native_machine #(.SYSPER(MACHINE_SYSPER)) u_native (\n'+con)


CAPACITY_SDC='''# Only the added virtual capacity contract ports; all real clocks/IO unchanged.
set capacity_clock [get_clocks {emu|pll|pll_inst|altera_pll_i|general[0].gpll~PLL_OUTPUT_COUNTER|divclk}]
if {[get_collection_size $capacity_clock]!=1} {error "Expected native 50MHz host clock"}
set capacity_inputs [get_ports {CAPACITY_SERVICES_HELD[*] CAPACITY_TRANSPORT_RESET}]
if {[get_collection_size $capacity_inputs]!=6} {error "Expected six virtual capacity inputs"}
set_input_delay -clock $capacity_clock -max 2.000 $capacity_inputs
set_input_delay -clock $capacity_clock -min 0.000 $capacity_inputs
set_output_delay -clock $capacity_clock -max 2.000 [get_ports CAPACITY_MEMORY_DRAINED]
set_output_delay -clock $capacity_clock -min 0.000 [get_ports CAPACITY_MEMORY_DRAINED]
'''


def stage(baseline,out,assets,receipt):
    record=json.loads((assets/'assets.json').read_text());assert sha(Path(__file__))==record['builder_sha256']
    for name,digest in record['assets'].items():assert sha(assets/name)==digest
    old=json.loads(receipt.read_text());verified={}
    for name,digest in old.items():
        if name.startswith('src/'):
            assert sha(baseline/name)==digest,'frozen Native59 receipt differs: '+name
            verified[name]=digest
    assert len(verified)==405
    out.mkdir(parents=True,exist_ok=False);shutil.copytree(baseline/'src',out/'src')
    shutil.copytree(baseline/'core',out/'core',ignore=shutil.ignore_patterns('db','incremental_db','output_files','*.rpt','*.summary','*.log','c5_pin_model_dump.txt'))
    for p in(out/'core').rglob('*'):
        if p.is_file()and p.suffix in('.qsf','.qip','.sdc','.tcl','.qpf'):
            s=p.read_text()
            for root in(str(baseline),PARENT):s=s.replace(root,str(out))
            p.write_text(s)
    for path,transform in [(out/'src'/BP,lambda s:rewire_backplane(s,record)),(out/'src'/MACHINE,rewire_wrapper),
                           (out/'core/Dorado.sv',lambda s:rewire_core(s,record['schema_id']))]:path.write_text(transform(path.read_text()))
    top=out/'core/sys/sys_top.v';s=top.read_text()
    s=once(s,'module sys_top\n(','module sys_top\n(\n    input wire [4:0] CAPACITY_SERVICES_HELD,\n    input wire CAPACITY_TRANSPORT_RESET,\n    output wire CAPACITY_MEMORY_DRAINED,')
    s=once(s,'emu emu\n(','emu emu\n(\n    .CAPACITY_SERVICES_HELD(CAPACITY_SERVICES_HELD),\n    .CAPACITY_TRANSPORT_RESET(CAPACITY_TRANSPORT_RESET),\n    .CAPACITY_MEMORY_DRAINED(CAPACITY_MEMORY_DRAINED),');top.write_text(s)
    target=out/'src'/TARGET;target.mkdir()
    for name in record['assets']:shutil.copyfile(assets/name,target/name)
    for name in('catalog.json','assets.json'):shutil.copyfile(assets/name,target/name)
    qsf=out/'core/Dorado.qsf';lines=[];removed=[]
    for line in qsf.read_text().splitlines(keepends=True):
        if re.match(r'set_global_assignment -name (?:SYSTEMVERILOG_FILE|VERILOG_FILE) ',line)and '/snapshot_native'in line:
            removed.append(line.strip())
        else:lines.append(line)
    assert len(removed)>70
    s=''.join(lines);s=once(s,'set_global_assignment -name GENERATE_RBF_FILE ON','set_global_assignment -name GENERATE_RBF_FILE OFF')
    for name in record['assets']:
        if name.endswith(('.v','.sv')):s+='\nset_global_assignment -name SYSTEMVERILOG_FILE '+str(target/name)+'\n'
    s+='\nset_global_assignment -name SDC_FILE capacity_timing.sdc\n'
    for port in('CAPACITY_SERVICES_HELD[*]','CAPACITY_TRANSPORT_RESET','CAPACITY_MEMORY_DRAINED'):
        s+='set_instance_assignment -name VIRTUAL_PIN ON -to '+port+'\n'
    qsf.write_text(s);(out/'core/capacity_timing.sdc').write_text(CAPACITY_SDC)
    timing=out/'core/native_timing.sdc';s=timing.read_text()
    s=once(s,'msa_ddram:b_%s','dorado_serial_MSA:b_%s')
    s=once(s,'dorado_storage_group:u_storage_${group}','dorado_serial_MSA__dorado_storage_group_snapshot:u_storage_${group}')
    s=s.replace('cell_MK4096P_addr:u_dram_addr_${group}','dorado_serial_MSA__cell_MK4096P_addr_msa_snapshot:u_dram_addr_${group}')
    s+='''\n# Serial-mode extension of the existing own-source HOLD proof:
# During scan, saved_data shifts only from itself/old_data, never write_data;
# saved_addr shifts only from itself/cache_data, never row/col. Normal inputs
# are deselected by state_hold. Normal mode keeps ready=>!req_valid and the
# original guest-advance exclusion. No new path endpoints or setup cuts.
''';timing.write_text(s)
    for name in('device-corners.tcl','audit-adapter-crossings.tcl'):
        p=out/'core'/name;s=p.read_text().replace('native59-','serial61-')
        if name=='audit-adapter-crossings.tcl':
            start=s.index(' foreach {pattern label} {\n')+len(' foreach {pattern label} {\n')
            end=s.index(' } {\n',start)
            rows=''.join('  {*|'+item['module']+':b_'+item['board']+'|*} '+item['board'].lower()+'\n'for item in record['main_boards'])
            rows+=''.join('  {*|dorado_serial_MSA:'+member+'|*} '+member[2:].lower()+'\n'for member in record['msa_members'])
            s=s[:start]+rows+s[end:]
            s=once(s,'# Reporting only: unchanged SDC, no new timing exceptions.',
                   '# Reporting only: nineteen boards, unchanged clock periods; named serial-mode hold proof and virtual IO are in SDC.')
        p.write_text(s)
    changed=[];added=[]
    for p in(out/'src').rglob('*'):
        if not p.is_file():continue
        name=p.relative_to(out/'src');reference=baseline/'src'/name
        if not reference.exists():added.append(str(name))
        elif reference.read_bytes()!=p.read_bytes():changed.append(str(name))
    assert set(changed)=={BP,MACHINE}
    tracked={str(p.relative_to(out)):sha(p)for p in out.rglob('*')if p.is_file()}
    result=dict(scope=__doc__,baseline=str(baseline),project=str(out),schema_id=record['schema_id'],
        physical_board_bits=record['board_bits'],boards=19,slots=12,main_boards=record['main_boards'],msa_members=record['msa_members'],
        verified_native59_sources=verified,source_changes=changed,added_sources=added,removed_legacy_qsf_assignments=removed,
        source_hashes=tracked,builder_sha256=sha(Path(__file__)),capacity_only=True,assembly_permitted=False,
        real_pause_barriers=False,services='Six virtual input bits; no acknowledgments tied high',
        timing='Original periods/realIO/conditions; renamed same MSA hold exception endpoints with scan exclusion proof; constrained new virtual ports')
    (out/'serial-integration.json').write_text(json.dumps(result,indent=2)+'\n')
    print('Staged19actual boards/12slots; capacity-only virtual contracts, no assembly/deployment',flush=True)


def run(project,quartus):
    record=json.loads((project/'serial-integration.json').read_text());assert record['capacity_only']and not record['assembly_permitted']
    assert sha(Path(__file__))==record['builder_sha256']
    for name,digest in record['source_hashes'].items():assert sha(project/name)==digest,'staged source changed: '+name
    assert not(project/'map.log').exists(),'observe existing job; do not restart'
    for tool,args,label in [('quartus_map',['Dorado'],'map'),('quartus_fit',['Dorado'],'fit'),('quartus_sta',['Dorado'],'sta'),
                            ('quartus_sta',['-t','audit_native_clocks.tcl'],'clocks'),('quartus_sta',['-t','device-corners.tcl'],'corners'),
                            ('quartus_sta',['-t','audit-adapter-crossings.tcl'],'crossings')]:
        print('Serial61',label,flush=True)
        with(project/(label+'.log')).open('w')as log:subprocess.run([str(quartus/tool),*args],cwd=project/'core',stdout=log,stderr=subprocess.STDOUT,check=True)
    print('Capacity/timing run terminal; independent audit required. No assembly.',flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--prepare-assets',type=Path);p.add_argument('--reports',type=Path,default=ROOT/'fpga/mister/reports')
    p.add_argument('--stage',type=Path);p.add_argument('--baseline',type=Path);p.add_argument('--assets',type=Path);p.add_argument('--baseline-receipt',type=Path)
    p.add_argument('--run',type=Path);p.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'));a=p.parse_args()
    if a.prepare_assets:prepare(a.prepare_assets.resolve(),a.reports.resolve())
    elif a.stage and a.baseline and a.assets and a.baseline_receipt:stage(a.baseline.resolve(),a.stage.resolve(),a.assets.resolve(),a.baseline_receipt.resolve())
    elif a.run:run(a.run.resolve(),a.quartus)
    else:p.error('Specify preparation, staging with frozen receipt, or run')
