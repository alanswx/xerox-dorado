// tb_compute -- THE MACHINE COMPUTES. PARC's own ALU prologue, replayed on
// four boards: a value crosses from the BaseBoard into CPReg, onto the B bus,
// into Q, and out of Q into ALUFM -- ALUFM reads back the function PARC says
// it wrote, and a second value then goes through the ALU into T.
//
// LoadDoradoCode does this before it loads a single word of microcode, because
// `TFromCPReg#` carries the warning "requires ALUFM[0]=B" -- T is loaded
// THROUGH the ALU, so entry 0 has to hold the function that passes B first:
//
//     "Set up ALUFM[0] with a 25o, which is the logical function B"
//     LDXI 25o / LDAI QFromCPReg#-IRTable / JSR SetCPAndDoIRTableInst
//     LDAI ALUFM[0]FromQ#-IRTable / JSR DoIRTableInst
//
// `alufm0_is_b` below is that, instruction for instruction. Note
// SetCPAndDoIRTableInst, NOT the tilde form: the ALU function code goes in
// uncomplemented.
//
// WHAT THIS GATES, and why each check is here rather than a pixel-style
// "something moved" count:
//
//   Q == 0015 after QFromCPReg#      25 octal, the value PARC sends.
//   Q == 0015 still, one instruction later.
//   ALUFM[0] == 010101               25 octal read MSB-first, off the F10145A
//                                    storage at ProcL e13/e14 -- not off a bus.
//   T == 1234 and T == a55a          loaded THROUGH the ALU, exact, both values.
//   all 24 ALU functions              A from T, B from CPReg, result back into
//                                     T -- every logical and arithmetic entry
//                                     of HM Table 9, against the C emulator.
//   RM at four addresses              written, read back, and landing where
//                                     the address pins say -- not just
//                                     round-tripping through a permutation.
//
// EACH ONE CAUGHT A CELL BUG, and neither bug was visible to any other gate --
// all eighteen passed with both of them in place.
//
//   Q's value catches cell_MC10141. The four-bit universal shift register had
//   its PARALLEL ENTRY wired Q0<-D3, Q1<-D0, Q2<-D1, Q3<-D2, so every nibble
//   loaded ROTATED ONE PLACE: 0015 arrived as 008a, which is what this probe
//   measured for weeks. The datasheet truth table (two Motorola books, they
//   agree) says Parallel Entry is Q0<-D0 .. Q3<-D3, flat. 60 packages.
//
//   Q's SURVIVAL catches cell_MC10119. The four-wide OR-AND gate's pin 10 is
//   shared by two of its OR groups -- it is a 4-3-3-3 part with only twelve
//   input pins -- and modelling it as a standalone AND term instead pulled the
//   whole output low whenever pin 10 was low. On ProcH b17/c17 pin 10 is
//   FA=0', so EVERY microinstruction with FA=0, including PARC's own Nop,
//   asserted QshiftL'/QshiftR'. Both asserted is PARALLEL ENTRY on the
//   MC10141, so Q was reloaded from a dead B bus every single cycle and could
//   not survive to be stored. 10 packages.
//
// THE PIPELINE IS REAL AND THE NOPS ARE NOT PADDING. The Qshift controls come
// out of a register (ProcH b15) clocked by Clock1', while the MC10141's own
// clock QClock' is gated off PreClock1' -- which is EARLIER. So the controls in
// force at any load edge are the ones the PREVIOUS instruction latched:
// decode in cycle N, act in cycle N+1. That is why PARC follows each of these
// with a Nop, and why its comment for DoIRTableInstAndNop -- "the Nop holds
// CPReg constant through T3 of the previous instruction" -- is load-bearing.
// Q is not loaded by QFromCPReg#; it is loaded by the Nop after it.
//
// TWO OPERANDS, AND THE ALU AS THE BOARDS WIRE IT. Everything above moves ONE
// value. The sweep at the end computes: A from T, B from CPReg, the function
// from ALUFM, result back into T. It reaches the A side by changing exactly
// one field of PARC's `TFromCPReg#` -- ASEL[4] (A<-RM/STK, which is why alua
// read 0000 in every earlier probe) becomes ASEL[6] (A<-T), so byte 4 goes C0
// -> E0. That the byte layout in `doradoboot.masm` reproduces PARC's own C0
// for ASEL[4] is the check that the encoding is right.
//
// This is the first thing to exercise the ALU IN THE MACHINE. `alu-diff`
// matches the C emulator on 10,752 vectors but builds four MC10181 slices in a
// TESTBENCH; here they are ProcH's and ProcL's own, with the carry chain
// running f61 -> e61 -> across the backplane, and all 24 entries of HM Table 9
// agree with the C emulator's alu_op() -- including A+B, which only comes out
// right if carries ripple across all four slices.
//
// THE ALUFM ENTRY IS NOT A CONTIGUOUS FIELD OF B. HM Table 11d says
// "ALUFMEM <- B.8, B[11:15]": the entry's MSB, which is the ALU's CARRY IN,
// comes from B.08, and the other five from B[11:15]. Every LOGICAL entry is
// <= 037 octal and so lands entirely in those low five bits, which is why the
// logical half of this sweep passed while `A+1` did not -- writing the carry
// at bit 5 of B puts it nowhere. `alufm_word()` below does the mapping, and
// cpu.c documents the same one from the same table.
//
// RM, THE PER-TASK REGISTER FILE. The last block of the datapath. `LC[6]` is
// "RM/STK <- Pd", so with ALUFM[0] = B the operand goes straight from CPReg
// into RM; reading it back takes `ASEL[4]` (A<-RM/STK) with ALUFM[0] = 37
// octal (A) and `LC[1]`. Four addresses, four values, written and read.
//
// A WRITE-THEN-READ ALONE WOULD PROVE LITTLE, so the PHYSICAL address is
// checked too. RM's address pins take `RbAdr.0-.3` unprimed but
// `RbAdr.4'-.7'` PRIMED, and a mis-modelled polarity there is a consistent
// PERMUTATION -- it reads back perfectly and is still wrong. That is exactly
// the trap that hid the IM address reversal until Boot0 compared IM with
// something external, and nothing external ever sees RM.
//
// What the hardware actually does: **the low four address bits are ~RSTK**.
// `RbAdr.4'` is driven by an MC1662 NOR (ProcH k08) from `RSTK.0` -- a 2:1
// read/write address mux whose other input is `RbWadr` -- so the RSTK half of
// the address reaches the RAM complemented, which is what its primed name
// says. Three independent things agree: `mir-diff` proves RSTK is right in the
// MIR, the wire list shows the NOR, and this measures ~RSTK end to end. It is
// harmless to the machine (a permutation within each RBase bank) but it MUST
// be applied if RM is ever diffed against the C emulator, whose RM[n] is
// index n.
//
// The high four bits are RBase and are only checked for CONSTANCY here -- that
// RSTK does not bleed into them. They come from an MC10231's TRUE output on
// ProcL (g10) across the backplane and read 15 in this configuration; nothing
// in the IRTable loads RBase, so which value that represents is not settled.
//
// WHAT THIS GATE DOES NOT COVER, found by mutating the fixes it exists to
// catch. Swapping the MC10141's two SHIFT modes against each other still
// passes here, because PARC's prologue only ever uses PARALLEL ENTRY -- Q's
// shift paths are what the microcode's shift/cycle operations use, and nothing
// in the suite drives one yet. The corrected cell is right on three
// independent sources (both Motorola books, and EclDict's own [FF] arcs, which
// pair DR/pin 5 with Q3/pin 3 and DL/pin 13 with Q0/pin 14 -- the mutation
// contradicts all three), but it is ARGUED, not gated. Mechanising it is a
// contained task: `sil_check_cells.py` already parses the [FF] lines for their
// CLK and RS pins and throws the per-output data-input arcs away, and those
// arcs are exactly a per-output dependency list for the clocked parts, the
// same check it already applies to combinational ones.
//
// T LOADS THROUGH THE ALU AND TAKES THE OPERAND EXACTLY -- 1234 gives 1234
// and a55a gives a55a. It briefly looked COMPLEMENTED, and that was the
// probe's own fault: it drove CPReg with `SetCPReg~` (which writes the
// complement) and then compared T against the value BEFORE that complement.
//
// THE CONVENTION, which is worth having straight because it explains PARC's
// own code. BMux carries the complement of CPReg; the ALU's B operand `alub`
// is taken off BMux through an MC1662 NOR, which inverts it back; the ALU
// passes it (ALUFM[0] = 25 octal = "the logical function B"); and T is loaded
// from that. So T ends up equal to CPReg, while a value travelling the
// un-inverted path -- IM write data -- ends up equal to the complement of
// CPReg. THAT IS WHY PARC USES A DIFFERENT SETTER FOR EACH: `SendViaMIR`
// sends IM data with `SetCPReg~`, and `PrepareProcessor` loads T with the
// plain `SetCPReg`:
//
//     LDXI 103o / LDAI TFromCPReg#-IRTable / JSR SetCPAndDoIRTableInst
//
// This gate drives T the plain way, as PARC does, and asserts the exact value.
// Reversing EITHER of the MC10173's two conventions -- SB's sense, or which
// clock level is transparent -- makes it fail, so both are gated here rather
// than merely read off the datasheet.

`default_nettype none


module tb_compute;

  // Try the prologue both ways: +define+TILDE selects SetCPReg~.
`ifdef TILDE
  localparam USE_TILDE = 1'b1;
`else
  localparam USE_TILDE = 1'b0;
`endif

  localparam integer GAP = 200;   // sys_clk between Control strobes
  // PARC loads T with the PLAIN SetCPReg (PrepareProcessor). +define+TTILDE
  // runs it the other way for comparison.
`ifdef TTILDE
  localparam T_PLAIN = 1'b0;
`else
  localparam T_PLAIN = 1'b1;
`endif

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
  integer mk, bk, n_aleg, mbad;
  wire [15:0] r_reg = {m.b_ProcH.R_00, m.b_ProcH.R_01, m.b_ProcH.R_02, m.b_ProcH.R_03,
                      m.b_ProcH.R_04, m.b_ProcH.R_05, m.b_ProcH.R_06, m.b_ProcH.R_07,
                      m.b_ProcL.R_08, m.b_ProcL.R_09, m.b_ProcL.R_10, m.b_ProcL.R_11,
                      m.b_ProcL.R_12, m.b_ProcL.R_13, m.b_ProcL.R_14, m.b_ProcL.R_15};
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

  // PARC's PLAIN SetCPReg -- the byte as it stands. `SetCPReg~` complements
  // (EORI 0ff); this does not, and PrepareProcessor uses THIS one for
  // TFromCPReg#: "LDXI 103o / LDAI TFromCPReg# / JSR SetCPAndDoIRTableInst".
  task set_cpreg_plain(input [15:0] v);
    begin
      strobe(3'd2, v[15:8], 1'b0);
      strobe(3'd3, v[7:0],  1'b0);
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

  // A GENERAL MICROINSTRUCTION, from the byte layout doradoboot.masm states:
  //   0: RSTK.0, P015, JCN.7, P1631, 0,0,0,0
  //   1: RSTK.1, RSTK.2, RSTK.3, ALUF.0, BLOCK, FF.0, FF.1, FF.2
  //   2: ALUF.1, ALUF.2, ALUF.3, BSEL.0, FF.3, FF.4, FF.5, FF.6
  //   3: BSEL.1, BSEL.2, LC.0, LC.1, FF.7, JCN.0, JCN.1, JCN.2
  //   4: LC.2, ASEL.0, ASEL.1, ASEL.2, JCN.3, JCN.4, JCN.5, JCN.6
  // PARC numbers every field MSB-FIRST, so RSTK.0 is the MSB of rstk and
  // JCN.7 the LSB of jcn. This encoder reproduces ALL THIRTEEN IRTable entries
  // byte for byte (parity bits aside, which PARC states per instruction), so
  // it is checked against PARC's own hand-coding rather than trusted.
  // P015/P1631 are left at 1: a jammed instruction fails IM parity anyway --
  // that is the jam mechanism -- and the enables are cleared here.
  function [39:0] mi(input [3:0] rstk, input [3:0] aluf, input [2:0] bsel,
                     input [2:0] lc,   input [2:0] asel, input [7:0] ff,
                     input [7:0] jcn,  input block);
    reg [7:0] b0, b1, b2, b3, b4;
    begin
      b0 = {rstk[3], 1'b1,    jcn[0],  1'b1,    4'b0000};
      b1 = {rstk[2], rstk[1], rstk[0], aluf[3], block, ff[7], ff[6], ff[5]};
      b2 = {aluf[2], aluf[1], aluf[0], bsel[2], ff[4], ff[3], ff[2], ff[1]};
      b3 = {bsel[1], bsel[0], lc[2],   lc[1],   ff[0], jcn[7], jcn[6], jcn[5]};
      b4 = {lc[0],   asel[2], asel[1], asel[0], jcn[4], jcn[3], jcn[2], jcn[1]};
      mi = {b0, b1, b2, b3, b4};
    end
  endfunction

  task jam_mi(input [39:0] m);
    begin parc_micro(m[39:32], m[31:24], m[23:16], m[15:8], m[7:0]); end
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


  reg [15:0] bmux_after_jam;

  // ALUFM is ProcL e13/e14 -- two F10145A 16x4 register files giving the six
  // bits {Cn,S3,S2,S1,S0,M} the MC10181 takes. Read the entry directly, and
  // watch ALUFWrite'.
  integer nalufw; reg palufw;
  reg [5:0] alub_at_write; reg [15:0] q_at_write;

  // The RM address AS THE CELL SEES IT, latched when the write strobe asserts.
  // h06's address pins are RbAdr.0-.3 then RbAdr.4'-.7' -- the high half
  // arrives PRIMED, which is why this is measured rather than assumed.
  wire [7:0] rbadr_now = {m.b_ProcH.RbAdr_0, m.b_ProcH.RbAdr_1,
                          m.b_ProcH.RbAdr_2, m.b_ProcH.RbAdr_3,
                          m.b_ProcH.RbAdr_4_p_, m.b_ProcH.RbAdr_5_p_,
                          m.b_ProcH.RbAdr_6_p_, m.b_ProcH.RbAdr_7_p_};
  reg [7:0] rbadr_at_write; reg rbw_d = 1'b1;
  always @(posedge sys_clk) begin
    rbw_d <= m.b_ProcH.RbWrite_p_a;
    if (rbw_d && !m.b_ProcH.RbWrite_p_a) rbadr_at_write <= rbadr_now;
  end
  reg [15:0] q_after_load, q_after_hold;
  reg [15:0] t_first, t_second, t_before_op;
  integer fi, ai; reg [5:0] afn; reg [15:0] aexp;
  reg [3:0] rstk_n; reg [15:0] rm_val; integer rm_seen;
  reg [3:0] rbase_seen;          // measured, not assumed
  reg seen_addr [0:255];
  localparam [15:0] AV = 16'hA55A, BV = 16'h1234;
  // ALUFM entry 0 as PARC numbers it: ALUFdec.0 is the HIGH bit.
  // e13 stores ALUFdec.0-.3 in its Q0-Q3, e14 stores .4-.5 -- and .0 is the
  // HIGH bit, so the MSB of the entry is e13's Q0 and the LSB is e14's Q1.
  wire [5:0] alufm0 = {m.b_ProcL.u_e13.mem[0][0], m.b_ProcL.u_e13.mem[0][1],
                       m.b_ProcL.u_e13.mem[0][2], m.b_ProcL.u_e13.mem[0][3],
                       m.b_ProcL.u_e14.mem[0][0], m.b_ProcL.u_e14.mem[0][1]};

  // TRACE the T chain, hop by hop, for the low four bits. The path is
  //   alua/alub -> MC10181 alu.nn -> Pdata.nn -> (k01, Md select) -> dTm.nn
  //   -> l03 F10145A register file, addressed by the TASK NUMBER -> dT.nn
  //   -> (i03, TbBypass) -> T.nn
  // so an inversion anywhere shows up as one hop where the nibble flips.
  task show_tchain(input [63:0] tag);
    $display("      %0s alub=%b%b%b%b alua=%b%b%b%b alu=%b%b%b%b Pdata=%b%b%b%b dTm=%b%b%b%b dT=%b%b%b%b T=%b%b%b%b  TbBypass=%b TbSelMd=%b",
             tag,
             m.b_ProcH.alub_00, m.b_ProcH.alub_01, m.b_ProcH.alub_02, m.b_ProcH.alub_03,
             m.b_ProcH.alua_00, m.b_ProcH.alua_01, m.b_ProcH.alua_02, m.b_ProcH.alua_03,
             m.b_ProcH.alu_00,  m.b_ProcH.alu_01,  m.b_ProcH.alu_02,  m.b_ProcH.alu_03,
             m.b_ProcH.Pdata_00, m.b_ProcH.Pdata_01, m.b_ProcH.Pdata_02, m.b_ProcH.Pdata_03,
             m.b_ProcH.dTm_00,  m.b_ProcH.dTm_01,  m.b_ProcH.dTm_02,  m.b_ProcH.dTm_03,
             m.b_ProcH.dT_00,   m.b_ProcH.dT_01,   m.b_ProcH.dT_02,   m.b_ProcH.dT_03,
             m.b_ProcH.T_00,    m.b_ProcH.T_01,    m.b_ProcH.T_02,    m.b_ProcH.T_03,
             m.b_ProcH.TbBypass, m.b_ProcH.TbSelMd);
  endtask

  // TRACE: every change of Q or of the signals that control it. Printing on a
  // change of Q ALONE reads the controls a cycle late -- the NBA region means
  // a read in another always block sees the pre-edge value -- so log the whole
  // set and let the ORDER of the lines say which moved first.
  reg qtrace = 0;
  wire [24:0] qsig = {q_reg, m.b_ProcH.QClock_p_,
                      m.b_ProcH.QshiftL_p_, m.b_ProcH.QshiftR_p_,
                      m.b_ProcH.Bmux0, m.b_ProcH.Bmux1,
                      m.b_ProcH.alub_00a, m.b_ProcH.alub_01a,
                      m.b_ProcH.alub_02a, m.b_ProcH.alub_03a};
  reg [24:0] qsig_d;
  integer qtick = 0;
  always @(posedge sys_clk) begin
    qtick <= qtick + 1;
    qsig_d <= qsig;
    if (qtrace && qsig !== qsig_d)
      $display("      [t%0d] Q=%h QClock'=%b Qshift{L,R}'=%b%b Bmux{0,1}=%b%b alub.00a-03a=%b%b%b%b",
               qtick, qsig[24:9], qsig[8], qsig[7], qsig[6], qsig[5], qsig[4],
               qsig[3], qsig[2], qsig[1], qsig[0]);
  end

  wire [15:0] q_reg = {m.b_ProcH.Q_00, m.b_ProcH.Q_01, m.b_ProcH.Q_02, m.b_ProcH.Q_03,
                       m.b_ProcH.Q_04, m.b_ProcH.Q_05, m.b_ProcH.Q_06, m.b_ProcL.Q_07,
                       m.b_ProcL.Q_08, m.b_ProcL.Q_09, m.b_ProcL.Q_10, m.b_ProcL.Q_11,
                       m.b_ProcL.Q_12, m.b_ProcL.Q_13, m.b_ProcL.Q_14, m.b_ProcL.Q_15};
  always @(posedge sys_clk) begin
    if (m.b_ProcL.ALUFWrite_p_ !== palufw) begin
      nalufw = nalufw + 1; palufw = m.b_ProcL.ALUFWrite_p_;
    end
    if (m.b_ProcL.ALUFWrite_p_ == 1'b0) begin
      alub_at_write <= {m.b_ProcL.alub_15a, m.b_ProcL.alub_14a, m.b_ProcL.alub_13a,
                        m.b_ProcL.alub_12a, m.b_ProcL.alub_11a, m.b_ProcL.alub_08a};
      q_at_write <= q_reg;
    end
  end
  // MAR is a four-way mux on ProcH -- b02 selects T or R (RM), c02 selects Q
  // or Ain, under MarMuxAEn'/MarMuxBEn'/Amux1'. It leaves the board active
  // low. With a known value in T this shows whether the T leg is selected.
  wire [15:0] mar_reg = ~{m.MAR_00_p_, m.MAR_01_p_, m.MAR_02_p_, m.MAR_03_p_,
                          m.MAR_04_p_, m.MAR_05_p_, m.MAR_06_p_, m.MAR_07_p_,
                          m.MAR_08_p_, m.MAR_09_p_, m.MAR_10_p_, m.MAR_11_p_,
                          m.MAR_12_p_, m.MAR_13_p_, m.MAR_14_p_, m.MAR_15_p_};

  wire [15:0] t_reg = {m.b_ProcH.T_00, m.b_ProcH.T_01, m.b_ProcH.T_02, m.b_ProcH.T_03,
                       m.b_ProcH.T_04, m.b_ProcH.T_05, m.b_ProcH.T_06, m.b_ProcH.T_07,
                       m.b_ProcL.T_08, m.b_ProcL.T_09, m.b_ProcL.T_10, m.b_ProcL.T_11,
                       m.b_ProcL.T_12, m.b_ProcL.T_13, m.b_ProcL.T_14, m.b_ProcL.T_15};

  // PARC'S PROLOGUE, and TFromCPReg# does not work without it. Its own comment
  // says "requires ALUFM[0]=B": T is loaded THROUGH THE ALU, so ALUFM entry 0
  // has to hold the function that passes B before anything can be moved into
  // T. LoadDoradoCode does exactly this, before it loads a single word:
  //
  //     "Set up ALUFM[0] with a 25o, which is the logical function B"
  //     LDXI 25o / LDAI QFromCPReg#-IRTable / JSR SetCPAndDoIRTableInst
  //     LDAI ALUFM[0]FromQ#-IRTable / JSR DoIRTableInst
  //
  // Note SetCPAndDoIRTableInst, NOT the tilde form: the ALU function code goes
  // in UNCOMPLEMENTED.
  // HM Table 11d: "ALUFMEM <- B.8, B[11:15]". The six-bit entry is NOT a
  // contiguous field of B -- its MSB, which is the ALU's CARRY IN, is taken
  // from B.08 while the other five come from B[11:15]. So every LOGICAL entry
  // (all <= 037 octal) happens to be its own bit pattern in the low five bits,
  // and only the ARITHMETIC ones with a carry need bit .08 = C bit 7.
  // The C emulator's cpu.c documents the same mapping from the same table.
  function [15:0] alufm_word(input [5:0] e);
    alufm_word = {8'd0, e[5], 2'd0, e[4:0]};   // B.08 = carry, B[11:15] = rest
  endfunction

  task alufm0_is(input [15:0] fn, input TILDE);
    begin
      if (TILDE) set_cpreg_tilde(fn);
      else set_cpreg_plain(fn);
      parc_micro(8'h30, 8'h13, 8'hEF, 8'hC4, 8'h40); // QFromCPReg#
      nop_micro;                                     // ...AndNop
      $display("      after QFromCPReg#: Q=%h BMux=%h", q_reg, bmux);
      q_after_load = q_reg;
      qtrace = 1;
      $display("      before ALUFM[0]FromQ#: Bmux{0,1}=%b%b BmuxEn'=%b Qshift{L,R}'=%b%b Q=%h",
               m.b_ProcH.Bmux0, m.b_ProcH.Bmux1, m.b_ProcH.BmuxEn_p_,
               m.b_ProcH.QshiftL_p_, m.b_ProcH.QshiftR_p_, q_reg);
      parc_micro(8'h30, 8'h05, 8'h09, 8'hC4, 8'h40); // ALUFM[0]FromQ#
      $display("      at the ALUFM write: alub{15,14,13,12,11,08}=%b Q=%h",
               alub_at_write, q_at_write);
      qtrace = 0;
      q_after_hold = q_reg;
      $display("      after  ALUFM[0]FromQ#: Bmux{0,1}=%b%b BmuxEn'=%b Qshift{L,R}'=%b%b Q=%h",
               m.b_ProcH.Bmux0, m.b_ProcH.Bmux1, m.b_ProcH.BmuxEn_p_,
               m.b_ProcH.QshiftL_p_, m.b_ProcH.QshiftR_p_, q_reg);
    end
  endtask

  // TFromCPReg#, then a Nop -- DoIRTableInstAndNop, the Nop holding CPReg
  // constant through T3 of the instruction before it.
  task t_from_cpreg(input [15:0] v);
    begin
      if (T_PLAIN) set_cpreg_plain(v); else set_cpreg_tilde(v);
      parc_micro(8'h70, 8'h03, 8'h0F, 8'h04, 8'hC0);
      bmux_after_jam = bmux;
      show_tchain("jam ");
      $display("   MAR=%h  (T=%h; MarMuxAEn'=%b MarMuxBEn'=%b Amux1'=%b)",
               mar_reg, t_reg, m.b_ProcH.MarMuxAEn_p_, m.b_ProcH.MarMuxBEn_p_,
               m.b_ProcH.Amux1_p_);
      $display("   after the jam: BMux=%h T=%h LC=%b%b%b B<-Link'=%b UseCPReg=%b ALUF=%b%b%b%b",
               bmux, t_reg, m.LC_0, m.LC_1, m.LC_2,
               m.b_ContA.B_u_Link_p_, m.b_ContA.UseCPReg,
               m.ALUF_0, m.ALUF_1, m.ALUF_2, m.ALUF_3);
      nop_micro;
      show_tchain("nop ");
      $display("   after the nop: BMux=%h T=%h", bmux, t_reg);
    end
  endtask

  // PARC's TFromCPReg# with ONE field changed: ASEL[4] (A<-RM/STK) becomes
  // ASEL[6] (A<-T), so the ALU gets a real second operand. Byte 4 is
  //   LC.2, ASEL.0, ASEL.1, ASEL.2, JCN.3, JCN.4, JCN.5, JCN.6
  // and PARC's own ASEL[4] byte is C0, which is what that layout gives for
  // LC.2=1, ASEL=100 -- so ASEL=110 is E0. Everything else is unchanged.
  task t_from_a_op_b(input [15:0] v);
    begin
      set_cpreg_plain(v);
      parc_micro(8'h70, 8'h03, 8'h0F, 8'h04, 8'hE0);
      show_tchain("jam ");
      nop_micro;
      show_tchain("nop ");
    end
  endtask

  initial begin
    force m.DMuxData = dmd;
    force m.DMuxClk  = dmc;
    force m.UseDMD   = udmd;
    repeat (2000) @(posedge sys_clk);
    manifold(12'h030);
    manifold(12'h1E0);            // hold the MIR so the jam survives
    p0 = m.b_ContA.clk0_p_Ca; p1 = m.b_ContA.clk1_p_Ca; p2 = m.b_ContA.clk2_p_Bc;
    zero;
    nop_micro; nop_micro;         // warm the pipeline
    nalufw = 0; palufw = m.b_ProcL.ALUFWrite_p_;
    alufm0_is(alufm_word(6'o25), USE_TILDE);   // 25 octal = the logical function B
    $display("tb_compute: prologue with %s -- ALUFWrite' edges %0d",
             USE_TILDE ? "SetCPReg~ (complemented)" : "SetCPReg (plain)", nalufw);
    $display("tb_compute: ALUFM addr pins A0..A3 = %b %b %b %b (p10,p9,p7,p6=ALUF.3)",
             m.b_ProcL.ProcL11_sil_pl_3, m.b_ProcL.ProcL11_sil_pl_2,
             m.b_ProcL.ProcL11_sil_pl_1, m.b_ProcL.ALUF_3);
    $display("tb_compute: ALUFM[0] e13 mem = %b%b%b%b  e14 mem = %b%b",
             m.b_ProcL.u_e13.mem[0][3], m.b_ProcL.u_e13.mem[0][2],
             m.b_ProcL.u_e13.mem[0][1], m.b_ProcL.u_e13.mem[0][0],
             m.b_ProcL.u_e14.mem[0][1], m.b_ProcL.u_e14.mem[0][0]);
    $display("tb_compute: ALUFdec=%b%b%b%b%b%b -> aluF3..0=%b%b%b%b aluM=%b aluC=%b",
             m.b_ProcL.ALUFdec_5, m.b_ProcL.ALUFdec_4, m.b_ProcL.ALUFdec_3,
             m.b_ProcL.ALUFdec_2, m.b_ProcL.ALUFdec_1, m.b_ProcL.ALUFdec_0,
             m.b_ProcL.aluF3, m.b_ProcL.aluF2, m.b_ProcL.aluF1, m.b_ProcL.aluF0,
             m.b_ProcL.aluM, m.b_ProcL.aluC);

    t_from_cpreg(16'h1234);
    t_first = t_reg;
    $display("tb_compute: TFromCPReg# 1234 -> BMux=%h T=%h", bmux, t_reg);
    // BMux carries the COMPLEMENT of CPReg, and the ALU's own operand `alub`
    // re-inverts it, so T ends up equal to CPReg. That is not a quirk of the
    // model -- it is why PARC uses `SetCPReg~` to send IM data (which travels
    // the un-inverted path) and PLAIN `SetCPReg` for TFromCPReg#.
    if (bmux_after_jam !== ~16'h1234)
      $fatal(1, "B bus carried %h, expected %h (the complement of CPReg)",
             bmux_after_jam, ~16'h1234);

    t_from_cpreg(16'hA55A);
    t_second = t_reg;
    $display("tb_compute: TFromCPReg# a55a -> BMux=%h T=%h", bmux, t_reg);
    if (bmux_after_jam !== ~16'hA55A)
      $fatal(1, "B bus carried %h for a second value, expected %h",
             bmux_after_jam, ~16'hA55A);

    // ---- the gate ----------------------------------------------------
    // 25 octal into Q, off PARC's own QFromCPReg#. Catches the MC10141
    // parallel-entry rotation, which delivered 008a for 0015.
    if (q_after_load !== 16'h0015)
      $fatal(1, "Q took %h, not 0015 (25 octal). If it is 008a, cell_MC10141's parallel entry is rotated a place.",
             q_after_load);

    // ...and it is still there one instruction later. Catches the MC10119
    // shared-pin bug, which made every FA=0 instruction reload Q from a dead
    // B bus.
    if (q_after_hold !== 16'h0015)
      $fatal(1, "Q was %h by the end of ALUFM[0]FromQ#, not 0015 -- something reloaded it. Check QshiftL'/QshiftR' on a Nop: both asserted is PARALLEL ENTRY.",
             q_after_hold);

    // ...and ALUFM entry 0 holds it, read out of the F10145A storage itself.
    // PARC numbers MSB first, so ALUFdec.0 is the high bit: 010101 = 25 octal.
    if (alufm0 !== 6'b010101)
      $fatal(1, "ALUFM[0] = %b, not 010101 (25 octal, the logical function B)", alufm0);

    // T takes the operand EXACTLY, both values -- so a stuck latch cannot pass.
    if (t_first !== 16'h1234)
      $fatal(1, "T took %h, not 1234", t_first);
    if (t_second !== 16'hA55A)
      $fatal(1, "T took %h, not a55a", t_second);

    // ---- TWO OPERANDS, AND THE ALU IN THE MACHINE ----------------------
    // Everything above moves ONE value. This computes: A comes from T, B from
    // CPReg, the function from ALUFM -- and the result goes back into T.
    //
    // It also exercises the ALU AS THE BOARDS WIRE IT, which nothing did
    // before: `alu-diff` matches the C emulator on 10,752 vectors but builds
    // four MC10181 slices in a TESTBENCH, never as ProcH and ProcL chain them.
    // All sixteen LOGICAL entries of HM Table 9 are swept here, with the
    // expected values taken from the C emulator's own alu_op().
    for (fi = 0; fi < 16; fi = fi + 1) begin
      afn = 6'(fi*2 + 1);                       // 01,03,05 .. 37 octal
      case (afn)
        6'o01: aexp = ~AV;            6'o03: aexp = ~AV | ~BV;
        6'o05: aexp = ~AV |  BV;      6'o07: aexp = 16'hFFFF;
        6'o11: aexp = ~AV & ~BV;      6'o13: aexp = ~BV;
        6'o15: aexp = ~(AV ^ BV);     6'o17: aexp =  AV | ~BV;
        6'o21: aexp = ~AV &  BV;      6'o23: aexp =  AV ^  BV;
        6'o25: aexp =  BV;            6'o27: aexp =  AV |  BV;
        6'o31: aexp = 16'h0000;       6'o33: aexp =  AV & ~BV;
        6'o35: aexp =  AV &  BV;      6'o37: aexp =  AV;
      endcase

      alufm0_is(alufm_word(6'o25), 1'b0);       // 25 octal = B, so T can load
      t_from_cpreg(AV);                         // A operand into T
      if (t_reg !== AV)
        $fatal(1, "could not stage the A operand: T=%h, wanted %h", t_reg, AV);

      alufm0_is(alufm_word(afn), 1'b0);         // the function under test
      if (alufm0 !== afn)
        $fatal(1, "ALUFM[0] = %b, not %b (%0o octal)", alufm0, afn, afn);
      if (t_reg !== AV)
        $fatal(1, "loading ALUFM disturbed T: %h, wanted %h", t_reg, AV);

      t_from_a_op_b(BV);                        // T <- ALUFM[0](T, CPReg)
      $display("tb_compute:   ALUFM[0]=%02o octal  %h op %h -> %h  (expect %h)%s",
               afn, AV, BV, t_reg, aexp, (t_reg === aexp) ? "" : "   <-- WRONG");
      if (t_reg !== aexp)
        $fatal(1, "ALU entry %0o octal: got %h, the C emulator's alu_op says %h",
               afn, t_reg, aexp);
    end
    // The ARITHMETIC half of Table 9. The entry's high bit is the carry-in,
    // so `A+B' is 014 octal and `A-B' is 022 with that bit set = 062.
    for (fi = 0; fi < 8; fi = fi + 1) begin
      case (fi)
        0: begin afn = 6'o000; aexp =  AV;              end  // A
        1: begin afn = 6'o040; aexp =  AV + 16'd1;      end  // A + 1
        2: begin afn = 6'o006; aexp =  AV + AV;         end  // 2A
        3: begin afn = 6'o014; aexp =  AV + BV;         end  // A + B
        4: begin afn = 6'o054; aexp =  AV + BV + 16'd1; end  // A + B + 1
        5: begin afn = 6'o022; aexp =  AV - BV - 16'd1; end  // A - B - 1
        6: begin afn = 6'o062; aexp =  AV - BV;         end  // A - B
        7: begin afn = 6'o036; aexp =  AV - 16'd1;      end  // A - 1
      endcase

      alufm0_is(alufm_word(6'o25), 1'b0);
      t_from_cpreg(AV);
      alufm0_is(alufm_word(afn), 1'b0);
      if (alufm0 !== afn)
        $fatal(1, "ALUFM[0] = %b, not %b (%0o octal)", alufm0, afn, afn);
      t_from_a_op_b(BV);
      $display("tb_compute:   ALUFM[0]=%02o octal  %h op %h -> %h  (expect %h)%s",
               afn, AV, BV, t_reg, aexp, (t_reg === aexp) ? "" : "   <-- WRONG");
      if (t_reg !== aexp)
        $fatal(1, "ALU entry %0o octal: got %h, the C emulator's alu_op says %h",
               afn, t_reg, aexp);
    end
    $display("tb_compute: and all 8 ARITHMETIC entries too, carry in and out.");

    // ---- RM: the per-task register file -------------------------------
    // LC[6] is "RM/STK <- Pd" and RM's address is RBase<<4 | RSTK (HM p.88),
    // so with ALUFM[0] = B the operand goes straight from CPReg into RM[RSTK].
    // Reading it back needs ASEL[4] (A<-RM/STK), ALUFM[0] = 37 octal (A) and
    // LC[1] (T<-Pd).
    //
    // A write-then-read alone would NOT be worth much: RM's address pins take
    // RbAdr.0-.3 unprimed but RbAdr.4'-.7' PRIMED, so a mis-modelled polarity
    // is a consistent PERMUTATION -- it would read back perfectly and still be
    // wrong, exactly the trap that hid the IM address reversal. So the
    // PHYSICAL address is checked too, against RBase<<4 | RSTK.
    for (ai = 0; ai < 256; ai = ai + 1) seen_addr[ai] = 1'b0;
    for (fi = 0; fi < 4; fi = fi + 1) begin
      rstk_n  = 4'(fi * 5);                       // 0, 5, 12, 15
      rm_val  = 16'hC000 + 16'(fi * 16'h1111);
      rm_seen = -1;

      alufm0_is(alufm_word(6'o25), 1'b0);         // ALUFM[0] = B
      set_cpreg_plain(rm_val);
      jam_mi(mi(rstk_n, 4'd0, 3'd0, 3'd6, 3'd4, 8'o176, 8'o201, 1'b0));
      nop_micro;                                  // the write lands here

      // Which PHYSICAL cell took it? h06 holds RM bits .00-.03.
      // The cell that is newly non-zero is the one this write went to.
      rm_seen = -1;
      for (ai = 0; ai < 256; ai = ai + 1)
        if (m.b_ProcH.u_h06.mem[ai] !== 4'b0000 && !seen_addr[ai]) begin
          rm_seen = ai; seen_addr[ai] = 1'b1;
        end

      alufm0_is(alufm_word(6'o37), 1'b0);         // ALUFM[0] = A
      jam_mi(mi(rstk_n, 4'd0, 3'd0, 3'd1, 3'd4, 8'o176, 8'o201, 1'b0));
      nop_micro;

      $display("tb_compute:   RM[RBase<<4|%02o] <- %h  read back T=%h  landed at %0d, address pins said %0d",
               rstk_n, rm_val, t_reg, rm_seen, rbadr_at_write);
      if (t_reg !== rm_val)
        $fatal(1, "RM[%02o]: wrote %h, read back %h", rstk_n, rm_val, t_reg);
      if (rm_seen[3:0] !== ~rstk_n)
        $fatal(1, "RM[%02o] landed at low nibble %0d, expected ~RSTK = %0d",
               rstk_n, rm_seen[3:0], ~rstk_n);
      if (fi == 0) rbase_seen = 4'(rm_seen[7:4]);
      else if (rm_seen[7:4] !== rbase_seen)
        $fatal(1, "RSTK bled into the RBase half of the address: %0d, was %0d",
               rm_seen[7:4], rbase_seen);

    end
    $display("tb_compute: RM writes and reads back at four addresses; the low four");
    $display("tb_compute:   address bits are ~RSTK (the MC1662 NOR mux) and RBase reads %0d.", rbase_seen);

    $display("tb_compute: all 16 LOGICAL ALU functions agree with the C emulator,");
    $display("tb_compute:   computed on the real ProcH/ProcL slices, A from T and B from CPReg.");

    $display("tb_compute: Q takes 25 octal from CPReg, HOLDS it, and ALUFM[0] stores it.");
    $display("tb_compute: T loads through the ALU and takes the operand exactly.");

    // ---- MAR, the address the memory section is given. It is a four-way mux
    // on ProcH: b02 selects T or R (RM), c02 selects Q or Ain, under
    // MarMuxAEn' / MarMuxBEn' / Amux1'. T holds a known value at this point,
    // so this shows which leg the microinstruction's fields have selected.
    $display("tb_compute: MAR=%h with T=%h  (MarMuxAEn'=%b MarMuxBEn'=%b Amux1'=%b)",
             mar_reg, t_reg, m.b_ProcH.MarMuxAEn_p_, m.b_ProcH.MarMuxBEn_p_,
             m.b_ProcH.Amux1_p_);
    // PARC's TFromCPReg# carries ASEL[4], and ASEL 4 is NOT a memory reference
    // (MemC's b24 makes WantProcRef' = IgnoreProc | ASEL.0, so a reference
    // needs ASEL <= 3 -- see refdecode-test). So NEITHER leg of the MAR mux
    // may be enabled here, and with both MC10159s disabled they drive their
    // outputs low, which on these active-low lines reads as all ones. A
    // machine that put T on the address bus for a non-reference would be
    // handing the memory section an address it was never given.
    if (m.b_ProcH.MarMuxAEn_p_ !== 1'b1 || m.b_ProcH.MarMuxBEn_p_ !== 1'b1)
      $fatal(1, "a MAR mux leg is enabled for ASEL=4, which is not a reference");
    if (mar_reg !== 16'hffff)
      $fatal(1, "MAR is %h with no source selected, want ffff", mar_reg);
    $display("tb_compute: MAR takes no source when the instruction is not a reference.");

    // ---- Now the POSITIVE half: a microinstruction that DOES make a
    // reference. None of PARC's thirteen IRTable entries does one, so these
    // are built with mi(). ASEL 0-3 is a reference (MemC b24); the A source --
    // and therefore MAR, which shares the select -- is chosen by Amux0/Amux1,
    // and those come off a wired-OR of three drivers, so which ASEL puts T on
    // the address is discovered here rather than derived.
    n_aleg = 0; mbad = 0;
    for (mk = 0; mk < 4; mk = mk + 1) begin
      for (bk = 0; bk < 8; bk = bk + 1) begin
        jam_mi(mi(4'd0, 4'd0, bk[2:0], 3'd0, mk[2:0], 8'd0, 8'o201, 1'b0));
        if (m.b_ProcH.MarMuxAEn_p_ === 1'b0) begin
          // The A leg carries T or R, chosen by Amux1'. Whichever it is, MAR
          // must equal it exactly -- this is the address the memory section
          // will be given.
          n_aleg = n_aleg + 1;
          if (mar_reg !== (m.b_ProcH.Amux1_p_ ? r_reg : t_reg)) begin
            $display("tb_compute: FAIL ASEL=%0d BSEL=%0d -> MAR=%h, want %h (T=%h R=%h Amux1'=%b)",
                     mk, bk, mar_reg, (m.b_ProcH.Amux1_p_ ? r_reg : t_reg),
                     t_reg, r_reg, m.b_ProcH.Amux1_p_);
            mbad = mbad + 1;
          end
        end
        nop_micro;
      end
    end
    $display("tb_compute: MAR carries the A-leg register exactly in %0d reference cases", n_aleg);
    if (n_aleg < 8) $fatal(1, "no reference microinstruction enabled the MAR A leg");
    if (mbad != 0)  $fatal(1, "MAR does not equal the register the A mux selected");
    $display("tb_compute: THE MACHINE COMPUTES -- four boards, PARC's own ALU prologue.");
    $finish;
  end

endmodule

`default_nettype wire
