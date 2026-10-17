# Additional analysis only. Do not edit the fitted project's SDC or RBF.
project_open Dorado
foreach model {slow fast} {
    create_timing_netlist -model $model
    read_sdc
    set hdmi_clock {pll_hdmi|pll_hdmi_inst|altera_pll_i|cyclonev_pll|counter[0].output_counter|divclk}
    remove_clock [get_clocks $hdmi_clock]
    create_generated_clock -name $hdmi_clock -source [get_pins {pll_hdmi|pll_hdmi_inst|altera_pll_i|cyclonev_pll|counter[0].output_counter|vco0ph[0]}] -divide_by 3 [get_pins $hdmi_clock]
    derive_clock_uncertainty
    update_timing_netlist
    report_clocks -file hdmi148-${model}-clocks.rpt
    report_ucp -file hdmi148-${model}-unconstrained.rpt
    foreach check {setup hold recovery removal} {
        report_timing -$check -npaths 10 -detail full_path -file hdmi148-${model}-${check}.rpt
    }
    delete_timing_netlist
}
project_close
