#!/usr/bin/env python3
"""Exact post-edge predictor versus frozen original frontend, two chip widths.

Compare every original state/output and registered CPU-ready value. Exercise
all binary controls, unknown flags/scan data/control, and live reset/init guards.
The added aggregate register is REQUIRED state; no native timing claim.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import generate_msa_ready_prediction as gen

HERE=Path(__file__).resolve().parent


def sha(raw):return hashlib.sha256(raw).hexdigest()


def diagnostics_only(source):
    begin=source.index('    // synthesis translate_off\n')
    end=source.index('    // synthesis translate_on\n',begin)+len('    // synthesis translate_on\n')
    block=source[begin:end];assert 'always @(posedge host_clk)'in block and '$fatal'in block
    result=source[:begin]+source[end:];assert result[:begin]+block+result[begin:]==source
    return result,block


def run(out):
    out.mkdir(parents=True,exist_ok=False);inputs=out/'executed-inputs';inputs.mkdir()
    original=gen.reference('storage_group.sv');candidate=gen.frontend(original);assert gen.inverse_frontend(candidate)==original
    original_test,diagnostic=diagnostics_only(original);candidate_test,other=diagnostics_only(candidate);assert diagnostic==other
    reference=gen.once(original_test,'module dorado_serial_MSA__dorado_storage_group_snapshot #','module dorado_ready_reference #')
    shared='''.state_guest_advance(advance),.state_hold(held),.state_scan_step(scan),.state_serial_in(serial_in),
 .host_clk(clk),.machine_clk(1'b0),.reset(storage_reset),.ras_n(ras_n),.cas_n(cas_n),.we_n(we_n),
 .addr_will_change(address_change),.addr(16'h59a3),.data_in(data_in),
 .req_ready(req_ready),.rsp_valid(rsp_valid),.rsp_data({{(16-W){1'b0}},rsp_word})'''
    instances='dorado_ready_reference #(.WIDTH(W)) reference('+shared+');\n'
    instances+='dorado_serial_MSA__dorado_storage_group_snapshot #(.WIDTH(W)) dut('+shared+',.state_flags_ready_next(next_flags_ready));'
    fixture=gen.once((HERE/'test_storage_ready_prediction.sv').read_text(),'        // INSTANCE_BLOCK',instances)
    for n,s in(('original.sv',original),('candidate.sv',candidate),('reference-test.sv',reference),('diagnostics-only.sv',diagnostic)):(inputs/n).write_text(s)
    for p in(Path(__file__),Path(gen.__file__),HERE/'test_storage_ready_prediction.sv'):(inputs/p.name).write_bytes(p.read_bytes())
    cases=[('candidate',None,None,None,None),
        ('current_instead_of_next','rtl','!ready_flags_next[3] && ready_flags_next[1] &&\n        (!ready_flags_next[0] || ready_flags_next[2])','!waiting && cache_valid && (!write_pending || write_done)','registered readiness differs'),
        ('wrong_hold_priority','rtl','if(held)begin',"if(1'b0)begin",'registered readiness differs'),
        ('wrong_scan_source','rtl','state_bits[6+3*WIDTH]','state_bits[7+3*WIDTH]','registered readiness differs'),
        ('wrong_response_kind','rtl','waiting && rsp_valid,req_write','waiting && rsp_valid,write_pending','registered readiness differs'),
        ('lost_reset_transition','rtl',"if(reset_now)next_flags[3:1]=3'b000;","if(1'b0)next_flags[3:1]=3'b000;",'registered readiness differs'),
        ('wrong_register_edge','fixture','always @(posedge clk)cached_ready','always @(negedge clk)cached_ready','registered readiness differs'),
        ('guest_only_prediction','fixture','cached_ready<=next_flags_ready;','if(advance)cached_ready<=next_flags_ready;','registered readiness differs')]
    results=[]
    for label,target,before,after,diagnostic in cases:
        case=out/label;case.mkdir();rtl=candidate_test;top=fixture
        if target=='rtl':rtl=gen.once(rtl,before,after)
        elif target=='fixture':top=gen.once(top,before,after)
        (case/'candidate.sv').write_text(rtl);(case/'fixture.sv').write_text(top)
        cmd=['iverilog','-g2012','-s','tb','-o',str(case/'test.vvp'),str(case/'fixture.sv'),str(case/'candidate.sv'),str(inputs/'reference-test.sv')]
        (case/'commands.json').write_text(json.dumps([cmd,['vvp',str(case/'test.vvp')]],indent=2)+'\n')
        with(case/'build.log').open('w')as log:subprocess.run(cmd,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run(['vvp',str(case/'test.vvp')],capture_output=True,text=True,timeout=180)
        text=result.stdout+result.stderr;(case/'test.log').write_text(text)
        if diagnostic:assert result.returncode and diagnostic in text,text
        else:assert not result.returncode and'READY PREDICTION FOURSTATE PASS'in text,text
        print(label+': '+text.strip(),flush=True)
        results.append(dict(case=label,returncode=result.returncode,expected_failure=diagnostic,
            rtl_sha256=sha(rtl.encode()),fixture_sha256=sha(top.encode()),log_sha256=sha(text.encode())))
    (out/'results.json').write_text(json.dumps(dict(status='PASS',scope=__doc__,cases=results,
        new_frontend_sequential_bits=0,required_external_register_bits_per_msa=1,
        original_rtl_inverse_equal=True,removed_test_diagnostics_only=True,
        inputs={p.name:sha(p.read_bytes())for p in inputs.iterdir()},
        limits='Frontend and one external aggregate-register prototype only. Complete MSA/SDRAM traffic, native integration, state access, actual capacity/timing and FPGA boot remain required.'),indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args();run(a.output.resolve())
