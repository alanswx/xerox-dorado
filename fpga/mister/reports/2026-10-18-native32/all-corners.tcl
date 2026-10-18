project_open Dorado
foreach model {fast slow} {
 create_timing_netlist -model $model
 read_sdc
 update_timing_netlist
 foreach mode {setup hold recovery removal} {
  report_timing -$mode -npaths 3 -detail full_path -file native32-$mode-all-$model.rpt
 }
 report_min_pulse_width -file native32-mpw-all-$model.rpt
 delete_timing_netlist
}
project_close
