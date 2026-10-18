// Reuse the actual processor/control/memory/IFU RTL and CP-bus startup. Tiny
// microprograms increment T once per instruction, independently checking
// the proposed monitor against architectural register changes. No CPU model
// supplies expected instruction counts. Generated fixtures are plain IM bits.
`timescale 1ns/1ps
module tb_instruction_progress;
    tb_exec harness();
    wire [31:0] issued,held,cycles,clocks;
    dorado_instruction_monitor monitor(
        .clk(harness.sys_clk),.reset(1'b0),
        .start_cycle(harness.m.instruction_signals[4]),
        .repeat_current(harness.m.instruction_signals[3]),
        .clock2(harness.m.instruction_signals[2]),
        .running(harness.m.instruction_signals[1]),.jam(harness.m.instruction_signals[0]),
        .issued(issued),.held(held),.cycles(cycles),.clocks(clocks));
    integer arm_pc=256,expect_holds=0,extended=0;
    initial begin
        void'($value$plusargs("arm_pc=%d",arm_pc));
        void'($value$plusargs("expect_holds=%d",expect_holds));
        void'($value$plusargs("extended=%d",extended));
    end
    reg armed=0;
    reg [15:0] first_t,last_t;
    reg [31:0] first_issued,first_held,first_cycles,first_clocks;
    integer changes=0,pc_changes=0;
    reg [11:0] last_pc;
    always @(negedge harness.sys_clk)begin
        if(!armed && harness.cia_now==12'(arm_pc) &&
           harness.brc_t>16'd140 && harness.brc_t<16'd150)begin
            armed=1;first_t=harness.brc_t;last_t=harness.brc_t;
            first_issued=issued;first_held=held;
            first_cycles=cycles;first_clocks=clocks;last_pc=harness.cia_now;
        end else if(armed)begin
            if(harness.cia_now!=last_pc)pc_changes++;
            last_pc=harness.cia_now;
            if(harness.brc_t!=last_t)begin
                if(harness.brc_t!==last_t+16'd1)$fatal(1,"T did not increment by one");
                last_t=harness.brc_t;changes++;
                // Boundaries have the same pipeline phase: no +/- allowance
                // in the actual instruction/architectural comparison.
                if(changes==100)begin
                    if(issued-first_issued!=100 || 16'(harness.brc_t-first_t)!=100)
                        $fatal(1,"issued %0d, architectural increments100",issued-first_issued);
                    if(cycles-first_cycles!=(issued-first_issued)+(held-first_held))
                        $fatal(1,"cycle partition");
                    if(expect_holds && held-first_held<10)$fatal(1,"holds not exercised");
                    if(!expect_holds && held!=first_held)$fatal(1,"unexpected hold");
                    if(extended && (clocks-first_clocks<395 || clocks-first_clocks>405))
                        $fatal(1,"IM/TPC extension not exercised");
                    if(!extended && !expect_holds && clocks-first_clocks!=200)
                        $fatal(1,"ordinary instructions did not use two clocks");
                    if(!extended && pc_changes!=0)$fatal(1,"self-loop unexpectedly changed PC");
                    $display("INSTRUCTION-PROGRESS PASS increments=100 issued=%0d held=%0d cycles=%0d clocks=%0d PCchanges=%0d",
                        issued-first_issued,held-first_held,cycles-first_cycles,
                        clocks-first_clocks,pc_changes);
                    $finish;
                end
            end
        end
    end
    initial begin #1000000;$fatal(1,"instruction-progress deadline");end
endmodule
