// MiSTer PS/2 packets -> the terminal's relative motion and active-low buttons.
// Runs on the ungated host clock so disk stalls cannot lose mouse packets.
`default_nettype none
module dorado_mouse #(parameter integer ACCUM_BITS = 24) (
    input wire clk, reset,
    input wire [24:0] ps2_mouse,
    input wire mouse_taken,
    input wire signed [8:0] mouse_sent_dx, mouse_sent_dy,
    output wire [15:0] key_word4,
    output wire signed [8:0] mouse_dx, mouse_dy,
    output wire mouse_pending,
    output reg motion_overflow = 0
);
  reg packet_d = 0, taken_d = 0;
  reg [2:0] buttons = 0;
  reg signed [ACCUM_BITS-1:0] pending_x = 0, pending_y = 0;
  wire packet = ps2_mouse[24] != packet_d;
  // The terminal clock may stop with mouse_taken high. Consume its report
  // once, rather than once per ungated host clock while the machine pauses.
  wire take = mouse_taken && !taken_d;

  // MiSTer: {toggle,Y byte,X byte,PS/2 flags}. The sign is in the FLAGS,
  // not bit 7 of the data byte: +255 and -256 are valid nine-bit deltas.
  wire signed [ACCUM_BITS:0] packet_x = ps2_mouse[6]
      ? (ps2_mouse[4] ? -256 : 255)
      : $signed({{(ACCUM_BITS-8){ps2_mouse[4]}}, ps2_mouse[15:8]});
  wire signed [ACCUM_BITS:0] packet_y_up = ps2_mouse[7]
      ? (ps2_mouse[5] ? -256 : 255)
      : $signed({{(ACCUM_BITS-8){ps2_mouse[5]}}, ps2_mouse[23:16]});
  // PS/2 is positive up; the terminal/guest convention is positive down.
  wire signed [ACCUM_BITS:0] packet_y = -packet_y_up;
  wire signed [ACCUM_BITS:0] next_x =
      $signed({pending_x[ACCUM_BITS-1],pending_x}) +
      (packet ? packet_x : $signed({(ACCUM_BITS+1){1'b0}})) -
      (take ? $signed(mouse_sent_dx) : 9'sd0);
  wire signed [ACCUM_BITS:0] next_y =
      $signed({pending_y[ACCUM_BITS-1],pending_y}) +
      (packet ? packet_y : $signed({(ACCUM_BITS+1){1'b0}})) -
      (take ? $signed(mouse_sent_dy) : 9'sd0);
  localparam signed [ACCUM_BITS:0] LIMIT_HI = {2'b00,{(ACCUM_BITS-1){1'b1}}};
  localparam signed [ACCUM_BITS:0] LIMIT_LO = {2'b11,{(ACCUM_BITS-1){1'b0}}};

  always @(posedge clk) begin
    packet_d <= ps2_mouse[24];
    taken_d <= mouse_taken;
    if (reset) begin
      pending_x <= 0; pending_y <= 0; buttons <= 0;
      motion_overflow <= 0;
    end else begin
      if (packet) buttons <= ps2_mouse[2:0];
      if (next_x > LIMIT_HI) pending_x <= LIMIT_HI[ACCUM_BITS-1:0];
      else if (next_x < LIMIT_LO) pending_x <= LIMIT_LO[ACCUM_BITS-1:0];
      else pending_x <= next_x[ACCUM_BITS-1:0];
      if (next_y > LIMIT_HI) pending_y <= LIMIT_HI[ACCUM_BITS-1:0];
      else if (next_y < LIMIT_LO) pending_y <= LIMIT_LO[ACCUM_BITS-1:0];
      else pending_y <= next_y[ACCUM_BITS-1:0];
      if (next_x > LIMIT_HI || next_x < LIMIT_LO ||
          next_y > LIMIT_HI || next_y < LIMIT_LO ||
          (packet && |ps2_mouse[7:6])) motion_overflow <= 1;
    end
  end
  // HM Table 24 / Alto UTILIN: left=bit2, right=bit1, middle=bit0.
  // The keyset stays released. A button packet carries no artificial motion.
  assign key_word4 = {13'h1fff,~buttons[0],~buttons[1],~buttons[2]};
  assign mouse_dx = pending_x > 127 ? 9'sd127 :
                    pending_x < -128 ? -9'sd128 : pending_x[8:0];
  assign mouse_dy = pending_y > 127 ? 9'sd127 :
                    pending_y < -128 ? -9'sd128 : pending_y[8:0];
  assign mouse_pending = pending_x != 0 || pending_y != 0;
endmodule
`default_nettype wire
