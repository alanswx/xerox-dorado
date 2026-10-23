# Read-only fitted input-driver query. Run only after the guarded Python
# inspector has verified the builder/report processes are terminal.
project_open Dorado
create_timing_netlist -model slow
read_sdc
update_timing_netlist
set nodes [get_registers [list {*|r_front_upper_q*} {*|dorado_r_file_ports:files|altdpram:stk_h_rtl_0|*rdaddr_reg*} {*|dorado_r_file_ports:files|altdpram:stk_l_rtl_0|*rdaddr_reg*}]]
set seen [dict create]
set count 0
foreach_in_collection node $nodes {
 set name [get_register_info -name $node]
 if {[string match {*|r_front_upper_q*} $name] && ![regexp {\|r_front_upper_q\[(24|25|26|27|3[0-9]|40|41)\]$} $name]} {continue}
 incr count
 if {[get_register_info -is_latch $node]} {error "Unexpected latch in address register mapping: $name"}
 puts "STK_QUERY_REGISTER $name"
 foreach category {synch clock asynch} {
  foreach edge [get_register_info -${category}_edges $node] {
   set src [get_edge_info -src $edge]
   set dst [get_edge_info -dst $edge]
   set net "-"
   if {[get_node_info -type $src]=="pin"} {
    set netid [get_pin_info -net $src]
    if {$netid!=""} {set net [get_object_info -name $netid]}
   }
   puts "STK_QUERY_EDGE $category [get_node_info -name $src] -> [get_node_info -name $dst] net=$net"
  }
 }
 set cell [get_node_info -cell $node]
 if {![dict exists $seen $cell]} {
  dict set seen $cell 1
  puts "STK_QUERY_CELL [get_cell_info -name $cell] [get_cell_info -wysiwyg_type $cell]"
  foreach_in_collection pin [get_cell_info -in_pins $cell] {
   set netid [get_pin_info -net $pin]
   set net "-"
   if {$netid!=""} {set net [get_object_info -name $netid]}
   puts "STK_QUERY_PIN [get_pin_info -name $pin] net=$net"
   foreach category {synch clock asynch} {
    foreach edge [get_node_info -${category}_edges $pin] {
     set src [get_edge_info -src $edge]
     puts "STK_QUERY_DRIVER [get_pin_info -suffix $pin] $category [get_node_info -name $src] -> [get_node_info -name $pin]"
    }
   }
  }
 }
}
puts "STK_QUERY_COUNT $count"
delete_timing_netlist
project_close
