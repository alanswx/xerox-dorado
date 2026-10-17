// Replay Initial's actual controller waveform through the MiSTer packet
// bridge, including pauses of the shared emulated clock.
`default_nettype none
module tb_initial_wire;
  reg clk=0;
  always #1 clk=~clk;
  reg reset=1,step=0,tx_line=0;
  reg [9:0] tx_addr=0;
  wire [15:0] tx_data;
  wire tx_valid;
  wire [10:0] tx_words;
  wire [31:0] good,bad,dropped;
  dorado_ethernet_wire #(.SYSPER(4),.MAX_WORDS(1024)) dut(
    .clk(clk),.reset(reset),.step(step),.tx_line(tx_line),.rx_line(),
    .rx_wr(1'b0),.rx_commit(1'b0),.rx_addr(10'd0),.rx_data(16'd0),
    .rx_words(11'd0),.rx_ready(),.tx_addr(tx_addr),.tx_data(tx_data),
    .tx_ack(1'b0),.tx_valid(tx_valid),.tx_words(tx_words),
    .tx_good(good),.tx_bad(bad),.tx_dropped(dropped),.rx_bad());
  integer times[0:4095],levels[0:4095];
  reg [15:0] expected[0:12]='{16'h0022,16'h0200,16'h0016,16'h00b4,
    16'h0001,16'h004b,16'h0000,16'h0000,16'h0004,16'h0022,
    16'h0001,16'h0001,16'hffff};
  string path;
  integer fd,n,t,v,start,i,pauses;
  initial begin
    if(!$value$plusargs("recording=%s",path))$fatal(1,"recording is required");
    fd=$fopen(path,"r");if(!fd)$fatal(1,"cannot open %s",path);
    n=0;
    while($fscanf(fd,"%d %d",t,v)==2)begin
      // The recording includes power-up settling at samples 0 and 14.
      // Retain every transition of the later real boot request unchanged.
      if(t<1000)continue;
      if(n==4096)$fatal(1,"recording exceeds capacity");
      times[n]=t;levels[n]=v;n++;
    end
    $fclose(fd);if(n<100)$fatal(1,"no recorded packet");
    start=times[0]-20;i=0;pauses=0;
    repeat(3)@(negedge clk);reset=0;
    for(t=start;t<=times[n-1]+100;t++)begin
      if(i<n && t==times[i])begin tx_line=!levels[i];i++;end
      step=1;@(negedge clk);
      if((t-start)%17==0)begin
        step=0;repeat(37)@(negedge clk);pauses++;
      end
    end
    step=0;
    if(!tx_valid || tx_words!=13 || good!=1 || bad || dropped)
      $fatal(1,"recorded request rejected: valid=%b words=%0d good=%0d bad=%0d",tx_valid,tx_words,good,bad);
    for(integer w=0;w<13;w++)begin
      tx_addr=w;repeat(2)@(negedge clk);
      if(tx_data!==expected[w])$fatal(1,"word%0d got%04h expected%04h",w,tx_data,expected[w]);
    end
    $display("INITIAL WIRE PASS: all13 original request words and real CRC accepted through %0d clock pauses",pauses);
    $finish;
  end
endmodule
`default_nettype wire
