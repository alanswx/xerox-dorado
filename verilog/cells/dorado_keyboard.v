// dorado_keyboard -- a PS/2 keyboard as the Dorado's 61-key matrix.
//
// Turns MiSTer's `ps2_key` events into the four keyboard words the terminal
// microcomputer reports (Alto 177034B-177037B), which `dorado_terminal` then
// serialises as HM Table 24 messages.
//
// THE MATRIX IS NOT A FORMULA, though it looks like one. The C emulator's
// `key_map` (dorado/src/display.c) appears to assign bits strictly in
// enumeration order, and `word = (idx-1)/16, bit = 15-((idx-1)%16)` does
// reproduce the first 47 keys. It then breaks: WORD 2 CARRIES ONLY FIFTEEN
// KEYS, so the formula puts R in word 2 bit 0 where the real matrix starts
// word 3 with it. The Alto matrix has a hole -- the sort of thing Figure 6
// shows and arithmetic does not.
//
// So the positions below are stated one by one, taken from key_map, and
// `make -C verilog key-matrix-check` compares this file against display.c on
// every run. Nothing here re-derives the matrix: that 61-key layout was
// audited against three independent sources (Alto HW Manual Figure 6,
// ContrAlto, and Cedar's own TerminalDefs.mesa KeyName, which settles Figure
// 6's one ambiguous row), and this module only has to agree with it.
//
// ACTIVE LOW: Alto HW Manual doc p.27 -- depressed = 0. Words idle at 0xFFFF.
//
// THREE KEYS HAVE NO MARKING on the Alto keytops and are named Look, Next and
// Swat; they take F1-F3 here. Line feed is a real key on that keyboard with no
// modern equivalent, so it takes F4, and the Alto's left-arrow takes the
// backtick. Those five are the only choices in this file that are conventions
// rather than facts, and they are listed together so they are easy to change.

`default_nettype none

module dorado_keyboard (
    input  wire        sys_clk,
    input  wire        reset,

    // MiSTer's ps2_key: {strobe_toggle, pressed, extended, code[7:0]}.
    input  wire [10:0] ps2_key,

    output reg  [15:0] key_word0,
    output reg  [15:0] key_word1,
    output reg  [15:0] key_word2,
    output reg  [15:0] key_word3
);

  reg  prev_strobe;
  wire        strobe   = ps2_key[10];
  wire        pressed  = ps2_key[9];
  wire        extended = ps2_key[8];
  wire [7:0]  code     = ps2_key[7:0];

  // PS/2 set 2 -> a POSITION in the matrix: {valid, word[1:0], bit[3:0]}.
  //
  // NOT A FORMULA. The enumeration order looks purely positional and holds
  // for the first 47 keys, then breaks -- word 2 carries only FIFTEEN keys,
  // so `(idx-1)/16` puts R in word 2 bit 0 where the real matrix starts
  // word 3 with it. The Alto matrix has a hole. These positions come from
  // dorado/src/display.c's key_map and `make -C verilog key-matrix-check`
  // compares the two files on every run.
  function [6:0] key_pos_f(input [7:0] c, input e);
    begin
      key_pos_f = 7'b0_00_0000;
      if (e) begin
        case (c)
      8'h71: key_pos_f = 7'b1_10_0001;            // DORADO_KEY_DEL
          default: key_pos_f = 7'b0_00_0000;
        endcase
      end else begin
        case (c)
      8'h2E: key_pos_f = 7'b1_00_1111;              // DORADO_KEY_5
      8'h25: key_pos_f = 7'b1_00_1110;              // DORADO_KEY_4
      8'h36: key_pos_f = 7'b1_00_1101;              // DORADO_KEY_6
      8'h24: key_pos_f = 7'b1_00_1100;              // DORADO_KEY_E
      8'h3D: key_pos_f = 7'b1_00_1011;              // DORADO_KEY_7
      8'h23: key_pos_f = 7'b1_00_1010;              // DORADO_KEY_D
      8'h3C: key_pos_f = 7'b1_00_1001;              // DORADO_KEY_U
      8'h2A: key_pos_f = 7'b1_00_1000;              // DORADO_KEY_V
      8'h45: key_pos_f = 7'b1_00_0111;              // DORADO_KEY_0
      8'h42: key_pos_f = 7'b1_00_0110;              // DORADO_KEY_K
      8'h4E: key_pos_f = 7'b1_00_0101;          // DORADO_KEY_MINUS
      8'h4D: key_pos_f = 7'b1_00_0100;              // DORADO_KEY_P
      8'h4A: key_pos_f = 7'b1_00_0011;         // DORADO_KEY_FSLASH
      8'h5D: key_pos_f = 7'b1_00_0010;         // DORADO_KEY_BSLASH
      8'h0C: key_pos_f = 7'b1_00_0001;             // DORADO_KEY_LF
      8'h66: key_pos_f = 7'b1_00_0000;             // DORADO_KEY_BS
      8'h26: key_pos_f = 7'b1_01_1111;              // DORADO_KEY_3
      8'h1E: key_pos_f = 7'b1_01_1110;              // DORADO_KEY_2
      8'h1D: key_pos_f = 7'b1_01_1101;              // DORADO_KEY_W
      8'h15: key_pos_f = 7'b1_01_1100;              // DORADO_KEY_Q
      8'h1B: key_pos_f = 7'b1_01_1011;              // DORADO_KEY_S
      8'h1C: key_pos_f = 7'b1_01_1010;              // DORADO_KEY_A
      8'h46: key_pos_f = 7'b1_01_1001;              // DORADO_KEY_9
      8'h43: key_pos_f = 7'b1_01_1000;              // DORADO_KEY_I
      8'h22: key_pos_f = 7'b1_01_0111;              // DORADO_KEY_X
      8'h44: key_pos_f = 7'b1_01_0110;              // DORADO_KEY_O
      8'h4B: key_pos_f = 7'b1_01_0101;              // DORADO_KEY_L
      8'h41: key_pos_f = 7'b1_01_0100;          // DORADO_KEY_COMMA
      8'h52: key_pos_f = 7'b1_01_0011;          // DORADO_KEY_QUOTE
      8'h5B: key_pos_f = 7'b1_01_0010;       // DORADO_KEY_RBRACKET
      8'h06: key_pos_f = 7'b1_01_0001;    // DORADO_KEY_BLANKMIDDLE
      8'h05: key_pos_f = 7'b1_01_0000;       // DORADO_KEY_BLANKTOP
      8'h16: key_pos_f = 7'b1_10_1111;              // DORADO_KEY_1
      8'h76: key_pos_f = 7'b1_10_1110;            // DORADO_KEY_ESC
      8'h0D: key_pos_f = 7'b1_10_1101;            // DORADO_KEY_TAB
      8'h2B: key_pos_f = 7'b1_10_1100;              // DORADO_KEY_F
      8'h14: key_pos_f = 7'b1_10_1011;           // DORADO_KEY_CTRL
      8'h21: key_pos_f = 7'b1_10_1010;              // DORADO_KEY_C
      8'h3B: key_pos_f = 7'b1_10_1001;              // DORADO_KEY_J
      8'h32: key_pos_f = 7'b1_10_1000;              // DORADO_KEY_B
      8'h1A: key_pos_f = 7'b1_10_0111;              // DORADO_KEY_Z
      8'h12: key_pos_f = 7'b1_10_0110;         // DORADO_KEY_LSHIFT
      8'h49: key_pos_f = 7'b1_10_0101;         // DORADO_KEY_PERIOD
      8'h4C: key_pos_f = 7'b1_10_0100;      // DORADO_KEY_SEMICOLON
      8'h5A: key_pos_f = 7'b1_10_0011;         // DORADO_KEY_RETURN
      8'h0E: key_pos_f = 7'b1_10_0010;          // DORADO_KEY_ARROW
      8'h2D: key_pos_f = 7'b1_11_1111;              // DORADO_KEY_R
      8'h2C: key_pos_f = 7'b1_11_1110;              // DORADO_KEY_T
      8'h34: key_pos_f = 7'b1_11_1101;              // DORADO_KEY_G
      8'h35: key_pos_f = 7'b1_11_1100;              // DORADO_KEY_Y
      8'h33: key_pos_f = 7'b1_11_1011;              // DORADO_KEY_H
      8'h3E: key_pos_f = 7'b1_11_1010;              // DORADO_KEY_8
      8'h31: key_pos_f = 7'b1_11_1001;              // DORADO_KEY_N
      8'h3A: key_pos_f = 7'b1_11_1000;              // DORADO_KEY_M
      8'h58: key_pos_f = 7'b1_11_0111;           // DORADO_KEY_LOCK
      8'h29: key_pos_f = 7'b1_11_0110;          // DORADO_KEY_SPACE
      8'h54: key_pos_f = 7'b1_11_0101;       // DORADO_KEY_LBRACKET
      8'h55: key_pos_f = 7'b1_11_0100;           // DORADO_KEY_PLUS
      8'h59: key_pos_f = 7'b1_11_0011;         // DORADO_KEY_RSHIFT
      8'h04: key_pos_f = 7'b1_11_0010;    // DORADO_KEY_BLANKBOTTOM
          default: key_pos_f = 7'b0_00_0000;
        endcase
      end
    end
  endfunction

  wire [6:0]  pos   = key_pos_f(code, extended);
  wire        valid = pos[6];
  wire [1:0]  wsel  = pos[5:4];
  wire [3:0]  bsel  = pos[3:0];
  wire [15:0] mask  = 16'd1 << bsel;

  always @(posedge sys_clk) begin
    if (reset) begin
      key_word0 <= 16'hFFFF; key_word1 <= 16'hFFFF;
      key_word2 <= 16'hFFFF; key_word3 <= 16'hFFFF;
      prev_strobe <= strobe;
    end else begin
      prev_strobe <= strobe;
      // The strobe TOGGLES per event, so an edge on it is one key event --
      // and a repeat of the same code is a distinct event, which matters
      // because the terminal reports transitions.
      if (strobe != prev_strobe && valid) begin
        case (wsel)
          2'd0: key_word0 <= pressed ? (key_word0 & ~mask) : (key_word0 | mask);
          2'd1: key_word1 <= pressed ? (key_word1 & ~mask) : (key_word1 | mask);
          2'd2: key_word2 <= pressed ? (key_word2 & ~mask) : (key_word2 | mask);
          2'd3: key_word3 <= pressed ? (key_word3 & ~mask) : (key_word3 | mask);
        endcase
      end
    end
  end

endmodule

`default_nettype wire
