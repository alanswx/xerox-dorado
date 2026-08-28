# Xerox Dorado for MiSTer

The eleven-board machine, generated from PARC's own Sil wire lists, inside the
MiSTer framework.

    sh fpga/mister/build.sh [TEMPLATE_DIR] [OUT_DIR]
    cd <OUT_DIR> && quartus_sh --flow compile Dorado

The core is ASSEMBLED rather than checked in whole: `sys/` belongs to the MiSTer
framework and the pin assignments belong to the DE10-Nano, so both come from the
template. Only what is ours lives here -- `Dorado.sv` and `build.sh`.

## RESULT (2026-08-27, Quartus Prime Lite 17.0.2, 5CSEBA6U23I7)

    Logic utilization (in ALMs) : 33,818 / 41,910  ( 81 % )
    Total registers             : 62,238
    Total RAM Blocks            : 181 / 553        ( 33 % )
    Total block memory bits     : 902,786 / 5,662,720 ( 16 % )
    Total DSP Blocks            : 33 / 112         ( 29 % )
    Timing                      : all setup slacks POSITIVE
    Output                      : Dorado.rbf, 3.6 MB

So a loadable core exists and it meets timing. The machine alone measured
25,644 ALMs standalone (`fpga/quartus`); the framework -- scaler, HDMI, HPS,
audio -- accounts for the rest, and for all 33 DSP blocks.

## WHAT IT DOES AND DOES NOT DO

Video is wired: `AltoTTLVideo` is the monochrome pixel stream, expanded to
white/black, with `AltoHSync` / `AltoVSync'` (active low) and
`~(HBlank|VBlank)` as DE. The Dorado's mono display is genuinely 1 bpp; DispM,
the colour board, is a SEPARATE SCREEN at its own raster and is not in this
configuration.

NOT yet wired, and deliberately absent rather than misleadingly stubbed:

- **Disk.** The Trident interface is enumerated -- 46 signals, and the
  controller obeys real commands -- but there is no medium. `hps_io`'s virtual
  drive is what supplies one, from a real pack image.
- **Keyboard.** The 61-key matrix is audited against three independent sources
  and `KeyBits` sits at 177033-177037, active low. PS/2 from `hps_io` maps onto
  it; this is a mapping job, not a discovery job.
- **Main storage.** The MSA infers block RAM today. A real Dorado has up to 4M
  words, which belongs in SDRAM -- and the DE10-Nano's SDRAM is 16 bits wide,
  exactly the Dorado's word.

## THE CLOCK, WHICH IS THE INTERESTING CONSTRAINT

The RTL recovers each distributed ECL clock net by OVERSAMPLING it SYSPER times
per microinstruction, so real time needs `sys_clk = SYSPER x 16.67 MHz` (the
60 ns cycle). Measured Fmax for the machine is 48.99 MHz:

    SYSPER=16 -> 266.7 MHz  no   0.18x      SYSPER=4 -> 66.7 MHz  no   0.73x
    SYSPER=8  -> 133.3 MHz  no   0.37x      SYSPER=2 -> 33.3 MHz  YES  1.47x

This core runs the template PLL's 20 MHz, so at SYSPER=16 it is 1.25 MHz of
microinstructions -- 0.075x a real Dorado. Slow, but running, and with enormous
timing margin. Regenerating the PLL for 33.3 MHz once SYSPER=2 is validated
makes the same core real-time.

Lowering SYSPER stresses cell fidelity -- one bug "survived at 16x only because
the pulse happened to sit inside one stable address" -- which the 50 Verilator
gates can hunt without any FPGA.
