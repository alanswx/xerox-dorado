# Raw SDRAM backend,20..50MHz host; physical SDRAM clock is inverted.
# AS4C32M16SB Rev1.2 table16: CL2 tAC max6ns, tOH min2.5ns,
# input setup1.5ns/hold0.8ns. Budget1ns for board flight/skew in each
# interface calculation (module plugs directly into the MiSTer header).
# No false paths or multicycle exceptions are added here.
set sdram_source [get_pins -nowarn {emu|pll|pll_inst|altera_pll_i|general[0].gpll~PLL_OUTPUT_COUNTER|divclk}]
if {[get_collection_size $sdram_source] != 1} {
    error "Expected exactly one native PLL output for SDRAM clock"
}
create_generated_clock -name dorado_sdram_clk -source $sdram_source -invert -divide_by 1 [get_ports {SDRAM_CLK}]
set sdram_data [get_ports {SDRAM_DQ[*]}]
set sdram_outputs [get_ports {SDRAM_A[*] SDRAM_BA[*] SDRAM_nCS SDRAM_nRAS SDRAM_nCAS SDRAM_nWE SDRAM_DQML SDRAM_DQMH SDRAM_CKE SDRAM_DQ[*]}]
set_input_delay -clock dorado_sdram_clk -max 7.0 $sdram_data
set_input_delay -clock dorado_sdram_clk -min 2.5 $sdram_data
set_output_delay -clock dorado_sdram_clk -max 2.5 $sdram_outputs
set_output_delay -clock dorado_sdram_clk -min -1.8 $sdram_outputs
derive_clock_uncertainty
