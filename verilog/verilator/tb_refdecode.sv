// tb_refdecode -- WHAT MAKES A MEMORY REFERENCE, checked against the C
// emulator's rule.
//
// The first gate on the memory section's front door. A Dorado microinstruction
// asks for a storage reference through the ASEL field, and the kind of
// reference is chosen by ASEL together with two bits of FF. The C emulator
// states the rule in include/memory.h, written from the Hardware Manual:
//
//     "Called from the microengine when ASEL is a memory reference
//      (ASEL = 0..3 with FF[0:1] decoding the kind)"
//
// The hardware says the same thing in gates, and this checks that it does.
// MemC b24 is an MC10103 quad OR wired
//
//     WantProcRef' = IgnoreProc | ASEL.0
//
// and PARC numbers a field MSB first, so ASEL.0 is the top bit of the
// three-bit ASEL -- it is 0 exactly when ASEL <= 3. So `WantProcRef'`, which
// enables the reference-type decoder, asserts precisely on the C emulator's
// condition, from a completely independent derivation.
//
// The kind decoder itself is MemC a24, an MC10162 one-of-eight, addressed by
// {ASEL.1, ASEL.2, FF.1mem} and enabled by `FF.0mem'` and `WantProcRef'`.
// `Ifetch_` is one of its eight outputs, and is required here to select
// exactly one of the sixteen (ASEL, FF.1) combinations -- a one-of-eight
// decoder that lit for two inputs would not be one.
//
// NOT gated here, deliberately: which kind each combination IS. `Lfetch_`,
// `Store_` and the rest are not raw decoder outputs -- they are qualified by
// board state this bench does not drive (Dbusy, CacheRefInA', WantCR,
// IgnoreProc), so standalone they read as that undriven state rather than as
// a decode. Pinning the full ASEL/FF[0:1] -> kind table against cpu.c's
// dispatch needs those driven, and is the next step.
//
// One encoding cross-checks itself on the way: ContA b17 decodes `_Map` as
// FA=0, FB=3, FC=1 = 0o31, and cpu.c's comment on DM_REF_RMAP says the read
// form "carries the ReadMap function (FA forced 0, FB=3, FC=1 -- the FF[2:7]
// subfield decodes to 0o31)". Two models, no shared code, same number.

`default_nettype none

module tb_refdecode;

  reg sys_clk = 1'b0;
  always #1 sys_clk = ~sys_clk;

  reg a0 = 1'b0, a1 = 1'b0, a2 = 1'b0, f0 = 1'b1, f1 = 1'b0;

  MemC_m_Rev_m_Be mc (
    .sys_clk(sys_clk),
    .ASEL_0(a0), .ASEL_0_p_mem(~a0),
    .ASEL_1_p_(~a1), .ASEL_2_p_(~a2),
    .FF_0mem_p_(~f0), .FF_1mem(f1)
  );

  integer i, bad, n_ifetch;
  reg [2:0] asel;

  initial begin
    bad = 0; n_ifetch = 0;
    for (i = 0; i < 16; i = i + 1) begin
      {a0, a1, a2, f1} = i[3:0];
      repeat (30) @(posedge sys_clk);
      asel = {a0, a1, a2};

      // The C emulator's rule: ASEL 0..3 is a reference, 4..7 is not.
      if (mc.WantProcRef_p_ !== (asel > 3'd3)) begin
        $display("tb_refdecode: FAIL ASEL=%0d -> WantProcRef'=%b, want %b",
                 asel, mc.WantProcRef_p_, (asel > 3'd3));
        bad = bad + 1;
      end
      if (mc.Ifetch_u_ === 1'b1) n_ifetch = n_ifetch + 1;
    end

    $display("tb_refdecode: WantProcRef' asserts for ASEL 0-3 and not 4-7, all 16 cases");
    $display("tb_refdecode: Ifetch_ selected by %0d of the 16 (ASEL, FF.1) combinations", n_ifetch);
    if (n_ifetch !== 1) begin
      $display("tb_refdecode: FAIL -- a one-of-eight decoder must select exactly one");
      bad = bad + 1;
    end
    if (bad != 0) $fatal(1, "the memory reference decode disagrees with cpu.c");
    $display("tb_refdecode: PASS -- ASEL 0-3 is a storage reference, as the C emulator says");
    $finish;
  end

endmodule

`default_nettype wire
