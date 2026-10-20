#!/usr/bin/env python3
"""Invert the precise MSA clock-interface edits against qualified receipt bytes.

All original pins/equations/parameters, physical fields, scan addresses and eight
helper bodies remain exact. This is source proof, not gate/barrier qualification.
"""
import argparse
import copy
import hashlib
import json
from pathlib import Path
from generate_serial_boards import raw_receipt

ROOT=Path(__file__).resolve().parents[3]
NEW=".state_host_clk(state_ff_clk),.state_advance(1'b1),.state_hold(state_hold)"
OLD='.state_host_clk(storage_clk),.state_advance(state_guest_advance),.state_hold(state_hold)'


def sha(raw):return hashlib.sha256(raw).hexdigest()


def audit(case,manifest,reports,overrides=None):
    overrides=overrides or {};read=lambda n:overrides.get(n,(case/n).read_bytes())
    reference_root=reports/'2026-10-19-snapshot-msa-transport'
    expected=raw_receipt(reference_root,'simulation-v5/candidate/msa_board.sv')
    source=read('msa_board.sv');assert sha(source)==manifest['generated_sha256']
    text=source.decode();assert text.count(NEW)==78,'not all78physical helpers use the new clock'
    text=text.replace(NEW,OLD)
    line='    input wire state_ff_clk,output wire state_ff_clock_request,\n';assert text.count(line)==1;text=text.replace(line,'',1)
    line='\n  assign state_ff_clock_request=scan_step;';assert text.count(line)==1;text=text.replace(line,'',1)
    assert text.encode()==expected,'normal pins/equations/scan wiring differs'
    assert manifest['reference_sha256']==sha(expected)
    for name in manifest['cloned_sources']:
        raw=read(name);qualified=raw_receipt(reference_root,'simulation-v5/candidate/'+name)
        assert raw==qualified and sha(raw)==manifest['cloned_sources'][name],'qualified helper changed: '+name
    qualified=json.loads(raw_receipt(reports/'2026-10-19-snapshot-msa-board','simulation-v4/manifest.json'))
    for key in ('layout','scan','ff','frontends','parameters','logical_state_bits','ff_bits','frontend_bits'):
        assert manifest[key]==qualified[key],'physical schema differs: '+key
    return dict(status='PASS_CLOCK_INTERFACE_SOURCE_ONLY',scope=__doc__,ff_packages=78,physical_bits=2690,fields=458,helper_bodies=8,
                byte_identity_after_inverse=True,reference_sha256=sha(expected),clock_gate_implementation='External; not qualified by this audit')


def controls(case,manifest,reports):
    results=[]
    for name,target,before,after in [('ff_clock','msa_board.sv','.state_host_clk(state_ff_clk)','.state_host_clk(storage_clk)'),
        ('old_advance','msa_board.sv',".state_advance(1'b1)",'.state_advance(state_guest_advance)'),
        ('clock_request','msa_board.sv','assign state_ff_clock_request=scan_step;','assign state_ff_clock_request=scan_busy;'),
        ('extra_logic','msa_board.sv','endmodule',"assign storage_ready=1'b1;\nendmodule"),
        ('helper_logic','cell_SN74166.sv','endmodule','wire hidden=1;\nendmodule')]:
        raw=(case/target).read_bytes();assert before.encode()in raw;damaged=raw.replace(before.encode(),after.encode(),1);record=copy.deepcopy(manifest)
        if target=='msa_board.sv':record['generated_sha256']=sha(damaged)
        else:record['cloned_sources'][target]=sha(damaged)
        try:audit(case,record,reports,{target:damaged})
        except AssertionError as error:results.append(dict(case=name,rejected=True,diagnostic=str(error),damaged_metadata_rehashed=True))
        else:raise AssertionError('damaged clock source accepted: '+name)
    record=copy.deepcopy(manifest);record['layout'][1]['lsb']-=1
    try:audit(case,record,reports)
    except AssertionError as error:results.append(dict(case='schema_alias',rejected=True,diagnostic=str(error)))
    else:raise AssertionError('damaged clock schema accepted')
    return results


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('tests',type=Path);p.add_argument('--reports',type=Path,default=ROOT/'fpga/mister/reports');p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    manifest=json.loads((a.tests/'manifest.json').read_text());result=audit(a.tests/'candidate',manifest,a.reports)
    result['failure_controls']=controls(a.tests/'candidate',manifest,a.reports);a.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(dict(status=result['status'],physical_bits=2690,failure_controls=len(result['failure_controls']))))
