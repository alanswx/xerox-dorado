// tb_writeim -- a jammed Write-IM deposits into IM, measured against a clean
// baseline.
//
// THE BASELINE IS THE POINT, and getting it wrong is what made the first
// version of this test worthless. IM comes up with 17 cells set: from the
// all-zero initial state the ContB write logic asserts its ACTIVE-LOW enables
// for a few cycles until the combinational logic settles, and the array takes a
// write of whatever `RBMux` then held. That is a settling transient, not a
// stuck enable -- MEASURED: after 2,000 idle cycles `WER'Aa`, `WEL'Aa` and
// `preWE'a` all read 1, de-asserted, and across the next 20,000 cycles the
// array takes NOTHING.
//
// A first version of this test counted non-zero cells from time zero, found 17,
// and called it a successful write. It was the transient. Four mutations passed
// and a no-stimulus control reproduced it exactly, with the machine stopped.
// The lesson is the ordinary one: settle, WIPE, then measure.
//
// (Nor is any of this a fault in the hardware. Boot0 exists to load IM; the
// machine never assumes IM holds anything at power-up.)
//
// WHAT IT PROVES, and it is checked BOTH WAYS: jamming the right-half Write-IM
// sets right-half cells and leaves every left-half cell of all 147,456 alone,
// and jamming the left-half one does the exact opposite. `RSTK[3]` selects the
// half, precisely as `cpu.c` says it does.
//
// WHAT IT DOES NOT PROVE. The DATA does not come from CPReg: zeroing the CPReg
// the microinstruction is supposed to read leaves the written pattern
// unchanged. Nor does the address -- every write lands at IM[0] whatever CPReg
// holds. Both travel paths that are not yet working: the data through
// `B <- RWCPReg` to `RBMux`, and the address through Link to `TNIA` on ContA
// and an MC1662 multiplexer at ContB f21. So this establishes the MECHANISM and
// the half-select, not the operand.
//
// EVERYTHING JAMMED HERE IS PARC'S OWN, byte for byte, from the IRTable in
// `doradoboot.masm` -- microinstructions hand-written in the five-byte format
// `DoDoradoMicroInst` takes, whose layout comment is the mir-diff table exactly:
//
//     CPRegToLink#    30 13 EF 04 40   B_RWCPReg, LocalJump[1]
//     CPRegToIM#      20 13 EF 03 4F   IMRHB'POK_RWCPReg  (= IMRHBLOCKIs0#)
//     Nop#            70 01 0F 4C 40
//
// and each jam is followed by a Nop, as `DoIRTableInstAndNop` does it: "the Nop
// holds CPReg constant through T3 of the previous instruction".

`default_nettype none

module tb_writeim;


  // THE OVERSAMPLING RATIO -- sys_clk per microinstruction. SYSPER reaches only
  // `cell_CLOCKGEN`, which lives on the BaseBoard, so a sub-machine's clock is
  // whatever its bench drives; this one used to hard-code a divide-by-16 and
  // therefore only ever ran at 16x. Real time needs sys_clk = SYSPER x 16.67
  // MHz and measured Fmax is 48.99 MHz, so SYSPER=2 is the target.
  //
  //     make -C verilog/verilator <gate> SYSPER=2
`ifndef SYSPER_OVERRIDE
  `define SYSPER_OVERRIDE 16
`endif
  localparam integer SYSPER = `SYSPER_OVERRIDE;
  // SCALE A FABRIC-CYCLE WAIT so it means the same amount of DORADO time at any
  // ratio. NOT `SYSPER / 16` -- integer division gives ZERO below 16x, which
  // turns every `repeat` into no wait at all.
  function automatic integer WT(input integer n);
    begin
      // ROUND TO NEAREST, not toward zero. Truncation flattens the SHAPE of a
      // waveform at low ratios -- at SYSPER=4 a 4-cycle low and a 6-cycle
      // setup both truncate to 1, turning 4:6 into 1:1 -- and the single-step
      // chain depends on strobe SPACING (SetRun must survive three RunClk'
      // edges). That cost step-test at 4x and nothing else. Exact at
      // SYSPER=16, where WT(n) = n with no remainder.
      WT = (n * SYSPER + 8) / 16;
      if (WT < 1) WT = 1;
    end
  endfunction

  reg sys_clk = 1'b0;
  always #1 sys_clk = ~sys_clk;
  reg [3:0] ckd = 4'd0;
  // At SYSPER=16 this is bit for bit the counter it replaces:
  // ckd counts 0..15 and mclk is ckd >= 8, i.e. ckd[3].
  always @(posedge sys_clk)
    ckd <= (ckd == SYSPER - 1) ? 4'd0 : ckd + 4'd1;
  wire mclk = (ckd >= SYSPER / 2);

  reg [2:0] addr_n = 3'b111;
  reg [8:0] cpout  = 9'd0;
  reg       strb_n = 1'b1;
  reg       setrun = 1'b0, setss_n = 1'b1;
  reg       dmd = 1'b0, dmc = 1'b0, udmd = 1'b0;

  dorado_proc m (
      .sys_clk(sys_clk),
      .CLK_ca_p_(mclk), .CLK_cb_p_(mclk), .CLK_ph_p_(mclk), .CLK_pl_p_(mclk),
      .CPAddr_0_p_(addr_n[2]), .CPAddr_1_p_(addr_n[1]), .CPAddr_2_p_(addr_n[0]),
      .CPOut_0(cpout[8]), .CPOut_1(cpout[7]), .CPOut_2(cpout[6]),
      .CPOut_3(cpout[5]), .CPOut_4(cpout[4]), .CPOut_5(cpout[3]),
      .CPOut_6(cpout[2]), .CPOut_7(cpout[1]), .CPOut_8(cpout[0]),
      .CPStrb_p_(strb_n), .SetRun(setrun), .SetSS_p_(setss_n)
  );

  // THE BASEBOARD'S CONTROL REGISTER, modelled here because this machine has no
  // BaseBoard. `SetRun` and `SetSS'` are BACKPLANE wires, not CP-bus lines --
  // BaseBd f02 (an MC10124 TTL-to-ECL translator) drives them, and what it
  // translates is latched by g07, an SN74LS175 clocked by `TControlStrb'`:
  //
  //     g07 .p9  = TControlStrb'   (clock -- so it latches on strobe RELEASE)
  //     g07 .p4  = TCPBus.07  -> .p2 TSetRun          -> f02 -> SetRun
  //     g07 .p5  = TCPBus.08  -> .p7 (Q')             -> f02 -> SetSS'
  //
  // PARC numbers MSB first, so over the nine CP-bus bits .07 is the low bit of
  // the eight-bit Control byte -- `SetRun = 1` in doradoio.mdefs -- and .08 is
  // the ninth bit, where DoControl's CARRY lands ("LDAI Control^1 / RORA"),
  // i.e. SetSS. So a Control strobe ALREADY carries both, and setting the
  // ports by hand as well was a second, unsynchronised copy of the same state.
  task strobe(input [2:0] fn, input [7:0] data, input ss);
    begin
      addr_n = ~fn; cpout = {data, ss};
      repeat (WT(4)) @(posedge sys_clk);
      strb_n = 1'b0; repeat (WT(6)) @(posedge sys_clk);
      strb_n = 1'b1;
      if (fn == 3'd0) begin           // Control: g07 clocks on release
        setrun  =  data[0];
        setss_n = ~ss;
      end
      repeat (WT(4)) @(posedge sys_clk);
    end
  endtask
  // PARC's SetMufflerAddress, and the two words doradomufman.masm writes just
  // after the supplies come up. They HOLD THE MIR, which is what makes a jam
  // survive its own first clock -- see tb_operand.sv for the whole chain.
  // Forced because BaseBd l24 drives DMuxData/DMuxClk/UseDMD and this
  // configuration has no BaseBoard.
  integer kk;
  task manifold(input [11:0] word);
    begin
      for (kk = 11; kk >= 0; kk = kk - 1) begin
        dmd = word[kk];
        repeat (WT(4)) @(posedge sys_clk); dmc = 1'b1;
        repeat (WT(4)) @(posedge sys_clk); dmc = 1'b0;
        repeat (WT(4)) @(posedge sys_clk);
      end
      udmd = 1'b1; repeat (WT(12)) @(posedge sys_clk);
      udmd = 1'b0; repeat (WT(12)) @(posedge sys_clk);
    end
  endtask

  task setcpreg(input [15:0] v);
    begin strobe(3'd2, v[15:8], 1'b0); strobe(3'd3, v[7:0], 1'b0); end
  endtask
  task jam1(input [7:0] b0, input [7:0] b1, input [7:0] b2,
            input [7:0] b3, input [7:0] b4);
    begin
      // Stop first: with the MIR clock held, a running machine repeats the
      // jammed instruction forever, so the next jam starts from rest.
      setrun = 0; setss_n = 1;
      repeat (WT(400)) @(posedge sys_clk);
      strobe(3'd0, 8'h4E, 1'b0);   // ClrStop+ClrMIR+ClrCT+Freeze
      strobe(3'd0, 8'h00, 1'b1);
      strobe(3'd4, b1, b0[7]); strobe(3'd5, b2, b0[6]);
      strobe(3'd6, b3, b0[5]); strobe(3'd7, b4, b0[4]);
      strobe(3'd0, 8'h41, 1'b1);   // ClrStop AND SetRun, see tb_run
      repeat (WT(400)) @(posedge sys_clk);
    end
  endtask
  task jamnop(input [7:0] b0, input [7:0] b1, input [7:0] b2,
              input [7:0] b3, input [7:0] b4);
    begin jam1(b0,b1,b2,b3,b4); jam1(8'h70,8'h01,8'h0F,8'h4C,8'h40); end
  endtask

  integer i, right_hits, left_hits;

  initial begin
    force m.DMuxData = dmd;
    force m.DMuxClk  = dmc;
    force m.UseDMD   = udmd;
    repeat (WT(2000)) @(posedge sys_clk);          // let the logic settle
    manifold(12'h030);                         // DisableDoradoErrors
    manifold(12'h1E0);                         // SetMidasStopMIRClk
    if (m.StopMIRClk !== 1'b1) $fatal(1, "the MIR clock is not held");
    if (m.b_ContB.WER_p_Aa !== 1'b1 || m.b_ContB.WEL_p_Aa !== 1'b1)
      $fatal(1, "the IM write enables did not settle de-asserted");
    for (i=0;i<1024;i=i+1) m.b_ContB.u_a06.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_a07.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_a08.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_a09.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_a10.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_a11.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_a14.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_a15.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_a16.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_a17.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_a18.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_a19.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_b06.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_b07.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_b08.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_b09.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_b10.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_b11.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_b14.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_b15.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_b16.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_b17.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_b18.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_b19.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_c06.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_c07.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_c08.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_c09.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_c10.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_c11.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_c14.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_c15.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_c16.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_c17.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_c18.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_c19.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_d06.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_d07.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_d08.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_d09.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_d10.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_d11.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_d14.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_d15.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_d16.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_d17.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_d18.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_d19.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_e06.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_e07.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_e08.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_e09.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_e10.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_e11.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_e14.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_e15.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_e16.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_e17.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_e18.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_e19.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_f06.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_f07.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_f08.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_f09.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_f10.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_f11.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_f14.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_f15.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_f16.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_f17.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_f18.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_f19.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_g06.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_g07.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_g08.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_g09.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_g10.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_g11.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_g14.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_g15.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_g16.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_g17.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_g18.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_g19.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_h06.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_h07.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_h08.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_h09.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_h10.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_h11.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_h14.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_h15.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_h16.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_h17.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_h18.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_h19.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_i06.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_i07.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_i08.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_i09.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_i10.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_i11.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_i14.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_i15.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_i16.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_i17.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_i18.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_i19.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_j06.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_j07.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_j08.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_j09.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_j10.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_j11.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_j14.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_j15.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_j16.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_j17.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_j18.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_j19.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_k06.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_k07.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_k08.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_k09.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_k10.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_k11.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_k14.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_k15.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_k16.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_k17.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_k18.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_k19.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_l06.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_l07.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_l08.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_l09.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_l10.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_l11.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_l14.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_l15.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_l16.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_l17.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_l18.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_l19.mem[i] = 1'b0;

    strobe(3'd1, 8'h21, 1'b0);                 // Clock: InhibitCAHolds+ClrReady
    setcpreg(16'h0000);   // BMux inverts, so this is the all-ones pattern
    jamnop(8'h20, 8'h13, 8'hEF, 8'h03, 8'h4F); // CPRegToIM#, RIGHT half

    right_hits = 0; left_hits = 0;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_a06.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_a07.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_a08.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_a09.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_a10.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_a11.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_a14.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_a15.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_a16.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_a17.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_a18.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_a19.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_b06.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_b07.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_b08.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_b09.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_b10.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_b11.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_b14.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_b15.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_b16.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_b17.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_b18.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_b19.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_c06.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_c07.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_c08.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_c09.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_c10.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_c11.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_c14.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_c15.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_c16.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_c17.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_c18.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_c19.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_d06.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_d07.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_d08.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_d09.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_d10.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_d11.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_d14.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_d15.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_d16.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_d17.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_d18.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_d19.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_e06.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_e07.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_e08.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_e09.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_e10.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_e11.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_e14.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_e15.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_e16.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_e17.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_e18.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_e19.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_f06.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_f07.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_f08.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_f09.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_f10.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_f11.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_f14.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_f15.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_f16.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_f17.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_f18.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_f19.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_g06.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_g07.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_g08.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_g09.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_g10.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_g11.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_g14.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_g15.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_g16.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_g17.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_g18.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_g19.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_h06.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_h07.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_h08.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_h09.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_h10.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_h11.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_h14.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_h15.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_h16.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_h17.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_h18.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_h19.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_i06.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_i07.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_i08.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_i09.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_i10.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_i11.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_i14.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_i15.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_i16.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_i17.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_i18.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_i19.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_j06.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_j07.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_j08.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_j09.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_j10.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_j11.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_j14.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_j15.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_j16.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_j17.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_j18.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_j19.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_k06.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_k07.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_k08.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_k09.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_k10.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_k11.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_k14.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_k15.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_k16.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_k17.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_k18.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_k19.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_l06.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_l07.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_l08.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_l09.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_l10.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_l11.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_l14.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_l15.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_l16.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_l17.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_l18.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_l19.mem[i]) left_hits = left_hits + 1;

    $display("tb_writeim: right-half Write-IM -- %0d right-half cells set, %0d left-half",
             right_hits, left_hits);
    if (right_hits == 0) $fatal(1, "the right-half Write-IM deposited nothing");
    if (left_hits != 0)  $fatal(1, "the half-select failed -- the left half was written too");

    // And the mirror image, from a wiped array again.
    for (i=0;i<1024;i=i+1) m.b_ContB.u_a06.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_a07.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_a08.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_a09.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_a10.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_a11.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_a14.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_a15.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_a16.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_a17.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_a18.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_a19.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_b06.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_b07.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_b08.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_b09.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_b10.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_b11.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_b14.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_b15.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_b16.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_b17.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_b18.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_b19.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_c06.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_c07.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_c08.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_c09.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_c10.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_c11.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_c14.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_c15.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_c16.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_c17.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_c18.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_c19.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_d06.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_d07.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_d08.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_d09.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_d10.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_d11.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_d14.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_d15.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_d16.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_d17.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_d18.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_d19.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_e06.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_e07.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_e08.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_e09.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_e10.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_e11.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_e14.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_e15.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_e16.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_e17.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_e18.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_e19.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_f06.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_f07.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_f08.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_f09.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_f10.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_f11.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_f14.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_f15.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_f16.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_f17.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_f18.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_f19.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_g06.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_g07.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_g08.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_g09.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_g10.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_g11.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_g14.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_g15.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_g16.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_g17.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_g18.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_g19.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_h06.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_h07.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_h08.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_h09.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_h10.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_h11.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_h14.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_h15.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_h16.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_h17.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_h18.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_h19.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_i06.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_i07.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_i08.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_i09.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_i10.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_i11.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_i14.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_i15.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_i16.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_i17.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_i18.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_i19.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_j06.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_j07.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_j08.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_j09.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_j10.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_j11.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_j14.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_j15.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_j16.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_j17.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_j18.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_j19.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_k06.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_k07.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_k08.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_k09.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_k10.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_k11.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_k14.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_k15.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_k16.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_k17.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_k18.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_k19.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_l06.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_l07.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_l08.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_l09.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_l10.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_l11.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_l14.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_l15.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_l16.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_l17.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_l18.mem[i] = 1'b0;
    for (i=0;i<1024;i=i+1) m.b_ContB.u_l19.mem[i] = 1'b0;
    setcpreg(16'h0000);   // BMux inverts, so this is the all-ones pattern
    jamnop(8'h60, 8'h33, 8'hEF, 8'h03, 8'h4F); // IMLHRSTK.0Is0#, LEFT half
    right_hits = 0; left_hits = 0;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_a06.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_a07.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_a08.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_a09.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_a10.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_a11.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_a14.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_a15.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_a16.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_a17.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_a18.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_a19.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_b06.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_b07.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_b08.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_b09.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_b10.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_b11.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_b14.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_b15.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_b16.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_b17.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_b18.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_b19.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_c06.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_c07.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_c08.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_c09.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_c10.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_c11.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_c14.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_c15.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_c16.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_c17.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_c18.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_c19.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_d06.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_d07.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_d08.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_d09.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_d10.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_d11.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_d14.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_d15.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_d16.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_d17.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_d18.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_d19.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_e06.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_e07.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_e08.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_e09.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_e10.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_e11.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_e14.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_e15.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_e16.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_e17.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_e18.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_e19.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_f06.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_f07.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_f08.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_f09.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_f10.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_f11.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_f14.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_f15.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_f16.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_f17.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_f18.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_f19.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_g06.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_g07.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_g08.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_g09.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_g10.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_g11.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_g14.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_g15.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_g16.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_g17.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_g18.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_g19.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_h06.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_h07.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_h08.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_h09.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_h10.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_h11.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_h14.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_h15.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_h16.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_h17.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_h18.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_h19.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_i06.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_i07.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_i08.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_i09.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_i10.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_i11.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_i14.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_i15.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_i16.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_i17.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_i18.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_i19.mem[i]) right_hits = right_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_j06.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_j07.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_j08.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_j09.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_j10.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_j11.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_j14.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_j15.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_j16.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_j17.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_j18.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_j19.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_k06.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_k07.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_k08.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_k09.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_k10.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_k11.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_k14.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_k15.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_k16.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_k17.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_k18.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_k19.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_l06.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_l07.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_l08.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_l09.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_l10.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_l11.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_l14.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_l15.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_l16.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_l17.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_l18.mem[i]) left_hits = left_hits + 1;
    for (i=0;i<1024;i=i+1) if (m.b_ContB.u_l19.mem[i]) left_hits = left_hits + 1;
    $display("tb_writeim: left-half Write-IM  -- %0d right-half cells set, %0d left-half",
             right_hits, left_hits);
    if (left_hits == 0)  $fatal(1, "the left-half Write-IM deposited nothing");
    if (right_hits != 0) $fatal(1, "the half-select failed -- the right half was written too");
    $display("tb_writeim: a jammed Write-IM deposits into IM, and RSTK[3] selects the half.");
    $finish;
  end

endmodule

`default_nettype wire
