// dorado_trident -- a Trident disk drive at the end of the cable.
//
// The DskEth board talks to up to four drives over a cable the wire lists
// enumerate completely (46 signals: 4 drives x {Clock pair in, Data pair
// bidirectional, SecIndx', Selected'}, 9 status lines in, 13 tag lines out).
// Everything on the far end of that cable is a SEPARATE BOX, so none of it is
// in any netlist -- which is why `disk-input-test` had to force those lines by
// hand to prove the controller sees them. This module is those forces made
// synthesisable, so a real core can have a drive attached.
//
// WHAT IT PRESENTS, and each line is one the gates already established:
//
//   TtlReady'  TtlOnLine'  TtlTerm'   all asserted together. TtlTerm' IS PART
//     OF BEING ON LINE -- DskEth c24 wired-ORs pin 14 (from TtlOnLine') with
//     pin 1 (from TtlTerm'), so asserting OnLine alone leaves `NotOnLine`
//     high. A drive is on line when it is on line AND the cable is TERMINATED.
//   Selected'  asserted when the controller has selected this unit.
//   SecIndx'   the rotation. One pulse per sector, and a LONGER one per
//     revolution which the controller separates into `Sector` and `Index''
//     (c03, an MC10124) and thence into SectorTW and IndexTW, the two task
//     wakeups -- the same pair `src/disk.c` models as ctl->index_tw and
//     ctl->sector_tw.
//
// ACTIVE LOW, ALL OF THEM, and that is not decoration: `disk-input-test`
// gates that with no drive attached all seventeen of these idle DEASSERTED,
// because six of them once read asserted and INVENTED A DRIVE that was not
// there. So `attached` low must leave every output high.
//
// THE ROTATION IS REAL TIME. A Trident spins at 3600 RPM = 16.67 ms per
// revolution, which at the Dorado's 60 ns microcycle is 277,778 microcycles
// -- `DORADO_DISK_CYCLES_PER_REV` in dorado/include/disk.h, the same constant
// the C emulator's timing model uses. In sys_clk that is SYSPER times as
// many, so the period is derived from SYSPER rather than written down, and
// stays right when the oversampling ratio changes.
//
// THE DATA PATH. The drive returns bits on a differential pair with its own
// differential clock, and both pairs are cable lines the machine now takes a
// contribution on (`CABLE_BIDIR` in tools/sil_backplane.py). The DATA pair is
// genuinely bidirectional -- DskEth drives it through g02, an SN74125 quad
// TRI-STATE buffer enabled by `Unit0'Data'`, for writes -- and a disabled
// SN74125 contributes 0, so the wired-OR does the arbitration and the drive
// simply adds its own contribution. The CLOCK pair is tied to ground on the
// board (h41.4, a bias network at the receiver), which contributes 0 for the
// same reason.
//
// The bit rate follows WORDS_PER_TRACK and the revolution period. Electronic
// sector pulses need not divide the physical track capacity evenly: the
// Dorado uses 117 pulses on a 10,080-word T-80 track. The fractional divider
// preserves that rate without rounding every bit to a shorter period.
//
// WHAT IT SERVES. `data_bit` is supplied by the caller, which advances it on
// each `data_adv` pulse -- so the pack's actual contents live outside this
// module, where the HPS can fill them. With nothing supplying data the line
// reads 0, which is a blank pack rather than a fabricated one.

`default_nettype none

module dorado_trident #(
    // sys_clk per microinstruction, so the rotation is real time.
    parameter integer SYSPER = 2,
    // Sectors per revolution. The C emulator takes this from the pack's own
    // geometry (28 for PDI media, `disk.c`), so it belongs to the medium and
    // not to the drive -- hence a parameter with the common value as default.
    parameter integer SECTORS_PER_REV = 28,
    // 16-bit words in a sector, which with the rotation sets the bit rate.
    parameter integer WORDS_PER_SECTOR = 256,
    // Physical track capacity is independent of electronic sector pulses.
    // T-80: 20,160 bytes/track, Trident 76205-902 (1980), Table 2-1.
    parameter integer WORDS_PER_TRACK = SECTORS_PER_REV * WORDS_PER_SECTOR
) (
    input  wire sys_clk,
    input  wire reset,

    // Is a pack mounted? Low leaves every cable line DEASSERTED, which is what
    // "no drive" has to look like.
    input  wire attached,

    // Has the controller selected this unit? (From the tag lines; the caller
    // decodes the unit number.)
    input  wire selected,

    output wire TtlReady_n,
    output wire TtlOnLine_n,
    output wire TtlTerm_n,
    output wire Selected_n,
    output wire SecIndx_n,
    // THE INDEX HAS ITS OWN LINE. DskEth c03 (an MC10124) takes TtlIndex' and
    // TtlSector' as two inputs and makes Index' and Sector from them; the
    // longer pulse on SecIndx' above is kept, but it is this line the
    // controller's sector counter resets on (2026-09-04).
    output wire TtlIndex_n,

    // ---- the read data path ----
    // The next bit to send, and a pulse when the drive has taken it. The
    // caller owns the pack's contents.
    input  wire data_bit,
    output reg  data_adv,
    // The differential pairs, as contributions to the cable's wired-OR.
    output wire DataP,
    output wire DataM,
    output wire ClockP,
    output wire ClockM,
    output wire [15:0] sector_out
);

  // 277,778 microcycles per revolution, x SYSPER sys_clk each.
  localparam integer CYCLES_PER_REV = 277778 * SYSPER;
  localparam integer CYCLES_PER_SEC = CYCLES_PER_REV / SECTORS_PER_REV;
  // TriconD06.sil timing diagram: 1.2 us sector, 4.5 us index, with a
  // 3.0 us controller one-shot between them. The T-80 specification allows
  // 1.24 +/- .24 us and 4 +/- 1 us respectively. These are fixed pulse
  // widths, not fractions of the selected sector spacing. SYSPER here
  // counts fabric samples per 60 ns microinstruction.
  localparam integer SECTOR_PULSE   = 1200 * SYSPER / 60;
  localparam integer INDEX_PULSE    = 4500 * SYSPER / 60;

  // Physical bit cells include the format's preambles, sync and ECC, not
  // only the payload words. Two clock transitions consume one bit cell.
  localparam integer HALF_BITS_PER_REV = WORDS_PER_TRACK * 16 * 2;

  reg [31:0] bit_count;     // position within the bit cell
  reg        bitclk;

  reg [31:0] rev_count;     // position within the revolution
  reg [31:0] sec_count;     // position within the sector
  reg [15:0] sector;        // which sector is under the head

  always @(posedge sys_clk) begin
    if (reset || !attached) begin
      rev_count <= 32'd0;
      sec_count <= 32'd0;
      sector    <= 16'd0;
    end else begin
      if (rev_count >= CYCLES_PER_REV - 1) begin
        rev_count <= 32'd0;
        sec_count <= 32'd0;
        sector    <= 16'd0;              // index: back to sector 0
      end else begin
        rev_count <= rev_count + 32'd1;
        // Integer division may leave a few samples at the end of a turn.
        // They extend the last slot; they must not invent pulse 117 just
        // before index (which can start a controller transfer prematurely).
        if (sec_count >= CYCLES_PER_SEC - 1 && sector < SECTORS_PER_REV - 1) begin
          sec_count <= 32'd0;
          sector    <= sector + 16'd1;
        end else begin
          sec_count <= sec_count + 32'd1;
        end
      end
    end
  end

  // The index pulse is the sector-0 pulse, made longer. That is how a Trident
  // does it -- one line carries both, and the controller times them apart.
  wire in_index  = (sector == 16'd0) && (sec_count < INDEX_PULSE);
  wire in_sector = (sector != 16'd0) && (sec_count < SECTOR_PULSE);

  // Media position advances whenever the platter turns, INCLUDING while
  // deselected. Selection gates the cable below, not the physical bit cells.
  // Otherwise a selection partway through a sector resumes its preamble at
  // an arbitrary angle even though the sector/index pulses kept running.
  always @(posedge sys_clk) begin
    if (reset || !attached) begin
      bit_count <= 32'd0; bitclk <= 1'b0; data_adv <= 1'b0;
    end else begin
      data_adv <= 1'b0;
      // Fractional divider: rounding each half-bit down changes the data
      // rate and can make a formatted sector overrun its angular slot.
      if (bit_count + HALF_BITS_PER_REV >= CYCLES_PER_REV) begin
        bit_count <= bit_count + HALF_BITS_PER_REV - CYCLES_PER_REV;
        bitclk    <= ~bitclk;
        // One bit is consumed per FULL clock period, on the same edge the
        // controller's shift register uses.
        if (bitclk) data_adv <= 1'b1;
      end else begin
        bit_count <= bit_count + HALF_BITS_PER_REV;
      end
    end
  end

  wire live = attached & selected;
  // TriconD07.sil: f02 compares DataM (V1) against DataP (V2), whereas
  // f01 compares ClockP against ClockM. The write path agrees: b02/g02
  // drive DataP with the complemented data and DataM with true data.
  assign DataP  = live ? ~data_bit : 1'b0;
  assign DataM  = live ?  data_bit : 1'b0;
  assign ClockP = live ?  bitclk   : 1'b0;
  assign ClockM = live ? ~bitclk   : 1'b0;

  assign SecIndx_n   = attached ? ~(in_index | in_sector) : 1'b1;
  // Trident 76205-902 section 4.2: the nine bussed status outputs,
  // including Index, are gated with Select; composite SecIndx is not.
  // DiskSubrs.mc UpdateSector explicitly relies on missing IndexTW while
  // deselected when the subsector count does not divide a revolution.
  assign TtlIndex_n  = live ? ~in_index : 1'b1;
  assign TtlReady_n  = ~attached;
  assign TtlOnLine_n = ~attached;
  assign TtlTerm_n   = ~attached;    // terminated, and c24 needs it to be
  assign Selected_n  = ~(attached & selected);

  assign sector_out = sector;
endmodule

`default_nettype wire
