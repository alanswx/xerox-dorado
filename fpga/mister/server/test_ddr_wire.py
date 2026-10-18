#!/usr/bin/env python3
"""Test production Dorado.sv frame wiring, real adapters, and the C ring peer."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import tempfile

HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[2]

def fixture(source):
    # Extract the actual production declarations/mux/engine, falling-edge
    # wire enable and both adapter instantiations. Only CPU diagnostics and
    # unrelated devices are outside this fixture; no replacement packet mux.
    start=source.index('wire ethernet_rx, ethernet_tx_n, net_rx_ready, net_tx_valid;')
    end=source.index('`ifdef DORADO_NATIVE',source.index('always @(negedge clk_sys) network_step',start))
    prefix=source[start:end].replace('wire ethernet_rx, ethernet_tx_n, net_rx_ready, net_tx_valid;',
                                    'wire net_rx_ready, net_tx_valid;')
    start=source.index('dorado_ethernet_uio #',end)
    end=source.index('\naltclkctrl #',start)
    adapters=source[start:end].rstrip()
    assert adapters.endswith('`endif')
    adapters=adapters[:-len('`endif')]+'\n'
    return '''`define DORADO_ETHERNET_DDR
module dorado_ddr_wire_fixture(
 input wire clk_sys,pll_locked,machine_enable,ethernet_tx_n,
 output wire ethernet_rx,
 input wire io_enable,io_strobe,input wire [15:0] io_din,
 output wire [15:0] io_dout,output wire io_selected,
 input wire DDRAM_BUSY,DDRAM_DOUT_READY,input wire [63:0] DDRAM_DOUT,
 output wire [28:0] DDRAM_ADDR,output wire DDRAM_RD,DDRAM_WE,
 output wire [7:0] DDRAM_BURSTCNT,DDRAM_BE,output wire [63:0] DDRAM_DIN,
 output wire stopped,initialized,owned,enabled,rx_ready,tx_valid,wire_step,
 output wire [31:0] fault,errors,good,bad,dropped,rx_bad,
 output wire [63:0] epoch
);
localparam MACHINE_SYSPER=4;
wire [35:0] ethernet_ext_bus;
assign ethernet_ext_bus[35:33]={1'b0,io_enable,io_strobe};
assign ethernet_ext_bus[31:16]=io_din;
assign io_dout=ethernet_ext_bus[15:0];assign io_selected=ethernet_ext_bus[32];
''' +prefix+adapters+'''
assign stopped=net_ddr_stopped;assign initialized=net_ddr_initialized;
assign owned=net_ddr_owned;assign enabled=net_ddr_enable;
assign fault=net_ddr_fault;assign errors=net_ddr_control_errors;
assign epoch=net_ddr_epoch;assign rx_ready=net_rx_ready;assign tx_valid=net_tx_valid;
assign good=net_tx_good;assign bad=net_tx_bad;assign dropped=net_tx_dropped;
assign rx_bad=net_rx_bad;assign wire_step=network_step;
endmodule
'''

def run(out,mutations):
    out.mkdir(parents=True,exist_ok=True)
    source=(ROOT/'fpga/mister/Dorado.sv').read_text()
    cases=[('production',source,None)]
    if mutations:
        for name,before,after,diagnostic in (
            ('wrong_mux','assign net_rx_wr=net_ddr_owned ? net_ddr_rx_wr : net_spi_rx_wr;',
             'assign net_rx_wr=net_ddr_owned ? net_spi_rx_wr : net_ddr_rx_wr;','serialized payload/CRC differs'),
            ('ungated_wire','always @(negedge clk_sys) network_step <= machine_enable;',
             'always @(negedge clk_sys) network_step <= 1\'b1;','wire advanced while paused'),
        ):
            assert source.count(before)==1
            cases.append((name,source.replace(before,after),diagnostic))
    host=out/'host.o'
    subprocess.run([os.environ.get('CC','cc'),'-std=c11','-O2','-Wall','-Wextra','-Werror',
                    '-c',str(HERE/'dorado_ddr_ring.c'),'-o',str(host)],check=True)
    results=[]
    for name,text,diagnostic in cases:
        case=out/name;case.mkdir(exist_ok=True)
        top=case/'fixture.sv';top.write_text(fixture(text))
        command=['verilator','--cc','--exe','--build','-j','4','--top-module','dorado_ddr_wire_fixture',
                 '-Wno-fatal','-Wno-PROCASSINIT','--Mdir',str(case/'obj'),str(top),
                 *[str(ROOT/'fpga/mister'/n) for n in ('dorado_ethernet_ddr.sv','dorado_ethernet_uio.sv','dorado_ethernet_wire.sv')],
                 str(HERE/'test_ddr_wire.cpp'),'-CFLAGS',f'-std=c++17 -I{HERE}','-LDFLAGS',str(host)]
        (case/'command.json').write_text(json.dumps(command,indent=2)+'\n')
        with (case/'build.log').open('w') as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run([str(case/'obj/Vdorado_ddr_wire_fixture')],stdout=subprocess.PIPE,stderr=subprocess.STDOUT,text=True,timeout=90)
        (case/'test.txt').write_text(result.stdout)
        if diagnostic:assert result.returncode and diagnostic in result.stdout,(name,result.returncode,result.stdout)
        else:assert result.returncode==0 and 'DDR WIRE PASS' in result.stdout,result.stdout
        results.append(dict(case=name,returncode=result.returncode,expected_failure=diagnostic,
                            fixture_sha256=hashlib.sha256(top.read_bytes()).hexdigest()))
        print(name+': '+result.stdout.strip(),flush=True)
    (out/'results.json').write_text(json.dumps(dict(production_source_sha256=hashlib.sha256(source.encode()).hexdigest(),
        scope='Production wrapper wiring/packet adapters/C endpoint, independent DDR+serial oracles; no CPU or DskEth board in this fixture',cases=results),indent=2)+'\n')

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path);p.add_argument('--mutations',action='store_true');args=p.parse_args()
    if args.output:run(args.output.resolve(),args.mutations)
    else:
        with tempfile.TemporaryDirectory(prefix='dorado-ddr-wire-',dir='/tmp') as tmp:run(Path(tmp),args.mutations)
