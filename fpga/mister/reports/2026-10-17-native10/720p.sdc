# Post-fit qualification for the fixed MiSTer mode 0 (720p60).
# Main video.cpp: pixel clock 74.25 MHz, vsync_adjust=0.
# Preserve all core/control/setup/hold checks; replace only reconfigurable HDMI clock.
# Keep the derived PLL VCO clock and physical source latency. Divide its
# output by six instead of three: 74.268 MHz, conservatively above 74.25.
set hdmi_clock {pll_hdmi|pll_hdmi_inst|altera_pll_i|cyclonev_pll|counter[0].output_counter|divclk}
create_generated_clock -name $hdmi_clock -source [get_pins {pll_hdmi|pll_hdmi_inst|altera_pll_i|cyclonev_pll|counter[0].output_counter|vco0ph[0]}] -divide_by 6 [get_pins $hdmi_clock]
derive_clock_uncertainty
