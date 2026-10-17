// Complete receive data path, from serial cable through FIFO/bus register.
// Task-selection levels and I/O strobes are external component stimuli.
// A warmup packet establishes the original receiver's BOP state; no
// receiver state, data, CRC, attention or completion is substituted.
`timescale 1ns/1ps
module tb_ethernet_receive_sequence;
reg clk=0;always #1 clk=~clk;
reg[2:0] div=0;always @(posedge clk)div<=div+1'b1;
reg io_reset=1,input_n=1,output_n=1;
reg[7:0]tioa=8'o16;reg[15:0]out_word=16'hf6ff;
wire rx_line,rx_ready;
reg wr=0,commit=0;reg[8:0]addr=0;reg[15:0]data_word=0;
DskEth_m_Rev_m_Cf #(.SYSPER(4)) d(.sys_clk(clk),.CLK_disk_p_(div[1]),
.CLKEnable_p_a(1'b0),.MemSH_p_(div[2]),.IOReset(io_reset),.RcvData(rx_line),.Collision(1'b0),
.IOin_p_(input_n),.IOout_p_(output_n),.IOHold(1'b0),
.Next_0(1'b0),.Next_1(1'b1),.Next_2(1'b1),.Next_3(1'b1),
.IOB_00(out_word[15]),
.IOB_01(out_word[14]),
.IOB_02(out_word[13]),
.IOB_03(out_word[12]),
.IOB_04(out_word[11]),
.IOB_05(out_word[10]),
.IOB_06(out_word[9]),
.IOB_07(out_word[8]),
.IOB_08(out_word[7]),
.IOB_09(out_word[6]),
.IOB_10(out_word[5]),
.IOB_11(out_word[4]),
.IOB_12(out_word[3]),
.IOB_13(out_word[2]),
.IOB_14(out_word[1]),
.IOB_15(out_word[0]),
.TIOA_0(tioa[7]),
.TIOA_1(tioa[6]),
.TIOA_2(tioa[5]),
.TIOA_3(tioa[4]),
.TIOA_4(tioa[3]),
.TIOA_5(tioa[2]),
.TIOA_6(tioa[1]),
.TIOA_7(tioa[0]));
dorado_ethernet_wire link(.clk(clk),.reset(1'b0),.step(1'b1),.tx_line(1'b0),.rx_line(rx_line),
.rx_wr(wr),.rx_commit(commit),.rx_addr(addr),.rx_data(data_word),.rx_words(10'd12),.rx_ready(rx_ready),
.tx_addr(9'd0),.tx_ack(1'b0));
wire[15:0] bus_word={d.EthData_00,d.EthData_01,d.EthData_02,d.EthData_03,d.EthData_04,d.EthData_05,d.EthData_06,d.EthData_07,d.EthData_08,d.EthData_09,d.EthData_10,d.EthData_11,d.EthData_12,d.EthData_13,d.EthData_14,d.EthData_15};
reg[191:0]packet={16'h2201,16'h0200,16'h001a,16'h0081,16'h0000,16'h0000,
16'h0122,16'h0000,16'h0002,16'h0101,16'h0000,16'h0002};
integer n=0;reg[15:0]seen;
task tick;@(negedge clk);endtask
initial begin
force d.Curr_eq_EthRx=1;force d.Prev_eq_EthRx=1;
repeat(1000)tick();io_reset=0;repeat(1000)tick();
// Drive a control write during one complete board clock.
output_n=0;repeat(8)tick();output_n=1;repeat(20)tick();tioa=8'o15;
$display("CONTROL RxOn=%b BOP=%b curr=%b prev=%b",d.RxOn,d.RxBOP,d.Curr_eq_EthRx,d.Prev_eq_EthRx);
for(integer i=0;i<12;i++)begin addr=i;data_word=packet[(11-i)*16+:16];wr=1;tick();end
wr=0;commit=1;tick();commit=0;
wait(d.EthData_18 && d.RxBusRegFull);repeat(12)tick();
$display("WARMUP end BOP=%b full=%b data=%04h",d.RxBOP,d.RxBusRegFull,bus_word);
input_n=0;repeat(8)tick();input_n=1;repeat(16)tick();
wait(rx_ready);repeat(30)tick();
$display("PRE SECOND BOP=%b full=%b",d.RxBOP,d.RxBusRegFull);
commit=1;tick();commit=0;
while(n<14)begin
 wait(d.RxBusRegFull);repeat(12)tick();
 seen=bus_word;
 $display("INPUT n=%0d data=%04h EOP=%b BOP=%b full=%b",n,seen,d.EthData_18,d.RxBOP,d.RxBusRegFull);
 if(n<12 && seen!==packet[(11-n)*16+:16])$fatal(1,"CPU receive sequence mismatch at %0d got%04h",n,seen);
 if(!d.RxBusRegFull || d.TIOA_eq_EthData_p_)$fatal(1,"word was not held for an addressed Input");
 if(n==12 && (seen!==16'h3b50 || d.EthData_18))$fatal(1,"CRC word or its EOP differs");
 if(n==13 && (seen!==0 || !d.EthData_18))$fatal(1,"final status or its EOP differs");
 input_n=0;repeat(8)tick();input_n=1;repeat(16)tick();
 if(d.RxBusRegFull)$fatal(1,"Input did not consume the held word");
 n++;
end
$display("ETHERNET RECEIVE SEQUENCE PASS");$finish;
end
initial begin repeat(200000)tick();$fatal(1,"timeout full=%b BOP=%b RxOn=%b",d.RxBusRegFull,d.RxBOP,d.RxOn);end
endmodule
