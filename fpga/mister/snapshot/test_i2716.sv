// Four-state oracle: empty sockets remain unknown, not an invented zero ROM.
module i2716_fixture;
 reg host=0,ref_clk=0,advance=0,freeze=0,held=0,request=0,write=0;
 reg [10:0] address=0;reg [7:0] cs_n=0,pd_n=0;
 reg [4:0] index=0;reg [17:0] data=0;
 wire [7:0] frozen,done,error;
 wire [7:0] q[0:7],original_q[0:7],expected_word[0:7];
 wire [8:0] raw[0:7],original_raw[0:7];wire [17:0] result[0:7];
 reg [8:0] model[0:7],saved[0:7];
 reg was_frozen=0;
 integer edges=0,holds=0,accepted=0,writes=0,late=0;
 reg [31:0] seed=32'h27160072;
 function [31:0] random_word;
  begin seed=seed^(seed<<13);seed=seed^(seed>>17);seed=seed^(seed<<5);random_word=seed;end
 endfunction
 // INSERT_INSTANCES
 task tick(input do_reference,input compare_reference);
  reg hold,accept,valid;reg [8:0] previous;reg [7:0] expected_q;
  integer k,bank;
  begin
   host=0;ref_clk=0;#1;
   hold=freeze&&held;accept=hold&&was_frozen&&request;valid=index[1:0]==0;bank=index[4:2];previous=model[bank];
   if(accept&&valid&&write)model[bank]=data[8:0];
   if(!hold&&advance)for(k=0;k<8;k=k+1)model[k]={~cs_n[k]&~pd_n[k],expected_word[k]};
   if(!hold&&advance&&do_reference)ref_clk=1;
   host=1;#1;edges=edges+1;if(hold)holds=holds+1;if(accept)accepted=accepted+1;if(accept&&valid&&write)writes=writes+1;
   if(frozen!==(hold?8'hff:8'h00)||done!==(accept?(8'b1<<bank):8'b0)||error!==((accept&&!valid)?(8'b1<<bank):8'b0))$fatal(1,"2716 handshake differs at %0d",edges);
   for(k=0;k<8;k=k+1)begin
    expected_q=model[k][8]?model[k][7:0]:8'b0;
    if(raw[k]!==model[k])$fatal(1,"2716 raw pipeline differs at %0d bank%0d",edges,k);
    if(q[k]!==expected_q)$fatal(1,"2716 output differs at %0d bank%0d",edges,k);
    if(compare_reference&&(original_raw[k]!==model[k]||original_q[k]!==expected_q))$fatal(1,"2716 untouched original differs at %0d bank%0d",edges,k);
   end
   if(accept&&result[bank]!==((valid&&!write)?{9'b0,previous}:18'b0))$fatal(1,"2716 response/padding differs at %0d",edges);
   was_frozen=hold;#1;host=0;ref_clk=0;#1;
  end
 endtask
 task pins;
  begin address=random_word()&2047;cs_n=random_word();pd_n=random_word();advance=random_word()&1;end
 endtask
 task access(input integer bank,input integer row,input wr,input [17:0] value);
  begin pins();late=late+1;index=(bank<<2)|row;request=1;write=wr;data=value;tick(0,0);request=0;end
 endtask
 task random_steps(input integer count,input do_reference,input compare_reference);
  integer n;
  begin for(n=0;n<count;n=n+1)begin pins();request=random_word()&1;write=random_word()&1;index=random_word()&31;data=random_word();tick(do_reference,compare_reference);end request=0;end
 endtask
 integer a,b,k,n,cs,pd;
 initial begin
  #1;
  // Every real image address and empty-socket X is read through original pins.
  advance=1;cs_n=0;pd_n=0;
  for(a=0;a<2048;a=a+1)begin address=a;tick(1,1);end
  for(cs=0;cs<256;cs=cs+1)for(pd=0;pd<4;pd=pd+1)begin
   cs_n=cs;pd_n=pd==0?0:pd==1?8'hff:pd==2?8'h55:8'haa;address=random_word()&2047;advance=1;tick(1,1);
   advance=0;cs_n=~cs_n;pd_n=~pd_n;address=random_word()&2047;tick(1,1);
  end
  random_steps(5000,1,1);
  // Freeze alone and held alone do not stop ordinary advancement.
  freeze=1;held=0;random_steps(127,1,1);freeze=0;held=1;random_steps(127,1,1);held=0;
  advance=1;address=39;cs_n=0;pd_n=0;tick(1,1);advance=0;address=713;tick(1,1);
  for(k=0;k<8;k=k+1)saved[k]=model[k];
  freeze=1;held=1;request=1;write=1;index=0;data=18'h3ffff;tick(0,1);request=0;
  for(k=0;k<8;k=k+1)access(k,0,1,0);
  for(b=0;b<72;b=b+1)begin access(b/9,0,1,18'b1<<(b%9));access(b/9,0,0,0);access(b/9,0,1,0);end
  for(k=0;k<8;k=k+1)begin
   access(k,0,1,18'h3fe00);access(k,0,0,0);
   for(a=1;a<4;a=a+1)begin access(k,a,1,18'h3ffff);access(k,a,0,0);end
   // Also preserve X/Z data in the nine pipeline bits exactly in four-state RTL.
   access(k,0,1,18'b0000000001xz10zx01);access(k,0,0,0);
   access(k,0,1,{9'b0,saved[k]});
  end
  freeze=0;held=0;request=1;write=1;index=0;data=0;advance=0;tick(1,1);request=0;advance=1;tick(1,1);
  random_steps(1000,1,1);
  for(k=0;k<8;k=k+1)saved[k]=model[k];
  freeze=1;held=1;request=0;tick(0,1);
  freeze=0;held=0;random_steps(2000,0,0);
  freeze=1;held=1;request=0;tick(0,0);
  for(k=0;k<8;k=k+1)begin access(k,0,1,{9'b0,saved[k]});access(k,0,0,0);end
  freeze=0;held=0;request=1;write=1;advance=0;index=0;data=18'h3ffff;tick(1,1);
  request=0;advance=1;tick(1,1);random_steps(20000,1,1);
  // Re-read every ROM address after all state writes; ROM is immutable.
  advance=1;cs_n=0;pd_n=0;
  for(a=0;a<2048;a=a+1)begin address=a;tick(1,1);end
  $display("I2716 PASS: %0d host/%0d held/%0d accepted/%0d writes/%0d late changes;72bits/all32indices/allROMaddresses/emptyX/unknown-state/freeze/restore/firstedge/20000more",edges,holds,accepted,writes,late);
  $finish;
 end
endmodule
