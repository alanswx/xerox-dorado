#!/usr/bin/env python3
"""Matched actual-width 2125 SRAM fit; no FPGA programming or production edits."""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess


def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def prepare(output):
    from fit_f10415 import AUDIT
    from fit_f10415_enable import SDC
    from audit_baseboard_ram import CELL_SHA,CORE_SHA
    here=Path(__file__).resolve().parent;root=here.parents[2]
    assert digest(root/'verilog/cells/cell_i2125.v')==CELL_SHA
    assert digest(here/'f10415_word_bank.sv')==CORE_SHA
    output.mkdir(parents=True,exist_ok=False)
    top='''module snapshot_fit(
 input wire host_clk,input wire [3:0] advance,ce_n,we_n,freeze,held,request,write,
 input wire [39:0] address,input wire [31:0] data,input wire [71:0] state_data,
 input wire [43:0] word_index,output reg [31:0] q,output reg [71:0] result,
 output reg [3:0] frozen,done,error);
 // Same opposite-edge launch/capture shell and10ns embedded paths.
 // Independent controls preserve four separately writable byte groups.
'''
    for i in range(4):
        top+=f'''
 reg [53:0] launch_{i};
 always @(negedge host_clk)launch_{i}<=
  {{advance[{i}],address[{i*10}+:10],data[{i*8}+:8],ce_n[{i}],we_n[{i}],
    freeze[{i}],held[{i}],request[{i}],write[{i}],word_index[{i*11}+:11],state_data[{i*18}+:18]}};
 wire step_{i},ce_{i},we_{i},freeze_{i},held_{i},request_{i},write_{i};
 wire [9:0] address_{i};wire [7:0] data_{i};wire [17:0] state_{i};wire [10:0] word_{i};
 assign {{step_{i},address_{i},data_{i},ce_{i},we_{i},freeze_{i},held_{i},request_{i},write_{i},word_{i},state_{i}}}=launch_{i};
 wire [7:0] q_{i};wire [17:0] result_{i};wire frozen_{i},done_{i},error_{i};
 always @(negedge host_clk)
  {{q[{i*8}+:8],frozen[{i}],done[{i}],error[{i}],result[{i*18}+:18]}}<=
  {{q_{i},frozen_{i},done_{i},error_{i},result_{i}}};
`ifdef SNAPSHOT_BANK_EXPERIMENT
 dorado_snapshot_i2125_byte_bank bank_{i}(
  .host_clk(host_clk),.guest_advance(step_{i}),.address(address_{i}),.data(data_{i}),
  .ce_n(ce_{i}),.we_n(we_{i}),.q(q_{i}),.freeze(freeze_{i}),.guest_held(held_{i}),.frozen(frozen_{i}),
  .request(request_{i}),.write(write_{i}),.word_index(word_{i}),.state_data(state_{i}),
  .done(done_{i}),.error(error_{i}),.result(result_{i}));
`else
 assign frozen_{i}=0;assign done_{i}=0;assign error_{i}=0;assign result_{i}=0;
 genvar b_{i};
 generate for(b_{i}=0;b_{i}<8;b_{i}=b_{i}+1)begin:original_{i}
  cell_i2125 bit_cell(.sys_clk(host_clk),.p13(address_{i}[9]),.p12(address_{i}[8]),
   .p11(address_{i}[7]),.p10(address_{i}[6]),.p9(address_{i}[5]),.p6(address_{i}[4]),
   .p5(address_{i}[3]),.p4(address_{i}[2]),.p3(address_{i}[1]),.p2(address_{i}[0]),
   .p15(data_{i}[b_{i}]),.p1(ce_{i}),.p14(we_{i}),.p7(q_{i}[b_{i}]),.p8(1'b0),.p16(1'b1));
 end endgenerate
`endif
'''
    top+='endmodule\n'
    qsf='''set_global_assignment -name FAMILY "Cyclone V"
set_global_assignment -name DEVICE 5CSEBA6U23I7
set_global_assignment -name TOP_LEVEL_ENTITY snapshot_fit
set_global_assignment -name MIN_CORE_JUNCTION_TEMP -40
set_global_assignment -name MAX_CORE_JUNCTION_TEMP 100
set_global_assignment -name TIMEQUEST_MULTICORNER_ANALYSIS OFF
set_global_assignment -name OPTIMIZATION_MODE "HIGH PERFORMANCE EFFORT"
set_global_assignment -name OPTIMIZE_HOLD_TIMING ALL_PATHS
set_global_assignment -name NUM_PARALLEL_PROCESSORS 4
set_global_assignment -name SYSTEMVERILOG_FILE top.sv
set_global_assignment -name SYSTEMVERILOG_FILE i2125_byte_bank.sv
set_global_assignment -name SYSTEMVERILOG_FILE f10415_word_bank.sv
set_global_assignment -name VERILOG_FILE cell_i2125.v
set_global_assignment -name SDC_FILE bank.sdc
set_location_assignment PIN_V11 -to host_clk
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to host_clk
'''
    for port in ('advance','ce_n','we_n','freeze','held','request','write','address','data','state_data','word_index','q','result','frozen','done','error'):
        qsf+='set_instance_assignment -name VIRTUAL_PIN ON -to '+port+'[*]\n'
    for variant in ('baseline','snapshot'):
        case=output/variant;case.mkdir()
        for p in [here/'i2125_byte_bank.sv',here/'f10415_word_bank.sv',root/'verilog/cells/cell_i2125.v']:
            shutil.copyfile(p,case/p.name)
        for name,text in {'top.sv':top,'bank.sdc':SDC,'audit.tcl':AUDIT,
            'snapshot_fit.qpf':'PROJECT_REVISION = "snapshot_fit"\n',
            'snapshot_fit.qsf':qsf+('set_global_assignment -name VERILOG_MACRO "SNAPSHOT_BANK_EXPERIMENT=1"\n'if variant=='snapshot'else'')}.items():
            (case/name).write_text(text)
    sources=[Path(__file__).resolve(),here/'i2125_byte_bank.sv',here/'f10415_word_bank.sv',root/'verilog/cells/cell_i2125.v',
             here/'fit_f10415.py',here/'fit_f10415_enable.py',here/'audit_baseboard_ram.py',here/'audit_contb_im.py']
    executed=output/'executed-sources';executed.mkdir()
    for p in sources:shutil.copyfile(p,executed/p.name)
    record=dict(scope='Four independent1024x8 2125 banks,32768RAM+32rawread bits; original full-advance vs exact-advance/state-access, same50MHz negative-edge launch/capture shell and10ns embedded paths,2ns max0ns min virtual I/O,no cuts. Counts include shell/admin/pruning; no complete BaseBoard/global barrier/physical snapshot.',
        widths=[8]*4,modeled_state_bits=32800,builder_sha256=digest(Path(__file__)),
        source_hashes={str(p.relative_to(root)):digest(p)for p in sources},
        compiled_files={str(p.relative_to(output)):digest(p)for p in output.glob('*/*')if p.is_file()},
        expected_qsf={v:(output/v/'snapshot_fit.qsf').read_text()for v in ('baseline','snapshot')})
    (output/'provenance.json').write_text(json.dumps(record,indent=2)+'\n')


def run(directory,quartus):
    record=json.loads((directory/'provenance.json').read_text())
    assert digest(Path(__file__))==record['builder_sha256'],'builder differs from preparation'
    for name,expected in record['compiled_files'].items():assert digest(directory/name)==expected,'prepared input differs: '+name
    for variant in ('baseline','snapshot'):
        case=directory/variant;assert not(case/'map.log').exists(),'inspect existing job; do not restart'
        for tool,args,label in [('quartus_map',['snapshot_fit'],'map'),('quartus_fit',['snapshot_fit'],'fit'),
                                ('quartus_sta',['snapshot_fit'],'sta'),('quartus_sta',['-t','audit.tcl'],'corners')]:
            print(variant,label,flush=True)
            with (case/(label+'.log')).open('w')as log:subprocess.run([str(quartus/tool),*args],cwd=case,stdout=log,stderr=subprocess.STDOUT,check=True)
    print('2125 SRAM fits terminal; audit inference/all corners before qualification.',flush=True)



if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--prepare',type=Path)
    parser.add_argument('--run',type=Path)
    parser.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'))
    args=parser.parse_args()
    if args.prepare:prepare(args.prepare.resolve())
    elif args.run:run(args.run.resolve(),args.quartus)
    else:parser.error('Specify --prepare or --run')
