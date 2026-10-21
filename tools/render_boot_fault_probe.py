#!/usr/bin/env python3
"""Render a bounded Main-owned first-error readout, without guest writes."""
import argparse
from pathlib import Path

TEMPLATE = r'''set pagination off
set confirm off
set language c++
python
from pathlib import Path
import hashlib,json,time
base=Path(@DEPLOYMENT@)
deployment=json.loads((base/'deployment.json').read_text())
pid=gdb.selected_inferior().pid
assert pid==deployment['main_pid']==@PID@
assert hashlib.sha256(Path('/proc/%d/exe'%pid).read_bytes()).hexdigest()==deployment['main_sha256']=='21e67a3f5588a875320437133df316b7e8f87061eb06b6dcaf63ecb139e29b60'
core=Path('/media/fat/_Computer')/('DoradoRTL_'+base.name)/'Dorado.rbf'
assert hashlib.sha256(core.read_bytes()).hexdigest()==deployment['rbf_sha256']
end
tbreak input_poll
continue
python
result=dict(scope='Main-owned passive boot-error pages; live page and retained event are separate observations',main_pid=pid,rbf_sha256=deployment['rbf_sha256'],started=time.time(),pages=[],selector_restored=False)
def select(field,value):
 gdb.execute('call ((void(*)(const char*,unsigned int,int))0x9e98d)("'+field+'",'+str(value)+',0)',to_string=True)
def read(page):
 words=[]
 try:
  words.append(int(gdb.parse_and_eval('((unsigned short(*)(unsigned short))0x9ca91)(0x70)')))
  for i in range(12):words.append(int(gdb.parse_and_eval('((unsigned short(*)(unsigned short))0x701a5)(0)')))
 finally:gdb.execute('call ((void(*)())0x9c8b5)()',to_string=True)
 assert words[0]==0xd074,words
 row=dict(page=page,words=words,observation=words[8]|words[9]<<16,cpu=words[11]|words[12]<<16,status=words[10],monotonic=time.monotonic())
 result['pages'].append(row)
 return row
def decode_sources(value):
 names=['IM_LH','IM_RH','IO','RM_STK_T','MEMORY','MD']
 return dict(word=value,enables={n:bool(value&(1<<(15-i)))for i,n in enumerate(names)},raw_error_flags={n:bool(value&(1<<(9-i)))for i,n in enumerate(names)},im_error_group=bool(value&2),other_error_group=bool(value&1))
try:
 select('[8:5]',0);select('[10:9]',0)
 select('[15:11]',19);marker=read(3)
 assert marker['observation']>>16==0xd0e2,('Compiled boot-fault marker absent',marker)
 select('[15:11]',16);live=read(0)
 assert live['observation']>>16==0xd0e1,('Live source marker absent',live)
 select('[15:11]',17);cpu=read(1)
 select('[15:11]',18);sources=read(2)
 select('[15:11]',19);last=read(3)
 assert last['observation']>>16==0xd0e2
 result.update(armed=bool(last['observation']&2),capture_valid=bool(last['observation']&1),live_sources=decode_sources(live['observation']&65535),live_error_n=bool(live['status']&0x4000))
 result['live_error_group_consistent']=result['live_error_n']==(not bool(live['observation']&3))
 if marker['observation']&1 and last['observation']&1:
  first=cpu['observation']
  result.update(first_processor=first,first_CIA=(first>>12)&4095,first_TPC=first&4095,first_task=(first>>24)&15,first_sources=decode_sources(sources['observation']&65535),preceding_sources=decode_sources(sources['observation']>>16))
 else:result['retained_event_read_requires_repeat']=True
finally:
 try:
  select('[15:11]',0);select('[8:5]',0);select('[10:9]',0)
  result['selector_restored']=True
 finally:
  result['finished']=time.time()
  Path(@OUTPUT@).write_text(json.dumps(result,indent=2)+chr(10))
print('BOOT_FAULT_OBSERVATION '+json.dumps(result))
end
detach
'''


if __name__ == '__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--pid',type=int,required=True)
    p.add_argument('--deployment',type=Path,required=True)
    p.add_argument('--remote-output',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True)
    a=p.parse_args()
    assert a.pid>1 and a.deployment.is_absolute() and a.remote_output.is_absolute()
    assert not a.output.exists()
    text=TEMPLATE.replace('@PID@',str(a.pid)).replace('@DEPLOYMENT@',repr(str(a.deployment))).replace('@OUTPUT@',repr(str(a.remote_output)))
    # Catch Python syntax errors before attaching to the live Main process.
    for block in text.split('\npython\n')[1:]:
        compile(block.split('\nend\n',1)[0],str(a.output),'exec')
    a.output.write_text(text)
