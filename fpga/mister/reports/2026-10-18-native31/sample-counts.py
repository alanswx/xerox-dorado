from pathlib import Path
import json,re,sys,time
samples=int(sys.argv[1]) if len(sys.argv)>1 else 21
f=Path('/media/fat/games/DoradoRTL/Cedar31_20261018/main.log').open()
f.seek(0,2)
pat=re.compile(r'machine=([0-9a-f]{8}) state=([0-9a-f]{4}) cpu=([0-9a-f]{8})')
a=[];end=time.monotonic()+60
while len(a)<samples and time.monotonic()<end:
 l=f.readline()
 if not l:time.sleep(.01);continue
 m=pat.search(l)
 if m:a.append(dict(observed_monotonic=time.monotonic(),enabled=int(m[1],16),host=int(m[3],16),status=int(m[2],16),line=l.strip()))
assert len(a)==samples,len(a)
dh=(a[-1]['host']-a[0]['host'])&0xffffffff
dm=(a[-1]['enabled']-a[0]['enabled'])&0xffffffff
dt=a[-1]['observed_monotonic']-a[0]['observed_monotonic']
assert 0<dt<60 and 0<dh<0xffffffff and 0<=dm<=dh,(dt,dh,dm)
print(json.dumps(dict(units='fabric rising edges; NOT retired instructions',samples=a,elapsed_observer_s=dt,delta_host=dh,delta_enabled=dm,enabled_fraction=dm/dh,implied_host_mhz=dh/dt/1e6,instruction_ceiling_per_s=50e6*dm/dh/8),indent=2))
