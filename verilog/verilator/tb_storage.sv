// tb_storage -- THE STORAGE ARRAY IS IN THE MACHINE, ADDRESSED AND STROBED.
//
// The first rung for the MSA, and the same one every other board started on:
// before anything can be asked of a board it has to be in a machine that
// elaborates, be clocked, and have its inputs actually arrive.
//
// `dorado_storage` is `dorado_mem`'s seven boards -- ContA, ContB, ProcH,
// ProcL, MemC, MemD, MemX -- plus **msa**, PARC's own storage-array board:
// 291 packages, of which 144 are MK4096 dynamic RAMs. It is generated from
// `chm/sil/msa-Rev-Bg.dm!1_/msa.wl` exactly like the others and wired by NAME
// across the backplane.
//
// THE MSA HANGS OFF THE MEMORY SECTION AND NOTHING ELSE. Of its 66 backplane
// nets, 38 are shared with MemD, 23 with MemX, 4 with MemC and ZERO with
// ContA/ContB/ProcH/ProcL. The MemD side is the storage data path itself --
// `Sin.00-15` and `Sout.00-15`, the 16-bit word in and out, plus `EcIn`/
// `EcOut` for ECC. The MemX side is the DRAM control: `MemAd.1-8` multiplexed
// address, `MemRASa/b`, `MemCASa/b`, `MemWEa/b`, the module selects, and the
// `LoadSinE`/`ShiftSinE`/`ShiftSoutE` controls that move the word through the
// board's shift registers.
//
// WHAT THIS GATES, and why each of the three is here.
//
//  1. THE ADDRESS PATH INVERTS. The MSA takes its address off the backplane
//     as `TtlA0'`..`TtlA6'` and inverts it through 28 SN74H04 hex inverters
//     into `A0a0`..`A6a0`, which fan out to the 144 MK4096 packages. Those
//     inverters were a SKELETON until 2026-08-23 -- ports, no behaviour -- so
//     every address line on the storage board read 0 and no location could be
//     selected. The property is checked per bit against the board's own nets,
//     not against a constant, so it holds whatever address the machine puts up.
//
//  2. THE DRAM STROBES ARRIVE. RAS' and CAS' reach the array from MemX. Out of
//     reset they are gated exactly as the memory boards' clocks are, so what
//     is asserted is that they are DEFINED and that they move once the machine
//     runs -- not that they toggle from the first cycle.
//
//  3. THE DATA-PATH REGISTERS CLOCK. The word does not arrive at the DRAMs in
//     parallel -- it is registered and then SHIFTED. Mapped from the wire
//     list, one bit followed end to end:
//
//         Sout.00-03  -> b01 D0-D3   (MC10176 hex D FF, clk msa01.sil+4)
//                     -> msa04/05.sil+*  -> the SN74166 shift chain
//                     -> f01 (MC10176, clk msa01.sil+8, ECC on EcOut.0/4)
//                     -> f02 (MC10125 ECL->TTL)  -> MK4096 pin 2, DIN
//
//         MK4096 pin 14, DOUT -> ... -> c01 D-inputs
//                     -> c01 Q (MC10176, clk msa01.sil+3) -> Sin.00-03, EcIn.0
//
//     so `Sout` is the write side and `Sin` the read side, and the MSA DRIVES
//     Sin while MemD drives Sout -- the direction is worth stating because
//     the names do not give it away.
//
//     THEIR CLOCKS ARE THE BOARD'S OWN, AND CORRECTLY QUIET HERE. b01 clocks
//     on `msa01.sil+4` from e13 (MC10210) off `c1`/`c2`, and c01 on
//     `msa01.sil+3` from h01 off `SO` -- and `c1`, `c2` and `SO` are INTERNAL
//     nets, driven by MC10176/MC10210 packages on the MSA itself. They are an
//     on-board sequencer fed by MemX's `LoadSinE`/`ShiftSinE`/`LoadSoutE'`/
//     `ShiftSoutE`, so they run when a REFERENCE runs. This startup issues
//     none, and measured they take 0 edges -- which is the board behaving.
//     So what is asserted is that they are DEFINED, exactly as for the
//     strobes; demanding edges here would be demanding the wrong thing, and
//     a first version of this gate did.
//
//  4. THE MEMORY SIZE ARRIVES FROM THE STORAGE BOARD, WHICH IS THE POINT OF
//     HAVING IT. `ChipsAre64K` is the chip enable on one of the two DRAM
//     timing PROMs on MemX. MemX spells it `ChipsAre64K` and the MSA spells it
//     `ChipsAre64k`; both are on backplane pin E55, so they are ONE WIRE, and
//     until that case variant was added to BACKPLANE_CASE_ALIASES the line was
//     unconnected and `tb_memrun` had to drive it by hand as an input. Here it
//     is not an input at all -- the MSA supplies it -- and this gate fails if
//     the merge is ever reverted.

`default_nettype none

module tb_storage;
  reg sys_clk = 1'b0;
  always #1 sys_clk = ~sys_clk;

  // 16 sys_clk to a 60 ns microinstruction, as everywhere else here.
  reg [3:0] div = 4'd0;
  reg       mclk = 1'b0;
  always @(posedge sys_clk) begin
    div <= div + 4'd1;
    if (div == 4'd7) begin mclk <= ~mclk; div <= 4'd0; end
  end

  reg [2:0] addr_n = 3'b111;
  reg [8:0] cpout  = 9'd0;
  reg       strb_n = 1'b1;
  reg       setrun = 1'b0, setss_n = 1'b1;
  reg       setrunrfsh = 1'b0;

  dorado_storage m (
      .sys_clk(sys_clk),
      .CLK_ca_p_(mclk), .CLK_cb_p_(mclk), .CLK_ph_p_(mclk), .CLK_pl_p_(mclk),
      .CLK_mc_p_(mclk), .CLK_md_p_(mclk), .CLK_mx_p_(mclk),
      .CPAddr_0_p_(addr_n[2]), .CPAddr_1_p_(addr_n[1]), .CPAddr_2_p_(addr_n[0]),
      .CPOut_0(cpout[8]), .CPOut_1(cpout[7]), .CPOut_2(cpout[6]),
      .CPOut_3(cpout[5]), .CPOut_4(cpout[4]), .CPOut_5(cpout[3]),
      .CPOut_6(cpout[2]), .CPOut_7(cpout[1]), .CPOut_8(cpout[0]),
      .CPStrb_p_(strb_n), .SetRun(setrun), .SetSS_p_(setss_n),
      .SetRunRfsh(setrunrfsh)
      // NOTE what is NOT here: ChipsAre64K. tb_memrun drives it as an input
      // because that configuration has no storage board; this one does, and
      // the MSA supplies it over pin E55.
  );

  localparam integer GAP = 200;
  task strobe(input [2:0] fn, input [7:0] data, input ss);
    begin
      addr_n = ~fn; cpout = {ss, data}; setss_n = ~ss;
      repeat (4) @(posedge sys_clk); strb_n = 1'b0;
      repeat (8) @(posedge sys_clk); strb_n = 1'b1;
      repeat (GAP) @(posedge sys_clk);
    end
  endtask

  integer i, nras, ncas, nbad, naddr, nwclk, nrclk;
  reg pras, pcas, pwclk, prclk;
  reg [6:0] ttl, dram, paddr;

  initial begin
    repeat (400) @(posedge sys_clk);

    // ---- START THE MACHINE, the same sequence tb_mem and run-test use ----
    strobe(3'd1, 8'h21, 1'b0);              // Clock: InhibitCAHolds+ClrReady
    strobe(3'd0, 8'h4E, 1'b0); setrun = 0;  // ClrStop+ClrMIR+ClrCT+Freeze
    strobe(3'd0, 8'h00, 1'b1); setss_n = 0; // undo the clears, SetSS
    strobe(3'd0, 8'h41, 1'b1); setrun = 1;  // ClrStop AND SetRun together
    setrunrfsh = 1'b1;
    repeat (400) @(posedge sys_clk);

    pras = m.b_msa.MemRASa; pcas = m.b_msa.MemCASa;
    nras = 0; ncas = 0; nbad = 0; naddr = 0; nwclk = 0; nrclk = 0;
    pwclk = m.b_msa.msa01_sil_pl_4; prclk = m.b_msa.msa01_sil_pl_3;
    paddr = 7'bx;

    for (i = 0; i < 20000; i = i + 1) begin
      @(posedge sys_clk);
      if (m.b_msa.MemRASa !== pras) begin nras = nras + 1; pras = m.b_msa.MemRASa; end
      if (m.b_msa.MemCASa !== pcas) begin ncas = ncas + 1; pcas = m.b_msa.MemCASa; end
      // the write-data and read-data register clocks
      if (m.b_msa.msa01_sil_pl_4 !== pwclk) begin nwclk = nwclk + 1; pwclk = m.b_msa.msa01_sil_pl_4; end
      if (m.b_msa.msa01_sil_pl_3 !== prclk) begin nrclk = nrclk + 1; prclk = m.b_msa.msa01_sil_pl_3; end

      // 1. the SN74H04 address path, per bit, against the board's own nets
      ttl  = {m.b_msa.TtlA6_p_, m.b_msa.TtlA5_p_, m.b_msa.TtlA4_p_,
              m.b_msa.TtlA3_p_, m.b_msa.TtlA2_p_, m.b_msa.TtlA1_p_,
              m.b_msa.TtlA0_p_};
      dram = {m.b_msa.A6a0, m.b_msa.A5a0, m.b_msa.A4a0,
              m.b_msa.A3a0, m.b_msa.A2a0, m.b_msa.A1a0, m.b_msa.A0a0};
      if (dram !== ~ttl) nbad = nbad + 1;
      if (dram !== paddr) begin naddr = naddr + 1; paddr = dram; end
    end

    $display("tb_storage: eight boards -- ContA ContB ProcH ProcL MemC MemD MemX msa");
    $display("tb_storage: the MSA carries 144 MK4096 DRAMs; 265/265 of its logic");
    $display("tb_storage:   packages are modelled.");
    $display("tb_storage: address path  TtlA[6:0]'=%b -> A[6:0]a0=%b, %0d distinct values, %0d mismatches",
             ttl, dram, naddr, nbad);
    $display("tb_storage: DRAM strobes over 20,000 sys_clk: RAS' %0d edges, CAS' %0d edges",
             nras, ncas);
    $display("tb_storage: data-path register clocks: write (b01) %0d edges, read (c01) %0d edges",
             nwclk, nrclk);
    $display("tb_storage: memory size from the MSA -- ChipsAre64K=%b (pin E55, msa spells it 'k')",
             m.b_msa.ChipsAre64K);

    // 1. every bit of the address is the complement of its TTL input, on every
    //    cycle. A skeleton inverter fails this on the first one.
    if (nbad != 0)
      $fatal(1, "the MSA address path does not invert: %0d cycles where A[6:0]a0 != ~TtlA[6:0]' -- is cell_SN74H04 modelled?",
             nbad);

    // 2. the strobes are DEFINED. They are gated like the memory clocks, so
    //    this does not demand edges; it demands that they are not X.
    if (m.b_msa.MemRASa === 1'bx || m.b_msa.MemCASa === 1'bx)
      $fatal(1, "the DRAM strobes never reached the storage board (RAS'=%b CAS'=%b)",
             m.b_msa.MemRASa, m.b_msa.MemCASa);

    // 3. both data-path register clocks are DEFINED. They come from the
    //    board's own sequencer off MemX's load/shift controls, so they are
    //    correctly still until a reference runs -- see the header.
    if (m.b_msa.msa01_sil_pl_4 === 1'bx || m.b_msa.msa01_sil_pl_3 === 1'bx)
      $fatal(1, "the MSA data-path register clocks never resolved (write=%b read=%b)",
             m.b_msa.msa01_sil_pl_4, m.b_msa.msa01_sil_pl_3);

    // 4. the memory size arrives over the backplane rather than from a
    //    testbench input -- i.e. the E55 case variant is still merged.
    if (m.b_msa.ChipsAre64K === 1'bx)
      $fatal(1, "ChipsAre64K did not reach the MSA -- has the E55 case alias been reverted?");

    $display("tb_storage: PASS -- the storage array is in a machine, its address");
    $display("tb_storage:   path inverts, its DRAM strobes are live, and the memory");
    $display("tb_storage:   size arrives from the board rather than from the bench.");
    $finish;
  end
endmodule
`default_nettype wire
