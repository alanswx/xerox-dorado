#!/usr/bin/env python3
"""Stage the frozen corrected native read caller for a full-core15ns budget.

This is map/fit/timing only: keep SYSPER4, original ROM and all devices, and
do not assemble/load. The PLL and existing host watchdog frequency become
66.666667MHz; shared SDRAM consequently runs at that clock too and remains
unqualified. Nominal ordinary instructions are120ns in this diagnostic, not
the future60ns event-batch implementation. No timing exceptions are added.
"""
import argparse
import gzip
import json
from pathlib import Path
import re
import shutil

from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha
from fit_r_front import quartus_cone

ROOT=Path(__file__).resolve().parents[1]
CONFIG={'.qsf','.qip','.qpf','.sdc','.tcl'}


def prepare(native,out,project,lower=False):
    assert not out.exists()and project.is_absolute()
    assert project.parent==Path('/home/alans/mister')
    p=json.loads(gzip.decompress((native/'provenance.json.gz').read_bytes()));verify(native/'inputs',p['inputs'])
    assert p['experimental_j19_ordered_epochs']and not p['paired_mode_enabled']
    assert p['actual_four_native_R_cells_retained']and p['one_SRAM_owner']
    assert not p['new_branch_condition_family_integrated']and not p['assembly_permitted']
    parent=Path(p['stage'])/p['stage_manifest_name'];assert sha(parent)==p['stage_manifest_sha256']
    old=json.loads(parent.read_text());old_project=old['project']
    out.mkdir(parents=True);sources={};changes={}
    for name,h in p['inputs'].items():
        source=native/'inputs'/name;raw=source.read_bytes()
        if Path(name).suffix in CONFIG:raw=raw.replace(old_project.encode(),str(project).encode())
        target=out/name;target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes(raw)
        sources[name]=sha(target)
    qsf=out/'core/Dorado.qsf';raw=qsf.read_text()
    marker='DORADO_HOST_CLOCK_HZ=57142857';assert raw.count(marker)==1
    raw=raw.replace(marker,'DORADO_HOST_CLOCK_HZ=66666667',1)
    added=[]
    for name in p['used_hdl']:
        assert name.startswith('src/')and name in p['inputs']
        path=str(project/name)
        if not re.search(r'(?:SYSTEMVERILOG|VERILOG)_FILE\s+"?'+re.escape(path)+r'"?(?:\s|$)',raw):
            raw+='\nset_global_assignment -name SYSTEMVERILOG_FILE '+path+'\n';added.append(name)
    qsf.write_text(raw)
    pll=out/'core/rtl/pll/pll_0002.v';raw=pll.read_text()
    marker='output_clock_frequency0("57.142857 MHz")';assert raw.count(marker)==1
    pll.write_text(raw.replace(marker,'output_clock_frequency0("66.666667 MHz")',1))
    config=out/'core/dorado_clock_config.json';c=json.loads(config.read_text())
    assert c['sys_hz']==57142857;c.update(sys_mhz='66.666667',sys_hz=66666667);config.write_text(json.dumps(c,indent=2)+'\n')
    lowered=None
    if lower:
        name='src/verilog/rtl/dorado_r_front_cone.sv';path=out/name;original=path.read_text()
        path.write_text(quartus_cone(original))
        lowered=dict(path=name,before_sha256=p['inputs'][name],after_sha256=sha(path),
                     helper='fit_r_front.quartus_cone',helper_source_sha256=sha(ROOT/'tools/fit_r_front.py'),
                     changes='Exact connected-wire substitutions for observations and explicit generate/endgenerate; no state/clock logic change')
    for name in sources:
        h=sha(out/name)
        if h!=sources[name]:changes[name]=dict(before_sha256=sources[name],after_sha256=h)
        sources[name]=h
        if name.endswith('.sdc'):
            assert (out/name).read_bytes()==(native/'inputs'/name).read_bytes().replace(old_project.encode(),str(project).encode()),name
    builder=ROOT/'tools/build_r_front_ready_native.py';shutil.copy2(builder,out/builder.name)
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    r={**old,'status':'PREPARED_FULL_NATIVE_CP_READ_15NS_DIAGNOSTIC','scope':__doc__,
       'project':str(project),'parent_project':old_project,'parent_stage':str(native),
       'parent_native_provenance_sha256':sha(native/'provenance.json.gz'),
       'source_hashes':sources,'configuration_changes':changes,'added_QSF_sources':added,
       'frequency_hz':66666667,'frequency_mhz':66.666667,'sysper':4,'nominal_ordinary_ns':120,
       'state_admin':'INACTIVE','assembly_permitted':False,'firmware_qualified':False,
       'physical_timing_qualified':False,'hardware_speedup_qualified':False,
       'full60ns_event_batch_connected':False,'new_branch_condition_family_integrated':False,
       'all194_native_HDL_source_bytes_unchanged':all(sha(out/n)==p['inputs'][n]for n in p['used_hdl']),
       'Quartus_combinational_cone_lowering':lowered,
       'no_new_timing_exceptions':True,'shared_SDRAM_clock_hz':66666667,'SDRAM_protocol_or_physical_timing_qualified':False,
       'full_goal_active':True}
    assert [n for n in p['used_hdl']if sha(out/n)!=p['inputs'][n]]==([lowered['path']]if lower else[])
    (out/'r-front-control-native.json').write_text(json.dumps(r,indent=2)+'\n')
    print(r['status'],len(sources),'inputs',len(added),'added source assignments')
    return r


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['native','output','project']:p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--quartus-lower',action='store_true')
    a=p.parse_args();prepare(a.native.resolve(),a.output.resolve(),a.project,a.quartus_lower)
