// tb_startseq -- PARC's own DoDoradoMicroInst, replayed against the RTL.
//
// A DIAGNOSTIC, NOT A GATE. It asserts nothing and always exits zero; it prints
// the Control section's state after each step of the boot sequence so the next
// person can see where the machine stops. `make -C verilog startseq`.
//
// THE SEQUENCE IS PARC'S, transcribed from
// `chm/dorado/expanded/doradobaserom.dm!12_/doradocpint.masm`, which is the
// BaseBoard's own 6502 source. This is how a Dorado is made to execute one
// microinstruction, and the BaseBoard does it 475 times during a cold boot:
//
//     DoDoradoMicroInst:
//       JSR WaitForCPControl                       ; gain the CP bus
//       LDAI InhibitCAHolds+ClrReady / JSR DoClock ; function 1
//       LDAI ClrStop+ClrMIR+ClrCT+Freeze / CLC     ; function 0, 0x4E, no SetSS
//       JSR DoControl
//       LDAI 0 / SEC / JSR DoControl               ; function 0, 0x00, SetSS
//       ... four MIR strobes, the extra bit rotated into the SetSS position ...
//       LDAI SetRun / JSR DoControl                ; function 0, 0x01, SetSS
//       LDAI SetRun / SEC / JSR DoControl          ; and stop it again
//       LDAI 0      / SEC / JSR DoControl
//
// with the bit values from `doradoio.mdefs`, which is a complete specification
// of this interface and agrees with the C emulator exactly:
//
//     Control=0x00 Clock=0x10 ABMux0=0x20 ABMux1=0x30
//     MIR0=0x40 MIR1=0x50 MIR2=0x60 MIR3=0x70          function, in bits 6:4
//
//     ClrStop=0x40 StopAtT1=0x20 Jam=0x10 Freeze=0x08  in Control
//     ClrMIR=0x04 ClrCT=0x02 SetRun=0x01
//     SetSS=0x80                                       in the low-order byte
//
//     DAddrBit=0x80 ShiftDMD=0x40 InhibitCAHolds=0x20  in Clock -- the
//     GetTLINK=0x10 UseCPReg=0x08 UseDMD=0x04          function the C emulator
//     BaseAttn=0x02 ClrReady=0x01                      treats as a no-op
//
// WHAT IT SHOWS TODAY: every strobe lands and `dRun` follows `SetRun`, so the
// command path works end to end -- but `CLKEnable'a` never clears, so `clk0'`
// never runs and the machine never executes. See docs/verilog-handoff.md, "the
// machine will not start", for the analysis and for the hypotheses already
// eliminated.

`default_nettype none

module tb_startseq;

  reg sys_clk = 1'b0;
  always #1 sys_clk = ~sys_clk;

  // The backplane clock the BaseBoard would fan out.
  reg [3:0] ckd = 4'd0;
  always @(posedge sys_clk) ckd <= ckd + 4'd1;
  wire mclk = ckd[3];

  reg [2:0] addr_n = 3'b111;
  reg [8:0] cpout  = 9'd0;
  reg       strb_n = 1'b1;
  reg       setrun = 1'b0, setss_n = 1'b1, setrunrfsh = 1'b0;

  dorado_control m (
      .sys_clk(sys_clk), .CLK_ca_p_(mclk), .CLK_cb_p_(mclk),
      .CPAddr_0_p_(addr_n[2]), .CPAddr_1_p_(addr_n[1]), .CPAddr_2_p_(addr_n[0]),
      .CPOut_0(cpout[8]), .CPOut_1(cpout[7]), .CPOut_2(cpout[6]),
      .CPOut_3(cpout[5]), .CPOut_4(cpout[4]), .CPOut_5(cpout[3]),
      .CPOut_6(cpout[2]), .CPOut_7(cpout[1]), .CPOut_8(cpout[0]),
      .CPStrb_p_(strb_n),
      .SetRun(setrun), .SetSS_p_(setss_n), .SetRunRfsh(setrunrfsh)
  );

  task strobe(input [2:0] fn, input [7:0] data, input ss);
    begin
      addr_n = ~fn;
      cpout  = {data, ss};
      repeat (6) @(posedge sys_clk);
      strb_n = 1'b0;
      repeat (8) @(posedge sys_clk);
      strb_n = 1'b1;
      repeat (6) @(posedge sys_clk);
    end
  endtask

  // A Control strobe also latches SetRun and SetSS on the BASEBOARD, in the
  // SN74LS175 at g07, and ContA sees them as backplane wires rather than as
  // CPOut bits. With no BaseBoard here, they are driven to match.
  task control(input [7:0] data, input ss);
    begin
      strobe(3'd0, data, ss);
      setrun  =  data[0];
      setss_n = ~ss;
      repeat (8) @(posedge sys_clk);
    end
  endtask

  integer i, n;
  reg     p;
  task report(input string what);
    begin
      n = 0;
      for (i = 0; i < 3000; i = i + 1) begin
        @(posedge sys_clk);
        if (m.b_ContB.clk0_p_Bc !== p) begin
          n = n + 1;
          p = m.b_ContB.clk0_p_Bc;
        end
      end
      $display("startseq: %-26s CLKEnable'a=%b StartCycle'a=%b dRun=%b dStop=%b Phase0=%b DoradoStopped=%b  clk0' edges=%0d",
               what, m.CLKEnable_p_a, m.StartCycle_p_a, m.b_ContA.dRun,
               m.b_ContA.dStop, m.b_ContA.Phase0, m.b_ContB.DoradoStopped, n);
    end
  endtask

  initial begin
    repeat (60) @(posedge sys_clk);
    report("power-up");

    strobe(3'd1, 8'h21, 1'b0);   report("Clock InhibitCAHolds+ClrReady");
    control(8'h4E, 1'b0);        report("Control ClrStop+MIR+CT+Freeze");
    control(8'h00, 1'b1);        report("Control 0, SetSS");

    strobe(3'd4, 8'h00, 1'b0);
    strobe(3'd5, 8'h00, 1'b0);
    strobe(3'd6, 8'h00, 1'b0);
    strobe(3'd7, 8'h00, 1'b0);   report("MIR0..MIR3");

    control(8'h01, 1'b1);        report("Control SetRun, SetSS");
    control(8'h01, 1'b1);        report("Control SetRun, SetSS again");
    control(8'h00, 1'b1);        report("Control 0, SetSS");

    // Verilog has no implicit string concatenation across lines.
    $display("startseq: the command path works; the machine does not run -- see docs/verilog-handoff.md");
    $finish;
  end

endmodule

`default_nettype wire
