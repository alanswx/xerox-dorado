from pathlib import Path
import subprocess,json,re,shlex,time
out=Path(__file__).resolve().parent
views={2:('ddr_only_pause_cycles','host_cycles'),3:('pack_only_pause_cycles','host_cycles'),4:('overlap_pause_cycles','host_cycles'),5:('ddr_read_commands','ddr_write_commands'),6:('backing_read_hits','raw_read_requests'),7:('ddr_issue_busy_cycles','ddr_return_pending_cycles'),12:('raw_read_requests','raw_write_requests'),1:('enabled_cycles','host_cycles')}
remote=r"""import time,re,json
f=open('/media/fat/games/DoradoRTL/Cedar35_20261018/main.log');f.seek(0,2)
p=re.compile(r'machine=([0-9a-f]{8}) state=([0-9a-f]{4}) cpu=([0-9a-f]{8})')
a=[];end=time.monotonic()+30
while len(a)<13 and time.monotonic()<end:
 l=f.readline()
 if not l:time.sleep(.01);continue
 m=p.search(l)
 if m:a.append(dict(monotonic=time.monotonic(),machine=int(m[1],16),cpu=int(m[3],16),status=int(m[2],16),line=l.strip()))
assert len(a)==13,len(a)
print(json.dumps(a[2:]))
"""
for view,fields in views.items():
 gdb=f"""set pagination off
set confirm off
python
assert 'user_io_status_set' in gdb.execute('info symbol 0x9aa74',to_string=True)
end
tbreak input_poll
continue
call ((void (*)(const char *, unsigned int, int))0x9aa75)("[8:5]",{view},0)
detach
"""
 (out/'view.gdb').write_text(gdb)
 subprocess.run(['scp',str(out/'view.gdb'),'mister14:/tmp/dorado35-view.gdb'],check=True,timeout=20)
 with (out/f'view-{view}-control.log').open('w') as log:
  subprocess.run(['ssh','mister14','gdb -q -batch /tmp/dorado-main-1fb2.elf -p 5785 -x /tmp/dorado35-view.gdb'],stdout=log,stderr=subprocess.STDOUT,check=True,timeout=20)
 a=json.loads(subprocess.check_output(['ssh','mister14','python3 -c '+shlex.quote(remote)],timeout=40))
 assert all(not(x['status']&16) for x in a),'pack fault'
 d={fields[0]:(a[-1]['machine']-a[0]['machine'])&0xffffffff,fields[1]:(a[-1]['cpu']-a[0]['cpu'])&0xffffffff}
 if fields[1]=='host_cycles':assert 0<=d[fields[0]]<=d[fields[1]]
 r=dict(view=view,workload='Original firmware cold boot; Cedar login waiting, no guest typing',samples=a,delta=d,elapsed_observer_s=a[-1]['monotonic']-a[0]['monotonic'],units='Fabric cycles or transaction counts as named; NOT instructions',control='Normal Main status setter at input_poll while idle login; first two post-detach observations discarded; no SPI owner added')
 if fields[1]=='host_cycles':r['fraction']=d[fields[0]]/d[fields[1]]
 (out/f'view-{view}.json').write_text(json.dumps(r,indent=2)+'\n')
 print(view,fields,d,r.get('fraction'),flush=True)
