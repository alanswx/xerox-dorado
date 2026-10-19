from pathlib import Path
import json,shutil,subprocess,tempfile
b=Path(__file__).resolve().parent
q=json.loads((b/'qualification.json').read_text());cases=[]
expected={'late_capture':'no capture completed inside actual delay interval',
 'wrong_visual_capture':'reviewed capture is late or mismatched',
 'poll_gap':"'event': 'poll_gap'",'crc_fault':'crc_bad=1',
 'missing_disk_reads':'no concurrent read/write disk service'}
for name in ('baseline',*expected):
 with tempfile.TemporaryDirectory(prefix='dorado-worker-input-check-',dir='/tmp')as tmp:
  d=Path(tmp)
  for filename in ('qualify.py','observer.json','capture-timing.json','visual-review.json','disk-prefix.jsonl','pretyped.png','during-1.png','during-2.png'):shutil.copyfile(b/filename,d/filename)
  if name=='late_capture':
   p=d/'capture-timing.json';a=json.loads(p.read_text());a[0]['ns']=q['end_ns']+1;p.write_text(json.dumps(a))
  elif name=='wrong_visual_capture':
   p=d/'visual-review.json';a=json.loads(p.read_text());a['capture_number']=2;p.write_text(json.dumps(a))
  elif name=='poll_gap':
   p=d/'disk-prefix.jsonl';a=[json.loads(x)for x in p.read_text().splitlines()]
   a.append(dict(ns=q['begin_ns']+100000000,event='poll_gap',duration_ns=30000000))
   a.sort(key=lambda r:r['ns']);p.write_text(''.join(json.dumps(x)+'\n'for x in a))
  elif name=='crc_fault':
   p=d/'observer.json';a=json.loads(p.read_text())
   for r in a['main']:r['text']=r['text'].replace('crc_bad=0','crc_bad=1')
   p.write_text(json.dumps(a))
  elif name=='missing_disk_reads':
   p=d/'disk-prefix.jsonl';a=[json.loads(x)for x in p.read_text().splitlines()]
   p.write_text(''.join(json.dumps(x)+'\n'for x in a if not(q['begin_ns']<x['ns']<q['end_ns']and x['event']=='read_ack')))
  r=subprocess.run(['python3',str(d/'qualify.py')],capture_output=True,text=True)
  if name=='baseline':assert r.returncode==0,r.stderr
  else:
   assert r.returncode!=0 and expected[name]in r.stderr,(name,r.stderr)
   cases.append(dict(case=name,rejected=True,diagnostic=r.stderr.splitlines()[-1]))
(b/'failure-controls.json').write_text(json.dumps(cases,indent=2)+'\n')
print('PASS: actual-case baseline and five controls rejected for the intended reason')
