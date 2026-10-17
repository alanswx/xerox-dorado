`timescale 1ns/1ps
module tb_ethernet_wire;
`ifdef LARGE_FRAMES
  localparam WORDS=1024;
`else
  localparam WORDS=512;
`endif
  localparam ADDRESS_BITS=$clog2(WORDS);
  reg clk=0; always #1 clk=~clk;
  reg reset=1,step=1;
  reg tx_line=0,rx_wr=0,rx_commit=0,tx_ack=0;
  reg [ADDRESS_BITS-1:0] rx_addr=0,tx_addr=0;
  reg [15:0] rx_data=0;
  reg [ADDRESS_BITS:0] rx_words=0;
  wire rx_line,rx_ready,tx_valid;
  wire [15:0] tx_data;
  wire [ADDRESS_BITS:0] tx_words;
  wire [31:0] tx_good,tx_bad,tx_dropped,rx_bad;
  reg loopback=0;
  // Actual DskEth output from the complete Executive/Invaders RTL run:
  // a raw FileSystem lookup and the controller's own CRC, with no patch to
  // its transmitter, CPU or completion flags. CRC(sync+all 13 words)=0.
  reg [207:0] captured={16'h00ff,16'h8002,16'h0000,16'h0001,16'h0003,
    16'h0080,16'h0a46,16'h696c,16'h6553,16'h7973,16'h7465,16'h6d00,16'h7c50};
  dorado_ethernet_wire #(.GAP_TICKS(100),.MAX_WORDS(WORDS)) dut (
    .clk(clk),.reset(reset),.step(step),.tx_line(loopback?rx_line:tx_line),.rx_line(rx_line),
    .rx_wr(rx_wr),.rx_commit(rx_commit),.rx_addr(rx_addr),.rx_data(rx_data),.rx_words(rx_words),.rx_ready(rx_ready),
    .tx_addr(tx_addr),.tx_data(tx_data),.tx_ack(tx_ack),.tx_valid(tx_valid),.tx_words(tx_words),
    .tx_good(tx_good),.tx_bad(tx_bad),.tx_dropped(tx_dropped),.rx_bad(rx_bad));
  task tick; @(negedge clk); endtask
  // Independently constructed fixed 46-sample cells (1.5% slow). Decoder
  // must tolerate this just as the external cable clock can drift.
  task bit_out(input bit b);
    tx_line=~b;repeat(23)tick();tx_line=b;repeat(23)tick();
  endtask
  function automatic [15:0] check_word(input integer index);
    check_word = index==0 ? 16'h2201 : index==1 ? 16'o1000 : (index*1777)^16'ha55a;
  endfunction
  task send(input integer n,input bit corrupt);
    reg [15:0] crc,w;reg b;
    crc=16'h8005;bit_out(1); // CRC includes the leading sync 1
    for(integer i=0;i<n;i++)begin
      w=check_word(i);
      for(integer j=15;j>=0;j--)begin
        b=w[j];bit_out(b);
        crc={crc[14:0],1'b0} ^ ((crc[15]^b)?16'h8005:16'd0);
      end
    end
    for(integer j=15;j>=0;j--)bit_out(crc[j]^(corrupt&&j==8));
    tx_line=0;repeat(100)tick();
  endtask
  task verify(input integer n);
    if(!tx_valid||tx_words!=n)$fatal(1,"wrong packet: valid=%b words=%d expected=%d bad=%d",tx_valid,tx_words,n,tx_bad);
    for(integer i=0;i<n;i++)begin
      tx_addr=i;repeat(2)tick();
      if(tx_data!==check_word(i))$fatal(1,"word %d differs: %h",i,tx_data);
    end
  endtask
  task ack;tx_ack=1;tick();tx_ack=0;tick();endtask
  initial begin
    repeat(3)tick();reset=0;
    send(17,0);verify(17);ack();
    send(17,1);
    if(tx_valid||tx_bad!=1)$fatal(1,"corrupt CRC was accepted");
    send(WORDS,0);verify(WORDS);
    send(9,0); // a busy host must not overwrite its previous frame
    if(tx_dropped!=1)$fatal(1,"full buffer did not record dropped frame");
    verify(WORDS);ack();
    // Exercise the serializer's fractional 340ns timing, real CRC, and
    // host writes while the emulated machine is paused.
    step=0;
    for(integer i=0;i<31;i++)begin
      rx_addr=i;rx_data=check_word(i);rx_wr=1;tick();
    end
    rx_wr=0;rx_words=31;rx_commit=1;tick();rx_commit=0;
    repeat(100)tick();
    if(rx_ready||rx_line)$fatal(1,"wire advanced during machine pause");
    loopback=1;step=1;
    wait(rx_ready);repeat(100)tick();verify(31);ack();
    for(integer i=0;i<WORDS;i++)begin
      rx_addr=i;rx_data=check_word(i);rx_wr=1;tick();
    end
    rx_wr=0;rx_words=WORDS;rx_commit=1;tick();rx_commit=0;
    repeat(1000)tick();step=0;
    begin
      reg held;
      held=rx_line;repeat(100)tick();
      if(rx_line!==held)$fatal(1,"wire changed in a paused bit cell");
    end
    step=1;wait(rx_ready);repeat(100)tick();verify(WORDS);ack();
    loopback=0;bit_out(1);
    for(integer i=207;i>=0;i--)bit_out(captured[i]);
    tx_line=0;repeat(100)tick();
    if(!tx_valid||tx_words!=12)$fatal(1,"actual Dorado packet rejected");
    for(integer i=0;i<12;i++)begin
      tx_addr=i;repeat(2)tick();
      if(tx_data!==captured[(12-i)*16+:16])$fatal(1,"actual Dorado packet differs");
    end
    ack();
    if(tx_good!=5||rx_bad)$fatal(1,"unexpected packet counters");
    $display("ETHERNET-WIRE PASS: serial framing, fractional clock, CRC rejection, full-size packets, host backpressure, paused machine");
    $finish;
  end
  initial begin repeat(2000000)tick();$fatal(1,"timeout");end
endmodule
