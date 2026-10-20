#!/usr/bin/env python3
"""Snapshot continuation through actual production mux/UIO/DDR/serial-wire RTL.

Enable the new pause only in the extracted fixture. Observe every sequential
bit and both packet RAMs; Main ordering, physical DDR visibility and complete
machine restore remain separate qualification requirements.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import xml.etree.ElementTree as ET
import test_ddr_wire as wire
import test_network_snapshot as engine

HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[2]


def inverse_uio(text):
    text=engine.once(text,'    parameter integer DDR_CAPABLE=0,\n    parameter integer SNAPSHOT_PAUSE=0',
        '    parameter integer DDR_CAPABLE=0')
    text=engine.once(text,'    input wire snapshot_hold,\n    output wire snapshot_idle,\n    output wire snapshot_held,\n','')
    block='''  // Latch only between transactions. While held, even another EXT_BUS
  // decoder's snapshot traffic must not mutate this parser or issue data IO.
  // Release only with the shared bus deselected, preserving framing.
  reg snapshot_paused=0;
  assign snapshot_held=(SNAPSHOT_PAUSE!=0) && snapshot_paused;
  assign snapshot_idle=(SNAPSHOT_PAUSE!=0) && snapshot_hold && snapshot_paused &&
      !reset && !tx_ack && !rx_wr && !rx_commit;
'''
    text=engine.once(text,block,'');text=engine.once(text,'      snapshot_paused<=0;\n','')
    text=engine.once(text,'''    end else if(snapshot_paused)begin
      if(!snapshot_hold && !enable)snapshot_paused<=0;
    end else begin
      if((SNAPSHOT_PAUSE!=0) && snapshot_hold && !enable && !receiving &&
         !ddr_control && !tx_ack && !rx_wr && !rx_commit)snapshot_paused<=1;
      if(!enable)begin''','    end else if(!enable)begin')
    return engine.once(text,'    end\n    end\n  end\nendmodule','    end\n  end\nendmodule')


def run(out,barrier=False):
    out.mkdir(parents=True,exist_ok=False);inputs=out/'executed-inputs';inputs.mkdir()
    names=('Dorado.sv','dorado_ethernet_ddr.sv','dorado_ethernet_uio.sv','dorado_ethernet_wire.sv')
    for name in names:(inputs/name).write_bytes((ROOT/'fpga/mister'/name).read_bytes())
    old_uio=engine.raw_receipt(ROOT/'fpga/mister/reports/2026-10-20-snapshot-native-memory-pause-rejected','terminal-fit/src/fpga/mister/dorado_ethernet_uio.sv')
    assert inverse_uio((inputs/'dorado_ethernet_uio.sv').read_text()).encode()==old_uio,'unapproved normal UIO edit'
    (inputs/'original-uio.sv').write_bytes(old_uio)
    source=(inputs/'Dorado.sv').read_text();top=wire.fixture(source)
    top=engine.once(top,' input wire clk_sys,pll_locked,machine_enable,ethernet_tx_n,',
        ' input wire snapshot_hold,output wire snapshot_idle,\n input wire clk_sys,pll_locked,machine_enable,ethernet_tx_n,')
    top=engine.once(top,'dorado_ethernet_ddr u_network_ddr (','dorado_ethernet_ddr #(.SNAPSHOT_PAUSE(1)) u_network_ddr (')
    top=engine.once(top,".enable(net_ddr_enable),.epoch(net_ddr_epoch),\n    .snapshot_hold(1'b0),.snapshot_idle(),",'.enable(net_ddr_enable),.epoch(net_ddr_epoch),\n    .snapshot_hold(snapshot_hold),.snapshot_idle(snapshot_ddr_idle),')
    top=engine.once(top,'dorado_ethernet_uio #(.MAX_WORDS(1024)','dorado_ethernet_uio #(.MAX_WORDS(1024),.SNAPSHOT_PAUSE(1)')
    top=engine.once(top,".bus(ethernet_ext_bus),\n    .snapshot_hold(1'b0),.snapshot_idle(),",'.bus(ethernet_ext_bus),\n    .snapshot_hold(snapshot_hold),.snapshot_idle(snapshot_uio_idle),')
    top=engine.once(top,'wire [35:0] ethernet_ext_bus;','wire snapshot_ddr_idle,snapshot_uio_idle;\nassign snapshot_idle=snapshot_ddr_idle && snapshot_uio_idle;\nwire [35:0] ethernet_ext_bus;')
    if barrier:
        top=engine.once(top,' input wire snapshot_hold,output wire snapshot_idle,',
            ''' input wire snapshot_hold,output wire snapshot_idle,
 input wire machine_reseed,service_abort,service_fault,
 output wire service_hold,parser_held,service_faulted,pause_drained,''')
        top=engine.once(top,'assign snapshot_idle=snapshot_ddr_idle && snapshot_uio_idle;',
            '''wire parser_hold;
assign pause_drained=u_network_pause.drained;
dorado_network_pause_barrier u_network_pause(
 .clk(clk_sys),.machine_reseed(machine_reseed),.owner_hold(snapshot_hold),
 .guest_held(service_hold && !network_step),.abort(service_abort),
 .device_reset(!pll_locked),.device_fault(service_fault || (|net_ddr_fault) || (|net_ddr_control_errors)),
 .engine_idle(snapshot_ddr_idle),.parser_idle(snapshot_uio_idle),.parser_held(parser_held),
 .hold(service_hold),.parser_hold(parser_hold),.ready(snapshot_idle),.fault(service_faulted));''')
        top=engine.once(top,'always @(negedge clk_sys) network_step <= machine_enable;',
            'always @(negedge clk_sys) network_step <= machine_enable && !service_hold;')
        top=engine.once(top,'.snapshot_hold(snapshot_hold),.snapshot_idle(snapshot_ddr_idle),',
            '.snapshot_hold(service_hold),.snapshot_idle(snapshot_ddr_idle),')
        top=engine.once(top,'.snapshot_hold(snapshot_hold),.snapshot_idle(snapshot_uio_idle),.snapshot_held(),',
            '.snapshot_hold(parser_hold),.snapshot_idle(snapshot_uio_idle),.snapshot_held(parser_held),')
        (inputs/'network_pause_barrier.sv').write_bytes((ROOT/'fpga/mister/snapshot/network_pause_barrier.sv').read_bytes())
    (inputs/'fixture.sv').write_text(top)
    rtl=[str(inputs/n)for n in names[1:]]
    if barrier:rtl.append(str(inputs/'network_pause_barrier.sv'))
    cmd=['verilator','--xml-only','--top-module','dorado_ddr_wire_fixture','-Wno-fatal','--xml-output',str(out/'fixture.xml'),str(inputs/'fixture.sv'),*rtl]
    (out/'xml-command.json').write_text(json.dumps(cmd,indent=2)+'\n')
    with(out/'xml.log').open('w')as log:subprocess.run(cmd,stdout=log,stderr=subprocess.STDOUT,check=True)
    raw=(out/'fixture.xml').read_bytes();data=engine.inventory.inventory(raw);tree=ET.fromstring(raw)
    observed=[];review=[];bits=0;arrays=[]
    modules={m.get('name'):m for m in tree.findall('.//module')}
    for row in data['candidates']:
        name=row['path'].removeprefix('dorado_ddr_wire_fixture.')
        if row['category']=='subroutine_review'and '.@'in name:
            assert name.startswith('u_network_wire.@')and name.endswith('.crc_bit')and row['bits']==16
            module=modules[row['module']];func=module.findall('./func');assert len(func)==1 and func[0].get('name')=='crc_bit'
            local={v.get('name')for v in func[0].findall('.//var')};assert not func[0].findall('.//assigndly')
            for assign in func[0].iter('assign'):assert engine.inventory.destinations(assign[1])<=local
            assert 'function automatic [15:0] crc_bit'in(inputs/'dorado_ethernet_wire.sv').read_text()
            review.append(dict(path=row['path'],bits=16,reason='Automatic function return, no global writes/NBAs.'));continue
        if row['category']not in('sequential','subroutine_review'):continue
        if row['category']=='subroutine_review':
            assert name in{'u_network_ddr.'+n for n in('ddram_addr','ddram_be','ddram_wdata','return_state')}
        if '.@'in name:
            assert name.startswith('u_network_ddr.@')and name.endswith('.updated')and row['bits']==64
            name='u_network_ddr.pack_word.updated'
        assert name=='network_step'or name.startswith(('u_network_ddr.','u_network_wire.','u_network_uio.','u_network_pause.'))
        if row['unpacked_dimensions']:
            assert name in('u_network_wire.rx_buffer','u_network_wire.tx_buffer')and row['width']==16 and row['unpacked_dimensions']==[[0,1023]]
            arrays.append(name);observed.extend(name+'['+str(i)+']'for i in range(1024))
        else:observed.append(name)
        bits+=row['bits']
    assert sorted(arrays)==['u_network_wire.rx_buffer','u_network_wire.tx_buffer']and len(review)==1 and bits>33000
    description=dict(observed_bits=bits,packet_ram_bits=32768,function_scratch_review=review,sequential_exclusions=[],observed_expressions=observed)
    (out/'state-review.json').write_text(json.dumps(description,indent=2)+'\n')
    top=engine.once(top,'module dorado_ddr_wire_fixture(',f'module dorado_ddr_wire_fixture(\n output wire [{bits-1}:0] observed_state,')
    top=engine.once(top,'endmodule','''// Test-only arbitrary power-up contents, including unused packet words.
initial begin
 for(integer i=0;i<1024;i++)begin
  u_network_wire.rx_buffer[i]=16'((i*16'h9e37)^16'ha36c);
  u_network_wire.tx_buffer[i]=16'((i*16'h531b)^16'h6ca3);
 end
end
assign observed_state={\n'''+',\n'.join(observed)+'};\nendmodule')
    legacy=(HERE/'test_ddr_wire.cpp').read_text();legacy=engine.once(legacy,'t.clk_sys=0;t.pll_locked=0;','t.snapshot_hold=0;t.clk_sys=0;t.pll_locked=0;')
    if barrier:legacy=engine.once(legacy,'t.snapshot_hold=0;',
        't.machine_reseed=0;t.service_abort=0;t.service_fault=0;t.snapshot_hold=0;')
    if barrier:
        legacy=engine.once(legacy,'uint64_t cycles=0;','uint64_t cycles=0;unsigned pause_stable=0;')
        legacy=engine.once(legacy,'t.clk_sys=1;t.eval();if(record',
            '''t.clk_sys=1;t.eval();
        pause_stable=t.pause_drained?pause_stable+1:0;
        if(t.snapshot_idle)check(pause_stable>=4,"acknowledged before two settling clocks");
        if(record''')
    legacy=engine.once(legacy,'m.reads,m.writes,m.holds,m.immediate);\n}', 'm.reads,m.writes,m.holds,m.immediate);\nreturn 0;\n}')
    (inputs/'legacy_wire_test.cpp').write_text(legacy)
    for name in('dorado_ddr_ring.c','dorado_ddr_ring.h','test_network_snapshot_wire.cpp','test_network_snapshot_wire.py','test_ddr_wire.cpp','test_ddr_wire.py'):
        (inputs/name).write_bytes((HERE/name).read_bytes())
    host=out/'host.o';cc=[os.environ.get('CC','cc'),'-std=c11','-O2','-Wall','-Wextra','-Werror','-I'+str(inputs),'-c',str(inputs/'dorado_ddr_ring.c'),'-o',str(host)]
    (out/'c-command.json').write_text(json.dumps(cc,indent=2)+'\n');subprocess.run(cc,check=True,capture_output=True)
    gate='always @(negedge clk_sys) network_step <= machine_enable'+(' && !service_hold'if barrier else'')+';'
    cases=[('candidate',top,None),('ungated_guest',engine.once(top,gate,"always @(negedge clk_sys) network_step <= 1'b1;"),
        'engine did not drain to snapshot pause'if barrier else'held network state changed')]
    results=[]
    uio=(inputs/'dorado_ethernet_uio.sv').read_text()
    cases.extend([('uio_unheld',top,'snapshot SPI changed held network state'),
                  ('uio_early',top,'pause truncated active SPI transaction'),
                  ('uio_release',top,'parser released inside shared SPI packet')])
    if barrier:
        cases.extend([('early_ready',top,'acknowledged before two settling clocks'),
                      ('early_guest_release',top,'guest resumed before parser release'),
                      ('nonsticky_fault',top,'fault did not retain global hold')])
    for label,fixture,diagnostic in cases:
        case=out/label;case.mkdir();(case/'fixture.sv').write_text(fixture)
        case_rtl=rtl
        if label in('early_ready','early_guest_release','nonsticky_fault'):
            control=(inputs/'network_pause_barrier.sv').read_text()
            if label=='early_ready':control=engine.once(control,"phase==FROZEN && (owner_hold===1'b1)","(phase==SETTLE || phase==FROZEN) && (owner_hold===1'b1)")
            elif label=='early_guest_release':control=engine.once(control,'assign hold=requested || phase!=RUN;',"assign hold=requested || (phase!=RUN && phase!=RELEASE);")
            else:control=control.replace('phase<=QUARANTINE;','phase<=RUN;')
            (case/'network_pause_barrier.sv').write_text(control)
            case_rtl=[str(case/'network_pause_barrier.sv')if Path(p).name=='network_pause_barrier.sv'else p for p in rtl]
        if label.startswith('uio_'):
            if label=='uio_unheld':damaged=engine.once(uio,'end else if(snapshot_paused)begin',"end else if(1'b0)begin")
            elif label=='uio_early':damaged=engine.once(uio,'snapshot_hold && !enable && !receiving','snapshot_hold && !receiving')
            else:damaged=engine.once(uio,'if(!snapshot_hold && !enable)snapshot_paused<=0;','if(!snapshot_hold)snapshot_paused<=0;')
            (case/'dorado_ethernet_uio.sv').write_text(damaged)
            case_rtl=[str(case/'dorado_ethernet_uio.sv')if Path(p).name=='dorado_ethernet_uio.sv'else p for p in rtl]
        cmd=['verilator','--cc','--exe','--build','-j','4','--top-module','dorado_ddr_wire_fixture','-Wno-fatal','-Wno-PROCASSINIT','--Mdir',str(case/'obj'),str(case/'fixture.sv'),*case_rtl,
             str(inputs/'test_network_snapshot_wire.cpp'),'-CFLAGS',f'-std=c++17 -I{inputs} -DNETWORK_STATE_WORDS={(bits+31)//32} -DNETWORK_STATE_BITS={bits}'+(' -DNETWORK_PAUSE_BARRIER=1'if barrier else''),'-LDFLAGS',str(host)]
        (case/'command.json').write_text(json.dumps(cmd,indent=2)+'\n')
        with(case/'build.log').open('w')as log:subprocess.run(cmd,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run([str(case/'obj/Vdorado_ddr_wire_fixture')],capture_output=True,text=True,timeout=90)
        text=result.stdout+result.stderr;(case/'test.log').write_text(text)
        if diagnostic:assert result.returncode==23 and diagnostic in text,text
        else:assert not result.returncode and'NETWORK WIRE SNAPSHOT PASS'in text and'DDR WIRE PASS'in text,text
        print(label+': '+(diagnostic or text.splitlines()[-1]),flush=True)
        results.append(dict(case=label,returncode=result.returncode,expected_failure=diagnostic,log_sha256=engine.sha((case/'test.log').read_bytes())))
    (out/'qualification.json').write_text(json.dumps(dict(status='PASS',scope=__doc__,state_review=description,cases=results,inputs={p.name:engine.sha(p.read_bytes())for p in inputs.iterdir()},
        new_required_state_bits=6 if barrier else 1,real_network_pause_barrier=barrier,
        limits='Actual network controller/guest gate'if barrier else'External guest gate and manual settling',
        incomplete='Not Main coordination, native memory integration, physical DDR visibility, state import or complete restore.'),indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--barrier',action='store_true');a=p.parse_args();run(a.output.resolve(),a.barrier)
