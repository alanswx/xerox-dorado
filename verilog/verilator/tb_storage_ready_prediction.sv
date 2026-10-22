// Temporal equivalence against the frozen native group and original address cell.
// Full original-chip/physical-memory comparison is a separate board test.
`timescale 1ns/1ps
module tb_storage_ready_prediction #(parameter integer WIDTH=12);
 reg clk=0;always #5 clk=~clk;
 reg run=0,reset=1,ras_n=1,cas_n=1,we_n=1;
 wire machine_clk=clk && run;
 reg [7:0] a=0;
 reg [WIDTH-1:0] data_in=0;
 wire [15:0] gold_addr,dut_addr,after_addr;
 cell_MK4096P_addr #(.ROW_BITS(8)) address_gold(.sys_clk(machine_clk),.p4(ras_n),.p15(cas_n),.a(a),.addr(gold_addr));
 cell_MK4096P_addr_preview #(.ROW_BITS(8)) address_dut(.sys_clk(machine_clk),.p4(ras_n),.p15(cas_n),.a(a),.addr(dut_addr),.addr_after_edge(after_addr));
 wire gr,gv,gw,gs,dr,dv,dw,ds;
 wire [15:0] ga,gd,da,dd;
 wire [WIDTH-1:0] go_,do_;
 reg req_ready=0,rsp_valid=0;
 reg [15:0] rsp_data=0;
 reference_storage_group #(.ADDR_BITS(16),.WIDTH(WIDTH)) gold(
  .host_clk(clk),.machine_clk(machine_clk),.reset(reset),.ras_n(ras_n),.cas_n(cas_n),.we_n(we_n),.addr(gold_addr),.data_in(data_in),.data_out(go_),
  .ready(gr),.req_valid(gv),.req_ready(req_ready),.req_write(gw),.req_addr(ga),.req_data(gd),.rsp_valid(rsp_valid),.rsp_ready(gs),.rsp_data(rsp_data));
 dorado_storage_group #(.ADDR_BITS(16),.WIDTH(WIDTH)) dut(
  .host_clk(clk),.machine_clk(machine_clk),.reset(reset),.machine_step(run),.addr_after_edge(after_addr),.ras_n(ras_n),.cas_n(cas_n),.we_n(we_n),.addr(dut_addr),.data_in(data_in),.data_out(do_),
  .ready(dr),.req_valid(dv),.req_ready(req_ready),.req_write(dw),.req_addr(da),.req_data(dd),.rsp_valid(rsp_valid),.rsp_ready(ds),.rsp_data(rsp_data));
 reg [15:0] memory_[0:65535];
 reg pending=0;
 reg [15:0] saved=0;
 integer latency=0,ticks=0,checks=0,steps=0,reads=0,writes=0,paused_responses=0,reset_cycles=0;
 reg [31:0] rng=32'h5c1bca39;
 // Explicit FPGA-zero power-on fixture for the otherwise unspecified original
 // address registers. No running machine or emulator state is restored.
 initial begin
  address_gold.row=0;address_gold.col=0;address_gold.ras_d=0;address_gold.cas_d=0;
  address_dut.row=0;address_dut.col=0;address_dut.ras_d=0;address_dut.cas_d=0;
  for(integer n=0;n<65536;n++)memory_[n]=16'(n^16'hc59a);
 end
 always @(negedge clk)begin
  ticks=ticks+1;rng={rng[30:0],rng[31]^rng[21]^rng[1]^rng[0]};
  reset=(ticks<5)||(ticks%10000<3);
  run=!reset && gr && rng[5] && rng[12];
  ras_n=rng[0];cas_n=rng[2];we_n=rng[7];a=rng[23:16];data_in=WIDTH'(rng);
  req_ready=!reset && !pending && rng[9];
 end
 always @(posedge clk)begin
  if(reset)begin pending<=0;rsp_valid<=0;reset_cycles<=reset_cycles+1;end
  else begin
   if(run)steps<=steps+1;
   if(gv && req_ready)begin
    if(pending)$fatal(1,"STORAGE_READY_PREDICTION fixture multiple requests");
    pending<=1;latency<=1+int'(rng[13:10]);saved<=gw ? gd : memory_[ga];
    if(gw)begin memory_[ga]<=gd;writes<=writes+1;end
    else reads<=reads+1;
   end
   if(pending && !rsp_valid)begin
    if(latency!=0)latency<=latency-1;
    else begin rsp_valid<=1;rsp_data<=saved;end
   end
   if(rsp_valid && gs)begin
    pending<=0;rsp_valid<=0;
    if(!run)paused_responses<=paused_responses+1;
   end
  end
  #1;checks=checks+1;
  if({gr,gv,gw,ga,gd,gs,go_}!=={dr,dv,dw,da,dd,ds,do_})
   $fatal(1,"STORAGE_READY_PREDICTION group outputs differ at tick%0d",ticks);
  if({address_gold.row,address_gold.col,address_gold.ras_d,address_gold.cas_d}!=={address_dut.row,address_dut.col,address_dut.ras_d,address_dut.cas_d})
   $fatal(1,"STORAGE_READY_PREDICTION original address Q differs");
  if({gold.cas_d,gold.we_d,gold.write_pending,gold.write_data,gold.read_pipe,gold.cache_valid,gold.write_done,gold.waiting,gold.saved_write,gold.cache_addr,gold.saved_addr,gold.cache_data,gold.saved_data,gold.old_data}!==
     {dut.cas_d,dut.we_d,dut.write_pending,dut.write_data,dut.read_pipe,dut.cache_valid,dut.write_done,dut.waiting,dut.saved_write,dut.cache_addr,dut.saved_addr,dut.cache_data,dut.saved_data,dut.old_data})
   $fatal(1,"STORAGE_READY_PREDICTION original group Q differs");
 end
 initial begin
  repeat(40000)@(negedge clk);#2;
  if(steps<500 || reads<500 || writes<50 || paused_responses<500 || reset_cycles<10)
   $fatal(1,"STORAGE_READY_PREDICTION insufficient coverage: %0d %0d %0d %0d %0d",steps,reads,writes,paused_responses,reset_cycles);
  $display("STORAGE_READY_GROUP PASS width=%0d checks=%0d steps=%0d reads=%0d writes=%0d paused_responses=%0d reset_cycles=%0d",WIDTH,checks,steps,reads,writes,paused_responses,reset_cycles);
  $finish;
 end
endmodule
