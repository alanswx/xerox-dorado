// cell_F10145A -- 16 x 4 Register File (64-bit RAM)
//
// Pin numbers and names: PARC's EclDict.Analyze. Used in 105 package
// position(s) across the sixteen Dorado boards.
//
// RAM INFERENCE. ECL RAMs are ASYNCHRONOUS -- no clock, write is level
// sensitive on WE'. Written that way here because that is the part, and it
// simulates correctly. On an FPGA an async-write array does NOT infer block
// RAM: Quartus will give distributed/LUT RAM or registers, which for the
// larger arrays here is not viable. This cell is FULLY SYNCHRONOUS and infers
// block RAM: the write is clocked on `sys_clk` with the part's own write
// enable as a one-cycle enable, so there is no asynchronous write and no
// gated clock. An earlier header deferred this to verilog/README.md as
// unresolved; it was resolved when the strobes became enables.

// SYNCHRONOUS WRITE on the fabric clock, with the part's own write level as an
// enable -- the convention this whole design uses for a clocked element, and
// the same change the two DRAM cells already carry.
//
// The part's write is ASYNCHRONOUS: it is level-sensitive on the write and
// chip enables, so while they are asserted the stored bit follows the data
// input. That makes a COMBINATIONAL LOOP wherever the machine writes back
// something it computed from the same location -- read, gate, write, read --
// which is most of a datapath. A latch is also not synthesisable on an FPGA;
// a registered write infers memory. The READ stays combinational, which is
// what the machine's timing assumes and what distributed RAM gives you.

`default_nettype none

module cell_F10145A (
    input  wire sys_clk,
    input  wire p10,
    input  wire p9,
    input  wire p7,
    input  wire p6,
    input  wire p5,
    input  wire p4,
    input  wire p11,
    input  wire p12,
    input  wire p3,
    input  wire p13,
    output wire p2,
    output wire p1,
    output wire p15,
    output wire p14,
    input  wire p8
);

  // A0-A3 = 10,9,7,6 address; D0-D3 = 5,4,11,12 data in;
  // Q0-Q3 = 2,1,15,14 data out; CE'=3 chip enable, WE'=13 write enable.
  (* ramstyle = "MLAB, no_rw_check" *) reg [3:0] mem [0:15];
  //
  // ADDRESS BIT ORDER: A0 IS THE MOST SIGNIFICANT. PARC numbers every field
  // MSB-first -- `RSTK.0` is the top RSTK bit, `TNIA.04` the top address bit --
  // and the dictionary's A-numbering follows suit. `cell_F10414`, the same
  // family on the same board, says so in its own comment ("A0..A7, MSB
  // first"), and `cell_i2716` was corrected for exactly this ("PARC names
  // those MSB-first: the dictionary's A0 is pin 19, the sheet's A10 ... a
  // reversed address would have scrambled the image silently").
  wire [3:0] a = {p10, p9, p7, p6};   // A0=p10 .. A3=p6
  // ONE WRITE PER PULSE, on the RISING edge of WE'.
  //
  // The real part writes while WE' is LOW, and a static RAM latches what is on
  // its pins at the END of that window, when address and data are settled.
  // Taking "while low" literally in the fabric writes on EVERY sys_clk the
  // pulse spans -- and if the address moves inside the pulse, it writes
  // SEVERAL CELLS. That is exactly what happened at 8 sys_clk per
  // microinstruction: the startup `Link<-` landed in every task's slot
  // instead of task 0's, and tb_taskrun read the same value out of all
  // sixteen. It survived at 16x only because the pulse happened to sit inside
  // one stable address there.
  //
  // AND THE ADDRESS IS LATCHED DURING THE WINDOW, not sampled after it.
  // "Latches what is on its pins at the END of that window" means the pins
  // WHILE WE' IS LOW. Sampling `a` on the RISING edge instead reads the address
  // once the window has already closed -- which is a different cycle's address
  // as soon as the pulse is short. At 8 sys_clk per microinstruction the
  // startup `Link<-` landed in the wrong task's slot AGAIN, for this reason
  // rather than the multiple-write one; at 16x the address simply had not
  // moved yet. Latching during the window is what the part does and is
  // RATIO-INDEPENDENT, which is the property that matters: real time on an
  // FPGA needs SYSPER=2, not 16.
  reg we_d;
  reg [3:0] a_lat, d_lat;
  reg       ce_lat;
  always @(posedge sys_clk) begin
    we_d <= p13;
    if (!p13) begin                          // while WE' is asserted (low)
      a_lat  <= a;
      d_lat  <= {p12, p11, p4, p5};
      ce_lat <= ~p3;
    end
    if (p13 && !we_d && ce_lat) mem[a_lat] <= d_lat;   // WE' rising: commit
  end
  // `+ramwe`: the WRITE-PULSE ADDRESS CHECK. A real 10145 needs its address
  // stable for the whole of WE' low (setup before the leading edge, hold past
  // the trailing one); the board's timing guarantees that and the RTL's
  // clock-family skews may not. This commits at the trailing edge with the
  // LAST address seen, so an address that moves one sys_clk before WE' rises
  // sends the word to the NEW address -- for a per-task file (ProcH's T-file,
  // MemD's MDM) that is the previous task's word landing in the next task's
  // slot. Found 2026-09-05: the junk task's T read another task's value after
  // a one-instruction slice, and task 0's Md changed between two reads with
  // no fetch between them. Counted per package; the first few are printed.
  // ...and the same for the DATA. The word is taken at the trailing edge, so
  // a data bus that moves on to the next reference's word before WE' rises
  // stores that word instead (MemD's MDM file: task 0's Md read back as
  // another word with no fetch between, 2026-09-05).
  // Diagnostic counters are simulator state, not part of the register file.
  // Quartus 17 does not implement SystemVerilog's final block.
  // synthesis translate_off
  reg  [3:0] a_first = 4'd0, d_first = 4'd0; reg in_we = 1'b0;
  integer n_amoved = 0, n_dmoved = 0, n_writes = 0;
  reg  ramwe_dbg = 1'b0; initial ramwe_dbg = $test$plusargs("ramwe");
  always @(posedge sys_clk) begin
    if (!p13) begin
      if (!in_we) begin a_first <= a; d_first <= {p12, p11, p4, p5}; in_we <= 1'b1; end
    end else if (in_we) begin
      in_we <= 1'b0; n_writes = n_writes + 1;
      if (a_first != a_lat) begin
        n_amoved = n_amoved + 1;
        if (ramwe_dbg && n_amoved <= 12)
          $display("tb_exec: RAMWE %m: address moved during WE' low, first %h last %h, data %h (write #%0d)", a_first, a_lat, d_lat, n_writes);
      end
      if (d_first != d_lat) begin
        n_dmoved = n_dmoved + 1;
        if (ramwe_dbg && n_dmoved <= 12)
          $display("tb_exec: RAMWE %m: DATA moved during WE' low, first %h last %h, at address %h (write #%0d)", d_first, d_lat, a_lat, n_writes);
      end
    end
  end
  final if (ramwe_dbg && (n_amoved > 0 || n_dmoved > 0))
    $display("tb_exec: RAMWE %m: of %0d writes, %0d had the address move and %0d the data move during WE' low", n_writes, n_amoved, n_dmoved);
  // synthesis translate_on
  wire [3:0] q = mem[a];
  assign {p14, p15, p1, p2} = (!p3) ? q : 4'b0000;          // CE' gates the read


  // Board-wired pins the dictionary does not name (power and the like)
  wire _unused_pins = &{1'b0, p8, 1'b0};

endmodule

`default_nettype wire
