`timescale 1ns/1ps
module tb;
    reg clk=0,machine_reseed=0,owner_hold=0,guest_held=0,abort=0;
    reg frontends_drained=0,arbiter_idle=0,raw_service_idle=0,frontends_frozen=0;
    wire guest_hold,frontend_hold,ready,fault;
    dorado_memory_pause_barrier dut(.*);
    integer checks=0;
    task tick;begin #1;clk=1;#1;clk=0;#1;end endtask
    task check(input bit ok,input string message);
        begin checks=checks+1;if(!ok)$fatal(1,"%s",message);end
    endtask
    function automatic reg val(input integer n);
        case(n) 0:val=0;1:val=1;2:val=1'bx;3:val=1'bz;endcase
    endfunction
    task reseed;
        begin
            machine_reseed=1;owner_hold=0;abort=0;guest_held=0;
            frontends_drained=0;arbiter_idle=0;raw_service_idle=0;frontends_frozen=0;
            tick();machine_reseed=0;#1;
            check(!guest_hold&&!frontend_hold&&!ready&&!fault,"machine reseed failed");
        end
    endtask
    task enter;
        begin
            reseed();owner_hold=1;guest_held=1;frontends_drained=1;
            arbiter_idle=1;raw_service_idle=1;tick();tick();
            check(guest_hold&&frontend_hold&&!ready&&!fault,"freeze staging differs");
            frontends_frozen=1;#1;check(ready,"frozen acknowledgment missing");
        end
    endtask
    integer a,b,c,d,e,f,drain_cases=0,ready_cases=0;
    initial begin
        // A raw/frontend-ready predicate alone is never a service barrier.
        // Unknown acknowledgments fail closed at every qualification input.
        for(a=0;a<4;a=a+1)for(b=0;b<4;b=b+1)
        for(c=0;c<4;c=c+1)for(d=0;d<4;d=d+1)begin
            reseed();owner_hold=1;tick();
            guest_held=val(a);frontends_drained=val(b);
            arbiter_idle=val(c);raw_service_idle=val(d);tick();
            if(a==1&&b==1&&c==1&&d==1)
                check(frontend_hold===1'b1,"complete drain did not freeze");
            else check(frontend_hold===1'b0,"incomplete/unknown drain froze frontend");
            check(ready===1'b0,"ready before frontend hold acknowledgment");drain_cases=drain_cases+1;
        end
        for(a=0;a<4;a=a+1)for(b=0;b<4;b=b+1)
        for(c=0;c<4;c=c+1)for(d=0;d<4;d=d+1)
        for(e=0;e<4;e=e+1)for(f=0;f<4;f=f+1)begin
            enter();owner_hold=val(a);guest_held=val(b);frontends_frozen=val(c);
            arbiter_idle=val(d);raw_service_idle=val(e);abort=val(f);#1;
            if(a==1&&b==1&&c==1&&d==1&&e==1&&f==0)
                check(ready===1'b1,"complete frozen boundary not ready");
            else check(ready===1'b0,"incomplete/unknown boundary ready");
            ready_cases=ready_cases+1;
        end
        enter();frontends_drained=0;tick();
        check(ready&&frontend_hold,"serial data rotation released freeze");
        owner_hold=0;tick();check(!guest_hold&&!frontend_hold&&!ready&&!fault,"normal release failed");
        // Abort and lost service ownership quarantine until a TRUE reseed.
        for(a=0;a<4;a=a+1)begin
            enter();case(a)0:abort=1;1:guest_held=0;2:arbiter_idle=0;3:raw_service_idle=0;endcase
            tick();check(fault&&frontend_hold&&guest_hold&&!ready,"fault not quarantined");
            owner_hold=0;abort=0;guest_held=1;arbiter_idle=1;raw_service_idle=1;
            repeat(8)begin tick();check(fault&&frontend_hold&&guest_hold&&!ready,"fault released without reseed");end
        end
        reseed();owner_hold=1;tick();abort=1;tick();
        check(fault&&frontend_hold&&guest_hold&&!ready,"drain abort released state");
        reseed();owner_hold=1;tick();owner_hold=0;tick();
        check(!fault&&!frontend_hold&&!guest_hold&&!ready,"safe pre-freeze cancellation failed");
        $display("MEMORY PAUSE FOURSTATE PASS checks=%0d drain_cases=%0d ready_cases=%0d state_bits=2",checks,drain_cases,ready_cases);
        $finish;
    end
endmodule
