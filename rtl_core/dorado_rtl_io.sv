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
(
    input  logic           clk,
    input  logic           rst,
    input  logic           clk_en,
    input  logic [63:0]    cycles,

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
    output logic           disk_sector_write_req
);
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
    logic [7:0]  fb [0:DISPLAY_FB_BYTES-1];
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
    logic [15:0] ddc_cur_words      [0:1][0:127];

    // Display FIFOs (64 words each for channel A and B)
    logic [15:0] ddc_fifo_a [0:63];
    logic [15:0] ddc_fifo_b [0:63];
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

    logic [15:0] eth_tx_words     [0:1023];
    int          eth_tx_count;
    logic [15:0] eth_rx_words     [0:4095];
    logic        eth_rx_attention [0:4095];
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
    int          disk_drv_seek_in_prog   [0:3];
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
    int          disk_fifo_head;
    int          disk_fifo_tail;
    int          disk_fifo_count;

    logic        disk_read_stream_active;
    logic        disk_read_block_framing;
    int          disk_read_stream_index;
    logic        disk_write_stream_active;
    logic        disk_write_block_framing;
    int          disk_write_stream_index;

    logic [2:0]  disk_current_block;
    logic [1:0]  disk_current_block_op;
    logic [15:0] disk_current_block_words;
    logic [15:0] disk_current_block_pos;
    logic        disk_current_block_prefix;
    logic [3:0]  disk_current_block_trailer;

    // Latched sector buffer (populated by C++ testbench or FPGA HPS disk backend)
    logic [15:0] disk_sec_hdr  [0:1];
    logic [15:0] disk_sec_lbl  [0:9];
    logic [15:0] disk_sec_data [0:1023];
    logic        disk_sec_valid;
    logic        disk_sec_modified;

    logic [63:0] disk_next_sector_cycle;
    logic [31:0] disk_cycles_per_rev;

    assign disk_selected_drive_out = disk_selected_drive;
    assign disk_cur_cyl_out        = disk_drv_cur_cyl[disk_selected_drive];
    assign disk_cur_head_out       = disk_drv_cur_head[disk_selected_drive];
    assign disk_cur_sector_out     = disk_drv_cur_sector[disk_selected_drive];

    // =========================================================================
    // 5. Helper Functions & Tasks
    // =========================================================================

    // Set pixel in 1024x808 1-bpp framebuffer
    task automatic ddc_set_pixel(input int x, input int y, input logic pix);
        int byte_idx;
        int bit_idx;
        if (x < 0 || x >= DISPLAY_W || y < 0 || y >= DISPLAY_H) return;
        byte_idx = y * DISPLAY_ROW_BYTES + (x >> 3);
        bit_idx  = 7 - (x & 7);
        if (pix) fb[byte_idx] = fb[byte_idx] | (8'd1 << bit_idx);
        else     fb[byte_idx] = fb[byte_idx] & ~(8'd1 << bit_idx);
    endtask

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

    task automatic ddc_render_line_task(input int ch);
        int c, width_pixels, y, x, visible_words, start_off, line_pixels, remaining;
        logic [15:0] word;
        c = ch & 1;
        if (!ddc_cur_valid[c] || ddc_cur_word_count[c] == 16'd0) return;

        width_pixels = ddc_decode_width_pixels_fn(ddc_cur_width[c]);
        if (width_pixels <= 0) return;

        y = ddc_cur_abs_y[c] ? int'(ddc_cur_y[c])
                             : (int'(ddc_cur_line[c]) * 2 + int'(ddc_cur_odd[c]));
        if (y < 0 || y >= DISPLAY_H) return;

        x             = 0;
        visible_words = (width_pixels + 15) / 16;
        start_off     = int'(ddc_cur_ptr[c] & 16'o17);
        line_pixels   = 0;
        ddc_lines_rendered = ddc_lines_rendered + 64'd1;

        for (int px = 0; px < width_pixels && px < DISPLAY_W; px++) begin
            ddc_set_pixel(px, y, 1'b0);
        end

        for (int i = 0; i < visible_words && x < DISPLAY_W; i++) begin
            int src;
            src  = start_off + i;
            word = (src < int'(ddc_cur_word_count[c]) && src < 128) ? ddc_cur_words[c][src] : 16'd0;
            remaining = width_pixels - (i * 16);
            if (remaining > 16) remaining = 16;
            for (int b = 0; b < remaining && x < DISPLAY_W; b++, x++) begin
                logic bit_val;
                bit_val = word[15 - b];
                ddc_set_pixel(x, y, bit_val);
                if (bit_val) line_pixels++;
            end
        end
        ddc_pixels_rendered = ddc_pixels_rendered + 64'(line_pixels);
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
                w = 0;
                while (w < 5 && !ddc_kbd_pending[w]) w++;
                if (w >= 5) w = 0;
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
            return;
        end

        if (ddc_term_msg_bit == 8'd0) begin
            ddc_term_msg_active = 1'b1;
            if (ddc_kbd_pending != 5'd0) begin
                int w;
                w = 0;
                while (w < 5 && !ddc_kbd_pending[w]) w++;
                if (w >= 5) w = 0;
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
    endtask

    // =========================================================================
    // 6. Disk Controller Helper Functions & Tasks
    // =========================================================================
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
            8'o012: return !(disk_active && disk_current_block_op == 2'd1); // !Read
            8'o013: return !(disk_active && disk_current_block_op == 2'd3); // !Write
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

    function automatic logic [15:0] disk_block_word_fn(
        input logic [1:0] drv,
        input int         blk,
        input int         pos
    );
        if (pos < 0) return 16'd0;
        case (blk)
            0: return (pos < int'(disk_drv_hdr_words[drv]) && pos < 2)     ? disk_sec_hdr[pos]  : 16'd0;
            1: return (pos < int'(disk_drv_lbl_words[drv]) && pos < 10)    ? disk_sec_lbl[pos]  : 16'd0;
            2: return (pos < int'(disk_drv_data_words[drv]) && pos < 1024) ? disk_sec_data[pos] : 16'd0;
            default: return 16'd0;
        endcase
    endfunction

    function automatic logic [15:0] disk_sector_word_fn(
        input logic [1:0] drv,
        input int         idx_in
    );
        int hw, lw, dw, idx;
        hw  = int'(disk_drv_hdr_words[drv]);
        lw  = int'(disk_drv_lbl_words[drv]);
        dw  = int'(disk_drv_data_words[drv]);
        idx = idx_in;
        if (idx < hw) return (idx < 2) ? disk_sec_hdr[idx] : 16'd0;
        idx -= hw;
        if (idx < lw) return (idx < 10) ? disk_sec_lbl[idx] : 16'd0;
        idx -= lw;
        if (idx < dw) return (idx < 1024) ? disk_sec_data[idx] : 16'd0;
        return 16'd0;
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
        for (int b = 0; b < blk; b++) begin
            start_idx += disk_block_default_words_fn(drv, b);
        end
        disk_read_stream_active    = 1'b0;
        disk_rd_fifo_tw            = 1'b0;
        disk_write_stream_active   = 1'b1;
        disk_write_block_framing   = 1'b1;
        disk_write_stream_index    = start_idx;
        disk_current_block         = 3'(blk);
        disk_current_block_op      = 2'd3; // OP_WRITE
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
        started = 1'b0;
        while (disk_current_block < 3'd4) begin
            logic [1:0] op;
            op = disk_control_block_op_fn(disk_control, int'(disk_current_block));
            if (op == 2'd0) begin
                disk_current_block = disk_current_block + 3'd1;
                continue;
            end
            if (op == 2'd3) begin
                disk_arm_write_block_task(drv, int'(disk_current_block));
                started = 1'b1;
                return;
            end
            if (op == 2'd1 || op == 2'd2) begin
                disk_write_stream_active   = 1'b0;
                disk_read_stream_active    = 1'b1;
                disk_current_block_op      = op;
                disk_current_block_words   = disk_format_block_words_fn(drv, int'(disk_current_block));
                disk_current_block_pos     = 16'd0;
                disk_current_block_prefix  = 1'b0;
                disk_current_block_trailer = 4'd0;
                if (op == 2'd2) disk_compare_err = 1'b1;
                disk_update_fifo_tws_task();
                started = 1'b1;
                return;
            end
            disk_current_block = disk_current_block + 3'd1;
        end
        disk_read_stream_active    = 1'b0;
        disk_write_stream_active   = 1'b0;
        disk_current_block_op      = 2'd0;
        disk_current_block_words   = 16'd0;
        disk_current_block_pos     = 16'd0;
        disk_current_block_prefix  = 1'b0;
        disk_current_block_trailer = 4'd0;
    endtask

    task automatic disk_refill_fifo_task();
        logic [1:0] drv;
        int         total;
        logic       sequenced;
        if (!disk_read_stream_active) return;
        drv = disk_selected_drive;
        if (!disk_drv_online[drv] || !disk_sec_valid) begin
            disk_read_stream_active = 1'b0;
            return;
        end

        total = int'(disk_drv_hdr_words[drv]) + int'(disk_drv_lbl_words[drv]) +
                int'(disk_drv_data_words[drv]);
        if (disk_read_block_framing) total += 12; // 3 * 4 trailer words
        sequenced = disk_read_block_framing;

        for (int iter = 0; iter < 32; iter++) begin
            logic [15:0] w;
            if (disk_fifo_count >= DISK_FIFO_WORDS) break;
            w = 16'd0;
            if (sequenced) begin
                if (disk_current_block_op == 2'd0) break;
                if (disk_current_block_pos < disk_current_block_words) begin
                    w = disk_block_word_fn(drv, int'(disk_current_block), int'(disk_current_block_pos));
                    if (disk_restore_hdr_check &&
                        disk_current_block == 3'd0 &&
                        disk_current_block_op == 2'd2 &&
                        disk_current_block_pos == 16'd0) begin
                        w |= 16'h0001;
                    end
                    disk_current_block_pos = disk_current_block_pos + 16'd1;
                end else if (disk_current_block_trailer < 4'd4) begin
                    w = 16'd0;
                    disk_current_block_trailer = disk_current_block_trailer + 4'd1;
                end else begin
                    logic blk_started;
                    if (disk_fifo_count > 0) break;
                    disk_current_block = disk_current_block + 3'd1;
                    if (disk_current_block < 3'd4) begin
                        disk_start_current_block_task(drv, blk_started);
                        if (blk_started) continue;
                    end
                    disk_read_stream_active    = 1'b0;
                    disk_current_block_op      = 2'd0;
                    disk_current_block_words   = 16'd0;
                    disk_current_block_pos     = 16'd0;
                    disk_current_block_prefix  = 1'b0;
                    disk_current_block_trailer = 4'd0;
                    break;
                end
            end else begin
                if (disk_read_stream_index >= total) break;
                w = disk_sector_word_fn(drv, disk_read_stream_index);
            end
            disk_fifo[disk_fifo_head] = w;
            disk_fifo_head            = (disk_fifo_head + 1) % DISK_FIFO_WORDS;
            disk_fifo_count           = disk_fifo_count + 1;
            disk_read_stream_index    = disk_read_stream_index + 1;
        end

        disk_update_fifo_tws_task();
        if ((disk_read_stream_index >= total ||
             (sequenced && disk_current_block_op == 2'd0)) &&
            disk_fifo_count == 0) begin
            disk_read_stream_active = 1'b0;
            disk_active             = 1'b0;
            disk_rd_fifo_tw         = 1'b0;
            disk_tag_tw             = 1'b1;
        end
    endtask

    task automatic disk_begin_read_stream_task(output logic ok);
        logic [1:0] drv;
        logic       dummy_started;
        drv = disk_selected_drive;
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
            if (!disk_write_block_framing) begin
                if (idx < hw && (hw - 1 - idx) < 2) begin
                    disk_sec_hdr[hw - 1 - idx] = w;
                end else if (idx < hw + lw && (lw - 1 - (idx - hw)) < 10) begin
                    disk_sec_lbl[lw - 1 - (idx - hw)] = w;
                end else if (idx < total && (dw - 1 - (idx - hw - lw)) < 1024) begin
                    disk_sec_data[dw - 1 - (idx - hw - lw)] = w;
                end
            end else if (disk_current_block == 3'd0 && pos < hw && pos < 2) begin
                disk_sec_hdr[pos] = w;
            end else if (disk_current_block == 3'd1 && pos < lw && pos < 10) begin
                disk_sec_lbl[pos] = w;
            end else if (disk_current_block == 3'd2 && pos < dw && pos < 1024) begin
                disk_sec_data[pos] = w;
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
                disk_update_fifo_tws_task();
            end
        end
    endtask

    task automatic disk_start_pending_transfer_task();
        logic [1:0] drv;
        logic       has_op, has_read_or_chk, has_write;
        drv = disk_selected_drive;
        has_op          = (disk_control[7:0] != 8'd0);
        has_read_or_chk = (disk_control[7:6] == 2'd1 || disk_control[7:6] == 2'd2 ||
                           disk_control[5:4] == 2'd1 || disk_control[5:4] == 2'd2 ||
                           disk_control[3:2] == 2'd1 || disk_control[3:2] == 2'd2 ||
                           disk_control[1:0] == 2'd1 || disk_control[1:0] == 2'd2);
        has_write       = (disk_control[7:6] == 2'd3 || disk_control[5:4] == 2'd3 ||
                           disk_control[3:2] == 2'd3 || disk_control[1:0] == 2'd3);

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
                io_read_data = 16'h0000;
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
                    io_read_data       = eth_rx_words[eth_rx_pos & 4095];
                end
            end
        end
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
    end

    // Combinational IOAtten' (sampled at instruction issue when ctask != 0)
    always_comb begin
        io_atten = 1'b0;
        if (io_read_task == TASK_EIT && io_read_tioa == TIOA_ETH_DATA) begin
            if (eth_rx_hold == 32'd0 && eth_rx_pos < eth_rx_count &&
                eth_rx_attention[eth_rx_pos & 4095]) begin
                io_atten = 1'b1;
            end
        end
    end

    // Combinational DMux status for Disk (02000..02037)
    always_comb begin
        dmux_ext_handled = 1'b0;
        dmux_ext_val     = 16'h0000;
        if (dmux_cur_addr >= 12'o2000 && dmux_cur_addr <= 12'o2037) begin
            dmux_ext_handled = 1'b1;
            dmux_ext_val     = disk_muffler_bit_fn(8'(dmux_cur_addr - 12'o2000)) ? 16'h8000 : 16'h0000;
        end
    end

    // Combinational Fast I/O input munch (for DM_REF_IOSTORE on TASK_DSK)
    always_comb begin
        for (int i = 0; i < 16; i++) begin
            if (fastio_task == TASK_DSK && i < disk_fifo_count) begin
                fastio_in_munch[i] = disk_fifo[(disk_fifo_tail + i) % DISK_FIFO_WORDS];
            end else begin
                fastio_in_munch[i] = 16'd0;
            end
        end
    end

    // =========================================================================
    // 8. Main Synchronous State Machine (clocked on negedge clk so CPU's
    //    posedge clk outputs io_read_stb / io_write_stb / fastio_stb are
    //    processed in the second half of the same microinstruction cycle)
    // =========================================================================
    always @(negedge clk) begin
        if (rst) begin
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
            ddc_nlcb_cursor_lo      = 8'd0;
            ddc_scan_line           = 32'd0;
            ddc_scan_pixel          = 32'd0;
            ddc_frame_count         = 64'd0;
            ddc_pixels_rendered     = 64'd0;
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
            disk_next_sector_cycle     = 64'd0;
            disk_cycles_per_rev        = 32'd277777;
            disk_sector_read_req       = 1'b0;
            disk_sector_write_req      = 1'b0;

            ext_wakeup_req             = 16'd0;
            ext_subtask_we             = 1'b0;
            ext_subtask_task           = 4'd0;
            ext_subtask_val            = 2'd0;
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
                    ddc_kbd_pending[i] = 1'b1;
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
                    io_read_stb_tioa == TIOA_TSTATUS) begin
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
                // 3. Disk FIFO pop
                else if (io_read_stb_task == TASK_DSK && io_read_stb_tioa == TIOA_DISKDATA) begin
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
                            ddc_render_line_task(ch);
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
                            4'o16: begin // CursorLo
                                ddc_nlcb_cursor_lo = val[7:0];
                                ddc_nlcb_line      = ddc_nlcb_line + 16'd1;
                            end
                            4'o17: begin // CursorHi
                                logic [15:0] row_bits;
                                row_bits = {val[7:0], ddc_nlcb_cursor_lo};
                                if (row_bits != 16'd0) begin
                                    int x, y;
                                    x = 3741 - int'(ddc_nlcb_cursor_x); // CURSOR_X_BIAS = 07235
                                    y = (int'(ddc_nlcb_line) - 1) * 2 + int'(ddc_nlcb_field_odd);
                                    for (int b2 = 0; b2 < 16; b2++) begin
                                        if (row_bits[15 - b2]) begin
                                            ddc_set_pixel(x + b2, y, 1'b1);
                                        end
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
                                int skipped;
                                skipped = 0;
                                while (eth_rx_pos < eth_rx_count &&
                                       !eth_rx_attention[eth_rx_pos & 4095]) begin
                                    eth_rx_pos = eth_rx_pos + 1;
                                    skipped    = skipped + 1;
                                end
                                if (eth_rx_pos < eth_rx_count) begin
                                    eth_rx_pos = eth_rx_pos + 1;
                                    skipped    = skipped + 1;
                                end
                                eth_rx_hold = 32'(skipped * 170);
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
                // 3. Disk (Trident)
                else if (io_write_task == TASK_DSK) begin
                    case (io_write_tioa)
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
                                disk_control = io_write_data;
                                if (io_write_data[7:0] != 8'd0) begin
                                    disk_xfer_pending     = 1'b1;
                                    disk_transfer_restore = disk_restore_pending;
                                    disk_restore_pending  = 1'b0;
                                end
                                disk_format_ram_addr = 4'd0;
                                if (io_write_data[10]) begin // CLR_ENABLE_RUN (0x0400)
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
                                if (io_write_data[9]) disk_debug_mode       = 1'b1; // SET_DEBUG_MODE (0x0200)
                                if (io_write_data[8]) disk_block_till_index = 1'b1; // BLOCK_TILL_INDEX (0x0100)
                                if (io_write_data[9] && (io_write_data[7:0] != 8'd0)) begin
                                    disk_current_block    = 3'd0;
                                    disk_current_block_op = (io_write_data[7:6] != 2'd0) ? io_write_data[7:6] :
                                                            (io_write_data[5:4] != 2'd0) ? io_write_data[5:4] :
                                                            (io_write_data[3:2] != 2'd0) ? io_write_data[3:2] :
                                                                                           io_write_data[1:0];
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
                            disk_muff_addr = io_write_data[7:0];
                            if (io_write_data[11]) disk_index_tw    = 1'b0; // CLEAR_INDEX_TW (0x0800)
                            if (io_write_data[10]) disk_sector_tw   = 1'b0; // CLEAR_SECTOR_TW (0x0400)
                            if (io_write_data[9])  disk_tag_tw      = 1'b0; // CLEAR_SEEKTAG_TW (0x0200)
                            if (io_write_data[13]) disk_compare_err = 1'b0; // CLEAR_COMPARE_ERR (0x2000)
                            if (io_write_data[12]) disk_read_data_err = 1'b1; // SET_CHECKSUM_ERR (0x1000)
                            if (io_write_data[8]) begin // CLEAR_ERRORS (0x0100)
                                disk_compare_err    = 1'b0;
                                disk_read_data_err  = 1'b0;
                                disk_fifo_underflow = 1'b0;
                                disk_fifo_overflow  = 1'b0;
                            end
                        end

                        TIOA_DISKDATA: begin
                            if (disk_write_stream_active) begin
                                disk_write_stream_word_task(io_write_data);
                                disk_update_fifo_tws_task();
                            end else if (disk_fifo_count < DISK_FIFO_WORDS) begin
                                disk_fifo[disk_fifo_head] = io_write_data;
                                disk_fifo_head            = (disk_fifo_head + 1) % DISK_FIFO_WORDS;
                                disk_fifo_count           = disk_fifo_count + 1;
                                disk_update_fifo_tws_task();
                            end else begin
                                disk_fifo_overflow = 1'b1;
                            end
                        end

                        TIOA_DISKRAM: begin
                            disk_format_ram[disk_format_ram_addr] = io_write_data;
                            disk_format_ram_writes                = disk_format_ram_writes + 1;
                            if (disk_format_ram_addr == 4'd15) begin
                                disk_enable_run = 1'b1;
                            end
                            disk_format_ram_addr = disk_format_ram_addr + 4'd1;
                        end

                        TIOA_DISKTAG: begin
                            logic [11:0] bus;
                            logic [1:0]  drv;
                            disk_tag = io_write_data;
                            bus      = io_write_data[11:0];
                            drv      = disk_selected_drive;

                            if (io_write_data[15]) begin // TAG_DRIVE (0x8000)
                                if (bus[5]) begin
                                    int cnt, div_val, spr;
                                    cnt = int'(bus[9:6]);
                                    disk_drv_subsector_cnt[drv] = 6'(cnt);
                                    div_val = cnt + 1;
                                    spr     = (117 + div_val - 1) / div_val;
                                    if (spr <= 0) spr = 1;
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
                            end else if (io_write_data[13]) begin // TAG_HEAD (0x2000)
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
                            end else if (io_write_data[14]) begin // TAG_CYLINDER (0x4000)
                                if (disk_drv_online[drv]) begin
                                    disk_drv_cur_cyl[drv]      = bus;
                                    disk_drv_cur_sector[drv]   = 7'd0;
                                    disk_drv_seek_in_prog[drv] = int'(disk_drv_sectors_per_rev[drv]);
                                    disk_sector_read_req       = 1'b1;
                                end
                                disk_tag_tw = 1'b1;
                            end else if (io_write_data[12]) begin // TAG_CONTROL (0x1000)
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
                                        disk_write_stream_active   = 1'b1;
                                        disk_write_block_framing   = 1'b0;
                                        disk_write_stream_index    = 0;
                                        disk_current_block         = 3'd0;
                                        disk_current_block_op      = 2'd3;
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
            end

            // -----------------------------------------------------------------
            // D. Fast I/O Munch Side Effects (fastio_stb)
            // -----------------------------------------------------------------
            if (fastio_stb) begin
                if (fastio_task == TASK_AWT || fastio_task == TASK_DWT) begin
                    if (fastio_kind == DM_REF_IOFETCH) begin
                        int ch;
                        ch = fastio_subtask[1] ? 1 : 0;
                        for (int i = 0; i < 16; i++) begin
                            if (ddc_current_wcb_flag[ch]) begin
                                if (ddc_cur_valid[ch] && ddc_cur_word_count[ch] < 16'd128) begin
                                    ddc_cur_words[ch][ddc_cur_word_count[ch][6:0]] = fastio_out_munch[i];
                                    ddc_cur_word_count[ch] = ddc_cur_word_count[ch] + 16'd1;
                                end
                            end else begin
                                if (fastio_subtask == 2'd0) begin
                                    int next_h;
                                    next_h = (ddc_fifo_a_head + 1) % 64;
                                    if (next_h == ddc_fifo_a_tail) break;
                                    ddc_fifo_a[ddc_fifo_a_head] = fastio_out_munch[i];
                                    ddc_fifo_a_head             = next_h;
                                end else begin
                                    int next_h;
                                    next_h = (ddc_fifo_b_head + 1) % 64;
                                    if (next_h == ddc_fifo_b_tail) break;
                                    ddc_fifo_b[ddc_fifo_b_head] = fastio_out_munch[i];
                                    ddc_fifo_b_head             = next_h;
                                end
                            end
                        end
                    end
                end else if (fastio_task == TASK_DSK) begin
                    if (fastio_kind == DM_REF_IOFETCH) begin
                        for (int i = 0; i < 16; i++) begin
                            if (disk_fifo_count >= DISK_FIFO_WORDS) break;
                            disk_fifo[disk_fifo_head] = fastio_out_munch[i];
                            disk_fifo_head            = (disk_fifo_head + 1) % DISK_FIFO_WORDS;
                            disk_fifo_count           = disk_fifo_count + 1;
                        end
                        disk_update_fifo_tws_task();
                    end else if (fastio_kind == DM_REF_IOSTORE) begin
                        for (int i = 0; i < 16; i++) begin
                            if (disk_fifo_count > 0) begin
                                disk_fifo_tail  = (disk_fifo_tail + 1) % DISK_FIFO_WORDS;
                                disk_fifo_count = disk_fifo_count - 1;
                                disk_refill_fifo_task();
                            end
                        end
                        disk_update_fifo_tws_task();
                    end
                end
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
                dwt_woke = 1'b0;
                for (int ch = 0; ch < 4; ch++) begin
                    if (!dwt_woke) begin
                        if (ddc_raster_next_wt_flag[ch] && !ddc_raster_current_wt_flag[ch]) begin
                            ddc_raster_next_wt_flag[ch]    = 1'b0;
                            ddc_raster_current_wt_flag[ch] = 1'b1;
                            ext_subtask_we                 = 1'b1;
                            ext_subtask_task               = TASK_DWT;
                            ext_subtask_val                = 2'(ch);
                            ext_wakeup_req[TASK_DWT]       = 1'b1;
                            dwt_woke                       = 1'b1;
                        end else if (ddc_raster_current_wt_flag[ch] &&
                                     ddc_fifo_used_fn(ch) > 0 && ddc_fifo_free_fn(ch) >= 16) begin
                            ext_subtask_we           = 1'b1;
                            ext_subtask_task         = TASK_DWT;
                            ext_subtask_val          = 2'(ch);
                            ext_wakeup_req[TASK_DWT] = 1'b1;
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
                            ext_subtask_task         = TASK_DWT;
                            ext_subtask_val          = ch ? 2'd2 : 2'd0;
                            ext_wakeup_req[TASK_DWT] = 1'b1;
                            dwt_woke                 = 1'b1;
                        end
                    end
                    for (int ch = 0; ch < 2; ch++) begin
                        int st;
                        st = ch ? 2 : 0;
                        if (!dwt_woke && ddc_current_wcb_flag[ch] && ddc_fifo_free_fn(st) >= 16) begin
                            ext_subtask_we           = 1'b1;
                            ext_subtask_task         = TASK_DWT;
                            ext_subtask_val          = 2'(st);
                            ext_wakeup_req[TASK_DWT] = 1'b1;
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

            // 3. Disk Rotation Timer & Wakeups
            begin
                logic [1:0] drv;
                drv = disk_selected_drive;
                if (disk_drv_online[drv] && disk_enable_run) begin
                    int spr;
                    logic [63:0] cps;
                    spr = int'(disk_drv_sectors_per_rev[drv]);
                    if (spr <= 0) spr = 1;
                    cps = {32'd0, disk_cycles_per_rev} / 64'(spr);
                    if (cps == 64'd0) cps = 64'd1;

                    if (disk_next_sector_cycle == 64'd0) begin
                        disk_next_sector_cycle = cycles + cps;
                    end else if (cycles >= disk_next_sector_cycle) begin
                        logic at_index;
                        disk_drv_cur_sector[drv] = 7'((int'(disk_drv_cur_sector[drv]) + 1) % spr);
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
        end
    end

endmodule
/* verilator lint_on BLKSEQ */
