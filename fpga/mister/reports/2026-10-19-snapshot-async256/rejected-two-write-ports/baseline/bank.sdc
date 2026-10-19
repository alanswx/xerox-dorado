create_clock -name host_clock -period 20.000 [get_ports host_clk]
derive_clock_uncertainty
set_input_delay -clock host_clock -max 2.000 [remove_from_collection [all_inputs] [get_ports host_clk]]
set_input_delay -clock host_clock -min 0.000 [remove_from_collection [all_inputs] [get_ports host_clk]]
set_output_delay -clock host_clock -max 2.000 [all_outputs]
set_output_delay -clock host_clock -min 0.000 [all_outputs]
