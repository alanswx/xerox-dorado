// tb_baseboard -- the BaseBoard's 6502 boots from its own EPROMs.
//
// The BaseBoard is the one board where every piece is real: Andrew Holme's
// netlist-derived 6502, MiSTer's 6532 RIOTs, the SN74LS138 that decodes
// `Rom0'`..`Rom7'`, eight 2716 sockets holding the base ROM the C emulator
// boots, and the crystal / '163 / '74 chain that clocks and releases the
// processor. So it can be asked the only question that matters of a
// processor board: does it run its own firmware?
//
// FOUR THINGS AT ONCE. A 6502 comes out of reset by reading 0xFFFC and
// 0xFFFD, and in this ROM those hold 0xF3A7. Seeing that address presented,
// seeing 0xA7 and 0xF3 come back on the data bus, and then seeing the
// processor FETCH from 0xF3A7 means the core, the '138 decode, the socket
// mapping and the stored image are all right together -- four things this
// project derived separately, from PARC's wire list, a chip-label photo, the
// address space the C emulator uses, and a `.MB` from the archive.
//
// The instruction stream is checked too, against
// chm/disassembly/bb_F000-FFFF.s: F3A7 is `CLD`, then `LDX #$00`, `LDA #$00`,
// and a `STA $00,X` / `DEX` / `BNE` loop clearing zero page. A 6502 that
// fetched the vector but latched nothing would still walk plausible
// addresses -- during bring-up it walked 0x0000 upward for thousands of
// cycles, looking healthy -- so the gate reads the OPCODES.
//
// It runs the board inside `dorado_baseboard`, the one-board machine that
// tools/sil_backplane.py emits, and not the board module alone. That is not a
// convenience: `BootMC'` -- which the 6502's reset flip-flop takes as its D
// input -- is a BACKPLANE net that the BaseBoard both drives and reads back,
// so the board on its own has it as an unconnected input, reads it low, and
// holds the processor in reset forever. The one-board machine makes the same
// connection the backplane does.
//
// Everything else is left unconnected, which the harness treats as low; see
// dorado_machine for why that is the honest default.
//
// The bus is sampled in the middle of phase 2, which is when the ROM drives
// and the processor reads. Sampling at the address change instead shows a
// dead bus and says nothing.

`default_nettype none

// The board, inside the one-board machine.
`define BB m.u_machine.b_BaseBd

module tb_baseboard;

  reg sys_clk = 1'b0;
  always #1 sys_clk = ~sys_clk;

  dorado_baseboard_machine m (.sys_clk(sys_clk));

  // The 6502's address and data buses, by hierarchical reference.
  wire [15:0] mca = {`BB.MCA_15, `BB.MCA_14, `BB.MCA_13, `BB.MCA_12,
                     `BB.MCA_11, `BB.MCA_10, `BB.MCA_09, `BB.MCA_08,
                     `BB.MCA_07, `BB.MCA_06, `BB.MCA_05, `BB.MCA_04,
                     `BB.MCA_03, `BB.MCA_02, `BB.MCA_01, `BB.MCA_00};
  wire [7:0]  mcd = {`BB.MCD_7, `BB.MCD_6, `BB.MCD_5, `BB.MCD_4,
                     `BB.MCD_3, `BB.MCD_2, `BB.MCD_1, `BB.MCD_0};

  // The reset routine, from chm/disassembly/bb_F000-FFFF.s: address, byte.
  localparam integer NPROG = 8;
  reg [23:0] prog [0:NPROG-1];

  // ---- MEASURE THE CLOCK GENERATOR'S WAVEFORM ----------------------------
  // Seven packages make it: h06 (the MPQ3303 VCO substitute, itself a counter
  // on sys_clk), g05/h05 (MC1660 shaping) and g03/g04/h03/h04 (MC1690
  // dividers). Everything after the VCO recovers its clock by EDGE DETECTION,
  // which is what forces the oversampling ratio. To replace the chain with a
  // phase counter, first record exactly what it emits.
  integer ph_i, ph_scp, ph_ecp, ph_last_scp, ph_period;
  reg     ph_p1, ph_p2;
  initial begin
    ph_scp = 0; ph_ecp = 0; ph_last_scp = -1; ph_period = 0;
    repeat (2000) @(posedge sys_clk);          // let the VCO and dividers settle
    ph_p1 = `BB.StartClockPulse_p_;
    ph_p2 = `BB.EndClockPulse;
    for (ph_i = 0; ph_i < 4000; ph_i = ph_i + 1) begin
      @(posedge sys_clk);
      if (`BB.StartClockPulse_p_ !== ph_p1) begin
        ph_p1 = `BB.StartClockPulse_p_;
        if (!ph_p1) begin                       // falling edge of StartClockPulse'
          if (ph_last_scp >= 0 && ph_period == 0) ph_period = ph_i - ph_last_scp;
          ph_last_scp = ph_i;
        end
        ph_scp = ph_scp + 1;
      end
      if (`BB.EndClockPulse !== ph_p2) begin
        ph_p2 = `BB.EndClockPulse; ph_ecp = ph_ecp + 1;
      end
    end
    $display("tb_baseboard: CLOCKGEN over 4000 sys_clk -- StartClockPulse' %0d edges, EndClockPulse %0d edges, StartClockPulse' period %0d sys_clk",
             ph_scp, ph_ecp, ph_period);
    // THE WAVEFORM ITSELF: sample both nets across two whole periods, so the
    // phase counter that replaces this chain can reproduce them exactly.
    $write("tb_baseboard: SCP' ");
    for (ph_i = 0; ph_i < 32; ph_i = ph_i + 1) begin
      $write("%b", `BB.StartClockPulse_p_); @(posedge sys_clk);
    end
    $display("");
    $write("tb_baseboard: ECP  ");
    for (ph_i = 0; ph_i < 32; ph_i = ph_i + 1) begin
      $write("%b", `BB.EndClockPulse); @(posedge sys_clk);
    end
    $display("");
  end

  initial begin
    prog[0] = {16'hF3A7, 8'hD8};   // CLD
    prog[1] = {16'hF3A8, 8'hA2};   // LDX #$00
    prog[2] = {16'hF3A9, 8'h00};
    prog[3] = {16'hF3AA, 8'hA9};   // LDA #$00
    prog[4] = {16'hF3AB, 8'h00};
    prog[5] = {16'hF3AC, 8'h95};   // STA $00,X
    prog[6] = {16'hF3AE, 8'hCA};   // DEX
    prog[7] = {16'hF3AF, 8'hD0};   // BNE $F3AC
  end

  integer i, j, changes, seen_rom, matched;
  reg [NPROG-1:0] seen_prog;   // once each: the loop runs 256 times
  integer vec_lo, vec_hi, ran_from_vector;
  reg [15:0] prev;
  reg        clk_prev;

  initial begin
    changes = 0; seen_rom = 0; matched = 0; seen_prog = {NPROG{1'b0}};
    vec_lo = -1; vec_hi = -1; ran_from_vector = 0;
    prev = 16'hXXXX; clk_prev = 1'b0;

    for (i = 0; i < 400000; i = i + 1) begin
      @(posedge sys_clk);
      // Phase 2 has just gone high; wait until the middle of it, where the
      // ROM is driving and the processor is reading.
      if (`BB.MCClk && !clk_prev) begin
        repeat (20) @(posedge sys_clk);

        if (mca !== prev) begin
          changes = changes + 1;
          prev = mca;
          if (mca >= 16'hC000) seen_rom = seen_rom + 1;
          if (mca == 16'hFFFC) vec_lo = mcd;
          if (mca == 16'hFFFD) vec_hi = mcd;
          if (vec_hi >= 0 && mca == {vec_hi[7:0], vec_lo[7:0]})
            ran_from_vector = 1;
          for (j = 0; j < NPROG; j = j + 1)
            if (mca == prog[j][23:8] && mcd == prog[j][7:0] && !seen_prog[j])
              begin seen_prog[j] = 1'b1; matched = matched + 1; end
        end
      end
      clk_prev = `BB.MCClk;
    end

    $display("tb_baseboard: address bus changed %0d times, %0d into ROM (0xC000+)",
             changes, seen_rom);
    if (vec_lo < 0 || vec_hi < 0)
      $display("tb_baseboard: FAIL -- 0xFFFC/D was never addressed");
    else
      $display("tb_baseboard: reset vector read off the bus as %02h%02h",
               vec_hi[7:0], vec_lo[7:0]);
    if (ran_from_vector)
      $display("tb_baseboard: the 6502 then fetched from it and ran");
    else
      $display("tb_baseboard: FAIL -- it never fetched from the vector");
    $display("tb_baseboard: %0d of %0d reset-routine bytes match the disassembly",
             matched, NPROG);

    if (vec_lo < 0 || vec_hi < 0) $fatal(1, "no reset vector fetch");
    if ({vec_hi[7:0], vec_lo[7:0]} != 16'hF3A7)
      $fatal(1, "reset vector is not 0xF3A7");
    if (!ran_from_vector) $fatal(1, "never ran from the vector");
    if (matched != NPROG)
      $fatal(1, "the instruction stream does not match the ROM");
    $finish;
  end

endmodule

`default_nettype wire
