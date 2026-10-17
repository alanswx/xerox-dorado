// Focused controller/cable diagnostic; initialization uses slow-I/O ports.
// This does not replace the full microcode/OS boot gate.
`timescale 1ns/1ps
module tb_disk_stream;
`ifndef SYSPER_OVERRIDE
  `define SYSPER_OVERRIDE 16
`endif
  localparam integer SYSPER = `SYSPER_OVERRIDE;
  reg sys_clk=0; always #1 sys_clk=~sys_clk;
  integer ticks=0;
  always @(posedge sys_clk) ticks<=ticks+1;
  wire clk_disk = (ticks % SYSPER) >= SYSPER/2;
  reg reset=1, out_n=1, in_n=1;
  reg trace_on=0;
  initial trace_on=$test$plusargs("streamtrace");
  reg write_test=0, checked_write=0, read_armed=0, checked_headers_done=0;
  initial begin
    checked_write=$test$plusargs("checkedwrite");
    write_test=$test$plusargs("writeback") || checked_write;
  end
  reg fast_io=0;
  initial fast_io=$test$plusargs("fastio");
  reg timed_polls=0;
  initial timed_polls=$test$plusargs("pollsectors");
  reg [7:0] addr=0;
  reg [15:0] data=0;
  wire dp,dm,cp,cm,adv,bit_,sel_n,sec_n,index_n,ready_n,online_n,term_n;
  wire select_n;
  wire writing;
  wire [31:0] write_blocks;
  reg corrupt_write=0; initial corrupt_write=$test$plusargs("badwritecrc");
  wire bad_write_bit=corrupt_write && pk.wr_state==2 && pk.wr_bits==16;
  DskEth_m_Rev_m_Cf #(.SYSPER(SYSPER)) d (
    .sys_clk(sys_clk), .CLK_disk_p_(clk_disk), .CLKEnable_p_a(1'b0),
    .IOReset(reset), .IOout_p_(out_n), .IOin_p_(in_n), .IOHold(1'b0),
    .BNTGtCT_p_b(1'b1), .MemSH_p_((ticks % (2*SYSPER)) < SYSPER),
    .OKToSelect(d.OKToSelect__drv),
    .OS0(d.OS0__drv), .OS1(d.OS1__drv), .OS2(d.OS2__drv), .OS3(d.OS3__drv),
    .ClockP0(cp), .ClockM0(cm), .DataP0(writing ? 1'b0 : dp), .DataM0(writing ? 1'b0 : dm),
    .Selected0_p_(sel_n), .Selected1_p_(1'b1), .Selected2_p_(1'b1), .Selected3_p_(1'b1),
    .SecIndx0_p_(sec_n), .SecIndx1_p_(1'b1), .SecIndx2_p_(1'b1), .SecIndx3_p_(1'b1),
    .TtlReady_p_(ready_n), .TtlOnLine_p_(online_n), .TtlTerm_p_(term_n),
    .TtlSector_p_(d.TtlSector_p___drv), .TtlIndex_p_(index_n), .TtlEndOfCyl_p_(1'b1),
    .TtlDeviceCk_p_(1'b1), .TtlOffSet_p_(1'b1), .TtlReadOnly_p_(1'b1), .TtlSeekInc_p_(1'b1),
    .Select0_p___drv(select_n),
    .TIOA_0(addr[7]),
    .TIOA_1(addr[6]),
    .TIOA_2(addr[5]),
    .TIOA_3(addr[4]),
    .TIOA_4(addr[3]),
    .TIOA_5(addr[2]),
    .TIOA_6(addr[1]),
    .TIOA_7(addr[0]),
    .IOB_00(data[15]),
    .IOB_01(data[14]),
    .IOB_02(data[13]),
    .IOB_03(data[12]),
    .IOB_04(data[11]),
    .IOB_05(data[10]),
    .IOB_06(data[9]),
    .IOB_07(data[8]),
    .IOB_08(data[7]),
    .IOB_09(data[6]),
    .IOB_10(data[5]),
    .IOB_11(data[4]),
    .IOB_12(data[3]),
    .IOB_13(data[2]),
    .IOB_14(data[1]),
    .IOB_15(data[0]),
    .IOB_16(~^data[15:8]), .IOB_17(~^data[7:0])
  );
  dorado_trident #(.SYSPER(2*SYSPER), .SECTORS_PER_REV(117), .WORDS_PER_TRACK(10080)) drv (
    .sys_clk(sys_clk), .reset(reset), .attached(1'b1), .selected(!select_n),
    .TtlReady_n(ready_n), .TtlOnLine_n(online_n), .TtlTerm_n(term_n),
    .Selected_n(sel_n), .SecIndx_n(sec_n), .TtlIndex_n(index_n),
    .data_bit(bit_), .data_adv(adv), .DataP(dp), .DataM(dm), .ClockP(cp), .ClockM(cm));
  dorado_pack pk (
    .selected(!select_n), .write_data_p(d.DataP0__drv ^ bad_write_bit),
    .write_data_m(d.DataM0__drv ^ bad_write_bit),
    .write_gate(writing), .n_write_blocks(write_blocks),
    .sys_clk(sys_clk), .attached(1'b1),
    .tagbus_n({d.TagBus_000_p___drv,d.TagBus_00_p___drv,d.TagBus_0_p___drv,
      d.TagBus_1_p___drv,d.TagBus_2_p___drv,d.TagBus_3_p___drv,d.TagBus_4_p___drv,
      d.TagBus_5_p___drv,d.TagBus_6_p___drv,d.TagBus_7_p___drv,d.TagBus_8_p___drv,d.TagBus_9_p___drv}),
    .cyltag_n(d.CylinderTag_p___drv), .headtag_n(d.HeadTag_p___drv),
    .drivetag_n(d.DriveTag_p___drv), .conttag_n(d.ContTag_p___drv),
    .pulse(drv.sector), .data_adv(adv), .data_bit(bit_));
  task put(input [7:0] a,input [15:0] v);
    @(negedge sys_clk); addr=a; data=v;
    repeat(fast_io ? 2*SYSPER : 4*SYSPER) @(negedge sys_clk);
    @(posedge d.PreClock1_p_Cb); @(negedge sys_clk); out_n=0;
    @(negedge d.PreClock1_p_Cb); @(posedge d.PreClock1_p_Cb);
    repeat(3) @(negedge sys_clk); out_n=1;
    repeat(fast_io ? 0 : 8*SYSPER) @(negedge sys_clk);
    if(trace_on) $display("STREAM PUT @%0d a=%o v=%h addr=%h enable=%b",ticks,a,v,
      {d.RamAddr_0,d.RamAddr_1,d.RamAddr_2,d.RamAddr_3},d.EnableRun);
  endtask
  task send_tag(input [15:0] v);
    // DiskSubrs.mc SendTag clears the previous tag wakeup before issuing
    // a tag, waits for its timed completion, then clears it again.
    put(8'o11,16'h0200);
    put(8'o11,16'h0000);
    put(8'o14,v);
    repeat(2048) @(negedge sys_clk);
    put(8'o11,16'h0200);
    put(8'o11,16'h0000);
  endtask
  localparam [15:0] FMT[16]='{16'o1,16'o7,16'o377,16'o0,16'o104,16'o204,16'o4,16'o0,
    16'o33,16'o6,16'o11,16'o2,16'o2,16'o1,16'o0,16'o0};
  integer i;
  reg armed=0;
  integer words=1; // number disk words from 1; do not consume setup state
  string path;
  integer fd,rc;
  integer stream_sector=0;
  integer stream_pulse;
  reg [7:0] expected_bytes[0:533];
  integer expected_index;
  reg [15:0] received_word;
  function [15:0] expected_word(input integer k);
    expected_word={expected_bytes[2*k+1],expected_bytes[2*k]};
  endfunction
  function [15:0] written_word(input integer b,input integer k);
    written_word=16'h6000 ^ (b<<8) ^ k;
  endfunction
  reg writer_done=0;
  initial begin
    wait(armed && write_test);
    // AltoDiabloDisk.mc ACmmdWrite selects DiskData BEFORE waiting on
    // WriteFifoTW: selecting any other TIOA explicitly disables that wakeup.
    @(negedge sys_clk); addr=8'o12;
    repeat(8*SYSPER) @(negedge sys_clk);
    $display("WRITEBACK producer armed @%0d WrTW=%b",ticks,d.WrFifoTW);
    if(checked_write) wait(checked_headers_done);
    for(integer b=(checked_write ? 2 : 0);b<3;b=b+1) begin
      // ACmmdWrite sends SYNC and the first payload word unconditionally.
      // In a read/check -> write transition the old OutReg's type still
      // disables WriteFifoTW until this first outgoing word reaches it.
      put(8'o12,16'o201);
      $display("WRITEBACK block%0d sync sent @%0d",b,ticks);
      for(integer k=0;k<(b==0 ? 2 : b==1 ? 8 : 256);k=k+1) begin
        if(k!=0) wait(d.WrFifoTW);
        put(8'o12,written_word(b,k));
      end
    end
    writer_done=1;
  end
  wire [15:0] result_word={d.DskData_00,d.DskData_01,d.DskData_02,d.DskData_03,d.DskData_04,d.DskData_05,d.DskData_06,d.DskData_07,d.DskData_08,d.DskData_09,d.DskData_10,d.DskData_11,d.DskData_12,d.DskData_13,d.DskData_14,d.DskData_15};
  initial begin
    wait(armed && checked_write); wait(d.Active); wait(d.RdFifoTW);
    for(integer b=0;b<2;b=b+1) begin
      addr=8'o12;
      for(integer k=0;k<(b==0 ? 6 : 12);k=k+1) begin
        reg [15:0] got;
        wait(d.OutRegFull);
        @(posedge d.Clock1_p_Ca); repeat(2) @(negedge sys_clk);
        got=result_word; in_n=0;
        if(trace_on) $display("CHECKPREFIX @%0d block%0d word%0d data=%h",ticks,b,k,got);
        @(negedge d.Clock1_p_Ca); @(posedge d.Clock1_p_Ca);
        repeat(2) @(negedge sys_clk); in_n=1;
        repeat(4*SYSPER) @(negedge sys_clk);
        if(k<(b==0 ? 2 : 8) && got!==expected_word((b==0 ? 1 : 3)+k))
          $fatal(1,"checked-write prefix mismatch block%0d word%0d: %h",b,k,got);
        if(k==(b==0 ? 1 : 7)) begin
          // ACmmdCheck clears CompareErr BEFORE CheckECC drains the tail.
          // Clearing it after the ECC words is too late: hardware has
          // already latched a failed comparison and inhibited the write.
          put(8'o11,16'h2000); put(8'o11,16'h0000);
          @(negedge sys_clk); addr=8'o12;
          repeat(8*SYSPER) @(negedge sys_clk);
        end
      end
    end
    checked_headers_done=1;
    $display("CHECKPREFIX done @%0d",ticks);
    @(negedge sys_clk); addr=8'o12;
  end
  initial begin
    // ACmmdRead waits for RdFifoTW, not just an idle OutRegFull flag.
    // Reading the initial holding-register state fabricates an underflow.
    wait(read_armed); wait(d.Active); wait(d.RdFifoTW); addr=8'o12;
    forever begin
      wait(d.OutRegFull);
      // Input/error sampling uses Clock1'Ca, not the format-write preclock.
      // A pulse starting at the preclock spans TWO Ca edges and underflows
      // on the second one, after the requested word has already been read.
      @(posedge d.Clock1_p_Ca); repeat(2) @(negedge sys_clk);
      received_word=result_word; in_n=0;
      @(negedge d.Clock1_p_Ca); @(posedge d.Clock1_p_Ca);
      repeat(2) @(negedge sys_clk); in_n=1;
      // Input consumes the CURRENT OutReg word. Its later refill is the
      // following word, not the value returned by this Input.
      repeat(4*SYSPER) @(negedge sys_clk);
      if(trace_on) $display("STREAM DATA #%0d @%0d = %04h",words,ticks,received_word);
      expected_index=-1;
      if(words>=1 && words<=2) expected_index=words;
      if(words>=7 && words<=14) expected_index=words-4;
      if(words>=19 && words<=274) expected_index=words-8;
      if(expected_index>=0 && received_word!==expected_word(expected_index))
        $fatal(1,"controller payload word %0d is %h, expected %h",words,received_word,expected_word(expected_index));
      if((words==5 || words==6 || words==17 || words==18 || words==277 || words==278) && received_word!==16'h0)
        $fatal(1,"controller ECC remainder word %0d is %h",words,received_word);
      words=words+1;
    end
  end
  function [3:0] revn(input [3:0] v); revn={v[0],v[1],v[2],v[3]}; endfunction
  initial begin
    if(d.u_a05.TW != 3000*SYSPER/30)
      $fatal(1,"generated board did not scale the 3 us index timer with SYSPER");
    if(!$value$plusargs("pack=%s",path)) $fatal(1,"stream test requires +pack");
    void'($value$plusargs("streamsector=%d",stream_sector));
    if(stream_sector<0 || stream_sector>28) $fatal(1,"streamsector must be 0..28");
    stream_pulse=stream_sector==28 ? 0 : 4*(stream_sector+1);
    fd=$fopen(path,"rb");
    if(fd==0) $fatal(1,"cannot open stream test pack");
    rc=$fseek(fd,((3*5+4)*29+stream_sector)*534,0);
    for(i=0;i<534;i=i+1) expected_bytes[i]=$fgetc(fd);
    $fclose(fd);
    repeat(1024) @(negedge sys_clk); reset=0;
    put(8'o10,16'h0400); put(8'o10,16'h0400);
    put(8'o14,16'h8000); put(8'o14,16'h0000);
    put(8'o11,16'h0f00); put(8'o11,16'h0000);
    for(i=0;i<16;i=i+1) put(8'o13,FMT[i]);
    for(i=0;i<16;i=i+1)
      if ({revn(d.u_e16.mem[i]),revn(d.u_f16.mem[i]),revn(d.u_f17.mem[i])} !== FMT[i][11:0])
        $fatal(1,"stream format[%0d] did not match",i);
    $display("STREAM init EnableRun=%b RamAddr=%h",d.EnableRun,{d.RamAddr_0,d.RamAddr_1,d.RamAddr_2,d.RamAddr_3});
    if(!d.EnableRun) $fatal(1,"format load did not enable controller");
    // InitRamDiablo ends with clearAll through DiskMuff (HM 9.9 bits 4..7).
    // This resets FIFO/wakeup state as well as controller error latches.
    put(8'o11,16'h0f00); put(8'o11,16'h0000);
    put(8'o14,16'h08d0); put(8'o14,16'h88d0); put(8'o14,16'h08d0);
    repeat(2048) @(negedge sys_clk);
    put(8'o14,16'h08f0); put(8'o14,16'h88f0); put(8'o14,16'h08f0);
    repeat(2048) @(negedge sys_clk);
    put(8'o14,16'h0810); put(8'o14,16'h8810); put(8'o14,16'h0810);
    repeat(2048) @(negedge sys_clk);
    if($test$plusargs("pollselect") || timed_polls) begin
      // Real DSKInitPC probes head 5, resets status, then AltoLoop toggles
      // selection. These tags must not reprogram the subsector divider.
      send_tag(16'h2005);
      send_tag(16'h1008);
      if(timed_polls) wait(drv.rev_count > drv.CYCLES_PER_REV-10000);
      for(integer j=0;j<(timed_polls ? 13 : 8);j=j+1) begin
        if(timed_polls) wait(drv.sector==4*j && drv.sec_count>(j==0 ? 3250 : 1500));
        put(8'o14,16'h0810); put(8'o14,16'h8810); put(8'o14,16'h0810);
        if(timed_polls) repeat(2048) @(negedge sys_clk);
        put(8'o14,16'h0800); put(8'o14,16'h8800); put(8'o14,16'h0800);
        if({d.TriconD06_sil_pl_5,d.TriconD06_sil_pl_6,d.TriconD06_sil_pl_7,d.TriconD06_sil_pl_8}!==4'd3)
          $fatal(1,"drive-selection poll changed the programmed subsector reload");
      end
      put(8'o14,16'h0810); put(8'o14,16'h8810); put(8'o14,16'h0810);
    end
    // SendTag leaves the tag register asserted until its timed TagTW,
    // rather than withdrawing the tag with a following zero-tag output.
    send_tag(16'h4003);
    send_tag(16'h2004);
    // Arm just before index, after the drive-selection setup has settled.
    // The first revolution began before the format/divider was initialized.
    wait(drv.rev_count > drv.CYCLES_PER_REV-10000);
    if(stream_pulse!=0) begin
      // Issue during the preceding slot, well after its pulse. This tests
      // a normal divided sector pulse, not only the separate Index input.
      wait(drv.sector==stream_pulse-4 && drv.sec_count>10000);
    end
    if(write_test) begin
      // This isolated fixture has not read any valid FIFO words yet. Clear
      // startup status after the timing track and selection have settled,
      // using the documented Muffler operation, before testing a write.
      put(8'o11,16'h0100); put(8'o11,16'h0000);
      if(d.FifoParityErr) $fatal(1,"FIFO parity status did not clear before write");
    end
    put(8'o10,checked_write ? 16'h00a4 : write_test ? 16'h0054 : 16'h00fc);
    if(trace_on) $display("STREAM armed full=%b fifoempty=%b readtw=%b",d.OutRegFull,d.FifoEmpty,d.RdFifoTW);
    armed=1;
    read_armed=!write_test;
    wait(d.Active);
    $display("STREAM Active @%0d pulse=%0d",ticks,drv.sector);
    if(drv.sector!=stream_pulse)
      $fatal(1,"controller started at pulse %0d, expected %0d",drv.sector,stream_pulse);
    if(write_test) begin
      wait(writer_done); wait(write_blocks==(checked_write ? 1 : 3)); wait(!d.Active);
      if(d.IOBParityErr || d.FifoParityErr || d.FifoOverflow || d.WriteError)
        $fatal(1,"write controller error: IOB=%b FIFOpar=%b overflow=%b write=%b",
          d.IOBParityErr,d.FifoParityErr,d.FifoOverflow,d.WriteError);
      // Re-loading the track must restore the volatile changes, rather
      // than discard them or modify the original source file.
      send_tag(16'h4004); send_tag(16'h4003);
      for(integer b=(checked_write ? 2 : 0);b<3;b=b+1)
        for(integer k=0;k<(b==0 ? 2 : b==1 ? 8 : 256);k=k+1) begin
          integer wi;
          reg [15:0] expected;
          wi=(b==0 ? 1 : b==1 ? 3 : 11)+k;
          expected=written_word(b,k);
          if(pk.track_word(stream_sector*267+wi)!==expected)
            $fatal(1,"volatile write lost on seek: block%0d word%0d",b,k);
          expected_bytes[2*wi]=expected[7:0];
          expected_bytes[2*wi+1]=expected[15:8];
        end
      put(8'o11,16'h0f00); put(8'o11,16'h0000);
      wait(drv.rev_count>drv.CYCLES_PER_REV-10000);
      if(stream_pulse!=0) wait(drv.sector==stream_pulse-4 && drv.sec_count>10000);
      put(8'o10,16'h00fc); read_armed=1;
      wait(d.Active);
    end
    repeat(400000) @(negedge sys_clk);
    if(words!=279) $fatal(1,"controller delivered %0d disk words, expected 266 data + 12 ECC",words-1);
    if(d.IOBParityErr || d.FifoUnderflow || d.FifoOverflow || d.ReadDataErr)
      $fatal(1,"controller error after stream: IOB=%b underflow=%b overflow=%b ECC=%b",
        d.IOBParityErr,d.FifoUnderflow,d.FifoOverflow,d.ReadDataErr);
    $display("STREAM PASS -- 266 payload words and all six zero ECC remainder words through the real controller FIFO");
    if(write_test) $display("WRITEBACK PASS -- serial writes, Fire-code validation, seek persistence and controller readback");
    $finish;
  end
  reg wc_d=0;
  reg [22:0] last_io=0;
  reg under_d=0;
  reg parity_d=0;
  wire [15:0] sr={d.ShiftReg_00,d.ShiftReg_01,d.ShiftReg_02,d.ShiftReg_03,d.ShiftReg_04,d.ShiftReg_05,d.ShiftReg_06,d.ShiftReg_07,d.ShiftReg_08,d.ShiftReg_09,d.ShiftReg_10,d.ShiftReg_11,d.ShiftReg_12,d.ShiftReg_13,d.ShiftReg_14,d.ShiftReg_15};
  always @(posedge sys_clk) begin
    parity_d<=d.FifoParityErr;
    if(write_test && trace_on && d.FifoParityErr && !parity_d) begin
      $display("WRITEBACK PARITY @%0d SR=%h saved=%b%b computed=%b%b bad=%b sample'=%b data=%h ckEn=%b pre=%b",
        ticks,sr,d.OutPar_16,d.OutPar_17,d.ShiftReg_16,d.ShiftReg_17,d.dFifoParityErr,d.CkFifoParity_p_,result_word,d.u_c09.cka_en,d.u_c09.d_pre_a);
      $fflush();
    end
    if(write_test && armed && !read_armed &&
       (d.FifoParityErr || d.IOBParityErr || d.FifoUnderflow || d.FifoOverflow || d.WriteError))
      $fatal(1,"controller error during write: FIFOpar=%b IOBpar=%b under=%b over=%b write=%b read=%b dataErr=%b secOvfl=%b select=%b ready=%b ro=%b offset=%b compare'=%b checksum=%b",
        d.FifoParityErr,d.IOBParityErr,d.FifoUnderflow,d.FifoOverflow,d.WriteError,
        d.ReadError,d.ReadDataErr,d.SectorOvfl,d.NotSelected,d.NotReady,d.ReadOnly,d.CylOffset,d.CompareErr_p_,d.CheckSumErr);
    under_d<=d.FifoUnderflow;
    if(d.FifoUnderflow && !under_d)
      $display("STREAM UNDERFLOW @%0d word=%0d active=%b in'=%b full=%b empty=%b set=%b clk=%b pre=%b clear=%b",ticks,
        words,d.Active,in_n,d.OutRegFull,d.FifoEmpty,d.TriconD08_sil_pl_2,d.u_c15.clkb,d.u_c15.d_pre_b,d.ClearErrors);
    if(trace_on && addr==8'o11 && !out_n)
      $display("STREAM MUFF @%0d clk0=%b clk1=%b io=%b sample=%b tw=%b err=%b data=%h",ticks,
        d.Clock0_p_Da,d.Clock1_p_Cb,d.DskEth01_sil_pl_5,d.MufAdr_u_IOB_p_,d.ClearTWs,d.ClearErrors,data);
    if(trace_on && armed && words<4 &&
       {in_n,d.OutRegFull,d.FifoEmpty,d.OutRegWrite_p_,d.OutRegCl_p_A,d.Clock1_p_Ca,d.TriconD08_sil_pl_11,result_word}!==last_io) begin
      $display("STREAM IO @%0d in'=%b full=%b empty=%b write'=%b outclk'=%b clk1'=%b clear=%b data=%h",ticks,
        in_n,d.OutRegFull,d.FifoEmpty,d.OutRegWrite_p_,d.OutRegCl_p_A,d.Clock1_p_Ca,d.TriconD08_sil_pl_11,result_word);
      last_io={in_n,d.OutRegFull,d.FifoEmpty,d.OutRegWrite_p_,d.OutRegCl_p_A,d.Clock1_p_Ca,d.TriconD08_sil_pl_11,result_word};
    end
    wc_d<=d.WordClock_p_;
    if(trace_on && d.Active && d.WordClock_p_ && !wc_d)
      $display("STREAM @%0d pulse=%0d sr=%04h rd=%b ram=%h cnt=%h load'=%b done'=%b active=%b compute=%b shiftin=%b", ticks,drv.sector,sr,d.ReadData,
        {d.RamAddr_0,d.RamAddr_1,d.RamAddr_2,d.RamAddr_3},
        {d.u_c19.q,d.u_c20.q,d.u_c21.q},d.LoadCnt_p_,d.CntDone_p_,d.Active,d.ComputeECC,d.ShiftIn);
  end
  initial begin
    repeat(20000000) @(negedge sys_clk); $fatal(1,"stream watchdog: writer=%b blocks=%0d WrTW=%b FifoEmpty=%b",writer_done,write_blocks,d.WrFifoTW,d.FifoEmpty);
  end
endmodule
