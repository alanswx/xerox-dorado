from pathlib import Path
import hashlib,json,re,tarfile
root=Path('/home/alans/mister/Dorado_Native49_20261019')
core=root/'core'
files=list(sorted(core.glob('native49-*.rpt')))
records=[]
for p in files:
 if '-clocks-' in p.name or '-ucp-' in p.name:continue
 s=p.read_text()
 m=re.search(r'Worst.case slack is\s+([-+0-9.]+)',s,re.I)
 if not m:m=re.search(r';\s*([-+]?\d+\.\d+)\s*;',s)
 assert m,(p.name,s[:1500])
 slack=float(m[1]);assert slack>=0,(p.name,slack)
 records.append(dict(report=p.name+'.gz',slack_ns=slack))
assert len(records)==36,len(records)
summary=core/'output_files/Dorado.sta.summary'
checks=re.findall(r'Type\s*:\s*([^\n]+)\nSlack\s*:\s*([-+\d.]+)',summary.read_text())
assert len(checks)==31 and min(float(v)for _,v in checks)>=0,checks
ucp=(core/'native-unconstrained.rpt').read_text()
for category in ('Illegal Clocks','Unconstrained Clocks'):
 m=re.search(r';\s*'+category+r'\s*;\s*(\d+)\s*;\s*(\d+)\s*;',ucp)
 assert m and not any(map(int,m.groups())),(category,m.groups()if m else None)
rbf=core/'output_files/Dorado.rbf'
inputs=[core/'Dorado.sv',core/'Dorado.qsf',core/'native_timing.sdc',core/'sdram_timing.sdc',root/'src/fpga/mister/dorado_storage_sdram.sv',root/'src/fpga/mister/dorado_instruction_monitor.sv',root/'src/verilog/cells/cell_F10145A.v']
cp=root/'src/build/rtl-executive/native16-tx-cable'
inputs += [root/'src/fpga/mister/dorado_storage_arb.sv',root/'src/fpga/mister/dorado_storage_group.sv',cp/'msa_ddram.v']
inputs += [root/'src/fpga/mister'/n for n in ('dorado_ethernet_uio.sv','dorado_ethernet_ddr.sv')]
inputs += [cp/n for n in ('ContA_monitor.v','dorado_native_backplane.v','dorado_native_machine.sv','manifest.json')]
q=dict(hardware='NOT DEPLOYED',frequency_mhz=50,sysper=4,operating_conditions_checked=4,path_categories_checked=36,timing_checks=len(checks),minimum_slack_ns=min(r['slack_ns']for r in records),illegal_clocks=0,unconstrained_clocks=0,extra_timing_exceptions='None',raw_backend='SDRAM, no raw line buffer',raw_line_cache_entries=0,storage_defer_reads=False,storage_prefetch=True,storage_interleave=True,network_ddr=True,required_sdram_mb=128,guest_ram_mib=8,raw_backing_mib=16,change='Frozen Native48 plus frozen Native44 physical raw group/word interleaving; same physical DDR network, successor arbitration/no cache and original clocks/boards/constraints',reports=records,rbf_sha256=hashlib.sha256(rbf.read_bytes()).hexdigest())
(root/'qualification.json').write_text(json.dumps(q,indent=2)+'\n')
(root/'compiled-inputs.json').write_text(json.dumps({str(p.relative_to(root)):hashlib.sha256(p.read_bytes()).hexdigest()for p in inputs},indent=2)+'\n')
with tarfile.open('/tmp/native49-evidence.tar.gz','w:gz')as t:
 for p in files+[core/'native-unconstrained.rpt',summary,core/'output_files/Dorado.fit.summary',root/'qualification.json',root/'compiled-inputs.json',root/'interleave-experiment.json']:
  t.add(p,arcname=p.name)
print(json.dumps({k:v for k,v in q.items()if k!='reports'},indent=2))
print((core/'output_files/Dorado.fit.summary').read_text())
