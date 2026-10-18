project_open Dorado
create_timing_netlist -model slow
read_sdc
update_timing_netlist
set core_clock {emu|pll|pll_inst|altera_pll_i|general[0].gpll~PLL_OUTPUT_COUNTER|divclk}
report_timing -setup -from_clock $core_clock -to_clock $core_clock -npaths 10 -detail full_path -file clock-audit-core-setup.rpt
report_clock_fmax_summary -file clock-audit-fmax.rpt
delete_timing_netlist
project_close
