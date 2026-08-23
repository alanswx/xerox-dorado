derive_pll_clocks
derive_clock_uncertainty

# core specific constraints
#
# `derive_pll_clocks` picks the frequency up from the PLL's own parameters,
# so the number that matters is in rtl/pll: ask for 7 MHz and Quartus meets
# it trivially and stops optimising, whatever the design could do. The PLL
# now asks for 50 MHz -- at 2 sys_clk per microinstruction that is 25 MHz of
# microinstructions, 1.5x the real Dorado's 16.67 MHz.
