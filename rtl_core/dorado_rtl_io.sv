// =============================================================================
// dorado_rtl_io.sv — Xerox Dorado Slow & Fast I/O Controller Subsystem
// =============================================================================
// Derived from the schematic-audited C reference (dorado/src/io.c, fastio.c,
// display.c, disk.c, ethernet.c) and verilog/cells/dorado_trident.v,
// dorado_terminal.v.
//
// Implements:
//   1. Slow I/O Router by (task, tioa) with floating-bus default (0xFFFF,
//      bad_parity = 1).
//   2. Fast I/O Router (16-word munch IOFetch<- and IOStore<-).
//   3. Display Controller (DDC: Statics, DHTFlag/AHTFlag, DWTFlag/AWTFlag,
//      NLCB/TNLCB, HRam, TStatus, DDCStatus, RastTaskCmd, WCB line buffer,
//      scanline timer, and terminal keyboard/mouse serializer).
//   4. Ethernet Controller (TIOA 015 Data, TIOA 016 Control, TX/RX 1024-word
//      packet buffers, IOAtten' status marker, EOT/EIT wakeups).
//   5. Trident Disk Controller (Task 014, TIOA 010..014, 16-word Format RAM,
//      16-word Data FIFO, Drive/Head/Cylinder/Control tags, DMux 02000..02177,
//      and sector read/write sequencer).
// =============================================================================

/* verilator lint_off BLKSEQ */
module dorado_rtl_io
    import dorado_rtl_pkg::*;
#(
    // The Trident pack image, in beats (8 bytes) of the disk engine's memory
    // port: drive d's records start at DISK_PACK_BEAT_BASE + d * DISK_DRIVE_BEATS.
    parameter logic [28:0] DISK_PACK_BEAT_BASE = 29'd0,
    parameter logic [28:0] DISK_DRIVE_BEATS    = 29'h0080_0000      // 64 MB
)
(
    input  logic           clk,
    input  logic           rst,
    input  logic           clk_en,
    input  logic [63:0]    cycles,
    // The disk's rotation time base (sector/index pulses). The top drives
    // `cycles` here, or, in lock-step against the C emulator, the C
    // machine's own disk clock (bb.cycles), so the two disks spin alike.
    input  logic [63:0]    disk_time,

    // Slow I/O Bus Interface (to/from dorado_rtl_cpu)
    input  logic [3:0]     io_read_task,
    input  logic [1:0]     io_read_subtask,
    input  logic [7:0]     io_read_tioa,
    input  logic           io_read_stb,
    input  logic [3:0]     io_read_stb_task,
    input  logic [1:0]     io_read_stb_subtask,
    input  logic [7:0]     io_read_stb_tioa,
    output logic [15:0]    io_read_data,
    output logic           io_read_bad_parity,
    output logic           io_has_write,
    output logic           io_atten,

    input  logic           io_write_stb,
    input  logic [3:0]     io_write_task,
    input  logic [1:0]     io_write_subtask,
    input  logic [7:0]     io_write_tioa,
    input  logic [15:0]    io_write_data,

    // Fast I/O Munch Interface (to/from dorado_rtl_mem)
    input  logic           fastio_stb,
    input  dm_ref_kind_t   fastio_kind,
    input  logic [3:0]     fastio_task,
    input  logic [1:0]     fastio_subtask,
    input  logic [27:0]    fastio_va,
    input  logic [15:0]    fastio_out_munch [0:15],
    output logic [15:0]    fastio_in_munch  [0:15],

    // Disk DMux Status Interface (to/from dorado_rtl_mem)
    input  logic [11:0]    dmux_cur_addr,
    output logic           dmux_ext_handled,
    output logic [15:0]    dmux_ext_val,

    // External Task Wakeups & DWT Subtask Select (to dorado_rtl_cpu)
    output logic [15:0]    ext_wakeup_req,
    output logic           ext_subtask_we,
    output logic [3:0]     ext_subtask_task,
    output logic [1:0]     ext_subtask_val,

    // External Display / Terminal Controls
    input  logic           display_wake_gate, // 1 when world display list is active
    // Frame lines between vertical sync and the first visible line, dropped
    // from the framebuffer (the NLCB line count includes the top border,
    // which a monitor never shows). 0 for the Alto worlds; 22 measured for
    // Cedar's LF display (its first DCB line lands on frame line 22).
    input  logic [5:0]     display_top_lines,
    input  logic [15:0]    kbd_words_in [0:4],
    input  logic signed [15:0] mouse_dx_in,
    input  logic signed [15:0] mouse_dy_in,
    input  logic           mouse_delta_stb,

    // External Ethernet Packet Interface (for MiSTer UIO/DDR or C++ bridge)
    input  logic [7:0]     eth_local_host,
    output logic           eth_tx_packet_stb,
    output logic [10:0]    eth_tx_packet_words,

    // External Disk Sector Buffer Interface (for MiSTer HPS or C++ bridge)
    output logic [1:0]     disk_selected_drive_out,
    output logic [11:0]    disk_cur_cyl_out,
    output logic [5:0]     disk_cur_head_out,
    output logic [6:0]     disk_cur_sector_out,
    output logic           disk_sector_read_req,
    output logic           disk_sector_write_req,
    // The disk engine's memory port: single 64-bit beats, a request held until
    // its one-clock ack (the DDR storage engine's aux port protocol)
    output logic           disk_mem_req,
    output logic           disk_mem_we,
    output logic [28:0]    disk_mem_addr,
    output logic [63:0]    disk_mem_wdata,
    output logic [7:0]     disk_mem_be,
    input  logic           disk_mem_ack,
    input  logic [63:0]    disk_mem_rdata,

    // Video scan-out: registered read of the 1-bpp framebuffer on clk,
    // independent of clk_en (a second port, so the DDC write is unaffected).
    input  logic [19:0]    fb_rd_addr,      // y * 1024 + x
    output logic           fb_rd_data,

    // High while the I/O side cannot accept the next microinstruction's
    // strobes (the CPU waits in C_FETCH): the tick itself, the clock after it
    // (registered read data settles), a framebuffer job, a receive skip.
    output logic           io_busy,
    // ...and the same without the tick clock itself: the jobs (registered),
    // which the CPU's retire -- the next tick -- waits for
    output logic           io_jobs_busy,

    // Receive buffer fill (from the boot server): words, then rx_load with
    // the count restarts the controller's read position at 0.
    input  logic           rx_wr_en,
    input  logic [11:0]    rx_wr_addr,
    input  logic [15:0]    rx_wr_data,
    input  logic           rx_wr_att,
    input  logic           rx_load,
    input  logic [12:0]    rx_load_count,

    // Transmit buffer readout (the packet announced by eth_tx_packet_stb)
    input  logic [9:0]     eth_tx_rd_addr,
    output logic [15:0]    eth_tx_rd_data,

    // Receiver state for the boot server
    output logic           eth_rx_on_out,
    output logic           eth_rx_drained,
    output logic [14:0]    eth_rx_loads,    // receive fills applied (the mailbox's count, checked)

    // Loader assist (dorado_rtl_assist): set keyboard word 0 directly;
    // whether a display line is pending (feeds its display gate)
    input  logic           kbd0_poke,
    input  logic [15:0]    kbd0_val,
    output logic           next_wcb_any
);
    assign next_wcb_any = ddc_next_wcb_flag[0] || ddc_next_wcb_flag[1];
    assign eth_rx_on_out  = eth_rx_on;
    // A fill (rx_load) waits in rx_load_pend while the WaitForBOP walk has
    // the receive state, rather than being dropped; until it is applied the
    // receiver is not drained. eth_rx_loads counts the fills applied, so the
    // host can check every one it sent arrived (status beat 11).
    logic        rx_load_pend;
    logic [14:0] rx_applied;
    assign eth_rx_drained = !rx_load_pend && (eth_rx_pos >= eth_rx_count);
    assign eth_rx_loads   = rx_applied;
    // Framebuffer render jobs (main block -> engine)
    logic        rj_start;
    logic        rj_cursor;
    logic [63:0] ddc_cursor_rows;   // cursor rows posted to the render engine
    logic [63:0] ddc_cursor_wr;     // CursorHi writes (with or without bits)
    logic [15:0] ddc_cursor_dbg_x, ddc_cursor_dbg_y, ddc_cursor_dbg_ymin;   // the last cursor row drawn; the field's first
    logic        rj_ch;
    logic [9:0]  rj_y;
    logic [10:0] rj_width;
    logic [3:0]  rj_start_off;
    logic [7:0]  rj_wcount;
    logic [15:0] rj_cx;
    logic [15:0] rj_cbits;
    logic        r_busy;             // a framebuffer/copy job is queued or running
    logic        wc_start;           // munch copy job (main block -> engine)
    logic        wc_ch;
    logic [6:0]  wc_base;
    logic [4:0]  wc_n;
    logic disk_eng_hold;     // the disk engine is at work (see the sector data engine)
    assign io_busy = clk_en || rj_start || wc_start || r_busy || eth_skip_busy || disk_eng_hold;
    assign io_jobs_busy = rj_start || wc_start || r_busy || eth_skip_busy || disk_eng_hold;
    // r_busy: a framebuffer/copy job is queued or running (set below)
    // =========================================================================
    // 1. Constants & Device Address Definitions
    // =========================================================================
    // Display Tasks & TIOAs
    localparam logic [3:0] TASK_EMU = 4'o00;
    localparam logic [3:0] TASK_DHT = 4'o03;
    localparam logic [3:0] TASK_AHT = 4'o04;
    localparam logic [3:0] TASK_EOT = 4'o06;
    localparam logic [3:0] TASK_EIT = 4'o07;
    localparam logic [3:0] TASK_AWT = 4'o11;
    localparam logic [3:0] TASK_DWT = 4'o13;
    localparam logic [3:0] TASK_DSK = 4'o14;

    localparam logic [7:0] TIOA_DISKCONTROL = 8'o010;
    localparam logic [7:0] TIOA_DISKMUFF    = 8'o011;
    localparam logic [7:0] TIOA_DISKDATA    = 8'o012;
    localparam logic [7:0] TIOA_DISKRAM     = 8'o013;
    localparam logic [7:0] TIOA_DISKTAG     = 8'o014;

    localparam logic [7:0] TIOA_ETH_DATA    = 8'o015;
    localparam logic [7:0] TIOA_ETH_CTL     = 8'o016;

    localparam logic [7:0] TIOA_RAST_SELCMD  = 8'o320;
    localparam logic [7:0] TIOA_RAST_ADDRCMD = 8'o321;
    localparam logic [7:0] TIOA_RAST_DATACMD = 8'o322;
    localparam logic [7:0] TIOA_RAST_TASKCMD = 8'o323;

    localparam logic [7:0] TIOA_TSTATUS     = 8'o360;
    localparam logic [7:0] TIOA_AWTFLAG     = 8'o363;
    localparam logic [7:0] TIOA_AHTFLAG     = 8'o364;
    localparam logic [7:0] TIOA_TNLCB       = 8'o366;
    localparam logic [7:0] TIOA_TSTATICS    = 8'o367;
    localparam logic [7:0] TIOA_DDCSTATUS   = 8'o370;
    localparam logic [7:0] TIOA_DWTFLAG     = 8'o373;
    localparam logic [7:0] TIOA_DHTFLAG     = 8'o374;
    localparam logic [7:0] TIOA_HRAM        = 8'o375;
    localparam logic [7:0] TIOA_NLCB        = 8'o376;
    localparam logic [7:0] TIOA_STATICS     = 8'o377;

    localparam int DISPLAY_W         = 1024;
    localparam int DISPLAY_H         = 808;
    localparam int DISPLAY_ROW_BYTES = DISPLAY_W / 8; // 128 bytes/row
    localparam int DISPLAY_FB_BYTES  = DISPLAY_ROW_BYTES * DISPLAY_H; // 103,424 bytes

    // =========================================================================
    // 2. Display Controller (DDC) & Terminal State
    // =========================================================================
    // One bit per pixel, address y*1024 + x: the render engine only ever
    // writes single pixels, so the framebuffer is a simple two-port RAM (one
    // write port, the scan-out read port) with no read-modify-write.
    (* ramstyle = "M10K" *) logic fb [0:DISPLAY_W*DISPLAY_H-1];
    logic [19:0] fb_addr_a;
    logic        fb_we_a;
    logic        fb_wd_a;
    always_ff @(posedge clk) begin
        if (fb_we_a) fb[fb_addr_a] <= fb_wd_a;
    end
    // The cursor, mixed in at scan-out (as the display board mixes it into
    // the video): the 16 cursor bits of each screen line, from the CursorLo/
    // CursorHi outputs Cedar's display microcode ends every line with, and
    // the cursor's X. Drawn into the framebuffer instead, a cursor row was
    // overwritten by the same line's render, which is posted after it.
    (* ramstyle = "M10K" *) logic [15:0] cur_line [0:1023];
    logic        cl_we;
    logic [9:0]  cl_addr;
    logic [15:0] cl_wd;
    logic [15:0] cur_x;
    logic [15:0] cl_q;
    logic [9:0]  cl_px;
    logic        fb_q;
    always_ff @(posedge clk) begin
        if (cl_we) cur_line[cl_addr] <= cl_wd;
    end
    always_ff @(posedge clk) begin
        fb_q  <= fb[fb_rd_addr];
        cl_q  <= cur_line[fb_rd_addr[19:10]];
        cl_px <= fb_rd_addr[9:0];
    end
    always_comb begin
        logic [15:0] d;
        d = {6'd0, cl_px} - cur_x;           // the pixel's place in the cursor row
        fb_rd_data = fb_q || (d < 16'd16 && cl_q[4'd15 - d[3:0]]);
    end
    logic [15:0] ddc_statics;
    logic [15:0] ddc_riob;
    logic [3:0]  ddc_terminal_task;
    logic        ddc_ram_keep;
    logic [15:0] ddc_ram_addr;
    logic [7:0]  ddc_hram [0:511];

    logic [15:0] ddc_nlcb [0:15];
    logic [15:0] ddc_nlcb_line;
    logic        ddc_nlcb_field_odd;
    logic [15:0] ddc_nlcb_cursor_x;
    logic [7:0]  ddc_nlcb_cursor_lo;
    logic [31:0] ddc_scan_line;
    logic [31:0] ddc_scan_pixel;
    logic [63:0] ddc_frame_count;
    logic [63:0] ddc_pixels_rendered;
    logic [63:0] ddc_lines_rendered;

    logic        ddc_next_wcb_flag    [0:1];
    logic        ddc_current_wcb_flag [0:1];
    logic [15:0] ddc_wcb_draw_x       [0:1];
    logic [15:0] ddc_wcb_draw_y       [0:1];

    // Pending & Current Line Buffers (matching dorado_display_ddc_line)
    logic        ddc_pend_valid [0:1];
    logic        ddc_pend_odd   [0:1];
    logic [15:0] ddc_pend_line  [0:1];
    logic [15:0] ddc_pend_lmarg [0:1];
    logic [15:0] ddc_pend_width [0:1];
    logic [15:0] ddc_pend_ptr   [0:1];
    logic [15:0] ddc_pend_scan  [0:1];

    logic        ddc_cur_valid      [0:1];
    logic        ddc_cur_odd        [0:1];
    logic        ddc_cur_abs_y      [0:1];
    logic [15:0] ddc_cur_y          [0:1];
    logic [15:0] ddc_cur_line       [0:1];
    logic [15:0] ddc_cur_lmarg      [0:1];
    logic [15:0] ddc_cur_width      [0:1];
    logic [15:0] ddc_cur_ptr        [0:1];
    logic [15:0] ddc_cur_scan       [0:1];
    logic [15:0] ddc_cur_word_count [0:1];
    // Words gathered for the line each channel is drawing: block RAM
    // {channel, index}, filled by the munch-copy engine, read by the renderer.
    (* ramstyle = "M10K" *) logic [15:0] ddc_cur_words [0:255];

    // Display FIFOs (64 words each for channel A and B)
    // The raster FIFOs' contents are never read; only their head/tail
    // positions (which gate DWT wakeups) are modelled.
    int          ddc_fifo_a_head, ddc_fifo_a_tail;
    int          ddc_fifo_b_head, ddc_fifo_b_tail;

    // Monterey Raster Task state
    logic [15:0] ddc_raster_taskcmd;
    logic        ddc_raster_lt_enabled;
    logic [1:0]  ddc_raster_next_channel;
    logic        ddc_raster_next_wt_flag    [0:3];
    logic        ddc_raster_current_wt_flag [0:3];

    // Terminal Microcomputer Serializer State
    logic [31:0] ddc_boot_button_scanlines;
    logic [15:0] ddc_kbd_words      [0:4];
    logic [4:0]  ddc_kbd_pending;
    // the pending words in the order they changed (display.c display_kbd_order):
    // the terminal reports transitions as they happen, so a Shift pressed
    // before a letter reaches the guest first
    logic [2:0]  ddc_kbd_order [0:4];
    logic [2:0]  ddc_kbd_order_n;
    int          ddc_mouse_dx;
    int          ddc_mouse_dy;
    logic        ddc_mouse_pending;
    logic [7:0]  ddc_term_msg_bit;
    logic [7:0]  ddc_term_msg_word;
    logic [7:0]  ddc_term_msg_type;
    logic [15:0] ddc_term_msg_body;
    logic        ddc_term_msg_active;
    logic [63:0] ddc_term_bits;
    logic [63:0] ddc_term_messages;

    // Scanline wakeup cadence state (matching machine.c SCANLINE_CHECK_CYCLES)
    localparam int SCANLINE_PERIOD_CYCLES = 1058; // 16.67 MHz / (30 Hz * 525 lines)
    logic [15:0] ddc_scanline_cd;

    // =========================================================================
    // 3. Ethernet Controller State
    // =========================================================================
    logic        eth_tx_on;
    logic        eth_tx_eop;
    logic        eth_tx_cntdwn;
    logic        eth_rx_on;
    logic        eth_no_wakeups;
    logic [31:0] eth_rx_hold;
    logic [31:0] eth_rx_wire_timer;
    logic [15:0] eth_world_rx_words;

    (* ramstyle = "M10K" *) logic [15:0] eth_tx_words [0:1023];
    int          eth_tx_count;
    (* ramstyle = "M10K" *) logic [15:0] eth_rx_words     [0:4095];
    (* ramstyle = "M10K" *) logic        eth_rx_attention [0:4095];
`ifdef DORADO_CHECKPOINT
    initial begin
        $readmemh("init/eth_rx_words.hex", eth_rx_words);
        $readmemh("init/eth_rx_attention.hex", eth_rx_attention);
    end
`endif
    logic [15:0] eth_rx_q;      // eth_rx_words[eth_rx_pos], one clock late
    logic        eth_rx_att_q;
    logic        eth_skip_busy; // WaitForBOP skip in progress
    logic        eth_skip_phase;
    int          eth_skip_count;
    int          eth_rx_count;
    int          eth_rx_pos;

    logic [63:0] eth_tx_packets;
    logic [63:0] eth_rx_packets;
    logic [63:0] eth_data_reads;
    logic [63:0] eth_data_writes;

    // =========================================================================
    // 4. Trident Disk Controller State
    // =========================================================================
    localparam int DISK_FIFO_WORDS       = 16;
    // DiskControl per-block operations, as dorado/include/disk.h numbers them
    // (DORADO_DISK_OP_*). The C encoding is the one every disk-booted world
    // runs on; this RTL first had READ and WRITE the other way round.
    localparam logic [1:0] DISK_OP_DONE  = 2'd0;
    localparam logic [1:0] DISK_OP_WRITE = 2'd1;
    localparam logic [1:0] DISK_OP_RDCHK = 2'd2;
    localparam logic [1:0] DISK_OP_READ  = 2'd3;
    localparam int DISK_FORMAT_RAM_WORDS = 16;
    localparam int DISK_MAX_SECTOR_WORDS = 1048; // 2 hdr + 10 lbl + 1024 data + 12 trailer

    logic [1:0]  disk_selected_drive;
    logic        disk_drv_online         [0:3];
    logic        disk_drv_selected       [0:3];
    logic        disk_drv_read_only      [0:3];
    logic        disk_drv_head_ovfl      [0:3];
    logic [5:0]  disk_drv_heads          [0:3];
    logic [6:0]  disk_drv_sectors        [0:3];
    logic [6:0]  disk_drv_sectors_per_rev[0:3];
    logic [5:0]  disk_drv_subsector_cnt  [0:3];
    logic [11:0] disk_drv_cur_cyl        [0:3];
    logic [5:0]  disk_drv_cur_head       [0:3];
    logic [6:0]  disk_drv_cur_sector     [0:3];
    logic [7:0]  disk_drv_seek_in_prog   [0:3];
    logic        disk_drv_index_pulse    [0:3];
    logic        disk_drv_is_diablo      [0:3];
    logic [3:0]  disk_drv_hdr_words      [0:3];
    logic [4:0]  disk_drv_lbl_words      [0:3];
    logic [10:0] disk_drv_data_words     [0:3];

    logic [15:0] disk_control;
    logic [15:0] disk_tag;
    logic [7:0]  disk_muff_addr;
    logic [15:0] disk_format_ram [0:DISK_FORMAT_RAM_WORDS-1];
    logic [3:0]  disk_format_ram_addr;
    int          disk_format_ram_writes;

    logic        disk_enable_run;
    logic        disk_debug_mode;
    logic        disk_block_till_index;
    logic        disk_active;
    logic        disk_xfer_pending;
    logic        disk_restore_pending;
    logic        disk_transfer_restore;
    logic        disk_restore_hdr_check;

    logic        disk_index_tw;
    logic        disk_sector_tw;
    logic        disk_tag_tw;
    logic        disk_rd_fifo_tw;
    logic        disk_wr_fifo_tw;

    logic        disk_compare_err;
    logic        disk_read_data_err;
    logic        disk_fifo_underflow;
    logic        disk_fifo_overflow;

    logic [15:0] disk_fifo [0:DISK_FIFO_WORDS-1];
    logic [3:0]  disk_fifo_head;
    logic [3:0]  disk_fifo_tail;
    logic [4:0]  disk_fifo_count;

    logic        disk_read_stream_active;
    logic        disk_read_block_framing;
    logic [11:0] disk_read_stream_index;
    logic        disk_write_stream_active;
    logic        disk_write_block_framing;
    logic [11:0] disk_write_stream_index;

    logic [2:0]  disk_current_block;
    logic [1:0]  disk_current_block_op;
    logic [15:0] disk_current_block_words;
    logic [15:0] disk_current_block_pos;
    logic        disk_current_block_prefix;
    logic [3:0]  disk_current_block_trailer;

    // ---- The sector data engine ---------------------------------------------
    // A transfer's sector lives in block RAM, one 64-bit pack beat (four words,
    // word 4b+l in lane l) per entry, the record's words in pack order:
    // header | label | data. The pack image is a flat array of such records in
    // CHS order -- (cyl * heads + head) * sectors + (sector mod sectors), as
    // disk.c's dorado_disk_pack_sector -- ceil(words / 4) beats each, reached
    // through the beat port (disk_mem_*): DDR on the MiSTer, a DPI model in the
    // lock-step harness.
    //
    // The tick only asks for work (disk_job_*). The engine does it between
    // ticks, one step a clock, holding io_busy so the CPU waits in C_FETCH: a
    // load, a commit and a FIFO refill each complete between two
    // microinstructions, which is how atomic they are in disk.c. (The first
    // form did all three inside the tick, a 32-iteration loop reading the
    // sector arrays combinationally -- a behavioural model no FPGA could run.)
    localparam int DISK_SBUF_BEATS = 260;          // 2 + 10 + 1024 words, rounded up
    // four 16-bit lane RAMs (word 4b+l is lane l of beat b), one block RAM
    // each: a byte-enabled 64-bit RAM written lane by lane is not inferred
    // as RAM by Quartus 17 -- it became 16,640 registers and a 260:1 mux
    (* ramstyle = "M10K" *) logic [15:0] disk_sbuf0 [0:DISK_SBUF_BEATS-1];
    (* ramstyle = "M10K" *) logic [15:0] disk_sbuf1 [0:DISK_SBUF_BEATS-1];
    (* ramstyle = "M10K" *) logic [15:0] disk_sbuf2 [0:DISK_SBUF_BEATS-1];
    (* ramstyle = "M10K" *) logic [15:0] disk_sbuf3 [0:DISK_SBUF_BEATS-1];
    logic        sbuf_we;                           // these five are driven with <=
    logic [3:0]  sbuf_wlane;
    logic [8:0]  sbuf_waddr;
    logic [63:0] sbuf_wdata;
    logic [8:0]  sbuf_raddr;
    logic [63:0] sbuf_q;
    logic        disk_sec_valid;                    // the transfer's sector exists on the pack
    logic        disk_sec_modified;

    logic        disk_job_load, disk_job_commit, disk_job_refill;
    logic [1:0]  disk_stage;                        // 1, 2: the disk stage under way
    logic        dk_rd_pop, dk_wr, dk_fio;          // the tick's disk strobes, latched
    logic [7:0]  dk_wr_tioa;
    logic [15:0] dk_wr_data;
    dm_ref_kind_t dk_fio_kind;
    logic [15:0] dk_fio_munch [0:15];
    logic        disk_eng_busy;
    typedef enum logic [3:0] {
        DE_IDLE, DE_ADDR, DE_ADDR2, DE_ADDR3, DE_BEAT, DE_BEAT_WAIT, DE_CM_RD, DE_CM_RD2, DE_CM_WR,
        DE_CM_WAIT, DE_RF_HEAD, DE_RF_RD, DE_RF_RD2, DE_RF_POST
    } disk_eng_t;
    disk_eng_t   disk_eng;
    logic        disk_eng_commit;                   // the beat job under way is a commit
    logic [8:0]  disk_eng_beat, disk_eng_beats;
    logic [28:0] disk_eng_base;                     // the record's first beat
    logic [1:0]  disk_eng_drv;                      // the record's drive, and the address
    logic [17:0] disk_eng_ch;                       // arithmetic, a multiply a clock:
    logic [6:0]  disk_eng_ms;                       //   cyl * heads + head, sector mod sectors
    logic [24:0] disk_eng_rec;                      //   the record number
    logic [5:0]  disk_rf_iter;                      // refill loop iterations
    logic [1:0]  disk_rf_lane;
    logic        disk_rf_restore;                   // OR the restore bit into this word
    // the record a commit writes (latched at the write's end: a load may follow)
    logic [1:0]  disk_cm_drv;
    logic [11:0] disk_cm_cyl;
    logic [5:0]  disk_cm_head;
    logic [6:0]  disk_cm_sec;
    logic [11:0] disk_drv_cyls  [0:3];
    logic        disk_drv_media [0:3];

    logic [63:0] disk_next_sector_cycle;
    logic [31:0] disk_cycles_per_rev;
    // Microinstruction cycles per sector pulse, DORADO_DISK_CYCLES_PER_REV /
    // sectors per revolution: a table built at elaboration (a divider by a
    // register in the tick would be a long combinational path).
    localparam int DISK_CYCLES_PER_REV = 277778;
    logic [31:0] disk_cps_rom [0:127];
    initial for (int k = 0; k < 128; k++) disk_cps_rom[k] = (k == 0) ? 32'(DISK_CYCLES_PER_REV) : 32'(DISK_CYCLES_PER_REV / k);
    // Controller sector pulses per revolution for a Tag subsector count: one
    // every cnt + 1 of the drive's 117 subsector pulses, rounded up (disk.c
    // disk_set_subsector_count; TriconD's SectorCounters test)
    function automatic int disk_spr_fn(input logic [3:0] cnt);
        case (cnt)
            4'd0:  return 117;  4'd1:  return 59;  4'd2:  return 39;  4'd3:  return 30;
            4'd4:  return 24;   4'd5:  return 20;  4'd6:  return 17;  4'd7:  return 15;
            4'd8:  return 13;   4'd9:  return 12;  4'd10: return 11;  4'd11: return 10;
            4'd12: return 9;    4'd13: return 9;   4'd14: return 8;   4'd15: return 8;
        endcase
    endfunction

    // The sector a transfer works on, latched when it starts (disk.c's
    // read_stream_sector / write_stream_sector): a transfer's drain spans
    // many sector pulses, so it must keep the sector it began on, not the
    // drive's advancing cur_sector.
    logic [1:0]  disk_strm_drv;
    logic [11:0] disk_strm_cyl;
    logic [5:0]  disk_strm_head;
    logic [6:0]  disk_strm_sec;


    assign disk_selected_drive_out = disk_selected_drive;
    assign disk_cur_cyl_out        = disk_drv_cur_cyl[disk_selected_drive];
    assign disk_cur_head_out       = disk_drv_cur_head[disk_selected_drive];
    assign disk_cur_sector_out     = disk_drv_cur_sector[disk_selected_drive];

`ifndef DORADO_NO_DISK
    // the sector buffer: one write (a lane mask of a beat) and one registered
    // read a clock
    always_ff @(posedge clk) begin
        if (sbuf_we && sbuf_wlane[0]) disk_sbuf0[sbuf_waddr] <= sbuf_wdata[15:0];
        sbuf_q[15:0] <= disk_sbuf0[sbuf_raddr];
    end
    always_ff @(posedge clk) begin
        if (sbuf_we && sbuf_wlane[1]) disk_sbuf1[sbuf_waddr] <= sbuf_wdata[31:16];
        sbuf_q[31:16] <= disk_sbuf1[sbuf_raddr];
    end
    always_ff @(posedge clk) begin
        if (sbuf_we && sbuf_wlane[2]) disk_sbuf2[sbuf_waddr] <= sbuf_wdata[47:32];
        sbuf_q[47:32] <= disk_sbuf2[sbuf_raddr];
    end
    always_ff @(posedge clk) begin
        if (sbuf_we && sbuf_wlane[3]) disk_sbuf3[sbuf_waddr] <= sbuf_wdata[63:48];
        sbuf_q[63:48] <= disk_sbuf3[sbuf_raddr];
    end
    assign disk_eng_hold = disk_eng_busy || disk_stage != 2'd0;
`else
    assign disk_eng_hold = 1'b0;
    assign disk_mem_req = 1'b0;
    assign disk_mem_we = 1'b0;
    assign disk_mem_addr = 29'd0;
    assign disk_mem_wdata = 64'd0;
    assign disk_mem_be = 8'd0;
`endif

    // =========================================================================
    // 5. Helper Functions & Tasks
    // =========================================================================

    function automatic int ddc_decode_width_pixels_fn(input logic [15:0] raw);
        int pixels;
        pixels = int'((32'd0 - {20'd0, raw[11:0]}) & 32'h0FFF) - 255;
        if (pixels < 0)         pixels = 0;
        if (pixels > DISPLAY_W) pixels = DISPLAY_W;
        return pixels;
    endfunction

    task automatic ddc_snapshot_line_task(input int ch);
        int base;
        int c;
        c    = ch & 1;
        base = c ? 8 : 0;
        ddc_pend_valid[c] = 1'b1;
        ddc_pend_odd[c]   = ddc_nlcb_field_odd;
        ddc_pend_line[c]  = ddc_nlcb_line;
        ddc_pend_lmarg[c] = ddc_nlcb[base + 1] & 16'h0FFF;
        ddc_pend_width[c] = ddc_nlcb[base + 2] & 16'h0FFF;
        ddc_pend_ptr[c]   = ddc_nlcb[base + 3] & 16'h0FFF;
        ddc_pend_scan[c]  = ddc_nlcb[base + 4] & 16'h0FFF;
    endtask

    task automatic ddc_start_line_task(input int ch, input int y);
        int c;
        c = ch & 1;
        ddc_cur_valid[c]      = ddc_pend_valid[c];
        ddc_cur_odd[c]        = ddc_pend_odd[c];
        ddc_cur_abs_y[c]      = 1'b1;
        ddc_cur_y[c]          = 16'(y);
        ddc_cur_line[c]       = ddc_pend_line[c];
        ddc_cur_lmarg[c]      = ddc_pend_lmarg[c];
        ddc_cur_width[c]      = ddc_pend_width[c];
        ddc_cur_ptr[c]        = ddc_pend_ptr[c];
        ddc_cur_scan[c]       = ddc_pend_scan[c];
        ddc_cur_word_count[c] = 16'd0;
    endtask

    function automatic int ddc_fifo_free_fn(input int subtask);
        int head, tail, used;
        if (subtask == 0) begin
            head = ddc_fifo_a_head;
            tail = ddc_fifo_a_tail;
        end else begin
            head = ddc_fifo_b_head;
            tail = ddc_fifo_b_tail;
        end
        used = head - tail;
        if (used < 0) used += 64;
        return 63 - used;
    endfunction

    function automatic int ddc_fifo_used_fn(input int subtask);
        return 63 - ddc_fifo_free_fn(subtask);
    endfunction

    // Compute current terminal bit WITHOUT advancing (for combinational Pd<-Input)
    function automatic logic [15:0] ddc_peek_terminal_bit_fn(
        input logic [3:0] rd_task
    );
        logic [7:0]  m_type;
        logic [15:0] m_body;
        logic [31:0] msg;
        logic        b_val;

        if (rd_task != TASK_DHT && rd_task != TASK_AHT) begin
            return 16'h0000; // No DispM board
        end
        if (ddc_boot_button_scanlines > 32'd0) begin
            return 16'h8000;
        end

        if (ddc_term_msg_bit == 8'd0) begin
            if (ddc_kbd_pending != 5'd0) begin
                int w;
                w = int'(ddc_kbd_order[0]);         // the oldest transition
                m_type = (w < 4) ? 8'(w + 1) : 8'd5;
                m_body = ddc_kbd_words[w];
            end else if (ddc_mouse_pending) begin
                int dx, dy;
                dx = ddc_mouse_dx;
                dy = ddc_mouse_dy;
                if (dx > 127)       dx = 127;
                else if (dx < -128) dx = -128;
                if (dy > 127)       dy = 127;
                else if (dy < -128) dy = -128;
                m_type = 8'd6;
                m_body = {8'(dx + 128), 8'(dy + 128)};
            end else begin
                int w;
                w      = int'(ddc_term_msg_word) % 5;
                m_type = (w < 4) ? 8'(w + 1) : 8'd5;
                m_body = ddc_kbd_words[w];
            end
        end else begin
            m_type = ddc_term_msg_type;
            m_body = ddc_term_msg_body;
        end

        msg   = (32'd1 << 31) | ({24'd0, m_type} << 24) | ({16'd0, m_body} << 8) | (32'd1 << 7);
        b_val = msg[31 - (ddc_term_msg_bit & 8'd31)];
        return b_val ? 16'h8000 : 16'h0000;
    endfunction

    // Advance terminal serializer state on Pd<-Input to TSTATUS
    task automatic ddc_advance_terminal_bit_task();
        if (ddc_boot_button_scanlines > 32'd0) begin
            ddc_boot_button_scanlines = ddc_boot_button_scanlines - 32'd1;
        end else begin
        if (ddc_term_msg_bit == 8'd0) begin
            ddc_term_msg_active = 1'b1;
            if (ddc_kbd_pending != 5'd0) begin
                int w;
                w = int'(ddc_kbd_order[0]);         // the oldest transition
                for (int k = 0; k < 4; k++) ddc_kbd_order[k] = ddc_kbd_order[k + 1];
                ddc_kbd_order_n = ddc_kbd_order_n - 3'd1;
                ddc_kbd_pending[w] = 1'b0;
                ddc_term_msg_word  = 8'(w);
                ddc_term_msg_type  = (w < 4) ? 8'(w + 1) : 8'd5;
                ddc_term_msg_body  = ddc_kbd_words[w];
            end else if (ddc_mouse_pending) begin
                int dx, dy;
                dx = ddc_mouse_dx;
                dy = ddc_mouse_dy;
                if (dx > 127)       dx = 127;
                else if (dx < -128) dx = -128;
                if (dy > 127)       dy = 127;
                else if (dy < -128) dy = -128;
                ddc_mouse_dx = ddc_mouse_dx - dx;
                ddc_mouse_dy = ddc_mouse_dy - dy;
                if (ddc_mouse_dx == 0 && ddc_mouse_dy == 0) begin
                    ddc_mouse_pending = 1'b0;
                end
                ddc_term_msg_type = 8'd6;
                ddc_term_msg_body = {8'(dx + 128), 8'(dy + 128)};
            end else begin
                int w;
                w                 = int'(ddc_term_msg_word) % 5;
                ddc_term_msg_type = (w < 4) ? 8'(w + 1) : 8'd5;
                ddc_term_msg_body = ddc_kbd_words[w];
                ddc_term_msg_word = 8'((w + 1) % 5);
            end
        end

        ddc_term_bits    = ddc_term_bits + 64'd1;
        ddc_term_msg_bit = ddc_term_msg_bit + 8'd1;
        if (ddc_term_msg_bit >= 8'd32) begin
            ddc_term_msg_bit    = 8'd0;
            ddc_term_msg_active = 1'b0;
            ddc_term_messages   = ddc_term_messages + 64'd1;
        end
        end
    endtask

    // =========================================================================
    // 6. Disk Controller Helper Functions & Tasks
    // =========================================================================
`ifndef DORADO_NO_DISK
    function automatic logic disk_muff_word_bit_fn(
        input logic [7:0]  addr,
        input logic [7:0]  base,
        input logic [15:0] word
    );
        int bit_idx;
        bit_idx = int'(addr) - int'(base);
        if (bit_idx < 0 || bit_idx >= 16) return 1'b0;
        return word[15 - bit_idx];
    endfunction

    function automatic logic disk_muffler_bit_fn(input logic [7:0] addr);
        logic [1:0] drv;
        drv = disk_selected_drive;

        if (addr >= 8'o040 && addr < 8'o060) begin
            logic [15:0] word;
            word = {disk_format_ram_addr, disk_format_ram[disk_format_ram_addr][11:0]};
            return disk_muff_word_bit_fn(addr, 8'o040, word);
        end
        if (addr >= 8'o060 && addr < 8'o100) begin
            return disk_muff_word_bit_fn(addr, 8'o060, disk_tag);
        end
        if (addr >= 8'o100 && addr < 8'o120) begin
            int read_addr;
            logic [15:0] word;
            read_addr = (disk_fifo_tail + (disk_fifo_count > 0 ? 1 : 0)) & 15;
            word      = {4'd0, (disk_fifo_count > 0) ? 4'o7 : 4'o2,
                         4'(disk_fifo_head & 15), 4'(read_addr & 15)};
            return disk_muff_word_bit_fn(addr, 8'o100, word);
        end

        case (addr)
            8'o001: return disk_index_tw;
            8'o002: return disk_sector_tw;
            8'o003: return disk_tag_tw;
            8'o004: return disk_rd_fifo_tw;
            8'o005: return disk_enable_run && (disk_active || disk_write_stream_active) &&
                           (disk_fifo_count < (DISK_FIFO_WORDS - 2));
            8'o010: return disk_enable_run;
            8'o011: return disk_debug_mode;
            8'o012: return !(disk_active && disk_current_block_op == DISK_OP_READ);  // !Read
            8'o013: return !(disk_active && disk_current_block_op == DISK_OP_WRITE); // !Write
            8'o014: return !(disk_active && disk_current_block_op == 2'd2); // !RdChk
            8'o015: return disk_active;
            8'o016: return drv[1];
            8'o017: return drv[0];
            8'o021: return disk_drv_head_ovfl[drv];
            8'o023: return !disk_drv_selected[drv] || !disk_drv_online[drv];
            8'o024: return !disk_drv_selected[drv] || !disk_drv_online[drv];
            8'o025: return !disk_drv_selected[drv] || (disk_drv_seek_in_prog[drv] > 0) || !disk_drv_online[drv];
            8'o027: return disk_fifo_underflow;
            8'o030: return disk_fifo_overflow;
            8'o031: return disk_read_data_err || disk_compare_err;
            8'o032: return disk_drv_read_only[drv];
            8'o036: return !disk_drv_selected[drv] || !disk_drv_online[drv] ||
                           (disk_drv_seek_in_prog[drv] > 0) || disk_drv_head_ovfl[drv] ||
                           disk_drv_read_only[drv] || disk_fifo_underflow ||
                           disk_fifo_overflow || disk_read_data_err || disk_compare_err;
            8'o037: return !disk_drv_selected[drv] || !disk_drv_online[drv] ||
                           (disk_drv_seek_in_prog[drv] > 0) || disk_drv_head_ovfl[drv] ||
                           disk_fifo_underflow || disk_fifo_overflow ||
                           disk_read_data_err || disk_compare_err;
            default: return 1'b0;
        endcase
    endfunction

    function automatic logic [1:0] disk_control_block_op_fn(
        input logic [15:0] ctrl,
        input int          blk
    );
        case (blk)
            0: return ctrl[7:6];
            1: return ctrl[5:4];
            2: return ctrl[3:2];
            3: return ctrl[1:0];
            default: return 2'd0;
        endcase
    endfunction

    function automatic int disk_block_default_words_fn(
        input logic [1:0] drv,
        input int         blk
    );
        case (blk)
            0: return int'(disk_drv_hdr_words[drv]);
            1: return int'(disk_drv_lbl_words[drv]);
            2: return int'(disk_drv_data_words[drv]);
            default: return 0;
        endcase
    endfunction

    function automatic logic [15:0] disk_format_block_words_fn(
        input logic [1:0] drv,
        input int         blk
    );
        logic [15:0] n;
        if (blk < 0 || blk >= 4) return 16'd0;
        n = disk_format_ram[blk] + 16'd1;
        if (disk_format_ram_writes == 0 && n == 16'd1) begin
            n = 16'(disk_block_default_words_fn(drv, blk));
        end
        return n;
    endfunction

    // The record word (header | label | data) a framed block position reads;
    // -1 past the block's geometry, which reads as 0.
    function automatic int disk_block_addr_fn(
        input logic [1:0] drv,
        input int         blk,
        input int         pos
    );
        int hw, lw, dw;
        hw = int'(disk_drv_hdr_words[drv]);
        lw = int'(disk_drv_lbl_words[drv]);
        dw = int'(disk_drv_data_words[drv]);
        if (pos < 0) return -1;
        case (blk)
            0: return (pos < hw) ? pos : -1;
            1: return (pos < lw) ? hw + pos : -1;
            2: return (pos < dw) ? hw + lw + pos : -1;
            default: return -1;
        endcase
    endfunction

    function automatic int disk_rec_words_fn(input logic [1:0] drv);
        return int'(disk_drv_hdr_words[drv]) + int'(disk_drv_lbl_words[drv]) + int'(disk_drv_data_words[drv]);
    endfunction

    task automatic disk_update_fifo_tws_task();
        if (disk_read_stream_active) begin
            int threshold;
            threshold = 3;
            if (disk_current_block_op == 2'd2 || disk_current_block_op == 2'd0) begin
                threshold = 1;
            end
            disk_rd_fifo_tw = (disk_fifo_count >= threshold);
        end else begin
            disk_rd_fifo_tw = 1'b0;
        end
        if (disk_write_stream_active) begin
            disk_wr_fifo_tw = ((DISK_FIFO_WORDS - disk_fifo_count) >= 4);
        end else begin
            disk_wr_fifo_tw = 1'b0;
        end
    endtask

    task automatic disk_arm_write_block_task(
        input logic [1:0] drv,
        input int         blk
    );
        int start_idx;
        start_idx = 0;
        for (int b = 0; b < 3; b++) begin
            if (b < blk) start_idx += disk_block_default_words_fn(drv, b);
        end
        disk_read_stream_active    = 1'b0;
        disk_rd_fifo_tw            = 1'b0;
        disk_write_stream_active   = 1'b1;
        disk_write_block_framing   = 1'b1;
        disk_write_stream_index    = start_idx;
        disk_current_block         = 3'(blk);
        disk_current_block_op      = DISK_OP_WRITE;
        disk_current_block_words   = disk_format_block_words_fn(drv, blk);
        disk_current_block_pos     = 16'd0;
        disk_current_block_prefix  = 1'b1;
        disk_current_block_trailer = 4'd0;
        disk_active                = 1'b1;
        disk_update_fifo_tws_task();
    endtask

    task automatic disk_start_current_block_task(
        input  logic [1:0] drv,
        output logic       started
    );
        // the first block from current_block on whose op is not DONE (at most
        // four: a bounded loop, for synthesis)
        started = 1'b0;
        for (int k = 0; k < 4; k++) begin
            if (!started && disk_current_block < 3'd4) begin
                logic [1:0] op;
                op = disk_control_block_op_fn(disk_control, int'(disk_current_block));
                if (op == DISK_OP_WRITE) begin
                    disk_arm_write_block_task(drv, int'(disk_current_block));
                    started = 1'b1;
                end else if (op == DISK_OP_READ || op == DISK_OP_RDCHK) begin
                    disk_write_stream_active   = 1'b0;
                    disk_read_stream_active    = 1'b1;
                    disk_current_block_op      = op;
                    disk_current_block_words   = disk_format_block_words_fn(drv, int'(disk_current_block));
                    disk_current_block_pos     = 16'd0;
                    disk_current_block_prefix  = 1'b0;
                    disk_current_block_trailer = 4'd0;
                    if (op == DISK_OP_RDCHK) disk_compare_err = 1'b1;
                    disk_update_fifo_tws_task();
                    started = 1'b1;
                end else begin
                    disk_current_block = disk_current_block + 3'd1;
                end
            end
        end
        if (!started) begin
            disk_read_stream_active    = 1'b0;
            disk_write_stream_active   = 1'b0;
            disk_current_block_op      = 2'd0;
            disk_current_block_words   = 16'd0;
            disk_current_block_pos     = 16'd0;
            disk_current_block_prefix  = 1'b0;
            disk_current_block_trailer = 4'd0;
        end
    endtask

    // Start of a transfer: latch its sector and have the engine load it.
    // disk.c refuses a sector the pack does not have (dorado_disk_pack_sector
    // is NULL out of range): the same bounds here.
    task automatic disk_load_sector_task();
        logic [1:0] drv;
        drv = disk_selected_drive;
        disk_strm_drv  = drv;
        disk_strm_cyl  = disk_drv_cur_cyl[drv];
        disk_strm_head = disk_drv_cur_head[drv];
        disk_strm_sec  = disk_drv_cur_sector[drv];
        disk_sec_modified = 1'b0;
        disk_sec_valid = disk_drv_online[drv] && disk_drv_media[drv] && disk_drv_sectors[drv] != 7'd0 &&
                         disk_drv_cur_cyl[drv] < disk_drv_cyls[drv] &&
                         disk_drv_cur_head[drv] < disk_drv_heads[drv];
        if (disk_sec_valid) disk_job_load = 1'b1;
    endtask

    // End of a write: have the engine write the buffer back to its sector.
    task automatic disk_commit_sector_task();
        disk_cm_drv  = disk_strm_drv;
        disk_cm_cyl  = disk_strm_cyl;
        disk_cm_head = disk_strm_head;
        disk_cm_sec  = disk_strm_sec;
        if (disk_sec_valid) disk_job_commit = 1'b1;
        disk_sec_modified = 1'b0;
    endtask

    // The FIFO refill: done by the engine, before the next microinstruction.
    task automatic disk_refill_fifo_task();
        if (disk_read_stream_active) disk_job_refill = 1'b1;
    endtask

    // A word of the sector into the FIFO (refill).
    task automatic disk_push_word_task(input logic [15:0] w);
        disk_fifo[disk_fifo_head] = w;
        disk_fifo_head            = (disk_fifo_head + 1) % DISK_FIFO_WORDS;
        disk_fifo_count           = disk_fifo_count + 1;
        disk_read_stream_index    = disk_read_stream_index + 1;
    endtask


    // ---- The disk stage: the disk's share of an I/O tick, in the clocks after
    // it (stage 1 the strobes the tick latched, stage 2 the rotation timer and
    // the DSK wakeup); then the sector engine if they queued work. io_busy
    // holds the CPU throughout, so to the microcode it is all one tick.
    task automatic disk_stage1_task();
        if (dk_rd_pop) begin
                    if (disk_fifo_count > 0) begin
                        disk_fifo_tail  = (disk_fifo_tail + 1) % DISK_FIFO_WORDS;
                        disk_fifo_count = disk_fifo_count - 1;
                        disk_refill_fifo_task();
                        disk_update_fifo_tws_task();
                    end else begin
                        disk_fifo_underflow = 1'b1;
                        disk_update_fifo_tws_task();
                    end
                end
        if (dk_wr) begin
                    case (dk_wr_tioa)
                        TIOA_DISKCONTROL: begin
                            if (disk_active) begin
                                disk_active                = 1'b0;
                                disk_read_stream_active    = 1'b0;
                                disk_read_stream_index     = 0;
                                disk_current_block         = 3'd0;
                                disk_current_block_op      = 2'd0;
                                disk_current_block_words   = 16'd0;
                                disk_current_block_pos     = 16'd0;
                                disk_current_block_prefix  = 1'b0;
                                disk_current_block_trailer = 4'd0;
                                disk_write_stream_active   = 1'b0;
                                disk_write_block_framing   = 1'b0;
                                disk_write_stream_index    = 0;
                                disk_fifo_head             = 0;
                                disk_fifo_tail             = 0;
                                disk_fifo_count            = 0;
                                disk_update_fifo_tws_task();
                            end else begin
                                disk_control = dk_wr_data;
                                if (dk_wr_data[7:0] != 8'd0) begin
                                    disk_xfer_pending     = 1'b1;
                                    disk_transfer_restore = disk_restore_pending;
                                    disk_restore_pending  = 1'b0;
                                end
                                disk_format_ram_addr = 4'd0;
                                if (dk_wr_data[10]) begin // CLR_ENABLE_RUN (0x0400)
                                    disk_enable_run       = 1'b0;
                                    disk_debug_mode       = 1'b0;
                                    disk_block_till_index = 1'b0;
                                    disk_index_tw         = 1'b0;
                                    disk_sector_tw        = 1'b0;
                                    disk_tag_tw           = 1'b0;
                                    disk_rd_fifo_tw       = 1'b0;
                                    disk_wr_fifo_tw       = 1'b0;
                                    disk_current_block_op = 2'd0;
                                end
                                if (dk_wr_data[9]) disk_debug_mode       = 1'b1; // SET_DEBUG_MODE (0x0200)
                                if (dk_wr_data[8]) disk_block_till_index = 1'b1; // BLOCK_TILL_INDEX (0x0100)
                                if (dk_wr_data[9] && (dk_wr_data[7:0] != 8'd0)) begin
                                    disk_current_block    = 3'd0;
                                    disk_current_block_op = (dk_wr_data[7:6] != 2'd0) ? dk_wr_data[7:6] :
                                                            (dk_wr_data[5:4] != 2'd0) ? dk_wr_data[5:4] :
                                                            (dk_wr_data[3:2] != 2'd0) ? dk_wr_data[3:2] :
                                                                                           dk_wr_data[1:0];
                                    disk_current_block_pos     = 16'd0;
                                    disk_current_block_prefix  = 1'b0;
                                    disk_current_block_trailer = 4'd0;
                                    disk_active                = 1'b1;
                                    disk_xfer_pending          = 1'b0;
                                    disk_update_fifo_tws_task();
                                end else begin
                                    disk_start_pending_transfer_task();
                                end
                            end
                        end

                        TIOA_DISKMUFF: begin
                            disk_muff_addr = dk_wr_data[7:0];
                            if (dk_wr_data[11]) disk_index_tw    = 1'b0; // CLEAR_INDEX_TW (0x0800)
                            if (dk_wr_data[10]) disk_sector_tw   = 1'b0; // CLEAR_SECTOR_TW (0x0400)
                            if (dk_wr_data[9])  disk_tag_tw      = 1'b0; // CLEAR_SEEKTAG_TW (0x0200)
                            if (dk_wr_data[13]) disk_compare_err = 1'b0; // CLEAR_COMPARE_ERR (0x2000)
                            if (dk_wr_data[12]) disk_read_data_err = 1'b1; // SET_CHECKSUM_ERR (0x1000)
                            if (dk_wr_data[8]) begin // CLEAR_ERRORS (0x0100)
                                disk_compare_err    = 1'b0;
                                disk_read_data_err  = 1'b0;
                                disk_fifo_underflow = 1'b0;
                                disk_fifo_overflow  = 1'b0;
                            end
                        end

                        TIOA_DISKDATA: begin
                            if (disk_write_stream_active) begin
                                disk_write_stream_word_task(dk_wr_data);
                                disk_update_fifo_tws_task();
                            end else if (disk_fifo_count < DISK_FIFO_WORDS) begin
                                disk_fifo[disk_fifo_head] = dk_wr_data;
                                disk_fifo_head            = (disk_fifo_head + 1) % DISK_FIFO_WORDS;
                                disk_fifo_count           = disk_fifo_count + 1;
                                disk_update_fifo_tws_task();
                            end else begin
                                disk_fifo_overflow = 1'b1;
                            end
                        end

                        TIOA_DISKRAM: begin
                            disk_format_ram[disk_format_ram_addr] = dk_wr_data;
                            disk_format_ram_writes                = disk_format_ram_writes + 1;
                            if (disk_format_ram_addr == 4'd15) begin
                                disk_enable_run = 1'b1;
                            end
                            disk_format_ram_addr = disk_format_ram_addr + 4'd1;
                        end

                        TIOA_DISKTAG: begin
                            logic [11:0] bus;
                            logic [1:0]  drv;
                            disk_tag = dk_wr_data;
                            bus      = dk_wr_data[11:0];
                            drv      = disk_selected_drive;

                            if (dk_wr_data[15]) begin // TAG_DRIVE (0x8000)
                                if (bus[5]) begin
                                    int cnt, spr;
                                    cnt = int'(bus[9:6]);
                                    disk_drv_subsector_cnt[drv] = 6'(cnt);
                                    spr = disk_spr_fn(bus[9:6]);   // ceil(117 / (cnt + 1))
                                    disk_drv_sectors_per_rev[drv] = 7'(spr);
                                    if (disk_drv_cur_sector[drv] >= 7'(spr)) begin
                                        disk_drv_cur_sector[drv] = 7'd0;
                                    end
                                end
                                if (bus[4] && bus[3:0] <= 4'd3) begin
                                    disk_selected_drive = bus[1:0];
                                    for (int i = 0; i < 4; i++) begin
                                        disk_drv_selected[i] = (i == int'(bus[1:0]));
                                    end
                                end
                            end else if (dk_wr_data[13]) begin // TAG_HEAD (0x2000)
                                logic [5:0] hd;
                                hd = bus[5:0];
                                if (disk_drv_heads[drv] > 6'd0 && hd < disk_drv_heads[drv]) begin
                                    disk_drv_cur_head[drv]  = hd;
                                    disk_drv_head_ovfl[drv] = 1'b0;
                                    disk_sector_read_req    = 1'b1;
                                end else if (disk_drv_online[drv]) begin
                                    disk_drv_cur_head[drv]  = hd;
                                    disk_drv_head_ovfl[drv] = 1'b1;
                                end
                                disk_tag_tw = 1'b1;
                            end else if (dk_wr_data[14]) begin // TAG_CYLINDER (0x4000)
                                if (disk_drv_online[drv]) begin
                                    disk_drv_cur_cyl[drv]      = bus;
                                    disk_drv_cur_sector[drv]   = 7'd0;
                                    disk_drv_seek_in_prog[drv] = int'(disk_drv_sectors_per_rev[drv]);
                                    disk_sector_read_req       = 1'b1;
                                end
                                disk_tag_tw = 1'b1;
                            end else if (dk_wr_data[12]) begin // TAG_CONTROL (0x1000)
                                if (bus[3]) begin // DiskReset
                                    disk_drv_head_ovfl[drv] = 1'b0;
                                end
                                if (bus[1]) begin // ReZero
                                    disk_drv_cur_cyl[drv]      = 12'd0;
                                    disk_drv_cur_head[drv]     = 6'd0;
                                    disk_drv_cur_sector[drv]   = 7'd0;
                                    disk_drv_head_ovfl[drv]    = 1'b0;
                                    disk_drv_seek_in_prog[drv] = int'(disk_drv_sectors_per_rev[drv]);
                                    disk_restore_pending       = 1'b1;
                                    disk_sector_read_req       = 1'b1;
                                end
                                if (bus[0]) begin // HeadAdvance
                                    disk_drv_cur_head[drv] = disk_drv_cur_head[drv] + 6'd1;
                                    disk_sector_read_req   = 1'b1;
                                end
                                if (bus[6]) begin // Read
                                    logic ok;
                                    disk_read_block_framing = 1'b0;
                                    disk_begin_read_stream_task(ok);
                                    if (ok) disk_active = 1'b1;
                                end
                                if (bus[7]) begin // Write
                                    if (disk_drv_online[drv] && !disk_drv_read_only[drv]) begin
                                        disk_load_sector_task();
                                        disk_write_stream_active   = 1'b1;
                                        disk_write_block_framing   = 1'b0;
                                        disk_write_stream_index    = 0;
                                        disk_current_block         = 3'd0;
                                        disk_current_block_op      = DISK_OP_WRITE;
                                        disk_current_block_words   = 16'(int'(disk_drv_hdr_words[drv]) +
                                                                         int'(disk_drv_lbl_words[drv]) +
                                                                         int'(disk_drv_data_words[drv]));
                                        disk_current_block_pos     = 16'd0;
                                        disk_current_block_prefix  = 1'b0;
                                        disk_current_block_trailer = 4'd0;
                                        disk_fifo_count            = 0;
                                        disk_fifo_head             = 0;
                                        disk_fifo_tail             = 0;
                                    end
                                    disk_active = 1'b1;
                                    disk_update_fifo_tws_task();
                                end
                                disk_tag_tw = 1'b1;
                            end
                        end
                        default: ;
                    endcase
                end
        if (dk_fio) begin
                    if (dk_fio_kind == DM_REF_IOFETCH) begin
                // the munch into the FIFO, as many words as it has room for
                logic [4:0] n;
                n = 5'(DISK_FIFO_WORDS) - disk_fifo_count;
                for (int i = 0; i < 16; i++)
                    if (5'(i) < n) disk_fifo[4'(disk_fifo_head + 4'(i))] = dk_fio_munch[i];
                disk_fifo_head  = disk_fifo_head + 4'(n);
                disk_fifo_count = disk_fifo_count + n;
                disk_update_fifo_tws_task();
            end else if (dk_fio_kind == DM_REF_IOSTORE) begin
                // the munch out of the FIFO (the engine refills it after)
                if (disk_fifo_count > 5'd0) begin
                    logic [4:0] n;
                    n = (disk_fifo_count > 5'd16) ? 5'd16 : disk_fifo_count;
                    disk_fifo_tail  = disk_fifo_tail + 4'(n);
                    disk_fifo_count = disk_fifo_count - n;
                    disk_refill_fifo_task();
                end
                disk_update_fifo_tws_task();
            end
                end
    endtask

    task automatic disk_stage2_task();
        begin
                logic [1:0] drv;
                drv = disk_selected_drive;
                if (disk_drv_online[drv] && disk_enable_run) begin
                    int spr;
                    logic [63:0] cps;
                    spr = int'(disk_drv_sectors_per_rev[drv]);
                    if (spr <= 0) spr = 1;
                    cps = {32'd0, disk_cps_rom[disk_drv_sectors_per_rev[drv]]};

                    if (disk_next_sector_cycle == 64'd0) begin
                        disk_next_sector_cycle = disk_time + cps;
                    end else if (disk_time >= disk_next_sector_cycle) begin
                        logic at_index;
                        // (cur + 1) mod spr: cur < spr (the Tag write that sets spr resets it)
                        disk_drv_cur_sector[drv] = (int'(disk_drv_cur_sector[drv]) + 1 >= spr) ? 7'd0
                                                   : disk_drv_cur_sector[drv] + 7'd1;
                        at_index                 = (disk_drv_cur_sector[drv] == 7'd0);
                        disk_sector_read_req     = 1'b1;

                        if (disk_drv_seek_in_prog[drv] > 0) begin
                            disk_drv_seek_in_prog[drv] = disk_drv_seek_in_prog[drv] - 1;
                            if (at_index) disk_drv_seek_in_prog[drv] = 0;
                        end
                        if (at_index) begin
                            disk_drv_index_pulse[drv] = 1'b1;
                            disk_index_tw             = 1'b1;
                            disk_sector_tw            = 1'b1;
                            disk_block_till_index     = 1'b0;
                            disk_start_pending_transfer_task();
                        end else begin
                            disk_drv_index_pulse[drv] = 1'b0;
                            if (!disk_block_till_index) begin
                                disk_sector_tw = 1'b1;
                                disk_start_pending_transfer_task();
                            end
                        end
                        disk_next_sector_cycle = disk_next_sector_cycle + cps;
                    end
                end
                if (disk_index_tw || disk_sector_tw || disk_tag_tw ||
                    disk_rd_fifo_tw || disk_wr_fifo_tw) begin
                    ext_wakeup_req[TASK_DSK] = 1'b1;
                end
            end
    endtask

    // One clock of the engine (the main block runs it between ticks).
    task automatic disk_engine_step_task();
        logic [1:0] drv;
        int         hw, lw, dw, total;
        logic       sequenced;
        drv       = disk_selected_drive;
        hw        = int'(disk_drv_hdr_words[drv]);
        lw        = int'(disk_drv_lbl_words[drv]);
        dw        = int'(disk_drv_data_words[drv]);
        total     = hw + lw + dw;
        if (disk_read_block_framing) total += 12;  // 3 blocks x 4 trailer words
        sequenced = disk_read_block_framing;
        case (disk_eng)
        DE_IDLE: begin
            // a commit first (it writes the buffer a load would overwrite),
            // then a load, then the refill that reads it
            if (disk_job_commit) begin
                disk_job_commit = 1'b0;
                disk_eng_commit = 1'b1;
                disk_eng        = DE_ADDR;
            end else if (disk_job_load) begin
                disk_job_load   = 1'b0;
                disk_eng_commit = 1'b0;
                disk_eng        = DE_ADDR;
            end else if (disk_job_refill) begin
                disk_job_refill = 1'b0;
                if (disk_read_stream_active) begin
                    if (!disk_drv_online[drv] || !disk_sec_valid) begin
                        disk_read_stream_active = 1'b0;
                    end else begin
                        disk_rf_iter = 6'd0;
                        disk_eng     = DE_RF_HEAD;
                    end
                end
            end else begin
                disk_eng_busy = 1'b0;
            end
        end
        // the record's first beat: (cyl * heads + head) * sectors + sector mod
        // sectors (disk_media_sector), ceil(words / 4) beats a record -- one
        // multiply a clock (the CPU is held; the time is invisible to it)
        DE_ADDR: begin
            logic [11:0] c;
            logic [5:0]  h;
            int          ms;
            disk_eng_drv = disk_eng_commit ? disk_cm_drv : disk_strm_drv;
            c  = disk_eng_commit ? disk_cm_cyl  : disk_strm_cyl;
            h  = disk_eng_commit ? disk_cm_head : disk_strm_head;
            ms = int'(disk_eng_commit ? disk_cm_sec : disk_strm_sec);
            for (int k = 0; k < 4; k++)
                if (ms >= int'(disk_drv_sectors[disk_eng_drv])) ms -= int'(disk_drv_sectors[disk_eng_drv]);
            disk_eng_ms = 7'(ms);
            disk_eng_ch = 18'(int'(c) * int'(disk_drv_heads[disk_eng_drv]) + int'(h));
            disk_eng    = DE_ADDR2;
        end
        DE_ADDR2: begin
            disk_eng_rec = 25'(int'(disk_eng_ch) * int'(disk_drv_sectors[disk_eng_drv]) + int'(disk_eng_ms));
            disk_eng     = DE_ADDR3;
        end
        DE_ADDR3: begin
            int stride;
            stride = (disk_rec_words_fn(disk_eng_drv) + 3) / 4;
            disk_eng_base  = DISK_PACK_BEAT_BASE + 29'(disk_eng_drv) * DISK_DRIVE_BEATS +
                             29'(int'(disk_eng_rec) * stride);
            disk_eng_beats = 9'(stride);
            disk_eng_beat  = 9'd0;
            disk_eng       = disk_eng_commit ? DE_CM_RD : DE_BEAT;
        end
        // load: beat by beat from the pack into the buffer
        DE_BEAT: begin
            disk_mem_req  <= 1'b1;
            disk_mem_we   <= 1'b0;
            disk_mem_addr <= disk_eng_base + 29'(disk_eng_beat);
            disk_eng       = DE_BEAT_WAIT;
        end
        DE_BEAT_WAIT: begin
            if (disk_mem_ack) begin
                disk_mem_req <= 1'b0;
                sbuf_we      <= 1'b1;
                sbuf_wlane   <= 4'hF;
                sbuf_waddr   <= disk_eng_beat;
                sbuf_wdata   <= disk_mem_rdata;
                disk_eng_beat = disk_eng_beat + 9'd1;
                disk_eng      = (disk_eng_beat == disk_eng_beats) ? DE_IDLE : DE_BEAT;
            end
        end
        // commit: beat by beat from the buffer back to the pack
        DE_CM_RD: begin
            sbuf_raddr <= disk_eng_beat;
            disk_eng    = DE_CM_RD2;
        end
        DE_CM_RD2: disk_eng = DE_CM_WR;          // the buffer's registered read
        DE_CM_WR: begin
            disk_mem_req   <= 1'b1;
            disk_mem_we    <= 1'b1;
            disk_mem_addr  <= disk_eng_base + 29'(disk_eng_beat);
            disk_mem_wdata <= sbuf_q;
            disk_mem_be    <= 8'hFF;
            disk_eng        = DE_CM_WAIT;
        end
        DE_CM_WAIT: begin
            if (disk_mem_ack) begin
                disk_mem_req  <= 1'b0;
                disk_mem_we   <= 1'b0;
                disk_eng_beat  = disk_eng_beat + 9'd1;
                disk_eng       = (disk_eng_beat == disk_eng_beats) ? DE_IDLE : DE_CM_RD;
            end
        end
        // the refill, one iteration of disk.c's loop a step
        DE_RF_HEAD: begin
            if (disk_fifo_count >= DISK_FIFO_WORDS || disk_rf_iter >= 6'd32) begin
                disk_eng = DE_RF_POST;
            end else begin
                disk_rf_iter = disk_rf_iter + 6'd1;
                if (sequenced) begin
                    if (disk_current_block_op == DISK_OP_DONE) begin
                        disk_eng = DE_RF_POST;
                    end else if (disk_current_block_pos < disk_current_block_words) begin
                        int a;
                        a = disk_block_addr_fn(drv, int'(disk_current_block), int'(disk_current_block_pos));
                        disk_rf_restore = disk_restore_hdr_check && disk_current_block == 3'd0 &&
                                          disk_current_block_op == DISK_OP_RDCHK &&
                                          disk_current_block_pos == 16'd0;
                        disk_current_block_pos = disk_current_block_pos + 16'd1;
                        if (a < 0) begin
                            disk_push_word_task(disk_rf_restore ? 16'h0001 : 16'h0000);
                        end else begin
                            sbuf_raddr  <= 9'(a >> 2);
                            disk_rf_lane = 2'(a & 3);
                            disk_eng     = DE_RF_RD;
                        end
                    end else if (disk_current_block_trailer < 4'd4) begin
                        disk_current_block_trailer = disk_current_block_trailer + 4'd1;
                        disk_push_word_task(16'h0000);
                    end else if (disk_fifo_count > 0) begin
                        disk_eng = DE_RF_POST;
                    end else begin
                        logic blk_started;
                        blk_started = 1'b0;
                        disk_current_block = disk_current_block + 3'd1;
                        if (disk_current_block < 3'd4)
                            disk_start_current_block_task(drv, blk_started);
                        if (blk_started) begin
                            // a write block armed ends the read stream
                            if (!disk_read_stream_active) disk_eng = DE_RF_POST;
                        end else begin
                            disk_read_stream_active    = 1'b0;
                            disk_current_block_op      = DISK_OP_DONE;
                            disk_current_block_words   = 16'd0;
                            disk_current_block_pos     = 16'd0;
                            disk_current_block_prefix  = 1'b0;
                            disk_current_block_trailer = 4'd0;
                            disk_eng = DE_RF_POST;
                        end
                    end
                end else begin
                    if (disk_read_stream_index >= total) begin
                        disk_eng = DE_RF_POST;
                    end else begin
                        disk_rf_restore = 1'b0;
                        sbuf_raddr  <= 9'(disk_read_stream_index >> 2);
                        disk_rf_lane = 2'(disk_read_stream_index & 3);
                        disk_eng     = DE_RF_RD;
                    end
                end
            end
        end
        DE_RF_RD:  disk_eng = DE_RF_RD2;          // the buffer's registered read
        DE_RF_RD2: begin
            logic [15:0] w;
            w = sbuf_q[16 * int'(disk_rf_lane) +: 16];
            if (disk_rf_restore) w |= 16'h0001;
            disk_push_word_task(w);
            disk_eng = DE_RF_HEAD;
        end
        DE_RF_POST: begin
            disk_update_fifo_tws_task();
            if ((disk_read_stream_index >= total ||
                 (sequenced && disk_current_block_op == DISK_OP_DONE)) &&
                disk_fifo_count == 0) begin
                disk_read_stream_active = 1'b0;
                disk_active             = 1'b0;
                disk_rd_fifo_tw         = 1'b0;
                disk_tag_tw             = 1'b1;
            end
            if (disk_index_tw || disk_sector_tw || disk_tag_tw || disk_rd_fifo_tw || disk_wr_fifo_tw)
                ext_wakeup_req[TASK_DSK] = 1'b1;
            disk_eng = DE_IDLE;
        end
        default: disk_eng = DE_IDLE;
        endcase
    endtask

    task automatic disk_begin_read_stream_task(output logic ok);
        logic [1:0] drv;
        logic       dummy_started;
        drv = disk_selected_drive;
        if (disk_drv_online[drv]) disk_load_sector_task();
        if (!disk_drv_online[drv] || !disk_sec_valid) begin
            ok = 1'b0;
            return;
        end
        disk_fifo_count            = 0;
        disk_fifo_head             = 0;
        disk_fifo_tail             = 0;
        disk_read_stream_index     = 0;
        disk_read_stream_active    = 1'b1;
        disk_current_block         = 3'd0;
        disk_current_block_op      = 2'd0;
        disk_current_block_words   = 16'd0;
        disk_current_block_pos     = 16'd0;
        disk_current_block_prefix  = 1'b0;
        disk_current_block_trailer = 4'd0;
        disk_restore_hdr_check     = disk_transfer_restore && disk_drv_is_diablo[drv];
        disk_transfer_restore      = 1'b0;
        if (disk_read_block_framing) begin
            disk_start_current_block_task(drv, dummy_started);
        end
        disk_refill_fifo_task();
        ok = disk_read_stream_active || (disk_fifo_count > 0);
    endtask

    task automatic disk_write_stream_word_task(input logic [15:0] w);
        logic [1:0] drv;
        int hw, lw, dw, idx, total, pos;
        drv = disk_selected_drive;
        if (!disk_drv_online[drv] || disk_drv_read_only[drv]) begin
            disk_write_stream_active = 1'b0;
            return;
        end
        hw    = int'(disk_drv_hdr_words[drv]);
        lw    = int'(disk_drv_lbl_words[drv]);
        dw    = int'(disk_drv_data_words[drv]);
        idx   = disk_write_stream_index;
        total = hw + lw + dw;

        if (disk_write_block_framing && disk_current_block_prefix) begin
            disk_current_block_prefix = 1'b0;
            return;
        end

        pos = int'(disk_current_block_pos);
        if (!disk_compare_err) begin
            int rw;   // the record word written: header | label | data
            rw = -1;
            if (!disk_write_block_framing) begin
                if (idx < hw)                 rw = hw - 1 - idx;
                else if (idx < hw + lw)       rw = hw + (lw - 1 - (idx - hw));
                else if (idx < total)         rw = hw + lw + (dw - 1 - (idx - hw - lw));
            end else if (disk_current_block == 3'd0 && pos < hw) begin
                rw = pos;
            end else if (disk_current_block == 3'd1 && pos < lw) begin
                rw = hw + pos;
            end else if (disk_current_block == 3'd2 && pos < dw) begin
                rw = hw + lw + pos;
            end
            if (rw >= 0) begin
                sbuf_we    <= 1'b1;
                sbuf_wlane <= 4'(1 << (rw & 3));
                sbuf_waddr <= 9'(rw >> 2);
                sbuf_wdata <= {4{w}};
            end
            disk_sec_modified = 1'b1;
        end

        disk_write_stream_index = disk_write_stream_index + 1;
        disk_current_block_pos  = disk_current_block_pos + 16'd1;
        if (disk_write_stream_index >= total ||
            disk_current_block_pos >= disk_current_block_words) begin
            logic blk_started;
            disk_write_stream_active = 1'b0;
            disk_current_block       = disk_current_block + 3'd1;
            disk_start_current_block_task(drv, blk_started);
            if (blk_started) begin
                disk_update_fifo_tws_task();
            end else begin
                disk_active            = 1'b0;
                disk_current_block_op  = 2'd0;
                disk_tag_tw            = 1'b1;
                disk_sector_write_req  = 1'b1;
                disk_commit_sector_task();
                disk_update_fifo_tws_task();
            end
        end
    endtask

    task automatic disk_start_pending_transfer_task();
        logic [1:0] drv;
        logic       has_op, has_read_or_chk, has_write;
        drv = disk_selected_drive;
        has_op          = (disk_control[7:0] != 8'd0);
        has_read_or_chk = (disk_control[7:6] == DISK_OP_READ || disk_control[7:6] == DISK_OP_RDCHK ||
                           disk_control[5:4] == DISK_OP_READ || disk_control[5:4] == DISK_OP_RDCHK ||
                           disk_control[3:2] == DISK_OP_READ || disk_control[3:2] == DISK_OP_RDCHK ||
                           disk_control[1:0] == DISK_OP_READ || disk_control[1:0] == DISK_OP_RDCHK);
        has_write       = (disk_control[7:6] == DISK_OP_WRITE || disk_control[5:4] == DISK_OP_WRITE ||
                           disk_control[3:2] == DISK_OP_WRITE || disk_control[1:0] == DISK_OP_WRITE);

        if (disk_active || !disk_enable_run || !disk_xfer_pending || !has_op) return;

        if (has_read_or_chk) begin
            logic ok;
            disk_read_block_framing = 1'b1;
            disk_begin_read_stream_task(ok);
            if (ok) begin
                disk_active       = 1'b1;
                disk_xfer_pending = 1'b0;
            end
        end else if (has_write) begin
            logic started;
            disk_load_sector_task();
            disk_current_block = 3'd0;
            disk_start_current_block_task(drv, started);
            if (started) begin
                disk_fifo_head    = 0;
                disk_fifo_tail    = 0;
                disk_fifo_count   = 0;
                disk_xfer_pending = 1'b0;
                disk_update_fifo_tws_task();
            end
        end
    endtask

    // =========================================================================
    // 7. Combinational Read, Write-Check, IOAtten', and DMux Multiplexers
    // =========================================================================
`endif
    function automatic logic is_display_task_fn(input logic [3:0] t);
        return (t == TASK_DHT || t == TASK_AHT || t == TASK_AWT || t == TASK_DWT);
    endfunction

    function automatic logic is_display_tioa_fn(input logic [7:0] a);
        return (a >= 8'o360) ||
               (a >= TIOA_RAST_SELCMD && a <= TIOA_RAST_TASKCMD);
    endfunction

    // Combinational io_has_write for the currently executing instruction
    always_comb begin
        io_has_write = 1'b0;
        if (is_display_task_fn(io_read_task) && is_display_tioa_fn(io_read_tioa)) begin
            io_has_write = 1'b1;
        end else if (io_read_task == TASK_EMU &&
                     (io_read_tioa == TIOA_TSTATUS ||
                      io_read_tioa == TIOA_DDCSTATUS ||
                      io_read_tioa == TIOA_STATICS ||
                      io_read_tioa == TIOA_ETH_CTL)) begin
            io_has_write = 1'b1;
        end else if ((io_read_task == TASK_EOT || io_read_task == TASK_EIT) &&
                     (io_read_tioa == TIOA_ETH_DATA || io_read_tioa == TIOA_ETH_CTL)) begin
            io_has_write = 1'b1;
        end else if (io_read_task == TASK_DSK &&
                     (io_read_tioa >= TIOA_DISKCONTROL && io_read_tioa <= TIOA_DISKTAG)) begin
            io_has_write = 1'b1;
        end
    end

    // Combinational Slow I/O Read (Pd<-Input / Pd<-InputNoPE)
    always_comb begin
        io_read_data       = 16'hFFFF;
        io_read_bad_parity = 1'b1;

        // 1. Display (DDC)
        if ((is_display_task_fn(io_read_task) && is_display_tioa_fn(io_read_tioa)) ||
            (io_read_task == TASK_EMU &&
             (io_read_tioa == TIOA_TSTATUS ||
              io_read_tioa == TIOA_DDCSTATUS ||
              io_read_tioa == TIOA_STATICS))) begin
            io_read_bad_parity = 1'b0;
            if (io_read_tioa == TIOA_TSTATUS) begin
                io_read_data = ddc_peek_terminal_bit_fn(io_read_task);
            end else if (io_read_tioa == TIOA_DDCSTATUS) begin
                // DispY Status bit 0 carries the terminal's serial data when
                // DispY controls the terminal (DisplayAux.mc DisplayInitConfig);
                // Cedar's ReadTerminal reads it here. 0 for the emulator task.
                io_read_data = ddc_peek_terminal_bit_fn(io_read_task);
            end else begin
                io_read_data = 16'h0000;
            end
        end
        // 2. Ethernet
        else if ((io_read_task == TASK_EOT || io_read_task == TASK_EIT || io_read_task == TASK_EMU) &&
                 (io_read_tioa == TIOA_ETH_DATA || io_read_tioa == TIOA_ETH_CTL)) begin
            if (io_read_tioa == TIOA_ETH_CTL) begin
                io_read_bad_parity = 1'b0;
                io_read_data       = {eth_local_host, 8'h00};
            end else if (io_read_tioa == TIOA_ETH_DATA && io_read_task == TASK_EIT) begin
                if (eth_rx_pos >= eth_rx_count || eth_rx_hold != 32'd0 ||
                    (eth_rx_pos == 0 && eth_rx_count > int'(eth_world_rx_words) + 2)) begin
                    io_read_bad_parity = 1'b1;
                    io_read_data       = 16'hFFFF;
                end else begin
                    io_read_bad_parity = 1'b0;
                    io_read_data       = eth_rx_q;
                end
            end
        end
`ifndef DORADO_NO_DISK
        // 3. Disk (Trident)
        else if (io_read_task == TASK_DSK &&
                 (io_read_tioa >= TIOA_DISKCONTROL && io_read_tioa <= TIOA_DISKTAG)) begin
            if (io_read_tioa == TIOA_DISKDATA) begin
                io_read_bad_parity = 1'b0;
                if (disk_fifo_count > 0) begin
                    io_read_data = disk_fifo[disk_fifo_tail & 15];
                end else begin
                    io_read_data = 16'hFFFF;
                end
            end else if (io_read_tioa == TIOA_DISKMUFF) begin
                io_read_bad_parity = 1'b0;
                io_read_data       = disk_muffler_bit_fn(disk_muff_addr) ? 16'h0001 : 16'h0000;
            end
        end
`endif
    end

    // Combinational IOAtten' (sampled at instruction issue when ctask != 0)
    always_comb begin
        io_atten = 1'b0;
        if (io_read_task == TASK_EIT && io_read_tioa == TIOA_ETH_DATA) begin
            if (eth_rx_hold == 32'd0 && eth_rx_pos < eth_rx_count &&
                eth_rx_att_q) begin
                io_atten = 1'b1;
            end
        end
    end

    // Combinational DMux status for Disk (02000..02037)
    always_comb begin
        dmux_ext_handled = 1'b0;
        dmux_ext_val     = 16'h0000;
`ifndef DORADO_NO_DISK
        if (dmux_cur_addr >= 12'o2000 && dmux_cur_addr <= 12'o2037) begin
            dmux_ext_handled = 1'b1;
            dmux_ext_val     = disk_muffler_bit_fn(8'(dmux_cur_addr - 12'o2000)) ? 16'h8000 : 16'h0000;
        end
`endif
    end

    // Combinational Fast I/O input munch (for DM_REF_IOSTORE on TASK_DSK)
    always_comb begin
        for (int i = 0; i < 16; i++) begin
            fastio_in_munch[i] = 16'd0;
`ifndef DORADO_NO_DISK
            if (fastio_task == TASK_DSK && i < disk_fifo_count) begin
                fastio_in_munch[i] = disk_fifo[(disk_fifo_tail + i) % DISK_FIFO_WORDS];
            end
`endif
        end
    end

    always_ff @(posedge clk) begin
        if (rx_wr_en) begin
            eth_rx_words[rx_wr_addr]     <= rx_wr_data;
            eth_rx_attention[rx_wr_addr] <= rx_wr_att;
        end
        eth_rx_q     <= eth_rx_words[eth_rx_pos[11:0]];
        eth_rx_att_q <= eth_rx_attention[eth_rx_pos[11:0]];
    end

    always_ff @(posedge clk) begin
        eth_tx_rd_data <= eth_tx_words[eth_tx_rd_addr];
    end


    // =========================================================================
    // Framebuffer render engine, in two parts.
    //
    // The copy engine (a_*) owns the line buffer port: it copies the DWT's
    // munches into ddc_cur_words, and for a render job it snapshots the
    // words the line needs into a private buffer (r_line) and latches the
    // job (rr_*). io_busy holds the CPU while it works -- a munch, or at most
    // 64 words for a full LF line.
    //
    // The pixel engine (p_*) then writes the framebuffer from that snapshot,
    // one pixel per clock, WITHOUT holding the CPU: a 1024-pixel line used to
    // hold it for 1024 clocks, 12% of all clocks on Cedar's desktop. Only a
    // new render job that arrives while it is still drawing waits for it.
    // The framebuffer is a two-port RAM (this port and scan-out).
    //
    // Line: pixels x < width take the word bits MSB first from
    // ddc_cur_words[ch][start_off + x/16] (0 past the gathered words); pixels
    // at or beyond width are left as they were (C model ddc_render_line).
    // Cursor: the 16 row bits are ORed in at x (C model's cursor overlay).
    // =========================================================================
    typedef enum logic [1:0] { A_IDLE, A_COPY, A_SNAP, A_HAND } astate_t;
    typedef enum logic [2:0] { P_IDLE, P_WADDR, P_WWAIT, P_PIX, P_CUR } pstate_t;
    astate_t     a_st;
    pstate_t     p_st;
    logic        r_rpend, r_wpend;    // render / copy job waiting for the copy engine
    logic [4:0]  r_wj;                // copy index
    logic [6:0]  a_k;                 // snapshot word index
    logic        a_kv;                // a_k - 1's read is arriving (line buffer reads are one clock late)
    logic [6:0]  a_kd;
    logic        a_cursor;            // the snapshot is a cursor row
    // the job being drawn
    logic        rr_cursor;
    logic [9:0]  rr_y;
    logic [10:0] rr_width;
    logic [15:0] rr_cx;
    logic [15:0] rr_cbits;
    logic [10:0] r_x;                 // pixel being written
    logic [15:0] r_word;              // line word holding pixel r_x
    logic [4:0]  r_b;                 // cursor bit
    logic [19:0] r_addr;
    logic        r_we, r_wd;

    // Line buffer port: the copy engine writes munches and reads snapshots.
    logic [7:0]  cw_addr;
    logic        cw_we;
    logic [15:0] cw_wd;
    logic [15:0] cw_q;
    always_ff @(posedge clk) begin
        if (cw_we) ddc_cur_words[cw_addr] <= cw_wd;
        cw_q <= ddc_cur_words[cw_addr];
    end
    // The pixel engine's private copy of the line.
    (* ramstyle = "M10K" *) logic [15:0] r_line [0:63];
    logic        rl_we;
    logic [5:0]  rl_waddr, rl_raddr;
    logic [15:0] rl_wd, rl_q;
    always_ff @(posedge clk) begin
        if (rl_we) r_line[rl_waddr] <= rl_wd;
        rl_q <= r_line[rl_raddr];
    end

    // held: the copy engine has work, or a job waits for the pixel engine
    assign r_busy = r_rpend || r_wpend || (a_st != A_IDLE);

    always_comb begin
        logic [7:0] src;
        fb_addr_a = r_addr;
        fb_we_a   = r_we;
        fb_wd_a   = r_wd;
        src       = {4'd0, rj_start_off} + {1'b0, a_k};
        cw_we     = (a_st == A_COPY);
        cw_addr   = (a_st == A_COPY) ? {wc_ch, wc_base + {2'd0, r_wj}} : {rj_ch, src[6:0]};
        cw_wd     = fastio_out_munch[r_wj[3:0]];
        rl_raddr  = r_x[9:4];
    end

    always_ff @(posedge clk) begin
        r_we  <= 1'b0;
        rl_we <= 1'b0;
        if (rst) begin
            a_st                <= A_IDLE;
            p_st                <= P_IDLE;
            r_rpend             <= 1'b0;
            r_wpend             <= 1'b0;
            a_kv                <= 1'b0;
            ddc_pixels_rendered <= 64'd0;
        end else begin
            // A tick posts at most one render and one copy; the render (from
            // a write strobe, which the C model handles before the munch)
            // goes first.
            if (rj_start) r_rpend <= 1'b1;
            if (wc_start) r_wpend <= 1'b1;

            // ---- copy engine --------------------------------------------
            case (a_st)
            A_IDLE: begin
                // a render first, and only once the pixel engine has finished
                // with r_line (a munch copy behind it waits too, in order)
                if (r_rpend) begin
                    if (p_st == P_IDLE) begin
                    r_rpend  <= 1'b0;
                    a_cursor <= rj_cursor;
                    a_k      <= 7'd0;
                    a_kv     <= 1'b0;
                    a_st     <= rj_cursor ? A_HAND : A_SNAP;
                    end
                end else if (r_wpend) begin
                    r_wpend <= 1'b0;
                    r_wj    <= 5'd0;
                    a_st    <= A_COPY;
                end
            end
            A_COPY: begin
                if (r_wj == wc_n - 5'd1) a_st <= A_IDLE;
                else                     r_wj <= r_wj + 5'd1;
            end
            A_SNAP: begin
                // read word a_k this clock; store word a_k-1 that arrives
                logic [7:0] srcd;
                srcd = {4'd0, rj_start_off} + {1'b0, a_kd};
                if (a_kv) begin
                    rl_we    <= 1'b1;
                    rl_waddr <= a_kd[5:0];
                    rl_wd    <= (srcd < rj_wcount && srcd < 8'd128) ? cw_q : 16'd0;
                end
                a_kd <= a_k;
                a_kv <= 1'b1;
                if (a_kv && ({4'd0, a_kd} + 11'd1) * 16 >= rj_width) a_st <= A_HAND;
                else a_k <= a_k + 7'd1;
            end
            A_HAND: begin
                // hand the job to the pixel engine once it is free
                if (p_st == P_IDLE) begin
                    rr_cursor <= a_cursor;
                    rr_y      <= rj_y;
                    rr_width  <= rj_width;
                    rr_cx     <= rj_cx;
                    rr_cbits  <= rj_cbits;
                    r_x       <= 11'd0;
                    r_b       <= 5'd0;
                    p_st      <= a_cursor ? P_CUR : P_WADDR;
                    a_st      <= A_IDLE;
                end
            end
            default: a_st <= A_IDLE;
            endcase

            // ---- pixel engine (does not hold the CPU) -------------------
            case (p_st)
            P_IDLE: ;
            P_WADDR: p_st <= P_WWAIT;           // rl_raddr = word holding r_x
            P_WWAIT: begin
                r_word <= rl_q;
                p_st   <= P_PIX;
            end
            P_PIX: begin
                // line: pixels x < width take the word bits, MSB first
                logic px;
                px     = r_word[4'd15 - r_x[3:0]];
                r_we   <= 1'b1;
                r_wd   <= px;
                r_addr <= {rr_y, r_x[9:0]};
                if (px) ddc_pixels_rendered <= ddc_pixels_rendered + 64'd1;
                if (r_x == rr_width - 11'd1)  p_st <= P_IDLE;
                else begin
                    r_x <= r_x + 11'd1;
                    if (r_x[3:0] == 4'd15) p_st <= P_WADDR;
                end
            end
            P_CUR: begin
                // cursor: OR the 16 row bits in at x (never clears)
                logic signed [16:0] x;
                x = $signed({rr_cx[15], rr_cx}) + $signed({12'd0, r_b});
                if (rr_cbits[4'd15 - r_b[3:0]] && x >= 0 && x < DISPLAY_W) begin
                    r_we   <= 1'b1;
                    r_wd   <= 1'b1;
                    r_addr <= {rr_y, x[9:0]};
                end
                if (r_b == 5'd15) p_st <= P_IDLE;
                else              r_b  <= r_b + 5'd1;
            end
            default: p_st <= P_IDLE;
            endcase
        end
    end

    // =========================================================================
    // 8. Main Synchronous State Machine. clk_en is the CPU's step_done: the
    //    controllers advance exactly once per microinstruction, in the clock
    //    after it completes, consuming the strobes (io_read_stb, io_write_stb,
    //    fastio_stb) it left. The CPU does not start the next microinstruction
    //    until that clock has passed, so its I/O reads see the result.
    // =========================================================================
    always @(posedge clk) begin
        rj_start = 1'b0;
        wc_start = 1'b0;
        cl_we    = 1'b0;
`ifndef DORADO_NO_DISK
        sbuf_we <= 1'b0;
`endif
        if (kbd0_poke && !rst) ddc_kbd_words[0] = kbd0_val;
        if (rx_load && !rst) rx_load_pend = 1'b1;
        if (rst) begin
            rx_load_pend   = 1'b0;
            rx_applied     = 15'd0;
            wc_ch          = 1'b0;
            wc_base        = 7'd0;
            wc_n           = 5'd0;
            eth_skip_busy  = 1'b0;
            eth_skip_phase = 1'b0;
            eth_skip_count = 0;
            rj_cursor      = 1'b0;
            rj_ch          = 1'b0;
            rj_y           = 10'd0;
            rj_width       = 11'd0;
            rj_start_off   = 4'd0;
            rj_wcount      = 8'd0;
            rj_cx          = 16'd0;
            rj_cbits       = 16'd0;
            // Display reset
            ddc_statics             = 16'h0003; // STATICS_ALL_SHUTUP
            ddc_riob                = 16'd0;
            ddc_terminal_task       = 4'd0;
            ddc_ram_keep            = 1'b1;
            ddc_ram_addr            = 16'd0;
            for (int i = 0; i < 16; i++) ddc_nlcb[i] = 16'd0;
            ddc_nlcb_line           = 16'd0;
            ddc_nlcb_field_odd      = 1'b0;
            ddc_nlcb_cursor_x       = 16'd0;
            ddc_cursor_rows         = 64'd0;
            ddc_cursor_wr           = 64'd0;
            ddc_cursor_dbg_ymin     = 16'hFFFF;
            ddc_nlcb_cursor_lo      = 8'd0;
            ddc_scan_line           = 32'd0;
            ddc_scan_pixel          = 32'd0;
            ddc_frame_count         = 64'd0;
            ddc_lines_rendered      = 64'd0;
            for (int c = 0; c < 2; c++) begin
                ddc_next_wcb_flag[c]    = 1'b0;
                ddc_current_wcb_flag[c] = 1'b0;
                ddc_wcb_draw_x[c]       = 16'd0;
                ddc_wcb_draw_y[c]       = 16'd0;
                ddc_pend_valid[c]       = 1'b0;
                ddc_cur_valid[c]        = 1'b0;
                ddc_cur_word_count[c]   = 16'd0;
            end
            ddc_fifo_a_head         = 0;
            ddc_fifo_a_tail         = 0;
            ddc_fifo_b_head         = 0;
            ddc_fifo_b_tail         = 0;
            ddc_raster_taskcmd      = 16'd0;
            ddc_raster_lt_enabled   = 1'b0;
            ddc_raster_next_channel = 2'd0;
            for (int c = 0; c < 4; c++) begin
                ddc_raster_next_wt_flag[c]    = 1'b0;
                ddc_raster_current_wt_flag[c] = 1'b0;
            end
            ddc_boot_button_scanlines = 32'd0;
            for (int i = 0; i < 5; i++) ddc_kbd_words[i] = 16'hFFFF;
            ddc_kbd_pending         = 5'd0;
            ddc_kbd_order_n         = 3'd0;
            for (int k = 0; k < 5; k++) ddc_kbd_order[k] = 3'd0;
            ddc_mouse_dx            = 0;
            ddc_mouse_dy            = 0;
            ddc_mouse_pending       = 1'b0;
            ddc_term_msg_bit        = 8'd0;
            ddc_term_msg_word       = 8'd0;
            ddc_term_msg_type       = 8'd0;
            ddc_term_msg_body       = 16'd0;
            ddc_term_msg_active     = 1'b0;
            ddc_term_bits           = 64'd0;
            ddc_term_messages       = 64'd0;
            ddc_scanline_cd         = 16'(SCANLINE_PERIOD_CYCLES);

            // Ethernet reset
            eth_tx_on               = 1'b0;
            eth_tx_eop              = 1'b0;
            eth_tx_cntdwn           = 1'b0;
            eth_rx_on               = 1'b0;
            eth_no_wakeups          = 1'b0;
            eth_rx_hold             = 32'd0;
            eth_rx_wire_timer       = 32'd0;
            eth_world_rx_words      = 16'hFFFF;
            eth_tx_count            = 0;
            eth_rx_count            = 0;
            eth_rx_pos              = 0;
            eth_tx_packets          = 64'd0;
            eth_rx_packets          = 64'd0;
            eth_data_reads          = 64'd0;
            eth_data_writes         = 64'd0;
            eth_tx_packet_stb       = 1'b0;
            eth_tx_packet_words     = 11'd0;

`ifndef DORADO_NO_DISK
            // Disk reset
            disk_selected_drive     = 2'd0;
            for (int i = 0; i < 4; i++) begin
                disk_drv_online[i]          = 1'b0;
                disk_drv_selected[i]        = 1'b0;
                disk_drv_read_only[i]       = 1'b0;
                disk_drv_head_ovfl[i]       = 1'b0;
                disk_drv_heads[i]           = 6'd5;
                disk_drv_sectors[i]         = 7'd29;
                disk_drv_sectors_per_rev[i] = 7'd30;
                disk_drv_subsector_cnt[i]   = 6'd3;
                disk_drv_cur_cyl[i]         = 12'd0;
                disk_drv_cur_head[i]        = 6'd0;
                disk_drv_cur_sector[i]      = 7'd0;
                disk_drv_seek_in_prog[i]    = 0;
                disk_drv_index_pulse[i]     = 1'b0;
                disk_drv_is_diablo[i]       = 1'b1;
                disk_drv_hdr_words[i]       = 4'd2;
                disk_drv_lbl_words[i]       = 5'd8;
                disk_drv_data_words[i]      = 11'd256;
            end
            disk_control               = 16'd0;
            disk_tag                   = 16'd0;
            disk_muff_addr             = 8'd0;
            for (int i = 0; i < DISK_FORMAT_RAM_WORDS; i++) disk_format_ram[i] = 16'd0;
            disk_format_ram_addr       = 4'd0;
            disk_format_ram_writes     = 0;
            disk_enable_run            = 1'b0;
            disk_debug_mode            = 1'b0;
            disk_block_till_index      = 1'b0;
            disk_active                = 1'b0;
            disk_xfer_pending          = 1'b0;
            disk_restore_pending       = 1'b0;
            disk_transfer_restore      = 1'b0;
            disk_restore_hdr_check     = 1'b0;
            disk_index_tw              = 1'b0;
            disk_sector_tw             = 1'b0;
            disk_tag_tw                = 1'b0;
            disk_rd_fifo_tw            = 1'b0;
            disk_wr_fifo_tw            = 1'b0;
            disk_compare_err           = 1'b0;
            disk_read_data_err         = 1'b0;
            disk_fifo_underflow        = 1'b0;
            disk_fifo_overflow         = 1'b0;
            disk_fifo_head             = 0;
            disk_fifo_tail             = 0;
            disk_fifo_count            = 0;
            disk_read_stream_active    = 1'b0;
            disk_read_block_framing    = 1'b0;
            disk_read_stream_index     = 0;
            disk_write_stream_active   = 1'b0;
            disk_write_block_framing   = 1'b0;
            disk_write_stream_index    = 0;
            disk_current_block         = 3'd0;
            disk_current_block_op      = 2'd0;
            disk_current_block_words   = 16'd0;
            disk_current_block_pos     = 16'd0;
            disk_current_block_prefix  = 1'b0;
            disk_current_block_trailer = 4'd0;
            disk_sec_valid             = 1'b0;
            disk_sec_modified          = 1'b0;
            disk_job_load              = 1'b0;
            disk_job_commit            = 1'b0;
            disk_job_refill            = 1'b0;
            disk_eng_busy              = 1'b0;
            disk_stage                 = 2'd0;
            dk_rd_pop                  = 1'b0;
            dk_wr                      = 1'b0;
            dk_fio                     = 1'b0;
            disk_eng                   = DE_IDLE;
            disk_mem_req              <= 1'b0;
            disk_mem_we               <= 1'b0;
            for (int i = 0; i < 4; i++) begin
                disk_drv_cyls[i]  = 12'd0;
                disk_drv_media[i] = 1'b0;
            end
            disk_next_sector_cycle     = 64'd0;
            disk_cycles_per_rev        = 32'd277778;   // DORADO_DISK_CYCLES_PER_REV
            disk_sector_read_req       = 1'b0;
            disk_sector_write_req      = 1'b0;

`endif
            ext_wakeup_req             = 16'd0;
            ext_subtask_we             = 1'b0;
            ext_subtask_task           = 4'd0;
            ext_subtask_val            = 2'd0;
`ifdef DORADO_CHECKPOINT
`include "checkpoint_io.svh"
`endif
        end else if (eth_skip_busy) begin
            // WaitForBOP walk: phase 0 waits for eth_rx_att_q at eth_rx_pos.
            if (!eth_skip_phase) begin
                eth_skip_phase = 1'b1;
            end else begin
                eth_skip_phase = 1'b0;
                eth_rx_pos     = eth_rx_pos + 1;
                eth_skip_count = eth_skip_count + 1;
                if (eth_rx_att_q || eth_rx_pos >= eth_rx_count) begin
                    eth_rx_hold   = 32'(eth_skip_count * 170) - 32'd1;
                    eth_skip_busy = 1'b0;
                end
            end
        end else if (rx_load_pend) begin
            rx_load_pend = 1'b0;
            rx_applied   = rx_applied + 15'd1;
            eth_rx_pos   = 0;
            eth_rx_count = int'(rx_load_count);
`ifndef DORADO_NO_DISK
        end else if (disk_stage == 2'd1) begin
            disk_stage1_task();
            disk_stage = 2'd2;
        end else if (disk_stage == 2'd2) begin
            disk_stage2_task();
            disk_stage = 2'd0;
            // work the strobes or the timer queued for the sector engine
            if (disk_job_load || disk_job_commit || disk_job_refill) disk_eng_busy = 1'b1;
        end else if (disk_eng_busy) begin
            disk_engine_step_task();
`endif
        end else if (clk_en) begin
            eth_tx_packet_stb     = 1'b0;
            disk_sector_read_req  = 1'b0;
            disk_sector_write_req = 1'b0;
            ext_wakeup_req        = 16'd0;
            ext_subtask_we        = 1'b0;

            // -----------------------------------------------------------------
            // A. External Keyboard / Mouse Input Latches
            // -----------------------------------------------------------------
            for (int i = 0; i < 5; i++) begin
                if (kbd_words_in[i] != ddc_kbd_words[i]) begin
                    ddc_kbd_words[i]   = kbd_words_in[i];
                    if (!ddc_kbd_pending[i]) begin
                        ddc_kbd_pending[i] = 1'b1;
                        ddc_kbd_order[ddc_kbd_order_n] = 3'(i);
                        ddc_kbd_order_n = ddc_kbd_order_n + 3'd1;
                    end
                end
            end
            if (mouse_delta_stb && (mouse_dx_in != 16'sd0 || mouse_dy_in != 16'sd0)) begin
                ddc_mouse_dx      = ddc_mouse_dx + int'(mouse_dx_in);
                ddc_mouse_dy      = ddc_mouse_dy + int'(mouse_dy_in);
                ddc_mouse_pending = 1'b1;
            end

            // -----------------------------------------------------------------
            // B. Slow I/O Read Side Effects (io_read_stb)
            // -----------------------------------------------------------------
            if (io_read_stb) begin
                // 1. Display TSTATUS serial bit advance
                if ((io_read_stb_task == TASK_DHT || io_read_stb_task == TASK_AHT) &&
                    (io_read_stb_tioa == TIOA_TSTATUS || io_read_stb_tioa == TIOA_DDCSTATUS)) begin
                    ddc_advance_terminal_bit_task();
                end
                // 2. Ethernet RX word pop
                else if (io_read_stb_task == TASK_EIT && io_read_stb_tioa == TIOA_ETH_DATA) begin
                    eth_data_reads = eth_data_reads + 64'd1;
                    if (eth_rx_pos < eth_rx_count && eth_rx_hold == 32'd0 &&
                        !(eth_rx_pos == 0 && eth_rx_count > int'(eth_world_rx_words) + 2)) begin
                        eth_rx_pos = eth_rx_pos + 1;
                    end
                end
`ifndef DORADO_NO_DISK
                // 3. Disk FIFO pop: the disk stage (disk_tick_pend) does it
`endif
            end

            // -----------------------------------------------------------------
            // C. Slow I/O Write Side Effects (io_write_stb)
            // -----------------------------------------------------------------
            if (io_write_stb) begin
                // 1. Display (DDC)
                if ((is_display_task_fn(io_write_task) && is_display_tioa_fn(io_write_tioa)) ||
                    (io_write_task == TASK_EMU &&
                     (io_write_tioa == TIOA_TSTATUS ||
                      io_write_tioa == TIOA_DDCSTATUS ||
                      io_write_tioa == TIOA_STATICS))) begin
                    logic old_ram_keep;
                    old_ram_keep = ddc_ram_keep;
                    ddc_riob     = io_write_data;

                    if ((io_write_task == TASK_DHT || io_write_task == TASK_AHT) &&
                        (io_write_tioa == TIOA_STATICS || io_write_tioa == TIOA_TSTATICS)) begin
                        if (ddc_terminal_task == 4'd0) ddc_terminal_task = io_write_task;
                    end

                    if (io_write_tioa == TIOA_STATICS || io_write_tioa == TIOA_TSTATICS) begin
                        ddc_statics = io_write_data;
                        if (io_write_data[1]) begin // STATICS_DWT_SHUTUP
                            for (int c = 0; c < 2; c++) begin
                                ddc_next_wcb_flag[c]    = 1'b0;
                                ddc_current_wcb_flag[c] = 1'b0;
                                ddc_pend_valid[c]       = 1'b0;
                                ddc_cur_valid[c]        = 1'b0;
                                ddc_cur_word_count[c]   = 16'd0;
                            end
                            ddc_fifo_a_head = 0; ddc_fifo_a_tail = 0;
                            ddc_fifo_b_head = 0; ddc_fifo_b_tail = 0;
                        end
                        if (io_write_data[0]) begin // STATICS_DHT_SHUTUP
                            ddc_raster_lt_enabled = 1'b0;
                            for (int c = 0; c < 4; c++) begin
                                ddc_raster_next_wt_flag[c]    = 1'b0;
                                ddc_raster_current_wt_flag[c] = 1'b0;
                            end
                        end
                    end else if (io_write_tioa == TIOA_HRAM) begin
                        if (!old_ram_keep) begin
                            if (io_write_data & 16'o040000) begin // RAM_LOADADDR
                                ddc_ram_addr = io_write_data & 16'd511;
                            end
                            if (!(io_write_data & 16'o020000)) begin // !RAM_WRITE
                                ddc_hram[ddc_ram_addr & 16'd511] = {5'd0, io_write_data[2:0]};
                                ddc_ram_addr = (ddc_ram_addr + 16'd1) & 16'd511;
                            end
                        end
                        ddc_ram_keep = (io_write_data & 16'o100000) != 16'd0;
                    end else if (io_write_tioa == TIOA_DHTFLAG || io_write_tioa == TIOA_AHTFLAG) begin
                        if (io_write_task == ddc_terminal_task) begin
                            if (io_write_data & 16'o0002) begin
                                ddc_snapshot_line_task(0);
                                ddc_next_wcb_flag[0] = 1'b1;
                            end
                        end else begin
                            if (io_write_data & 16'o0002) begin
                                ddc_snapshot_line_task(0);
                                ddc_next_wcb_flag[0] = 1'b1;
                            end
                            if (io_write_data & 16'o0004) begin
                                ddc_snapshot_line_task(1);
                                ddc_next_wcb_flag[1] = 1'b1;
                            end
                        end
                    end else if (io_write_tioa == TIOA_DWTFLAG || io_write_tioa == TIOA_AWTFLAG) begin
                        int ch;
                        ch = io_write_subtask[1] ? 1 : 0;
                        if (io_write_data & 16'o0020) begin
                            // IOFetch pacing pulse: no change
                        end else if (io_write_data & 16'o0001) begin
                            int draw_y;
                            draw_y = int'(ddc_scan_line);
                            if (ddc_nlcb_line > 16'd0) begin
                                draw_y = (int'(ddc_nlcb_line) - 1) * 2 + int'(ddc_nlcb_field_odd);
                            end
                            ddc_current_wcb_flag[ch] = 1'b1;
                            ddc_next_wcb_flag[ch]    = 1'b0;
                            ddc_wcb_draw_x[ch]       = 16'd0;
                            ddc_wcb_draw_y[ch]       = 16'(draw_y);
                            ddc_start_line_task(ch, draw_y);
                        end else begin
                            // Render the line this channel has gathered: post a
                            // job to the framebuffer engine (it writes while
                            // io_busy holds the CPU). Matches the C model's
                            // ddc_render_line guards.
                            int width_pixels, y;
                            width_pixels = ddc_decode_width_pixels_fn(ddc_cur_width[ch]);
                            y = (ddc_cur_abs_y[ch] ? int'(ddc_cur_y[ch])
                                                   : (int'(ddc_cur_line[ch]) * 2 + int'(ddc_cur_odd[ch])))
                                - int'(display_top_lines);
                            if (ddc_cur_valid[ch] && ddc_cur_word_count[ch] != 16'd0 &&
                                width_pixels > 0 && y >= 0 && y < DISPLAY_H) begin
                                ddc_lines_rendered = ddc_lines_rendered + 64'd1;
                                rj_start     = 1'b1;
                                rj_cursor    = 1'b0;
                                rj_ch        = ch[0];
                                rj_y         = 10'(y);
                                rj_width     = 11'(width_pixels);
                                rj_start_off = ddc_cur_ptr[ch][3:0];
                                rj_wcount    = ddc_cur_word_count[ch][7:0];
                            end
                            ddc_current_wcb_flag[ch] = 1'b0;
                        end
                    end else if (io_write_tioa == TIOA_RAST_TASKCMD) begin
                        ddc_raster_taskcmd = io_write_data;
                        if (io_write_task == TASK_DHT) begin
                            ddc_raster_lt_enabled = (io_write_data & 16'h0002) ? 1'b0 : 1'b1;
                            if (io_write_data & 16'h0004) begin
                                int ch;
                                ch = int'(ddc_raster_next_channel);
                                ddc_raster_next_wt_flag[ch] = 1'b1;
                                ddc_raster_next_channel     = 2'(ch + 1);
                            end
                        end else if (io_write_task == TASK_DWT) begin
                            int ch;
                            ch = int'(io_write_subtask);
                            if (io_write_data & 16'h0001) begin
                                ddc_raster_current_wt_flag[ch] = 1'b0;
                            end
                            if (io_write_data & 16'h0008) begin
                                ddc_raster_next_wt_flag[ch]    = 1'b0;
                                ddc_raster_current_wt_flag[ch] = 1'b0;
                            end
                        end
                    end else if (io_write_tioa == TIOA_NLCB || io_write_tioa == TIOA_TNLCB) begin
                        logic [3:0]  reg_sel;
                        logic [11:0] val;
                        reg_sel           = io_write_data[15:12];
                        val               = io_write_data[11:0];
                        ddc_nlcb[reg_sel] = {4'd0, val};
                        case (reg_sel)
                            4'o00: begin // VCW
                                if (io_write_data & 16'o0002) begin // VSync
                                    ddc_nlcb_line      = 16'd0;
                                    ddc_nlcb_field_odd = io_write_data[0];
                                    for (int c = 0; c < 2; c++) begin
                                        ddc_pend_valid[c]     = 1'b0;
                                        ddc_cur_valid[c]      = 1'b0;
                                        ddc_cur_word_count[c] = 16'd0;
                                    end
                                end
                            end
                            4'o13, 4'o15: begin // CursorX
                                if (!(reg_sel == 4'o13 && val == 12'd0)) begin
                                    ddc_nlcb_cursor_x = {4'd0, val};
                                end
                            end
                            4'o03: begin // APointer -- or Lyric's CursorX
                                // LispDisplayMain.mc EndBootCheck sends
                                // CursorX_ CursorXConst - cursorX, the constant
                                // from ReadIM[2]/[3]; under aemu.eb its select
                                // bits arrive as 03 (Cedar's as 13, above), the
                                // same open ReadIM question. APointer's own
                                // writes (the reader pointer, `T_ APointer`)
                                // are < 0400; a cursor value is >= 04000.
                                // Measured in C (aemu.eb, Lyric): cursorX 100
                                // -> 07532, 500 -> 06712, i.e. x = 4030 - val;
                                // rebased onto the 3771 the draw below uses.
                                if (io_write_tioa == TIOA_TNLCB && ddc_terminal_task == TASK_AHT &&
                                    val >= 12'o4000) begin
                                    ddc_nlcb_cursor_x = {4'd0, val} - 16'd259;
                                end
                            end
                            4'o16: begin // CursorLo
                                ddc_nlcb_cursor_lo = val[7:0];
                                ddc_nlcb_line      = ddc_nlcb_line + 16'd1;
                                begin
                                    int yl;
                                    yl = (int'(ddc_nlcb_line) - 1) * 2 + int'(ddc_nlcb_field_odd)
                                         - int'(display_top_lines);
                                    if (yl >= 0 && yl < DISPLAY_H) begin
                                        cl_we = 1'b1; cl_addr = 10'(yl); cl_wd = 16'd0;
                                    end
                                end
                            end
                            4'o17: begin // CursorHi
                                logic [15:0] row_bits;
                                row_bits = {val[7:0], ddc_nlcb_cursor_lo};
                                ddc_cursor_wr = ddc_cursor_wr + 64'd1;
                                if (row_bits != 16'd0) begin
                                    int x, y;
                                    // Cedar's DisplayMain.mc sends base - cursorX: measured
                                    // on the Cedar desktop, cursor X 0 arrives as 3771 and
                                    // X 700 as 3071 (was 07235 = 3741, 30 pixels left)
                                    x = 3771 - int'(ddc_nlcb_cursor_x); // CURSOR_X_BIAS = 07273
                                    y = (int'(ddc_nlcb_line) - 1) * 2 + int'(ddc_nlcb_field_odd)
                                        - int'(display_top_lines);
                                    if (y >= 0 && y < DISPLAY_H && x > -16 && x < DISPLAY_W) begin
                                        cl_we     = 1'b1;
                                        cl_addr   = 10'(y);
                                        cl_wd     = row_bits;
                                        cur_x     = 16'(x);
                                        ddc_cursor_rows = ddc_cursor_rows + 64'd1;
                                        ddc_cursor_dbg_x = 16'(x);
                                        ddc_cursor_dbg_y = 16'(y);
                                        if (16'(y) < ddc_cursor_dbg_ymin) ddc_cursor_dbg_ymin = 16'(y);
                                    end
                                end
                                ddc_nlcb_cursor_lo = 8'd0;
                            end
                            default: ;
                        endcase
                    end
                end
                // 2. Ethernet
                else if (io_write_tioa == TIOA_ETH_CTL &&
                         (io_write_task == TASK_EOT || io_write_task == TASK_EIT || io_write_task == TASK_EMU)) begin
                    if (!io_write_data[15]) begin // !TXCMDENBLN (0x8000)
                        logic on;
                        on = io_write_data[14]; // TXON (0x4000)
                        if (on && !eth_tx_on) begin
                            eth_tx_count = 0;
                        end
                        eth_tx_on = on;
                        if (!on) begin
                            eth_tx_eop    = 1'b0;
                            eth_tx_cntdwn = 1'b0;
                            eth_tx_count  = 0;
                        end else begin
                            eth_tx_eop    = io_write_data[13]; // TXEOP (0x2000)
                            eth_tx_cntdwn = 1'b0;
                            if (eth_tx_eop && eth_tx_count > 0) begin
                                eth_tx_packet_stb   = 1'b1;
                                eth_tx_packet_words = 11'(eth_tx_count);
                                eth_tx_packets      = eth_tx_packets + 64'd1;
                                eth_tx_eop          = 1'b0;
                                eth_tx_count        = 0;
                            end
                        end
                    end
                    if (!io_write_data[11]) begin // !RXCMDENBLN (0x0800)
                        logic on;
                        on = io_write_data[10]; // RXON (0x0400)
                        if (on && !io_write_data[9] && eth_rx_on) begin // !RXBOPN (0x0200): WaitForBOP
                            if (eth_rx_hold == 32'd0) begin
                                // Skip to just past the next attention word.
                                // The walk runs one word per two clocks after
                                // this tick (io_busy holds the CPU); hold = 170
                                // per word skipped, minus this tick's
                                // decrement. Hold 2 meanwhile, so this tick's
                                // wakeup check sees a held receiver exactly
                                // as it would after the walk.
                                if (eth_rx_pos < eth_rx_count) begin
                                    eth_skip_busy  = 1'b1;
                                    eth_skip_phase = 1'b0;
                                    eth_skip_count = 0;
                                    eth_rx_hold    = 32'd2;
                                end
                            end
                        end
                        eth_rx_on = on;
                    end
                    if (!io_write_data[7]) begin // !TESTCMDENBLN (0x0080)
                        eth_no_wakeups = io_write_data[4]; // NOWAKEUPS (0x0010)
                    end
                end else if (io_write_tioa == TIOA_ETH_DATA && io_write_task == TASK_EOT) begin
                    eth_data_writes = eth_data_writes + 64'd1;
                    if (eth_tx_count < 1024) begin
                        eth_tx_words[eth_tx_count] = io_write_data;
                        eth_tx_count               = eth_tx_count + 1;
                    end
                end
`ifndef DORADO_NO_DISK
                // 3. Disk (Trident): the disk stage (disk_tick_pend) does it
`endif
            end

            // -----------------------------------------------------------------
            // D. Fast I/O Munch Side Effects (fastio_stb)
            // -----------------------------------------------------------------
            if (fastio_stb) begin
                if (fastio_task == TASK_AWT || fastio_task == TASK_DWT) begin
                    if (fastio_kind == DM_REF_IOFETCH) begin
                        int ch;
                        ch = fastio_subtask[1] ? 1 : 0;
                        if (ddc_current_wcb_flag[ch]) begin
                            // Append the munch to the line (128 words max):
                            // the copy engine moves it a word per clock.
                            if (ddc_cur_valid[ch] && ddc_cur_word_count[ch] < 16'd128) begin
                                logic [7:0] room;
                                room     = 8'd128 - ddc_cur_word_count[ch][7:0];
                                wc_start = 1'b1;
                                wc_ch    = ch[0];
                                wc_base  = ddc_cur_word_count[ch][6:0];
                                wc_n     = (room < 8'd16) ? room[4:0] : 5'd16;
                                ddc_cur_word_count[ch] = ddc_cur_word_count[ch] + {11'd0, wc_n};
                            end
                        end else begin
                            // Raster FIFO: advance by the munch, or as far
                            // as the free space (a full FIFO drops the rest).
                            int head, tail, room;
                            head = (fastio_subtask == 2'd0) ? ddc_fifo_a_head : ddc_fifo_b_head;
                            tail = (fastio_subtask == 2'd0) ? ddc_fifo_a_tail : ddc_fifo_b_tail;
                            room = (tail - head - 1) & 63;
                            if (room > 16) room = 16;
                            if (fastio_subtask == 2'd0) ddc_fifo_a_head = (head + room) & 63;
                            else                        ddc_fifo_b_head = (head + room) & 63;
                        end
                    end
                end
`ifndef DORADO_NO_DISK
`endif
            end

            // -----------------------------------------------------------------
            // E. Periodic Timers & Wakeup Generation
            // -----------------------------------------------------------------

            // 1. Display Scanline & DWT Wakeups
            if (ddc_scanline_cd > 16'd0) begin
                ddc_scanline_cd = ddc_scanline_cd - 16'd1;
            end
            if (ddc_scanline_cd == 16'd0) begin
                ddc_scanline_cd = 16'(SCANLINE_PERIOD_CYCLES);
                if (display_wake_gate) begin
                    // Advance scanline position
                    ddc_scan_line = ddc_scan_line + 32'd1;
                    if (ddc_scan_line >= 32'(DISPLAY_H)) begin
                        ddc_scan_line   = 32'd0;
                        ddc_frame_count = ddc_frame_count + 64'd1;
                    end
                    if (!ddc_statics[0] &&
                        (ddc_terminal_task == TASK_DHT || ddc_terminal_task == TASK_AHT)) begin
                        ext_wakeup_req[ddc_terminal_task] = 1'b1;
                    end
                    if (ddc_raster_lt_enabled) begin
                        ext_wakeup_req[TASK_DHT] = 1'b1;
                    end
                end
            end

            if (display_wake_gate) begin
                logic dwt_woke;
                logic [3:0] word_task;
                // the word task the terminal's horizontal task pairs with: DWT
                // for DHT (DispY), AWT for AHT (the DispM board's Alto-terminal
                // interface, which Interlisp-D Lyric drives) -- machine.c's
                // display wake does the same
                word_task = (ddc_terminal_task == TASK_AHT) ? TASK_AWT : TASK_DWT;
                dwt_woke = 1'b0;
                for (int ch = 0; ch < 4; ch++) begin
                    if (!dwt_woke) begin
                        if (ddc_raster_next_wt_flag[ch] && !ddc_raster_current_wt_flag[ch]) begin
                            ddc_raster_next_wt_flag[ch]    = 1'b0;
                            ddc_raster_current_wt_flag[ch] = 1'b1;
                            ext_subtask_we                 = 1'b1;
                            ext_subtask_task               = word_task;
                            ext_subtask_val                = 2'(ch);
                            ext_wakeup_req[word_task]       = 1'b1;
                            dwt_woke                       = 1'b1;
                        end else if (ddc_raster_current_wt_flag[ch] &&
                                     ddc_fifo_used_fn(ch) > 0 && ddc_fifo_free_fn(ch) >= 16) begin
                            ext_subtask_we           = 1'b1;
                            ext_subtask_task         = word_task;
                            ext_subtask_val          = 2'(ch);
                            ext_wakeup_req[word_task] = 1'b1;
                            dwt_woke                 = 1'b1;
                        end
                    end
                end
                if (!dwt_woke && !ddc_statics[1]) begin
                    for (int ch = 0; ch < 2; ch++) begin
                        if (!dwt_woke && ddc_next_wcb_flag[ch] && !ddc_current_wcb_flag[ch]) begin
                            ddc_next_wcb_flag[ch]    = 1'b0;
                            ddc_current_wcb_flag[ch] = 1'b1;
                            ddc_wcb_draw_x[ch]       = 16'd0;
                            ddc_wcb_draw_y[ch]       = 16'(ddc_scan_line);
                            ext_subtask_we           = 1'b1;
                            ext_subtask_task         = word_task;
                            ext_subtask_val          = ch ? 2'd2 : 2'd0;
                            ext_wakeup_req[word_task] = 1'b1;
                            dwt_woke                 = 1'b1;
                        end
                    end
                    for (int ch = 0; ch < 2; ch++) begin
                        int st;
                        st = ch ? 2 : 0;
                        if (!dwt_woke && ddc_current_wcb_flag[ch] && ddc_fifo_free_fn(st) >= 16) begin
                            ext_subtask_we           = 1'b1;
                            ext_subtask_task         = word_task;
                            ext_subtask_val          = 2'(st);
                            ext_wakeup_req[word_task] = 1'b1;
                            dwt_woke                 = 1'b1;
                        end
                    end
                end
            end

            // 2. Ethernet Timers & Wakeups
            if (eth_rx_hold > 32'd0)       eth_rx_hold       = eth_rx_hold - 32'd1;
            if (eth_rx_wire_timer > 32'd0) eth_rx_wire_timer = eth_rx_wire_timer - 32'd1;
            if (!eth_no_wakeups) begin
                if (eth_tx_on && !eth_tx_eop && !eth_tx_cntdwn) begin
                    ext_wakeup_req[TASK_EOT] = 1'b1;
                end
                if (eth_rx_on && eth_rx_hold == 32'd0 && eth_rx_wire_timer == 32'd0 &&
                    eth_rx_pos < eth_rx_count &&
                    !(eth_rx_pos == 0 && eth_rx_count > int'(eth_world_rx_words) + 2)) begin
                    ext_wakeup_req[TASK_EIT] = 1'b1;
                end
            end

`ifndef DORADO_NO_DISK
            // 3. Disk: latch this tick's disk strobes; the disk stage handles
            // them in the clocks after the tick (io_busy holds the CPU), so the
            // disk's logic is not chained onto the rest of the tick's
            dk_rd_pop  = io_read_stb && io_read_stb_task == TASK_DSK && io_read_stb_tioa == TIOA_DISKDATA;
            dk_wr      = io_write_stb && io_write_task == TASK_DSK;
            dk_wr_tioa = io_write_tioa;
            dk_wr_data = io_write_data;
            dk_fio     = fastio_stb && fastio_task == TASK_DSK;
            dk_fio_kind = fastio_kind;
            for (int i = 0; i < 16; i++) dk_fio_munch[i] = fastio_out_munch[i];
            // the DSK wakeup stands while a TW latch is set (cheap: registers)
            if (disk_index_tw || disk_sector_tw || disk_tag_tw || disk_rd_fifo_tw || disk_wr_fifo_tw)
                ext_wakeup_req[TASK_DSK] = 1'b1;
            // the stage only when there is disk work: this tick's strobes
            // (stage 1, then 2), or a sector pulse due (stage 2 alone) --
            // otherwise the rotation timer has nothing to do this tick
            if (dk_rd_pop || dk_wr || dk_fio)
                disk_stage = 2'd1;
            else if (disk_drv_online[disk_selected_drive] && disk_enable_run &&
                     (disk_next_sector_cycle == 64'd0 || disk_time >= disk_next_sector_cycle))
                disk_stage = 2'd2;
`endif
        end
    end

endmodule
/* verilator lint_on BLKSEQ */
