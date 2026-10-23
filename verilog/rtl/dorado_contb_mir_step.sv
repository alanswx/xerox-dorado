// Experimental original ContB MIR/maintenance evaluator, not native RTL.
`default_nettype none
module dorado_contb_mir_step #(parameter integer WAVES=16)(
 input wire [39:0] state_q,input wire [55:0] pins_before,pins_after,
 output reg [39:0] state_next,output reg [24:0] clock_events,repeated_events,
 output reg unsettled,output wire [25:0] flags);
 function automatic stop_mir(input [39:0] q,input [55:0] p);
  stop_mir=q[28] & !((p[7] | !q[23]) & (p[8] | !q[22]));
 endfunction
 function automatic [24:0] levels_at(input [39:0] q,input [55:0] p);
  reg normal,repeat_cur,enable;reg [2:0] selector;reg [24:0] v;
  begin
   normal=p[0]|p[1]|p[2]|stop_mir(q,p);repeat_cur=p[3]&p[4];
   v={25{normal|repeat_cur}};v[0]=repeat_cur;v[17]=repeat_cur;
   enable=p[5]&!p[46];selector={p[19],p[20],p[21]};
   v[22]=!enable|(selector!=3'd0);v[23]=!enable|(selector!=3'd7);v[24]=!enable|(selector!=3'd1);
   levels_at=v;
  end
 endfunction
 function automatic [39:0] transfer(input [39:0] q,input [24:0] capture,input [55:0] p);
  reg [39:0] n;reg reset_mir,cp_enable;reg [1:0] selector;
  begin
   n=q;reset_mir=p[6]|q[34];cp_enable=!(p[10]|p[13]);selector={p[11],p[12]};
   if(capture[0])n[0]=0;
   if(cp_enable && selector==2'd0 && p[15])n[1]=1;
   else if(reset_mir)n[1]=0;else if(capture[1])n[1]=p[28];
   if(cp_enable && selector==2'd0 && p[16])n[2]=1;
   else if(reset_mir)n[2]=0;else if(capture[2])n[2]=p[29];
   if(cp_enable && selector==2'd0 && p[15])n[3]=1;
   else if(reset_mir)n[3]=0;else if(capture[3])n[3]=p[28];
   if(cp_enable && selector==2'd2 && p[18])n[4]=1;
   else if(reset_mir)n[4]=0;else if(capture[4])n[4]=p[45];
   if(cp_enable && selector==2'd0 && p[17])n[5]=1;
   else if(reset_mir)n[5]=0;else if(capture[5])n[5]=p[30];
   if(cp_enable && selector==2'd1 && p[14])n[6]=1;
   else if(reset_mir)n[6]=0;else if(capture[6])n[6]=p[32];
   if(cp_enable && selector==2'd2 && p[17])n[7]=1;
   else if(reset_mir)n[7]=0;else if(capture[7])n[7]=p[31];
   if(cp_enable && selector==2'd1 && p[16])n[8]=1;
   else if(reset_mir)n[8]=0;else if(capture[8])n[8]=p[34];
   if(cp_enable && selector==2'd1 && p[15])n[9]=1;
   else if(reset_mir)n[9]=0;else if(capture[9])n[9]=p[33];
   if(cp_enable && selector==2'd0 && p[14])n[10]=1;
   else if(reset_mir)n[10]=0;else if(capture[10])n[10]=p[36];
   if(cp_enable && selector==2'd1 && p[17])n[11]=1;
   else if(reset_mir)n[11]=0;else if(capture[11])n[11]=p[35];
   if(cp_enable && selector==2'd2 && p[14])n[12]=1;
   else if(reset_mir)n[12]=0;else if(capture[12])n[12]=p[38];
   if(cp_enable && selector==2'd3 && p[17])n[13]=1;
   else if(reset_mir)n[13]=0;else if(capture[13])n[13]=p[37];
   if(cp_enable && selector==2'd2 && p[16])n[14]=1;
   else if(reset_mir)n[14]=0;else if(capture[14])n[14]=p[40];
   if(cp_enable && selector==2'd2 && p[15])n[15]=1;
   else if(reset_mir)n[15]=0;else if(capture[15])n[15]=p[39];
   if(cp_enable && selector==2'd0 && p[15])n[16]=1;
   else if(reset_mir)n[16]=0;else if(capture[16])n[16]=p[28];
   if(capture[17])n[17]=0;
   if(cp_enable && selector==2'd3 && p[16])n[18]=1;
   else if(reset_mir)n[18]=0;else if(capture[18])n[18]=p[44];
   if(cp_enable && selector==2'd3 && p[15])n[19]=1;
   else if(reset_mir)n[19]=0;else if(capture[19])n[19]=p[43];
   if(cp_enable && selector==2'd3 && p[14])n[20]=1;
   else if(reset_mir)n[20]=0;else if(capture[20])n[20]=p[42];
   if(cp_enable && selector==2'd3 && p[18])n[21]=1;
   else if(reset_mir)n[21]=0;else if(capture[21])n[21]=p[41];
   if(capture[22])n[27:22]=p[27:22];
   if(capture[23])n[33:28]=p[27:22];
   if(capture[24])n[39:34]=p[27:22];
   transfer=n;
  end
 endfunction
 reg [39:0] q,n;reg [24:0] previous,levels,capture;reg settled;
 always @* begin
  q=state_q;n=q;previous=levels_at(q,pins_before);levels=previous;capture=0;
  clock_events=0;repeated_events=0;settled=0;
  for(integer wave=0;wave<WAVES;wave++)begin
   levels=levels_at(q,pins_after);capture=levels & ~previous;
   repeated_events|=capture&clock_events;clock_events|=capture;
   n=transfer(q,capture,pins_after);
   settled=(n==q && levels==previous);previous=levels;q=n;
  end
  state_next=q;unsettled=!settled;
 end
 assign flags={state_next[22],state_next[23],state_next[28],pins_after[3]&pins_after[4],
   pins_after[6]|state_next[34],stop_mir(state_next,pins_after),state_next[4],~state_next[16],
   state_next[18],state_next[19],state_next[20],state_next[21],state_next[14],state_next[15],
   state_next[12],state_next[13],state_next[10],state_next[11],state_next[8],
   ~state_next[9],~state_next[6],~state_next[7],~state_next[5],~state_next[2],~state_next[3],~state_next[1]};
endmodule
`default_nettype wire
