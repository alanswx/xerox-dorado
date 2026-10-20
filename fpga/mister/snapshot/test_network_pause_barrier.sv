`timescale 1ns/1ps
module tb;
    reg clk=0,machine_reseed=0,owner_hold=0,guest_held=0,abort=0,device_reset=0;
    reg engine_idle=0,parser_idle=0,parser_held=0,device_fault=0;
    wire hold,parser_hold,ready,fault;
    dorado_network_pause_barrier dut(.*);
    integer checks=0,drain_cases=0,ready_cases=0;
    task tick;begin #1;clk=1;#1;clk=0;#1;end endtask
    task check(input bit ok,input string message);
        begin checks=checks+1;if(!ok)$fatal(1,"%s",message);end
    endtask
    function automatic reg val(input integer n);
        case(n)0:val=0;1:val=1;2:val=1'bx;3:val=1'bz;endcase
    endfunction
    task reseed;
        begin
            machine_reseed=1;owner_hold=0;guest_held=0;abort=0;device_reset=0;
            engine_idle=0;parser_idle=0;parser_held=0;device_fault=0;
            tick();machine_reseed=0;#1;
            check(!hold&&!parser_hold&&!ready&&!fault,"reseed failed");
        end
    endtask
    task enter;
        begin
            reseed();owner_hold=1;guest_held=1;engine_idle=1;parser_idle=1;parser_held=1;
            tick();check(hold&&parser_hold&&!ready&&!fault,"drain staging differs");
            tick();check(!ready,"ready before settling");
            tick();check(!ready,"ready before two settling clocks");
            tick();check(ready&&hold&&parser_hold&&!fault,"frozen boundary missing");
        end
    endtask
    integer a,b,c,d,e,f,g,h;
    initial begin
        for(a=0;a<4;a=a+1)for(b=0;b<4;b=b+1)
        for(c=0;c<4;c=c+1)for(d=0;d<4;d=d+1)begin
            reseed();owner_hold=1;tick();guest_held=val(a);engine_idle=val(b);
            parser_idle=val(c);parser_held=val(d);repeat(4)tick();
            if(a==1&&b==1&&c==1&&d==1)check(ready===1'b1,"complete drain did not qualify");
            else check(ready===1'b0,"unknown/incomplete drain qualified");
            check(hold===1'b1&&parser_hold===1'b1,"drain lost hold");drain_cases=drain_cases+1;
        end
        for(a=0;a<4;a=a+1)for(b=0;b<4;b=b+1)
        for(c=0;c<4;c=c+1)for(d=0;d<4;d=d+1)
        for(e=0;e<4;e=e+1)for(f=0;f<4;f=f+1)
        for(g=0;g<4;g=g+1)for(h=0;h<4;h=h+1)begin
            enter();owner_hold=val(a);guest_held=val(b);engine_idle=val(c);parser_idle=val(d);
            parser_held=val(e);abort=val(f);device_reset=val(g);device_fault=val(h);#1;
            if(a==1&&b==1&&c==1&&d==1&&e==1&&f==0&&g==0&&h==0)
                check(ready===1'b1,"complete frozen boundary not ready");
            else check(ready===1'b0,"unknown/incomplete frozen boundary ready");
            ready_cases=ready_cases+1;
        end
        // Release drops the parser request first, and waits for its actual
        // latch. An unknown latch can never release guest or memory boards.
        for(a=0;a<4;a=a+1)begin
            enter();owner_hold=0;tick();
            check(hold&&!parser_hold&&!ready,"parser release did not retain guest hold");
            parser_held=val(a);repeat(3)tick();
            if(a==0)check(!hold&&!fault,"deselected parser did not release");
            else check(hold&&!fault,"held/unknown parser released guest");
        end
        // Losing any frozen dependency, unknown fault input or device reset
        // quarantines. Caller cancellation, PLL recovery and parser reset
        // cannot undo quarantine.
        for(a=0;a<7;a=a+1)for(b=0;b<3;b=b+1)begin
            enter();case(a)
                0:guest_held=b==0?1'b0:val(b+1);
                1:engine_idle=b==0?1'b0:val(b+1);
                2:parser_idle=b==0?1'b0:val(b+1);
                3:parser_held=b==0?1'b0:val(b+1);
                4:abort=val(b+1);5:device_reset=val(b+1);6:device_fault=val(b+1);
            endcase
            tick();check(fault&&hold&&parser_hold&&!ready,"lost frozen dependency escaped quarantine");
            owner_hold=0;guest_held=1;engine_idle=1;parser_idle=1;parser_held=0;
            abort=0;device_reset=0;device_fault=0;
            repeat(6)begin tick();check(fault&&hold&&parser_hold&&!ready,"quarantine released without reseed");end
        end
        reseed();owner_hold=1;tick();owner_hold=0;parser_held=0;repeat(3)tick();
        check(!hold&&!parser_hold&&!ready&&!fault,"early cancellation failed");
        // Unknown owner permission must inhibit guest advance immediately.
        for(a=2;a<4;a=a+1)begin
            reseed();owner_hold=val(a);#1;check(hold===1'b1&&parser_hold===1'b1&&!ready,"unknown owner failed open");
            repeat(5)tick();check(hold===1'b1&&!ready,"unknown owner acknowledged");
        end
        $display("NETWORK PAUSE FOURSTATE PASS checks=%0d drain_cases=%0d ready_cases=%0d state_bits=5",checks,drain_cases,ready_cases);
        $finish;
    end
endmodule
