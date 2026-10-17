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
// 28". DiskSubrs.mc UpdateSector numbers COMMAND sectors: a command issued
// after Index executes at the NEXT divided pulse (HM 9.5). Thus Alto file
// sector s (0..27) begins at pulse 4*(s+1), not 4*s. The spare file slot 28
// occupies the otherwise unused complete slot after Index; pulse 116 starts
// the unusable fractional slot. This mapping is specific to this Alto pack
// format, not a general Trident image codec. dsk2trident lays files out as
// 815 cylinders x 5 heads x 29 short sectors of 534 bytes each,
// dummy(1w) + header(2w) + label(8w) + data(256w), little-endian words,
// 63,105,450 bytes for games-trident.pack.
//
// HM 9.3 and AltoDiabloDisk.mc ACmmdTable require a 201B sync WORD for
// Diablo emulation (001B is the different Alto Trident format). The detector
// needs both ShiftReg.08 and .15; a single 1 misses the header entirely.
// Preamble/gap lengths follow HM 9.6's WRITE sequence, not merely the shorter
// READ delays: 1+2+1+28 words before sync, and 2 postamble + 1 advance +
// 1 write tag + 7 preamble words between blocks. Then the words MSB
// first, then the 32-bit Fire-code check word MSB first. The polynomial is
// HM 9.10's, X^32+X^23+X^21+X^11+X^2+1 with feedback taps {0,2,11,21,23},
// register zero before each SYNC WORD. The divider covers sync + payload;
// DskEth's read divider is already enabled while hunting for sync. The C
// emulator explicitly uses self-consistent, not physical, ECC over payload
// only; copying that omission here gives a nonzero hardware remainder.
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
    parameter integer PRE_WORDS = 32,     // HM 9.6 write steps 0..3
    parameter integer GAP_WORDS = 11,     // postamble + steps 8..10
    parameter integer LABEL_WORDS = 8,
    parameter integer TRACK_WORDS = 29 * (1+2+LABEL_WORDS+256),
    parameter integer TRACK_BYTES = 2 * TRACK_WORDS,
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
    // Returned data cable during writes: TriconD07 b02/g02 drives M=true,
    // P=complement. The drive samples one bit per timing-track bit cell.
    input  wire        selected,
    input  wire        write_data_p, write_data_m,
    output wire        write_gate,
    output reg [31:0]  n_write_blocks = 0,
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
  // Volatile media changes survive track switches, never modify +pack.
  reg [15:0] written_words [longint unsigned];
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
      off = ((c * HEADS) + h) * TRACK_BYTES;
      rc = $fseek(fd, off, 0);
      for (i = 0; i < TRACK_BYTES; i = i + 1) tbytes[i] = $fgetc(fd);
      for (i = 0; i < TRACK_WORDS; i = i + 1)
        if (written_words.exists((longint'(c) * HEADS + h) * TRACK_WORDS + i)) begin
          tbytes[2*i] = written_words[(longint'(c) * HEADS + h) * TRACK_WORDS + i][7:0];
          tbytes[2*i+1] = written_words[(longint'(c) * HEADS + h) * TRACK_WORDS + i][15:8];
        end
      cur_cyl = c; cur_head = h; n_tracks = n_tracks + 1;
    end
  endtask
  function [15:0] track_word(input integer wi);   // little-endian
    track_word = {tbytes[2*wi+1], tbytes[2*wi]};
  endfunction

  // Compatibility behavior inherited from disk.c: illegal/legal head tags
  // set/clear this status; DeviceCheckReset or ReZero also clears it.
  // This is NOT yet reconciled with Trident 76205-902 section 4.2, which
  // describes End of Cylinder as a level indicating an out-of-range head.
  // The former boot-failure rationale did not establish latch semantics:
  // the context-sized complement below made EVERY head appear illegal.
  // Keep the reset behavior separate from that proven width defect.
  reg eoc = 1'b0;
  assign endofcyl_n = ~(attached && eoc);
  // Keep the complement six bits BEFORE comparing with integer HEADS.
  // Context-sizing ~tagbus_n[5:0] inside that comparison otherwise extends
  // it to 32 bits first, making every legal head look larger than HEADS.
  wire [5:0] tag_head = ~tagbus_n[5:0];

  // ---- the tags -----------------------------------------------------------
  reg cyltag_d = 1'b1, headtag_d = 1'b1, drivetag_d = 1'b1, conttag_d = 1'b1;
  // HM 9.6 keeps ControlTag asserted while RAM[6] (head select) changes to
  // RAM[5] (write + head select). These drive functions are cable LEVELS,
  // not a command latched only on ControlTag's first falling edge.
  assign write_gate = attached && selected && !conttag_n &&
                      !tagbus_n[7] && !tagbus_n[2];
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
      head <= tag_head; n_headtag <= n_headtag + 1;
      eoc  <= (tag_head >= HEADS);
      if (fd != 0 && tag_head < HEADS && tag_head != cur_head) load_track(cyl, tag_head);
    end
    if (drivetag_d && !drivetag_n) n_drivetag <= n_drivetag + 1;
    if (conttag_d && !conttag_n) begin
      n_conttag <= n_conttag + 1;
      // Control tag (HM p.101, disk.c's table): bit 0 HeadAdvance, bit 1
      // ReZero, bit 3 DeviceCheckReset. Read, Write, the strobes and
      // AltoLeader are the controller's own business and are not the drive's.
      // Bus value = ~tagbus_n, LSB-numbered from .9.
      if (~tagbus_n[3] || ~tagbus_n[1]) eoc <= 1'b0;
      if (~tagbus_n[1]) begin
        cyl <= 12'd0; head <= 6'd0;
        if (fd != 0 && (cur_cyl != 12'd0 || cur_head != 6'd0)) load_track(12'd0, 6'd0);
      end else if (~tagbus_n[0]) begin
        head <= head + 6'd1;
        eoc  <= ((head + 6'd1) >= HEADS);
        if (fd != 0 && (head + 6'd1) < HEADS) load_track(cyl, head + 6'd1);
      end
    end
  end

  // ---- the bit stream -----------------------------------------------------
  localparam integer TAPS = (1 << 0) | (1 << 2) | (1 << 11) | (1 << 21) | (1 << 23);
  function automatic [31:0] sync_ecc;
    reg [31:0] r;
    reg [15:0] s;
    integer k;
    begin
      r=0; s=16'o201;
      for(k=15;k>=0;k=k-1)
        r={r[30:0],1'b0} ^ ((s[k]^r[31]) ? TAPS[31:0] : 32'd0);
      sync_ecc=r;
    end
  endfunction
  // phases within a short sector
  localparam PH_IDLE = 0, PH_PRE = 1, PH_SYNC = 2, PH_WORDS = 3, PH_ECC = 4, PH_GAP = 5;
  integer phase = PH_IDLE, blk = 0;          // blk 0 = header, 1 = label, 2 = data
  integer cnt = 0;                           // bits left in the current phase
  integer wi = 0;                            // word index within the sector (0..266)
  integer bi = 15;                           // bit within the word, MSB first
  reg [31:0] ecc = 32'd0;
  reg [15:0] sync_word;
  reg [15:0] pulse_d = 16'hFFFF;
  integer sec = 0;
  localparam integer SECTOR_WORDS = 1+2+LABEL_WORDS+256;
  function integer blk_words(input integer b);
    blk_words = (b == 0) ? 2 : (b == 1) ? LABEL_WORDS : 256;
  endfunction
  function integer blk_base(input integer b);        // word offset in the sector image
    blk_base = (b == 0) ? 1 : (b == 1) ? 3 : 3+LABEL_WORDS; // dummy word at 0
  endfunction
  wire [15:0] cur_word = track_word(sec * SECTOR_WORDS + wi);
  wire        cur_data_bit = cur_word[bi];

  // Capture actual serial writes, including SYNC and both Fire-code words.
  // Commit a complete checked block only; an invalid outgoing stream is a
  // diagnostic failure, never silently replaced with regenerated good ECC.
  integer wr_state=0, wr_bits=0, wr_count=0, wr_block=0, wr_sector=0;
  reg [11:0] wr_cyl;
  reg [5:0] wr_head;
  reg [15:0] wr_shift=0, wr_payload [0:255];
  reg [31:0] wr_ecc=0;
  reg wr_gate_d=0;
  longint unsigned wr_address;
  always @(posedge sys_clk) begin
    wr_gate_d <= write_gate;
    if(dbg && write_gate != wr_gate_d) begin
      $display("PACKWRITE gate=%b selected=%b control'=%b bus=%h block=%0d phase=%0d",
        write_gate,selected,conttag_n,~tagbus_n,blk,phase);
      $fflush();
    end
    if (!write_gate) begin
      if (wr_gate_d && wr_state != 0)
        $fatal(1,"dorado_pack: truncated write c%0d h%0d s%0d block%0d words%0d bits%0d",
          wr_cyl,wr_head,wr_sector,wr_block,wr_count,wr_bits);
      wr_state=0; wr_shift=0; wr_bits=0;
    end else if (data_adv) begin
      if (write_data_p == write_data_m) begin
        if (wr_state != 0) $fatal(1,"dorado_pack: invalid differential write data");
        wr_shift=0;
      end else begin
        case (wr_state)
          0: begin
            wr_shift={wr_shift[14:0],write_data_m};
            if (wr_shift == 16'o201) begin
              wr_state=1; wr_count=0; wr_bits=0; wr_shift=0;
              wr_ecc=sync_ecc(); wr_block=blk; wr_sector=sec;
              wr_cyl=cyl; wr_head=head;
              if(dbg) $display("PACKWRITE sync c%0d h%0d s%0d block%0d",wr_cyl,wr_head,wr_sector,wr_block);
              if (wr_block>2 || wr_head>=HEADS || wr_cyl>=815)
                $fatal(1,"dorado_pack: write outside supported pack geometry");
            end
          end
          1: begin
            wr_shift={wr_shift[14:0],write_data_m};
            wr_ecc={wr_ecc[30:0],1'b0} ^ ((write_data_m ^ wr_ecc[31]) ? TAPS[31:0] : 32'd0);
            wr_bits=wr_bits+1;
            if(wr_bits==16) begin
              wr_payload[wr_count]=wr_shift; wr_count=wr_count+1; wr_bits=0;
              if(wr_count==blk_words(wr_block)) wr_state=2;
            end
          end
          2: begin
            wr_ecc={wr_ecc[30:0],1'b0} ^ ((write_data_m ^ wr_ecc[31]) ? TAPS[31:0] : 32'd0);
            wr_bits=wr_bits+1;
            if(wr_bits==32) begin
              if(wr_ecc!=0)
                $fatal(1,"dorado_pack: write Fire-code remainder %h c%0d h%0d s%0d block%0d",
                  wr_ecc,wr_cyl,wr_head,wr_sector,wr_block);
              for(integer k=0;k<wr_count;k=k+1) begin
                wr_address=(longint'(wr_cyl)*HEADS+wr_head)*TRACK_WORDS+
                  wr_sector*SECTOR_WORDS+blk_base(wr_block)+k;
                written_words[wr_address]=wr_payload[k];
                if(cur_cyl==wr_cyl && cur_head==wr_head) begin
                  tbytes[2*(wr_sector*SECTOR_WORDS+blk_base(wr_block)+k)]=wr_payload[k][7:0];
                  tbytes[2*(wr_sector*SECTOR_WORDS+blk_base(wr_block)+k)+1]=wr_payload[k][15:8];
                end
              end
              n_write_blocks=n_write_blocks+1;
              $display("PACKWRITE c%0d h%0d s%0d block%0d words=%0d ECC=0 (volatile overlay)",
                wr_cyl,wr_head,wr_sector,wr_block,wr_count);
              wr_state=0; wr_bits=0; wr_shift=0;
            end
          end
        endcase
      end
    end
  end

  always @(posedge sys_clk) begin
    pulse_d <= pulse;
    // Translate physical angular slots into the converter's Alto file slots.
    // The C controller starts a read immediately in the command sector;
    // this real sequencer starts on its successor pulse (HM 9.5).
    if (attached && pulse != pulse_d && (pulse & 16'd3) == 16'd0) begin
      if ((pulse >> 2) <= 28) begin
        sec = ((pulse >> 2) + 28) % 29;
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
          if (cnt <= 1) begin
            phase = PH_SYNC; cnt = 16; sync_word = 16'o201;
            data_bit <= sync_word[15];
          end
          else cnt = cnt - 1;
        end
        PH_SYNC: begin
          cnt = cnt - 1;
          if (cnt != 0) begin
            sync_word = {sync_word[14:0], 1'b0};
            data_bit <= sync_word[15];
          end else begin
          // The complete sync word has been taken; first data bit next.
          // The Fire-code divider runs while hunting for sync, too.
          // The outgoing stream is sync + data, not just the pack payload.
          wi = blk_base(blk); bi = 15; ecc = sync_ecc();
          phase = PH_WORDS; cnt = blk_words(blk) * 16;
          data_bit <= track_word(sec * SECTOR_WORDS + blk_base(blk))[15];
          if (dbg) $display("dorado_pack: sector %0d block %0d starts at image word %0d: %04h %04h (tbytes[%0d..]=%02h %02h %02h %02h)",
                            sec, blk, wi, track_word(sec*SECTOR_WORDS+wi), track_word(sec*SECTOR_WORDS+wi+1),
                            2*(sec*SECTOR_WORDS+wi), tbytes[2*(sec*SECTOR_WORDS+wi)], tbytes[2*(sec*SECTOR_WORDS+wi)+1], tbytes[2*(sec*SECTOR_WORDS+wi)+2], tbytes[2*(sec*SECTOR_WORDS+wi)+3]);
          end
        end
        PH_WORDS: begin
          // the bit just taken was cur_word[bi]: fold it into the ECC
          ecc = {ecc[30:0], 1'b0} ^ ((cur_data_bit ^ ecc[31]) ? TAPS[31:0] : 32'd0);
          cnt = cnt - 1;
          if (cnt == 0) begin
            phase = PH_ECC; cnt = 32; data_bit <= ecc[31];
          end else begin
            if (bi == 0) begin bi = 15; wi = wi + 1; end else bi = bi - 1;
            data_bit <= track_word(sec * SECTOR_WORDS + wi)[bi];
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
