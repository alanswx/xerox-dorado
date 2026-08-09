Bringover [Cedar]<CedarChest6.1>Top>DoradoWelcome.df
Color.cm
Gargoyle



  ColorDisplay ?
  ColorDisplay on
  ColorDisplay right
  ColorDisplay 8

  Then run:

  Gargoyle.cm


ColorDisplay ?        report the mode (expect: on, 8 bpp, left, 640x480)
  ColorDisplay left     put the colour screen left of the b/w one
  ColorDisplay 8        8 bits/pixel; try 4, 2, 1, 24 as well
  ColorDisplay dither   dithered rather than flat 8bpp
