`timescale 1ns/1ps
module tb_ethernet_uio;
`ifdef LARGE_FRAMES
  localparam WORDS=1024;
`else
  localparam WORDS=512;
`endif
  localparam ADDRESS_BITS=$clog2(WORDS);
  reg clk=0;always #1 clk=~clk;
  reg reset=1,enable=0,strobe=0;
  reg [15:0] din=0;
  wire [35:0] bus;
  assign bus[35:33]={1'b0,enable,strobe};
  assign bus[31:16]=din;
  reg rx_ready=1,tx_valid=1;
  reg [15:0] tx_data=0;
  reg [31:0] machine_debug=32'h1357abcd,processor_debug=32'h02468ace;
  reg [15:0] machine_status=16'h2468;
  wire [ADDRESS_BITS-1:0] tx_addr,rx_addr;
  wire tx_ack,rx_wr,rx_commit;
  wire [15:0] rx_data;
  wire [ADDRESS_BITS:0] rx_words;
  wire [31:0] aborted;
  reg [15:0] received[0:WORDS-1];
  integer commits=0,written=0,acks=0;
  always @(posedge clk)begin
    tx_data<=16'ha500|tx_addr;
    if(tx_ack)begin tx_valid<=0;acks<=acks+1;end
    if(rx_wr)begin received[rx_addr]<=rx_data;written<=written+1;end
    if(rx_commit)commits<=commits+1;
  end
  dorado_ethernet_uio #(.MAX_WORDS(WORDS)) dut(.clk(clk),.reset(reset),.bus(bus),
    .rx_ready(rx_ready),.tx_valid(tx_valid),.tx_words(10'd17),.tx_data(tx_data),
    .processor_debug(processor_debug),.machine_debug(machine_debug),.machine_status(machine_status),
    .tx_good(32'd111),.tx_bad(32'd222),.tx_dropped(32'd333),.rx_bad(32'd444),
    .tx_addr(tx_addr),.tx_ack(tx_ack),.rx_wr(rx_wr),.rx_commit(rx_commit),
    .rx_addr(rx_addr),.rx_data(rx_data),.rx_words(rx_words),.aborted(aborted));
  task tick;@(negedge clk);endtask
  task word(input [15:0] w,output [15:0] response);
    din=w;strobe=1;tick();strobe=0;tick();response=bus[15:0];
  endtask
  task deselect;enable=0;repeat(3)tick();endtask
  reg [15:0] r;
  initial begin
    repeat(3)tick();reset=0;tick();enable=1;
    word(16'h70,r);if(r!==16'hd074||!bus[32])$fatal(1,"missing capability magic");
    // Change both counter halves/status after the command. The entire reply
    // must retain the command-time snapshot, not a mixture of two moments.
    machine_debug=32'h13580001;processor_debug=32'h0247ffff;machine_status=16'hface;
    word(0,r);if(r!==(WORDS>512?7:3))$fatal(1,"wrong status flags");
    word(0,r);if(r!==17)$fatal(1,"wrong length");
    word(0,r);if(r!==111)$fatal(1,"wrong good counter");
    word(0,r);if(r!==222)$fatal(1,"wrong CRC counter");
    word(0,r);if(r!==333)$fatal(1,"wrong dropped counter");
    word(0,r);if(r!==444)$fatal(1,"wrong bad RX counter");
    word(0,r);if(r!==0)$fatal(1,"wrong aborted count");
    word(0,r);if(r!==16'habcd)$fatal(1,"wrong debug low word");
    word(0,r);if(r!==16'h1357)$fatal(1,"wrong debug high word");
    word(0,r);if(r!==16'h2468)$fatal(1,"wrong machine status");
    word(0,r);if(r!==16'h8ace)$fatal(1,"wrong processor low word");
    word(0,r);if(r!==16'h0246)$fatal(1,"wrong processor high word");deselect();
    enable=1;word(16'h70,r);
    repeat(7)word(0,r);
    word(0,r);if(r!==16'h0001)$fatal(1,"next snapshot missing machine low");
    word(0,r);if(r!==16'h1358)$fatal(1,"next snapshot missing machine high");
    word(0,r);if(r!==16'hface)$fatal(1,"next snapshot missing status");
    word(0,r);if(r!==16'hffff)$fatal(1,"next snapshot missing processor low");
    word(0,r);if(r!==16'h0247)$fatal(1,"next snapshot missing processor high");deselect();
    enable=1;word(16'h71,r);if(r!==17)$fatal(1,"wrong TX length");
    for(integer i=0;i<17;i++)begin
      word(0,r);if(r!==(16'ha500|i))$fatal(1,"TX data timing: word %d got %h",i,r);
    end
    deselect();if(!tx_valid||acks)$fatal(1,"reading acknowledged TX early");
    enable=1;word(16'h72,r);deselect();if(tx_valid||acks!=1)$fatal(1,"TX ack missing");
    enable=1;word(16'h73,r);if(r!==1)$fatal(1,"RX not ready");word(17,r);
    for(integer i=0;i<17;i++)word(16'h5500|i,r);
    if(commits)$fatal(1,"committed before transaction ended");
    deselect();if(commits!=1||written!=17)$fatal(1,"complete RX not committed");
    for(integer i=0;i<17;i++)if(received[i]!==(16'h5500|i))$fatal(1,"RX data differs");
    // Short/overlong/busy transactions may alter an idle staging buffer,
    // but must never put a partial or overwritten packet on the cable.
    enable=1;word(16'h73,r);word(17,r);word(1,r);deselect();
    if(commits!=1||aborted!=1)$fatal(1,"short RX accepted");
    enable=1;word(16'h73,r);word(2,r);word(1,r);word(2,r);word(3,r);deselect();
    if(commits!=1||aborted!=2)$fatal(1,"overlong RX accepted");
    rx_ready=0;enable=1;word(16'h73,r);if(r!==0)$fatal(1,"busy status missing");
    word(2,r);word(1,r);word(2,r);deselect();
    if(commits!=1||aborted!=3)$fatal(1,"busy RX accepted");
    rx_ready=1;enable=1;word(16'h73,r);word(WORDS,r);
    for(integer i=0;i<WORDS;i++)word(16'h9000^i,r);
    deselect();if(commits!=2||written!=17+WORDS+3)$fatal(1,"full frame not committed");
    for(integer i=0;i<WORDS;i++)if(received[i]!==(16'h9000^i))$fatal(1,"full frame address/data differs");
    enable=1;word(16'h16,r);if(bus[32])$fatal(1,"extension intercepted framework command");
    deselect();
    $display("ETHERNET-UIO PASS: capability, synchronous reads, explicit TX ack, complete RX commit, aborted/busy isolation");
    $finish;
  end
endmodule
