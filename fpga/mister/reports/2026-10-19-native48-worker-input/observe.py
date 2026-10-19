from pathlib import Path
import json,os,time

events=Path('/tmp/dorado48-input-worker.jsonl')
limit=time.monotonic()+6
while not events.exists()or'waiting'not in events.read_text():
 assert time.monotonic()<limit,'actual worker did not start';time.sleep(.01)
files={name:open(path)for name,path in (
 ('main','/tmp/dorado48-main.log'),
 ('wire','/media/fat/games/DoradoRTL/Cedar48_20261019/server.log'))}
for f in files.values():f.seek(0,2)
fresh=0;limit=time.monotonic()+4
while fresh<2:
 fresh+=files['main'].read().count('Dorado net:')
 assert time.monotonic()<limit,'Main heartbeat did not resume after setup'
 time.sleep(.01)
files['wire'].read()
last={name:time.monotonic_ns()for name in files}
Path('/tmp/dorado48-input-worker.arm').open('x').close()
limit=time.monotonic()+2
while True:
 rows=[json.loads(x)for x in events.read_text().splitlines()]
 begin=next((x for x in rows if x['phase']=='begin'),None)
 if begin:break
 assert time.monotonic()<limit,'actual delayed interval absent';time.sleep(.001)
print('BEGIN '+json.dumps(begin),flush=True)
observations={name:[]for name in files};end=None;limit=time.monotonic()+10
while time.monotonic()<limit:
 for name,f in files.items():
  text=f.read();now=time.monotonic_ns()
  if text:observations[name].append(dict(interval_begin_ns=last[name],observed_ns=now,text=text))
  last[name]=now
 rows=[json.loads(x)for x in events.read_text().splitlines()]
 end=next((x for x in rows if x['phase']=='end'),None)
 if end and time.monotonic_ns()>end['ns']+2000000000:break
 time.sleep(.01)
assert end and end['result']==end['error']==0,'real fsync did not succeed'
result=dict(events=rows,**observations,finished_ns=time.monotonic_ns())
Path('/tmp/dorado48-input-worker-observer.json').write_text(json.dumps(result,indent=2)+'\n')
print('COMPLETE '+json.dumps(dict(events=rows,main_chunks=len(observations['main']))),flush=True)
