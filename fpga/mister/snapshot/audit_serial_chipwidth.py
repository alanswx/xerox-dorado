#!/usr/bin/env python3
"""Independent exact-source review of chip-width nineteen-board integration.

Use the reviewed Serial63 archive as the immutable parent, not the builder's
expected strings. Only qualified compact assets, router name, schema, metadata
may change. All timing constraints remain exact. Original pins/equations,
clocks/IO, macros and virtual service ownership are otherwise byte-identical.
"""
import argparse
import copy
from functools import lru_cache
import hashlib
import json
from pathlib import Path
import re
import tempfile
from generate_serial_boards import raw_receipt
import serial_state_format_msa_chipwidth as fmt
import audit_msa_chipwidth_transport as transport
from audit_enabled_core import ports
from audit_serial_core import WIDTHS,MEMBERS,header

ROOT=Path(__file__).resolve().parents[3]
PREFIX='src/fpga/mister/snapshot_serial/'
BP='src/build/rtl-executive/native16-tx-cable/dorado_native_backplane.v'
MSA_FILES=['msa_board.sv','cell_MC10141.sv','cell_MC10176.sv','cell_F10016.sv','cell_SN74166.sv',
           'cell_SN74S174.sv','cell_MK4096P_addr.sv','storage_group.sv','ff_scan_controller.sv']


def sha(raw):return hashlib.sha256(raw).hexdigest()


def check(ok,why):
    if not ok:raise ValueError(why)


@lru_cache(maxsize=2)
def parent_inputs(reports):
    parent=reports/'2026-10-19-snapshot-serial-compact'
    area=json.loads(raw_receipt(parent,'project/compact-integration.json'));serial=area
    review=json.loads(raw_receipt(parent,'serial63-terminal-source-audit-v1-20261019.json'))
    check(review['status']=='PASS_COMPACT_NATIVE_SOURCE_ONLY'and len(review['failure_controls'])==23,'unqualified parent')
    files={n:raw_receipt(parent,'project/'+n)for n in area['source_hashes']}
    check(len(files)==659,'parent count differs')
    for n,raw in files.items():check(sha(raw)==area['source_hashes'][n],'parent receipt hash differs: '+n)
    native=area['verified_native59_inputs']
    for n,digest in native.items():check(sha(raw_receipt(reports/'2026-10-19-snapshot-serial-area','baseline-native59/'+n))==digest,'Native59 receipt differs: '+n)
    check(len(native)==413,'native baseline count differs')
    qualified=reports/'2026-10-19-snapshot-msa-chipwidth-transport';replacements={}
    for name in MSA_FILES:replacements[PREFIX+'MSA__'+name]=raw_receipt(qualified,'simulation-v1/candidate/'+name)
    for name in ('board_state_fabric_msa.sv','board_state_uio_msa.sv','msa_state_group.sv'):
        source='msa_state_group_chipwidth.sv'if name=='msa_state_group.sv'else name
        replacements[PREFIX+name]=raw_receipt(qualified,'simulation-v1/candidate/'+source)
    catalog=fmt.build_catalog(reports)
    check(catalog==json.loads(raw_receipt(qualified,'simulation-v1/catalog.json')),'catalog differs from tested bytes')
    manifest=json.loads(raw_receipt(qualified,'simulation-v1/manifest.json'))
    return area,serial,files,native,replacements,catalog,manifest


def qsf_normalize(actual,expected):
    # Quartus17 may save one complete relative alias set for the existing
    # absolute snapshot files. No new or selectively repeated source is allowed.
    lines=actual.splitlines(keepends=True)
    alias=[line for line in lines if re.fullmatch(r'set_global_assignment -name SYSTEMVERILOG_FILE ../src/fpga/mister/snapshot_serial/[^\s]+\s*',line)]
    if alias:
        wanted=re.findall(r'^set_global_assignment -name SYSTEMVERILOG_FILE \S+/src/fpga/mister/snapshot_serial/([^\s]+)\s*$',expected,re.M)
        found=[line.strip().rsplit('/',1)[-1]for line in alias]
        check(sorted(found)==sorted(wanted)and len(found)==len(set(found)),'incomplete/foreign Quartus aliases')
        lines=[line for line in lines if line not in alias]
    result=''.join(lines)
    suffix='set_global_assignment -name LAST_QUARTUS_VERSION "17.0.2 Lite Edition"'
    # Parent may already contain this exact assignment. No other metadata edits.
    if suffix not in expected and suffix in result:
        check(result.count(suffix)==1,'duplicate Quartus version')
        result=result.replace('\n'+suffix+'\n','\n').replace('\n'+suffix,'\n')
    return result


def audit(project,reports,overrides=None):
    overrides=overrides or {}
    def read(n):
        raw=overrides.get(n,(project/n).read_bytes())
        return raw.encode()if isinstance(raw,str)else raw
    record=json.loads(read('chipwidth-integration.json'))
    area,serial,files,native,replacements,catalog,manifest=parent_inputs(reports)
    check((record['boards'],record['slots'],record['physical_board_bits'],record['schema_id'])==(19,12,1957048,catalog['schema_id']),'integration scope/schema differs')
    check(record['capacity_only']and not record['assembly_permitted']and not record['real_pause_barriers'],'capacity scope differs')
    check(record['verified_parent_sources']==area['source_hashes']and record['verified_native59_inputs']==native,'parent/native receipt map differs')
    check(set(record['source_hashes'])==set(files),'selected input inventory differs')
    path=record['project'];oldpath=serial['project'];changed=[];same=0
    for name,before in files.items():
        expected=before
        if Path(name).suffix in ('.qsf','.qpf','.qip','.sdc','.tcl'):expected=before.decode().replace(oldpath,path).encode()
        if name in replacements:expected=replacements[name]
        elif name==BP:
            text=expected.decode();needle='dorado_msa_state_group_compact u_snapshot_msa('
            check(text.count(needle)==1,'parent router missing');expected=text.replace(needle,'dorado_msa_state_group_chipwidth u_snapshot_msa(',1).encode()
        elif name=='core/Dorado.sv':
            needle=".SCHEMA_ID(64'h56e997e5c1fb54d0)";text=expected.decode()
            check(text.count(needle)==1,'parent schema missing');expected=text.replace(needle,f".SCHEMA_ID(64'h{catalog['schema_id']})",1).encode()
        elif name==PREFIX+'catalog.json':
            check(json.loads(read(name))==catalog,'actual catalog differs');continue
        elif name==PREFIX+'assets.json':continue
        actual=read(name)
        if name=='core/Dorado.qsf':
            actual=qsf_normalize(actual.decode(),expected.decode()).encode()
            # Quartus can leave blank lines where it appends its aliases.
            actual=b'\n'.join(line for line in actual.splitlines()if line.strip())
            expected=b'\n'.join(line for line in expected.splitlines()if line.strip())
        check(actual==expected,'unapproved source/policy/pin/constraint edit: '+name)
        if expected!=before and Path(name).suffix not in ('.qsf','.qpf','.qip','.sdc','.tcl')or name in replacements:changed.append(name)
        else:same+=1
    assets=json.loads(read(PREFIX+'assets.json'));oldassets=json.loads(files[PREFIX+'assets.json'])
    for key in ('main_boards','msa_members'):check(assets[key]==oldassets[key],'board/member metadata differs')
    check((assets['board_bits'],assets['schema_id'],assets['catalog_sha256'])==(1957048,catalog['schema_id'],catalog['catalog_sha256']),'asset geometry/schema differs')
    check(set(assets['assets'])==set(oldassets['assets']),'asset inventory differs')
    for n,digest in assets['assets'].items():check(sha(read(PREFIX+n))==digest,'actual asset hash differs: '+n)
    # Recount and compare original pins/parameters directly to Native59 as well
    # as proving transitive byte identity with the reviewed nineteen-board parent.
    native_bp=raw_receipt(reports/'2026-10-19-snapshot-serial-area','baseline-native59/'+BP).decode()
    bp=read(BP).decode();normal_count=0
    for name in ['b_'+n for n,_ in WIDTHS]+MEMBERS:
        normal=lambda pairs:[(p,v)for p,v in pairs if not p.startswith('state_')]
        expected=normal(ports(native_bp,name));actual=normal(ports(bp,name))
        check(actual==expected,'original Native59 pins differ: '+name)
        check(header(bp,name)[1]==header(native_bp,name)[1],'original Native59 parameters differ: '+name)
        normal_count+=len(expected)
    check(normal_count==2761,'normal pin count differs')
    # Directly audit actual files, not their metadata or a builder-generated copy.
    actual={n:read(PREFIX+('MSA__'+n if n in ['msa_board.sv',*manifest['cloned_sources']]else'msa_state_group.sv'if n=='msa_state_group_chipwidth.sv'else n))
            for n in ['msa_board.sv',*manifest['cloned_sources'],'board_state_fabric_msa.sv','board_state_uio_msa.sv','msa_state_group_chipwidth.sv']}
    # Make a namespaced view of actual selected bytes for the component reviewer.
    # This avoids requiring unprefixed helper filenames in the native project.
    with tempfile.TemporaryDirectory(prefix='dorado-compact-source-')as temporary:
        view=Path(temporary)
        for n,raw in actual.items():(view/n).write_bytes(raw)
        transport.audit(view,manifest,reports)
    return dict(status='PASS_CHIPWIDTH_NATIVE_SOURCE_ONLY',scope=__doc__,boards=19,slots=12,physical_board_bits=1957048,
        verified_parent_inputs=659,verified_native59_inputs=413,original_pin_connections=normal_count,
        source_contract='Exact reviewed parent with qualified compact assets, router name, schema; all constraints unchanged',
        schema_id=catalog['schema_id'],sequential_exclusions=[],capacity_only=True,assembly_permitted=False,
        native_barriers_implemented=False,limits='Source proof only; actual selected-source geometry, fit, timing and full snapshots are separate.')


def controls(project,reports):
    cases=[('msa_alias',BP,'.state_request(snapshot_msa_request[7])','.state_request(snapshot_msa_request[0])'),
        ('global_hold',BP,'.state_global_hold(snapshot_hold)',".state_global_hold(1'b0)"),
        ('guest_advance',BP,'.state_guest_advance(contb_advance)',".state_guest_advance(1'b1)"),
        ('router_module',BP,'dorado_msa_state_group_chipwidth u_snapshot_msa(','dorado_msa_state_group_compact u_snapshot_msa('),
        ('service_high','core/Dorado.sv','.services_held(CAPACITY_SERVICES_HELD)',".services_held(5'h1f)"),
        ('schema','core/Dorado.sv',"SCHEMA_ID(64'haf0b647cb43a3a62)","SCHEMA_ID(64'h56e997e5c1fb54d0)"),
        ('reset','core/Dorado.sv','.system_reset(~pll_locked)','.system_reset(reset)'),
        ('normal_keyboard','core/Dorado.sv','.ps2_key(native_key)','.ps2_key(ps2_key)'),
        ('top_ack','core/sys/sys_top.v','.CAPACITY_SERVICES_HELD(CAPACITY_SERVICES_HELD)',".CAPACITY_SERVICES_HELD(5'h1f)"),
        ('clock_macro','core/Dorado.qsf','DORADO_HOST_CLOCK_HZ=50000000','DORADO_HOST_CLOCK_HZ=100000000'),
        ('storage_policy','core/Dorado.qsf','DORADO_STORAGE_INPUT_DELAY=2','DORADO_STORAGE_INPUT_DELAY=0'),
        ('rbf','core/Dorado.qsf','GENERATE_RBF_FILE OFF','GENERATE_RBF_FILE ON'),
        ('area_mode','core/Dorado.qsf','Aggressive Area','HIGH PERFORMANCE EFFORT'),
        ('new_hold_cut','core/native_timing.sdc','# Compact MSA','set_false_path -hold -from [get_registers *]\n# Compact MSA'),
        ('loose_io','core/capacity_timing.sdc','-max 2.000','-max 4.000'),
        ('missing_corner','core/device-corners.tcl','foreach mode {setup hold recovery removal}','foreach mode {setup}'),
        ('old_outer_bound',PREFIX+'board_state_fabric_msa.sv',"index[7:0]<8'd86","index[7:0]<8'd150"),
        ('cross_group',PREFIX+'MSA__msa_board.sv','.addr(dram_addr_0),.addr_will_change(dram_change_0),','.addr(dram_addr_0),.addr_will_change(dram_change_1),'),
        ('guest_pipeline',PREFIX+'MSA__storage_group.sv','read_pipe<=read_value[WIDTH-1:0];','read_pipe<=write_data;'),
        ('source_missing','core/Dorado.qsf','MSA__storage_group.sv','MSA__missing_group.sv')]
    result=[]
    for name,path,before,after in cases:
        raw=(project/path).read_text();check(before in raw,'missing failure-control anchor: '+name)
        try:audit(project,reports,{path:raw.replace(before,after,1)})
        except (ValueError,AssertionError)as error:result.append(dict(case=name,rejected=True,diagnostic=str(error)))
        else:raise AssertionError('damaged compact integration accepted: '+name)
    qsf=(project/'core/Dorado.qsf').read_text()
    qsf=re.sub(r'^set_global_assignment -name SYSTEMVERILOG_FILE ../src/fpga/mister/snapshot_serial/[^\s]+[^\S\n]*\n','',qsf,flags=re.M)
    selected=re.findall(r'^set_global_assignment -name SYSTEMVERILOG_FILE \S+/src/fpga/mister/snapshot_serial/([^\s]+)\s*$',qsf,re.M)
    check(selected and len(selected)==len(set(selected)),'ambiguous source alias fixture')
    aliases=['set_global_assignment -name SYSTEMVERILOG_FILE ../src/fpga/mister/snapshot_serial/'+n+'\n'for n in selected]
    audit(project,reports,{'core/Dorado.qsf':qsf+'\n'+''.join(aliases)})
    for name,extra in [('partial_alias',aliases[:1]),('duplicate_alias',aliases+aliases[:1]),
                       ('foreign_alias',aliases+['set_global_assignment -name SYSTEMVERILOG_FILE ../src/fpga/mister/snapshot_serial/foreign.sv\n'])]:
        try:audit(project,reports,{'core/Dorado.qsf':qsf+'\n'+''.join(extra)})
        except (ValueError,AssertionError)as error:result.append(dict(case=name,rejected=True,diagnostic=str(error)))
        else:raise AssertionError('damaged Quartus source alias accepted: '+name)
    return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('project',type=Path);p.add_argument('--reports',type=Path,default=ROOT/'fpga/mister/reports')
    p.add_argument('--output',type=Path,required=True);p.add_argument('--negative-controls',action='store_true');a=p.parse_args()
    result=audit(a.project.resolve(),a.reports.resolve())
    if a.negative_controls:result['failure_controls']=controls(a.project.resolve(),a.reports.resolve())
    a.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(dict(status=result['status'],boards=19,failure_controls=len(result.get('failure_controls',[])))))
