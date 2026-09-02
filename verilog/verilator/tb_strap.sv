// tb_strap -- the configuration straps, checked against the RESULTS the
// schematics state.
//
// A resistor SIP sets a board's identity: its slow-I/O device address, its
// task number. Which legs are present is a property of the PART FITTED, so it
// is in no wire list -- only on the board's "Configuration Information" sheet.
// Two of those sheets do not merely give the cut list, they give the ANSWER,
// which makes the strap self-checking:
//
//   DispM30.sil Rev Ch 11/09/82
//     "SIP in location g41 is 100 ohm terminator with leg 6 cut
//      (making DDMTIOA = 360B)"
//     "SIP in location b52 is 100 ohm terminator with legs 3 and 4 cut
//      for Task 9D = 11B"
//
// PARC numbers a field MSB-first, so DDMTIOA.00 is the top bit. g41 pins 2..6
// carry DDMTIOA.00..04; cutting leg 6 drops the bottom bit and leaves 11110 =
// 36B. The stated 360B is that with a low octal digit for the register select,
// so the colour board answers at 360B..367B -- which is where the C emulator
// already finds it, reading 360B and 361B for the presence test. Two models
// that share no code, agreeing on an address neither one chose.
//
// b52 pins 2..5 carry AltoWTask.0..3; cutting legs 3 and 4 leaves 1001 = 9
// decimal = 11 octal, exactly the "Task 9D = 11B" written on the sheet.
//
// Also checked here, because the same wire-list artifact hid them: the IFU's
// reference net TTLHigh, and DskEth's active-low drive-status lines. Those
// legs are marked `in` in the wire list, so nothing sourced them and they
// read 0 -- TTLHigh low, and every drive-status line ASSERTED, which invents
// a disk that is not attached.

`default_nettype none

module tb_strap;

  DispM_m_Rev_m_Ch  dm ();
  DispY_m_Rev_m_Cl  dy ();
  IFU_m_Rev_m_Ch    ifu ();
  DskEth_m_Rev_m_Cf de ();

  // MSB-first, as PARC numbers them.
  wire [4:0] ddmtioa   = {dm.DDMTIOA_00, dm.DDMTIOA_01, dm.DDMTIOA_02,
                          dm.DDMTIOA_03, dm.DDMTIOA_04};
  wire [3:0] altowtask = {dm.AltoWTask_0, dm.AltoWTask_1,
                          dm.AltoWTask_2, dm.AltoWTask_3};
  // DskEth's IOA address, set the same way at e41 (dashes mangle to _m_).
  wire [4:0] tioa_ad   = {de.TIOA_m_Ad_0, de.TIOA_m_Ad_1, de.TIOA_m_Ad_2,
                          de.TIOA_m_Ad_3, de.TIOA_m_Ad_4};
  // DskEth's Ethernet task pair, j52 legs 6/7/8 = TskAd.0/1/2. The board
  // decodes the transmit task as {~TskAd, 0} and the receive task as
  // {~TskAd, 1} (a24 inverts Next.0-3, j23 XORs the top three against the
  // strap, h23 NORs with Next.3), so the strap holds the COMPLEMENT of the
  // task's top three bits.
  wire [2:0] tskad     = {de.TskAd_0, de.TskAd_1, de.TskAd_2};
  wire [3:0] ethtx     = {~tskad, 1'b0};
  wire [3:0] ethrx     = {~tskad, 1'b1};
  // DispY's, which its sheet sets by cutting nothing at g42.
  wire [4:0] ddctioa   = {dy.DDCTIOA_00, dy.DDCTIOA_01, dy.DDCTIOA_02,
                          dy.DDCTIOA_03, dy.DDCTIOA_04};
  wire [3:0] dwttask   = {dy.DWTTask_0, dy.DWTTask_1, dy.DWTTask_2, dy.DWTTask_3};

  integer bad;

  initial begin
    bad = 0;
    #1;

    $display("tb_strap: DispM DDMTIOA   = %b = %0o B  (device at %0o B..%0oB)",
             ddmtioa, ddmtioa, ddmtioa * 8, ddmtioa * 8 + 7);
    if (ddmtioa !== 5'b11110) begin
      $display("tb_strap: FAIL -- DispM30.sil says leg 6 cut makes DDMTIOA = 360B, i.e. 11110");
      bad = bad + 1;
    end

    $display("tb_strap: DispM AltoWTask = %b = %0d decimal = %0o octal",
             altowtask, altowtask, altowtask);
    if (altowtask !== 4'b1001) begin
      $display("tb_strap: FAIL -- DispM30.sil says legs 3,4 cut give Task 9D = 11B, i.e. 1001");
      bad = bad + 1;
    end

    $display("tb_strap: DskEth TIOA-Ad  = %b = %0o B  (IOA %0o B..%0oB, the starred standard row)",
             tioa_ad, tioa_ad, tioa_ad * 8, tioa_ad * 8 + 7);
    if (tioa_ad !== 5'b00001) begin
      $display("tb_strap: FAIL -- Ether12.sil's table says cutting P4..P7 gives IOA 010-017, i.e. 00001");
      bad = bad + 1;
    end

    $display("tb_strap: DskEth TskAd    = %b -> Ethernet tasks %0d (output) & %0d (input)",
             tskad, ethtx, ethrx);
    if (tskad !== 3'b100) begin
      $display("tb_strap: FAIL -- DskEth.pdf p.35: 'Cut SIP legs at j52 to set the Task numbers for the Ethernet. Standard tasks are 6 & 7' -- legs 7,8 cut = 100");
      bad = bad + 1;
    end

    // No stated result for these two, so they are recorded, not asserted --
    // g42 has no legs cut at all, and k52 has leg 3 cut.
    $display("tb_strap: DispY  DDCTIOA   = %b = %0o B  (IOA %0o B..%0oB)",
             ddctioa, ddctioa, ddctioa * 8, ddctioa * 8 + 7);
    $display("tb_strap: DispY  DWTTask   = %b = %0d decimal = %0o octal",
             dwttask, dwttask, dwttask);

    if (ifu.TTLHigh !== 1'b1) begin
      $display("tb_strap: FAIL -- the IFU's reference net TTLHigh reads %b", ifu.TTLHigh);
      bad = bad + 1;
    end

    // Active low: no drive attached, so every one of these must read high.
    // These leave the board, so what DskEth controls is its contribution to
    // the wired-OR (`__drv`), not the resolved net -- which standalone is
    // unconnected. The pull is the contribution.
    if (!(de.Selected0_p___drv === 1'b1 && de.Selected2_p___drv === 1'b1 &&
          de.SecIndx0_p___drv  === 1'b1 && de.SecIndx2_p___drv  === 1'b1 &&
          de.TtlTerm_p___drv   === 1'b1 && de.TtlEndOfCyl_p___drv === 1'b1 &&
          de.TtlOffSet_p___drv === 1'b1 && de.TtlReadOnly_p___drv === 1'b1)) begin
      $display("tb_strap: FAIL -- a DskEth drive-status line is asserted with no drive attached");
      bad = bad + 1;
    end

    if (bad != 0) $fatal(1);
    $display("tb_strap: PASS -- DDMTIOA 360B, task 11B, DskEth IOA 010-017 and Ethernet tasks 6 & 7, TTLHigh high, no phantom drive");
    $finish;
  end

endmodule
