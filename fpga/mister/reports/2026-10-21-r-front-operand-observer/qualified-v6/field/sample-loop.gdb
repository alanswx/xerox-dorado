set pagination off
set confirm off
set language c++
python
from pathlib import Path
import hashlib,json,time
base=Path('/media/fat/games/DoradoRTL/CedarROperands6_20261021')
deployment=json.loads((base/'deployment.json').read_text())
pid=gdb.selected_inferior().pid
assert pid==deployment['main_pid']
assert hashlib.sha256(Path('/proc/%d/exe'%pid).read_bytes()).hexdigest()==deployment['main_sha256']=='21e67a3f5588a875320437133df316b7e8f87061eb06b6dcaf63ecb139e29b60'
assert hashlib.sha256(Path('/media/fat/_Computer/DoradoRTL_CedarROperands6_20261021/Dorado.rbf').read_bytes()).hexdigest()==deployment['rbf_sha256']
assert 'user_io_status_set' in gdb.execute('info symbol 0x9e98c',to_string=True)
end
tbreak input_poll
continue
python
samples=[]
result=dict(scope='Bounded Main-owned command70 physical operand observations; pages are NOT one instruction trace',main_pid=pid,rbf_sha256=deployment['rbf_sha256'],started=time.time(),marker_verified=False,selector_restored=False)
def select(field,value):
 gdb.execute('call ((void(*)(const char*,unsigned int,int))0x9e98d)("'+field+'",'+str(value)+',0)',to_string=True)
def read(page):
 words=[]
 try:
  words.append(int(gdb.parse_and_eval('((unsigned short(*)(unsigned short))0x9ca91)(0x70)')))
  for i in range(12):words.append(int(gdb.parse_and_eval('((unsigned short(*)(unsigned short))0x701a5)(0)')))
 finally:gdb.execute('call ((void(*)())0x9c8b5)()',to_string=True)
 assert words[0]==0xd074,words
 machine=words[8]|words[9]<<16;cpu=words[11]|words[12]<<16
 sample=dict(page=page,words=words,machine=machine,cpu=cpu,task=(cpu>>24)&15,CIA=(cpu>>12)&4095,TPC=cpu&4095)
 if page==3:sample.update(eqZero_n=(machine>>13)&1,ltZero_n=(machine>>12)&1,Hphases=(machine>>6)&63,Lphases=machine&63)
 return sample
try:
 select('[10:9]',0)
 select('[15:11]',31)
 marker=read(15);result['marker']=marker
 assert marker['machine']==0xd0d0bad0,('Operand monitor marker absent; do not interpret readout',marker)
 result['marker_verified']=True
 for repetition in range(16):
  for page in range(4):
   select('[15:11]',16+page)
   sample=read(page);sample['repetition']=repetition;samples.append(sample)
   print('OPERAND_SAMPLE '+json.dumps(sample))
finally:
 try:
  select('[15:11]',0)
  select('[10:9]',1)
  result['selector_restored']=True
 finally:
  result.update(samples=samples,finished=time.time())
  Path('/tmp/dorado-r-operands6-loop-observation.json').write_text(json.dumps(result,indent=2)+'\n')
print('OPERAND_OBSERVATION '+json.dumps(result))
end
detach
