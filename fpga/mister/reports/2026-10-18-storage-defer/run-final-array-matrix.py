from pathlib import Path
import json,subprocess,re
b=Path('build/rtl-executive/storage-defer/final-array-matrix');b.mkdir(exist_ok=True)
sources=['fpga/mister/dorado_storage_ddram.sv','fpga/mister/dorado_storage_group.sv','fpga/mister/dorado_storage_arb.sv','verilog/cells/cell_MK4096P_6.v','verilog/verilator/tb_storage_array.sv']
results=[]
for cache,defer,prefetch,interleave in [(c,1,p,i) for c in (0,7) for p in (0,1) for i in (0,1)]+[(0,0,1,1),(7,0,1,1)]:
 name='c%d-d%d-p%d-i%d'%(cache,defer,prefetch,interleave);out=b/name
 args=['verilator','--binary','--top-module','tb_storage_array','--timing','-Wno-fatal','-Wno-PINMISSING','-Wno-WIDTHEXPAND','-Wno-WIDTHTRUNC','-GLINE_CACHE_BITS='+str(cache),'-GDEFER_READS='+str(defer),'-GPREFETCH_NEXT='+str(prefetch),'-GINTERLEAVE_GROUPS='+str(interleave),'--Mdir',str(out),'-o','array-test',*sources]
 with (b/(name+'-build.log')).open('w') as f:subprocess.run(args,stdout=f,stderr=subprocess.STDOUT,check=True,timeout=180)
 p=subprocess.run([str(out/'array-test')],stdout=subprocess.PIPE,stderr=subprocess.STDOUT,text=True,timeout=10)
 (b/(name+'-test.log')).write_text(p.stdout);assert p.returncode==0,p.stdout
 m=re.search(r'STORAGE-ARRAY PASS:.*?(\d+) chip cycles,\s*(\d+) DDR reads,\s*(\d+) writes,.*?host_cycles=(\d+)',p.stdout);assert m,p.stdout
 q=dict(name=name,cache_bits=cache,defer=defer,prefetch=prefetch,interleave=interleave,logical_cycles=int(m[1]),physical_reads=int(m[2]),physical_writes=int(m[3]),host_cycles=int(m[4]),command=args)
 assert q['logical_cycles']==1506 and q['physical_writes']==463
 results.append(q);print({k:v for k,v in q.items()if k!='command'},flush=True)
(b/'qualification.json').write_text(json.dumps(results,indent=2)+'\n')
