// Public-interface oracle against the qualified unsampled group. Requests
// are accepted immediately, including the first host edge after a machine
// edge changes the registered address. No native/guest state is planted.
`timescale 1ns/1ps
module tb_storage_request_sample;
  reg clk=0;always #10 clk=~clk;
  reg reset=1,step=0;
  initial begin repeat(8)@(negedge clk);reset=0;end
  wire machine_clk=clk&&step;
  reg [15:0] addr=0;
  wire [15:0] addr_next=addr+16'd17;
  always @(posedge machine_clk)addr<=addr_next;
  wire [1:0] ready,request,write_request,response_ready;
  wire [31:0] request_addr,request_data,data;
  reg response=0;reg [15:0] response_data=0;
  integer accepts=0,cycles=0;
  always @(negedge clk)step<=!reset&&ready[0];
  dorado_storage_group_original #(.ADDR_BITS(16),.WIDTH(16)) golden(
    .host_clk(clk),.machine_clk(machine_clk),.reset(reset),.machine_step(step),
    .addr_after_edge(addr_next),.addr(addr),.ras_n(1'b1),.cas_n(1'b1),.we_n(1'b1),.data_in(16'd0),
    .data_out(data[15:0]),.ready(ready[0]),.req_valid(request[0]),.req_ready(1'b1),
    .req_write(write_request[0]),.req_addr(request_addr[15:0]),.req_data(request_data[15:0]),
    .rsp_valid(response),.rsp_ready(response_ready[0]),.rsp_data(response_data));
  dorado_storage_group #(.ADDR_BITS(16),.WIDTH(16),.HOST_ADDR_SAMPLE(1)) candidate(
    .host_clk(clk),.machine_clk(machine_clk),.reset(reset),.machine_step(step),
    .addr_after_edge(addr_next),.addr(addr),.ras_n(1'b1),.cas_n(1'b1),.we_n(1'b1),.data_in(16'd0),
    .data_out(data[31:16]),.ready(ready[1]),.req_valid(request[1]),.req_ready(1'b1),
    .req_write(write_request[1]),.req_addr(request_addr[31:16]),.req_data(request_data[31:16]),
    .rsp_valid(response),.rsp_ready(response_ready[1]),.rsp_data(response_data));
  always @(posedge clk)begin
    response<=0;cycles<=cycles+1;
    if(!reset&&request[0])begin
      accepts<=accepts+1;response<=1;response_data<=request_addr[15:0]^16'ha53c;
    end
  end
  always @(negedge clk)if(!reset)begin
    if(ready[0]!==ready[1] || request[0]!==request[1] || write_request[0]!==write_request[1] ||
       response_ready[0]!==response_ready[1] || request_addr[15:0]!==request_addr[31:16] ||
       request_data[15:0]!==request_data[31:16] || data[15:0]!==data[31:16])
      $fatal(1,"STORAGE_REQUEST_ORACLE public interface differs from original group");
    if(cycles==2000)begin
      if(accepts<100)$fatal(1,"too few immediate-accept requests");
      $display("STORAGE_REQUEST_UNIT PASS: %0d immediate-accept requests, public interface/epochs identical",accepts);
      $finish;
    end
  end
endmodule
