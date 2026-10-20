# Serial65Ready: legal capacity, timing rejected

The readiness-only integration fits at 41,414 / 41,910 ALMs, leaving 496. The complete
driver is terminal; builder 2458594 / fitter 2461232 and all project processes are
absent. No assembly, RBF/SOF, reload or speed result. All 659 prepared inputs are
unchanged after synthesis, fit and timing.

Worst setup improves from Serial64's -0.239 ns to -0.038 ns, still REJECTED. All
reported hold conditions pass, minimum +0.106 ns. SDRAM input/output setup/hold
pass throughout. Actual clocks and inherited framework coverage match Native59:
zero illegal/unconstrained clocks; three input/fifty output exceptions remain.
This is not a claim that every physical framework IO is timed.

All 304 board input/output setup/hold reports across nineteen boards and four
corners are independently checked. Only MemC input setup at slow 100C fails:
four of twelve reported paths, worst -0.038 ns, from network_step into u_c22's
MB7071H snapshot RAM enable. The original source has 273 loads and a 10 ns budget.

Raw inputs, reports and process receipts are under `terminal-fit`; actual-QSF
XML and state accounting under `classification`; pinned passing Native59 timing
under `baseline-native59-timing`. Review JSON records capacity PASS and timing/
crossing REJECTION separately. Six source/four capacity controls reject; timing
reader controls use known passing Native59, and crossing controls use an actual
passing fast-corner report. A failing design is not a positive control baseline.

All 1,653 manifest members pass lossless readback. Extracted source/capacity/
timing/crossing readers and their controls replay, using the prior immutable
readiness receipt's tool dependencies. No working HDL is needed. No complete
machine snapshot or real service-barrier qualification is claimed.
