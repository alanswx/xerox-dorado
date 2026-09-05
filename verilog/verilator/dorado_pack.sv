// dorado_pack -- a Trident pack at the end of the cable, for the testbench.
//
// `dorado_trident` is the DRIVE: the rotation, the sector pulses, the bit
// clock, and the status lines. It leaves the pack's contents to the caller
// ("data_bit is supplied by the caller, which advances it on each data_adv
// pulse"). This is that caller: it decodes the controller's tags off the
// cable, fetches the addressed track from a pack file, and streams each short
// sector as the DskEth board expects to read it.
//
// THE FORMAT IS THE WORLD'S, not ours. AEmu's InitRamDiablo loads the format
// RAM with header count-1 = 1, label count-1 = 7, data count-1 = 377B, ECC
// words-1 = 1, read delay 11B word clocks before the first block and 2
// between blocks (AltoDiabloDisk.mc). The drive gives 117 pulses per
// revolution and the controller's subsector counter (count 3, divisor 4)
// makes 30 sector wakeups of them -- "29 + a fraction, though we use only
// 28" -- so short sector s begins at drive pulse 4s, and the 30th slot is
// the fraction and holds nothing. dsk2trident lays the pack out in exactly
// those terms: 815 cylinders x 5 heads x 29 short sectors of 534 bytes each,
// dummy(1w) + header(2w) + label(8w) + data(256w), little-endian words,
// 63,105,450 bytes for games-trident.pack.
//
// On the wire each block is: a run of zero bits (the controller's read delay
// is measured in WORD clocks, so PRE_WORDS of them), ONE sync bit of 1 --
// the controller starts its bit counter on the first 1 it sees
// (`sCountBits = (ShiftReg.08 | Tag.000) & ShiftReg.15`) -- the words MSB
// first, then the 32-bit Fire-code check word MSB first. The polynomial is
// HM 9.10's, X^32+X^23+X^21+X^11+X^2+1 with feedback taps {0,2,11,21,23},
// register zero at each block, exactly dorado_disk_ecc_compute in
// dorado/src/disk.c, bit-serial here because that is how the hardware
// makes it.
//
// TAGS. The controller presents a twelve-bit bus and strobes one of four
// tag lines (HM p.100, disk.c: DriveTag 0x8000, CylinderTag 0x4000, HeadTag
// 0x2000, ControlTag 0x1000 in the DiskTag word, bus = the low twelve bits).
// On the cable the bus lines are the 8T98 inverting copies of the tag
// register -- TagBus.k' = ~Tag.k -- named .000 .00 .0 .1 .. .9, MSB first:
// Tag[4:15] is the cylinder number, so .000 is the MSB. Head is the low six.
// Unit selection arrives on its own line, Select0', from the controller's
// SN7438 decode.
`default_nettype none

module dorado_pack #(
    parameter integer PRE_WORDS = 12,     // zero words before a block's sync
    parameter integer GAP_WORDS = 4,      // zero words between blocks
    parameter integer TRACK_WORDS = 29 * 267,
    parameter integer TRACK_BYTES = 29 * 534,
    parameter integer HEADS = 5           // a T-80; the world probes head 5 to tell it from a 19-head AMS-315
) (
    input  wire        sys_clk,
    input  wire        attached,
    // the cable, as the controller drives it: all ACTIVE LOW
    input  wire [11:0] tagbus_n,          // {.000,.00,.0,.1,...,.9}
    input  wire        cyltag_n,
    input  wire        headtag_n,
    input  wire        drivetag_n,
    input  wire        conttag_n,
    // the drive's rotation and bit clock
    input  wire [15:0] pulse,             // dorado_trident's `sector`: 0..116
    input  wire        data_adv,
    output reg         data_bit,
    // what it did, for the run report
    // THE DRIVE TYPE IS ON THE CABLE. DskEth b24 translates TtlEndOfCyl' into
    // HeadOvfl, the muffler bit AltoDiabloDisk.mc reads after selecting head 5:
    // asserted means a T-80 (MaxPartition 5, the Alto disk on head 4), clear
    // means an AMS-315 with 19 heads (MaxPartition 23B, head 18). A drive
    // that never asserts it is taken for the bigger machine.
    output wire        endofcyl_n,
    output reg  [11:0] cyl,
    output reg  [5:0]  head,
    output reg  [31:0] n_cyltag, n_headtag, n_drivetag, n_conttag,
    output reg  [31:0] n_tracks, n_sectors, n_bits, n_blocks
);
  // ---- the pack file, one track at a time ---------------------------------
  string  path; integer fd = 0, rc; integer dbg = 0;
  initial dbg = $test$plusargs("packdbg");
  reg [7:0]  tbytes [0:TRACK_BYTES-1];
  reg [11:0] cur_cyl  = 12'hFFF;         // the track in tbytes[], none yet
  reg [5:0]  cur_head = 6'h3F;
  initial begin
    cyl = 12'd0; head = 6'd0;
    n_cyltag = 0; n_headtag = 0; n_drivetag = 0; n_conttag = 0;
    n_tracks = 0; n_sectors = 0; n_bits = 0; n_blocks = 0;
    data_bit = 1'b0;
    if ($value$plusargs("pack=%s", path)) begin
      fd = $fopen(path, "rb");
      if (fd == 0) $fatal(1, "dorado_pack: cannot open %s", path);
      $display("tb_exec: +pack -- %s at the end of the disk cable (unit 0)", path);
    end
  end
  task load_track(input [11:0] c, input [5:0] h);
    integer off, i;
    begin
      off = ((c * 5) + h) * TRACK_BYTES;
      rc = $fseek(fd, off, 0);
      for (i = 0; i < TRACK_BYTES; i = i + 1) tbytes[i] = $fgetc(fd);
      cur_cyl = c; cur_head = h; n_tracks = n_tracks + 1;
    end
  endtask
  function [15:0] track_word(input integer wi);   // little-endian
    track_word = {tbytes[2*wi+1], tbytes[2*wi]};
  endfunction

  assign endofcyl_n = ~(attached && (head >= HEADS));

  // ---- the tags -----------------------------------------------------------
  reg cyltag_d = 1'b1, headtag_d = 1'b1, drivetag_d = 1'b1, conttag_d = 1'b1;
  always @(posedge sys_clk) begin
    cyltag_d <= cyltag_n; headtag_d <= headtag_n;
    drivetag_d <= drivetag_n; conttag_d <= conttag_n;
    // A SEEK MOVES THE HEADS AT ONCE: the track under them changes on the tag,
    // not at the next sector. (Loading lazily at the sector start served
    // cylinder 0 for a whole sector after the tags had named cylinder 3.)
    if (dbg && ((cyltag_d && !cyltag_n) || (headtag_d && !headtag_n) || (drivetag_d && !drivetag_n) || (conttag_d && !conttag_n)))
      $display("tb_exec: TAG %s bus=%03h (cyl %0d / head %0d / unit %0d)",
               (cyltag_d && !cyltag_n) ? "CYLINDER" : (headtag_d && !headtag_n) ? "HEAD" : (drivetag_d && !drivetag_n) ? "DRIVE" : "CONTROL",
               ~tagbus_n, ~tagbus_n, (~tagbus_n) & 12'h03f, (~tagbus_n) & 12'h00f);
    if (cyltag_d && !cyltag_n) begin
      cyl <= ~tagbus_n; n_cyltag <= n_cyltag + 1;
      if (fd != 0 && (~tagbus_n) != cur_cyl) load_track(~tagbus_n, head);
    end
    if (headtag_d && !headtag_n) begin
      head <= ~tagbus_n[5:0]; n_headtag <= n_headtag + 1;
      if (fd != 0 && (~tagbus_n[5:0]) != cur_head) load_track(cyl, ~tagbus_n[5:0]);
    end
    if (drivetag_d && !drivetag_n) n_drivetag <= n_drivetag + 1;
    if (conttag_d && !conttag_n)   n_conttag  <= n_conttag + 1;
  end

  // ---- the bit stream -----------------------------------------------------
  localparam integer TAPS = (1 << 0) | (1 << 2) | (1 << 11) | (1 << 21) | (1 << 23);
  // phases within a short sector
  localparam PH_IDLE = 0, PH_PRE = 1, PH_SYNC = 2, PH_WORDS = 3, PH_ECC = 4, PH_GAP = 5;
  integer phase = PH_IDLE, blk = 0;          // blk 0 = header, 1 = label, 2 = data
  integer cnt = 0;                           // bits left in the current phase
  integer wi = 0;                            // word index within the sector (0..266)
  integer bi = 15;                           // bit within the word, MSB first
  reg [31:0] ecc = 32'd0;
  reg [15:0] pulse_d = 16'hFFFF;
  integer sec = 0;
  function integer blk_words(input integer b);
    blk_words = (b == 0) ? 2 : (b == 1) ? 8 : 256;
  endfunction
  function integer blk_base(input integer b);        // word offset in the sector image
    blk_base = (b == 0) ? 1 : (b == 1) ? 3 : 11;    // dummy word at 0
  endfunction
  wire [15:0] cur_word = track_word(sec * 267 + wi);
  wire        cur_data_bit = cur_word[bi];

  always @(posedge sys_clk) begin
    pulse_d <= pulse;
    // a new short sector begins at drive pulse 4s
    if (attached && pulse != pulse_d && (pulse & 16'd3) == 16'd0) begin
      sec = pulse >> 2;
      if (sec <= 28) begin
        if (fd != 0 && (cyl != cur_cyl || head != cur_head)) load_track(cyl, head);
        phase = PH_PRE; cnt = PRE_WORDS * 16; blk = 0; n_sectors <= n_sectors + 1;
        data_bit <= 1'b0;
      end else begin
        phase = PH_IDLE; data_bit <= 1'b0;
      end
    end else if (attached && data_adv) begin
      n_bits <= n_bits + 1;
      case (phase)
        PH_PRE, PH_GAP: begin
          data_bit <= 1'b0;
          if (cnt <= 1) begin phase = PH_SYNC; data_bit <= 1'b1; end
          else cnt = cnt - 1;
        end
        PH_SYNC: begin
          // the sync bit has been taken; first word bit next
          wi = blk_base(blk); bi = 15; ecc = 32'd0;
          phase = PH_WORDS; cnt = blk_words(blk) * 16;
          data_bit <= track_word(sec * 267 + blk_base(blk))[15];
          if (dbg) $display("dorado_pack: sector %0d block %0d starts at image word %0d: %04h %04h (tbytes[%0d..]=%02h %02h %02h %02h)",
                            sec, blk, wi, track_word(sec*267+wi), track_word(sec*267+wi+1),
                            2*(sec*267+wi), tbytes[2*(sec*267+wi)], tbytes[2*(sec*267+wi)+1], tbytes[2*(sec*267+wi)+2], tbytes[2*(sec*267+wi)+3]);
        end
        PH_WORDS: begin
          // the bit just taken was cur_word[bi]: fold it into the ECC
          ecc = {ecc[30:0], 1'b0} ^ ((cur_data_bit ^ ecc[31]) ? TAPS[31:0] : 32'd0);
          cnt = cnt - 1;
          if (cnt == 0) begin
            phase = PH_ECC; cnt = 32; data_bit <= ecc[31];
          end else begin
            if (bi == 0) begin bi = 15; wi = wi + 1; end else bi = bi - 1;
            data_bit <= track_word(sec * 267 + wi)[bi];
          end
        end
        PH_ECC: begin
          ecc = {ecc[30:0], 1'b0};
          cnt = cnt - 1;
          if (cnt == 0) begin
            n_blocks <= n_blocks + 1;
            if (blk == 2) begin phase = PH_IDLE; data_bit <= 1'b0; end
            else begin blk = blk + 1; phase = PH_GAP; cnt = GAP_WORDS * 16; data_bit <= 1'b0; end
          end else data_bit <= ecc[31];
        end
        default: data_bit <= 1'b0;
      endcase
    end
  end
endmodule
`default_nettype wire
