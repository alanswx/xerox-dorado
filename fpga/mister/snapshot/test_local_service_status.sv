`timescale 1ns/1ps
module tb;
    localparam [63:0] SCHEMA=64'haf0b647cb43a3a62;
    reg clk=0,system_reset=1,transport_reset=0,enable=0,strobe=0;
    reg [15:0] din=0;
    reg [4:0] local_ack=0,foreign_permission=0;
    wire [4:0] services=foreign_permission & {2'b11,local_ack[2:0]};
    wire [35:0] original_bus,legacy_bus,service_bus;
    assign original_bus[35:33]={1'b0,enable,strobe};assign original_bus[31:16]=din;
    assign legacy_bus[35:33]={1'b0,enable,strobe};assign legacy_bus[31:16]=din;
    assign service_bus[35:33]={1'b0,enable,strobe};assign service_bus[31:16]=din;
    wire hold,command_hold,hold_update,hold_value,control_error,request,write,ready,done,fault,scan_abort;
    wire [3:0] board;wire [23:0] index;wire [17:0] data,result;
    wire [2:0] error;wire [11:0] board_request;
    wire [11:0] board_frozen=hold?12'hfff:12'b0;
    // Actual owner; endpoint state is a controlled model in this protocol test.
    dorado_board_state_fabric_msa owner(
        .clk(clk),.system_reset(system_reset),.transport_reset(transport_reset),
        .hold_update(hold_update),.hold_value(hold_value),.local_held(hold),.services_held(services),
        .hold(hold),.scan_abort(scan_abort),.command_hold(command_hold),.control_error(control_error),
        .request(request),.write(write),.board(board),.index(index),.data(data),.ready(ready),
        .done(done),.error(error),.result(result),.fault(fault),.board_frozen(board_frozen),
        .board_done(12'b0),.board_error(12'b0),.board_hold_required(12'b0),.board_fault(12'b0),
        .board_result(216'b0),.board_request(board_request));
    // INSTANCE_BLOCK
    integer checks=0,i,j,seed=32'h3a7564cb;
    reg [15:0] words[0:20];
    task check(input bit ok,input string message);
        begin checks=checks+1;if(!ok)$fatal(1,"%s",message);end
    endtask
    task tick;begin #1;clk=1;#1;clk=0;#1;
        check(original_bus[32]===legacy_bus[32] && original_bus[15:0]===legacy_bus[15:0],"default-off UIO output differs");
        // COMPARE_BLOCK
    end endtask
    task idle(input integer n);begin repeat(n)tick();end endtask
    task word(input [15:0] value);begin din=value;enable=1;strobe=1;tick();strobe=0;tick();end endtask
    task finish_packet;begin enable=0;idle(3);end endtask
    task control(input bit value);begin
        word(16'h77);word(16'hb04d);word(2);
        word(SCHEMA[15:0]);word(SCHEMA[31:16]);word(SCHEMA[47:32]);word(SCHEMA[63:48]);word({15'b0,value});finish_packet();
    end endtask
    task status(input [4:0] expected_local,input [4:0] expected_services,input bit expected_ready,input bit change_mid);
        begin
            word(16'h76);words[0]=service_bus[15:0];
            for(j=1;j<21;j=j+1)begin
                word(0);words[j]=service_bus[15:0];if(j==4&&change_mid)local_ack=0;
            end
            finish_packet();
            check(words[0]===16'hd0b0&&words[1]===2&&words[3]===12,"local status framing differs");
            check(words[2][8]===1'b1&&words[2][11]===1'b1,"local status capability or board-only scope missing");
            check(words[2][7]===expected_ready,"local ACK bypassed host permission or status tore");
            check(words[16]==={11'b0,expected_services},"foreign permission status tore");
            check(words[18]==={11'b0,expected_local},"local ACK snapshot tore or used foreign permission");
            check(words[19]===7&&words[20]===16'hac01,"local status mask/version differs");
            $write("LOCAL_SERVICE_STATUS");for(j=0;j<21;j=j+1)$write(" %04h",words[j]);$write("\n");
        end
    endtask
    initial begin
        idle(3);system_reset=0;idle(3);
        foreign_permission=5'h1d;local_ack=7;control(1);idle(3);
        check(hold&&!ready,"local disk ACK bypassed missing ARM lease");status(7,5'h1d,0,0);
        foreign_permission=5'h1f;local_ack=7;status(7,5'h1f,1,1);
        status(0,5'h18,0,0);
        local_ack=5'h1f;status(7,5'h1f,1,0);
        control(0);check(!hold,"local status changed ordinary hold release");
        // Arbitrary shared-bus traffic, malformed controls, status reads and
        // transport resets compare every original FF with the immutable UIO.
        for(i=0;i<512;i=i+1)begin
            local_ack=$random(seed);foreign_permission=$random(seed);
            word(16'h70+(i%11));word($random(seed));word($random(seed));finish_packet();
            if(i%43==0)begin transport_reset=1;tick();transport_reset=0;tick();end
            if(i%67==0)begin system_reset=1;tick();system_reset=0;tick();end
        end
        $display("LOCAL SERVICE STATUS PASS checks=%0d original_bits=ORIGINAL_BITS new_status_bits=5 permission_bypass=0",checks);$finish;
    end
endmodule
