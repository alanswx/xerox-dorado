`timescale 1ns/1ps
module tb;
    reg clk_sys=0,machine_clk=0,reset=1,attached=1,readonly=0,remounted=0;
    reg snapshot_hold=0,snapshot_guest_held=0,sd_ack=0,sd_buff_wr=0;
    reg [12:0] sd_buff_addr=0;
    reg [15:0] sd_buff_dout=0;
    wire snapshot_idle,snapshot_held;
    dorado_pack_hps #(.SNAPSHOT_PAUSE(1),.HOST_TIMEOUT_CLOCKS(100000)) dut(
        .clk_sys(clk_sys),.machine_clk(machine_clk),.reset(reset),.attached(attached),.readonly(readonly),.remounted(remounted),
        .snapshot_hold(snapshot_hold),.snapshot_guest_held(snapshot_guest_held),.snapshot_idle(snapshot_idle),.snapshot_held(snapshot_held),
        .tagbus_n(12'hfff),.cyltag_n(1'b1),.headtag_n(1'b1),.drivetag_n(1'b1),.conttag_n(1'b1),
        .selected(1'b1),.data_adv(1'b0),.pulse(16'b0),.write_data_p(1'b1),.write_data_m(1'b0),
        .sd_ack(sd_ack),.sd_buff_addr(sd_buff_addr),.sd_buff_dout(sd_buff_dout),.sd_buff_wr(sd_buff_wr));
    integer checks=0,a,b,c,d,cases=0;
    reg [15:0] before_read;
    task tick;begin #1;clk_sys=1;#1;clk_sys=0;#1;end endtask
    task check(input bit ok,input string message);
        begin checks=checks+1;if(!ok)$fatal(1,"%s",message);end
    endtask
    function automatic reg val(input integer n);
        case(n)0:val=0;1:val=1;2:val=1'bx;3:val=1'bz;endcase
    endfunction
    task reseed;
        begin
            reset=1;attached=1;readonly=0;remounted=0;snapshot_hold=0;
            snapshot_guest_held=0;sd_ack=0;sd_buff_wr=0;sd_buff_addr=0;sd_buff_dout=0;
            tick();reset=0;#1;check(!snapshot_idle&&!snapshot_held,"reset did not revoke disk pause");
        end
    endtask
    task enter;
        begin
            reseed();snapshot_hold=1;snapshot_guest_held=1;tick();
            check(snapshot_idle&&snapshot_held,"quiet disk did not pause");
        end
    endtask
    initial begin
        for(a=0;a<8192;a=a+1)dut.buffer[a]=16'(a*17)^16'hc53a;
        // Admission cannot mistake unknown guest/host signals for a boundary.
        for(a=0;a<4;a=a+1)for(b=0;b<4;b=b+1)
        for(c=0;c<4;c=c+1)for(d=0;d<4;d=d+1)begin
            reseed();snapshot_hold=1;snapshot_guest_held=val(a);
            sd_ack=val(b);sd_buff_wr=val(c);remounted=val(d);tick();
            if(a==1&&b==0&&c==0&&d==0)check(snapshot_idle===1'b1,"complete disk boundary not ready");
            else check(snapshot_idle===1'b0&&!snapshot_held,"incomplete/unknown disk boundary acknowledged");
            cases=cases+1;
        end
        for(a=0;a<4;a=a+1)begin
            reseed();snapshot_hold=1;snapshot_guest_held=1;attached=val(a);tick();
            if(a<2)check(snapshot_idle===1'b1,"known attached/inactive disk not ready");
            else check(snapshot_idle===1'b0&&!snapshot_held,"unknown attachment acknowledged");
        end
        // A held port ignores unrelated host addresses and even unexpected
        // writes. Its acknowledgment revokes while activity is present; the
        // whole-machine coordinator must quarantine that failed lease.
        enter();before_read=dut.sd_buff_din;dut.buffer[17]=16'hc53a;
        sd_buff_addr=17;sd_buff_dout=16'hf00d;sd_buff_wr=1;tick();
        check(!snapshot_idle&&snapshot_held,"unexpected host write did not revoke ready");
        check(dut.buffer[17]===16'hc53a&&dut.sd_buff_din===before_read,"held disk RAM port changed");
        sd_buff_wr=0;tick();check(snapshot_idle,"quiet held disk not ready");
        for(a=0;a<4;a=a+1)begin
            enter();snapshot_hold=0;sd_ack=val(a);tick();
            if(a==0)check(!snapshot_held,"quiet disk release failed");
            else check(snapshot_held&&!snapshot_idle,"active/unknown ACK released disk hold");
        end
        enter();snapshot_hold=0;sd_buff_wr=1;sd_buff_addr=17;sd_buff_dout=16'hdead;
        dut.buffer[17]=16'hc53a;repeat(4)tick();
        check(snapshot_held&&dut.buffer[17]===16'hc53a,"release request bypassed retained RAM hold");
        sd_buff_wr=0;tick();check(!snapshot_held,"completed host tail failed to release");
        for(a=2;a<4;a=a+1)begin
            enter();snapshot_hold=val(a);repeat(4)tick();
            check(snapshot_held&&!snapshot_idle,"unknown hold request released or acknowledged");
        end
        enter();attached=0;#1;check(!snapshot_idle,"unmounted disk remained snapshot ready");
        reseed();attached=0;snapshot_hold=1;snapshot_guest_held=1;tick();
        check(snapshot_idle&&snapshot_held,"inactive disk did not acknowledge pause");
        attached=1;#1;check(!snapshot_idle,"new attachment remained snapshot ready");
        enter();remounted=1;#1;check(!snapshot_idle,"replacement disk remained snapshot ready");
        enter();reset=1;#1;check(!snapshot_idle,"reset disk remained snapshot ready");
        tick();check(!snapshot_held,"architectural reset failed to revoke disk lease");
        $display("DISK PAUSE FOURSTATE PASS checks=%0d admission_cases=%0d new_state_bits=2",checks,cases);
        $finish;
    end
endmodule
