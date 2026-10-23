#!/usr/bin/env python3
"""Prepare bounded admission and pipelined writes on the exact 5c65 wrapper.

Transport only: preserve all original board/capture/clock/cache/Map/ECC wiring.
Accepted queue work retains raw-mux ownership through reset; selftest cannot
start its new epoch until the old work drains and the backend reset completes.
This transformation alone authorizes neither assembly nor a hardware load.
"""
import argparse
import hashlib
import json
from pathlib import Path

from prepare_storage_response_pipeline import replace_once

ROOT=Path(__file__).resolve().parents[1]
WRAPPER='src/build/rtl-executive/native16-tx-cable/dorado_native_machine.sv'


def sha(raw):return hashlib.sha256(raw).hexdigest()


def transform(raw):
    text=raw.decode()
    text=replace_once(text,'  assign storage_ready=storage_initialized && board_ready;',
        '''  // Reset drains accepted queue descriptors before selftest takes the port.
  wire storage_queue_reset,storage_queue_draining;
  wire storage_queue_grant=storage_initialized || storage_queue_draining;
  assign storage_ready=storage_initialized && !storage_queue_draining && board_ready;''')
    text=replace_once(text,'.clk(clk_host),.reset(reset),.ready(storage_initialized),',
        '.clk(clk_host),.reset(reset || storage_queue_draining),.ready(storage_initialized),')
    text=replace_once(text,
        '  dorado_storage_arb #(.GROUPS(GROUPS),.GROUP_ADDR_BITS(16),.RAW_ADDR_BITS(RAW_BITS),.PREFETCH_NEXT(1),.INTERLEAVE_GROUPS(1),.RESPONSE_OVERLAP(1),.REQUEST_LOOKAHEAD(1)) u_arb(\n    .clk(clk_host),.reset(reset || !storage_initialized),',
        '''  dorado_storage_queue #(.GROUPS(GROUPS),.GROUP_ADDR_BITS(16),.RAW_ADDR_BITS(RAW_BITS),.INTERLEAVE_GROUPS(1),.DEPTH(4)) u_arb(
    .clk(clk_host),.reset(reset),.enable(storage_initialized),
    .backend_reset(storage_queue_reset),.reset_draining(storage_queue_draining),.idle(),''')
    for old,new in [
        ('assign rr=storage_initialized && backend_ready;','assign rr=storage_queue_grant && backend_ready;'),
        ('assign sv=storage_initialized && backend_response;','assign sv=storage_queue_grant && backend_response;'),
        ('assign init_accept=!storage_initialized && backend_ready;','assign init_accept=!storage_queue_grant && backend_ready;'),
        ('assign init_response=!storage_initialized && backend_response;','assign init_response=!storage_queue_grant && backend_response;'),
        ('.EARLY_READ_RESPONSE(1)) u_sdram(','.EARLY_READ_RESPONSE(1),.BURST_READ_REUSE(1),.PIPELINED_ROW_WRITES(1)) u_sdram('),
        ('    .clk(clk_host),.reset(reset),\n    .req_valid(storage_initialized ? rv : init_valid)',
         '    .clk(clk_host),.reset(storage_queue_reset),\n    .req_valid(storage_queue_grant ? rv : init_valid)'),
        ('storage_initialized ? rw : init_write','storage_queue_grant ? rw : init_write'),
        ('storage_initialized ? ra : init_address','storage_queue_grant ? ra : init_address'),
        ('storage_initialized ? wd : init_data','storage_queue_grant ? wd : init_data'),
        ('storage_initialized ? sr : init_response_ready','storage_queue_grant ? sr : init_response_ready'),
    ]:text=replace_once(text,old,new)
    return text.encode()


def prepare(parent,qualification,generated,output):
    assert not output.exists(),'Inspect existing preparation'
    q=json.loads(qualification.read_text())
    assert q['status']=='PASS_NATIVE_OLD_WORD_EARLY_PREASSEMBLY_REVIEW'
    assert q['frequency_hz']==57142857 and q['sysper']==4 and len(q['source_hashes'])==503
    original=(parent/WRAPPER).read_bytes();assert sha(original)==q['source_hashes'][WRAPPER]
    proof=json.loads((generated/'provenance.json').read_text())
    assert proof['pipelined_row_writes'] and proof['burst_read_reuse']
    for n,h in proof['generated_hashes'].items():assert sha((generated/n).read_bytes())==h
    files={
        'dorado_native_machine.sv':transform(original),
        'dorado_storage_queue.sv':(ROOT/'fpga/mister/dorado_storage_queue.sv').read_bytes(),
        'dorado_storage_selftest.sv':(parent/'src/fpga/mister/dorado_storage_selftest.sv').read_bytes(),
        'dorado_storage_sdram_burst.sv':(generated/'dorado_storage_sdram_burst.sv').read_bytes(),
    }
    assert sha(files['dorado_storage_selftest.sv'])==q['source_hashes']['src/fpga/mister/dorado_storage_selftest.sv']
    output.mkdir(parents=True)
    for n,b in files.items():(output/n).write_bytes(b)
    record=dict(status='PREPARED_NATIVE_QUEUE_RESET_INIT_UNQUALIFIED',scope=__doc__,
        parent_qualification_sha256=sha(qualification.read_bytes()),parent_wrapper_sha256=sha(original),
        parent_source_hashes=q['source_hashes'],generated_proof=proof,
        generated_hashes={n:sha(b)for n,b in files.items()},
        native_group_count=128,native_group_address_bits=16,native_raw_address_bits=23,
        CPU_cache_Map_ECC_clocks_constraints_unchanged=True,assembly_permitted=False)
    (output/'provenance.json').write_text(json.dumps(record,indent=2)+'\n')
    return record


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['parent','qualification','generated','output']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();r=prepare(**{n:v.resolve()for n,v in vars(a).items()})
    print(r['status'])
