project_open Dorado
foreach model {fast slow} {
 create_timing_netlist -model $model
 read_sdc
 update_timing_netlist
 report_timing -hold -npaths 100 -detail full_path -file native32-hold-$model.rpt
 set core_clock {emu|pll|pll_inst|altera_pll_i|general[0].gpll~PLL_OUTPUT_COUNTER|divclk}
 report_timing -setup -from_clock $core_clock -to_clock $core_clock -npaths 10 -detail full_path -file native32-core-setup-$model.rpt
 report_clock_fmax_summary -file native32-fmax-$model.rpt
 delete_timing_netlist
}
project_close
