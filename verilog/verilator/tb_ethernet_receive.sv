// Real DskEth phase decoder, receive FSM, CRC and shift register. No CPU
// state or receive-completion flags are planted; stimulus is cable data.
`timescale 1ns/1ps
module tb_ethernet_receive;
  reg clk=0;always #1 clk=~clk;
  reg [1:0] div=0;
  reg io_reset=1;
  always @(posedge clk)div<=div+1'b1;
  wire rx_line,rx_ready;
  reg wr=0,commit=0;
  reg [8:0] addr=0;
  reg [15:0] data_word=0;
  wire [15:0] rx_word={d.RxSR_00,d.RxSR_01,d.RxSR_02,d.RxSR_03,
    d.RxSR_04,d.RxSR_05,d.RxSR_06,d.RxSR_07,d.RxSR_08,d.RxSR_09,d.RxSR_10,d.RxSR_11,
    d.RxSR_12,d.RxSR_13,d.RxSR_14,d.RxSR_15};
  wire [15:0] bad_rx_word={damaged.RxSR_00,damaged.RxSR_01,damaged.RxSR_02,damaged.RxSR_03,
    damaged.RxSR_04,damaged.RxSR_05,damaged.RxSR_06,damaged.RxSR_07,damaged.RxSR_08,damaged.RxSR_09,damaged.RxSR_10,damaged.RxSR_11,
    damaged.RxSR_12,damaged.RxSR_13,damaged.RxSR_14,damaged.RxSR_15};
  DskEth_m_Rev_m_Cf #(.SYSPER(4)) d(.sys_clk(clk),.CLK_disk_p_(div[1]),
    .CLKEnable_p_a(1'b0),.IOReset(io_reset),.RcvData(rx_line),.Collision(1'b0));
  wire damaged_line=rx_line ^ (link.rstate==4 && link.rbit==8);
  DskEth_m_Rev_m_Cf #(.SYSPER(4)) damaged(.sys_clk(clk),.CLK_disk_p_(div[1]),
    .CLKEnable_p_a(1'b0),.IOReset(io_reset),.RcvData(damaged_line),.Collision(1'b0));
  dorado_ethernet_wire link(.clk(clk),.reset(1'b0),.step(1'b1),.tx_line(1'b0),.rx_line(rx_line),
    .rx_wr(wr),.rx_commit(commit),.rx_addr(addr),.rx_data(data_word),.rx_words(10'd12),.rx_ready(rx_ready),
    .tx_addr(9'd0),.tx_ack(1'b0));
  reg [191:0] packet={16'h00ff,16'h8002,16'h0000,16'h0001,16'h0003,
    16'h0080,16'h0a46,16'h696c,16'h6553,16'h7973,16'h7465,16'h6d00};
  reg sync_d=1,bad_sync_d=1;
  reg carrier_seen=0,bad_carrier_seen=0;
  integer count=0,status=0,bad_status=0,bad_count=0;
  reg armed=0;
  always @(negedge clk)begin
    sync_d<=d.RxFifoWE_p_;
    bad_sync_d<=damaged.RxFifoWE_p_;
    if(armed && d.PDCarrier)carrier_seen<=1;
    if(armed && damaged.PDCarrier)bad_carrier_seen<=1;
    if(armed && bad_carrier_seen && bad_sync_d && !damaged.RxFifoWE_p_)begin
      bad_count<=bad_count+1;
      if(damaged.RxSR_18)begin
      // Read the status actually latched into the FIFO. The live CRC
      // checker can already have reset by this later Dorado clock edge.
      if(bad_rx_word!==16'h0008)
        $fatal(1,"damaged CRC not identified independently of framing/overflow: %h",bad_rx_word);
      bad_status<=bad_status+1;
      end else if(bad_count<12 && bad_rx_word!==packet[(11-bad_count)*16+:16])
        $fatal(1,"damage affected the payload rather than its CRC");
      else if(bad_count==12 && bad_rx_word!==16'h7d50)$fatal(1,"CRC damage not delivered");
    end
    if(armed && carrier_seen && sync_d && !d.RxFifoWE_p_)begin
      $display("DskEth RX word %d=%04x EOP=%b crc=%b",count,rx_word,d.RxSR_18,d.RxCRCError);
      if(d.RxSR_18)begin
        if(rx_word!==0)$fatal(1,"real receiver rejected adapter packet: status=%h",rx_word);
        status<=status+1;
      end else begin
        if(count<12 && rx_word!==packet[(11-count)*16+:16])$fatal(1,"real receiver data mismatch");
        if(count==12 && rx_word!==16'h7c50)$fatal(1,"real receiver CRC word differs");
        count<=count+1;
      end
    end
  end
  task tick;@(negedge clk);endtask
  integer phase=0;
  initial begin
    void'($value$plusargs("phase=%d",phase));
    repeat(1000)tick();io_reset=0;repeat(1000+phase)tick();
    for(integer i=0;i<12;i++)begin addr=i;data_word=packet[(11-i)*16+:16];wr=1;tick();end
    wr=0;armed=1;commit=1;tick();commit=0;
    wait(rx_ready);repeat(1000)tick();
    if(count!=13||status!=1||bad_status!=1||bad_count!=14)$fatal(1,"real receiver did not deliver payload, CRC and status: count=%d status=%d bad=%d",count,status,bad_status);
    $display("ETHERNET-RECEIVE PASS: phase=%d actual data, good status, damaged CRC rejected",phase);$finish;
  end
  initial begin repeat(100000)tick();$fatal(1,"timeout");end
endmodule
