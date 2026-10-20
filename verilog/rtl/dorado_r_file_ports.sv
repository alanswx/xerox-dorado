// Functional RM/STK ports for clock migration. Not yet wired into native RTL.
// Four 256x9 banks retain byte data plus the original generated odd parity.
// These are physical SRAM addresses, MSB-first by original pin order, not
// decoded logical R addresses. Address/selection producers remain external.
// Address bytes: H RM, H STK, L RM, L STK from low to high. select_n has
// the same bank order. Write gates are per half (H bit0, L bit1).
// No reset clearing, snapshot interface or flattened array-state transport.
`default_nettype none
module dorado_r_file_ports(
 input wire host_clk,advance,input wire [1:0] write_n,
 input wire [31:0] address,input wire [3:0] select_n,
 input wire [15:0] write_data,
 output wire [15:0] read_data,output wire [1:0] read_parity);
 (* ramstyle="MLAB, no_rw_check" *) reg [8:0] rm_h[0:255],stk_h[0:255];
 (* ramstyle="MLAB, no_rw_check" *) reg [8:0] rm_l[0:255],stk_l[0:255];
 wire [8:0] data_h={~^write_data[7:0],write_data[7:0]};
 wire [8:0] data_l={~^write_data[15:8],write_data[15:8]};
 always @(posedge host_clk) begin
   if(advance && !write_n[0]) begin
     if(!select_n[0]) rm_h[address[7:0]]<=data_h;
     if(!select_n[1]) stk_h[address[15:8]]<=data_h;
   end
   if(advance && !write_n[1]) begin
     if(!select_n[2]) rm_l[address[23:16]]<=data_l;
     if(!select_n[3]) stk_l[address[31:24]]<=data_l;
   end
 end
 wire [8:0] qh=(select_n[0] ? 9'b0 : rm_h[address[7:0]]) |
                (select_n[1] ? 9'b0 : stk_h[address[15:8]]);
 wire [8:0] ql=(select_n[2] ? 9'b0 : rm_l[address[23:16]]) |
                (select_n[3] ? 9'b0 : stk_l[address[31:24]]);
 assign read_data={ql[7:0],qh[7:0]};
 assign read_parity={ql[8],qh[8]};
endmodule
`default_nettype wire
