// Does the drive at the end of the cable present itself correctly, and does
// it ROTATE at 3600 RPM?
//
// The three things this has to establish, and the third is the one that has
// caught real bugs before:
//
//   1. NO DRIVE MEANS NO DRIVE. With `attached` low every one of these
//      active-low lines must idle HIGH. Six DskEth cable lines once read
//      asserted from their SIP pull-ups and INVENTED a drive that was not
//      there, so this is checked first and explicitly.
//   2. An attached drive asserts Ready, OnLine and TERM together -- TtlTerm'
//      is part of being on line, because DskEth c24 wired-ORs it with
//      TtlOnLine' and asserting OnLine alone leaves `NotOnLine` high.
//   3. It rotates at the right RATE. 3600 RPM is 277,778 microcycles per
//      revolution (DORADO_DISK_CYCLES_PER_REV in dorado/include/disk.h), and
//      the sector pulses divide that by the pack's sectors-per-revolution.
//
// AND THE GAPS ARE COUNTED, NOT JUST THE PULSES. "SecIndx' asserted N times"
// is also what a line stuck low gives. `disk-input-test` learned that the
// hard way -- "the gap half is what separates tracking from stuck" -- so this
// measures the duty cycle and requires the line to spend most of its time
// DEASSERTED, which a stuck line cannot do.

`default_nettype none

module tb_trident;

  // A small rotation so the test runs in reasonable time. The RATIOS are what
  // matter -- sectors per revolution, and pulse against gap -- and they are
  // independent of how long a revolution is.
  localparam integer SYSPER = 2;
  localparam integer SECTORS = 8;

  reg sys_clk = 1'b0;
  always #1 sys_clk = ~sys_clk;

  reg reset = 1'b1, attached = 1'b0, selected = 1'b0;
  wire rdy_n, onl_n, term_n, sel_n, si_n;

  // SECTORS_PER_REV=8 with the module's real 277,778-microcycle revolution
  // would take 4.4 M sys_clk to see one turn. The revolution length is a
  // localparam inside the module, so this bench instead checks the RATIOS
  // over a scaled revolution by overriding SYSPER down -- the arithmetic
  // (CYCLES_PER_REV = 277778 * SYSPER) is linear in it, so a smaller SYSPER
  // is a shorter revolution with every ratio preserved.
  dorado_trident #(.SYSPER(1), .SECTORS_PER_REV(SECTORS)) u (
      .sys_clk(sys_clk), .reset(reset), .attached(attached), .selected(selected),
      .TtlReady_n(rdy_n), .TtlOnLine_n(onl_n), .TtlTerm_n(term_n),
      .Selected_n(sel_n), .SecIndx_n(si_n));

  localparam integer REV = 277778;           // sys_clk per revolution at SYSPER=1
  integer i, bad = 0;
  integer n_low, n_high, n_fall, n_index, longest, run;
  reg si_d;

  task chk(input ok, input [639:0] what);
    begin
      if (!ok) begin $display("tb_trident: FAIL -- %0s", what); bad = bad + 1; end
      else       $display("tb_trident: ok   -- %0s", what);
    end
  endtask

  initial begin
    repeat (8) @(posedge sys_clk); reset = 1'b0;

    // ---- 1. NO DRIVE: every cable line deasserted ----------------------
    attached = 1'b0; selected = 1'b1;      // selected, but nothing there
    repeat (200) @(posedge sys_clk);
    chk(rdy_n && onl_n && term_n && sel_n && si_n,
        "no pack: Ready'/OnLine'/Term'/Selected'/SecIndx' all DEASSERTED");
    n_low = 0;
    for (i = 0; i < 5000; i = i + 1) begin
      @(posedge sys_clk);
      if (!si_n) n_low = n_low + 1;
    end
    chk(n_low == 0, "no pack: SecIndx' never pulses (a drive is not invented)");

    // ---- 2. ATTACHED: presence, and selection is separate ---------------
    attached = 1'b1; selected = 1'b0;
    repeat (200) @(posedge sys_clk);
    chk(!rdy_n && !onl_n && !term_n,
        "pack mounted: Ready', OnLine' and TERM' asserted together");
    chk(sel_n, "pack mounted but not selected: Selected' stays deasserted");
    selected = 1'b1;
    repeat (200) @(posedge sys_clk);
    chk(!sel_n, "selected: Selected' asserts");

    // ---- 3. ROTATION: rate, and the gaps ------------------------------
    // Watch one whole revolution. Count falling edges of SecIndx' (one per
    // sector), and the longest low run (the index pulse, which is longer).
    n_fall = 0; n_low = 0; n_high = 0; longest = 0; run = 0;
    si_d = si_n;
    for (i = 0; i < REV; i = i + 1) begin
      @(posedge sys_clk);
      if (!si_n && si_d) n_fall = n_fall + 1;
      if (!si_n) begin
        n_low = n_low + 1; run = run + 1;
        if (run > longest) longest = run;
      end else begin
        n_high = n_high + 1; run = 0;
      end
      si_d = si_n;
    end
    $display("tb_trident: one revolution -- %0d pulses, low on %0d of %0d samples (%0d%%), longest low run %0d",
             n_fall, n_low, REV, (100*n_low)/REV, longest);

    // EXACTLY ONE PULSE PER SECTOR. Not "about" -- the counter is exact.
    chk(n_fall == SECTORS,
        "one revolution gives exactly SECTORS_PER_REV pulses");

    // THE GAPS. A stuck-low line would give a healthy-looking pulse count on
    // any edge-triggered check; only the duty cycle separates them.
    chk(n_high > (REV * 9) / 10,
        "SecIndx' is DEASSERTED for most of a revolution (not stuck low)");
    chk(n_low > 0, "...and asserted some of it (not stuck high either)");

    // THE INDEX PULSE IS THE LONG ONE. Sector pulses are REV/SECTORS/64
    // samples; the index is four times that, and the controller tells them
    // apart by exactly this.
    chk(longest > (REV / SECTORS / 64) * 2,
        "one pulse per revolution is markedly LONGER -- that is index");

    // ---- and a second revolution repeats ------------------------------
    n_fall = 0; si_d = si_n;
    for (i = 0; i < REV; i = i + 1) begin
      @(posedge sys_clk);
      if (!si_n && si_d) n_fall = n_fall + 1;
      si_d = si_n;
    end
    chk(n_fall == SECTORS, "the next revolution gives the same count -- it keeps turning");

    // ---- unmounting stops it -------------------------------------------
    attached = 1'b0;
    repeat (200) @(posedge sys_clk);
    n_low = 0;
    for (i = 0; i < 5000; i = i + 1) begin @(posedge sys_clk); if (!si_n) n_low = n_low + 1; end
    chk(n_low == 0 && rdy_n && onl_n, "pack removed: the drive goes away again");

    if (bad) $fatal(1, "tb_trident: %0d checks failed", bad);
    $display("tb_trident: PASS -- a Trident presents itself and rotates at 3600 RPM");
    $finish;
  end

endmodule

`default_nettype wire
