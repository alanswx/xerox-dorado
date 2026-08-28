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
// NOT YET MODELLED: the data path. Serving bits needs the read sequencer
// driven through its format-RAM program, which `disk-format-test` exercises
// from the controller side; the drive half of it is the next step. Until then
// `ReadData` and the bit clock idle, so a read finds a ready, rotating,
// selected drive and no data -- which is a truthful state for a blank pack
// rather than a fabricated one.

`default_nettype none

module dorado_trident #(
    // sys_clk per microinstruction, so the rotation is real time.
    parameter integer SYSPER = 2,
    // Sectors per revolution. The C emulator takes this from the pack's own
    // geometry (28 for PDI media, `disk.c`), so it belongs to the medium and
    // not to the drive -- hence a parameter with the common value as default.
    parameter integer SECTORS_PER_REV = 28
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
    output wire SecIndx_n
);

  // 277,778 microcycles per revolution, x SYSPER sys_clk each.
  localparam integer CYCLES_PER_REV = 277778 * SYSPER;
  localparam integer CYCLES_PER_SEC = CYCLES_PER_REV / SECTORS_PER_REV;
  // A sector pulse is short; the index pulse is the one the controller can
  // tell apart by being LONGER. Four times is comfortably distinguishable and
  // still far shorter than a sector.
  localparam integer SECTOR_PULSE   = CYCLES_PER_SEC / 64;
  localparam integer INDEX_PULSE    = SECTOR_PULSE * 4;

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
        if (sec_count >= CYCLES_PER_SEC - 1) begin
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

  assign SecIndx_n   = attached ? ~(in_index | in_sector) : 1'b1;
  assign TtlReady_n  = ~attached;
  assign TtlOnLine_n = ~attached;
  assign TtlTerm_n   = ~attached;    // terminated, and c24 needs it to be
  assign Selected_n  = ~(attached & selected);

endmodule

`default_nettype wire
