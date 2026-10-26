#!/usr/bin/env python3
"""Prepare a native-only RIOT candidate with identical registered bus latency.

Keep the original model alongside it. Change only RAM representation and the
read-output mux needed to use a synchronous memory read. The native five
instances use init_7800=0; reject any other setting in this private candidate.
The artifact is not connected to a native core by this preparation step.
"""
import argparse
import json
from pathlib import Path
import re
import shutil

from prove_cp_prefix_cache import module
from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1]
RIOT='src/verilog/vendor/riot/M6532.sv'
HELPER='verilog/rtl/dorado_riot_ram.sv'


def candidate(original):
    raw=original;edits=[]
    def change(before,after):
        nonlocal raw
        assert raw.count(before)==1,before
        raw=raw.replace(before,after,1);edits.append((before,after))
    change('module M6532_ce','module dorado_riot_blockram_ce')
    change('output logic [7:0] d_out,','output wire [7:0] d_out,')
    change('reg [7:0] riot_ram[128];','''// Same-edge read publication: memory and selection both register here.
wire [7:0] ram_read_data;
reg ram_selected;
reg [7:0] other_d_out;
wire ram_read_enable=ce && (CS1 & ~CS2_n) && RW_n && ~RS_n;
wire ram_write_enable=ce && (CS1 & ~CS2_n) && ~RW_n && ~RS_n;
dorado_riot_ram native_ram(clk,guest_advance,~res_n,ram_read_enable,
 ram_write_enable,addr,d_in,ram_read_data);
assign d_out=ram_selected ? ram_read_data : other_d_out;
initial if(init_7800!=0)$fatal(1,"Native RIOT RAM requires init_7800=0");''')
    reset=re.search(r'\t// Set to specific value.*?(?=\n\tout_a <=)',raw,re.S)
    assert reset
    change(reset[0],'\t// Logical RAM reset is represented by native_ram validity.\n')
    change('riot_ram[addr] <= d_in;','begin end // The single native_ram port performs this write.')
    writes=list(re.findall(r'd_out <= ([^;]+);',raw))
    assert len(writes)==8,writes
    for expression in writes:
        before='d_out <= '+expression+';'
        after=('ram_selected <= 1\'b1;'if expression=='riot_ram[addr]'else
               "{ram_selected,other_d_out} <= {1'b0,"+expression+'};')
        change(before,after)
    restored=raw
    for before,after in reversed(edits):restored=restored.replace(after,before,1)
    assert restored==original
    return raw


def prepare(stage,out):
    assert not out.exists()
    m=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,m['source_hashes'])
    assert m['clock_enable_conversion_staged']and not m['assembly_permitted']
    source=(stage/RIOT).read_text();old=module(source,'M6532_ce');new=candidate(old)
    out.mkdir(parents=True)
    prelude=source[:source.index('module M6532')]
    (out/'reference.sv').write_text(prelude+old)
    (out/'candidate.sv').write_text(prelude+new)
    shutil.copy2(ROOT/HELPER,out/'ram.sv')
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    result=dict(status='PREPARED_NATIVE_RIOT_BLOCK_RAM_COMPONENT',stage=str(stage),
                stage_manifest_sha256=sha(stage/'r-front-control-native.json'),original_RIOT_sha256=sha(stage/RIOT),
                reference_module_sha256=sha(out/'reference.sv'),candidate_module_sha256=sha(out/'candidate.sv'),
                helper_sha256=sha(out/'ram.sv'),native_init_7800=0,
                non_RAM_state_and_transfer_rules_retained=True,additional_read_latency_in_guest_edges=0,
                native_connected=False,firmware_or_physical_timing_or_hardware_speed_qualified=False,
                files={q.name:sha(q)for q in out.iterdir()if q.is_file()})
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n')
    print(result['status'],flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['stage','output']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();prepare(a.stage.resolve(),a.output.resolve())
