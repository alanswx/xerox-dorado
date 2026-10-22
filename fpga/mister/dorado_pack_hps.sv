// File-backed T-80 medium for the Sil-derived disk controller.
// One 16 KB HPS window contains a complete 29-sector track, including the
// neighboring bytes needed to write an unaligned track back without damage.
// The machine pauses during host transfers; HPS and HDMI clocks keep running.
module dorado_pack_hps #(
    parameter integer SNAPSHOT_PAUSE = 0,
    // Experimental host write reduction. Keep the deployed whole-track
    // contract until the windowed build has passed native timing/boot gates.
    parameter integer DIRTY_BLOCK_WINDOW = 0,
    parameter integer SECTORS_PER_TRACK = 29,
    parameter integer LABEL_WORDS = 8,
    parameter [11:0] INITIAL_CYL = 0,
    parameter [5:0] INITIAL_HEAD = 0,
    parameter integer PRE_WORDS = 32,
    parameter integer GAP_WORDS = 11,
`ifdef DORADO_HOST_CLOCK_HZ
    parameter integer HOST_CLOCK_HZ = `DORADO_HOST_CLOCK_HZ,
`else
    parameter integer HOST_CLOCK_HZ = 33333333,
`endif
    // This bounds ARM service in WALL time, independent of guest speed or
    // pauses. build.sh derives HOST_CLOCK_HZ from the very same PLL setting.
    parameter integer HOST_TIMEOUT_CLOCKS = 4 * HOST_CLOCK_HZ
) (
    input wire clk_sys, machine_clk, reset, attached, readonly, remounted,
    input wire snapshot_hold, snapshot_guest_held,
    output wire snapshot_idle, snapshot_held,
    input wire [11:0] tagbus_n,
    input wire cyltag_n, headtag_n, drivetag_n, conttag_n,
    input wire selected, data_adv,
    input wire [15:0] pulse,
    input wire write_data_p, write_data_m,
    output reg data_bit = 0,
    output wire write_gate, endofcyl_n, ready, dirty,
    output wire fault,
    output wire [2:0] host_phase,
    output wire [31:0] fault_debug, fault_extra,
    output reg [31:0] sd_lba = 0,
    output reg [5:0] sd_blk_cnt = 0,
    output reg sd_rd = 0, sd_wr = 0,
    input wire sd_ack,
    input wire [12:0] sd_buff_addr,
    input wire [15:0] sd_buff_dout,
    input wire sd_buff_wr,
    output reg [15:0] sd_buff_din,
    output reg [31:0] reads = 0, writes = 0
);
  localparam integer SECTOR_WORDS = 1+2+LABEL_WORDS+256;
  localparam integer TRACK_WORDS = SECTORS_PER_TRACK*SECTOR_WORDS;
  localparam [31:0] TAPS = 32'h00a00805;
  reg serial_fault = 0, host_fault = 0;
  // Passive first-fault observation. Never changes readiness or I/O state.
  reg [31:0] serial_debug=0,serial_extra=0,host_debug=0,host_extra=0;
  assign fault = serial_fault | host_fault;
  assign fault_debug = serial_fault ? serial_debug : host_debug;
  assign fault_extra = serial_fault ? serial_extra : host_extra;
  function automatic [31:0] fault_status(input [3:0] reason);
    fault_status={reason,cyl,head,wr_block,wr_state,wr_bits};
  endfunction
  function automatic [31:0] host_fault_status(input [3:0] reason);
    // Host faults need the ACK phase, not the serial bit count. Preserve
    // cylinder/head/block/serial-state and keep the requested LBA in extra.
    host_fault_status={reason,cyl,head,wr_block,wr_state,host_state,sd_rd,sd_wr,sd_ack};
  endfunction
  reg [11:0] cyl = INITIAL_CYL;
  reg [5:0] head = INITIAL_HEAD;
  reg eoc = 0;
  reg cyltag_d=1, headtag_d=1, conttag_d=1;
  wire [5:0] tag_head = ~tagbus_n[5:0];
  assign endofcyl_n = ~(attached && eoc);
  assign write_gate = attached && selected && !conttag_n && !tagbus_n[7] && !tagbus_n[2];
  always @(posedge machine_clk) begin
    cyltag_d <= cyltag_n; headtag_d <= headtag_n; conttag_d <= conttag_n;
    if (cyltag_d && !cyltag_n) cyl <= ~tagbus_n;
    if (headtag_d && !headtag_n) begin head <= tag_head; eoc <= tag_head >= 5; end
    if (conttag_d && !conttag_n) begin
      if (!tagbus_n[3] || !tagbus_n[1]) eoc <= 0;
      if (!tagbus_n[1]) begin cyl <= 0; head <= 0; end
      else if (!tagbus_n[0]) begin head <= head+1'b1; eoc <= head+1'b1 >= 5; end
    end
  end

  // Both ports are synchronous and share their own read/write address, the
  // Cyclone V true-dual-port RAM template. No asynchronous 16 KB RAM mux.
  // A port's read value is unused while that port writes. HPS transfers
  // pause the machine, so no consumer relies on mixed-port collisions.
  (* ramstyle = "no_rw_check" *) reg [15:0] buffer [0:8191];
  reg [15:0] word_q;
  reg store = 0;
  reg [12:0] store_addr = 0;
  reg [15:0] store_data = 0;
  wire [12:0] read_addr;
  wire [12:0] disk_addr = store ? store_addr : read_addr;
  wire snapshot_block;
  reg [4:0] dirty_first_block = 0, dirty_last_block = 0;
  reg [12:0] host_write_base = 0;
  wire [12:0] host_buffer_addr = sd_buff_addr +
      (((DIRTY_BLOCK_WINDOW!=0) && (host_state==1 || host_state==2)) ? host_write_base : 13'd0);
  always @(posedge clk_sys) begin
    if (!snapshot_block) begin
    sd_buff_din <= buffer[host_buffer_addr];
    if (sd_buff_wr) buffer[host_buffer_addr] <= sd_buff_dout;
    end
  end
  always @(posedge machine_clk) begin
    word_q <= buffer[disk_addr];
    if (store) buffer[disk_addr] <= store_data;
  end

  reg valid = 0;
  reg [11:0] cached_cyl = 0;
  reg [5:0] cached_head = 0;
  reg [31:0] cached_lba = 0;
  reg [7:0] first_word = 0;
  reg [5:0] cached_count = 0;
  reg [31:0] generation = 0, flushed_generation = 0;
  assign dirty = generation != flushed_generation;
  wire need_track = !valid || cyl != cached_cyl || head != cached_head;
  reg [2:0] host_state = 0;
  assign host_phase = host_state;
  reg [31:0] host_wait = 0;
  // Illegal-head probes only need EndOfCyl; keep the CPU running for them.
  assign ready = attached && !remounted && !fault && host_state == 0 && (!need_track || head >= 5);
  wire [31:0] target_word = (cyl*5 + head)*TRACK_WORDS;
  // Complete already-issued HPS work before leasing the ARM image. Partial
  // serial blocks, pending guest-port stores and dirty buffer generations
  // remain machine state; never finish a guest write or flush partial data.
  reg snapshot_paused = 0;
  reg snapshot_attached = 0;
  wire snapshot_media_known = (attached===1'b1) ||
      ((attached===1'b0) && (valid===1'b0) && (dirty===1'b0));
  wire snapshot_quiet = (snapshot_guest_held===1'b1) &&
      (reset===1'b0) && snapshot_media_known && (remounted===1'b0) &&
      (fault===1'b0) && (host_state===3'd0) &&
      (sd_rd===1'b0) && (sd_wr===1'b0) && (sd_ack===1'b0) && (sd_buff_wr===1'b0);
  wire snapshot_enter = (SNAPSHOT_PAUSE!=0) && (snapshot_hold===1'b1) && snapshot_quiet;
  assign snapshot_block = (SNAPSHOT_PAUSE!=0) && (snapshot_paused || snapshot_enter);
  assign snapshot_held = (SNAPSHOT_PAUSE!=0) && snapshot_paused;
  assign snapshot_idle = snapshot_held && (snapshot_hold===1'b1) && snapshot_quiet &&
      (attached===snapshot_attached);
  always @(posedge clk_sys) begin
    if (reset) begin snapshot_paused <= 0; snapshot_attached <= 0; end
    else if (snapshot_paused) begin
      if ((snapshot_hold===1'b0) && (sd_ack===1'b0) && (sd_buff_wr===1'b0)) snapshot_paused <= 0;
    end else if (snapshot_enter) begin snapshot_paused <= 1; snapshot_attached <= attached; end
  end
  always @(posedge clk_sys) begin
    if (reset) begin
      valid <= 0; host_state <= 0; sd_rd <= 0; sd_wr <= 0;
      flushed_generation <= generation; host_fault <= 0; host_wait <= 0;
      host_debug <= 0; host_extra <= 0;
    end else if (!attached || remounted) begin
      // Main has already switched/closed the image. Never write an old
      // dirty buffer into the replacement image or silently mark it clean.
      if ((valid && dirty) || host_state != 0) begin
        host_fault <= 1;
        if (!host_fault) begin host_debug<=host_fault_status(5);host_extra<=sd_lba;end
      end
      valid <= 0; host_state <= 0; sd_rd <= 0; sd_wr <= 0; host_wait <= 0;
    end else if (!snapshot_block) begin
      if (host_state == 0) host_wait <= 0;
      else host_wait <= host_wait+1'b1;
      case (host_state)
      0: if (!fault) begin
        if (valid && dirty &&
            ((need_track && head < 5) || (!write_gate && wr_state == 0 && !store))) begin
          if (readonly) begin
            host_fault <= 1;
            if (!host_fault) begin host_debug<=host_fault_status(6);host_extra<=cached_lba;end
          end else if (wr_state != 0 || store) begin
            // A seek that interrupts a serial block cannot make partial
            // buffer data eligible for writeback.
            host_fault <= 1;
            if (!host_fault) begin host_debug<=host_fault_status(7);host_extra<=cached_lba;end
          end else begin
            sd_lba <= cached_lba + ((DIRTY_BLOCK_WINDOW!=0) ? {27'd0,dirty_first_block} : 32'd0);
            sd_blk_cnt <= (DIRTY_BLOCK_WINDOW!=0) ? {1'b0,dirty_last_block}-{1'b0,dirty_first_block} : cached_count;
            host_write_base <= {dirty_first_block,8'd0};
            sd_wr <= 1; host_state <= 1;
          end
        end else if (need_track && head < 5) begin
          cached_lba <= target_word >> 8;
          cached_cyl <= cyl; cached_head <= head;
          first_word <= target_word[7:0];
          cached_count <= (target_word[7:0] + TRACK_WORDS - 1) >> 8;
          sd_lba <= target_word >> 8;
          sd_blk_cnt <= (target_word[7:0] + TRACK_WORDS - 1) >> 8;
          sd_rd <= 1; host_state <= 3;
        end
      end
      1: if (sd_ack) begin sd_wr <= 0; host_state <= 2; end
      2: if (!sd_ack) begin
        flushed_generation <= generation; writes <= writes+1'b1;
        host_state <= 0;
      end
      3: if (sd_ack) begin sd_rd <= 0; host_state <= 4; end
      4: if (!sd_ack) begin valid <= 1; reads <= reads+1'b1; host_state <= 0; end
      default: ;
      endcase
      // No completion is invented when the host disappears or stalls.
      if (host_state != 0 && host_wait >= HOST_TIMEOUT_CLOCKS-1) begin
        host_fault <= 1; sd_rd <= 0; sd_wr <= 0; host_state <= 0;
        if (!host_fault) begin host_debug<=host_fault_status(8);host_extra<=sd_lba;end
      end
    end
  end

  function automatic [31:0] sync_ecc;
    reg [31:0] r;
    reg [15:0] s;
    integer k;
    begin
      r=0; s=16'o201;
      for(k=15;k>=0;k=k-1) r={r[30:0],1'b0} ^ ((s[k]^r[31]) ? TAPS : 32'd0);
      sync_ecc=r;
    end
  endfunction
  function automatic [8:0] block_words(input [1:0] b);
    block_words = b==0 ? 2 : b==1 ? LABEL_WORDS : 256;
  endfunction
  function automatic [3:0] block_base(input [1:0] b);
    block_base = b==0 ? 1 : b==1 ? 3 : 3+LABEL_WORDS;
  endfunction

  localparam IDLE=0, PRE=1, SYNC=2, WORDS=3, ECC=4, GAP=5;
  reg [2:0] phase = IDLE;
  reg [1:0] blk = 0;
  reg [12:0] cnt = 0;
  reg [8:0] wi = 0;
  reg [3:0] bi = 15;
  reg [4:0] sec = 0;
  reg [15:0] pulse_d = 16'hffff, sync_word = 0;
  reg [31:0] ecc = 0;
  reg [31:0] next_ecc;
  // Prefetch while the last bit of the current word is on the cable.
  assign read_addr = first_word + sec*SECTOR_WORDS + wi + ((phase==WORDS && bi==0) ? 1'b1 : 1'b0);
  always @(posedge machine_clk) begin
    pulse_d <= pulse;
    if (attached && pulse != pulse_d && pulse[1:0]==0 && (pulse>>2)<SECTORS_PER_TRACK) begin
      sec <= (pulse>>2)==0 ? SECTORS_PER_TRACK-1 : (pulse>>2)-1'b1;
      phase <= PRE; cnt <= PRE_WORDS*16; blk <= 0; data_bit <= 0;
    end else if (attached && data_adv) begin
      case (phase)
        PRE, GAP: begin
          data_bit <= 0;
          if (cnt <= 1) begin
            phase <= SYNC; cnt <= 16; sync_word <= 16'o201;
            wi <= block_base(blk); bi <= 15;
            data_bit <= 0; // MSB of 000201B
          end else cnt <= cnt-1'b1;
        end
        SYNC: begin
          cnt <= cnt-1'b1;
          if (cnt != 1) begin sync_word <= {sync_word[14:0],1'b0}; data_bit <= sync_word[14]; end
          else begin
            phase <= WORDS; cnt <= block_words(blk)*16; ecc <= sync_ecc();
            data_bit <= word_q[15];
          end
        end
        WORDS: begin
          next_ecc = {ecc[30:0],1'b0} ^ ((data_bit ^ ecc[31]) ? TAPS : 32'd0);
          ecc <= next_ecc; cnt <= cnt-1'b1;
          if (cnt == 1) begin phase <= ECC; cnt <= 32; data_bit <= next_ecc[31]; end
          else if (bi == 0) begin bi <= 15; wi <= wi+1'b1; data_bit <= word_q[15]; end
          else begin bi <= bi-1'b1; data_bit <= word_q[bi-1'b1]; end
        end
        ECC: begin
          ecc <= {ecc[30:0],1'b0}; cnt <= cnt-1'b1;
          if (cnt == 1) begin
            data_bit <= 0;
            if (blk == 2) phase <= IDLE;
            else begin blk <= blk+1'b1; phase <= GAP; cnt <= GAP_WORDS*16; end
          end else data_bit <= ecc[30];
        end
        default: data_bit <= 0;
      endcase
    end
  end

  // Capture the controller's real differential stream. The data reaches RAM
  // one word at a time; only a complete, CRC-checked block becomes dirty and
  // eligible for host writeback. A malformed stream stops the continuation.
  reg [1:0] wr_state = 0, wr_block = 0;
  reg [5:0] wr_bits = 0;
  reg [8:0] wr_count = 0;
  reg [12:0] wr_base = 0;
  reg [15:0] wr_shift = 0;
  reg [31:0] wr_ecc = 0;
  reg [15:0] shift_next;
  reg [31:0] ecc_next;
  reg wr_gate_d = 0;
  wire [13:0] completed_last_word = {1'b0,wr_base} + {5'd0,block_words(wr_block)} - 14'd1;
  always @(posedge machine_clk) begin
    store <= 0; wr_gate_d <= write_gate;
    if (!write_gate) begin
      if (wr_gate_d && wr_state != 0) begin
        serial_fault <= 1;
        if (!serial_fault) begin serial_debug<=fault_status(1);serial_extra<={7'd0,wr_count,wr_shift};end
      end
      wr_state <= 0; wr_shift <= 0; wr_bits <= 0;
    end else if (data_adv) begin
      if (write_data_p == write_data_m) begin
        if (wr_state != 0) begin
          serial_fault <= 1;
          if (!serial_fault) begin serial_debug<=fault_status(2);serial_extra<={7'd0,wr_count,wr_shift};end
        end
        wr_shift <= 0;
      end else begin
        shift_next = {wr_shift[14:0],write_data_m};
        case (wr_state)
          0: begin
            wr_shift <= shift_next;
            if (shift_next == 16'o201) begin
              wr_state <= 1; wr_count <= 0; wr_bits <= 0; wr_shift <= 0;
              wr_ecc <= sync_ecc(); wr_block <= blk;
              wr_base <= first_word + sec*SECTOR_WORDS + block_base(blk);
              if (head>=5 || cyl>=815 || readonly) begin
                serial_fault <= 1;
                if (!serial_fault) begin serial_debug<=fault_status(3);serial_extra<={31'd0,readonly};end
              end
            end
          end
          1: begin
            wr_shift <= shift_next;
            wr_ecc <= {wr_ecc[30:0],1'b0} ^ ((write_data_m ^ wr_ecc[31]) ? TAPS : 32'd0);
            wr_bits <= wr_bits+1'b1;
            if (wr_bits == 15) begin
              store <= 1; store_addr <= wr_base + wr_count; store_data <= shift_next;
              wr_count <= wr_count+1'b1; wr_bits <= 0;
              if (wr_count+1'b1 == block_words(wr_block)) wr_state <= 2;
            end
          end
          2: begin
            ecc_next = {wr_ecc[30:0],1'b0} ^ ((write_data_m ^ wr_ecc[31]) ? TAPS : 32'd0);
            wr_ecc <= ecc_next; wr_bits <= wr_bits+1'b1;
            if (wr_bits == 31) begin
              if (ecc_next != 0) begin
                serial_fault <= 1;
                if (!serial_fault) begin serial_debug<=fault_status(4);serial_extra<=ecc_next;end
              end
              else begin
                // The range becomes eligible at the SAME boundary as the
                // existing dirty generation: a complete, valid Fire code.
                // Machine execution pauses throughout HPS work, so these
                // machine-clock registers remain stable until ACK finishes.
                if (DIRTY_BLOCK_WINDOW!=0) begin
                  if (generation == flushed_generation || wr_base[12:8] < dirty_first_block)
                    dirty_first_block <= wr_base[12:8];
                  if (generation == flushed_generation || completed_last_word[12:8] > dirty_last_block)
                    dirty_last_block <= completed_last_word[12:8];
                end
                generation <= generation+1'b1;
              end
              wr_state <= 0; wr_bits <= 0; wr_shift <= 0;
            end
          end
        endcase
      end
    end
  end
  wire _unused = drivetag_n;
endmodule
