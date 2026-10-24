`timescale 1ns/1ps
// Independent absolute elapsed-time reference; compare both ordered edges,
// endpoint coincidence and frozen/reset state, not just average frequency.
module oscillator_events_case #(parameter SAMPLES=4,FREQUENCY=50000)
 (output reg done=0);
 reg clk=0;always #(30.0/SAMPLES)clk=~clk;
 reg reset=1,advance=0;
 wire [1:0] count;wire rise,fall,rise_first,at_tick,before_level,after_level;
 wire [31:0] first_offset,second_offset,interval_units;
 dorado_oscillator_events #(.SAMPLES_PER_INSTRUCTION(SAMPLES),.FREQUENCY_KHZ(FREQUENCY)) dut(
  .clk(clk),.reset(reset),.advance(advance),.edge_count(count),.rising(rise),.falling(fall),
  .rising_first(rise_first),.last_at_tick(at_tick),.level_before(before_level),.level_after(after_level),
  .first_offset_units(first_offset),.second_offset_units(second_offset),.interval_units(interval_units));
 longint unsigned elapsed_ps=0,total_edges=0,next_edges,next_ps,product;
 integer due,one=0,two=0,paused=0,coincident=0,epochs=0;
 reg expected_before=0,expected_after;
 initial begin
  repeat(2)@(negedge clk);
  for(integer i=0;i<4096;i++)begin
   reset=(i==0 || i==997 || i==2543);
   advance=(i%11<8);
   #0.001;
   if(reset)begin
    if(count!=0 || rise || fall || rise_first || at_tick)$fatal(1,"OSC_EVENTS reset exposed an event");
    if(first_offset || second_offset)$fatal(1,"OSC_EVENTS reset exposed timestamp");
    next_ps=0;next_edges=0;expected_after=0;epochs++;
   end else begin
    next_ps=elapsed_ps+(advance ? 60000/SAMPLES : 0);
    // frequency(kHz) *2 * time(ps) /1e9 = exact number of ideal transitions.
    product=next_ps*2*FREQUENCY;next_edges=product/1000000000;
    due=int'(next_edges-total_edges);expected_after=next_edges[0];
    if(before_level!==expected_before || count!==2'(due) || after_level!==expected_after)
     $fatal(1,"OSC_EVENTS count/state samples=%0d frequency=%0d step=%0d due=%0d actual=%0d",SAMPLES,FREQUENCY,i,due,count);
    if(rise!==((due==2)||(due==1&&!expected_before)) ||
       fall!==((due==2)||(due==1&&expected_before)) ||
       rise_first!==((due!=0)&&!expected_before))
     $fatal(1,"OSC_EVENTS edge order samples=%0d frequency=%0d step=%0d",SAMPLES,FREQUENCY,i);
    if(at_tick!==((due!=0)&&(product%1000000000==0)))$fatal(1,"OSC_EVENTS endpoint coincidence");
    if(interval_units!=120*FREQUENCY)$fatal(1,"OSC_EVENTS timestamp denominator");
    // Compare rational timestamps against absolute ideal transition times.
    // Avoid rounding the23530kHz reference's sub-picosecond edge positions.
    if(due>0)begin
     if(64'(first_offset)*(60000/SAMPLES)*2*FREQUENCY !=
        ((total_edges+1)*1000000000-elapsed_ps*2*FREQUENCY)*interval_units)
      $fatal(1,"OSC_EVENTS edge timestamp");
    end else if(first_offset!=0)$fatal(1,"OSC_EVENTS held timestamp");
    if(due==2)begin
     if(64'(second_offset)*(60000/SAMPLES)*2*FREQUENCY !=
        ((total_edges+2)*1000000000-elapsed_ps*2*FREQUENCY)*interval_units)
      $fatal(1,"OSC_EVENTS second edge timestamp");
    end else if(second_offset!=0)$fatal(1,"OSC_EVENTS spurious second timestamp");
    if(!advance)paused++;
    if(due==1)one++;if(due==2)two++;if(at_tick)coincident++;
   end
   @(posedge clk);#0.001;
   elapsed_ps=next_ps;total_edges=next_edges;expected_before=expected_after;
   if(before_level!==expected_before)$fatal(1,"OSC_EVENTS committed phase");
   @(negedge clk);
  end
  if(paused<100 || epochs!=3 || one==0 ||
     (SAMPLES==4 && FREQUENCY==50000 && (two==0 || coincident==0)))
   $fatal(1,"OSC_EVENTS incomplete coverage");
  $display("OSC_EVENTS_PASS samples=%0d frequency=%0d single=%0d double=%0d paused=%0d endpoint=%0d epochs=%0d",
   SAMPLES,FREQUENCY,one,two,paused,coincident,epochs);done=1;
 end
endmodule

module tb_oscillator_events;
 wire [11:0] done;
 genvar s,f;
 generate for(s=0;s<3;s++)begin:sample_case
  for(f=0;f<4;f++)begin:frequency_case
   oscillator_events_case #(.SAMPLES(4<<s),.FREQUENCY(f==0?10000:f==1?20000:f==2?23530:50000))
    test_case(done[4*s+f]);
  end
 end endgenerate
 initial begin wait(&done);$display("OSC_EVENTS_ALL_PASS cases=12");$finish;end
endmodule

// Probe the actual native active pixel generator at both current/target ratios.
// This intentionally demonstrates a rejected target rate; it changes no cell.
module native_square_wave_case #(parameter SAMPLES=4)(output reg done=0);
 reg clk=0;always #(30.0/SAMPLES)clk=~clk;
 wire pixel;
 cell_K1115A #(.FREQ_KHZ(50000),.SYSPER(SAMPLES)) actual(
  .sys_clk(clk),.p8(pixel),.p7(1'b0),.p14(1'b1));
 integer edges=0,escapes=0;reg previous=0;
 initial begin
  for(integer i=0;i<SAMPLES*1000;i++)begin
   @(posedge clk);#0.001;
   if(pixel!=previous)edges++;
   if(actual.acc<0 || actual.acc>=actual.SYS_KHZ)escapes++;
   previous=pixel;
  end
  if(SAMPLES==8 && (edges<5999 || edges>6001 || escapes!=0))$fatal(1,"OSC_NATIVE current clock ratio regressed");
  if(SAMPLES==4 && (edges!=4000 || escapes==0))$fatal(1,"OSC_NATIVE target-rate counterexample missing");
  $display("OSC_NATIVE_SQUARE samples=%0d expected_edges=6000 actual_edges=%0d remainder_escapes=%0d",SAMPLES,edges,escapes);
  done=1;
 end
endmodule
module tb_native_square_wave;
 wire [1:0] done;
 native_square_wave_case #(.SAMPLES(4)) target_case(done[0]);
 native_square_wave_case #(.SAMPLES(8)) current_case(done[1]);
 initial begin wait(&done);$display("OSC_NATIVE_CAPABILITY_PASS");$finish;end
endmodule
