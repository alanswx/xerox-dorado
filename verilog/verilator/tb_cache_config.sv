// MemC23: fit the address board for the 16K-word cache on MemD (F10470).
// Upper VA tag bits and lower VA row bits must never be wired together.
`timescale 1ns/1ps
`default_nettype none
module tb_cache_config;
  reg sys_clk=0;
  always #1 sys_clk=~sys_clk;
  reg [1:0] tag=0,row=0,proc_row=0;
  reg row_enable=0,hi_b=1;
  MemC_m_Rev_m_Be m (.sys_clk(sys_clk),.MemRfsh(1'b0));
  wire [1:0] tag_bits={m.dVA_5_s_20,m.dVA_6_s_21};
  // These two ProcVA contributions share the row bus with the address
  // ALU. Check their combined contribution independently of that ALU.
  wire [1:0] row_bits={m.MemC09_sil_pl_17__a05_3 | m.MemC09_sil_pl_17__d03_15,
                       m.MemC01_sil_pl_12__a05_4 | m.MemC01_sil_pl_12__d03_2};
  initial begin
    force m.dVA_05=tag[1]; force m.dVA_06=tag[0];
    force m.dAad_0_p_=~row[1]; force m.dAad_1_p_=~row[0];
    force m.ProcVA_20=proc_row[1]; force m.ProcVA_21=proc_row[0];
    force m._u_PrVArow=row_enable; force m._u_PrVACVA=~row_enable;
    force m.Hia=1; force m.Hib=hi_b;
    for(integer h=0;h<2;h=h+1)
      for(integer t=0;t<4;t=t+1)
        for(integer r=0;r<4;r=r+1) begin
          hi_b=h[0]; tag=t[1:0]; row=r[1:0];
          repeat(4) @(negedge sys_clk);
          if(tag_bits!==tag)
            $fatal(1,"16K cache tag VA5..6=%b row20..21=%b HiB=%b produced %b",tag,row,hi_b,tag_bits);
        end
    for(integer e=0;e<2;e=e+1)
      for(integer r=0;r<4;r=r+1) begin
        row_enable=e[0]; proc_row=r[1:0];
        repeat(4) @(negedge sys_clk);
        if(row_bits!==(row_enable?proc_row:2'b00))
          $fatal(1,"16K row mux row_enable=%b ProcVA20..21=%b produced %b",row_enable,proc_row,row_bits);
      end
    if({m.u_l11.p11,m.u_l11.p12}!==2'b10)
      $fatal(1,"CacheConfig=%b%b expected 2 (16K with parity)",m.u_l11.p11,m.u_l11.p12);
    if({m.u_l11.p13,m.u_l11.p14}!==2'b11)
      $fatal(1,"PageConfig changed from 256 words");
    $display("tb_cache_config: PASS -- independent tag/row bits, CacheConfig=2, PageConfig=3");
    $finish;
  end
endmodule
`default_nettype wire
