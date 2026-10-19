# Native49 group/word physical interleaving

TIMING PASS, NOT DEPLOYED. Frozen48 plus only previously simulation-qualified Native44
physical raw-address interleaving. The build asserts exactly three changes:
arbiter, generated INTERLEAVE_GROUPS parameter and manifest. Same successor
selection, data/ECC/pipeline, Main/server/DDR network, SDRAM/no raw cache,
50MHz/SYSPER4 and clock/timing constraints. No deferred reads.

Remote Dorado_Native49_20261019; handle69340/fitter512897 COMPLETED.
31 summary/36 detailed checks/four conditions pass, min+0.115ns, no added
cuts or illegal/unconstrained clocks. Frozen reports/RBF hash6e7866f3 included.
Earlier synthetic traffic/locality results are not hardware
speed evidence. Native48 software qualification is still in progress.
