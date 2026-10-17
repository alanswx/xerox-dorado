// Captured Cedar JLB failure: CompactVM must skip pageSource == pageGerm.
// Exercise ProcH's actual d13 instance, its flag polarity and the c21 gate.
// Operand-sign/carry inputs are test stimuli. No guest state is imported.
`default_nettype none
module tb_signed_compare;
  reg sys_clk=0;
  always #1 sys_clk=~sys_clk;
  reg [15:0] A=0,B=0;
  reg [3:0] S=9;
  reg M=0,Cn=0;
  wire [15:0] F;
  wire [4:0] carry;
  assign carry[0]=Cn;
  genvar k;
  generate
    for (k = 0; k < 4; k = k + 1) begin : slice
      cell_MC10181 u (
        .p10(A[4*k+3]), .p16(A[4*k+2]), .p18(A[4*k+1]), .p21(A[4*k+0]),
        .p9 (B[4*k+3]), .p11(B[4*k+2]), .p19(B[4*k+1]), .p20(B[4*k+0]),
        .p13(S[3]), .p15(S[2]), .p17(S[1]), .p14(S[0]),
        .p23(M), .p22(carry[k]),
        .p6 (F[4*k+3]), .p7 (F[4*k+2]), .p3 (F[4*k+1]), .p2 (F[4*k+0]),
        .p5 (carry[k+1]), .p4(), .p8()
      );
    end
  endgenerate

  ProcH_m_Rev_m_Ce ph(.sys_clk(sys_clk));
  // Only drive the five inputs of the board's documented overflow detector.
  // The generated board supplies d13's parameter and wiring under test.
  initial begin
    force ph.alub_00a=B[15];
    force ph.alua_00=A[15];
    force ph.aluF0=S[3];
    force ph.alu_00=result_sign;
    force ph.aluCout=carry_out;
  end
  wire no_overflow;
  // ContA c21: all unselected branch groups high; select Overflow' only.
  cell_MC10121 c21(.p4(1'b1),.p5(1'b0),.p6(1'b1),
    .p7(1'b1),.p9(1'b1),.p10(1'b0),.p11(1'b1),.p12(1'b1),
    .p13(ph.SignedCarry),.p14(1'b0),.p15(1'b0),.p2(),.p3(no_overflow));
  integer i,j,op,op_index,n=0;
  integer signed wide;
  reg ovf,jump_taken;
  reg result_sign=0,carry_out=0;
  reg [15:0] vals[0:7];
  initial begin
    vals[0]=0; vals[1]=1; vals[2]=16'h3e02; vals[3]=16'h3e27;
    vals[4]=16'h7fff; vals[5]=16'h8000; vals[6]=16'hffff; vals[7]=16'h8001;
    // Start with the captured JLB operation, including zero == zero.
    for(op_index=0;op_index<4;op_index=op_index+1)begin
      op=(op_index+2)%4;
      for(i=0;i<8;i=i+1)for(j=0;j<8;j=j+1)begin
        A=vals[i];B=vals[j];M=0;
        case(op)
          0: begin S=6;Cn=0;wide=int'($signed(A))+int'($signed(B));end
          1: begin S=6;Cn=1;wide=int'($signed(A))+int'($signed(B))+1;end
          2: begin S=9;Cn=0;wide=int'($signed(A))-int'($signed(B))-1;end
          3: begin S=9;Cn=1;wide=int'($signed(A))-int'($signed(B));end
        endcase
        #4; result_sign=F[15];carry_out=carry[4];#4;
        ovf=(wide>32767 || wide< -32768);
        if(F!==16'(wide))$fatal(1,"ALU result mismatch");
        if(ph.SignedCarry!==ovf || no_overflow!==!ovf) begin
          $fatal(1,"overflow polarity: op=%0d A=%h B=%h F=%h detector=%b want=%b branch=%b",
            op,A,B,F,ph.SignedCarry,ovf,no_overflow);
        end
        // Mesa JLB computes k-j-1, then repairs signed overflow.
        if(op==2)begin
          jump_taken=F[15] ^ no_overflow;
          if(jump_taken!==($signed(B)<$signed(A)))
            $fatal(1,"signed JLB mismatch: j=%h k=%h",B,A);
        end
        n=n+1;
      end
    end
    $display("SIGNED COMPARE PASS: %0d overflow/branch vectors, including equal germ bounds",n);
    $finish;
  end
endmodule
`default_nettype wire
