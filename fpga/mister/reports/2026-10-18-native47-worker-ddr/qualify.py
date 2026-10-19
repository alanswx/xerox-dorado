from pathlib import Path
from collections import Counter
import json,re
b=Path(__file__).resolve().parent
q=json.loads((b/'observer.json').read_text());events=q['events']
begin=next(x['ns']for x in events if x['phase']=='begin');end=next(x['ns']for x in events if x['phase']=='end')
last=events[-1];assert last['phase']=='end'and last['result']==last['error']==0 and last['duration_ns']>=4100000000
# This observer stamped rows before its reads. The NEXT recorded iteration
# starts after both reads have completed, so it is a conservative upper bound
# even if a regular-file read blocked or captured data appended concurrently.
times=sorted({r['observed_ns']for kind in ('main','wire')for r in q[kind]}|{q['finished_ns']})
upper={a:z for a,z in zip(times,times[1:])}
def lines(kind):
 pending='';low=high=0;result=[]
 for r in q[kind]:
  if not pending:low=r['interval_begin_ns']
  high=upper[r['observed_ns']];pending+=r['text']
  while '\n'in pending:
   text,pending=pending.split('\n',1)
   if begin<low and high<end:result.append(dict(earliest_ns=low,latest_ns=high,text=text))
   low=r['interval_begin_ns'] if pending else 0
 return result
main=lines('main');wire=lines('wire')
p=re.compile(r'good=(\d+) crc_bad=(\d+) dropped=(\d+) rx_bad=(\d+) aborted=(\d+) machine=([0-9a-f]+) state=([0-9a-f]+) cpu=([0-9a-f]+)')
samples=[]
for row in main:
 m=p.search(row['text'])
 if not m:continue
 good,crc,dropped,rx,aborted=map(int,m.groups()[:5]);issued,status,host=map(lambda x:int(x,16),m.groups()[5:])
 assert crc==rx==aborted==0 and not(status&16),row
 samples.append(dict(**row,good=good,dropped=dropped,issued=issued,host=host))
assert len(samples)>=3
issue_delta=(samples[-1]['issued']-samples[0]['issued'])&0xffffffff
host_delta=(samples[-1]['host']-samples[0]['host'])&0xffffffff
good_delta=(samples[-1]['good']-samples[0]['good'])&65535
assert issue_delta>1000000 and good_delta>0
assert samples[-1]['dropped']==samples[0]['dropped']
rows=[json.loads(x)for x in (b/'disk-prefix.jsonl').read_text().splitlines()]
assert rows[-1]['ns']>end+1000000000,'no post-window trace coverage'
assert not any('error'in r['event'] or r['event']=='trace_dropped'for r in rows)
inside=[r for r in rows if begin<r['ns']<end];counts=Counter(r['event']for r in inside)
assert counts['read_ack']>0 and counts['write_ack']>0
assert counts['pwrite_begin']==counts['fsync_begin']==0,'probe worker did not exclude real disk jobs'
gaps=[r for r in rows if r['event']=='poll_gap'and r['ns']>begin and r['ns']-r['duration_ns']<end]
assert not gaps,gaps
# Prove a complete, identifiable server-to-guest metadata exchange, then
# the guest naming its exact end back in an ACK. This is stronger than
# counting host enqueues alone and requires no guest/server-state injection.
queued={};matched=[]
for row in wire:
 m=re.match(r'FTP_QUEUE type=0o21 id=([0-9a-f]+)',row['text'])
 if m:queued[m[1]]=row
 m=re.match(r'FTP_ACK ack=([0-9a-f]+)',row['text'])
 if m and m[1]in queued:matched.append(dict(queued=queued[m[1]],ack=row))
assert matched,'no in-window guest ACK of in-window server response'
assert any('FTP_RETRIEVE 'in r['text']for r in wire)
metadata=[]
for row in wire:
 m=re.match(r'FTP_QUEUE type=0o20 id=([0-9a-f]+) bytes=168 ',row['text'])
 if not m:continue
 start=int(m[1],16);mark=(start+168)&0xffffffff;endpoint=(mark+1)&0xffffffff
 closing=next((r for r in wire if re.match(r'FTP_QUEUE type=0o23 id=%08x bytes=1 '%mark,r['text']) and r['earliest_ns']>=row['earliest_ns']),None)
 pair=next((r for r in matched if ('id=%08x'%endpoint)in r['queued']['text']),None)
 if closing and pair:metadata.append(dict(data=row,eoc=closing,probe_ack=pair))
assert metadata,'no guest ACK of exact168-byte metadata+EOC endpoint'
assert any('Watch>Watch.bcd'in r['text'] for r in wire),'wrong request workload'
result=dict(status='PASS for concurrent physical DDR packet, disk ACK and instruction progress during delayed actual Main worker flush work',
 begin_ns=begin,end_ns=end,duration_ns=last['duration_ns'],worker='One real Main worker callback sleeps4200ms then actual fsync; not an injected4.2s kernel syscall',
 disk_events=dict(counts),overlapping_poll_gaps=gaps,trace_errors_or_drops=0,
 main_samples=samples,delta_issued=issue_delta,delta_host_edges=host_delta,delta_good_inner_samples=good_delta,delta_dropped_inner_samples=0,
 wire_lines_fully_inside=len(wire),complete_tx_frames=sum(r['text'].startswith('TX n=')for r in wire),matched_response_guest_ack=matched,metadata_response_end_ack=metadata,
 window_wire=wire,capture=json.loads((b/'capture-timing.json').read_text()),
 qualifications=['Source hash/worker ABI checked; Main sole SPI owner','Observation intervals use next recorded iteration as conservative read-completion upper bound','Real Watch.bcd STP retrieve+168-byte metadata and matching EOC ACK inside window; not a claimed1478-byte file-data window','No keyboard command or mouse test performed during this loader window','DSO must remain loaded until Main exits; use a fresh Main for later clean performance comparisons','No cold-persistence/two-hour/full-snapshot claim'])
assert not result['capture']['ended']
(b/'qualification.json').write_text(json.dumps(result,indent=2)+'\n')
print(json.dumps({k:v for k,v in result.items()if k not in ('main_samples','window_wire','matched_response_guest_ack')},indent=2))
