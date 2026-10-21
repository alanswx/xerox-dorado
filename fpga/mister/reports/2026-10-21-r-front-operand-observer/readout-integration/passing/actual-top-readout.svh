// Read-only operand monitor. The guest never reads this selector or payload.
`ifdef DORADO_OPERAND_MONITOR
wire [127:0] native_operand_debug;
wire operand_monitor_active=status[15] && !pack_fault;
wire [31:0] operand_word=status[14:11]<4
    ? native_operand_debug[127-32*status[14:11]-:32] : 32'hd0d0bad0;
`else
wire operand_monitor_active=1'b0;
wire [31:0] operand_word=32'd0;
`endif
dorado_ethernet_uio #(.MAX_WORDS(1024)
`ifdef DORADO_ETHERNET_DDR
    ,.DDR_CAPABLE(1)
`endif
) u_network_uio (
    .clk(clk_sys), .reset(~pll_locked), .bus(ethernet_ext_bus),
    .rx_ready(net_rx_ready), .tx_valid(net_tx_valid), .tx_words(net_tx_words), .tx_data(net_tx_data),
    .tx_good(net_tx_good), .tx_bad(net_tx_bad), .tx_dropped(net_tx_dropped), .rx_bad(net_rx_bad),
`ifdef DORADO_NATIVE
`ifdef DORADO_RX_MONITOR
`ifdef DORADO_PACK_FAULT_MONITOR
    .machine_debug(pack_fault ? pack_fault_debug : operand_monitor_active ? operand_word : speed_monitor_active ? count_debug : native_key_overflow ?
        {16'h4b51,7'd0,native_key_queued} :
        (processor_debug[31] ? baseboard_debug : receive_debug)), .machine_status(machine_status),
    .processor_debug(pack_fault ? pack_fault_extra : operand_monitor_active ? processor_debug : speed_monitor_active ? count_reference : processor_debug),
`else
    .machine_debug(operand_monitor_active ? operand_word : speed_monitor_active ? count_debug : receive_debug), .machine_status(machine_status),
    .processor_debug(operand_monitor_active ? processor_debug : speed_monitor_active ? count_reference : processor_debug),
`endif
`else
`ifdef DORADO_PACK_FAULT_MONITOR
    .machine_debug(pack_fault ? pack_fault_debug : operand_monitor_active ? operand_word : speed_monitor_active ? count_debug : baseboard_debug), .machine_status(machine_status),
    .processor_debug(pack_fault ? pack_fault_extra : operand_monitor_active ? processor_debug : speed_monitor_active ? count_reference : processor_debug),
`else
    .machine_debug(operand_monitor_active ? operand_word : speed_monitor_active ? count_debug : baseboard_debug), .machine_status(machine_status),
    .processor_debug(operand_monitor_active ? processor_debug : speed_monitor_active ? count_reference : processor_debug),
`endif
`endif
`else
    .machine_debug(32'd0), .machine_status(16'd0), .processor_debug(32'd0),
`endif
`ifdef DORADO_ETHERNET_DDR
    .tx_addr(net_spi_tx_addr), .tx_ack(net_spi_tx_ack), .rx_wr(net_spi_rx_wr), .rx_commit(net_spi_rx_commit),
    .rx_addr(net_spi_rx_addr), .rx_data(net_spi_rx_data), .rx_words(net_spi_rx_words),
    .ddr_stopped(net_ddr_stopped),.ddr_initialized(net_ddr_initialized),.ddr_fault(net_ddr_fault),
    .ddr_enable(net_ddr_enable),.ddr_epoch(net_ddr_epoch),.ddr_control_errors(net_ddr_control_errors),.ddr_owned(net_ddr_owned),
`else
    .tx_addr(net_tx_addr), .tx_ack(net_tx_ack), .rx_wr(net_rx_wr), .rx_commit(net_rx_commit),
    .rx_addr(net_rx_addr), .rx_data(net_rx_data), .rx_words(net_rx_words),
    .ddr_stopped(1'b1),.ddr_initialized(1'b0),.ddr_fault(32'd0),
    .ddr_enable(),.ddr_epoch(),.ddr_control_errors(),.ddr_owned(),
`endif
    .aborted()
);
