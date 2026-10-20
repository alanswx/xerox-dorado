`timescale 1ns/1ps
// Real three-domain coordinators; device ACKs are independent test models.
// Separate packet/wire, Pilot/HPS and eight-MSA tests qualify those devices.
module tb;
    reg clk=0,machine_reseed=0,owner_hold=0,abort=0,device_reset=0;
    reg frontends_drained=0,arbiter_idle=0,raw_service_idle=0;
    reg engine_idle=0,parser_idle=0,parser_held=0,network_device_fault=0;
    reg disk_idle=0,disk_held=0,disk_device_fault=0;
    reg guest_step=0,frontends_frozen=0;
    wire global_hold,frontend_hold,memory_ready,memory_fault;
    wire network_hold,parser_hold,network_ready,network_fault;
    wire disk_hold,disk_request,disk_ready,disk_fault;
    wire guest_held=global_hold && !guest_step;
    dorado_memory_pause_barrier memory_pause(
        .clk(clk),.machine_reseed(machine_reseed),.owner_hold(owner_hold || network_hold || disk_hold),
        .guest_held(guest_held),.abort(abort || network_fault || disk_fault),
        .frontends_drained(frontends_drained),.arbiter_idle(arbiter_idle),
        .raw_service_idle(raw_service_idle),.frontends_frozen(frontends_frozen),
        .guest_hold(global_hold),.frontend_hold(frontend_hold),.ready(memory_ready),.fault(memory_fault));
    dorado_network_pause_barrier network_pause(
        .clk(clk),.machine_reseed(machine_reseed),.owner_hold(owner_hold),
        .guest_held(guest_held),.abort(abort || memory_fault || disk_fault),.device_reset(device_reset),
        .engine_idle(engine_idle),.parser_idle(parser_idle),.parser_held(parser_held),
        .device_fault(network_device_fault),.hold(network_hold),.parser_hold(parser_hold),
        .ready(network_ready),.fault(network_fault));
    dorado_disk_pause_barrier disk_pause(
        .clk(clk),.machine_reseed(machine_reseed),.owner_hold(owner_hold),
        .guest_held(guest_held),.abort(abort || memory_fault || network_fault),.device_reset(device_reset),
        .device_idle(disk_idle),.device_held(disk_held),.device_fault(disk_device_fault),
        .hold(disk_hold),.device_hold(disk_request),.ready(disk_ready),.fault(disk_fault));
    always @(negedge clk)guest_step<=!global_hold;
    always @(posedge clk)frontends_frozen<=frontend_hold;
    integer checks=0,a,b;
    task tick;begin #1;clk=1;#1;clk=0;#1;end endtask
    task check(input bit ok,input string message);
        begin checks=checks+1;if(!ok)$fatal(1,"%s",message);end
    endtask
    task reseed;
        begin
            machine_reseed=1;owner_hold=0;abort=0;device_reset=0;
            frontends_drained=0;arbiter_idle=0;raw_service_idle=0;
            engine_idle=0;parser_idle=0;parser_held=0;network_device_fault=0;
            disk_idle=0;disk_held=0;disk_device_fault=0;tick();machine_reseed=0;tick();
            check(!global_hold&&!frontend_hold&&!network_hold&&!disk_hold&&!memory_fault&&!network_fault&&!disk_fault&&guest_step,"services chain reseed failed");
        end
    endtask
    task enter;
        begin
            reseed();owner_hold=1;repeat(3)tick();
            check(global_hold&&!guest_step&&!frontend_hold&&!memory_ready&&!network_ready&&!disk_ready,"consumer stopped too late or frontend froze early");
            frontends_drained=1;arbiter_idle=1;raw_service_idle=1;repeat(3)tick();
            check(memory_ready&&frontend_hold&&!network_ready&&!disk_ready,"memory drain did not qualify independently");
            engine_idle=1;parser_idle=1;parser_held=1;repeat(4)tick();
            check(network_ready&&memory_ready&&!disk_ready,"network grant fabricated disk readiness");
            disk_idle=1;disk_held=1;repeat(2)tick();
            check(disk_ready&&network_ready&&memory_ready&&global_hold&&frontend_hold&&!guest_step,"three-domain boundary missing");
        end
    endtask
    initial begin
        enter();owner_hold=0;repeat(3)tick();
        check(global_hold&&frontend_hold&&!guest_step&&!parser_hold&&!disk_request,"release lost global hold");
        parser_held=0;repeat(4)tick();
        check(global_hold&&frontend_hold&&!guest_step&&!network_hold&&disk_hold,"network release thawed held disk");
        disk_held=1'bx;repeat(3)tick();check(global_hold&&frontend_hold&&!guest_step,"unknown disk latch thawed memory");
        disk_held=0;repeat(5)tick();check(!global_hold&&!frontend_hold&&guest_step&&!memory_fault&&!network_fault&&!disk_fault,"normal services release failed");
        // Reverse release order must retain the same global boundary.
        enter();owner_hold=0;repeat(2)tick();disk_held=0;repeat(4)tick();
        check(global_hold&&frontend_hold&&!guest_step&&network_hold&&!disk_hold,"disk release thawed held network parser");
        parser_held=0;repeat(5)tick();check(!global_hold&&guest_step,"reverse services release failed");
        for(a=0;a<2;a=a+1)for(b=0;b<4;b=b+1)begin
            if(a==1)enter();else begin reseed();owner_hold=1;tick();end
            case(b)0:abort=1;1:device_reset=1;2:network_device_fault=1;3:disk_device_fault=1;endcase
            repeat(3)tick();owner_hold=0;abort=0;device_reset=0;network_device_fault=0;disk_device_fault=0;
            parser_held=0;disk_held=0;frontends_drained=1;arbiter_idle=1;raw_service_idle=1;
            repeat(8)begin tick();check(global_hold&&frontend_hold&&!guest_step&&memory_fault&&network_fault&&disk_fault,"service fault did not quarantine every coordinator");end
        end
        $display("SERVICES CHAIN PASS checks=%0d coordinator_bits=10 physical_memory_devices=0",checks);$finish;
    end
endmodule
