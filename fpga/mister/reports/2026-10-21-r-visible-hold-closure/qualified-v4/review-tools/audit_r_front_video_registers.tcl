# Read-only physical register inventory, after every native timing job ends.
project_open Dorado
create_timing_netlist -model slow
read_sdc
update_timing_netlist
foreach {label pattern} {
 ascal {*ascal:ascal|*}
 shadowmask {*shadowmask:HDMI_shadowmask|*}
 vga {*vga_out:vga_scaler_out|*}
 yc {*yc_out:yc_out|*}
} {
 set registers [get_registers $pattern]
 set count [get_collection_size $registers]
 if {$count==0} {error "No physical video registers for $label"}
 puts "VIDEO_REGISTERS $label count=$count"
 foreach_in_collection node $registers {
  puts "VIDEO_REGISTER $label [get_register_info -name $node]"
 }
}
delete_timing_netlist
project_close
