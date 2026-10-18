// Independent scoreboard against the actual gated clock, including changes
// AFTER the falling-edge gate latch, overlapping waits and counter rollover.
`timescale 1ns/1ps
module tb_pause_monitor;
    reg clk=0;always #5 clk=~clk;
    reg reset=1,stop=0,pack=1,storage=1,initialized=1,step=0;
    reg [8:0] events=0;
    reg [2:0] phase=0;
    reg [31:0] reads=0,writes=0;
    wire machine=clk && step;
    wire [7:0] hosts,machines;
    wire [191:0] counters;
    dorado_speed_monitor #(.BITS(8),.TRACK_CAUSES(1)) dut(
        .clk(clk),.reset(reset),.machine_step(step),.host_edges(hosts),.machine_edges(machines),
        .reset_requested(stop),.pack_ready(pack),.storage_ready(storage),
        .storage_initialized(initialized),.storage_events(events),.pack_host_phase(phase),
        .pack_reads(reads),.pack_writes(writes),.performance(counters));
    integer expected[0:23];
    integer category=2,ddr_streak=0,read_streak=0,write_streak=0,edges=0,checks=0;
    bit init_sample=0,ddr_sample=0,pending=0;
    always @(negedge clk)begin
        step<=!stop && pack && storage;
        casez({stop,pack,storage})
        3'b1??:category=2;
        3'b011:category=1;
        3'b001:category=3;
        3'b010:category=4;
        3'b000:category=5;
        endcase
        init_sample=initialized;ddr_sample=!stop && !storage;
    end
    always @(posedge machine)if(!reset)edges++;
    always @(posedge clk)begin
        if(reset)begin
            for(integer i=0;i<24;i++)expected[i]=0;
            edges=0;ddr_streak=0;read_streak=0;write_streak=0;pending=0;
        end else begin
            expected[0]++;expected[category]++;
            if(!init_sample && category!=1)expected[6]++;
            for(integer bit_index=0;bit_index<4;bit_index++)
                if(events[bit_index])expected[7+bit_index]++;
            if(events[8])expected[11]++;
            if(events[5])expected[12]++;
            if(pending)expected[13]++;
            case({events[2],events[4]})
            2'b10:pending=1;
            2'b01,2'b11:pending=0;
            default:begin end
            endcase
            if(events[7])expected[14]++;
            case(phase)
            3:expected[15]++;
            4:expected[16]++;
            1:expected[17]++;
            2:expected[18]++;
            default:begin end
            endcase
            ddr_streak=ddr_sample ? ddr_streak+1 : 0;
            read_streak=(phase==3 || phase==4) ? read_streak+1 : 0;
            write_streak=(phase==1 || phase==2) ? write_streak+1 : 0;
            if(ddr_streak>expected[19])expected[19]=ddr_streak;
            if(read_streak>expected[20])expected[20]=read_streak;
            if(write_streak>expected[21])expected[21]=write_streak;
        end
        expected[22]=reads;expected[23]=writes;
        #1;
        for(integer i=0;i<24;i++)
            if(counters[i*8+:8]!==8'(expected[i]))
                $fatal(1,"counter %0d got %0d expected %0d",i,counters[i*8+:8],8'(expected[i]));
        if(machines!==8'(edges))$fatal(1,"enabled count differs from actual machine edges");
        if(8'(expected[1]+expected[2]+expected[3]+expected[4]+expected[5])!==hosts)
            $fatal(1,"pause categories do not partition host edges");
        checks++;
    end
    initial begin
        repeat(3)@(posedge clk);#2;reset=0;
        for(integer n=0;n<800;n++)begin
            @(posedge clk);#2;
            stop=(n%11==0);pack=n[0];storage=n[1];initialized=(n>25);
            events=9'((n*13)^(n>>2));phase=3'((n/4)%5);
            reads=32'(n*3);writes=32'(n*7);
            if(n==390)reset=1;
            if(n==394)reset=0;
            @(negedge clk);#1;
            // A raw-rising-edge sampler must fail this test.
            stop=~stop;pack=~pack;storage=~storage;initialized=~initialized;
        end
        @(posedge clk);#2;
        if(checks<800 || expected[5]==0 || expected[11]==0 || expected[13]==0)
            $fatal(1,"insufficient monitor coverage");
        $display("PAUSE-MONITOR PASS: all24 counters, actual gated edges, overlap, pre-gate sampling, reset, wrap, DDR/ARM phases and maxima");
        $finish;
    end
endmodule
