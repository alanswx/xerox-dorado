// tb_crc -- cell_F9401 against a published check value.
//
// CRC-16 (X16+X15+X2+1, select 000), register cleared, message bits entered
// MSB first with CWE high, then CWE low and the check word shifted out on Q
// MSB first. For the ASCII message "123456789" that is the CRC-16/BUYPASS
// check value 0xFEE8 (poly 0x8005, init 0, no reflection, no xorout).
// Then the message followed by its check word is entered again with CWE
// high, which must leave the register zero: ER low.
`timescale 1ns/1ps
module tb_crc;
  reg sys_clk = 0; always #1 sys_clk = ~sys_clk;
  reg cp = 1, pn = 1, mr = 0, cwe = 1, d = 0;
  wire q, er;
  cell_F9401 dut (.sys_clk(sys_clk), .p1(cp), .p2(pn), .p3(1'b0), .p4(mr), .p5(1'b0),
                  .p7(1'b0), .p8(1'b0), .p10(cwe), .p11(d), .p12(q), .p13(er), .p14(1'b1));
  reg [71:0] msg = {"1","2","3","4","5","6","7","8","9"};
  reg [15:0] crc; integer i;
  task tick; begin cp = 1; repeat (3) @(posedge sys_clk); cp = 0; repeat (3) @(posedge sys_clk); end endtask
  initial begin
    mr = 1; repeat (3) @(posedge sys_clk); mr = 0; repeat (2) @(posedge sys_clk);
    cwe = 1;
    for (i = 71; i >= 0; i = i - 1) begin d = msg[i]; tick; end
    cwe = 0; d = 0;
    for (i = 15; i >= 0; i = i - 1) begin crc[i] = q; tick; end
    $display("tb_crc: CRC-16 of \"123456789\" = %04x (want fee8)", crc);
    if (crc !== 16'hFEE8) $fatal(1, "tb_crc: FAIL -- wrong check word");
    // check: message + check word in, CWE high throughout
    mr = 1; repeat (3) @(posedge sys_clk); mr = 0; repeat (2) @(posedge sys_clk);
    cwe = 1;
    for (i = 71; i >= 0; i = i - 1) begin d = msg[i]; tick; end
    for (i = 15; i >= 0; i = i - 1) begin d = crc[i]; tick; end
    $display("tb_crc: after message + check word, ER = %b (want 0), register %04x", er, dut.r);
    if (er !== 1'b0) $fatal(1, "tb_crc: FAIL -- checker reports an error on a good message");
    // and a corrupted message must be caught
    mr = 1; repeat (3) @(posedge sys_clk); mr = 0; repeat (2) @(posedge sys_clk);
    for (i = 71; i >= 0; i = i - 1) begin d = msg[i] ^ (i == 40); tick; end
    for (i = 15; i >= 0; i = i - 1) begin d = crc[i]; tick; end
    $display("tb_crc: one bit flipped, ER = %b (want 1)", er);
    if (er !== 1'b1) $fatal(1, "tb_crc: FAIL -- checker missed a flipped bit");
    $display("tb_crc: PASS -- cell_F9401 generates and checks CRC-16 as the data sheet describes");
    $finish;
  end
endmodule
