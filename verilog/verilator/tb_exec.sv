// tb_exec -- THE MACHINE EXECUTES MICROCODE OUT OF IM.
//
// The rung above `boot0-test`. Real Xerox microcode (AEmu.mb!2, packed into
// PARC's hunks by dorado/tests/boot0_hunks.c) is loaded into IM through the
// BaseBoard's control-processor bus, the MIR clock is released, the start
// address is put into Link, and a jammed `Return#` sends the machine there --
// which is exactly what `LoadDoradoCode` does to start Boot0:
//
//     "Put into Link the address of the first instruction in the Boot0
//      microcode ... Start the Boot0 loader running in the Dorado:
//      LDAI Return# ... JSR RunDoradoInstructionStream"
//
// `RunDoradoInstructionStream` IS `DoDoradoMicroInst` with ShouldSingleStep=0:
// same clears, same four MIR bytes, then Control(SetRun) with SS NOT asserted
// and no BasicStopDorado. The machine free-runs from there.
//
// It then runs: over 20,000 fabric cycles `clk0'` turns 1,242 times, `Stop`
// stays clear, and both `TNIA` and the decoded `FF` field take a spread of
// values as successive microinstructions are fetched and decoded -- 000, 009,
// 003, 010, 001, 00b, 00a ... The machine is sequencing through the microcode
// it was given.
//
// THE PARITY ENABLES MUST BE OFF TO RUN, and finding out why is the useful
// part. With them on the machine executes exactly ONE instruction and stops
// dead with `dStop` = 0, `Run'` = 0 and `Stop` = 1 -- a state that looks
// impossible until you notice `bCLKEnable' = Stop | Run'` gates `clk2'`, which
// is the clock that would clear the stop latch. ONCE `Stop` SETS IT GATES THE
// CLOCK THAT WOULD CLEAR IT, and only `rStop` (ClrStop) gets you out. What set
// it was `Error'` reading 0 at the instant the machine started: the jammed
// `Return#` still in the MIR fails the IM parity check, and `dStop` is
//
//     ~[(bpreStartC'b + ContA31.sil+5) . (bpreStartC'b + Error')]
//
// so a parity error stops the machine regardless of the single-step chain.
//
// THAT IS A DISCREPANCY WITH PARC'S ROM AND IT IS THE NEXT QUESTION.
// `InitManifolds` sets `DisableDoradoErrors` = "all except IM parity errors
// disabled" and leaves them on for the whole boot, jamming IRTable
// instructions constantly. So either PARC's IRTable entries carry parity that
// satisfies the generator -- their five-byte format has explicit P015 and
// P1631 bits, and `Nop#` = 70 01 0F 4C 40 sets both -- or our MC10170 parity
// generators on ContB j20/j21 compute something different. Checking a known
// IRTable entry's parity against j20/j21 by hand is the way in.
//
// MUTATIONS, INCLUDING ONE THAT PASSES. Not loading IM leaves TNIA on ONE
// value for ever (a machine fetching zeros from a wiped array), leaving SS
// asserted stops it after one instruction, and leaving the parity enables on
// does the same. Writing `1E0` instead of `1C0` -- leaving StopMIRClkEn set --
// does NOT fail, and the reason is worth knowing: `StopMIRClk` is
// `NOR(parity-error term, StopMIRClkEn')`, so clearing the PARITY ENABLES
// releases the MIR clock on its own and the explicit release is
// belt-and-braces. It is kept because a boot sequence should say what it
// means, not because this test proves it necessary.
//
// The load half of this file is `boot0-test`'s; see tb_boot0.sv for the hunk
// format, and tb_sendmir.sv for why Control strobes must be spaced.

`default_nettype none


module tb_exec;

  localparam integer GAP = 200;   // sys_clk between Control strobes

  reg sys_clk = 1'b0;
  always #1 sys_clk = ~sys_clk;
  reg [3:0] ckd = 4'd0;
  always @(posedge sys_clk) ckd <= ckd + 4'd1;
  wire mclk = ckd[3];

  reg [2:0] addr_n = 3'b111;
  reg [8:0] cpout  = 9'd0;
  reg       strb_n = 1'b1;
  reg       setrun = 1'b0, setss_n = 1'b1;
  reg       dmd = 1'b0, dmc = 1'b0, udmd = 1'b0;

  // THE MACHINE. Four boards by default -- ContA, ContB, ProcH, ProcL -- which
  // is enough to fetch and decode microinstructions and nothing else. With
  // `+define+WORLD` it is NINE: the memory section, the storage array and the
  // IFU as well, i.e. everything a microcode world needs except the I/O boards
  // and the BaseBoard. The BaseBoard is deliberately absent even there, because
  // this bench drives the control-processor bus itself.
`ifdef WORLD
  // The BaseBoard fans the clock to EVERY slot, so a bigger configuration has
  // more CLK ports. Leaving them undriven gives those boards zero local clock
  // edges with their enables already asserted, which looks exactly like a
  // gating bug and is not one.
  reg [8:0] rfshdiv = 9'd0;
  reg       rfshper = 1'b0;
  always @(posedge sys_clk) begin
    rfshdiv <= rfshdiv + 9'd1;
    if (rfshdiv == 9'd0) rfshper <= ~rfshper;
  end
  // THE MEMORY SIZE IS A BACKPLANE INPUT: MemX takes these from the MSA and
  // they are the CHIP ENABLES on the two DRAM timing PROMs, so with both
  // undriven the memory state machine has no timing table at all. MemProms.bcpl
  // records the build -- from late 1979 the machines carry 16K parts.
  reg chips16k = 1'b1, chips64k = 1'b0;
  dorado_world m (
      .sys_clk(sys_clk),
      .CLK_ca_p_(mclk), .CLK_cb_p_(mclk), .CLK_ph_p_(mclk), .CLK_pl_p_(mclk),
      .CLK_mc_p_(mclk), .CLK_md_p_(mclk), .CLK_mx_p_(mclk),
      .CLK_ms0Even_p_(mclk), .CLK_ifu_p_(mclk),
      .CPAddr_0_p_(addr_n[2]), .CPAddr_1_p_(addr_n[1]), .CPAddr_2_p_(addr_n[0]),
      .CPOut_0(cpout[8]), .CPOut_1(cpout[7]), .CPOut_2(cpout[6]),
      .CPOut_3(cpout[5]), .CPOut_4(cpout[4]), .CPOut_5(cpout[3]),
      .CPOut_6(cpout[2]), .CPOut_7(cpout[1]), .CPOut_8(cpout[0]),
      .CPStrb_p_(strb_n), .SetRun(setrun), .SetSS_p_(setss_n),
      .SetRunRfsh(1'b1), .RfshPeriod(rfshper),
      .ChipsAre256_s_16K(chips16k), .ChipsAre64K(chips64k)
  );

  // ---- SEED MEMORY ---------------------------------------------------------
  //
  // A microcode world reads its INSTRUCTIONS through the memory section, so an
  // unseeded machine is a machine with nothing to run. Both levels are filled:
  // the MSA's eight DRAMs (storage) and MemD's 72 F10470s (the cache), each
  // cell holding that bit's value, so every address answers the same word --
  // the arrangement tb_readback already uses to gate the return path.
  //
  // The pattern is a RUN-TIME argument, `+dpat=` / `+cpat=`, precisely so two
  // different values can be compared: if what executes depends on what memory
  // holds, the world is reading it. That is the whole experiment.
  reg [7:0]  dpat;      // storage, {H,G,F,E,D,C,B,A}
  reg [17:0] cpat;      // cache, cpat[k] is D.k
  integer    dp, cp, si, ci;
  reg        hi_par, lo_par, prog_cache, prog_swap, prog_even;
  integer    prog_fn;
  reg [17:0] cw;
  initial begin
    if (!$value$plusargs("dpat=%d", dp)) dp = 172;      // 8'b1010_1100
    if (!$value$plusargs("cpat=%d", cp)) cp = 52045;
    dpat = dp[7:0]; cpat = cp[17:0];
    // FG PARITY. The cache word is 16 data bits plus TWO parity bits, one per
    // instruction byte -- D.16 and D.17 -- and the IFU checks them as the bytes
    // come through (FGParityErr). An arbitrary seed pattern cannot satisfy
    // that, so with `+cpatpar` the two are COMPUTED from the data instead:
    // D.16 over one byte and D.17 over the other. Which byte pairs with which
    // bit, and whether the convention is even or odd, are the two things not
    // stated anywhere we have read -- so both are switches (`+cpatswap`,
    // `+cpatodd`) and the measurement decides.
    prog_cache = $test$plusargs("cprog");
    prog_swap  = $test$plusargs("cprogswap");
    prog_even  = $test$plusargs("cprogeven");
    if (!$value$plusargs("cprogfn=%d", prog_fn)) prog_fn = 0;
    if (!$test$plusargs("cpatraw")) begin
      // ODD per byte, D.17 over D.08-15 and D.16 over D.00-07. This
      // MINIMISES FGParityErr across six different constant patterns
      // (52045, 49155, 0, 65535, 21845, 4660), all of which give exactly the
      // same residual 176 -- so the level does respond to these bits, and
      // consistently.
      //
      // IT IS NOT ESTABLISHED AS THE HARDWARE CONVENTION, and the honest
      // reason is worth keeping. With `+cprog` giving each line its own word,
      // WHICH assignment wins changes with the DATA FUNCTION: the plain rule
      // is clean for a byte-swapped word (`+cprogfn=3`) and dirty for the
      // plain one, and vice versa, while `+cprogfn=1` is nearly clean under
      // both. No real parity convention can depend on the data.
      //
      // What that means is that this counter is not measuring what it looks
      // like it measures. `IfuMemRef` makes TWO to SIX transitions over the
      // whole run, so there are essentially NO fetches -- and a count of
      // samples where a combinational signal sits high over 200,000 cycles is
      // an IDLE LEVEL, not a per-fetch check. Establishing the convention
      // needs the error sampled AT a fetch, which needs a machine that
      // fetches. See the IFUM parity above for the contrast: RamPe went to
      // EXACTLY ZERO, and IFUM entries are read continuously, so there the
      // level is meaningful.
      hi_par = ~(^cpat[15:8]);     // D.08-15
      lo_par = ~(^cpat[7:0]);      // D.00-07
      cpat[17:16] = {hi_par, lo_par};
    end
    for (si = 0; si < 4096; si = si + 1) begin
      m.b_msa.u_b05.mem[si] = dpat[0];   // A
      m.b_msa.u_c05.mem[si] = dpat[1];   // B
      m.b_msa.u_d05.mem[si] = dpat[2];   // C
      m.b_msa.u_e05.mem[si] = dpat[3];   // D
      m.b_msa.u_e06.mem[si] = dpat[4];   // E
      m.b_msa.u_d06.mem[si] = dpat[5];   // F
      m.b_msa.u_c06.mem[si] = dpat[6];   // G
      m.b_msa.u_b06.mem[si] = dpat[7];   // H
    end
    for (ci = 0; ci < 4096; ci = ci + 1) begin
      // EVERY ADDRESS ANSWERING THE SAME WORD is a machine that fetches ONE
      // opcode for ever, however well the IFU works. With `+cprog` the cache
      // holds a DIFFERENT word per line -- the line number itself -- so the
      // instruction stream varies. Parity is recomputed per word, odd per
      // byte, because a fixed pair of parity bits is wrong for varying data.
      if (prog_cache) begin
        // The WORD each line holds. Varying it is the point: a parity rule
        // that only holds for `ci` is a coincidence of the data, not a rule.
        case (prog_fn)
          1:       cw[15:0] = ci[15:0] ^ 16'h5A5A;
          2:       cw[15:0] = ci[15:0] * 16'd3;
          3:       cw[15:0] = {ci[7:0], ci[15:8]};
          default: cw[15:0] = ci[15:0];
        endcase
        // The pairing could not be pinned by the CONSTANT-pattern sweep: with
        // one word in every line, F and G always agree and the two bytes are
        // fetched from the same value, so a swapped assignment is invisible
        // unless the byte parities differ -- and then "swap" and "invert both"
        // are the same thing. Varying data separates them, so try it here.
        if (prog_swap) begin
          cw[16] = ~(^cw[15:8]);
          cw[17] = ~(^cw[7:0]);
        end else begin
          cw[16] = ~(^cw[7:0]);
          cw[17] = ~(^cw[15:8]);
        end
        if (prog_even) cw[17:16] = ~cw[17:16];
      end else cw = cpat;
      m.b_MemD.u_a03.mem[ci] = cw[0];
      m.b_MemD.u_d03.mem[ci] = cw[0];
      m.b_MemD.u_a05.mem[ci] = cw[1];
      m.b_MemD.u_d05.mem[ci] = cw[1];
      m.b_MemD.u_g03.mem[ci] = cw[2];
      m.b_MemD.u_j03.mem[ci] = cw[2];
      m.b_MemD.u_g05.mem[ci] = cw[3];
      m.b_MemD.u_j05.mem[ci] = cw[3];
      m.b_MemD.u_a13.mem[ci] = cw[4];
      m.b_MemD.u_d13.mem[ci] = cw[4];
      m.b_MemD.u_a15.mem[ci] = cw[5];
      m.b_MemD.u_d15.mem[ci] = cw[5];
      m.b_MemD.u_g13.mem[ci] = cw[6];
      m.b_MemD.u_j13.mem[ci] = cw[6];
      m.b_MemD.u_g15.mem[ci] = cw[7];
      m.b_MemD.u_j15.mem[ci] = cw[7];
      m.b_MemD.u_a09.mem[ci] = cw[8];
      m.b_MemD.u_d09.mem[ci] = cw[8];
      m.b_MemD.u_a11.mem[ci] = cw[9];
      m.b_MemD.u_d11.mem[ci] = cw[9];
      m.b_MemD.u_g09.mem[ci] = cw[10];
      m.b_MemD.u_j09.mem[ci] = cw[10];
      m.b_MemD.u_g11.mem[ci] = cw[11];
      m.b_MemD.u_j11.mem[ci] = cw[11];
      m.b_MemD.u_a17.mem[ci] = cw[12];
      m.b_MemD.u_d17.mem[ci] = cw[12];
      m.b_MemD.u_a19.mem[ci] = cw[13];
      m.b_MemD.u_d19.mem[ci] = cw[13];
      m.b_MemD.u_g17.mem[ci] = cw[14];
      m.b_MemD.u_j17.mem[ci] = cw[14];
      m.b_MemD.u_g19.mem[ci] = cw[15];
      m.b_MemD.u_j19.mem[ci] = cw[15];
      m.b_MemD.u_a07.mem[ci] = cw[16];
      m.b_MemD.u_d07.mem[ci] = cw[16];
      m.b_MemD.u_g07.mem[ci] = cw[17];
      m.b_MemD.u_j07.mem[ci] = cw[17];
    end
    $display("tb_exec: seeded storage dpat=%b cache cpat=%b", dpat, cpat);
  end

`else
  dorado_proc m (
      .sys_clk(sys_clk),
      .CLK_ca_p_(mclk), .CLK_cb_p_(mclk), .CLK_ph_p_(mclk), .CLK_pl_p_(mclk),
      .CPAddr_0_p_(addr_n[2]), .CPAddr_1_p_(addr_n[1]), .CPAddr_2_p_(addr_n[0]),
      .CPOut_0(cpout[8]), .CPOut_1(cpout[7]), .CPOut_2(cpout[6]),
      .CPOut_3(cpout[5]), .CPOut_4(cpout[4]), .CPOut_5(cpout[3]),
      .CPOut_6(cpout[2]), .CPOut_7(cpout[1]), .CPOut_8(cpout[0]),
      .CPStrb_p_(strb_n), .SetRun(setrun), .SetSS_p_(setss_n)
  );

`endif
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
      repeat (4) @(posedge sys_clk);
      strb_n = 1'b0; repeat (6) @(posedge sys_clk);
      strb_n = 1'b1;
      if (fn == 3'd0) begin           // Control: g07 clocks on release
        setrun  =  data[0];
        setss_n = ~ss;
      end
      repeat (4) @(posedge sys_clk);
    end
  endtask

  // SetMufflerAddress: twelve bits, MSB first, then the ManClk pulse that
  // DoClock(UseDMD)/DoClock(0) makes. The first bit shifted in ends up at
  // DMD.00, the last at DMD.11.
  integer k;
  task manifold(input [11:0] word);
    begin
      for (k = 11; k >= 0; k = k - 1) begin
        dmd = word[k];
        repeat (4) @(posedge sys_clk); dmc = 1'b1;
        repeat (4) @(posedge sys_clk); dmc = 1'b0;   // the chain shifts on the fall
        repeat (4) @(posedge sys_clk);
      end
      udmd = 1'b1; repeat (12) @(posedge sys_clk);
      udmd = 1'b0; repeat (12) @(posedge sys_clk);
    end
  endtask

  // BMux and RBMux, most significant bit first as PARC numbers them.
  wire [15:0] bmux = {m.BMux_00, m.BMux_01, m.BMux_02, m.BMux_03,
                      m.BMux_04, m.BMux_05, m.BMux_06, m.BMux_07,
                      m.BMux_08, m.BMux_09, m.BMux_10, m.BMux_11,
                      m.BMux_12, m.BMux_13, m.BMux_14, m.BMux_15};
  wire [15:0] rbmux = {m.b_ContB.RBMux_00, m.b_ContB.RBMux_01, m.b_ContB.RBMux_02,
                       m.b_ContB.RBMux_03, m.b_ContB.RBMux_04, m.b_ContB.RBMux_05,
                       m.b_ContB.RBMux_06, m.b_ContB.RBMux_07, m.b_ContB.RBMux_08,
                       m.b_ContB.RBMux_09, m.b_ContB.RBMux_10, m.b_ContB.RBMux_11,
                       m.b_ContB.RBMux_12, m.b_ContB.RBMux_13, m.b_ContB.RBMux_14,
                       m.b_ContB.RBMux_15};

  integer i, hits, hits_a, hits_b;
  reg [15:0] pat_a, pat_b;

  // Link[4:15] -- twelve bits, the whole IM address space, and exactly what
  // cpu.c takes the Write-IM address from (`link_at_issue & 0xFFF`). PARC
  // numbers the register MSB-first, so .04 is the top bit.
  wire [11:0] link_hi = ~{m.b_ContA.Link_04_p_, m.b_ContA.Link_05_p_,
                          m.b_ContA.Link_06_p_, m.b_ContA.Link_07_p_,
                          m.b_ContA.Link_08_p_, m.b_ContA.Link_09_p_,
                          m.b_ContA.Link_10_p_, m.b_ContA.Link_11_p_,
                          m.b_ContA.Link_12_p_, m.b_ContA.Link_13_p_,
                          m.b_ContA.Link_14_p_, m.b_ContA.Link_15_p_};
  wire [11:0] tnia = {m.TNIA_04, m.TNIA_05, m.TNIA_06, m.TNIA_07,
                      m.TNIA_08, m.TNIA_09, m.TNIA_10, m.TNIA_11,
                      m.TNIA_12, m.TNIA_13, m.TNIA_14, m.TNIA_15};
  // dRA' is the address as ContB's multiplexer leaves it; the MC10211 buffers
  // invert it again into the array's RA lines.
  wire [11:0] dra = ~{m.b_ContB.dRA_00_p_, m.b_ContB.dRA_01_p_,
                      m.b_ContB.dRA_02_p_, m.b_ContB.dRA_03_p_,
                      m.b_ContB.dRA_04_p_, m.b_ContB.dRA_05_p_,
                      m.b_ContB.dRA_06_p_, m.b_ContB.dRA_07_p_,
                      m.b_ContB.dRA_08_p_, m.b_ContB.dRA_09_p_,
                      m.b_ContB.dRA_10_p_, m.b_ContB.dRA_11_p_};

  // Jam CPRegToLink# -- PARC's IRTable, `30 13 EF 04 40`.
  task jam_link(input [15:0] v);
    begin
      setrun = 0; setss_n = 1;
      repeat (400) @(posedge sys_clk);
      strobe(3'd1, 8'h21, 1'b0);
      strobe(3'd2, v[15:8], 1'b0); strobe(3'd3, v[7:0], 1'b0);
      strobe(3'd0, 8'h4E, 1'b0);
      strobe(3'd0, 8'h00, 1'b1);
      strobe(3'd4, 8'h13, 1'b0); strobe(3'd5, 8'hEF, 1'b0);
      strobe(3'd6, 8'h04, 1'b0); strobe(3'd7, 8'h40, 1'b0);
      strobe(3'd0, 8'h41, 1'b1);
      repeat (600) @(posedge sys_clk);
    end
  endtask

  task wipe_im;
    begin
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
    end
  endtask

  task count_im(output integer n);
    begin
      n = 0;
      for (i=0;i<1024;i=i+1) begin
        if (m.b_ContB.u_a10.mem[i]) n = n + 1;
        if (m.b_ContB.u_a11.mem[i]) n = n + 1;
        if (m.b_ContB.u_a14.mem[i]) n = n + 1;
        if (m.b_ContB.u_a15.mem[i]) n = n + 1;
        if (m.b_ContB.u_a16.mem[i]) n = n + 1;
        if (m.b_ContB.u_a17.mem[i]) n = n + 1;
        if (m.b_ContB.u_a18.mem[i]) n = n + 1;
        if (m.b_ContB.u_a19.mem[i]) n = n + 1;
        if (m.b_ContB.u_b10.mem[i]) n = n + 1;
        if (m.b_ContB.u_b11.mem[i]) n = n + 1;
        if (m.b_ContB.u_b14.mem[i]) n = n + 1;
        if (m.b_ContB.u_b15.mem[i]) n = n + 1;
        if (m.b_ContB.u_b16.mem[i]) n = n + 1;
        if (m.b_ContB.u_b17.mem[i]) n = n + 1;
        if (m.b_ContB.u_b18.mem[i]) n = n + 1;
        if (m.b_ContB.u_b19.mem[i]) n = n + 1;
        if (m.b_ContB.u_c10.mem[i]) n = n + 1;
        if (m.b_ContB.u_c11.mem[i]) n = n + 1;
        if (m.b_ContB.u_c14.mem[i]) n = n + 1;
        if (m.b_ContB.u_c15.mem[i]) n = n + 1;
        if (m.b_ContB.u_c16.mem[i]) n = n + 1;
        if (m.b_ContB.u_c17.mem[i]) n = n + 1;
        if (m.b_ContB.u_c18.mem[i]) n = n + 1;
        if (m.b_ContB.u_c19.mem[i]) n = n + 1;
        if (m.b_ContB.u_d10.mem[i]) n = n + 1;
        if (m.b_ContB.u_d11.mem[i]) n = n + 1;
        if (m.b_ContB.u_d14.mem[i]) n = n + 1;
        if (m.b_ContB.u_d15.mem[i]) n = n + 1;
        if (m.b_ContB.u_d16.mem[i]) n = n + 1;
        if (m.b_ContB.u_d17.mem[i]) n = n + 1;
        if (m.b_ContB.u_d18.mem[i]) n = n + 1;
        if (m.b_ContB.u_d19.mem[i]) n = n + 1;
        if (m.b_ContB.u_e06.mem[i]) n = n + 1;
        if (m.b_ContB.u_e07.mem[i]) n = n + 1;
        if (m.b_ContB.u_e08.mem[i]) n = n + 1;
        if (m.b_ContB.u_e09.mem[i]) n = n + 1;
        if (m.b_ContB.u_e10.mem[i]) n = n + 1;
        if (m.b_ContB.u_e11.mem[i]) n = n + 1;
        if (m.b_ContB.u_e14.mem[i]) n = n + 1;
        if (m.b_ContB.u_e15.mem[i]) n = n + 1;
        if (m.b_ContB.u_e16.mem[i]) n = n + 1;
        if (m.b_ContB.u_e17.mem[i]) n = n + 1;
        if (m.b_ContB.u_e18.mem[i]) n = n + 1;
        if (m.b_ContB.u_e19.mem[i]) n = n + 1;
        if (m.b_ContB.u_f10.mem[i]) n = n + 1;
        if (m.b_ContB.u_f11.mem[i]) n = n + 1;
        if (m.b_ContB.u_f14.mem[i]) n = n + 1;
        if (m.b_ContB.u_f15.mem[i]) n = n + 1;
        if (m.b_ContB.u_f16.mem[i]) n = n + 1;
        if (m.b_ContB.u_f17.mem[i]) n = n + 1;
        if (m.b_ContB.u_f18.mem[i]) n = n + 1;
        if (m.b_ContB.u_f19.mem[i]) n = n + 1;
        if (m.b_ContB.u_g10.mem[i]) n = n + 1;
        if (m.b_ContB.u_g11.mem[i]) n = n + 1;
        if (m.b_ContB.u_g14.mem[i]) n = n + 1;
        if (m.b_ContB.u_g15.mem[i]) n = n + 1;
        if (m.b_ContB.u_g16.mem[i]) n = n + 1;
        if (m.b_ContB.u_g17.mem[i]) n = n + 1;
        if (m.b_ContB.u_g18.mem[i]) n = n + 1;
        if (m.b_ContB.u_g19.mem[i]) n = n + 1;
        if (m.b_ContB.u_h10.mem[i]) n = n + 1;
        if (m.b_ContB.u_h11.mem[i]) n = n + 1;
        if (m.b_ContB.u_h14.mem[i]) n = n + 1;
        if (m.b_ContB.u_h15.mem[i]) n = n + 1;
        if (m.b_ContB.u_h16.mem[i]) n = n + 1;
        if (m.b_ContB.u_h17.mem[i]) n = n + 1;
        if (m.b_ContB.u_h18.mem[i]) n = n + 1;
        if (m.b_ContB.u_h19.mem[i]) n = n + 1;
        if (m.b_ContB.u_i16.mem[i]) n = n + 1;
        if (m.b_ContB.u_i17.mem[i]) n = n + 1;
        if (m.b_ContB.u_i18.mem[i]) n = n + 1;
        if (m.b_ContB.u_i19.mem[i]) n = n + 1;
      end
    end
  endtask

  // Jam CPRegToIM# (PARC's IRTable, `20 13 EF 03 4F`) with `v` in CPReg.
  task jam_write(input [15:0] v);
    begin
      // Stop first. With the MIR clock held, a running machine repeats the
      // jammed instruction forever, and the next jam has to start from rest.
      setrun = 0; setss_n = 1;
      repeat (400) @(posedge sys_clk);
      strobe(3'd1, 8'h21, 1'b0);                       // Clock: InhibitCAHolds+ClrReady
      strobe(3'd2, v[15:8], 1'b0); strobe(3'd3, v[7:0], 1'b0);
      strobe(3'd0, 8'h4E, 1'b0);
      strobe(3'd0, 8'h00, 1'b1);
      strobe(3'd4, 8'h13, 1'b0); strobe(3'd5, 8'hEF, 1'b0);
      strobe(3'd6, 8'h03, 1'b1); strobe(3'd7, 8'h4F, 1'b0);
      strobe(3'd0, 8'h41, 1'b1);
      repeat (600) @(posedge sys_clk);
    end
  endtask

  integer n0, n1, n2, j, s;
  reg p0, p1, p2;
  always @(posedge sys_clk) begin
    if (m.b_ContA.clk0_p_Ca !== p0) begin n0 = n0 + 1; p0 = m.b_ContA.clk0_p_Ca; end
    if (m.b_ContA.clk1_p_Ca !== p1) begin n1 = n1 + 1; p1 = m.b_ContA.clk1_p_Ca; end
    if (m.b_ContA.clk2_p_Bc !== p2) begin n2 = n2 + 1; p2 = m.b_ContA.clk2_p_Bc; end
  end
  task zero; begin n0 = 0; n1 = 0; n2 = 0; end endtask
  task settle; begin repeat (200) @(posedge sys_clk); end endtask

  // Jam a microinstruction and take the first step: ClrStop+ClrMIR+ClrCT+
  // Freeze, then the four MIR bytes, then SetRun with SS and no ClrStop.
  task jam_step(input [7:0] b0, input [7:0] b1, input [7:0] b2,
                input [7:0] b3, input [7:0] b4);
    begin
      setrun = 0; setss_n = 1;
      repeat (400) @(posedge sys_clk);
      strobe(3'd1, 8'h21, 1'b0); repeat (GAP) @(posedge sys_clk);
      strobe(3'd0, 8'h4E, 1'b0);
      repeat (GAP) @(posedge sys_clk);
      strobe(3'd0, 8'h00, 1'b1);
      repeat (GAP) @(posedge sys_clk);
      strobe(3'd4, b1, b0[7]); strobe(3'd5, b2, b0[6]);
      strobe(3'd6, b3, b0[5]); strobe(3'd7, b4, b0[4]);
      strobe(3'd0, 8'h01, 1'b1);
    end
  endtask

  // One more microinstruction, jam untouched. PARC's three strobes; what the
  // machine needs is the FIRST plus at least one more without ClrStop.
  task step_again;
    begin
      strobe(3'd0, 8'h41, 1'b1);
      strobe(3'd0, 8'h01, 1'b1);
      strobe(3'd0, 8'h00, 1'b1);
    end
  endtask


  reg [11:0] link_seen;
  integer total_written;

  // Bits set at ONE within-bank index, across the right half's packages.
  task scan_one(input integer idx);
    begin
      i = idx;
      hits = 0;
        if (m.b_ContB.u_a10.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_a11.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_a14.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_a15.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_a16.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_a17.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_a18.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_a19.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_b10.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_b11.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_b14.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_b15.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_b16.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_b17.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_b18.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_b19.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_c10.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_c11.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_c14.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_c15.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_c16.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_c17.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_c18.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_c19.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_d10.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_d11.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_d14.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_d15.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_d16.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_d17.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_d18.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_d19.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_e06.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_e07.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_e08.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_e09.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_e10.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_e11.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_e14.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_e15.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_e16.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_e17.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_e18.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_e19.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_f10.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_f11.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_f14.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_f15.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_f16.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_f17.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_f18.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_f19.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_g10.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_g11.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_g14.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_g15.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_g16.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_g17.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_g18.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_g19.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_h10.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_h11.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_h14.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_h15.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_h16.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_h17.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_h18.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_h19.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_i16.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_i17.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_i18.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_i19.mem[i]) hits = hits + 1;
    end
  endtask

  task scan_all;
    begin
      total_written = 0;
      for (i = 0; i < 1024; i = i + 1) begin
        scan_one(i);
        if (hits != 0) begin
          $display("   IM[%0d] (0x%h): %0d right-half bits", i, i[11:0], hits);
          total_written = total_written + hits;
        end
      end
    end
  endtask

  task count_at(input integer addr, output integer n);
    begin scan_one(addr); n = hits; end
  endtask

  // ---- PARC's own routines, transcribed from doradoboot.masm/doradocpint.masm

  // SetCPReg~ : "LDA ToCPRegH / EORI 0ff / ... / LDA ToCPRegL / EORI 0ff".
  // The TILDE IS THE POINT -- the BaseBoard writes the COMPLEMENT, because
  // ContA b02's MC10159 inverts on the way to BMux. Write ~v and the datapath
  // delivers v.
  task set_cpreg_tilde(input [15:0] v);
    begin
      strobe(3'd2, ~v[15:8], 1'b0);
      strobe(3'd3, ~v[7:0],  1'b0);
    end
  endtask

  // DoDoradoMicroInst, single-step variant: DoClock(InhibitCAHolds+ClrReady),
  // Control(ClrStop+ClrMIR+ClrCT+Freeze) SS=0, Control(0) SS=1, the four MIR
  // bytes, Control(SetRun) SS=1, then BasicStopDorado's Control(SetRun) SS=1
  // and Control(0) SS=1.
  task parc_micro(input [7:0] b0, input [7:0] b1, input [7:0] b2,
                  input [7:0] b3, input [7:0] b4);
    begin
      strobe(3'd1, 8'h21, 1'b0); repeat (GAP) @(posedge sys_clk);
      strobe(3'd0, 8'h4E, 1'b0);
      repeat (GAP) @(posedge sys_clk);
      strobe(3'd0, 8'h00, 1'b1);
      repeat (GAP) @(posedge sys_clk);
      strobe(3'd4, b1, b0[7]); strobe(3'd5, b2, b0[6]);
      strobe(3'd6, b3, b0[5]); strobe(3'd7, b4, b0[4]);
      zero;
      // The real BaseBoard is a 1 MHz 6502 running JSR DoControl between
      // these; the gap is MICROSECONDS, not the fourteen sys_clk a testbench
      // takes. GAP models that.
      strobe(3'd0, 8'h01, 1'b1);   // Control(SetRun), SS
      repeat (GAP) @(posedge sys_clk);
      strobe(3'd0, 8'h01, 1'b1);               // BasicStopDorado
      repeat (GAP) @(posedge sys_clk);
      strobe(3'd0, 8'h00, 1'b1);
      repeat (800) @(posedge sys_clk);
      $display("      micro %02h: clk0' %0d clk1' %0d clk2' %0d | Stop=%b Link=%h FF=%b",
        b1, n0, n1, n2, m.b_ContA.Stop, link_hi,
        ~{m.b_ContA.FF_0_p_,m.b_ContA.FF_1_p_,m.b_ContA.FF_2_p_,m.b_ContA.FF_3_p_,
          m.b_ContA.FF_4_p_,m.b_ContA.FF_5_p_,m.b_ContA.FF_6_p_,m.b_ContA.FF_7_p_});
    end
  endtask

  task nop_micro;
    begin parc_micro(8'h70, 8'h01, 8'h0F, 8'h4C, 8'h40); end
  endtask

  // SendViaMIR: the address into Link (with a Nop after it, which "holds CPReg
  // constant through T3 of the previous instruction"), then the data and the
  // IMStoreTable entry for this half.
  //   IMStoreTable[0] IMLHRSTK.0Is0#   60 33 EF 03 4F
  //   IMStoreTable[1] IMLHRSTK.0Is1#   20 73 EF 03 4F
  //   IMStoreTable[2] IMRHBLOCKIs0#    20 13 EF 03 4F
  //   IMStoreTable[3] IMRHBLOCKIs1#    60 53 EF 03 4F
  task send_via_mir(input [15:0] imaddr, input [15:0] word,
                    input right_half, input extra);
    begin
      set_cpreg_tilde(imaddr);
      parc_micro(8'h30, 8'h13, 8'hEF, 8'h04, 8'h40);   // CPRegToLink#
      nop_micro;                                        // ...AndNop
      // The address is in Link HERE. The store below carries FF=176 too and
      // reloads Link with its own DATA -- the link_at_issue hazard -- so this
      // is the only moment it can be checked.
      link_seen = link_hi;
      if (link_seen !== imaddr[11:0])
        $fatal(1, "CPRegToLink# did not put the address in Link");
      set_cpreg_tilde(word);
      if (!right_half && !extra) parc_micro(8'h60, 8'h33, 8'hEF, 8'h03, 8'h4F);
      if (!right_half &&  extra) parc_micro(8'h20, 8'h73, 8'hEF, 8'h03, 8'h4F);
      if ( right_half && !extra) parc_micro(8'h20, 8'h13, 8'hEF, 8'h03, 8'h4F);
      if ( right_half &&  extra) parc_micro(8'h60, 8'h53, 8'hEF, 8'h03, 8'h4F);
    end
  endtask


  function [15:0] rd_L0(input integer idx);
    rd_L0 = {m.b_ContB.u_j06.mem[idx], m.b_ContB.u_k06.mem[idx], m.b_ContB.u_l06.mem[idx], m.b_ContB.u_i10.mem[idx], m.b_ContB.u_j10.mem[idx], m.b_ContB.u_k10.mem[idx], m.b_ContB.u_l10.mem[idx], m.b_ContB.u_f06.mem[idx], m.b_ContB.u_g06.mem[idx], m.b_ContB.u_h06.mem[idx], m.b_ContB.u_j16.mem[idx], m.b_ContB.u_k16.mem[idx], m.b_ContB.u_l16.mem[idx], m.b_ContB.u_a06.mem[idx], m.b_ContB.u_b06.mem[idx], m.b_ContB.u_c06.mem[idx]};
  endfunction
  function [15:0] rd_R0(input integer idx);
    rd_R0 = {m.b_ContB.u_a10.mem[idx], m.b_ContB.u_b10.mem[idx], m.b_ContB.u_c10.mem[idx], m.b_ContB.u_d10.mem[idx], m.b_ContB.u_a16.mem[idx], m.b_ContB.u_b16.mem[idx], m.b_ContB.u_c16.mem[idx], m.b_ContB.u_d16.mem[idx], m.b_ContB.u_e06.mem[idx], m.b_ContB.u_e10.mem[idx], m.b_ContB.u_f10.mem[idx], m.b_ContB.u_g10.mem[idx], m.b_ContB.u_h10.mem[idx], m.b_ContB.u_g16.mem[idx], m.b_ContB.u_h16.mem[idx], m.b_ContB.u_i16.mem[idx]};
  endfunction
  function [15:0] rd_L1(input integer idx);
    rd_L1 = {m.b_ContB.u_j07.mem[idx], m.b_ContB.u_k07.mem[idx], m.b_ContB.u_l07.mem[idx], m.b_ContB.u_i11.mem[idx], m.b_ContB.u_j11.mem[idx], m.b_ContB.u_k11.mem[idx], m.b_ContB.u_l11.mem[idx], m.b_ContB.u_f07.mem[idx], m.b_ContB.u_g07.mem[idx], m.b_ContB.u_h07.mem[idx], m.b_ContB.u_j17.mem[idx], m.b_ContB.u_k17.mem[idx], m.b_ContB.u_l17.mem[idx], m.b_ContB.u_a07.mem[idx], m.b_ContB.u_b07.mem[idx], m.b_ContB.u_c07.mem[idx]};
  endfunction
  function [15:0] rd_R1(input integer idx);
    rd_R1 = {m.b_ContB.u_a11.mem[idx], m.b_ContB.u_b11.mem[idx], m.b_ContB.u_c11.mem[idx], m.b_ContB.u_d11.mem[idx], m.b_ContB.u_a17.mem[idx], m.b_ContB.u_b17.mem[idx], m.b_ContB.u_c17.mem[idx], m.b_ContB.u_d17.mem[idx], m.b_ContB.u_e07.mem[idx], m.b_ContB.u_e11.mem[idx], m.b_ContB.u_f11.mem[idx], m.b_ContB.u_g11.mem[idx], m.b_ContB.u_h11.mem[idx], m.b_ContB.u_g17.mem[idx], m.b_ContB.u_h17.mem[idx], m.b_ContB.u_i17.mem[idx]};
  endfunction
  function [15:0] rd_L2(input integer idx);
    rd_L2 = {m.b_ContB.u_j08.mem[idx], m.b_ContB.u_k08.mem[idx], m.b_ContB.u_l08.mem[idx], m.b_ContB.u_i14.mem[idx], m.b_ContB.u_j14.mem[idx], m.b_ContB.u_k14.mem[idx], m.b_ContB.u_l14.mem[idx], m.b_ContB.u_f08.mem[idx], m.b_ContB.u_g08.mem[idx], m.b_ContB.u_h08.mem[idx], m.b_ContB.u_j18.mem[idx], m.b_ContB.u_k18.mem[idx], m.b_ContB.u_l18.mem[idx], m.b_ContB.u_a08.mem[idx], m.b_ContB.u_b08.mem[idx], m.b_ContB.u_c08.mem[idx]};
  endfunction
  function [15:0] rd_R2(input integer idx);
    rd_R2 = {m.b_ContB.u_a14.mem[idx], m.b_ContB.u_b14.mem[idx], m.b_ContB.u_c14.mem[idx], m.b_ContB.u_d14.mem[idx], m.b_ContB.u_a18.mem[idx], m.b_ContB.u_b18.mem[idx], m.b_ContB.u_c18.mem[idx], m.b_ContB.u_d18.mem[idx], m.b_ContB.u_e08.mem[idx], m.b_ContB.u_e14.mem[idx], m.b_ContB.u_f14.mem[idx], m.b_ContB.u_g14.mem[idx], m.b_ContB.u_h14.mem[idx], m.b_ContB.u_g18.mem[idx], m.b_ContB.u_h18.mem[idx], m.b_ContB.u_i18.mem[idx]};
  endfunction
  function [15:0] rd_L3(input integer idx);
    rd_L3 = {m.b_ContB.u_j09.mem[idx], m.b_ContB.u_k09.mem[idx], m.b_ContB.u_l09.mem[idx], m.b_ContB.u_i15.mem[idx], m.b_ContB.u_j15.mem[idx], m.b_ContB.u_k15.mem[idx], m.b_ContB.u_l15.mem[idx], m.b_ContB.u_f09.mem[idx], m.b_ContB.u_g09.mem[idx], m.b_ContB.u_h09.mem[idx], m.b_ContB.u_j19.mem[idx], m.b_ContB.u_k19.mem[idx], m.b_ContB.u_l19.mem[idx], m.b_ContB.u_a09.mem[idx], m.b_ContB.u_b09.mem[idx], m.b_ContB.u_c09.mem[idx]};
  endfunction
  function [15:0] rd_R3(input integer idx);
    rd_R3 = {m.b_ContB.u_a15.mem[idx], m.b_ContB.u_b15.mem[idx], m.b_ContB.u_c15.mem[idx], m.b_ContB.u_d15.mem[idx], m.b_ContB.u_a19.mem[idx], m.b_ContB.u_b19.mem[idx], m.b_ContB.u_c19.mem[idx], m.b_ContB.u_d19.mem[idx], m.b_ContB.u_e09.mem[idx], m.b_ContB.u_e15.mem[idx], m.b_ContB.u_f15.mem[idx], m.b_ContB.u_g15.mem[idx], m.b_ContB.u_h15.mem[idx], m.b_ContB.u_g19.mem[idx], m.b_ContB.u_h19.mem[idx], m.b_ContB.u_i19.mem[idx]};
  endfunction

  function sec_L0(input integer idx);
    sec_L0 = m.b_ContB.u_i06.mem[idx];
  endfunction
  function sec_R0(input integer idx);
    sec_R0 = m.b_ContB.u_f16.mem[idx];
  endfunction
  function sec_L1(input integer idx);
    sec_L1 = m.b_ContB.u_i07.mem[idx];
  endfunction
  function sec_R1(input integer idx);
    sec_R1 = m.b_ContB.u_f17.mem[idx];
  endfunction
  function sec_L2(input integer idx);
    sec_L2 = m.b_ContB.u_i08.mem[idx];
  endfunction
  function sec_R2(input integer idx);
    sec_R2 = m.b_ContB.u_f18.mem[idx];
  endfunction
  function sec_L3(input integer idx);
    sec_L3 = m.b_ContB.u_i09.mem[idx];
  endfunction
  function sec_R3(input integer idx);
    sec_R3 = m.b_ContB.u_f19.mem[idx];
  endfunction

  // The SECONDARY bit of a half -- RSTK[0] on the left, BLOCK on the right --
  // which the array stores beside the sixteen data bits. Checking only the
  // data leaves it unverified: a mutation that reversed the ExtraBits shift
  // passed until this was added.
  function sec_at(input integer bank, input right, input integer idx);
    begin
      sec_at = 1'bx;
      if (!right) begin
        case (bank)
          0: sec_at = sec_L0(idx); 1: sec_at = sec_L1(idx);
          2: sec_at = sec_L2(idx); 3: sec_at = sec_L3(idx);
        endcase
      end else begin
        case (bank)
          0: sec_at = sec_R0(idx); 1: sec_at = sec_R1(idx);
          2: sec_at = sec_R2(idx); 3: sec_at = sec_R3(idx);
        endcase
      end
    end
  endfunction

  // Search every bank for a word at one index. Returns the bank, or -1.
  function integer find_word(input integer idx, input right, input [15:0] want);
    begin
      find_word = -1;
      if (!right) begin
        if (rd_L0(idx) === want) find_word = 0;
        else if (rd_L1(idx) === want) find_word = 1;
        else if (rd_L2(idx) === want) find_word = 2;
        else if (rd_L3(idx) === want) find_word = 3;
      end else begin
        if (rd_R0(idx) === want) find_word = 0;
        else if (rd_R1(idx) === want) find_word = 1;
        else if (rd_R2(idx) === want) find_word = 2;
        else if (rd_R3(idx) === want) find_word = 3;
      end
    end
  endfunction

  // ---- SendAHunk ----------------------------------------------------------
  reg [7:0]  hunk [0:16];
  reg [7:0]  extrabits;
  integer    yy, kk2;
  reg [15:0] imaddr;

  task send_a_hunk(input [15:0] start_addr);
    begin
      extrabits = hunk[0];
      yy = 1;
      imaddr = start_addr;
      for (kk2 = 0; kk2 < 4; kk2 = kk2 + 1) begin
        // left half, then right, both at the SAME IMAddress
        send_via_mir(imaddr, {hunk[yy], hunk[yy+1]}, 1'b0, extrabits[7]);
        extrabits = {extrabits[6:0], 1'b0}; yy = yy + 2;
        send_via_mir(imaddr, {hunk[yy], hunk[yy+1]}, 1'b1, extrabits[7]);
        extrabits = {extrabits[6:0], 1'b0}; yy = yy + 2;
        imaddr = imaddr + 16'd1;
      end
    end
  endtask

  integer bnk, nfound, w;
  reg [15:0] expect_l [0:3];
  reg [15:0] expect_r [0:3];

  // ---- REAL MICROCODE, from the C emulator's own .MB loader ---------------
  integer fd, nf, hunks, halves, bad;
  string  path, tag, line;
  integer ha;
  reg [7:0]  b [0:16];
  reg [15:0] hw [0:7];


  // RunDoradoInstructionStream: DoDoradoMicroInst with ShouldSingleStep = 0 --
  // DoClock(0), the clears, the four MIR bytes, then Control(SetRun) with SS
  // NOT asserted, and no BasicStopDorado. The machine free-runs from there.
  task parc_run(input [7:0] b0, input [7:0] b1, input [7:0] b2,
                input [7:0] b3, input [7:0] b4);
    begin
      strobe(3'd1, 8'h00, 1'b0); repeat (GAP) @(posedge sys_clk);
      strobe(3'd0, 8'h4E, 1'b0);
      repeat (GAP) @(posedge sys_clk);
      strobe(3'd0, 8'h00, 1'b1);
      repeat (GAP) @(posedge sys_clk);
      strobe(3'd4, b1, b0[7]); strobe(3'd5, b2, b0[6]);
      strobe(3'd6, b3, b0[5]); strobe(3'd7, b4, b0[4]);
      // SS off, and GIVE IT TIME: ContA i03's chain needs three RunClk' edges
      // to walk the deasserted SS through before SetRun lands.
      setss_n = 1;
      repeat (GAP) @(posedge sys_clk);
      strobe(3'd0, 8'h01, 1'b0);
    end
  endtask

  integer hcount;
  integer n0a, n1a, j2, nff;
  // How many DISTINCT addresses and decoded FF fields go by -- a machine
  // fetching from wiped IM would show one of each for ever.
  integer n_tnia, n_ff, q;
  reg [11:0] tnia_seen [0:31];
  reg [7:0]  ff_seen   [0:31];
  wire [11:0] tnia_now = {m.TNIA_04,m.TNIA_05,m.TNIA_06,m.TNIA_07,m.TNIA_08,
                          m.TNIA_09,m.TNIA_10,m.TNIA_11,m.TNIA_12,m.TNIA_13,
                          m.TNIA_14,m.TNIA_15};
  wire [7:0] ff_now = ~{m.b_ContA.FF_0_p_,m.b_ContA.FF_1_p_,m.b_ContA.FF_2_p_,
                        m.b_ContA.FF_3_p_,m.b_ContA.FF_4_p_,m.b_ContA.FF_5_p_,
                        m.b_ContA.FF_6_p_,m.b_ContA.FF_7_p_};

  // ---- PRELOAD: a WHOLE WORLD in IM ---------------------------------------
  //
  // The control-processor bus above is the REAL loader and boot0-test gates it,
  // but 4,096 microinstructions of it will not simulate in reasonable time --
  // sixteen hunks already take a couple of million fabric cycles. So with
  // `+preload=<file>` the arrays are written directly, through the map
  // tools/sil_im_map.py derives from ContB's own wire list and boot0-test
  // checks against a CP-bus load of the same microcode.
  //
  // NOTHING IS TAKEN ON TRUST: every address written is read back through the
  // same map and compared against the file before the machine is started. A
  // wrong interleave gives a machine executing garbage, which is
  // INDISTINGUISHABLE from a boot that fails for its own reasons.
`include "im_preload.vh"
  reg [3:0] f_rstk [0:4095];   reg [3:0] f_aluf [0:4095];
  reg [2:0] f_bsel [0:4095];   reg [2:0] f_lc   [0:4095];
  reg [2:0] f_asel [0:4095];   reg [7:0] f_ff   [0:4095];
  reg [7:0] f_jcn  [0:4095];   reg       f_blk  [0:4095];
  reg       f_have [0:4095];
  reg [3:0] r_rstk, r_aluf;  reg [2:0] r_bsel, r_lc, r_asel;
  reg [7:0] r_ff, r_jcn;     reg r_blk;
  reg [1023:0] impath;
  reg [31:0] ia, ib, ic, id, ie, ig, ih, ii, ij;
  integer imfd, imn, mapaddr, nloaded, nver, nverbad, runcycles, startaddr;
  integer nalufm, nalufmbad, nifum, nifumbad;
  reg [5:0]  r_alu;
  reg [15:0] r_lo, r_hi, ifum_want;  reg ifum_raw;
  integer n_ifuref, n_ifuack, n_jchg;
  reg ifu_prev_ref, ifu_prev_ack; reg [7:0] j_prev;
  // THE INSTRUCTION BYTE STREAM, the path tb_ifufetch traced on MemD:
  //   cache -> e06 (F register, clocked by Fclk'a) -> f22 (G, loaded on GLd')
  //   -> f23 (the 4:1 mux, enabled by EnableFG') -> FG.0-8 -> the IFU
  // tb_ifufetch found this never clocks in a SYNTHETIC four-instruction loop
  // and said the reason was that memory could not serve the reference. A real
  // world issues real IFetches, so measuring it here is a new datum.
  integer n_fclk, n_gld, n_fg, n_enfg;
  // IFU PARITY. The run visits AEMUIFURAMPE and AEMUIFUFGPARITY early, and the
  // IM parity generators are already a known open question (the machine stops
  // dead unless exec-test clears the IM parity enables). IFUM carries three
  // RamParity bits which the preload takes verbatim from the .MB -- so if our
  // generator computes something different, the IFU raises a RAM parity error
  // and stops fetching, which is exactly the shape measured.
  integer n_rampe, n_sawram, n_fgpe, n_sawfg;
  // THE CACHE IS TWO HALVES, and the netlist says so plainly: a03 and d03 both
  // drive D.00, but a03 takes Dad0.10-12 with enable D0ACE' and d03 takes
  // Dad1.10-12 with enable D1ACE'. They are separate 4096x1 arrays wired-OR
  // onto one data bus, so exactly ONE should be enabled at a time. If both
  // are, a CONSTANT seed hides it completely -- both halves hold the same word
  // -- and varying data does not. That is the candidate for +cprog failing FG
  // parity, so count it rather than argue about it.
  integer n_d0, n_d1, n_both, n_neither, n_addrdiff;
  // DOES THE IFU PIPELINE EVER GET STARTED? cpu.c on FA=1 FB=0 FC=0: "PCF <- B
  // ... Loads PCF and starts the IFU pipeline. The pipeline needs ~5 cycles to
  // fill before the first IFUJump can succeed (HM page 67). Until then,
  // IFUJump traps to *34-37 NotReady." Our visited set is FULL of octal 10-17,
  // which is exactly that trap region -- so count the instruction that starts
  // it. FF = {FA,FB,FC} = {1,0,0} = 0o100.
  integer n_pcf;
  // IS THE MIR FROZEN? At the lock the decoded FF is 073 while IM[0o100] holds
  // 144, so the machine is NOT executing what IM contains -- which is what a
  // held MIR looks like: constant FF, constant TNIA, clk0' still running, no
  // hold asserted. This bench's header states StopMIRClk = NOR(parity-error
  // term, StopMIRClkEn'), and THE PRELOAD WRITES IM PARITY AS ZERO for every
  // word, which is wrong for most of them. The enables are cleared at startup;
  // whether the error TERM still reaches the MIR clock is the question.
  integer n_smc, n_smce, n_imlhpe, n_imrhpe, n_err;
  // A SHORT PC TRACE. Counters say how often; a trace says in what ORDER, and
  // for "why is it stuck" that is the difference between theorising and
  // looking. First N microinstructions only -- the interesting part is the
  // approach to the loop, not the loop.
  //
  // READ THE TWO COLUMNS SEPARATELY UNTIL THE OFFSET IS PINNED. `TNIA` and the
  // decoded `FF` are sampled from DIFFERENT PIPELINE STAGES, so the pair
  // printed on one line is not one instruction: the first three entries line
  // up as (next-address of A, FF of A-1), and the FF that appears beside the
  // stuck PC is not that address's own field. The PC column is the one to
  // trust for the path; pairing them needs the offset established first, and
  // reading them as one instruction produced a wrong lookup.
  integer n_tr, koff, nmatch, ncmp;
  reg [11:0] tr_pc [0:255]; reg [7:0] tr_ff [0:255];
  reg fclk_prev, gld_prev; reg [8:0] fg_prev;
  // HOW MUCH OF THE WORLD ACTUALLY EXECUTES. The distinct-value lists above cap
  // at 32, which is fine for "is it sequencing at all" and useless for "how far
  // does it get". A bitmap over the whole address space answers the second.
  reg       visited [0:4095];
  integer   nvisited, lastpc, stuck, maxrun, prevpc, maxpc;

`ifdef WORLD
  wire [8:0] fg_now = {m.FG_0, m.FG_1, m.FG_2, m.FG_3, m.FG_4,
                       m.FG_5, m.FG_6, m.FG_7, m.FG_8};
  wire [7:0] j_now = {m.b_IFU.J_0a, m.b_IFU.J_1a, m.b_IFU.J_2a, m.b_IFU.J_3a,
                      m.b_IFU.J_4a, m.b_IFU.J_5a, m.b_IFU.J_6a, m.b_IFU.J_7a};
`endif
  initial begin
    force m.DMuxData = dmd;
    force m.DMuxClk  = dmc;
    force m.UseDMD   = udmd;
    repeat (2000) @(posedge sys_clk);
    manifold(12'h030);
    manifold(12'h1E0);                    // SetMidasStopMIRClk ON, to load
    p0 = m.b_ContA.clk0_p_Ca; p1 = m.b_ContA.clk1_p_Ca; p2 = m.b_ContA.clk2_p_Bc;
    zero;
    wipe_im;
    nop_micro; nop_micro;

    nloaded = 0; nver = 0; nverbad = 0;
    if ($value$plusargs("preload=%s", impath)) begin
      imfd = $fopen(impath, "r");
      if (imfd == 0) $fatal(1, "cannot open %s", impath);
      for (i = 0; i < 4096; i = i + 1) f_have[i] = 1'b0;
      nalufm = 0; nalufmbad = 0; nifum = 0; nifumbad = 0;
      ifum_raw = $test$plusargs("ifumraw");
      // $fgets RETURNS 0 AT END OF FILE and leaves `line` alone, so the
      // `while (!$feof(...))` idiom processes the LAST line TWICE -- which
      // counted 17 ALUFM entries out of a 16-entry file. Gate on the read.
      while ($fgets(line, imfd) != 0) begin
        // addr rstk aluf bsel lc asel BLOCK ff jcn -- nine values after the tag.
        imn = $sscanf(line, "%s %h %h %h %h %h %h %h %h %h",
                      tag, ia, ib, ic, id, ie, ig, ij, ih, ii);
        // ALUFM -- 16 entries of 6 bits, ProcL e13/e14. ALUF is a POINTER into
        // this memory, so a world loaded without it runs on whatever the array
        // powered up with. Same file, its own tag.
        // NOTE $sscanf RETURNS -1 ON A PARTIAL MATCH, not the number of
        // conversions it made -- while still filling the arguments it DID
        // convert. A three-field ALUFM line read with a ten-field format comes
        // back as -1 with tag, ia and ib all correct, so gating on the count
        // silently skipped every one of them. Gate on the TAG.
        // IFUM -- 1024 x 27 on the IFU board, two words per entry because the
        // board has two write enables. Without it the IFU cannot decode an
        // opcode at all. GUARDED: the four-board machine has no IFU, and
        // tb_boot0 and the default tb_exec include the same generated map.
`ifdef WORLD
        if (tag == "IFUM") begin
          // THE .MB DOES NOT CARRY IFUM PARITY -- the real machine computes it
          // in its LOAD microcode (ifuRamSubrs.mc:ifuAddParity), and 248 of
          // AEmu's 256 entries fail cpu.c's own check as stored. Copying them
          // verbatim leaves the IFU in a permanent RAM parity error. The
          // fourth column is the fields word with the three IPar bits
          // recomputed; `+ifumraw` takes the stored ones instead, so the two
          // can be compared.
          ifum_want = ifum_raw ? ic[15:0] : id[15:0];
          ifum_preload_word(ia[9:0], ib[15:0], ifum_want);
          ifum_readback_word(ia[9:0], r_lo, r_hi);
          nifum = nifum + 1;
          // DecHi' holds only bits 05-15 of word 0, so compare that mask.
          if ((r_lo & 16'h07FF) !== (ib[15:0] & 16'h07FF) || r_hi !== ifum_want) begin
            nifumbad = nifumbad + 1;
            if (nifumbad < 6)
              $display("tb_exec: IFUM[%h] read %h/%h want %h/%h",
                       ia[9:0], r_lo, r_hi, ib[15:0], ifum_want);
          end
          continue;
        end
`endif
        if (tag == "ALUFM") begin
          alufm_preload_word(ia[3:0], ib[5:0]);
          alufm_readback_word(ia[3:0], r_alu);
          nalufm = nalufm + 1;
          if (r_alu !== ib[5:0]) begin
            nalufmbad = nalufmbad + 1;
            $display("tb_exec: ALUFM[%h] read %h want %h", ia[3:0], r_alu, ib[5:0]);
          end
          continue;
        end
        if (imn != 10 || tag != "IM") continue;
        mapaddr = ia[11:0];
        f_rstk[mapaddr] = ib[3:0];  f_aluf[mapaddr] = ic[3:0];
        f_bsel[mapaddr] = id[2:0];  f_lc[mapaddr]   = ie[2:0];
        f_asel[mapaddr] = ig[2:0];  f_ff[mapaddr]   = ih[7:0];
        f_jcn[mapaddr]  = ii[7:0];  f_blk[mapaddr]  = ij[0];
        f_have[mapaddr] = 1'b1;
        // dBlock' -- the array stores the COMPLEMENT (tb_boot0 measured all 64
        // right-half secondary bits inverted; BLOCK is the one field bit the
        // MIR wires through Q' rather than Q).
        im_preload_word(mapaddr, ib[3:0], ic[3:0], id[2:0], ie[2:0], ig[2:0],
                        ih[7:0], ii[7:0], ~ij[0]);
        nloaded = nloaded + 1;
      end
      $fclose(imfd);

      // READ IT ALL BACK. This is the part not to skip.
      for (i = 0; i < 4096; i = i + 1) begin
        if (!f_have[i]) continue;
        im_readback_word(i, r_rstk, r_aluf, r_bsel, r_lc, r_asel, r_ff, r_jcn, r_blk);
        nver = nver + 1;
        if (r_rstk !== f_rstk[i] || r_aluf !== f_aluf[i] || r_bsel !== f_bsel[i] ||
            r_lc !== f_lc[i] || r_asel !== f_asel[i] || r_ff !== f_ff[i] ||
            r_jcn !== f_jcn[i] || r_blk !== ~f_blk[i]) begin
          nverbad = nverbad + 1;
          if (nverbad < 8)
            $display("tb_exec: PRELOAD IM[0x%h] mismatch: rstk %h/%h ff %h/%h jcn %h/%h blk %b/%b",
                     i[11:0], r_rstk, f_rstk[i], r_ff, f_ff[i], r_jcn, f_jcn[i],
                     r_blk, ~f_blk[i]);
        end
      end
      $display("tb_exec: PRELOAD -- %0d microinstructions written, %0d read back, %0d wrong",
               nloaded, nver, nverbad);
      $display("tb_exec: PRELOAD -- %0d ALUFM entries written, %0d wrong", nalufm, nalufmbad);
`ifdef WORLD
      $display("tb_exec: PRELOAD -- %0d IFUM entries written, %0d wrong", nifum, nifumbad);
      if (nifumbad != 0) $fatal(1, "IFUM does not hold what was preloaded");
`endif
      if (nalufmbad != 0) $fatal(1, "ALUFM does not hold what was preloaded");
      if (nloaded == 0)  $fatal(1, "the preload file held no microinstructions");
      if (nverbad != 0)  $fatal(1, "IM does not hold what was preloaded");
    end else begin
    // Load real microcode at its own addresses.
    if (!$value$plusargs("vectors=%s", path)) path = "boot0.vec";
    fd = $fopen(path, "r");
    if (fd == 0) $fatal(1, "cannot open %s", path);
    hcount = 0;
    while (!$feof(fd) && hcount < 4) begin
      void'($fgets(line, fd));
      nf = $sscanf(line,
        "%s %h %h %h %h %h %h %h %h %h %h %h %h %h %h %h %h %h %h %h %h %h %h %h %h %h %h",
        tag, ha, b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7], b[8],
        b[9], b[10], b[11], b[12], b[13], b[14], b[15], b[16],
        hw[0], hw[1], hw[2], hw[3], hw[4], hw[5], hw[6], hw[7]);
      if (nf != 27 || tag != "HUNK") continue;
      for (w = 0; w < 17; w = w + 1) hunk[w] = b[w];
      send_a_hunk(ha[15:0]);
      hcount = hcount + 1;
    end
    $fclose(fd);
    $display("tb_exec: loaded %0d hunks (%0d microinstructions)", hcount, hcount*4);
    end

    // RELEASE the MIR clock -- register 7, data bit 0 -- so the MIR can reload
    // from IM. Without this the jam is held and nothing is ever fetched.
    manifold(12'h1C0);
    manifold(12'h000);          // ParityEnables := 0 -- IM parity OFF
    $display("tb_exec: parity enables IMLH=%b IMRH=%b, StopMIRClkEn=%b StopMIRClk=%b",
             m.b_ContB.IMLHPEenable, m.b_ContB.IMRHPEenable,
             m.b_ContB.StopMIRClkEn, m.StopMIRClk);

    // Put the start address in Link, then Return# to jump there and run.
    // The .MB carries no entry point -- that comes from the loader's STAMP, not
    // the file (mb2eb takes `start 01070` as a separate argument). So the start
    // address is an input here; 0 is what this bench has always used.
    if (!$value$plusargs("start=%d", startaddr)) startaddr = 0;
    $display("tb_exec: starting at IM[0x%h]", startaddr[11:0]);
    set_cpreg_tilde(startaddr[15:0]);
    parc_micro(8'h30, 8'h13, 8'hEF, 8'h04, 8'h40);   // CPRegToLink#
    nop_micro;
    $display("tb_exec: Link[4:15]=%h", link_hi);

    parc_run(8'h60, 8'h13, 8'hE1, 8'h42, 8'h43);      // Return#, free-running

    n0a = 0; n1a = 0; nff = 0; n_tnia = 0; n_ff = 0;
`ifdef WORLD
    n_ifuref = 0; n_ifuack = 0; n_jchg = 0;
    ifu_prev_ref = m.b_IFU.IfuMemRef; ifu_prev_ack = m.b_IFU.IfuMemAck; j_prev = j_now;
    n_fclk = 0; n_gld = 0; n_fg = 0; n_enfg = 0;
    n_rampe = 0; n_sawram = 0; n_fgpe = 0; n_sawfg = 0;
    n_d0 = 0; n_d1 = 0; n_both = 0; n_neither = 0; n_addrdiff = 0;
    n_pcf = 0; n_tr = 0;
    n_smc = 0; n_smce = 0; n_imlhpe = 0; n_imrhpe = 0; n_err = 0;
    fclk_prev = m.b_MemD.Fclk_p_a; gld_prev = m.b_MemD.GLd_p_; fg_prev = fg_now;
`endif
    for (i = 0; i < 4096; i = i + 1) visited[i] = 1'b0;
    nvisited = 0; stuck = 0; maxrun = 0; prevpc = -1; maxpc = -1;
    p0 = m.b_ContA.clk0_p_Ca; p1 = m.b_ContA.clk1_p_Ca;
    if (!$value$plusargs("cycles=%d", runcycles)) runcycles = 20000;
    for (j2 = 0; j2 < runcycles; j2 = j2 + 1) begin
      @(posedge sys_clk);
      if (m.b_ContA.clk0_p_Ca !== p0) begin
        n0a = n0a + 1; p0 = m.b_ContA.clk0_p_Ca;
        if (p0 === 1'b1) begin           // one sample per microinstruction
          lastpc = tnia_now;
          if (!visited[lastpc[11:0]]) begin
            visited[lastpc[11:0]] = 1'b1; nvisited = nvisited + 1;
          end
          if (n_tr < 256) begin
            tr_pc[n_tr] = lastpc[11:0]; tr_ff[n_tr] = ff_now; n_tr = n_tr + 1;
          end
          if (lastpc === prevpc) begin
            stuck = stuck + 1;
            // WHICH address it spins on, not just how long. A spin is a
            // microcode wait loop, and naming the address turns it into
            // something mbdis can disassemble.
            if (stuck > maxrun) begin maxrun = stuck; maxpc = lastpc; end
          end else stuck = 0;
          prevpc = lastpc;
        end
      end
      if (m.b_ContA.clk1_p_Ca !== p1) begin n1a = n1a + 1; p1 = m.b_ContA.clk1_p_Ca; end
      if (n_tnia < 32) begin
        for (q = 0; q < n_tnia; q = q + 1)
          if (tnia_seen[q] === tnia_now) q = 1000;
        if (q < 1000) begin tnia_seen[n_tnia] = tnia_now; n_tnia = n_tnia + 1; end
      end
      if (ff_now === 8'o100) n_pcf = n_pcf + 1;
      if (m.StopMIRClk)              n_smc    = n_smc + 1;
      if (m.b_ContB.StopMIRClkEn)    n_smce   = n_smce + 1;
      if (!m.b_ContB.IMLHPE_p_)      n_imlhpe = n_imlhpe + 1;
      if (!m.b_ContB.IMRHPE_p_)      n_imrhpe = n_imrhpe + 1;
      if (!m.b_ContA.Error_p_)       n_err    = n_err + 1;
      if (n_ff < 32) begin
        for (q = 0; q < n_ff; q = q + 1)
          if (ff_seen[q] === ff_now) q = 1000;
        if (q < 1000) begin ff_seen[n_ff] = ff_now; n_ff = n_ff + 1; end
      end
`ifdef WORLD
      // IS THE IFU ACTUALLY FETCHING? The opcode it decodes is J.0-7, and the
      // reference it makes to get it is IfuMemRef/IfuMemAck. If the opcode
      // never changes, the machine is dispatching on a stuck value rather than
      // on an instruction stream -- which is exactly the difference between
      // "it reaches an opcode handler" and "it is running a program".
      if (m.b_IFU.IfuMemRef !== ifu_prev_ref) begin
        n_ifuref = n_ifuref + 1; ifu_prev_ref = m.b_IFU.IfuMemRef;
      end
      if (m.b_IFU.IfuMemAck !== ifu_prev_ack) begin
        n_ifuack = n_ifuack + 1; ifu_prev_ack = m.b_IFU.IfuMemAck;
      end
      if (j_now !== j_prev) begin n_jchg = n_jchg + 1; j_prev = j_now; end
      if (m.b_MemD.Fclk_p_a !== fclk_prev) begin
        n_fclk = n_fclk + 1; fclk_prev = m.b_MemD.Fclk_p_a;
      end
      if (m.b_MemD.GLd_p_ !== gld_prev) begin n_gld = n_gld + 1; gld_prev = m.b_MemD.GLd_p_; end
      if (fg_now !== fg_prev) begin n_fg = n_fg + 1; fg_prev = fg_now; end
      if (!m.b_MemD.EnableFG_p_) n_enfg = n_enfg + 1;
      if (m.b_IFU.RamPe)             n_rampe  = n_rampe + 1;
      if (m.b_IFU.SawRamParityErr)   n_sawram = n_sawram + 1;
      if (!m.b_IFU.FGParityErr_p_)   n_fgpe   = n_fgpe + 1;
      if (m.b_IFU.SawFGParityErr)    n_sawfg  = n_sawfg + 1;
      if (!m.b_MemD.D0ACE_p_a &&  m.b_MemD.D1ACE_p_a) n_d0 = n_d0 + 1;
      if ( m.b_MemD.D0ACE_p_a && !m.b_MemD.D1ACE_p_a) n_d1 = n_d1 + 1;
      if (!m.b_MemD.D0ACE_p_a && !m.b_MemD.D1ACE_p_a) n_both = n_both + 1;
      if ( m.b_MemD.D0ACE_p_a &&  m.b_MemD.D1ACE_p_a) n_neither = n_neither + 1;
      // ...and whether the two halves are even addressed alike.
      if ({m.b_MemD.Dad0_10a, m.b_MemD.Dad0_11a, m.b_MemD.Dad0_12a} !==
          {m.b_MemD.Dad1_10a, m.b_MemD.Dad1_11a, m.b_MemD.Dad1_12a})
        n_addrdiff = n_addrdiff + 1;
      // SITTING ON ONE ADDRESS IS THE SIGNATURE OF A HOLD, not of a self-jump:
      // the memory section freezes the processor on the same microinstruction
      // until the reference completes. `PRhold` (ProcH/ProcL) and `PrHold`
      // (MemC) are ONE BACKPLANE WIRE spelled two ways -- so print who is
      // asking, and what the memory section is doing about it.
      if (j2 % (runcycles/10) == 0)
        $display("tb_exec: HOLD: Hold=%b PRhold=%b CBHold=%b IfuHold=%b IOHold=%b MXHold=%b DisHold=%b CHoldReq=%b",
                 m.Hold, m.PRhold, m.CBHold, m.IfuHold, m.IOHold, m.MXHold,
                 m.DisHold, m.CHoldReq);
`endif
      if (j2 % (runcycles/10) == 0)
        $display("      Error'=%b IMLHPE'=%b IMRHPE'=%b A31+5=%b dStop=%b Run'=%b",
          m.b_ContA.Error_p_, m.b_ContB.IMLHPE_p_, m.b_ContB.IMRHPE_p_,
          m.b_ContA.ContA31_sil_pl_5, m.b_ContA.dStop, m.b_ContA.Run_p_);
      if (j2 % (runcycles/10) == 0)
        $display("   t=%5d  clk0'=%0d  FF=%b RSTK=%b%b%b%b TNIA=%h Stop=%b",
          j2, n0a,
          ~{m.b_ContA.FF_0_p_,m.b_ContA.FF_1_p_,m.b_ContA.FF_2_p_,m.b_ContA.FF_3_p_,
            m.b_ContA.FF_4_p_,m.b_ContA.FF_5_p_,m.b_ContA.FF_6_p_,m.b_ContA.FF_7_p_},
          m.RSTK_0, m.RSTK_1, m.RSTK_2, m.RSTK_3,
          {m.TNIA_04,m.TNIA_05,m.TNIA_06,m.TNIA_07,m.TNIA_08,m.TNIA_09,
           m.TNIA_10,m.TNIA_11,m.TNIA_12,m.TNIA_13,m.TNIA_14,m.TNIA_15},
          m.b_ContA.Stop);
    end
    $display("tb_exec: %0d clk0' edges, %0d clk1' over %0d cycles, Stop=%b",
             n0a, n1a, runcycles, m.b_ContA.Stop);
    $display("tb_exec: %0d distinct TNIA values, %0d distinct FF values seen",
             n_tnia, n_ff);
`ifdef WORLD
    // PIN THE OFFSET, rather than eyeballing it. The preload already loaded
    // every address's FF from the .MB, so the question "how many stages apart
    // are TNIA and the decoded FF" is answerable by correlation: for each
    // candidate offset, count the samples where the observed FF equals the FF
    // the .MB gives for the address at that offset. The right offset should
    // stand far above the others; if none does, the two are not simply skewed
    // and pairing them at all is wrong.
    for (koff = -3; koff <= 3; koff = koff + 1) begin
      nmatch = 0; ncmp = 0;
      for (i = 0; i < n_tr; i = i + 1) begin
        if (i + koff < 0 || i + koff >= n_tr) continue;
        if (!f_have[tr_pc[i + koff]]) continue;
        // ONLY WHERE THE PC MOVES. 240 of the 256 samples are the identical
        // stuck pair, and counting them drowns the signal in a constant
        // mismatch -- the first attempt scored 14 of 255 at best and looked
        // like "no offset fits" when it really meant "the sample is 94% one
        // repeated point".
        if (i > 0 && tr_pc[i] === tr_pc[i-1]) continue;
        ncmp = ncmp + 1;
        if (tr_ff[i] === f_ff[tr_pc[i + koff]]) nmatch = nmatch + 1;
      end
      $display("tb_exec: OFFSET %0d: FF matches the .MB at pc[i%0s%0d] on %0d of %0d",
               koff, (koff < 0) ? "-" : "+", (koff < 0) ? -koff : koff, nmatch, ncmp);
    end
    $write("tb_exec: TRACE (pc/ff, first %0d):", n_tr);
    for (i = 0; i < n_tr; i = i + 1) $write(" %h/%o", tr_pc[i], tr_ff[i]);
    $write("\n");
    $display("tb_exec: MIR -- StopMIRClk %0d, StopMIRClkEn %0d, IMLHPE %0d, IMRHPE %0d, Error %0d, of %0d",
             n_smc, n_smce, n_imlhpe, n_imrhpe, n_err, runcycles);
    $display("tb_exec: PCF -- `PCF<-B` (FF=0o100, starts the IFU pipeline) seen on %0d samples",
             n_pcf);
    $display("tb_exec: CACHE -- D0 only %0d, D1 only %0d, BOTH enabled %0d, neither %0d, addr differs %0d, of %0d",
             n_d0, n_d1, n_both, n_neither, n_addrdiff, runcycles);
    $display("tb_exec: IFUPE -- RamPe high %0d, SawRamParityErr %0d, FGParityErr %0d, SawFGParityErr %0d, of %0d",
             n_rampe, n_sawram, n_fgpe, n_sawfg, runcycles);
    $display("tb_exec: BYTES -- Fclk'a %0d edges, GLd' %0d, FG changed %0d, EnableFG' low on %0d of %0d",
             n_fclk, n_gld, n_fg, n_enfg, runcycles);
    $display("tb_exec: IFU -- IfuMemRef %0d transitions, IfuMemAck %0d, opcode J changed %0d times",
             n_ifuref, n_ifuack, n_jchg);
`endif
    $display("tb_exec: %0d distinct IM addresses executed; last TNIA=%h; longest run on one address=%0d at TNIA=%h",
             nvisited, lastpc[11:0], maxrun, maxpc[11:0]);
    // Name them. A short cycle is the normal shape of a microcode wait loop, and
    // knowing WHICH addresses turns "it loops" into something disassemblable.
    if (nvisited <= 64) begin
      $write("tb_exec: addresses executed:");
      for (i = 0; i < 4096; i = i + 1) if (visited[i]) $write(" %h", i[11:0]);
      $write("\n");
    end
    if (m.b_ContA.Stop !== 1'b0)
      $fatal(1, "the machine stopped -- with the parity enables on it stops after one instruction");
    if (n0a < runcycles/20)
      $fatal(1, "the microinstruction clock is not free-running");
    // ONE OF EACH PER MICROINSTRUCTION, to within the window boundary. The
    // sample window is a fixed number of FABRIC cycles, so it can close
    // between clk0' and clk1' of the same microinstruction and leave the
    // counts one apart -- which is the property holding, not failing. This
    // used to demand exact equality, and that is a boundary artifact rather
    // than a real difference: at 8 sys_clk per microinstruction it read
    // 2492 against 2493 out of ~2492.
    if (n1a > n0a + 1 || n0a > n1a + 1)
      $fatal(1, "clk1' must run with clk0', one of each per microinstruction (%0d vs %0d)",
             n0a, n1a);
    // A machine fetching from WIPED IM would sit on one instruction for ever.
    if (n_tnia < 4)
      $fatal(1, "TNIA is not sequencing -- the machine is not fetching from IM");
    if (n_ff < 4)
      $fatal(1, "the decoded FF field never changes -- nothing is being fetched");
    $display("tb_exec: the machine executes microcode out of IM.");
    $finish;
  end

endmodule

`default_nettype wire
