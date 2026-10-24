`timescale 1ns/1ps
`default_nettype none
module clock_event_batch_case #(parameter integer PHASE=0)(output reg done=0);
 reg clk=0;always #7.5 clk=~clk;
 reg reset=1,advance=0;
 wire [3:0] before_levels,after_levels;
 wire [2:0] count;
 wire [15:0] offsets,masks,levels;
 dorado_clock_event_batch #(.INITIAL_CPU_PHASE(PHASE)) dut(
  .clk(clk),.reset(reset),.advance(advance),.levels_before(before_levels),.levels_after(after_levels),
  .event_count(count),.offsets(offsets),.masks(masks),.levels(levels));
 // Original four-flop wiring, not the candidate's phase lookup functions.
 // Packed{VCO,g04,h04,g03,h03}; the03 pair clocks on the opposite VCO edge.
 function automatic [4:0] divider_step(input [4:0] s);
  reg [4:0] n;
  begin
   n=s;n[4]=!s[4];
   if(s[4])begin n[1]=s[3];n[0]=s[1];end
   else begin n[3]=!s[2];n[2]=s[3];end
   divider_step=n;
  end
 endfunction
 function automatic [4:0] initial_divider;
  reg [4:0] s;
  begin s=5'b10001;for(integer i=0;i<PHASE;i++)s=divider_step(s);initial_divider=s;end
 endfunction
 function automatic [3:0] source_levels(input [4:0] s,input pixel);
  source_levels={pixel,!s[3]|s[0],s[0],!s[3]};
 endfunction
 reg [4:0] divider,next_divider;
 reg pixel=0,next_pixel;
 longint unsigned elapsed=0,next_elapsed;
 reg [3:0] cursor,next_levels,changed;
 reg [15:0] expected_offsets,expected_masks,expected_levels;
 integer expected_count,checks=0,paused=0,coincident=0,full_batches=0,epochs=0;
 initial begin
  divider=initial_divider();
  repeat(2)@(negedge clk);
  for(integer i=0;i<2048;i++)begin
   reset=(i==0 || i==509 || i==1313);
   advance=(i%11<8);
   #0.001;
   cursor=source_levels(divider,pixel);next_divider=divider;next_pixel=pixel;next_elapsed=elapsed;
   expected_count=0;expected_offsets=0;expected_masks=0;expected_levels=0;
   if(advance && !reset)for(integer step=1;step<=12;step++)begin
    next_elapsed=next_elapsed+1;
    if(next_elapsed%3==0)next_divider=divider_step(next_divider);
    if(next_elapsed%8==0)next_pixel=!next_pixel;
    next_levels=source_levels(next_divider,next_pixel);changed=cursor^next_levels;
    if(changed!=0)begin
     if(expected_count>=4)$fatal(1,"CLOCK_BATCH reference capacity exceeded");
     expected_offsets[4*expected_count+:4]=4'(step);
     expected_masks[4*expected_count+:4]=changed;
     expected_levels[4*expected_count+:4]=next_levels;
     if(changed[3] && |changed[2:0])coincident++;
     expected_count++;
    end
    cursor=next_levels;
   end
   if(before_levels!==source_levels(divider,pixel) || after_levels!==cursor ||
      count!==3'(expected_count) || {offsets,masks,levels}!=={expected_offsets,expected_masks,expected_levels})
    $fatal(1,"CLOCK_BATCH timeline mismatch phase=%0d interval=%0d actual=%0d/%h/%h/%h expected=%0d/%h/%h/%h",
     PHASE,i,count,offsets,masks,levels,expected_count,expected_offsets,expected_masks,expected_levels);
   if(!advance && !reset)paused++;
   if(count==4)full_batches++;
   checks++;
   @(posedge clk);#0.001;
   if(reset)begin divider=initial_divider();pixel=0;elapsed=0;epochs++;end
   else begin divider=next_divider;pixel=next_pixel;elapsed=next_elapsed;end
   if(before_levels!==source_levels(divider,pixel))$fatal(1,"CLOCK_BATCH committed phase mismatch");
   @(negedge clk);
  end
  if(checks!=2048 || paused<100 || epochs!=3 ||
     ((PHASE%4==0 || PHASE%4==3) && (full_batches==0 || coincident!=0)) ||
     ((PHASE%4==1 || PHASE%4==2) && (coincident==0 || full_batches!=0)))
   $fatal(1,"CLOCK_BATCH incomplete coverage phase=%0d coincident=%0d full=%0d",PHASE,coincident,full_batches);
  $display("CLOCK_BATCH_PASS phase=%0d checks=%0d paused=%0d coincident=%0d full=%0d resets=%0d",PHASE,checks,paused,coincident,full_batches,epochs);
  done=1;
 end
endmodule
module tb_clock_event_batch;
 wire [7:0] done;
 for(genvar p=0;p<8;p++)begin: rotations
  clock_event_batch_case #(.PHASE(p)) test_case(done[p]);
 end
 initial begin wait(&done);$display("CLOCK_BATCH_ALL_PASS rotations=8");$finish;end
endmodule
`default_nettype wire
