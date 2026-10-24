`timescale 1ns/1ps
module tb_baseboard_clock_events;
 reg clk=0,reset=1,advance=0,consume=0,fixture_load=1;
 reg [5:0] owners[0:7],fixture_states[0:7];
 reg [6:0] oracle_packet[0:7];
 reg debug_before[0:7],boundary_debug[0:7];
 reg [5:0] event_debug[0:7];
 wire [5:0] before_sources[0:7],next_state[0:7],oracle_state[0:7];
 wire [2:0] counts[0:7];
 wire [35:0] source_levels[0:7];
 wire [41:0] state_steps[0:7];
 wire [48:0] edge_steps[0:7];
 wire [6:0] roots[0:7],maxes[0:7],pres[0:7],auxes[0:7];
 wire [7:0] invalid,oracle_root,oracle_max,oracle_pre,oracle_aux;
 wire [223:0] oracle_counts[0:7];
 wire [194:0] producer[0:7];
 reg [194:0] expected[0:159999];
 reg [223:0] full_begin[0:7];
 string expected_path;
 integer samples=0,step_checks=0,hold_checks=0,restores=0;
 for(genvar p=0;p<8;p++)begin: phases
  wire [5:0] after_sources;
  wire [107:0] offsets;
  wire [35:0] masks;
  dorado_primary_clock_events #(.INITIAL_CPU_PHASE(p)) source(
   clk,reset,advance,before_sources[p],after_sources,counts[p],offsets,masks,source_levels[p]);
  assign producer[p]={before_sources[p],after_sources,counts[p],offsets,masks,source_levels[p]};
  dorado_baseboard_clock_step consumer(owners[p],before_sources[p],counts[p],source_levels[p],
   debug_before[p],boundary_debug[p],event_debug[p],consume,next_state[p],state_steps[p],edge_steps[p],
   roots[p],maxes[p],pres[p],auxes[p],invalid[p]);
  baseboard_clock_original reference(fixture_load,fixture_states[p],oracle_packet[p],oracle_state[p],
   oracle_root[p],oracle_max[p],oracle_pre[p],oracle_aux[p],oracle_counts[p]);
 end
 task pulse;
  begin clk=0;#1;clk=1;#1;clk=0;#1;end
 endtask
 task restart(input integer state_value);
  begin
   fixture_load=1;reset=1;advance=0;consume=0;pulse;reset=0;#2;
   for(integer p=0;p<8;p++)begin
    fixture_states[p]=6'(state_value);owners[p]=6'(state_value);
    oracle_packet[p]={1'b0,before_sources[p]};
    debug_before[p]=0;boundary_debug[p]=0;event_debug[p]=0;
   end
   #2;fixture_load=0;#2;fixture_load=1;#2;fixture_load=0;#2;
   for(integer p=0;p<8;p++)begin
    if(oracle_state[p] !== owners[p])$fatal(1,"BB_CONSUMER fixture restore phase=%0d",p);
    restores=restores+1;
   end
  end
 endtask
 task interval(input integer tick,trial,input bit natural_debug);
  reg [223:0] saved;
  reg [31:0] observed_edges;
  reg [6:0] predicted_edges;
  begin
   consume=1;advance=1;
   for(integer p=0;p<8;p++)begin
    debug_before[p]=oracle_packet[p][6];
    boundary_debug[p]=natural_debug ? 1'b0 : ((tick+trial+p)%3==0);
    event_debug[p]=natural_debug ? 6'd0 : 6'(tick*37+trial*11+p*13);
   end
   #2;
   for(integer p=0;p<8;p++)begin
    if(producer[p] !== expected[p*20000+tick])$fatal(1,"BB_CONSUMER changed primary source phase=%0d tick=%0d",p,tick);
    if(invalid[p])$fatal(1,"BB_CONSUMER invalid source count");
    for(integer step=0;step<=int'(counts[p]);step++)begin
     saved=oracle_counts[p];
     if(step==0)oracle_packet[p]={boundary_debug[p],before_sources[p]};
     else oracle_packet[p]={event_debug[p][step-1],source_levels[p][6*(step-1)+:6]};
     #2;
     if(oracle_state[p] !== state_steps[p][6*step+:6] ||
       {oracle_root[p],oracle_max[p],oracle_pre[p],oracle_aux[p]} !==
       {roots[p][step],maxes[p][step],pres[p][step],auxes[p][step]})
      $fatal(1,"BB_CONSUMER state/output mismatch phase=%0d trial=%0d tick=%0d step=%0d got=%h expected=%h",p,trial,tick,step,state_steps[p][6*step+:6],oracle_state[p]);
     predicted_edges=edge_steps[p][7*step+:7];
     for(integer bit_index=0;bit_index<7;bit_index++)begin
      observed_edges=oracle_counts[p][32*bit_index+:32]-saved[32*bit_index+:32];
      if(observed_edges !== 32'(predicted_edges[bit_index]))
       $fatal(1,"BB_CONSUMER edge mismatch phase=%0d trial=%0d tick=%0d step=%0d edge=%0d got=%0d expected=%0d",p,trial,tick,step,bit_index,observed_edges,predicted_edges[bit_index]);
     end
     step_checks=step_checks+1;
    end
    for(integer step=1;step<7;step++)if(step>int'(counts[p]))begin
     if(state_steps[p][6*step+:6] !== 0 || edge_steps[p][7*step+:7] !== 0 ||
       {roots[p][step],maxes[p][step],pres[p][step],auxes[p][step]} !== 4'b0)
      $fatal(1,"BB_CONSUMER unused event slot");
    end
    if(next_state[p] !== oracle_state[p])$fatal(1,"BB_CONSUMER final owner mismatch");
    owners[p]=next_state[p];samples=samples+1;
   end
   pulse;
  end
 endtask
 task hold_transport;
  begin
   consume=0;advance=0;
   for(integer p=0;p<8;p++)begin boundary_debug[p]=!oracle_packet[p][6];event_debug[p]=6'h3f;end
   #2;
   repeat(3)begin
    pulse;
    for(integer p=0;p<8;p++)begin
     if(next_state[p] !== owners[p] || state_steps[p] !== 0 || edge_steps[p] !== 0 ||
       {roots[p],maxes[p],pres[p],auxes[p]} !== 28'b0)
      $fatal(1,"BB_CONSUMER transport hold changed state/events");
     hold_checks=hold_checks+1;
    end
   end
  end
 endtask
 initial begin
  if(!$value$plusargs("expected=%s",expected_path))$fatal(1,"Missing independent source schedule");
  $readmemh(expected_path,expected);
  for(integer p=0;p<8;p++)begin owners[p]=0;fixture_states[p]=0;oracle_packet[p]=0;debug_before[p]=0;boundary_debug[p]=0;event_debug[p]=0;end
  for(integer trial=0;trial<64;trial++)begin
   restart(trial);
   for(integer tick=0;tick<32;tick++)begin interval(tick,trial,0);if(tick%7==0)hold_transport;end
  end
  restart(11);
  for(integer p=0;p<8;p++)full_begin[p]=oracle_counts[p];
  for(integer tick=0;tick<20000;tick++)begin interval(tick,64,1);if(tick%97==0)hold_transport;end
  for(integer p=0;p<8;p++)begin
   if(oracle_counts[p][31:0]-full_begin[p][31:0] != 3000 ||
      oracle_counts[p][63:32]-full_begin[p][63:32] != 600 ||
      oracle_counts[p][95:64]-full_begin[p][95:64] != 300 ||
      oracle_counts[p][127:96]-full_begin[p][127:96] != 10000 ||
      oracle_counts[p][159:128]-full_begin[p][159:128] != 10000 ||
      oracle_counts[p][191:160]-full_begin[p][191:160] != 300 ||
      oracle_counts[p][223:192]-full_begin[p][223:192] != 300 || owners[p] !== 6'h0b)
    $fatal(1,"BB_CONSUMER incomplete original clock cadence phase=%0d",p);
  end
  $display("BB_CONSUMER_PASS phases=8 initial_states=64 samples=%0d steps=%0d holds=%0d restores=%0d nominal_us=300 crystal_rises=3000 max_rises=600 pre_rises=300 root_rises=10000",samples,step_checks,hold_checks,restores);
  $finish;
 end
endmodule
