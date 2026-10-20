#!/usr/bin/env python3
"""MSA component with a shared external clock for its 78 physical FF helpers.

The supplied FF clock must pulse on precisely each real guest advance or scan
step. Host memory frontends and the scan controller retain the free-running
storage clock. Every physical field/helper equation and scan address remains.
This exposes a clock contract; native global clock implementation is separate.
"""
import argparse
import hashlib
import json
from pathlib import Path
import generate_msa_board as base
from test_msa_transport import hoist_msa_declarations


def generate():
    old,manifest,helpers=base.generate();old,normalization=hoist_msa_declarations(old)
    source=base.once(old,'    input wire state_guest_advance,state_freeze,state_guest_held,',
                     '    input wire state_ff_clk,output wire state_ff_clock_request,\n    input wire state_guest_advance,state_freeze,state_guest_held,')
    before='.state_host_clk(storage_clk),.state_advance(state_guest_advance),.state_hold(state_hold)'
    after=".state_host_clk(state_ff_clk),.state_advance(1'b1),.state_hold(state_hold)"
    assert source.count(before)==78
    source=source.replace(before,after)
    source=base.once(source,'  wire scan_serial_in,scan_step,scan_ready,scan_busy;',
                     '  wire scan_serial_in,scan_step,scan_ready,scan_busy;\n  assign state_ff_clock_request=scan_step;')
    inverse=source.replace(after,before).replace('    input wire state_ff_clk,output wire state_ff_clock_request,\n','').replace('\n  assign state_ff_clock_request=scan_step;','')
    assert inverse==old
    record=dict(manifest);record.update(scope=__doc__,reference_sha256=hashlib.sha256(old.encode()).hexdigest(),
        generated_sha256=hashlib.sha256(source.encode()).hexdigest(),declaration_normalization=normalization,
        ff_clocked_packages=78,physical_bits=2690,board_private_bits=83,
        clock_contract='Related storage-clock rising edges only; real guest advances or scans. Other-board scan pulses require this board already held. External gate phase must be captured/reconstructed by a future global barrier.',
        native_transport_integration='PENDING: external FF-clock contract is not integrated in the current4096-cycle twelve-slot transport.',
        limits='Component experiment; no native clock gate/barrier, area/timing, FPGA or speed qualification.')
    return source,record,helpers


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args();a.output.mkdir(parents=True,exist_ok=False)
    source,record,helpers=generate()
    for name,body in {'msa_board.sv':source,**helpers}.items():(a.output/name).write_text(body)
    (a.output/'manifest.json').write_text(json.dumps(record,indent=2)+'\n')
