# Run after quartus_sta Dorado, from the fitted core directory:
# quartus_sta -t /path/to/fpga/mister/audit_native_clocks.tcl
# Slack reports alone cannot qualify a design with an unconstrained clock.
project_open Dorado
create_timing_netlist -model slow
read_sdc
update_timing_netlist
report_clocks -file native-clocks.rpt
report_ucp -file native-unconstrained.rpt
delete_timing_netlist
project_close
