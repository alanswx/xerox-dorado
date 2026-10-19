import subprocess
from pathlib import Path
root=Path(__file__).resolve().parent
quartus=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin')
for variant in ('baseline','snapshot'):
 for tool,args,label in (('quartus_map',['snapshot_fit'],'map'),
                         ('quartus_fit',['snapshot_fit'],'fit'),
                         ('quartus_sta',['snapshot_fit'],'sta'),
                         ('quartus_sta',['-t','audit.tcl'],'corners')):
  print(variant,label,flush=True)
  with (root/variant/(label+'.log')).open('w')as log:
   subprocess.run([str(quartus/tool),*args],cwd=root/variant,
                  stdout=log,stderr=subprocess.STDOUT,check=True)
print('ContB fits terminal; inspect all evidence before qualification.',flush=True)
