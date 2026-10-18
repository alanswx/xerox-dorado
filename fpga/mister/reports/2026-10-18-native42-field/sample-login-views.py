from pathlib import Path
import subprocess,json,re,shlex,time,sys
out=Path(__file__).resolve().parent
pid=int(sys.argv[1])
logfile=sys.argv[2]
verify="from pathlib import Path; import hashlib; assert hashlib.sha256(Path('/proc/%d/exe').read_bytes()).hexdigest()=='40e14b6b97d62435c6e1ecd5c78bf9959c74b816a8e9452b1e64c28179ca8188'"%pid
subprocess.run(['ssh','mister14','python3 -c '+shlex.quote(verify)],check=True)
views=[('instruction',2,('held_cycles','start_cycles')),('instruction',3,('board_clocks','issued')),('memory',1,('enabled_edges','host_edges')),('memory',2,('storage_only_pause_edges','host_edges')),('memory',3,('pack_only_pause_edges','host_edges')),('memory',4,('overlap_pause_edges','host_edges')),('memory',5,('physical_reads','physical_writes')),('memory',6,('backing_cache_hits','raw_read_requests')),('memory',7,('issue_busy_edges','return_pending_edges')),('memory',12,('raw_read_requests','raw_write_requests'))]
remote='''import time,re,json
f=open(LOGFILE);f.seek(0,2)
p=re.compile(r'machine=([0-9a-f]{8}) state=([0-9a-f]{4}) cpu=([0-9a-f]{8})')
a=[];end=time.monotonic()+30
while len(a)<13 and time.monotonic()<end:
 l=f.readline()
 if not l:time.sleep(.01);continue
 m=p.search(l)
 if m:a.append(dict(monotonic=time.monotonic(),machine=int(m[1],16),cpu=int(m[3],16),status=int(m[2],16),line=l.strip()))
assert len(a)==13,len(a)
print(json.dumps(a[2:]))
'''.replace('LOGFILE',repr(logfile))
for kind,view,fields in views:
 control='''set pagination off
set confirm off
python
assert 'user_io_status_set' in gdb.execute('info symbol 0x9d71c',to_string=True)
end
tbreak input_poll
continue
'''
 for key,value in [('[10:9]',view if kind=='instruction' else 0),('[8:5]',view if kind=='memory' else 0)]:
  control+='call ((void (*)(const char *, unsigned int, int))0x9d71d)("%s",%d,0)\n'%(key,value)
 control+='detach\n'
 p=out/'select-view.gdb';p.write_text(control)
 subprocess.run(['scp',str(p),'mister14:/tmp/dorado-login-select-view.gdb'],check=True,timeout=20)
 with (out/('%s-%d-control.log'%(kind,view))).open('w')as f:
  subprocess.run(['ssh','mister14','gdb -q -batch /tmp/dorado-main-40e1.elf -p %d -x /tmp/dorado-login-select-view.gdb'%pid],stdout=f,stderr=subprocess.STDOUT,check=True,timeout=20)
 a=json.loads(subprocess.check_output(['ssh','mister14','python3 -c '+shlex.quote(remote)],timeout=40))
 assert all(not(x['status']&16)for x in a),'pack fault'
 d={fields[0]:(a[-1]['machine']-a[0]['machine'])&0xffffffff,fields[1]:(a[-1]['cpu']-a[0]['cpu'])&0xffffffff}
 dt=a[-1]['monotonic']-a[0]['monotonic']
 q=dict(kind=kind,view=view,workload='Matched DDR42 idle Cedar login; no Guest typing; fixed original seed and served tree, Main40e1 disk-cache+trace, quiet+BSP-history daemone237dffc, Pup time1792324800;50MHz/SYSPER4',samples=a,delta=d,elapsed_observer_s=dt,control='Main status setter at input_poll, first two observations discarded; separate view windows, no all-counter atomic correlation')
 if fields[1]=='host_edges':q['fraction']=d[fields[0]]/d[fields[1]]
 elif kind=='instruction':q['ratio']=d[fields[0]]/d[fields[1]]if d[fields[1]]else None
 else:q['observer_rates_per_s']={k:v/dt for k,v in d.items()}
 (out/('idle-%s-%d.json'%(kind,view))).write_text(json.dumps(q,indent=2)+'\n')
 print(kind,view,d,q.get('fraction',q.get('ratio')),flush=True)
