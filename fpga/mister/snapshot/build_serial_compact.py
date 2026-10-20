#!/usr/bin/env python3
"""Stage the qualified compact MSAs in the reviewed nineteen-board area build.

All parent bytes come from immutable receipt archives. Preserve real clocks,
IO, area policy and virtual capacity service contracts. Remove only obsolete
MSA hold exceptions to registers that no longer exist. No assembly/deployment.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess

ROOT=Path(__file__).resolve().parents[3]
TARGET='src/fpga/mister/snapshot_serial/'
BP='src/build/rtl-executive/native16-tx-cable/dorado_native_backplane.v'
PARENT_REPORT='2026-10-19-snapshot-serial-area'
TRANSPORT_REPORT='2026-10-19-snapshot-msa-compact-transport'
TIMING_NOTE='''# Compact MSA bridge has no saved_addr/saved_data registers.
# Their former own-source hold exceptions are removed, not broadened.
# Original remaining constraints follow unchanged; no new setup/hold cuts.
'''


def sha(raw):return hashlib.sha256(raw).hexdigest()


def stage(out,reports):
    from generate_serial_boards import raw_receipt,once
    import serial_state_format_msa_compact as fmt
    parent=reports/PARENT_REPORT;qualified=reports/TRANSPORT_REPORT
    area=json.loads(raw_receipt(parent,'project/area-integration.json'))
    serial=json.loads(raw_receipt(parent,'project/serial-integration.json'))
    review=json.loads(raw_receipt(parent,'source-audit.json'))
    assert review['status']=='PASS_SOURCE_AREA_OPTIONS_ONLY' and len(review['failure_controls'])==27
    original_project=serial['project'];catalog=fmt.build_catalog(reports)
    assert catalog==json.loads(raw_receipt(qualified,'simulation-v2/catalog.json'))
    inputs=json.loads(raw_receipt(parent,'inputs/native59-compiled-inputs.json'))
    for name,digest in inputs.items():assert sha(raw_receipt(parent,'baseline-native59/'+name))==digest,'Native59 receipt differs: '+name
    assert len(inputs)==413
    out.mkdir(parents=True,exist_ok=False);verified={}
    for name,digest in area['source_hashes'].items():
        raw=raw_receipt(parent,'project/'+name);assert sha(raw)==digest,'parent staged input differs: '+name
        verified[name]=digest
        if Path(name).suffix in ('.qsf','.qpf','.qip','.sdc','.tcl'):
            raw=raw.decode().replace(original_project,str(out)).encode()
        p=out/name;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(raw)
    assert len(verified)==659
    assets=json.loads((out/TARGET/'assets.json').read_text());receipts={}
    for name in assets['assets']:
        if name.startswith('MSA__'):source=name[len('MSA__'):]
        elif name in ('board_state_fabric_msa.sv','board_state_uio_msa.sv','msa_state_group.sv'):
            source='msa_state_group_compact.sv'if name=='msa_state_group.sv'else name
        else:continue
        raw=raw_receipt(qualified,'simulation-v2/candidate/'+source)
        (out/TARGET/name).write_bytes(raw);receipts[name]=dict(directory=TRANSPORT_REPORT,name='simulation-v2/candidate/'+source,sha256=sha(raw))
    p=out/BP;s=p.read_text();p.write_text(once(s,'dorado_msa_state_group u_snapshot_msa(','dorado_msa_state_group_compact u_snapshot_msa('))
    p=out/'core/Dorado.sv';s=p.read_text();p.write_text(once(s,".SCHEMA_ID(64'hb80a00977d7e7992)",f".SCHEMA_ID(64'h{catalog['schema_id']})"))
    p=out/'core/native_timing.sdc';s=p.read_text();begin=s.index('# Native15 exposes the same F10145A');end=s.index('\n# Serial-mode extension of the existing own-source HOLD proof:')
    assert 'saved_addr' in s[:begin] and 'saved_data' in s[:begin]
    p.write_text(TIMING_NOTE+s[begin:end])
    assets.update(scope=__doc__,board_bits=1957944,schema_id=catalog['schema_id'],catalog_sha256=catalog['catalog_sha256'],
                  assets={n:sha((out/TARGET/n).read_bytes())for n in assets['assets']},builder_sha256=sha(Path(__file__).read_bytes()),
                  source_receipts=receipts,parent_assets_sha256=sha(raw_receipt(parent,'project/'+TARGET+'assets.json')))
    (out/TARGET/'assets.json').write_text(json.dumps(assets,indent=2)+'\n')
    (out/TARGET/'catalog.json').write_text(json.dumps(catalog,indent=2)+'\n')
    record=dict(scope=__doc__,project=str(out),parent_project=original_project,parent_report=PARENT_REPORT,
        parent_area_sha256=sha(raw_receipt(parent,'project/area-integration.json')),parent_source_audit_sha256=sha(raw_receipt(parent,'source-audit.json')),
        verified_parent_sources=verified,verified_native59_inputs=inputs,qualified_replacements=receipts,
        source_hashes={n:sha((out/n).read_bytes())for n in verified},schema_id=catalog['schema_id'],physical_board_bits=1957944,boards=19,slots=12,
        driver_sha256=sha(Path(__file__).read_bytes()),capacity_only=True,assembly_permitted=False,real_pause_barriers=False,
        timing='Original real clocks/IO and area policy; obsolete MSA hold cuts removed; no new cuts',
        report_prefix='serial61',limits='Explicit virtual service acknowledgments; PLL reset proxy not true reseed. No complete snapshot, deployable image or speed qualification.')
    (out/'compact-integration.json').write_text(json.dumps(record,indent=2)+'\n')
    print('Staged nineteen boards with compact MSAs;',len(verified),'parent inputs;',record['physical_board_bits'],'physical bits',flush=True)


def run(out,quartus):
    record=json.loads((out/'compact-integration.json').read_text())
    assert record['capacity_only']and not record['assembly_permitted']and not record['real_pause_barriers']
    assert sha(Path(__file__).read_bytes())==record['driver_sha256']
    for name,digest in record['source_hashes'].items():assert sha((out/name).read_bytes())==digest,'staged source changed: '+name
    assert not(out/'map.log').exists(),'observe existing build; do not restart'
    for tool,args,label in [('quartus_map',['Dorado'],'map'),('quartus_fit',['Dorado'],'fit'),('quartus_sta',['Dorado'],'sta'),
                            ('quartus_sta',['-t','audit_native_clocks.tcl'],'clocks'),('quartus_sta',['-t','device-corners.tcl'],'corners'),
                            ('quartus_sta',['-t','audit-adapter-crossings.tcl'],'crossings')]:
        print('Serial63Compact',label,flush=True)
        with(out/(label+'.log')).open('w')as log:subprocess.run([str(quartus/tool),*args],cwd=out/'core',stdout=log,stderr=subprocess.STDOUT,check=True)
    print('Compact complete-core run terminal; independent timing/memory/source review required; no assembly.',flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--stage',type=Path)
    p.add_argument('--reports',type=Path,default=ROOT/'fpga/mister/reports');p.add_argument('--run',type=Path)
    p.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'));a=p.parse_args()
    if a.stage:stage(a.stage.resolve(),a.reports.resolve())
    elif a.run:run(a.run.resolve(),a.quartus)
    else:p.error('Specify --stage or --run')
