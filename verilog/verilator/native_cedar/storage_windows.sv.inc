// Simulation-only, passive windows on the actual native128 storage interface.
// No assignment targets DUT state or any stimulus. External control selects
// only WHEN to record: "epoch trigger" (0 immediate,1 first issue,2 Cedar113).
  string window_control_path;
  integer window_limit=8192,window_poll=65536;
  integer window_requested=1,window_trigger=2,window_id=0,window_count=0;
  integer window_fd,window_scan,window_file_id,window_file_trigger;
  reg window_open=0,window_owned=0,window_first_seen=0,window_ever_issued=0;
  longint unsigned window_begin_tick=0,window_request_tick=0,window_ticks=0;
  longint unsigned window_gate[0:6],window_block[0:8],window_fanout[0:128];
  longint unsigned window_group[0:127],window_front_wait[0:127],window_read_class[0:5];
  longint unsigned window_replies=0,window_first_count=0,window_first_sum=0,window_consume_sum=0;
  longint unsigned window_row_hit=0,window_row_closed=0,window_row_conflict=0;
  longint unsigned window_issues=0;
  reg window_issue_previous=0;
  reg [22:0] window_address=0;
  reg [15:0] window_data=0;
  reg window_write=0;
  wire window_raw_accept=storage_initialized && !reset && dut.rv && dut.rr;
  wire window_raw_reply=storage_initialized && !reset && dut.sv && dut.sr;
  initial begin
    if($value$plusargs("window-control=%s",window_control_path))begin end
    if($value$plusargs("window-limit=%d",window_limit))begin end
    if($value$plusargs("window-poll=%d",window_poll))begin end
    if(window_limit<1 || window_limit>8192 || window_poll<1 || window_poll>65536)
      $fatal(1,"NATIVE_WINDOW invalid observer settings");
    $display("NATIVE_WINDOW_ARMED limit=%0d poll=%0d default_trigger=Cedar113",window_limit,window_poll);
  end
  task automatic window_clear;
    begin
      window_count=0;window_replies=0;window_ticks=0;window_first_count=0;
      window_first_sum=0;window_consume_sum=0;window_issues=0;
      window_row_hit=0;window_row_closed=0;window_row_conflict=0;
      for(integer j=0;j<7;j++)window_gate[j]=0;
      for(integer j=0;j<9;j++)window_block[j]=0;
      for(integer j=0;j<129;j++)window_fanout[j]=0;
      for(integer j=0;j<128;j++)begin window_group[j]=0;window_front_wait[j]=0;end
      for(integer j=0;j<6;j++)window_read_class[j]=0;
    end
  endtask
  function automatic void window_report(input string status);
    longint unsigned gate_sum,block_sum,fanout_sum,request_sum,class_sum;
    begin
      gate_sum=0;block_sum=0;fanout_sum=0;request_sum=0;class_sum=0;
      for(integer j=0;j<7;j++)gate_sum+=window_gate[j];
      for(integer j=0;j<9;j++)block_sum+=window_block[j];
      for(integer j=0;j<129;j++)fanout_sum+=window_fanout[j];
      for(integer j=0;j<128;j++)request_sum+=window_group[j];
      for(integer j=0;j<6;j++)class_sum+=window_read_class[j];
      if(gate_sum!=window_ticks || block_sum!=window_ticks || fanout_sum!=window_ticks ||
         request_sum!=window_count || class_sum!=window_count)
        $fatal(1,"NATIVE_WINDOW counter partition mismatch");
      $display("NATIVE_WINDOW_%s id=%0d begin=%0d end=%0d ticks=%0d admitted=%0d completed=%0d first_count=%0d first_sum=%0d consume_sum=%0d issues=%0d pending=%b row_hit=%0d row_closed=%0d row_conflict=%0d",status,window_id,window_begin_tick,host_ticks,window_ticks,window_count,window_replies,window_first_count,window_first_sum,window_consume_sum,window_issues,window_owned,window_row_hit,window_row_closed,window_row_conflict);
      for(integer j=0;j<7;j++)$display("NATIVE_WINDOW_GATE id=%0d class=%0d ticks=%0d",window_id,j,window_gate[j]);
      for(integer j=0;j<9;j++)$display("NATIVE_WINDOW_BLOCK id=%0d class=%0d ticks=%0d",window_id,j,window_block[j]);
      for(integer j=0;j<129;j++)if(window_fanout[j])$display("NATIVE_WINDOW_FANOUT id=%0d pending=%0d ticks=%0d",window_id,j,window_fanout[j]);
      for(integer j=0;j<128;j++)if(window_group[j] || window_front_wait[j])
        $display("NATIVE_WINDOW_GROUP id=%0d group=%0d admitted=%0d offer_wait=%0d",window_id,j,window_group[j],window_front_wait[j]);
      for(integer j=0;j<6;j++)$display("NATIVE_WINDOW_READ_CLASS id=%0d class=%0d admitted=%0d",window_id,j,window_read_class[j]);
    end
  endfunction
  always @(posedge host_clk)begin
    if(window_control_path!="" && host_ticks%window_poll==0)begin
      window_fd=$fopen(window_control_path,"r");
      if(window_fd!=0)begin
        window_scan=$fscanf(window_fd,"%d %d",window_file_id,window_file_trigger);$fclose(window_fd);
        if(window_scan==2 && window_file_id>window_id && window_file_trigger>=0 && window_file_trigger<=2)begin
          window_requested=window_file_id;window_trigger=window_file_trigger;
        end
      end
    end
    if(step && instruction_signals[4])window_ever_issued=1;
    if(!window_open && window_raw_accept && window_requested>window_id)begin
      // Poll the DPI boot state procedurally. A no-argument function in a
      // continuous wire has no HDL input event when the host state changes.
      if(window_trigger==0 || (window_trigger==1 && window_ever_issued) ||
         (window_trigger==2 && native_boot_request()==32'o113))begin
        window_clear();window_id=window_requested;window_open=1;window_begin_tick=host_ticks;
        $display("NATIVE_WINDOW_BEGIN id=%0d tick=%0d trigger=%0d BFN=%o pc=%h limit=%0d",window_id,host_ticks,window_trigger,native_boot_request(),cpu_debug,window_limit);
      end
    end
    if(window_open)begin
      window_ticks++;
      if(reset)window_gate[0]++;
      else if(!storage_initialized)window_gate[1]++;
      else if(step)window_gate[2]++;
      else if(!pack_ready && !storage_ready)window_gate[3]++;
      else if(!pack_ready)window_gate[4]++;
      else if(!storage_ready)window_gate[5]++;
      else window_gate[6]++;
      window_fanout[$countones(dut.valid)]++;
      for(integer g=0;g<128;g++)if(dut.valid[g] && !dut.accept[g])window_front_wait[g]++;
      if(step && instruction_signals[4] && !window_issue_previous)window_issues++;
      if(window_raw_accept)window_block[0]++;
      else if(dut.rv && !dut.rr && dut.u_sdram.state==6)window_block[1]++;
      else if(dut.rv && !dut.rr && (dut.u_sdram.refresh_due || dut.u_sdram.state>=8))window_block[2]++;
      else if(dut.rv && !dut.rr && dut.u_sdram.state==7)window_block[3]++;
      else if(dut.rv && !dut.rr && (dut.u_sdram.state==3 || dut.u_sdram.state==4 || dut.u_sdram.state==5))window_block[4]++;
      else if(dut.sv && !dut.sr)window_block[5]++;
      else if(window_owned && !window_raw_reply)window_block[6]++;
      else if(dut.valid!=0)window_block[7]++;
      else window_block[8]++;
      if(window_owned && dut.sv && !window_first_seen)begin
        window_first_count++;window_first_sum+=host_ticks-window_request_tick;window_first_seen=1;
      end
      if(window_owned && window_raw_reply)begin
        $display("NATIVE_WINDOW_R id=%0d tick=%0d address=%h write=%b data=%h latency=%0d pc=%h",window_id,host_ticks,window_address,window_write,dut.sd,host_ticks-window_request_tick,cpu_debug);
        window_replies++;window_consume_sum+=host_ticks-window_request_tick;window_owned=0;
      end
      if(window_raw_accept && window_count<window_limit)begin
        if(window_owned)$fatal(1,"NATIVE_WINDOW overwritten captured raw owner");
        window_owned=1;window_first_seen=0;window_request_tick=host_ticks;
        window_address=dut.ra;window_write=dut.rw;window_data=dut.wd;
        window_count++;window_group[dut.ra[6:0]]++;
        if(dut.rw)window_read_class[5]++;
        else if(dut.u_sdram.lookup_hit)window_read_class[0]++;
        else if(dut.u_sdram.state==6 && dut.ra[22:3]==dut.u_sdram.address[22:3])begin
          if(dut.ra[2:0]<dut.u_sdram.fill_beat)window_read_class[1]++;
          else if(dut.ra[2:0]==dut.u_sdram.fill_beat)window_read_class[2]++;
          else window_read_class[3]++;
        end else window_read_class[4]++;
        // Row availability at admission, only for work requiring a command.
        // Native23-bit backing occupies rank0/bank0; this is not an IO waiver.
        if(dut.rw || (!dut.u_sdram.lookup_hit && dut.u_sdram.state!=6))begin
          if(!dut.u_sdram.open[0])window_row_closed++;
          else if(dut.u_sdram.rows[0]==dut.ra[22:10])window_row_hit++;
          else window_row_conflict++;
        end
        $display("NATIVE_WINDOW_A id=%0d tick=%0d address=%h write=%b data=%h mask=%h state=%0d fanout=%0d pc=%h",window_id,host_ticks,dut.ra,dut.rw,dut.wd,dut.valid,dut.u_sdram.state,$countones(dut.valid),cpu_debug);
      end
      if(reset)begin window_report("ABORT");window_open=0;window_owned=0;end
      else if(window_count==window_limit && !window_owned)begin
        if(window_replies!=window_count)$fatal(1,"NATIVE_WINDOW missing completion at cap");
        window_report("END");window_open=0;
      end
    end
    window_issue_previous=instruction_signals[4];
  end
  final if(window_open)window_report("PARTIAL");
