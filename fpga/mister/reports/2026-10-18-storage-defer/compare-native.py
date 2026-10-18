from pathlib import Path
import hashlib,json,os,subprocess
base=Path("build/rtl-executive/storage-defer/native-gen-check")
common=["--storage-modules","4","--storage-chip-kbits","64","--map-kwords","64","--pilot-disk","--receive-monitor","--instruction-monitor","--storage-prefetch","--storage-interleave"]
env=dict(os.environ,PYTHONPATH=str(Path("tools").resolve()))
results={}
for backend in ("ddr","sdram"):
 runs={}
 for kind,tool,flags in (("old","build/rtl-executive/storage-defer/old-native-generator.py",[]),("off","tools/sil_fpga_native.py",[]),("on","tools/sil_fpga_native.py",["--storage-defer-reads"])):
  out=base/backend/kind;out.mkdir(parents=True,exist_ok=True)
  subprocess.run(["python3",tool,"-o",str(out),"--storage-backend",backend,*common,*flags],env=env,check=True,stdout=subprocess.DEVNULL)
  runs[kind]=out
 files=sorted(p.name for p in runs["old"].iterdir() if p.is_file())
 changed=[]
 for name in files:
  values={k:(v/name).read_text().replace(str(v),"@OUTPUT@") for k,v in runs.items()}
  assert values["old"]==values["off"],(backend,name,"default changed")
  if values["on"]!=values["off"]:changed.append(name)
 assert set(changed)=={"manifest.json","msa_ddram.v","msa_ddram.mapping.json"},(backend,changed)
 a=(runs["on"]/"msa_ddram.v").read_text();b=(runs["off"]/"msa_ddram.v").read_text()
 assert a.count(".DEFER_READS(1)")==16 and a.replace(", .DEFER_READS(1)","")==b
 results[backend]={"default_files_unchanged":files,"opt_in_changed_only":changed}
(base/"qualification.json").write_text(json.dumps(results,indent=2)+"\n")
print(json.dumps(results,indent=2))
