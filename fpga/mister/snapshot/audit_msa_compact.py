#!/usr/bin/env python3
"""Invert compact-MSA wiring/pure predictor edits against qualified receipt bytes.

Guest DRAM process and all original FF bodies are exact. Host bridge semantics
require the independent original RTL/DRAM tests; this source audit alone is not
behavioral equivalence or native/global snapshot qualification.
"""
import argparse
import copy
import hashlib
import json
from pathlib import Path
from generate_serial_boards import raw_receipt
from classify_msa_compact import expected_fields
import generate_storage_group as original

ROOT=Path(__file__).resolve().parents[3]
PRED='  assign addr_will_change=((~p4 & ras_d) && (row != a)) || ((~p15 & cas_d) && (col != a));\n'


def sha(raw):return hashlib.sha256(raw).hexdigest()


def audit(case,manifest,reports,overrides=None):
    overrides=overrides or {};read=lambda n:overrides.get(n,(case/n).read_bytes())
    qualified=reports/'2026-10-19-snapshot-msa-transport'
    ref=lambda n:raw_receipt(qualified,'simulation-v5/candidate/'+n)
    board=read('msa_board.sv');assert sha(board)==manifest['generated_sha256']
    text=board.decode();assert text.count('.STATE_BITS(1650)')==1
    text=text.replace('.STATE_BITS(1650)','.STATE_BITS(2690)')
    for i in range(16):
        for before,after in [(f';wire dram_change_{i};',';'),
                             (f'.addr(dram_addr_{i}),.addr_will_change(dram_change_{i})\n',f'.addr(dram_addr_{i})\n'),
                             (f'.addr(dram_addr_{i}),.addr_will_change(dram_change_{i}),',f'.addr(dram_addr_{i}),')]:
            assert text.count(before)==1,'address predictor binding differs: '+str(i)
            text=text.replace(before,after,1)
    assert text.encode()==ref('msa_board.sv'),'original pin/parameter/equation changed'
    for name,digest in manifest['cloned_sources'].items():
        raw=read(name);assert sha(raw)==digest,'helper receipt differs'
        if name=='storage_group.sv':continue
        if name=='cell_MK4096P_addr.sv':
            source=raw.decode();assert source.count(PRED)==1,'address predictor changed'
            source=source.replace(PRED,'',1)
            line='    output wire addr_will_change,\n';assert source.count(line)==1
            raw=source.replace(line,'',1).encode()
        assert raw==ref(name),'original FF/control helper differs: '+name
    frontend=read('storage_group.sv').decode()
    old=original.original_source();assert sha(old)==original.SHA
    source=old.decode();guest=source[source.index('        cas_d<=cas_n;we_d<=we_n;'):source.index('    always @(posedge host_clk)')]
    assert guest.endswith('    end\n');guest=guest[:-len('    end\n')]
    assert frontend.count(guest)==1,'original guest DRAM pipeline changed'
    assert manifest['frontend_manifest']['original_sha256']==original.SHA
    assert manifest['frontend_manifest']['physical_bits']=={'width8':46,'width12':58}
    _,fields=expected_fields();expected={r['path']:r['width']for r in fields};positions=[]
    for row in manifest['layout']:
        assert expected.pop(row['path'],None)==row['width'],'compact scan field differs'
        positions.extend(range(row['lsb'],row['lsb']+row['width']))
    assert not expected and positions==list(range(1650)),'compact scan omits/aliases fields'
    assert manifest['scan']==dict(state_bits=1650,words=92,word_bits=18,last_word_bits=12,completion_edges=1650)
    return dict(status='PASS_COMPACT_SOURCE_SCOPE',scope=__doc__,original_ff_bits=866,actual_frontend_bits=784,
        actual_total_bits=1650,fields=378,normal_pins_parameters_equations_unchanged=True,guest_process_byte_identical=True,
        address_predictor='Pure output on original row/column helper; sixteen exact group bindings',
        limits='Host bridge relation requires behavioral tests. New transport/catalog and full core are pending.')


def controls(case,manifest,reports):
    cases=[('cross_group_binding','msa_board.sv','.addr(dram_addr_0),.addr_will_change(dram_change_0),','.addr(dram_addr_0),.addr_will_change(dram_change_1),'),
           ('predictor_logic','cell_MK4096P_addr.sv','(row != a)','(row == a)'),
           ('original_ff','cell_SN74166.sv','endmodule',"wire hidden=1'b1;\nendmodule"),
           ('guest_pipeline','storage_group.sv','read_pipe<=read_value[WIDTH-1:0];','read_pipe<=write_data;'),
           ('extra_board_logic','msa_board.sv','endmodule',"assign storage_ready=1'b1;\nendmodule"),
           ('wrong_clock','msa_board.sv','.state_host_clk(storage_clk)','.state_host_clk(sys_clk)')]
    results=[]
    for name,target,before,after in cases:
        raw=(case/target).read_bytes();assert before.encode()in raw
        damaged=raw.replace(before.encode(),after.encode(),1);record=copy.deepcopy(manifest)
        if target=='msa_board.sv':record['generated_sha256']=sha(damaged)
        else:record['cloned_sources'][target]=sha(damaged)
        try:audit(case,record,reports,{target:damaged})
        except AssertionError as error:results.append(dict(case=name,rejected=True,diagnostic=str(error),damaged_metadata_rehashed=True))
        else:raise AssertionError('damaged compact source accepted: '+name)
    for name in ('schema_alias','schema_missing','old_scan_bounds'):
        broken=copy.deepcopy(manifest)
        if name=='schema_alias':broken['layout'][1]['lsb']-=1
        elif name=='schema_missing':broken['layout'].pop()
        else:broken['scan']['words']=150
        try:audit(case,broken,reports)
        except AssertionError as error:results.append(dict(case=name,rejected=True,diagnostic=str(error)))
        else:raise AssertionError('damaged compact schema accepted: '+name)
    return results


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('tests',type=Path)
    p.add_argument('--reports',type=Path,default=ROOT/'fpga/mister/reports');p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    manifest=json.loads((a.tests/'manifest.json').read_text());case=a.tests/'candidate'
    result=audit(case,manifest,a.reports);result['failure_controls']=controls(case,manifest,a.reports)
    a.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(dict(status=result['status'],failure_controls=len(result['failure_controls']))))
