"""Read-only bounded timestamping of actual Main/server log visibility.

Run on MiSTer14 after idle measurement, before Guest. No SPI, guest writes,
packet injection or memory mapping. A separate analyser must establish the
actual loader endpoints; starting/stopping this reader is not a boot gate.
"""
from pathlib import Path
import hashlib,json,os,time

assert hashlib.sha256(Path('/proc/6414/exe').read_bytes()).hexdigest()=='21e67a3f5588a875320437133df316b7e8f87061eb06b6dcaf63ecb139e29b60'
assert hashlib.sha256(Path('/proc/6413/exe').read_bytes()).hexdigest()=='c59962490772f12d061ec61aa64e6a567144d26d7802a28349a8d9a044eda87e'
output=Path('/tmp/dorado49-loader-observer.jsonl')
stop=Path('/tmp/dorado49-loader-observer.stop')
assert not output.exists() and not stop.exists()
files={name:open(path,errors='replace')for name,path in (
 ('main','/tmp/dorado49-main.log'),
 ('wire','/media/fat/games/DoradoRTL/Cedar49_20261019/server.log'))}
for f in files.values():f.seek(0,2)
last={name:time.monotonic_ns()for name in files}
start=time.monotonic_ns();deadline=start+1200_000_000_000
with output.open('x',buffering=1)as out:
 def emit(row):out.write(json.dumps(row,separators=(',',':'))+'\n')
 emit(dict(event='ready',pid=os.getpid(),ns=start,utc_ns=time.time_ns(),limit_s=1200,
  scope='Regular-file visibility bounds; stderr protocol logs, no direct wire timestamp or complete-loader qualification'))
 print('OBSERVER_READY '+json.dumps(dict(pid=os.getpid(),ns=start,output=str(output))),flush=True)
 while True:
  for name,f in files.items():
   begin=time.monotonic_ns();chunk=f.read();end=time.monotonic_ns()
   if chunk:emit(dict(event='chunk',source=name,previous_read_end_ns=last[name],read_begin_ns=begin,read_end_ns=end,text=chunk))
   last[name]=end
  now=time.monotonic_ns()
  if stop.exists()or now>=deadline:
   emit(dict(event='end',ns=now,reason='stop file'if stop.exists()else'time limit'))
   print('OBSERVER_END',flush=True)
   break
  time.sleep(.02)
