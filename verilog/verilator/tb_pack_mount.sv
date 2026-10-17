// Reproduce Main: partial/nonzero img_size arrives BEFORE img_mounted.
`timescale 1ns/1ps
module tb_pack_mount #(parameter USE_MOUNT_GUARD=1);
  reg clk=0;always #1 clk=~clk;
  reg reset=1,size_nonzero=0,mounted=0;
  wire attached,guarded_attached,ready,fault,rd,wr;
  assign attached=USE_MOUNT_GUARD ? guarded_attached : size_nonzero;
  dorado_pack_mount mount(.clk(clk),.reset(reset),.size_nonzero(size_nonzero),.mounted(mounted),.attached(guarded_attached));
  dorado_pack_hps u_medium(.clk_sys(clk),.machine_clk(1'b0),.reset(reset),
    .attached(attached),.remounted(mounted),.readonly(1'b0),.tagbus_n(12'hfff),
    .cyltag_n(1'b1),.headtag_n(1'b1),.drivetag_n(1'b1),.conttag_n(1'b1),
    .selected(1'b1),.data_adv(1'b0),.pulse(16'd0),.write_data_p(1'b0),.write_data_m(1'b0),
    .ready(ready),.fault(fault),.sd_rd(rd),.sd_wr(wr),.sd_ack(1'b0),
    .sd_buff_addr(13'd0),.sd_buff_dout(16'd0),.sd_buff_wr(1'b0));
  task tick;@(negedge clk);endtask
  task notify;mounted=1;repeat(3)tick();mounted=0;repeat(3)tick();endtask
  initial begin
    repeat(3)tick();reset=0;tick();size_nonzero=1;
    repeat(20)tick();if(rd||wr||attached||fault)$fatal(1,"partial size started an early transfer");
    notify();if(!attached||!rd||fault)$fatal(1,"first mount failed");
    // A remount while a real transfer is outstanding must STILL fault.
    notify();if(!fault||ready||rd||wr)$fatal(1,"unsafe in-flight remount accepted");
    reset=1;tick();reset=0;size_nonzero=0;repeat(3)tick();
    size_nonzero=1;repeat(20)tick();
    if(rd||attached||fault)$fatal(1,"reattach started before notification");
    notify();if(!rd||fault)$fatal(1,"fresh mount after unmount failed");
    $display("PACK-MOUNT PASS: Main size/notify ordering, no early read, real interrupted transfer still faults, clean reattach");
    $finish;
  end
endmodule
