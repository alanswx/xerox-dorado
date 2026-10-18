// Complete MSA board equivalence: original chips versus external backing.
// Only the raw bit arrays move; every serial/ECC output is compared each cycle.
`timescale 1ns/1ps
module tb_storage_board #(parameter integer ADDR_BITS=14, LINE_CACHE_BITS=0,PREFETCH_NEXT=0);
    localparam integer CHIP_WORDS=1<<ADDR_BITS;
    reg clk=0;
`ifdef STORAGE_USE_SDRAM
    always #10 clk=~clk; //50MHz host, opposite-phase SDRAM clock
`else
    always #1 clk=~clk;
`endif
    reg run=0,reset=1;
    wire machine_clk=clk && run;
    reg [31:0] random_state=32'h73e91d4b;
    integer steps=0,reads=0,writes=0,output_changes=0,stored_ones=0;
    integer host_cycles=0;
    always @(posedge clk)host_cycles<=host_cycles+1;
    reg [2:0] phase=0;
    reg [63:0] stimulus=0;
    wire CLK_ms0Even_p_=steps[1];
    wire ChipsAre16k=ADDR_BITS==14;
    wire ChipsAre4k=1'b0;
    wire ChipsAre64K=ADDR_BITS==16;
    wire EcOut_0=stimulus[4];
    wire EcOut_1=stimulus[5];
    wire EcOut_4=stimulus[6];
    wire EcOut_5=stimulus[7];
    wire LoadEcOut_p_=stimulus[8];
    wire LoadSinE=stimulus[9];
    wire LoadSoutE_p_=stimulus[10];
    wire Mb0=1'b1;
    wire MemAd_1=stimulus[12];
    wire MemAd_2=stimulus[13];
    wire MemAd_3=stimulus[14];
    wire MemAd_4=stimulus[15];
    wire MemAd_5=stimulus[16];
    wire MemAd_6=stimulus[17];
    wire MemAd_7=stimulus[18];
    wire MemAd_8=stimulus[19];
    wire MemCASa=phase>=2 && phase!=7;
    wire MemCASb=phase>=2 && phase!=7;
    wire MemRASa=phase>=1 && phase!=7;
    wire MemRASb=phase>=1 && phase!=7;
    wire MemWEa=stimulus[63];
    wire MemWEb=stimulus[63];
    wire Mod0SinEn_p_=stimulus[26];
    wire Mod0StrEn_p_=1'b0;
    wire ShiftEcOut=stimulus[28];
    wire ShiftSinE=stimulus[29];
    wire ShiftSoutE=stimulus[30];
    wire Sout_00=stimulus[31];
    wire Sout_01=stimulus[32];
    wire Sout_02=stimulus[33];
    wire Sout_03=stimulus[34];
    wire Sout_04=stimulus[35];
    wire Sout_05=stimulus[36];
    wire Sout_06=stimulus[37];
    wire Sout_07=stimulus[38];
    wire Sout_08=stimulus[39];
    wire Sout_09=stimulus[40];
    wire Sout_10=stimulus[41];
    wire Sout_11=stimulus[42];
    wire Sout_12=stimulus[43];
    wire Sout_13=stimulus[44];
    wire Sout_14=stimulus[45];
    wire Sout_15=stimulus[46];
    wire [18:0] actual,expected;
    msa #(.SYSPER(4)) original (.sys_clk(machine_clk),
        .CLK_ms0Even_p_(CLK_ms0Even_p_),
        .ChipsAre16k(ChipsAre16k),
        .ChipsAre4k(ChipsAre4k),
        .ChipsAre64K(ChipsAre64K),
        .EcOut_0(EcOut_0),
        .EcOut_1(EcOut_1),
        .EcOut_4(EcOut_4),
        .EcOut_5(EcOut_5),
        .LoadEcOut_p_(LoadEcOut_p_),
        .LoadSinE(LoadSinE),
        .LoadSoutE_p_(LoadSoutE_p_),
        .Mb0(Mb0),
        .MemAd_1(MemAd_1),
        .MemAd_2(MemAd_2),
        .MemAd_3(MemAd_3),
        .MemAd_4(MemAd_4),
        .MemAd_5(MemAd_5),
        .MemAd_6(MemAd_6),
        .MemAd_7(MemAd_7),
        .MemAd_8(MemAd_8),
        .MemCASa(MemCASa),
        .MemCASb(MemCASb),
        .MemRASa(MemRASa),
        .MemRASb(MemRASb),
        .MemWEa(MemWEa),
        .MemWEb(MemWEb),
        .Mod0SinEn_p_(Mod0SinEn_p_),
        .Mod0StrEn_p_(Mod0StrEn_p_),
        .ShiftEcOut(ShiftEcOut),
        .ShiftSinE(ShiftSinE),
        .ShiftSoutE(ShiftSoutE),
        .Sout_00(Sout_00),
        .Sout_01(Sout_01),
        .Sout_02(Sout_02),
        .Sout_03(Sout_03),
        .Sout_04(Sout_04),
        .Sout_05(Sout_05),
        .Sout_06(Sout_06),
        .Sout_07(Sout_07),
        .Sout_08(Sout_08),
        .Sout_09(Sout_09),
        .Sout_10(Sout_10),
        .Sout_11(Sout_11),
        .Sout_12(Sout_12),
        .Sout_13(Sout_13),
        .Sout_14(Sout_14),
        .Sout_15(Sout_15),
        .EcIn_0__drv(expected[0]),
        .EcIn_1__drv(expected[1]),
        .M0__drv(expected[2]),
        .Sin_00__drv(expected[3]),
        .Sin_01__drv(expected[4]),
        .Sin_02__drv(expected[5]),
        .Sin_03__drv(expected[6]),
        .Sin_04__drv(expected[7]),
        .Sin_05__drv(expected[8]),
        .Sin_06__drv(expected[9]),
        .Sin_07__drv(expected[10]),
        .Sin_08__drv(expected[11]),
        .Sin_09__drv(expected[12]),
        .Sin_10__drv(expected[13]),
        .Sin_11__drv(expected[14]),
        .Sin_12__drv(expected[15]),
        .Sin_13__drv(expected[16]),
        .Sin_14__drv(expected[17]),
        .Sin_15__drv(expected[18]));
    msa_ddram #(.SYSPER(4)) external_memory (.sys_clk(machine_clk),
        .CLK_ms0Even_p_(CLK_ms0Even_p_),
        .ChipsAre16k(ChipsAre16k),
        .ChipsAre4k(ChipsAre4k),
        .ChipsAre64K(ChipsAre64K),
        .EcOut_0(EcOut_0),
        .EcOut_1(EcOut_1),
        .EcOut_4(EcOut_4),
        .EcOut_5(EcOut_5),
        .LoadEcOut_p_(LoadEcOut_p_),
        .LoadSinE(LoadSinE),
        .LoadSoutE_p_(LoadSoutE_p_),
        .Mb0(Mb0),
        .MemAd_1(MemAd_1),
        .MemAd_2(MemAd_2),
        .MemAd_3(MemAd_3),
        .MemAd_4(MemAd_4),
        .MemAd_5(MemAd_5),
        .MemAd_6(MemAd_6),
        .MemAd_7(MemAd_7),
        .MemAd_8(MemAd_8),
        .MemCASa(MemCASa),
        .MemCASb(MemCASb),
        .MemRASa(MemRASa),
        .MemRASb(MemRASb),
        .MemWEa(MemWEa),
        .MemWEb(MemWEb),
        .Mod0SinEn_p_(Mod0SinEn_p_),
        .Mod0StrEn_p_(Mod0StrEn_p_),
        .ShiftEcOut(ShiftEcOut),
        .ShiftSinE(ShiftSinE),
        .ShiftSoutE(ShiftSoutE),
        .Sout_00(Sout_00),
        .Sout_01(Sout_01),
        .Sout_02(Sout_02),
        .Sout_03(Sout_03),
        .Sout_04(Sout_04),
        .Sout_05(Sout_05),
        .Sout_06(Sout_06),
        .Sout_07(Sout_07),
        .Sout_08(Sout_08),
        .Sout_09(Sout_09),
        .Sout_10(Sout_10),
        .Sout_11(Sout_11),
        .Sout_12(Sout_12),
        .Sout_13(Sout_13),
        .Sout_14(Sout_14),
        .Sout_15(Sout_15),
        .EcIn_0__drv(actual[0]),
        .EcIn_1__drv(actual[1]),
        .M0__drv(actual[2]),
        .Sin_00__drv(actual[3]),
        .Sin_01__drv(actual[4]),
        .Sin_02__drv(actual[5]),
        .Sin_03__drv(actual[6]),
        .Sin_04__drv(actual[7]),
        .Sin_05__drv(actual[8]),
        .Sin_06__drv(actual[9]),
        .Sin_07__drv(actual[10]),
        .Sin_08__drv(actual[11]),
        .Sin_09__drv(actual[12]),
        .Sin_10__drv(actual[13]),
        .Sin_11__drv(actual[14]),
        .Sin_12__drv(actual[15]),
        .Sin_13__drv(actual[16]),
        .Sin_14__drv(actual[17]),
        .Sin_15__drv(actual[18]),
        .storage_clk(clk),.storage_reset(reset),.storage_ready(ready),
        .storage_req_valid(valid),.storage_req_ready(accept),.storage_req_write(wr),
        .storage_req_addr(addresses),.storage_req_data(wdata),
        .storage_rsp_valid(response),.storage_rsp_ready(response_ready),.storage_rsp_data(rdata));
    wire ready;
    wire [15:0] valid,accept,wr,response,response_ready;
    wire [16*ADDR_BITS-1:0] addresses;
    wire [255:0] wdata,rdata;
    wire rv,rr,rw,sv,sr;
    wire [ADDR_BITS+3:0] ra;
    wire [15:0] wd,sd;
    // Optional passive transaction trace for offline locality experiments.
    // Log completed raw operations and actual read replies, leaving every
    // original-board/storage equivalence check below active.
    integer raw_trace_fd=0;
    string raw_trace_path;
    reg [ADDR_BITS+3:0] trace_address;
    reg [15:0] trace_data;
    reg trace_write;
    initial if($value$plusargs("raw_trace=%s",raw_trace_path))begin
        raw_trace_fd=$fopen(raw_trace_path,"w");
        if(!raw_trace_fd)$fatal(1,"cannot open raw trace");
        $fwrite(raw_trace_fd,"host_cycle,kind,address,value\n");
    end
    always @(posedge clk)if(!reset)begin
        if(rv && rr)begin
            trace_address=ra;trace_data=wd;trace_write=rw;
        end
        if(sv && sr && raw_trace_fd)
            $fwrite(raw_trace_fd,"%0d,%0d,%0h,%0h\n",host_cycles,trace_write,
                trace_address,trace_write ? trace_data : sd);
    end
    final if(raw_trace_fd)$fclose(raw_trace_fd);
    wire [28:0] da;
    wire dr,dw;
    wire [7:0] be,burst;
    wire [63:0] ddata;
    reg busy=0,dvalid=0,read_pending=0;
    reg [63:0] dout=0,saved_read=0;
    reg [63:0] backing[0:4*CHIP_WORDS-1];
    integer latency=0;
    reg [7:0] host_random=8'hb7;
    dorado_storage_arb #(.GROUPS(16),.GROUP_ADDR_BITS(ADDR_BITS),.RAW_ADDR_BITS(ADDR_BITS+4),
        .PREFETCH_NEXT(PREFETCH_NEXT)) arb(
        .clk(clk),.reset(reset),.req_valid(valid),.req_write(wr),.req_ready(accept),
        .req_addr(addresses),.req_data(wdata),.rsp_valid(response),.rsp_ready(response_ready),.rsp_data(rdata),
        .raw_req_valid(rv),.raw_req_ready(rr),.raw_req_write(rw),.raw_req_addr(ra),.raw_req_data(wd),
        .raw_rsp_valid(sv),.raw_rsp_ready(sr),.raw_rsp_data(sd));
`ifdef STORAGE_USE_SDRAM
    tb_xsds_storage #(.ADDR_BITS(ADDR_BITS+4)) memory_port(
        .clk(clk),.reset(reset),.req_valid(rv),.req_ready(rr),.req_write(rw),.req_addr(ra),.req_data(wd),
        .rsp_valid(sv),.rsp_ready(sr),.rsp_data(sd),.physical_read(dr),.physical_write(dw));
    always @(negedge clk)begin
        if(dr)reads<=reads+1;
        if(dw)writes<=writes+1;
    end
`else
    dorado_storage_ddram #(.ADDR_BITS(ADDR_BITS+4),.LINE_CACHE_BITS(LINE_CACHE_BITS),.GROUP_ADDR_BITS(ADDR_BITS)) memory_port(
        .clk(clk),.reset(reset),.req_valid(rv),.req_ready(rr),.req_write(rw),.req_addr(ra),.req_data(wd),
        .rsp_valid(sv),.rsp_ready(sr),.rsp_data(sd),.ddram_busy(busy),.ddram_burstcount(burst),
        .ddram_addr(da),.ddram_read(dr),.ddram_write(dw),.ddram_wdata(ddata),.ddram_be(be),
        .ddram_rdata(dout),.ddram_rvalid(dvalid));
    always @(posedge clk)begin
        host_random<={host_random[6:0],host_random[7]^host_random[5]^host_random[4]^host_random[3]};
        dvalid<=0;busy<=host_random[3] && host_random[6];
        if(!busy && (dr || dw))begin
            if(da<29'h6000000 || da>=29'h6000000+4*CHIP_WORDS || burst!=1)$fatal(1,"bad DDR address/burst");
            if(dw)begin
                for(integer b=0;b<8;b++)if(be[b])backing[da-29'h6000000][b*8+:8]<=ddata[b*8+:8];
                writes<=writes+1;
            end else begin
                if(read_pending)$fatal(1,"multiple outstanding reads");
                read_pending<=1;latency<=1+((reads*3)%11);
                saved_read<=backing[da-29'h6000000];reads<=reads+1;
            end
        end
        if(read_pending)begin
            if(latency!=0)latency<=latency-1;
            else begin read_pending<=0;dout<=saved_read;dvalid<=1;end
        end
    end
`endif
    task tick;@(negedge clk);endtask
    reg [18:0] last_output=0;
    task step;
        wait(ready);tick();run=1;tick();run=0;steps++;
        if(actual!==expected)$fatal(1,"MSA outputs differ cycle %d: %h expected %h",steps,actual,expected);
        if(actual!=last_output)output_changes++;
        last_output=actual;
    endtask
    initial begin
        for(integer a=0;a<4*CHIP_WORDS;a++)backing[a]=0;
        repeat(5)tick();reset=0;
        for(integer i=0;i<12000;i++)begin
            wait(ready);tick();
            random_state={random_state[30:0],random_state[31]^random_state[21]^random_state[1]^random_state[0]};
            phase=(i/16)%8;
            stimulus={random_state,random_state^32'hac9356e1};
            step();
        end
        phase=7;
        repeat(12)step();
        wait(ready);tick();
`ifdef STORAGE_USE_SDRAM
        // Read the independent chip model, not a mirror of DUT requests.
        for(integer a=0;a<4*CHIP_WORDS;a++)backing[a]=memory_port.peek64(a);
        memory_port.check_model();
`endif
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_b04.mem[a] !== backing[(0*CHIP_WORDS+a)/4][((a%4)*16)+11])
                $fatal(1,"raw backing differs chip b04 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_b05.mem[a] !== backing[(0*CHIP_WORDS+a)/4][((a%4)*16)+10])
                $fatal(1,"raw backing differs chip b05 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_c06.mem[a] !== backing[(0*CHIP_WORDS+a)/4][((a%4)*16)+9])
                $fatal(1,"raw backing differs chip c06 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_d04.mem[a] !== backing[(0*CHIP_WORDS+a)/4][((a%4)*16)+8])
                $fatal(1,"raw backing differs chip d04 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_d05.mem[a] !== backing[(0*CHIP_WORDS+a)/4][((a%4)*16)+7])
                $fatal(1,"raw backing differs chip d05 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_e06.mem[a] !== backing[(0*CHIP_WORDS+a)/4][((a%4)*16)+6])
                $fatal(1,"raw backing differs chip e06 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_f04.mem[a] !== backing[(0*CHIP_WORDS+a)/4][((a%4)*16)+5])
                $fatal(1,"raw backing differs chip f04 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_f05.mem[a] !== backing[(0*CHIP_WORDS+a)/4][((a%4)*16)+4])
                $fatal(1,"raw backing differs chip f05 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_g06.mem[a] !== backing[(0*CHIP_WORDS+a)/4][((a%4)*16)+3])
                $fatal(1,"raw backing differs chip g06 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_h04.mem[a] !== backing[(0*CHIP_WORDS+a)/4][((a%4)*16)+2])
                $fatal(1,"raw backing differs chip h04 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_h05.mem[a] !== backing[(0*CHIP_WORDS+a)/4][((a%4)*16)+1])
                $fatal(1,"raw backing differs chip h05 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_i06.mem[a] !== backing[(0*CHIP_WORDS+a)/4][((a%4)*16)+0])
                $fatal(1,"raw backing differs chip i06 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_b06.mem[a] !== backing[(1*CHIP_WORDS+a)/4][((a%4)*16)+11])
                $fatal(1,"raw backing differs chip b06 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_c04.mem[a] !== backing[(1*CHIP_WORDS+a)/4][((a%4)*16)+10])
                $fatal(1,"raw backing differs chip c04 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_c05.mem[a] !== backing[(1*CHIP_WORDS+a)/4][((a%4)*16)+9])
                $fatal(1,"raw backing differs chip c05 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_d06.mem[a] !== backing[(1*CHIP_WORDS+a)/4][((a%4)*16)+8])
                $fatal(1,"raw backing differs chip d06 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_e04.mem[a] !== backing[(1*CHIP_WORDS+a)/4][((a%4)*16)+7])
                $fatal(1,"raw backing differs chip e04 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_e05.mem[a] !== backing[(1*CHIP_WORDS+a)/4][((a%4)*16)+6])
                $fatal(1,"raw backing differs chip e05 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_f06.mem[a] !== backing[(1*CHIP_WORDS+a)/4][((a%4)*16)+5])
                $fatal(1,"raw backing differs chip f06 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_g04.mem[a] !== backing[(1*CHIP_WORDS+a)/4][((a%4)*16)+4])
                $fatal(1,"raw backing differs chip g04 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_g05.mem[a] !== backing[(1*CHIP_WORDS+a)/4][((a%4)*16)+3])
                $fatal(1,"raw backing differs chip g05 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_h06.mem[a] !== backing[(1*CHIP_WORDS+a)/4][((a%4)*16)+2])
                $fatal(1,"raw backing differs chip h06 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_i04.mem[a] !== backing[(1*CHIP_WORDS+a)/4][((a%4)*16)+1])
                $fatal(1,"raw backing differs chip i04 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_i05.mem[a] !== backing[(1*CHIP_WORDS+a)/4][((a%4)*16)+0])
                $fatal(1,"raw backing differs chip i05 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_b07.mem[a] !== backing[(2*CHIP_WORDS+a)/4][((a%4)*16)+7])
                $fatal(1,"raw backing differs chip b07 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_c08.mem[a] !== backing[(2*CHIP_WORDS+a)/4][((a%4)*16)+6])
                $fatal(1,"raw backing differs chip c08 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_d07.mem[a] !== backing[(2*CHIP_WORDS+a)/4][((a%4)*16)+5])
                $fatal(1,"raw backing differs chip d07 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_e08.mem[a] !== backing[(2*CHIP_WORDS+a)/4][((a%4)*16)+4])
                $fatal(1,"raw backing differs chip e08 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_f07.mem[a] !== backing[(2*CHIP_WORDS+a)/4][((a%4)*16)+3])
                $fatal(1,"raw backing differs chip f07 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_g08.mem[a] !== backing[(2*CHIP_WORDS+a)/4][((a%4)*16)+2])
                $fatal(1,"raw backing differs chip g08 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_h07.mem[a] !== backing[(2*CHIP_WORDS+a)/4][((a%4)*16)+1])
                $fatal(1,"raw backing differs chip h07 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_i08.mem[a] !== backing[(2*CHIP_WORDS+a)/4][((a%4)*16)+0])
                $fatal(1,"raw backing differs chip i08 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_b08.mem[a] !== backing[(3*CHIP_WORDS+a)/4][((a%4)*16)+7])
                $fatal(1,"raw backing differs chip b08 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_c07.mem[a] !== backing[(3*CHIP_WORDS+a)/4][((a%4)*16)+6])
                $fatal(1,"raw backing differs chip c07 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_d08.mem[a] !== backing[(3*CHIP_WORDS+a)/4][((a%4)*16)+5])
                $fatal(1,"raw backing differs chip d08 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_e07.mem[a] !== backing[(3*CHIP_WORDS+a)/4][((a%4)*16)+4])
                $fatal(1,"raw backing differs chip e07 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_f08.mem[a] !== backing[(3*CHIP_WORDS+a)/4][((a%4)*16)+3])
                $fatal(1,"raw backing differs chip f08 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_g07.mem[a] !== backing[(3*CHIP_WORDS+a)/4][((a%4)*16)+2])
                $fatal(1,"raw backing differs chip g07 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_h08.mem[a] !== backing[(3*CHIP_WORDS+a)/4][((a%4)*16)+1])
                $fatal(1,"raw backing differs chip h08 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_i07.mem[a] !== backing[(3*CHIP_WORDS+a)/4][((a%4)*16)+0])
                $fatal(1,"raw backing differs chip i07 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_b09.mem[a] !== backing[(4*CHIP_WORDS+a)/4][((a%4)*16)+7])
                $fatal(1,"raw backing differs chip b09 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_c10.mem[a] !== backing[(4*CHIP_WORDS+a)/4][((a%4)*16)+6])
                $fatal(1,"raw backing differs chip c10 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_d09.mem[a] !== backing[(4*CHIP_WORDS+a)/4][((a%4)*16)+5])
                $fatal(1,"raw backing differs chip d09 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_e10.mem[a] !== backing[(4*CHIP_WORDS+a)/4][((a%4)*16)+4])
                $fatal(1,"raw backing differs chip e10 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_f09.mem[a] !== backing[(4*CHIP_WORDS+a)/4][((a%4)*16)+3])
                $fatal(1,"raw backing differs chip f09 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_g10.mem[a] !== backing[(4*CHIP_WORDS+a)/4][((a%4)*16)+2])
                $fatal(1,"raw backing differs chip g10 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_h09.mem[a] !== backing[(4*CHIP_WORDS+a)/4][((a%4)*16)+1])
                $fatal(1,"raw backing differs chip h09 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_i10.mem[a] !== backing[(4*CHIP_WORDS+a)/4][((a%4)*16)+0])
                $fatal(1,"raw backing differs chip i10 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_b10.mem[a] !== backing[(5*CHIP_WORDS+a)/4][((a%4)*16)+7])
                $fatal(1,"raw backing differs chip b10 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_c09.mem[a] !== backing[(5*CHIP_WORDS+a)/4][((a%4)*16)+6])
                $fatal(1,"raw backing differs chip c09 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_d10.mem[a] !== backing[(5*CHIP_WORDS+a)/4][((a%4)*16)+5])
                $fatal(1,"raw backing differs chip d10 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_e09.mem[a] !== backing[(5*CHIP_WORDS+a)/4][((a%4)*16)+4])
                $fatal(1,"raw backing differs chip e09 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_f10.mem[a] !== backing[(5*CHIP_WORDS+a)/4][((a%4)*16)+3])
                $fatal(1,"raw backing differs chip f10 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_g09.mem[a] !== backing[(5*CHIP_WORDS+a)/4][((a%4)*16)+2])
                $fatal(1,"raw backing differs chip g09 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_h10.mem[a] !== backing[(5*CHIP_WORDS+a)/4][((a%4)*16)+1])
                $fatal(1,"raw backing differs chip h10 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_i09.mem[a] !== backing[(5*CHIP_WORDS+a)/4][((a%4)*16)+0])
                $fatal(1,"raw backing differs chip i09 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_b11.mem[a] !== backing[(6*CHIP_WORDS+a)/4][((a%4)*16)+7])
                $fatal(1,"raw backing differs chip b11 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_c12.mem[a] !== backing[(6*CHIP_WORDS+a)/4][((a%4)*16)+6])
                $fatal(1,"raw backing differs chip c12 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_d11.mem[a] !== backing[(6*CHIP_WORDS+a)/4][((a%4)*16)+5])
                $fatal(1,"raw backing differs chip d11 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_e12.mem[a] !== backing[(6*CHIP_WORDS+a)/4][((a%4)*16)+4])
                $fatal(1,"raw backing differs chip e12 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_f11.mem[a] !== backing[(6*CHIP_WORDS+a)/4][((a%4)*16)+3])
                $fatal(1,"raw backing differs chip f11 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_g12.mem[a] !== backing[(6*CHIP_WORDS+a)/4][((a%4)*16)+2])
                $fatal(1,"raw backing differs chip g12 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_h11.mem[a] !== backing[(6*CHIP_WORDS+a)/4][((a%4)*16)+1])
                $fatal(1,"raw backing differs chip h11 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_i12.mem[a] !== backing[(6*CHIP_WORDS+a)/4][((a%4)*16)+0])
                $fatal(1,"raw backing differs chip i12 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_b12.mem[a] !== backing[(7*CHIP_WORDS+a)/4][((a%4)*16)+7])
                $fatal(1,"raw backing differs chip b12 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_c11.mem[a] !== backing[(7*CHIP_WORDS+a)/4][((a%4)*16)+6])
                $fatal(1,"raw backing differs chip c11 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_d12.mem[a] !== backing[(7*CHIP_WORDS+a)/4][((a%4)*16)+5])
                $fatal(1,"raw backing differs chip d12 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_e11.mem[a] !== backing[(7*CHIP_WORDS+a)/4][((a%4)*16)+4])
                $fatal(1,"raw backing differs chip e11 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_f12.mem[a] !== backing[(7*CHIP_WORDS+a)/4][((a%4)*16)+3])
                $fatal(1,"raw backing differs chip f12 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_g11.mem[a] !== backing[(7*CHIP_WORDS+a)/4][((a%4)*16)+2])
                $fatal(1,"raw backing differs chip g11 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_h12.mem[a] !== backing[(7*CHIP_WORDS+a)/4][((a%4)*16)+1])
                $fatal(1,"raw backing differs chip h12 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_i11.mem[a] !== backing[(7*CHIP_WORDS+a)/4][((a%4)*16)+0])
                $fatal(1,"raw backing differs chip i11 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_b15.mem[a] !== backing[(8*CHIP_WORDS+a)/4][((a%4)*16)+7])
                $fatal(1,"raw backing differs chip b15 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_c16.mem[a] !== backing[(8*CHIP_WORDS+a)/4][((a%4)*16)+6])
                $fatal(1,"raw backing differs chip c16 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_d15.mem[a] !== backing[(8*CHIP_WORDS+a)/4][((a%4)*16)+5])
                $fatal(1,"raw backing differs chip d15 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_e16.mem[a] !== backing[(8*CHIP_WORDS+a)/4][((a%4)*16)+4])
                $fatal(1,"raw backing differs chip e16 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_f15.mem[a] !== backing[(8*CHIP_WORDS+a)/4][((a%4)*16)+3])
                $fatal(1,"raw backing differs chip f15 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_g16.mem[a] !== backing[(8*CHIP_WORDS+a)/4][((a%4)*16)+2])
                $fatal(1,"raw backing differs chip g16 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_h15.mem[a] !== backing[(8*CHIP_WORDS+a)/4][((a%4)*16)+1])
                $fatal(1,"raw backing differs chip h15 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_i16.mem[a] !== backing[(8*CHIP_WORDS+a)/4][((a%4)*16)+0])
                $fatal(1,"raw backing differs chip i16 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_b16.mem[a] !== backing[(9*CHIP_WORDS+a)/4][((a%4)*16)+7])
                $fatal(1,"raw backing differs chip b16 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_c15.mem[a] !== backing[(9*CHIP_WORDS+a)/4][((a%4)*16)+6])
                $fatal(1,"raw backing differs chip c15 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_d16.mem[a] !== backing[(9*CHIP_WORDS+a)/4][((a%4)*16)+5])
                $fatal(1,"raw backing differs chip d16 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_e15.mem[a] !== backing[(9*CHIP_WORDS+a)/4][((a%4)*16)+4])
                $fatal(1,"raw backing differs chip e15 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_f16.mem[a] !== backing[(9*CHIP_WORDS+a)/4][((a%4)*16)+3])
                $fatal(1,"raw backing differs chip f16 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_g15.mem[a] !== backing[(9*CHIP_WORDS+a)/4][((a%4)*16)+2])
                $fatal(1,"raw backing differs chip g15 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_h16.mem[a] !== backing[(9*CHIP_WORDS+a)/4][((a%4)*16)+1])
                $fatal(1,"raw backing differs chip h16 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_i15.mem[a] !== backing[(9*CHIP_WORDS+a)/4][((a%4)*16)+0])
                $fatal(1,"raw backing differs chip i15 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_b17.mem[a] !== backing[(10*CHIP_WORDS+a)/4][((a%4)*16)+7])
                $fatal(1,"raw backing differs chip b17 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_c18.mem[a] !== backing[(10*CHIP_WORDS+a)/4][((a%4)*16)+6])
                $fatal(1,"raw backing differs chip c18 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_d17.mem[a] !== backing[(10*CHIP_WORDS+a)/4][((a%4)*16)+5])
                $fatal(1,"raw backing differs chip d17 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_e18.mem[a] !== backing[(10*CHIP_WORDS+a)/4][((a%4)*16)+4])
                $fatal(1,"raw backing differs chip e18 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_f17.mem[a] !== backing[(10*CHIP_WORDS+a)/4][((a%4)*16)+3])
                $fatal(1,"raw backing differs chip f17 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_g18.mem[a] !== backing[(10*CHIP_WORDS+a)/4][((a%4)*16)+2])
                $fatal(1,"raw backing differs chip g18 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_h17.mem[a] !== backing[(10*CHIP_WORDS+a)/4][((a%4)*16)+1])
                $fatal(1,"raw backing differs chip h17 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_i18.mem[a] !== backing[(10*CHIP_WORDS+a)/4][((a%4)*16)+0])
                $fatal(1,"raw backing differs chip i18 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_b18.mem[a] !== backing[(11*CHIP_WORDS+a)/4][((a%4)*16)+7])
                $fatal(1,"raw backing differs chip b18 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_c17.mem[a] !== backing[(11*CHIP_WORDS+a)/4][((a%4)*16)+6])
                $fatal(1,"raw backing differs chip c17 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_d18.mem[a] !== backing[(11*CHIP_WORDS+a)/4][((a%4)*16)+5])
                $fatal(1,"raw backing differs chip d18 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_e17.mem[a] !== backing[(11*CHIP_WORDS+a)/4][((a%4)*16)+4])
                $fatal(1,"raw backing differs chip e17 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_f18.mem[a] !== backing[(11*CHIP_WORDS+a)/4][((a%4)*16)+3])
                $fatal(1,"raw backing differs chip f18 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_g17.mem[a] !== backing[(11*CHIP_WORDS+a)/4][((a%4)*16)+2])
                $fatal(1,"raw backing differs chip g17 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_h18.mem[a] !== backing[(11*CHIP_WORDS+a)/4][((a%4)*16)+1])
                $fatal(1,"raw backing differs chip h18 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_i17.mem[a] !== backing[(11*CHIP_WORDS+a)/4][((a%4)*16)+0])
                $fatal(1,"raw backing differs chip i17 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_b19.mem[a] !== backing[(12*CHIP_WORDS+a)/4][((a%4)*16)+7])
                $fatal(1,"raw backing differs chip b19 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_c20.mem[a] !== backing[(12*CHIP_WORDS+a)/4][((a%4)*16)+6])
                $fatal(1,"raw backing differs chip c20 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_d19.mem[a] !== backing[(12*CHIP_WORDS+a)/4][((a%4)*16)+5])
                $fatal(1,"raw backing differs chip d19 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_e20.mem[a] !== backing[(12*CHIP_WORDS+a)/4][((a%4)*16)+4])
                $fatal(1,"raw backing differs chip e20 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_f19.mem[a] !== backing[(12*CHIP_WORDS+a)/4][((a%4)*16)+3])
                $fatal(1,"raw backing differs chip f19 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_g20.mem[a] !== backing[(12*CHIP_WORDS+a)/4][((a%4)*16)+2])
                $fatal(1,"raw backing differs chip g20 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_h19.mem[a] !== backing[(12*CHIP_WORDS+a)/4][((a%4)*16)+1])
                $fatal(1,"raw backing differs chip h19 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_i20.mem[a] !== backing[(12*CHIP_WORDS+a)/4][((a%4)*16)+0])
                $fatal(1,"raw backing differs chip i20 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_b20.mem[a] !== backing[(13*CHIP_WORDS+a)/4][((a%4)*16)+7])
                $fatal(1,"raw backing differs chip b20 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_c19.mem[a] !== backing[(13*CHIP_WORDS+a)/4][((a%4)*16)+6])
                $fatal(1,"raw backing differs chip c19 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_d20.mem[a] !== backing[(13*CHIP_WORDS+a)/4][((a%4)*16)+5])
                $fatal(1,"raw backing differs chip d20 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_e19.mem[a] !== backing[(13*CHIP_WORDS+a)/4][((a%4)*16)+4])
                $fatal(1,"raw backing differs chip e19 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_f20.mem[a] !== backing[(13*CHIP_WORDS+a)/4][((a%4)*16)+3])
                $fatal(1,"raw backing differs chip f20 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_g19.mem[a] !== backing[(13*CHIP_WORDS+a)/4][((a%4)*16)+2])
                $fatal(1,"raw backing differs chip g19 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_h20.mem[a] !== backing[(13*CHIP_WORDS+a)/4][((a%4)*16)+1])
                $fatal(1,"raw backing differs chip h20 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_i19.mem[a] !== backing[(13*CHIP_WORDS+a)/4][((a%4)*16)+0])
                $fatal(1,"raw backing differs chip i19 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_b21.mem[a] !== backing[(14*CHIP_WORDS+a)/4][((a%4)*16)+11])
                $fatal(1,"raw backing differs chip b21 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_c22.mem[a] !== backing[(14*CHIP_WORDS+a)/4][((a%4)*16)+10])
                $fatal(1,"raw backing differs chip c22 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_c23.mem[a] !== backing[(14*CHIP_WORDS+a)/4][((a%4)*16)+9])
                $fatal(1,"raw backing differs chip c23 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_d21.mem[a] !== backing[(14*CHIP_WORDS+a)/4][((a%4)*16)+8])
                $fatal(1,"raw backing differs chip d21 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_e22.mem[a] !== backing[(14*CHIP_WORDS+a)/4][((a%4)*16)+7])
                $fatal(1,"raw backing differs chip e22 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_e23.mem[a] !== backing[(14*CHIP_WORDS+a)/4][((a%4)*16)+6])
                $fatal(1,"raw backing differs chip e23 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_f21.mem[a] !== backing[(14*CHIP_WORDS+a)/4][((a%4)*16)+5])
                $fatal(1,"raw backing differs chip f21 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_g22.mem[a] !== backing[(14*CHIP_WORDS+a)/4][((a%4)*16)+4])
                $fatal(1,"raw backing differs chip g22 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_g23.mem[a] !== backing[(14*CHIP_WORDS+a)/4][((a%4)*16)+3])
                $fatal(1,"raw backing differs chip g23 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_h21.mem[a] !== backing[(14*CHIP_WORDS+a)/4][((a%4)*16)+2])
                $fatal(1,"raw backing differs chip h21 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_i22.mem[a] !== backing[(14*CHIP_WORDS+a)/4][((a%4)*16)+1])
                $fatal(1,"raw backing differs chip i22 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_i23.mem[a] !== backing[(14*CHIP_WORDS+a)/4][((a%4)*16)+0])
                $fatal(1,"raw backing differs chip i23 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_b22.mem[a] !== backing[(15*CHIP_WORDS+a)/4][((a%4)*16)+11])
                $fatal(1,"raw backing differs chip b22 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_b23.mem[a] !== backing[(15*CHIP_WORDS+a)/4][((a%4)*16)+10])
                $fatal(1,"raw backing differs chip b23 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_c21.mem[a] !== backing[(15*CHIP_WORDS+a)/4][((a%4)*16)+9])
                $fatal(1,"raw backing differs chip c21 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_d22.mem[a] !== backing[(15*CHIP_WORDS+a)/4][((a%4)*16)+8])
                $fatal(1,"raw backing differs chip d22 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_d23.mem[a] !== backing[(15*CHIP_WORDS+a)/4][((a%4)*16)+7])
                $fatal(1,"raw backing differs chip d23 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_e21.mem[a] !== backing[(15*CHIP_WORDS+a)/4][((a%4)*16)+6])
                $fatal(1,"raw backing differs chip e21 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_f22.mem[a] !== backing[(15*CHIP_WORDS+a)/4][((a%4)*16)+5])
                $fatal(1,"raw backing differs chip f22 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_f23.mem[a] !== backing[(15*CHIP_WORDS+a)/4][((a%4)*16)+4])
                $fatal(1,"raw backing differs chip f23 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_g21.mem[a] !== backing[(15*CHIP_WORDS+a)/4][((a%4)*16)+3])
                $fatal(1,"raw backing differs chip g21 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_h22.mem[a] !== backing[(15*CHIP_WORDS+a)/4][((a%4)*16)+2])
                $fatal(1,"raw backing differs chip h22 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_h23.mem[a] !== backing[(15*CHIP_WORDS+a)/4][((a%4)*16)+1])
                $fatal(1,"raw backing differs chip h23 address %d",a);
        for(integer a=0;a<CHIP_WORDS;a++)
            if(original.u_i21.mem[a] !== backing[(15*CHIP_WORDS+a)/4][((a%4)*16)+0])
                $fatal(1,"raw backing differs chip i21 address %d",a);
        for(integer a=0;a<4*CHIP_WORDS;a++)stored_ones+=$countones(backing[a]);
        $display("Stored one bits: %d",stored_ones);
        if(writes<100 || output_changes<100 || stored_ones<100)$fatal(1,"insufficient board activity: writes=%0d output_changes=%0d reads=%0d",writes,output_changes,reads);
        if(raw_trace_fd)begin
            // kind2 records nonzero FINAL words from the actual physical
            // model above (zeros are implicit). kind3 terminates the full
            // array scan with its word count. This detects dropped writes
            // even when all captured read replies happened to be zero.
            for(integer a=0;a<4*CHIP_WORDS;a++)
                for(integer lane=0;lane<4;lane++)
                    if(backing[a][lane*16+:16]!=0)
                        $fwrite(raw_trace_fd,"%0d,2,%0h,%0h\n",host_cycles,
                            a*4+lane,backing[a][lane*16+:16]);
            $fwrite(raw_trace_fd,"%0d,3,%0h,0\n",host_cycles,16*CHIP_WORDS);
        end
        $display("STORAGE-BOARD PASS: %d complete-board cycles, %d backing reads, %d writes, %d output transitions, all %0d stored bits identical, prefetch=%0d host_cycles=%0d",steps,reads,writes,output_changes,144*CHIP_WORDS,PREFETCH_NEXT,host_cycles);
        $finish;
    end
    initial begin #100000000;$fatal(1,"board test timeout");end
endmodule
