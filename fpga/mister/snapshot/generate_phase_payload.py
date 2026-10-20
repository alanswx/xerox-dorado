#!/usr/bin/env python3
"""Optional phase-qualified payload FFs from exact frozen Serial73 helpers.

The producer must prove at every capturing edge that a definite advance has
no hold and a definite scan write has a definite hold. The ordinary and scan
payloads are unchanged. Default-off is byte-invertible to the original helper.
This contract has NOT yet been proved for the complete native integration.
"""
import hashlib
import re
from generate_serial_boards import ROOT,raw_receipt,once

REPORT='2026-10-20-snapshot-native-ready-prediction-rejected'
PREFIX='terminal-fit/src/fpga/mister/snapshot_serial/'
KINDS={'MC10176':13,'MC10231':6}


def reference(kind):
    assert kind in KINDS
    return raw_receipt(ROOT/'fpga/mister/reports'/REPORT,PREFIX+'MemC__cell_'+kind+'_snapshot.v').decode()


def transform(source):
    source=once(source,'#(parameter PRECAP = 1)',
        '#(parameter PRECAP = 1, parameter PHASE_QUALIFIED = 0)')
    start=source.index('  always @(posedge state_host_clk) begin\n')
    # BaseBoard uses serial ports instead of a parallel state_data word and
    # keeps a combinational scan_out assignment after this payload block.
    suffix='    end\n  end'
    end=source.index(suffix,start)+len(suffix)
    original=source[start:end]
    # Keep the original normal payload verbatim. Only the already-qualified
    # scan/advance choices may change in the optional branch.
    head=re.match(r'  always @\(posedge state_host_clk\) begin\n'
        r'    if \(state_hold\) begin\n      if\s*\(state_write\)\s*',original)
    assert head
    split='\n    end else if (state_advance) begin\n'
    assert original.count(split)==1
    restore,normal=original[head.end():].split(split)
    assert restore.endswith(';')and normal.endswith('    end\n  end')
    payload=normal[:-len('    end\n  end')]
    branch='''  generate if(PHASE_QUALIFIED)begin:phase_payload
  // No new payload history. The source-phase contract replaces redundant
  // payload hold gates; admission/frozen/quarantine logic remains external.
  always @(posedge state_host_clk) begin
    if(state_write) '''+restore+'''
    else if(state_advance)begin
'''+payload+'''    end
  end
  // synthesis translate_off
  always @(posedge state_host_clk)begin
    if(state_advance===1'b1 && state_hold!==1'b0)
      $fatal(1,"phase payload advance/hold contract violated");
    if(state_write===1'b1 && state_hold!==1'b1)
      $fatal(1,"phase payload scan/hold contract violated");
  end
  // synthesis translate_on
  end else begin:original_payload
'''+original+'''
  end endgenerate
'''
    return source[:start]+branch+source[end:]


def inverse(source,original):
    start=source.index('  generate if(PHASE_QUALIFIED)begin:phase_payload\n')
    end=source.index('  end endgenerate\n',start)+len('  end endgenerate\n')
    default=source[source.index('  end else begin:original_payload\n',start)+len('  end else begin:original_payload\n'):end-len('  end endgenerate\n')]
    assert default.endswith('\n')
    result=source[:start]+default[:-1]+source[end:]
    result=once(result,'#(parameter PRECAP = 1, parameter PHASE_QUALIFIED = 0)',
        '#(parameter PRECAP = 1)')
    assert result==original,'original default payload changed'
    return result


def generate():
    sources={};manifest={}
    for kind,bits in KINDS.items():
        old=reference(kind);new=transform(old);assert inverse(new,old)==old
        sources[kind+'.sv']=new
        manifest[kind]=dict(physical_bits=bits,additional_sequential_bits=0,
            original_sha256=hashlib.sha256(old.encode()).hexdigest(),generated_sha256=hashlib.sha256(new.encode()).hexdigest(),
            default_off_inverse=True,phase_contract_required=True)
    return sources,dict(scope=__doc__,families=manifest,native_integration_qualified=False,
        limits='Two FF-family component prototype only; complete native producer/consumer phase proof, RAM families, state access, routed timing and FPGA qualification remain required.')
