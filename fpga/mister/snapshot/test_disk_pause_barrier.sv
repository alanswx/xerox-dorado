`timescale 1ns/1ps
module tb;
    reg clk=0,machine_reseed=0,owner_hold=0,guest_held=0,abort=0,device_reset=0;
    reg device_idle=0,device_held=0,device_fault=0;
    wire hold,device_hold,ready,fault;
    dorado_disk_pause_barrier dut(.*);
    integer checks=0,drain_cases=0,ready_cases=0,a,b,c,d,e,f,g;
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
            device_idle=0;device_held=0;device_fault=0;tick();machine_reseed=0;#1;
            check(!hold&&!device_hold&&!ready&&!fault,"disk coordinator reseed failed");
        end
    endtask
    task enter;
        begin
            reseed();owner_hold=1;guest_held=1;device_idle=1;device_held=1;tick();
            check(hold&&device_hold&&!ready&&!fault,"disk drain staging differs");
            tick();check(ready&&hold&&device_hold&&!fault,"disk frozen boundary missing");
        end
    endtask
    initial begin
        for(a=0;a<4;a=a+1)for(b=0;b<4;b=b+1)for(c=0;c<4;c=c+1)begin
            reseed();owner_hold=1;tick();guest_held=val(a);device_idle=val(b);device_held=val(c);repeat(3)tick();
            if(a==1&&b==1&&c==1)check(ready===1'b1,"complete disk drain did not qualify");
            else check(ready===1'b0,"unknown/incomplete disk drain qualified");
            check(hold===1'b1&&device_hold===1'b1,"disk drain lost hold");drain_cases=drain_cases+1;
        end
        for(a=0;a<4;a=a+1)for(b=0;b<4;b=b+1)for(c=0;c<4;c=c+1)
        for(d=0;d<4;d=d+1)for(e=0;e<4;e=e+1)for(f=0;f<4;f=f+1)for(g=0;g<4;g=g+1)begin
            enter();owner_hold=val(a);guest_held=val(b);device_idle=val(c);device_held=val(d);
            abort=val(e);device_reset=val(f);device_fault=val(g);#1;
            if(a==1&&b==1&&c==1&&d==1&&e==0&&f==0&&g==0)check(ready===1'b1,"valid disk boundary not ready");
            else check(ready===1'b0,"unknown/incomplete disk boundary ready");ready_cases=ready_cases+1;
        end
        for(a=0;a<4;a=a+1)begin
            enter();owner_hold=0;tick();check(hold&&!device_hold&&!ready,"disk latch release lost guest hold");
            device_held=val(a);repeat(3)tick();
            if(a==0)check(!hold&&!fault,"quiet disk did not release guest");
            else check(hold&&!fault,"active/unknown disk latch released guest");
        end
        for(a=0;a<6;a=a+1)begin
            enter();owner_hold=0;
            case(a)0:guest_held=0;1:device_idle=0;2:device_held=0;3:abort=1;4:device_reset=1;5:device_fault=1;endcase
            tick();check(fault&&hold&&device_hold&&!ready,"disk revocation coincident with release escaped quarantine");
            owner_hold=0;guest_held=1;device_idle=1;device_held=0;abort=0;device_reset=0;device_fault=0;
            repeat(8)begin tick();check(fault&&hold&&device_hold&&!ready,"disk quarantine released without machine reseed");end
        end
        reseed();owner_hold=1;tick();owner_hold=0;device_held=0;repeat(3)tick();
        check(!hold&&!device_hold&&!ready&&!fault,"pre-freeze disk cancellation failed");
        for(a=2;a<4;a=a+1)begin
            reseed();owner_hold=val(a);#1;check(hold===1'b1&&device_hold===1'b1&&!ready,"unknown disk owner failed open");
        end
        $display("DISK COORDINATOR FOURSTATE PASS checks=%0d drain_cases=%0d ready_cases=%0d state_bits=3",checks,drain_cases,ready_cases);
        $finish;
    end
endmodule
