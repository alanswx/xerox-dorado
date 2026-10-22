// Exact staged production native machine. External models provide only the
// SDRAM pins, normal PS/2 keys, raw Ethernet frames and HPS block transfers.
// Original BaseBoard EPROMs bootstrap IM; there are no state/IM/IOCB plants.
`timescale 1ns/1ps
module tb_native_cedar;
  import "DPI-C" function int native_host_open(input string medium_path,aemu,cedar,alto,root);
  import "DPI-C" function int unsigned native_pack_read(input int unsigned word_address);
  import "DPI-C" function void native_pack_write(input int unsigned word_address,value);
  import "DPI-C" function void native_net_tx_word(input int unsigned index,value);
  import "DPI-C" function void native_net_tx(input int unsigned words);
  import "DPI-C" function void native_net_tick(input longint unsigned host_ns);
  import "DPI-C" function int unsigned native_net_rx();
  import "DPI-C" function int unsigned native_net_rx_word(input int unsigned index);
  import "DPI-C" function int unsigned native_boot_request();
  import "DPI-C" function longint unsigned native_limit(input string path);
  // Match the qualified native build's actual fabric clock. The old50MHz
  // recipes retain exactly10ns half-periods and their original stimulus.
  localparam integer HOST_CLOCK_HZ=`DORADO_HOST_CLOCK_HZ;
  localparam realtime HALF_PERIOD_NS=500000000.0/HOST_CLOCK_HZ;
  localparam integer BOOT_DOWN_SAMPLES=4*HOST_CLOCK_HZ;
  localparam integer BOOT_UP_SAMPLES=24*HOST_CLOCK_HZ/5;
  localparam integer COOL_BOOT_UP_SAMPLES=6*HOST_CLOCK_HZ;
  localparam integer DISK_DELAY_CLOCKS=HOST_CLOCK_HZ*3/1000;
  reg host_clk=0;always #(HALF_PERIOD_NS) host_clk=~host_clk;
  reg reset=1;initial begin repeat(5)@(negedge host_clk);reset=0;end
  wire storage_ready,storage_initialized,pack_ready,pack_fault;
  wire source_enable=!reset && storage_ready && pack_ready;
  reg step=0;always @(negedge host_clk)step<=source_enable;
  wire machine_clk=host_clk && step;
  longint unsigned host_ticks=0,guest_ticks=0,limit;
  string limit_path,medium_path,aemu,cedar,alto,stp;
  integer boot_presses=1;
  reg [28:0] boot_samples=0;
  wire [28:0] boot_end_samples=29'(boot_presses==3 ? COOL_BOOT_UP_SAMPLES : BOOT_UP_SAMPLES);
  always @(posedge machine_clk)if(boot_samples<boot_end_samples)boot_samples<=boot_samples+1;
  // Original DoradoContinuous.masm: one press is a software HotBoot;
  // three presses select CoolBoot/LoadDoradoCode. Exercise only this normal
  // physical input, never a direct run/manifold/IM or processor-state plant.
  wire cool_boot_down=(boot_samples>=BOOT_DOWN_SAMPLES && boot_samples<22*HOST_CLOCK_HZ/5) ||
      (boot_samples>=24*HOST_CLOCK_HZ/5 && boot_samples<26*HOST_CLOCK_HZ/5) ||
      (boot_samples>=28*HOST_CLOCK_HZ/5 && boot_samples<COOL_BOOT_UP_SAMPLES);
  wire boot_n=!(boot_presses==3 ? cool_boot_down :
      (boot_samples>=BOOT_DOWN_SAMPLES && boot_samples<BOOT_UP_SAMPLES));
  reg [10:0] ps2_key=0;
  wire [10:0] machine_key;
  wire key_overflow;wire [8:0] key_queued;
  wire video,hsync,vsync_n,hblank,vblank,halfline,pixel_clk,video_field;
  dorado_key_queue keys(.clk_host(host_clk),.reset(reset),.ps2_key(ps2_key),
    .vsync_n(vsync_n),.machine_key(machine_key),.overflow(key_overflow),.queued(key_queued));
  initial begin
    if($value$plusargs("boot-presses=%d",boot_presses))begin
      if(boot_presses!=1 && boot_presses!=3)$fatal(1,"boot-presses must be 1 or 3");
    end
    $display("NATIVE_BOOT_INPUT presses=%0d clock_hz=%0d; original BaseBoard button only",boot_presses,HOST_CLOCK_HZ);
    if(!$value$plusargs("medium=%s",medium_path) || !$value$plusargs("aemu=%s",aemu) ||
       !$value$plusargs("cedar=%s",cedar) || !$value$plusargs("alto=%s",alto) ||
       !$value$plusargs("stp=%s",stp) || !$value$plusargs("limit-file=%s",limit_path))$fatal(1,"missing host artifact argument");
    if(!native_host_open(medium_path,aemu,cedar,alto,stp))$fatal(1,"host setup");
    limit=native_limit(limit_path);
    wait(!reset);repeat(3)@(negedge host_clk);
    ps2_key=11'h621; // normal Set2 C make, held before Initial chooses BFN
    while(native_boot_request()!=0113)repeat(1024)@(negedge host_clk);
    @(negedge host_clk);ps2_key=11'h021;
    $display("NATIVE_C_RELEASED host=%0d guest=%0d",host_ticks,guest_ticks);
  end
  wire [12:0] a;wire [1:0] ba;
  wire ncs,nras,ncas,nwe,dqml,dqmh,cke,dq_oe;
  wire [15:0] dq_out,dq;
  assign dq=dq_oe?dq_out:16'hzzzz;
  wire chip_clk=~host_clk;
  as4c32m16sb_6tin_chip_model #(.CHIP_NAME("NATIVE_RANK0"),
    .tRC_MIN(63.0),.tRFC_MIN(63.0),.tRCD_MIN(21.0),.tRP_MIN(21.0),
    .tRRD_MIN(14.0),.tMRD_MIN(14.0),.tWR_MIN(14.0),.tAC_MAX(6.0)) chip0(
    .Clk(chip_clk),.Cke(1'b1),.Cs_n(ncs),.Ras_n(nras),.Cas_n(ncas),.We_n(nwe),
    .Ba(ba),.Addr(a),.Ldqm(a[11]),.Udqm(a[12]),.Dq(dq));
  as4c32m16sb_6tin_chip_model #(.CHIP_NAME("NATIVE_RANK1"),
    .tRC_MIN(63.0),.tRFC_MIN(63.0),.tRCD_MIN(21.0),.tRP_MIN(21.0),
    .tRRD_MIN(14.0),.tMRD_MIN(14.0),.tWR_MIN(14.0),.tAC_MAX(6.0)) chip1(
    .Clk(chip_clk),.Cke(1'b1),.Cs_n(~ncs),.Ras_n(nras),.Cas_n(ncas),.We_n(nwe),
    .Ba(ba),.Addr(a),.Ldqm(a[11]),.Udqm(a[12]),.Dq(dq));
  reg ack=0,buff_wr=0;reg [12:0] buff_addr=0;reg [15:0] buff_data=0;
  wire [15:0] buff_q;wire [31:0] lba;wire [5:0] blocks;wire rd,wr;
  wire [31:0] fault_debug,fault_extra,pack_reads,pack_writes;
  wire [2:0] pack_phase;wire [8:0] storage_events;
  wire [127:0] operands;wire [31:0] bb_debug,cpu_debug;wire [9:0] control_debug;
`ifndef DORADO_OPERAND_MONITOR
  // Qualified MC1668 reference has no operand observer; keep its RTL unchanged.
  assign operands=128'd0;
`endif
  wire [4:0] instruction_signals;
  wire rx_line,tx_n;
  dorado_native_machine #(.SYSPER(4)) dut(
    .contb_advance(step),.sys_clk(machine_clk),.clk_host(host_clk),.reset(reset),
    .baseboard_boot_n(boot_n),.large_format(1'b1),.ethernet_host(8'o42),
    .ps2_key(machine_key),.mouse_keys(16'hffff),.mouse_dx(9'd0),.mouse_dy(9'd0),.mouse_pending(1'b0),
    .ethernet_rx(rx_line),.ethernet_tx_n(tx_n),
    .pack_attached(1'b1),.pack_readonly(1'b0),.pack_mounted(1'b0),
    .sd_ack(ack),.sd_buff_wr(buff_wr),.sd_buff_addr(buff_addr),.sd_buff_dout(buff_data),.sd_buff_din(buff_q),
    .sd_lba(lba),.sd_blk_cnt(blocks),.sd_rd(rd),.sd_wr(wr),.pack_ready(pack_ready),.pack_fault(pack_fault),
    .pack_fault_debug(fault_debug),.pack_fault_extra(fault_extra),.pack_host_phase(pack_phase),.pack_reads(pack_reads),.pack_writes(pack_writes),
    .storage_events(storage_events),.sdram_a(a),.sdram_ba(ba),.sdram_ncs(ncs),.sdram_nras(nras),.sdram_ncas(ncas),
    .sdram_nwe(nwe),.sdram_dqml(dqml),.sdram_dqmh(dqmh),.sdram_cke(cke),.sdram_dq_out(dq_out),.sdram_dq_oe(dq_oe),.sdram_dq_in(dq),
    .ddram_busy(1'b0),.ddram_rvalid(1'b0),.ddram_rdata(64'd0),
    .storage_ready(storage_ready),.storage_initialized(storage_initialized),
`ifdef DORADO_OPERAND_MONITOR
    .operand_debug(operands),
`endif
    .baseboard_debug(bb_debug),.processor_debug(cpu_debug),.control_debug(control_debug),
    .instruction_signals(instruction_signals),.video(video),.hsync(hsync),.vsync_n(vsync_n),.hblank(hblank),.vblank(vblank),
    .halfline(halfline),.pixel_clk(pixel_clk),.video_field(video_field));
  // Only HPS sectors are serviced here; no Pilot structures are decoded.
  integer disk_state=0,index=0,total=0,disk_delay=0;
  reg disk_write=0;reg [31:0] first_word=0;
  always @(negedge host_clk)if(!reset)begin
    buff_wr<=0;
    case(disk_state)
    0:if(rd || wr)begin
      first_word<=lba*256;total<=(int'(blocks)+1)*256;index<=0;disk_delay<=DISK_DELAY_CLOCKS;disk_write<=wr;disk_state<=1;
      $display("NATIVE_HPS_REQUEST host=%0d lba=%0d blocks=%0d write=%b",host_ticks,lba,int'(blocks)+1,wr);
    end
    1:if(disk_delay>0)disk_delay<=disk_delay-1;
      else begin buff_addr<=0;disk_state<=2;end
    2:begin
      if(disk_write)native_pack_write(first_word+index,int'(buff_q));
      else begin buff_addr<=13'(index);buff_data<=16'(native_pack_read(first_word+index));buff_wr<=1;end
      if(index+1==total)disk_state<=3;
      else begin index<=index+1;if(disk_write)buff_addr<=13'(index+1);end
    end
    3:begin ack<=1;disk_state<=4;end
    4:if(!rd && !wr)begin ack<=0;disk_state<=0;$display("NATIVE_HPS_ACK host=%0d",host_ticks);end
    endcase
  end
  reg rx_wr=0,rx_commit=0,tx_ack=0;
  reg [9:0] rx_addr=0,tx_addr=0;reg [15:0] rx_data=0;reg [10:0] rx_words=0;
  wire rx_ready,tx_valid;wire [10:0] tx_words;wire [15:0] tx_data;
  wire [31:0] tx_good,tx_bad,tx_dropped,rx_bad;
  dorado_ethernet_wire #(.SYSPER(4),.MAX_WORDS(1024)) wire_port(
    .clk(host_clk),.reset(reset),.step(step),.tx_line(~tx_n),.rx_line(rx_line),
    .rx_wr(rx_wr),.rx_commit(rx_commit),.rx_addr(rx_addr),.rx_data(rx_data),.rx_words(rx_words),.rx_ready(rx_ready),
    .tx_addr(tx_addr),.tx_data(tx_data),.tx_ack(tx_ack),.tx_valid(tx_valid),.tx_words(tx_words),
    .tx_good(tx_good),.tx_bad(tx_bad),.tx_dropped(tx_dropped),.rx_bad(rx_bad));
  integer ts=0,ti=0,rs=0,ri=0,n=0;
  always @(negedge host_clk)if(!reset)begin
    rx_wr<=0;rx_commit<=0;tx_ack<=0;
    case(ts)
    0:if(tx_valid)begin ti<=0;tx_addr<=0;ts<=1;end
    1:begin native_net_tx_word(ti,int'(tx_data));
      if(ti+1==tx_words)begin native_net_tx(int'(tx_words));tx_ack<=1;ts<=2;end
      else begin ti<=ti+1;tx_addr<=10'(ti+1);end
    end
    2:if(!tx_valid)ts<=0;
    endcase
    if((host_ticks%1024)==0)native_net_tick(host_ticks*64'd1000000000/HOST_CLOCK_HZ);
    case(rs)
    0:if(rx_ready)begin n=native_net_rx();if(n>0)begin rx_words<=11'(n);ri<=0;rs<=1;end end
    1:begin rx_addr<=10'(ri);rx_data<=16'(native_net_rx_word(ri));rx_wr<=1;
      if(ri+1==rx_words)rs<=2;else ri<=ri+1;
    end
    2:begin rx_commit<=1;rs<=3;end
    3:if(!rx_ready)rs<=0;
    endcase
  end
  // A retained ring captures actual native pins and phases before the first
  // persistent queue search. It never drives any machine signal.
  reg [159:0] ring[0:4095];integer ring_pos=0,queue_visits=0;
  reg [31:0] previous_cpu=0;
  reg first_trap_dumped=0;
  wire [11:0] cia=cpu_debug[23:12];wire [3:0] task_id=cpu_debug[27:24];
  always @(negedge host_clk)begin
    host_ticks<=host_ticks+1;
    if(step)begin
      guest_ticks<=guest_ticks+1;ring[ring_pos]<={cpu_debug,operands};ring_pos<=(ring_pos+1)%4096;
      if(task_id==0 && cpu_debug!=previous_cpu)begin
        if(cia==12'h6c1)queue_visits<=queue_visits+1;
        else if(cia!=12'h6c2 && cia!=12'h239 && cia!=12'h23a)queue_visits<=0;
      end
      previous_cpu<=cpu_debug;
    end
    if((host_ticks%10000000)==0)begin
      limit=native_limit(limit_path);
      $display("NATIVE_PROGRESS host=%0d guest=%0d initialized=%b init_addr=%h cpu=%h bb=%h control=%h keys=%0d disk=%d/%d fault=%h:%h tx=%0d crc=%0d rx=%0d",host_ticks,guest_ticks,storage_initialized,dut.init_address,cpu_debug,bb_debug,control_debug,key_queued,pack_reads,pack_writes,fault_debug,fault_extra,tx_good,tx_bad,rx_bad);
    end
    if(!reset && (pack_fault || key_overflow || tx_bad || tx_dropped || rx_bad))$fatal(1,"native device fault");
    if(chip0.error_count || chip1.error_count || chip0.warning_count || chip1.warning_count)$fatal(1,"independent SDRAM model diagnostic");
    if(!first_trap_dumped && native_boot_request()==0113 && task_id==0 && cia==12'h221)begin
      first_trap_dumped<=1;
      for(integer j=0;j<4096;j++)$display("NATIVE_FIRST_TRAP_RING %040h",ring[(ring_pos+j)%4096]);
      $display("NATIVE_FIRST_TRAP host=%0d machine_edges=%0d cpu=%h control=%h",host_ticks,guest_ticks,cpu_debug,control_debug);
    end
    if(queue_visits==128 || host_ticks>=limit)begin
      for(integer j=0;j<4096;j++)$display("NATIVE_PIN_RING %040h",ring[(ring_pos+j)%4096]);
      $display("NATIVE_DIAGNOSTIC_STOP reason=%s host=%0d guest=%0d cpu=%h",queue_visits==128?"QUEUE_SEARCH":"HOST_LIMIT",host_ticks,guest_ticks,cpu_debug);
      $finish;
    end
  end
endmodule
