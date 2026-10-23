from pathlib import Path
import json,shlex,subprocess,time
out=Path(__file__).resolve().parent
cli='/Applications/MiSTerKVMS.app/Contents/MacOS/misterkvms-cli'
r={'scope':'Temporary normal Main modeline command; restore original in finally; no core reload or persistent INI edit','actions':[]}
def run(args):
 p=subprocess.run(args,capture_output=True,text=True,timeout=20)
 r['actions'].append(dict(args=args,code=p.returncode,stdout=p.stdout,stderr=p.stderr,time=time.time()))
 (out/'mode-actions.json').write_text(json.dumps(r,indent=2)+'\n')
 p.check_returncode();return p.stdout
def mode(value):
 script="from pathlib import Path;import hashlib;assert hashlib.sha256(Path('/proc/13244/exe').read_bytes()).hexdigest()=='21e67a3f5588a875320437133df316b7e8f87061eb06b6dcaf63ecb139e29b60';Path('/dev/MiSTer_cmd').write_text("+repr('video_mode '+value+'\n')+")"
 run(['ssh','-o','ConnectTimeout=8','mister14','python3 -c '+shlex.quote(script)])
def shot(name):run([cli,'--mister','10.0.2.14','--stream','mkvm://10.0.2.128/Mister%2014','screenshot',str(out/name)])
try:
 mode('1280,110,40,220,720,5,5,20,74250,1,1');time.sleep(2);shot('temporary-720p60.png')
finally:
 mode('1920,88,44,148,1080,4,5,36,74250,1,1');time.sleep(2);shot('restored-1080p30.png')
