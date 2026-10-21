from pathlib import Path
import json,os,subprocess,time
root=Path(__file__).resolve().parent
path=root/'boot-run-state.json'
assert not path.exists(),'Observe existing boot; do not repeat input'
s=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),actions=[])
def save():path.write_text(json.dumps(s,indent=2)+'\n')
save()
try:
 for name in ['cool-boot','c-until-request']:
  with(root/(name+'.log')).open('x')as log:
   p=subprocess.Popen(['python3','-u',str(root/(name+'.py'))],stdout=log,stderr=subprocess.STDOUT)
   a=dict(name=name,pid=p.pid,started=time.time());s['actions'].append(a);save()
   code=p.wait();a.update(returncode=code,finished=time.time());save();assert code==0,(name,code)
 s.update(status='TERMINAL_ORIGINAL_BFN113_C_RELEASED',finished=time.time());save()
 print(s['status'],flush=True)
except BaseException:
 s.update(status='TERMINAL_FAILED',finished=time.time());save();raise
