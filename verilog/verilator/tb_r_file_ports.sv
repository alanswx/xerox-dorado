`timescale 1ns/1ps
`default_nettype none
module tb_r_file_ports;
 reg host_clk=0,advance=1,fixture_load=0;
 reg [1:0] write_n=3;
 reg [31:0] address=0;
 reg [3:0] select_n=15;
 reg [15:0] write_data=0;
 wire [15:0] read_data,ref_data;
 wire [1:0] read_parity,ref_parity,generated_parity;
 wire [9215:0] reference_state;
 dorado_r_file_ports dut(host_clk,advance,write_n,address,select_n,write_data,read_data,read_parity);
 r_file_ports_reference original(fixture_load,9216'b0,advance,write_n,address,
     select_n,write_data,ref_data,ref_parity,reference_state,generated_parity);
 integer checks=0,writes=0,paused=0,faults=0,both_reads=0;
 reg [255:0] addresses_seen[0:3];
 reg [15:0] selects_seen=0;
 task automatic check;
   if(read_data!==ref_data || read_parity!==ref_parity)
     $fatal(1,"R file port mismatch a=%h sel=%b we=%b advance=%b data=%h/%h parity=%b/%b",
         address,select_n,write_n,advance,read_data,ref_data,read_parity,ref_parity);
   if(generated_parity!=={~^write_data[15:8],~^write_data[7:0]})
     $fatal(1,"Original parity generator/source mismatch");
   checks=checks+1;selects_seen[select_n]=1;
   if(select_n[1:0]==0 || select_n[3:2]==0) both_reads=both_reads+1;
 endtask
 task automatic transact(input [31:0] a,input [3:0] s,input [1:0] w,
                         input [15:0] data,input enabled);
   // Close WE before changing data/address/select. The port fixture does not
   // stand in for the processor address/write-window timing proof.
   write_n=3;#20;
   address=a;select_n=s;write_data=data;advance=enabled;#20;
   write_n=w;#20;host_clk=1;#20;host_clk=0;#20;
   check();write_n=3;#20;
   if(w!=3) begin if(enabled) writes=writes+1;else paused=paused+1;end
 endtask
 function automatic [31:0] physical_addresses(input integer i);
   physical_addresses={8'(i^8'ha5),8'(i+17),8'(255-i),8'(i)};
 endfunction
 task automatic flip_parity(input integer bank,input integer slot);
   case(bank)
     0:begin dut.rm_h[slot][8]=~dut.rm_h[slot][8];original.h.j08.state_q[slot]=~original.h.j08.state_q[slot];end
     1:begin dut.stk_h[slot][8]=~dut.stk_h[slot][8];original.h.i08.state_q[slot]=~original.h.i08.state_q[slot];end
     2:begin dut.rm_l[slot][8]=~dut.rm_l[slot][8];original.l.j08.state_q[slot]=~original.l.j08.state_q[slot];end
     3:begin dut.stk_l[slot][8]=~dut.stk_l[slot][8];original.l.i08.state_q[slot]=~original.l.i08.state_q[slot];end
   endcase
 endtask
 reg [15:0] saved_data;reg [1:0] saved_parity;
 reg [31:0] rng=32'h9715c4ab;
 initial begin
   for(integer b=0;b<4;b=b+1) addresses_seen[b]=0;
   #20;fixture_load=1;#20;fixture_load=0;#20;
   // Initialize the candidate ONLY through real write ports, with unique
   // bank/address data. The reference's load port is private fixture seeding.
   for(integer b=0;b<4;b=b+1)
     for(integer i=0;i<256;i=i+1) begin
       transact({4{8'(i)}},~(4'b1<<b),~(2'b1<<(b/2)),
                {8'(i*37+b*19+7),8'(i*23+b*43+11)},1);
       addresses_seen[b][i]=1;
     end
   for(integer i=0;i<256;i=i+1)
     for(integer s=0;s<16;s=s+1) transact(physical_addresses(i),4'(s),3,16'(i*193+s),1);
   // Independent half enables, simultaneous selection and all address bits.
   for(integer i=0;i<256;i=i+1) begin
     transact(physical_addresses(i),0,2'(i),16'(i*317+9),1);
     transact(physical_addresses(i),0,3,0,1);
   end
   // The global native advance gate must suppress writes, even if its port
   // inputs move. This is the FPGA pause contract, not a claim about SRAM CEs.
   for(integer i=0;i<128;i=i+1)
     transact(physical_addresses(i*7),4'(i),0,16'(i*107+16'h5a19),0);
   // Every stored parity bit can fail independently of its byte. Injection
   // touches simulation memories only; there is no DUT injection/scan port.
   for(integer b=0;b<4;b=b+1)
     for(integer i=0;i<256;i=i+1) begin
       transact({4{8'(i)}},~(4'b1<<b),3,0,1);
       saved_data=read_data;saved_parity=read_parity;
       flip_parity(b,i);#20;check();
       if(read_data!==saved_data || read_parity!==(saved_parity^(2'b1<<(b/2))))
         $fatal(1,"R file parity injection changed data or wrong parity bank");
       flip_parity(b,i);#20;check();faults=faults+1;
     end
   for(integer i=0;i<4096;i=i+1) begin
     rng=rng^(rng<<13);rng=rng^(rng>>17);rng=rng^(rng<<5);
     transact(rng,4'(rng>>27),2'(rng>>24),rng[15:0],rng[26]);
   end
   for(integer b=0;b<4;b=b+1)
     if(addresses_seen[b]!=={256{1'b1}}) $fatal(1,"R file address coverage incomplete");
   if(selects_seen!=16'hffff || faults!=1024 || paused<128 || writes<1000 || both_reads<1000)
     $fatal(1,"R file coverage incomplete");
   $display("R_FILE_PORTS PASS checks=%0d writes=%0d paused=%0d injected=%0d select_masks=%h both_reads=%0d",
       checks,writes,paused,faults,selects_seen,both_reads);
   $finish;
 end
endmodule
`default_nettype wire
