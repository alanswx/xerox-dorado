from pathlib import Path
from collections import Counter
import hashlib,json,re
b=Path(__file__).resolve().parent
q=json.loads((b/'observer.json').read_text());events=q['events']
begin=next(r['ns']for r in events if r['phase']=='begin')
end=next(r['ns']for r in events if r['phase']=='end')
assert events[-1]['phase']=='end'and events[-1]['result']==events[-1]['error']==0
assert events[-1]['duration_ns']>=4100000000
pending='';low=0;samples=[]
pattern=re.compile(r'good=(\d+) crc_bad=(\d+) dropped=(\d+) rx_bad=(\d+) aborted=(\d+) machine=([0-9a-f]+) state=([0-9a-f]+) cpu=([0-9a-f]+)')
for row in q['main']:
 if not pending:low=row['interval_begin_ns']
 pending+=row['text']
 while'\n'in pending:
  text,pending=pending.split('\n',1);m=pattern.search(text)
  if m and begin<low and row['observed_ns']<end:
   good,crc,dropped,rx,aborted=map(int,m.groups()[:5]);issued,status,host=map(lambda x:int(x,16),m.groups()[5:])
   assert crc==rx==aborted==0 and not(status&16),text
   samples.append(dict(earliest_ns=low,latest_ns=row['observed_ns'],text=text,issued=issued,host=host,good=good,dropped=dropped))
  low=row['interval_begin_ns']if pending else 0
assert len(samples)>=3,samples
issues=(samples[-1]['issued']-samples[0]['issued'])&0xffffffff
assert issues>1000000
rows=[json.loads(x)for x in(b/'disk-prefix.jsonl').read_text().splitlines()]
assert rows[-1]['ns']>end+1000000000,'no post-window disk trace coverage'
assert not any('error'in r['event']or r['event']=='trace_dropped'for r in rows)
gaps=[r for r in rows if r['event']=='poll_gap'and r['ns']>begin and r['ns']-r['duration_ns']<end]
assert not gaps,gaps
counts=Counter(r['event']for r in rows if begin<r['ns']<end)
assert counts['read_ack']>0 and counts['write_ack']>0,'no concurrent read/write disk service'
assert counts['pwrite_begin']==counts['fsync_begin']==0,'actual worker did not exclude normal file jobs'
captures=[]
for row in json.loads((b/'capture-timing.json').read_text()):
 p=b/f"during-{row['number']}.png"
 captures.append(dict(**row,inside=begin<row['ns']<end and not row['ended'],sha256=hashlib.sha256(p.read_bytes()).hexdigest()))
assert any(r['inside']for r in captures),'no capture completed inside actual delay interval'
result=dict(status='PASS for real-worker delay, Main poll continuity and instruction progress; VISUAL INPUT QUALIFICATION PENDING',
 begin_ns=begin,end_ns=end,duration_ns=events[-1]['duration_ns'],delta_inner_issues=issues,main_samples=samples,
 overlapping_poll_gaps=gaps,disk_events=dict(counts),captures=captures,
 limitations=['Echo was fully typed before the interval; only Return and mouse move injected during it',
 'Inspect an inside-window image for a separate new worker48 output line AND following prompt and changed pointer before claiming input success',
 'Delayed worker flush work then actual fsync, not a 4.2s kernel syscall',
 'No claim of disk ACKs or network progress unless recorded in this specific interval',
 'DSO remains resident until Main exits; future clean performance comparison needs a fresh Main'])
review=b/'visual-review.json'
if review.exists():
 visual=json.loads(review.read_text())
 assert hashlib.sha256((b/'pretyped.png').read_bytes()).hexdigest()==visual['pretyped_sha256']
 selected=next(r for r in captures if r['number']==visual['capture_number'])
 assert selected['inside']and selected['sha256']==visual['capture_sha256'],'reviewed capture is late or mismatched'
 assert visual['separate_command_output_and_new_prompt']and visual['changed_pointer_observed']
 result['visual_review']=visual
 result['status']='PASS: actual Main delayed worker, completed pretyped Echo via in-window Return, mouse motion, disk ACKs and instruction/poll progress'
(b/'qualification.json').write_text(json.dumps(result,indent=2)+'\n')
print(json.dumps(result,indent=2))
