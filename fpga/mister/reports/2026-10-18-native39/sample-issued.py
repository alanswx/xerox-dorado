"""Run on MiSTer after selecting instruction view 1 and an idle workload.

Usage: python3 sample-issued.py /path/to/main.log
Main command70 supplies a coherent issue/host-edge pair. Discard two
post-selection observations. Each 10-second window is shorter than the
32-bit host-counter wrap interval at 50MHz. These are issues, not drained
pipeline retirements. The observer does not touch SPI or guest state.
"""
from pathlib import Path
import datetime
import json
import re
import sys
import time

pattern = re.compile(r'machine=([0-9a-f]{8}) state=([0-9a-f]{4}) cpu=([0-9a-f]{8})')
f = Path(sys.argv[1]).open()
f.seek(0, 2)
samples = []
deadline = time.monotonic() + 55
while len(samples) < 33 and time.monotonic() < deadline:
    line = f.readline()
    if not line:
        time.sleep(.01)
        continue
    match = pattern.search(line)
    if match:
        samples.append(dict(monotonic=time.monotonic(),
                            issued=int(match[1], 16),
                            status=int(match[2], 16),
                            host_edges=int(match[3], 16), line=line.strip()))
assert len(samples) == 33, len(samples)
samples = samples[2:]
assert all(not (x['status'] & 16) for x in samples), 'Pack fault'
windows = []
for i in range(3):
    a, b = samples[10*i], samples[10*i+10]
    host = (b['host_edges'] - a['host_edges']) & 0xffffffff
    issued = (b['issued'] - a['issued']) & 0xffffffff
    elapsed = b['monotonic'] - a['monotonic']
    assert 0 < elapsed < 60 and 0 < host < 0xffffffff
    assert 0 <= issued <= host // 8 + 2, (issued, host)
    windows.append(dict(delta_host_edges=host, delta_issued=issued,
                        elapsed_observer_s=elapsed,
                        implied_host_mhz=host/elapsed/1e6,
                        issued_per_second=50e6*issued/host,
                        ordinary_nominal_rate_fraction=(50e6*issued/host)/(1e9/60)))
print(json.dumps(dict(utc=datetime.datetime.now(datetime.timezone.utc).isoformat(),
                     units='StartCycle issues qualified Run/Jam, excluding RepeatCur; NOT pipeline-drained retirements',
                     selected_instruction_view=1, host_clock_hz=50000000,
                     samples=samples, windows=windows), indent=2))
