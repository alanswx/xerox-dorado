set pagination off
set confirm off
set language c++
python
from pathlib import Path
import hashlib,json,time
assert gdb.selected_inferior().pid==3401
assert hashlib.sha256(Path('/proc/3401/exe').read_bytes()).hexdigest()=='21e67a3f5588a875320437133df316b7e8f87061eb06b6dcaf63ecb139e29b60'
assert 'user_io_status_set' in gdb.execute('info symbol 0x9e98c',to_string=True)
end
tbreak input_poll
continue
python
observed=[]
def set_boot(value):
 gdb.execute('call ((void(*)(const char*,unsigned int,int))0x9e98d)("[3]",'+str(value)+',0)',to_string=True)
def sample():
 words=[]
 try:
  words.append(int(gdb.parse_and_eval('((unsigned short(*)(unsigned short))0x9ca91)(0x70)')))
  for i in range(12):words.append(int(gdb.parse_and_eval('((unsigned short(*)(unsigned short))0x701a5)(0)')))
 finally:gdb.execute('call ((void(*)())0x9c8b5)()',to_string=True)
 assert words[0]==0xd074,words
 return words
try:
 set_boot(0);time.sleep(.3)
 for pulse in range(3):
  set_boot(1);time.sleep(1)
  down=sample();assert down[10]&32==0,('BaseBoard boot pin did not assert',down)
  set_boot(0);time.sleep(1)
  up=sample();assert up[10]&32,('BaseBoard boot pin did not release',up)
  observed.append(dict(pulse=pulse+1,down=down,up=up))
finally:
 set_boot(0)
 Path('/tmp/dorado-r-operands6-explicit-boot.json').write_text(json.dumps(dict(scope='Normal core OSD boot bit3, observed at actual BaseBoard pin; no guest state writes',pulses=observed,restored=True),indent=2)+'\n')
print('EXPLICIT_BASEBOARD_BOOT '+json.dumps(observed))
end
detach
