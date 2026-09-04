// tb_pack -- dorado_trident + dorado_pack against the pack file.
//
// Strobes a cylinder and head onto the tag bus the way the controller does,
// lets the drive turn, and decodes the bit stream of one short sector: the
// first 1 after the zero preamble is the sync, then 16-bit words MSB first,
// then a 32-bit check word. Every decoded word is compared with the file
// read independently here, and the Fire-code register over data+check must
// come out zero. It checks the plumbing and the framing; whether the DskEth
// board reads the same stream is the boot's business.
`timescale 1ns/1ps
module tb_pack;
  localparam integer SYSPER = 2;
  reg sys_clk = 0; always #1 sys_clk = ~sys_clk;
  reg [11:0] tagbus_n = 12'hFFF; reg cyltag_n = 1, headtag_n = 1, drivetag_n = 1, conttag_n = 1;
  reg selected = 0;
  wire ready_n, online_n, term_n, sel_n, sec_n, adv, bit_, dp, dm, cp, cm;
  wire [11:0] cyl; wire [5:0] head; wire [31:0] n_ct, n_ht, n_dt, n_kt, n_tr, n_se, n_bi, n_bl;
  dorado_trident #(.SYSPER(2*SYSPER), .SECTORS_PER_REV(117), .WORDS_PER_SECTOR(80)) drv (
    .sys_clk(sys_clk), .reset(1'b0), .attached(1'b1), .selected(selected),
    .TtlReady_n(ready_n), .TtlOnLine_n(online_n), .TtlTerm_n(term_n), .Selected_n(sel_n),
    .SecIndx_n(sec_n), .data_bit(bit_), .data_adv(adv), .DataP(dp), .DataM(dm), .ClockP(cp), .ClockM(cm));
  dorado_pack pk (
    .sys_clk(sys_clk), .attached(1'b1), .tagbus_n(tagbus_n), .cyltag_n(cyltag_n), .headtag_n(headtag_n),
    .drivetag_n(drivetag_n), .conttag_n(conttag_n), .pulse(drv.sector), .data_adv(adv), .data_bit(bit_),
    .cyl(cyl), .head(head), .n_cyltag(n_ct), .n_headtag(n_ht), .n_drivetag(n_dt), .n_conttag(n_kt),
    .n_tracks(n_tr), .n_sectors(n_se), .n_bits(n_bi), .n_blocks(n_bl));

  // the file, read independently
  string path; integer fd, i, rc; reg [7:0] fb [0:533];
  localparam integer CYL = 3, HEAD = 4, SECTOR = 0;
  // decoder
  integer zeros = 0, state = 0, nb = 0, wcount = 0, blk = 0, want = 2, errs = 0, ecc_bad = 0;
  reg [15:0] sh; reg [31:0] eccr; integer wordn = 0;
  localparam integer TAPS = (1 << 0) | (1 << 2) | (1 << 11) | (1 << 21) | (1 << 23);
  function [15:0] fw(input integer wi); fw = {fb[2*wi+1], fb[2*wi]}; endfunction
  integer base [0:2]; integer nblk_done = 0; integer served_sector = -1;
  reg [15:0] pulse_d = 16'hFFFF;
  always @(posedge sys_clk) begin
    pulse_d <= drv.sector;
    if (drv.sector != pulse_d && (drv.sector & 16'd3) == 16'd0) served_sector = drv.sector >> 2;
    if (adv && served_sector == SECTOR) begin
      case (state)
        0: begin // hunting for sync after zeros
          if (bit_ == 1'b0) zeros = zeros + 1;
          else if (zeros >= 16) begin state = 1; nb = 0; sh = 0; wcount = 0; eccr = 0; end
          else zeros = 0;
        end
        1: begin // words
          sh = {sh[14:0], bit_}; nb = nb + 1;
          eccr = {eccr[30:0], 1'b0} ^ ((bit_ ^ eccr[31]) ? TAPS[31:0] : 32'd0);
          if (nb == 16) begin
            if (sh !== fw(base[blk] + wcount)) begin
              errs = errs + 1;
              if (errs <= 4) $display("tb_pack: block %0d word %0d: got %04h want %04h", blk, wcount, sh, fw(base[blk] + wcount));
            end
            wordn = wordn + 1; wcount = wcount + 1; nb = 0;
            if (wcount == want) begin state = 2; nb = 0; end
          end
        end
        2: begin // check word: 32 bits through the same register must leave it zero
          eccr = {eccr[30:0], 1'b0} ^ ((bit_ ^ eccr[31]) ? TAPS[31:0] : 32'd0);
          nb = nb + 1;
          if (nb == 32) begin
            if (eccr != 32'd0) ecc_bad = ecc_bad + 1;
            nblk_done = nblk_done + 1;
            blk = blk + 1; want = (blk == 1) ? 8 : 256; zeros = 0; state = (blk == 3) ? 3 : 0;
          end
        end
        default: ;
      endcase
    end
  end
  initial begin
    base[0] = 1; base[1] = 3; base[2] = 11;
    if (!$value$plusargs("pack=%s", path)) path = "chm/diskpacks/games-trident.pack";
    fd = $fopen(path, "rb");
    if (fd == 0) $fatal(1, "tb_pack: cannot open %s", path);
    rc = $fseek(fd, (((CYL * 5) + HEAD) * 29 + SECTOR) * 534, 0);
    for (i = 0; i < 534; i = i + 1) fb[i] = $fgetc(fd);
    $display("tb_pack: file sector c%0d h%0d s%0d: header %04h %04h, label[0] %04h, data[0..3] %04h %04h %04h %04h",
             CYL, HEAD, SECTOR, fw(1), fw(2), fw(3), fw(11), fw(12), fw(13), fw(14));
    // the controller seeks: cylinder then head, then selects the unit
    repeat (20) @(posedge sys_clk);
    tagbus_n = ~CYL[11:0]; @(posedge sys_clk); cyltag_n = 0; repeat (6) @(posedge sys_clk); cyltag_n = 1;
    repeat (6) @(posedge sys_clk);
    tagbus_n = ~{6'd0, HEAD[5:0]}; @(posedge sys_clk); headtag_n = 0; repeat (6) @(posedge sys_clk); headtag_n = 1;
    tagbus_n = 12'hFFF; selected = 1;
    // wait for the drive to serve the sector and the decoder to finish it
    wait (nblk_done == 3 || drv.rev_count > 277778 * 2 * SYSPER * 3 / 2);
    repeat (100) @(posedge sys_clk);
    $display("tb_pack: drive latched cyl %0d head %0d (tag strobes cyl %0d head %0d); tracks loaded %0d, sectors started %0d, bits %0d, blocks %0d",
             cyl, head, n_ct, n_ht, n_tr, n_se, n_bi, n_bl);
    $display("tb_pack: decoded %0d blocks, %0d words, %0d word mismatches, %0d bad check words", nblk_done, wordn, errs, ecc_bad);
    if (nblk_done != 3) $fatal(1, "tb_pack: FAIL -- the sector did not decode as three blocks");
    if (errs != 0)      $fatal(1, "tb_pack: FAIL -- served words differ from the file");
    if (ecc_bad != 0)   $fatal(1, "tb_pack: FAIL -- a check word does not cancel its block");
    if (cyl != CYL || head != HEAD) $fatal(1, "tb_pack: FAIL -- the tags decoded as cyl %0d head %0d", cyl, head);
    $display("tb_pack: PASS -- header, label and data of c%0d h%0d s%0d served with cancelling Fire-code check words, tags decoded", CYL, HEAD, SECTOR);
    $finish;
  end
endmodule
