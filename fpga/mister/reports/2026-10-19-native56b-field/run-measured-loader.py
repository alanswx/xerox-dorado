"""One first Native56b hardware qualification: fixed33-log-sample idle collection, then immediate Guest.

Start only after visual login verification and separate observer READY.
This fixes measurement-to-input cadence, not the preceding manual login gate.
"""
from pathlib import Path
import datetime,json,subprocess,time

here=Path(__file__).resolve().parent
assert 'OBSERVER_READY'in (here/'loader-observer.log').read_text()
assert not (here/'measurement-to-input.json').exists()
q=dict(scope=__doc__,utc=datetime.datetime.now(datetime.timezone.utc).isoformat(),
 start_ns=time.monotonic_ns(),key_interval_s=.2,main_pid=23403,
 limits='Prior manual login-screen gate varied; this is an observed first Native56b hardware qualification, not the final3-run automated benchmark')
with (here/'idle-measure.log').open('w')as log:
 subprocess.run(['python3',str(here/'measure-login.py'),'23403','/tmp/dorado56b-main.log',str(here)],stdout=log,stderr=subprocess.STDOUT,check=True)
q['measurement_end_ns']=time.monotonic_ns()
q['guest_start_ns']=time.monotonic_ns()
with (here/'guest-login.log').open('w')as log:
 subprocess.run(['python3','fpga/mister/reports/2026-10-18-native34/input-keyboard.py','Guest\n\n','.2'],stdout=log,stderr=subprocess.STDOUT,check=True)
q['guest_end_ns']=time.monotonic_ns()
(here/'measurement-to-input.json').write_text(json.dumps(q,indent=2)+'\n')
print('Idle measurement completed; actual Guest keyboard sequence completed',flush=True)
