// dorado_terminal -- the "processor in the keyboard", as RTL.
//
// The Dorado's keyboard, keyset and mouse do not reach the machine as parallel
// bits. A microcomputer in the terminal serialises them into 32-bit messages
// and sends them back up the 7-wire terminal cable, and the Dorado receives
// that stream at the BaseBoard:
//
//     cable  ->  BaseBd OISData/OISData'   (differential pair)
//            ->  f06, an MC10125 ECL->TTL translator      -> TKeyboardData
//            ->  an MC10124 TTL->ECL translator           -> KeyboardData
//            ->  backplane -> DispY c22 (MC10231 flip-flop, D = KeyboardData,
//                             CLOCK = RamdHBlank)         -> OISRcvdData
//            ->  gated onto IOB.00 for the microcode's ReadTerminal.
//
// Two things fall out of that chain and they set this module's whole shape.
//
// ONE BIT PER SCAN LINE. c22 is clocked by `RamdHBlank`, so the machine samples
// this line once per horizontal blanking and a 32-bit message takes 32 lines.
// A real terminal has no choice about this either: it is a separate box whose
// only timing reference is the video it is being sent, so it clocks its reply
// off that. This module does the same -- `line_tick` is a horizontal-blanking
// pulse -- which makes the rate right by construction rather than by a
// constant someone has to keep in step.
//
// THE BIT IS SENT MSB FIRST. DisplayAux.mc's ReadTerminal reads "Data = IOB[0]"
// and LCY-accumulates the message, so bit 31 goes first.
//
// THE MESSAGE FORMAT IS HM TABLE 24, and this is a transcription of the C
// emulator's `display_terminal_keyboard_bit` (dorado/src/display.c), which is
// itself grounded in the manual and has been run against five operating
// systems. Keeping the two models textually close is deliberate: it is what
// makes them cross-checkable.
//
//     bit 31      1                 start
//     bits 30:24  type              01B-04B keyboard words 0-3 (Alto
//                                   177034B-177037B), 05B mouse buttons +
//                                   keyset (177033B), 06B mouse X/Y change
//     bits 23:8   body              the 16-bit word
//     bit 7       1
//     bits 6:0    0
//
// ARBITRATION, from the manual's remark just above Table 24: a pending
// KEYBOARD TRANSITION outranks a mouse-position change -- "one keyboard word
// is reported instead of the mouse position change; thus, the correct state of
// the keyboard is eventually reported even if transitions are missed". When
// nothing has changed the five words are refreshed round-robin, which is what
// makes "eventually" true and is also the periodic cadence the guests' own
// terminal microcode expects. The C emulator's header records that this
// priority used to be INVERTED, and that letting a moving mouse delay a button
// transition is the exact shape of the Interlisp menu bug.
//
// KEYS ARE ACTIVE LOW -- Alto HW Manual doc p.27: depressed = 0. The caller
// hands over `key_words` already in that sense.
//
// THE BODY IS LATCHED AT MESSAGE START. Re-reading it per bit lets a key that
// changes mid-message produce a word assembled from two different keyboard
// states -- a value that never existed on the machine.

`default_nettype none

module dorado_terminal (
    input  wire        sys_clk,
    input  wire        reset,

    // One pulse per horizontal blanking. The machine samples this module's
    // output at exactly that rate, so it is the bit clock.
    input  wire        line_tick,

    // The five words the terminal reports, in the Dorado's own order:
    // 0-3 are keyboard words (Alto 177034B-177037B), 4 is mouse buttons and
    // keyset (177033B). Active low, depressed = 0.
    input  wire [15:0] key_word0,
    input  wire [15:0] key_word1,
    input  wire [15:0] key_word2,
    input  wire [15:0] key_word3,
    input  wire [15:0] key_word4,

    // Mouse motion since the last report, as a signed count of units. Held by
    // the caller; this module consumes what it can carry and says how much.
    input  wire signed [8:0] mouse_dx,
    input  wire signed [8:0] mouse_dy,
    input  wire        mouse_pending,
    output reg         mouse_taken,     // one pulse when a 06B message starts
    output reg  signed [8:0] mouse_sent_dx,
    output reg  signed [8:0] mouse_sent_dy,

    // The serial return channel, as the differential pair the BaseBoard's
    // MC10125 receives.
    output wire        OISData,
    output wire        OISData_n
);

  // ---- edge-detect the five words so a TRANSITION can outrank the mouse ----
  reg [15:0] prev0, prev1, prev2, prev3, prev4;
  reg  [4:0] kbd_pending;

  // ---- message state ----
  reg  [4:0] msg_bit;        // 0..31
  reg  [2:0] msg_word;       // round-robin cursor, 0..4
  reg  [6:0] msg_type;
  reg [15:0] msg_body;
  reg [31:0] msg;

  wire [15:0] word_of_0 = key_word0, word_of_1 = key_word1,
              word_of_2 = key_word2, word_of_3 = key_word3,
              word_of_4 = key_word4;

  function [15:0] word_of(input [2:0] w);
    case (w)
      3'd0: word_of = word_of_0;
      3'd1: word_of = word_of_1;
      3'd2: word_of = word_of_2;
      3'd3: word_of = word_of_3;
      default: word_of = word_of_4;
    endcase
  endfunction

  // Lowest pending word first, so a burst is reported in a stable order.
  function [2:0] lowest_pending(input [4:0] p);
    casez (p)
      5'b????1: lowest_pending = 3'd0;
      5'b???10: lowest_pending = 3'd1;
      5'b??100: lowest_pending = 3'd2;
      5'b?1000: lowest_pending = 3'd3;
      5'b10000: lowest_pending = 3'd4;
      default:  lowest_pending = 3'd0;
    endcase
  endfunction

  // Clamp to what a byte carries and let the caller keep the remainder, so a
  // fast host movement becomes several small deltas rather than one wrapped
  // one. Excess-200B: the wire value is the delta plus 128.
  wire signed [8:0] dx_clamped = (mouse_dx >  9'sd127) ?  9'sd127 :
                                 (mouse_dx < -9'sd128) ? -9'sd128 : mouse_dx;
  wire signed [8:0] dy_clamped = (mouse_dy >  9'sd127) ?  9'sd127 :
                                 (mouse_dy < -9'sd128) ? -9'sd128 : mouse_dy;
  wire [15:0] mouse_body = {dx_clamped[7:0] + 8'h80, dy_clamped[7:0] + 8'h80};

  always @(posedge sys_clk) begin
    mouse_taken <= 1'b0;
    if (reset) begin
      msg_bit <= 5'd0; kbd_pending <= 5'd0;
      // Preloaded with the first message the round-robin will send (type 1,
      // keyboard word 0), so the machine's first sampled bit is a real start
      // bit rather than whatever the register powered up holding.
      msg_type <= 7'd1; msg_body <= key_word0;
      msg <= {1'b1, 7'd1, key_word0, 1'b1, 7'd0};
      msg_word <= 3'd1;
      prev0 <= 16'hFFFF; prev1 <= 16'hFFFF; prev2 <= 16'hFFFF;
      prev3 <= 16'hFFFF; prev4 <= 16'hFFFF;
      mouse_sent_dx <= 9'sd0; mouse_sent_dy <= 9'sd0;
    end else begin
      // A transition on any word raises its pending bit. Sampled every
      // sys_clk, not per line, so a brief press is not missed.
      if (key_word0 !== prev0) begin kbd_pending[0] <= 1'b1; prev0 <= key_word0; end
      if (key_word1 !== prev1) begin kbd_pending[1] <= 1'b1; prev1 <= key_word1; end
      if (key_word2 !== prev2) begin kbd_pending[2] <= 1'b1; prev2 <= key_word2; end
      if (key_word3 !== prev3) begin kbd_pending[3] <= 1'b1; prev3 <= key_word3; end
      if (key_word4 !== prev4) begin kbd_pending[4] <= 1'b1; prev4 <= key_word4; end

      if (line_tick) begin
        // CHOOSE THE NEXT MESSAGE ON THE LAST BIT OF THIS ONE, not on the
        // first bit of the next. `msg` is a register and the output reads it
        // combinationally, so selecting at bit 0 would send bit 31 of the
        // PREVIOUS message -- which at power-up is the reset value, and the
        // very first start bit came out 0. Selecting one tick early means the
        // register already holds the right word when bit 31 is presented.
        if (msg_bit == 5'd31) begin
          // ---- message boundary: choose, then LATCH ----
          if (|kbd_pending) begin
            automatic reg [2:0] w = lowest_pending(kbd_pending);
            kbd_pending[w] <= 1'b0;
            msg_type <= (w < 3'd4) ? {4'd0, w} + 7'd1 : 7'd5;
            msg_body <= word_of(w);
            msg <= {1'b1, ((w < 3'd4) ? ({4'd0, w} + 7'd1) : 7'd5),
                    word_of(w), 1'b1, 7'd0};
          end else if (mouse_pending) begin
            msg_type <= 7'd6;
            msg_body <= mouse_body;
            msg <= {1'b1, 7'd6, mouse_body, 1'b1, 7'd0};
            mouse_taken   <= 1'b1;
            mouse_sent_dx <= dx_clamped;
            mouse_sent_dy <= dy_clamped;
          end else begin
            // Idle: refresh the words in rotation so the guest's copy
            // converges even if a transition was missed.
            msg_type <= (msg_word < 3'd4) ? {4'd0, msg_word} + 7'd1 : 7'd5;
            msg_body <= word_of(msg_word);
            msg <= {1'b1, ((msg_word < 3'd4) ? ({4'd0, msg_word} + 7'd1) : 7'd5),
                    word_of(msg_word), 1'b1, 7'd0};
            msg_word <= (msg_word == 3'd4) ? 3'd0 : msg_word + 3'd1;
          end
        end
        msg_bit <= msg_bit + 5'd1;   // wraps at 32 by width
      end
    end
  end

  // MSB first: bit 31 goes out at msg_bit == 0.
  assign OISData   = msg[5'd31 - msg_bit];
  assign OISData_n = ~OISData;

endmodule

`default_nettype wire
