// Passive transport accounting on the exact qualified5c65 native hierarchy.
// Host ticks, enabled fabric edges and StartCycle issues are separate units.
// No machine state is planted; all requests come from the original128 groups.
  longint unsigned traffic_ticks=0,traffic_issues=0,traffic_reads=0,traffic_writes=0;
  longint unsigned traffic_accept=0,traffic_complete=0,traffic_first_latency=0,traffic_response_latency=0;
  longint unsigned traffic_classes[0:7],traffic_fanout[0:128],traffic_raw_blocks[0:5];
  longint unsigned traffic_group[0:127],traffic_offer_wait[0:127];
  longint unsigned traffic_started=0;
  reg traffic_active=0,traffic_first_seen=0,traffic_issue_previous=0;
  reg [22:0] traffic_address=0;
  reg traffic_writing=0;
  integer traffic_first_count=0,traffic_captured=0;
  initial begin
    for(integer j=0;j<8;j++)traffic_classes[j]=0;
    for(integer j=0;j<129;j++)traffic_fanout[j]=0;
    for(integer j=0;j<6;j++)traffic_raw_blocks[j]=0;
    for(integer j=0;j<128;j++)begin traffic_group[j]=0;traffic_offer_wait[j]=0;end
  end
  always @(posedge host_clk)begin
    traffic_ticks++;
    // Mutually exclusive actual gate decisions at the host sampling edge.
    if(reset)traffic_classes[0]++;
    else if(!storage_initialized)traffic_classes[1]++;
    else if(step)traffic_classes[2]++;
    else if(!pack_ready && !storage_ready)traffic_classes[3]++;
    else if(!pack_ready)traffic_classes[4]++;
    else if(!storage_ready)traffic_classes[5]++;
    else traffic_classes[6]++;
    if(step && instruction_signals[4] && !traffic_issue_previous)traffic_issues++;
    traffic_issue_previous=instruction_signals[4];
    if(storage_initialized && !reset)begin
      traffic_fanout[$countones(dut.valid)]++;
      for(integer g=0;g<128;g++)
        if(dut.valid[g] && !dut.accept[g])traffic_offer_wait[g]++;
      if(dut.rv && !dut.rr)begin
        if(dut.u_sdram.state==6)traffic_raw_blocks[0]++;
        else if(dut.u_sdram.refresh_due || dut.u_sdram.state>=8)traffic_raw_blocks[1]++;
        else if(dut.u_sdram.state==7)traffic_raw_blocks[2]++;
        else if(dut.u_sdram.response_pending && !dut.sr)traffic_raw_blocks[3]++;
        else traffic_raw_blocks[4]++;
      end else traffic_raw_blocks[5]++;
      if(dut.sv && traffic_active && !traffic_first_seen)begin
        traffic_first_latency+=host_ticks-traffic_started;traffic_first_seen=1;traffic_first_count++;
      end
      if(dut.sv && dut.sr)begin
        if(!traffic_active)$fatal(1,"NATIVE_TRAFFIC orphan response");
        traffic_complete++;traffic_response_latency+=host_ticks-traffic_started;traffic_active=0;
        if(traffic_captured<8192)
          $display("NATIVE_TRAFFIC_R tick=%0d address=%h write=%b data=%h latency=%0d pc=%h",host_ticks,traffic_address,traffic_writing,dut.sd,host_ticks-traffic_started,cpu_debug);
      end
      if(dut.rv && dut.rr)begin
        if(traffic_active)$fatal(1,"NATIVE_TRAFFIC uncompleted raw owner");
        traffic_active=1;traffic_first_seen=0;traffic_started=host_ticks;
        traffic_address=dut.ra;traffic_writing=dut.rw;traffic_accept++;traffic_group[dut.ra[6:0]]++;
        if(dut.rw)traffic_writes++;else traffic_reads++;
        if(traffic_captured<8192)begin
          $display("NATIVE_TRAFFIC_A tick=%0d address=%h write=%b data=%h mask=%h state=%0d fanout=%0d pc=%h",host_ticks,dut.ra,dut.rw,dut.wd,dut.valid,dut.u_sdram.state,$countones(dut.valid),cpu_debug);
          traffic_captured++;
        end
      end
    end
    if(host_ticks%10000000==0)
      $display("NATIVE_TRAFFIC_PROGRESS host=%0d issues=%0d admitted=%0d completed=%0d reads=%0d writes=%0d BFN=%o",host_ticks,traffic_issues,traffic_accept,traffic_complete,traffic_reads,traffic_writes,native_boot_request());
  end
  final begin
    $display("NATIVE_TRAFFIC_TOTAL ticks=%0d issues=%0d accepted=%0d completed=%0d reads=%0d writes=%0d first_count=%0d first_latency_sum=%0d consume_latency_sum=%0d trace_limit=8192",traffic_ticks,traffic_issues,traffic_accept,traffic_complete,traffic_reads,traffic_writes,traffic_first_count,traffic_first_latency,traffic_response_latency);
    for(integer j=0;j<8;j++)$display("NATIVE_TRAFFIC_GATE class=%0d ticks=%0d",j,traffic_classes[j]);
    for(integer j=0;j<6;j++)$display("NATIVE_TRAFFIC_RAW_BLOCK class=%0d ticks=%0d",j,traffic_raw_blocks[j]);
    for(integer j=0;j<129;j++)$display("NATIVE_TRAFFIC_FANOUT pending=%0d ticks=%0d",j,traffic_fanout[j]);
    for(integer j=0;j<128;j++)$display("NATIVE_TRAFFIC_GROUP group=%0d requests=%0d offer_wait=%0d",j,traffic_group[j],traffic_offer_wait[j]);
  end
