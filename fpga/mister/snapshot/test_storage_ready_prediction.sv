`timescale 1ns/1ps
module tb;
    reg clk=0,held=0,scan=0,advance=0,global_reset=0;
    reg ras_n=1,cas_n=1,we_n=1,address_change=0,req_ready=0,rsp_valid=0;
    reg initialized=1,pack_ready=1,global_hold=0,serial_in=0;
    wire storage_reset=global_reset || !initialized;
    integer checks=0,binary_cases=0,fourstate_cases=0,guard_cases=0;
    task check(input bit ok,input string message);
        begin checks=checks+1;if(!ok)$fatal(1,"%s",message);end
    endtask
    function automatic reg val(input integer n);
        case(n)0:val=0;1:val=1;2:val=1'bx;3:val=1'bz;endcase
    endfunction
    for(genvar g=0;g<2;g=g+1)begin:groups
        localparam W=g==0?8:12,SB=6+4*W;
        wire next_flags_ready;
        reg cached_ready=0;
        wire [W-1:0] data_in=W'(16'h5a39),rsp_word=W'(16'hbe65);
        // INSTANCE_BLOCK
        always @(posedge clk)cached_ready<=next_flags_ready;
        task seed(input [3:0] flags,input [1:0] low_bits,input [1:0] strobes,input integer token);
            reg [SB-1:0] image;
            begin
                image=SB'(64'hc65a39b417028ded ^ (64'h9e3779b97f4a7c15*(token+1)));
                image[0]=strobes[0];image[1]=strobes[1];image[2]=flags[0];
                image[3+3*W]=flags[1];image[4+3*W]=flags[2];image[5+3*W]=flags[3];
                image[3]=low_bits[0];image[6+3*W]=low_bits[1];
                {reference.cache_data,reference.waiting,reference.write_done,reference.cache_valid,
                    reference.data_out,reference.read_pipe,reference.write_data,reference.write_pending,reference.we_d,reference.cas_d}=image;
                {dut.cache_data,dut.waiting,dut.write_done,dut.cache_valid,
                    dut.data_out,dut.read_pipe,dut.write_data,dut.write_pending,dut.we_d,dut.cas_d}=image;
                reference.state_frozen=0;dut.state_frozen=0;
            end
        endtask
        task verify;
            reg old_gate,new_gate;
            begin
                check(dut.state_bits===reference.state_bits && dut.state_frozen===reference.state_frozen,"original frontend state changed");
                check({dut.ready,dut.req_valid,dut.req_write,dut.req_addr,dut.req_data,dut.rsp_ready,dut.data_out,dut.state_drain_ready}===
                    {reference.ready,reference.req_valid,reference.req_write,reference.req_addr,reference.req_data,reference.rsp_ready,reference.data_out,reference.state_drain_ready},"original frontend outputs changed");
                check(cached_ready===(!reference.waiting && reference.cache_valid &&
                    (!reference.write_pending || reference.write_done)),"registered readiness differs from actual post-edge state");
                check((!storage_reset && cached_ready)===reference.state_drain_ready,"live reset/drain factoring differs");
                old_gate=!global_reset && pack_ready && initialized && reference.state_drain_ready && !global_hold;
                new_gate=!global_reset && pack_ready && initialized && cached_ready && !global_hold;
                check(old_gate===new_gate,"actual CPU enable guard factoring differs");
            end
        endtask
    end
    task run_case(input [3:0] flags,input [1:0] data_bits,input [11:0] controls,input integer token);
        begin
            clk=0;initialized=1;pack_ready=1;global_hold=0;
            held=controls[0];scan=controls[1];advance=controls[2];global_reset=controls[3];
            ras_n=controls[4];cas_n=controls[5];we_n=controls[6];address_change=controls[7];
            req_ready=controls[10];rsp_valid=controls[11];serial_in=token[0];
            groups[0].seed(flags,data_bits,controls[9:8],token);
            groups[1].seed(flags,data_bits,controls[9:8],token+37);
            #1;clk=1;#1;groups[0].verify();groups[1].verify();clk=0;#1;
        end
    endtask
    integer i,f,c,b,x,a,d,mask;
    reg [3:0] flags4;
    reg [1:0] data4;
    reg [11:0] controls4;
    initial begin
        // All readiness flags, twelve controls and both scan-source low bits.
        // Diagnostics only are removed from the reference to exercise even
        // arbitrary imported states; every original sequential write remains.
        for(i=0;i<262144;i=i+1)begin
            run_case(i[3:0],i[17:16],i[15:4],i);binary_cases=binary_cases+1;
        end
        for(f=0;f<256;f=f+1)for(c=0;c<64;c=c+1)for(b=0;b<16;b=b+1)begin
            flags4={val((f>>6)&3),val((f>>4)&3),val((f>>2)&3),val(f&3)};
            data4={val((b>>2)&3),val(b&3)};
            run_case(flags4,data4,12'((c*983)^(f*37)),f+c+b);fourstate_cases=fourstate_cases+1;
        end
        for(i=0;i<12;i=i+1)for(x=2;x<4;x=x+1)for(f=0;f<16;f=f+1)for(b=0;b<4;b=b+1)begin
            controls4=12'hd95;controls4[i]=val(x);
            run_case(4'(f),2'(b),controls4,i+f+b);fourstate_cases=fourstate_cases+1;
        end
        // Physical native reset is reset||!storage_initialized. Check all
        // four-valued live guards between edges with frozen original flags.
        for(f=0;f<256;f=f+1)begin
            flags4={val((f>>6)&3),val((f>>4)&3),val((f>>2)&3),val(f&3)};
            run_case(flags4,0,12'h001,f);
            for(a=0;a<4;a=a+1)for(b=0;b<4;b=b+1)
            for(c=0;c<4;c=c+1)for(d=0;d<4;d=d+1)begin
                global_reset=val(a);initialized=val(b);pack_ready=val(c);global_hold=val(d);#1;
                groups[0].verify();groups[1].verify();guard_cases=guard_cases+1;
            end
        end
        $display("READY PREDICTION FOURSTATE PASS checks=%0d binary_cases=%0d fourstate_cases=%0d guard_cases=%0d widths=8,12",checks,binary_cases,fourstate_cases,guard_cases);
        $finish;
    end
endmodule
