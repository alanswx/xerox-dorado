#!/usr/bin/env python3
"""Read-only full DskEth source/state retention/ROM placement/timing audit."""
import argparse,hashlib,json,re,shutil,tempfile
from pathlib import Path
from collections import Counter
CORNERS={'7_slow_1100mv_-40c','7_slow_1100mv_100c','MIN_fast_1100mv_-40c','MIN_fast_1100mv_100c'}
BOARD_SHA='14f3a2c55450231c0bd0feafa22b5e0c08f09eddad8358fec2797baf46ec5f81'
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def normalized(s):return '\n'.join(l for l in s.splitlines()if l.strip()and l!='set_global_assignment -name LAST_QUARTUS_VERSION "17.0.2 Lite Edition"')


def state_registers(text,m):
 header=None;rows={};expected={r['package']:r for r in m['ff']+m['memories']}
 for line in text.splitlines():
  if not line.startswith('; '):continue
  f=[x.strip()for x in line.split(';')[1:-1]]
  if f and f[0]=='Compilation Hierarchy Node':header=f;continue
  if not header or len(f)!=len(header):continue
  match=re.fullmatch(r'\|(\w+_snapshot):(u_\w+)\|',f[0])
  if not match:continue
  kind,pkg=match.groups()
  if pkg not in expected:raise AssertionError('unexpected snapshot state helper:'+pkg)
  r=expected[pkg];assert kind==r['kind']+'_snapshot'and pkg not in rows,'state helper changed/duplicated'
  row=dict(zip(header,f));raw=row['Dedicated Logic Registers'];a=re.fullmatch(r'(\d+) \((\d+)\)',raw);assert a and a[1]==a[2]
  wanted=r['width']if r in m['ff']else 85 if r['kind']=='cell_F10145A'and pkg=='u_d08'else 84 if r['kind']=='cell_F10145A'else r['logical_state_bits']+18
  assert int(a[1])==wanted,('state register retention differs',pkg,raw,wanted)
  assert row['Block Memory Bits']=='0','mutable small file unexpectedly moved to RAM'
  rows[pkg]=dict(kind=r['kind'],registers=int(a[1]),row=row)
 assert set(rows)==set(expected)and len(rows)==141,'missing state helper register row'
 return dict(rows=rows,scope='Synthesis entity counts retain every local mutable bit plus expected response registers; shared frozen flag kept at u_d08. These are synthesis counts, not a fitted unique-register count or enabled whole-core budget.')


def baseline_roms(text):
 start=text.find('; Fitter RAM Summary');assert start>=0,'missing baseline ROM table';header=None;rows=[]
 for line in text[start:].split('\n\n',1)[0].splitlines():
  if not line.startswith('; '):continue
  f=[x.strip()for x in line.split(';')[1:-1]]
  if f and f[0]=='Name':header=f
  elif f and '|cell_'in f[0]:assert header and len(header)==len(f);rows.append(dict(zip(header,f)))
 widths=dict(u_b14=4,u_h09=4,u_h10=4,u_h11=4,u_h14=4,u_h15=4,u_h16=2,u_l10=3,u_l15=3)
 assert len(rows)==9;seen=set();groups={}
 for row in rows:
  a=re.search(r'\|cell_MCM10149:(u_\w+)\|',row['Name']);assert a and a[1]in widths and a[1]not in seen,'unknown/duplicate fitted ROM'
  pkg=a[1];seen.add(pkg);w=widths[pkg]
  expected={'Type':'AUTO','Mode':'ROM','Clock Mode':'Single Clock','Port A Depth':'256','Port A Width':'4','Size':'1024',
   'Implementation Port A Depth':'256','Implementation Port A Width':str(w),'Implementation Bits':str(w*256),'M10K blocks':'1','MLAB cells':'0',
   'Port A Input Registers':'yes','Port A Output Registers':'yes','ECC Mode':'Off','ECC Pipeline Registers':'No'}
  for k,v in expected.items():assert row.get(k)==v,('baseline ROM geometry differs',pkg,k,row.get(k))
  site=row['Location'];assert re.fullmatch(r'M10K_X\d+_Y\d+_N\d+',site),'baseline ROM placement differs'
  groups.setdefault(site,[]).append(pkg)
 assert seen==set(widths)and len(groups)==5
 wanted={frozenset(x)for x in [('u_b14',),('u_h09','u_h10','u_h11'),('u_h14','u_h15','u_h16'),('u_l10',),('u_l15',)]}
 assert {frozenset(x)for x in groups.values()}==wanted,'unreviewed ROM packing'
 for packages in groups.values():assert sum(widths[p]*256 for p in packages)<=10240,'ROM physical capacity exceeded'
 assert sum(int(r['Implementation Bits'])for r in rows)==8192
 return dict(rows=rows,physical_groups=groups,scope='Immutable ROMs only: nine logical tables,8192implemented bits in5shared M10Ks. Other PROM logic is LUT based; all14source assets hash checked.')


def audit(root):
 p=json.loads((root/'provenance.json').read_text());m=p['manifest'];results={}
 assert p['kind']=='DskEth'and m['original_sha256']==p['source_board_sha256']==BOARD_SHA
 assert m==json.loads((root/'qualified-manifest.json').read_text()),'qualified schema differs'
 assert m['logical_state_bits']==2656 and m['parameters']==dict(SYSPER=4)and len(m['ff'])==117 and len(m['memories'])==24
 assert len(m['immutable_proms'])==14 and sum(a['words']*a['width']for a in m['immutable_proms'])==12032
 assert len([r for r in m['memories']if r['kind']=='cell_F10145A'])==18
 assert [r['parameters']for r in m['memories']if r['kind']=='cell_SN74123']==[dict(TW_NS=3000,SYSPER=4,TW=400)]*2
 assert next(r for r in m['memories']if r['kind']=='cell_K1115A')['parameters']==dict(FREQ_KHZ=23530,SYSPER=8)
 assert sorted(r['parameters']['PRECAP']for r in m['memories']if r['kind']=='cell_F9401')==[0,1]
 for n,h in p['compiled_files'].items():
  if not n.endswith('.qsf'):assert digest(root/n)==h,'compiled source/constraint differs:'+n
 for variant in ('baseline','snapshot'):
  c=root/variant;assert digest(c/'DskEth_original.v')==BOARD_SHA and digest(c/'DskEth_snapshot.sv')==m['generated_sha256']
  for n,h in m['original_sources'].items():assert digest(c/Path(n).name)==h,'original cell differs:'+n
  for n,h in m['cloned_sources'].items():assert digest(c/n)==h,'qualified clone differs:'+n
  for a in m['immutable_proms']:assert digest(c/a['file'])==a['sha256'],'immutable PROM differs'
  assert normalized((c/'snapshot_fit.qsf').read_text())==normalized(p['expected_qsf'][variant]),'project source/macro/constraint changed'
  text=(c/'snapshot_fit.fit.summary').read_text();assert 'Fitter Status : Successful'in text
  resources={}
  for k,label in [('alms',r'Logic utilization \(in ALMs\)'),('registers','Total registers'),('ram_bits','Total block memory bits'),('ram_blocks','Total RAM Blocks')]:
   a=re.search(label+r' : ([\d,]+)',text);assert a;resources[k]=int(a[1].replace(',',''))
  assert(resources['ram_bits'],resources['ram_blocks'])==((8192,5)if variant=='baseline'else(0,0)),'ROM placement/resources differ'
  assert {f.name[len('bank-setup-'):-4]for f in c.glob('bank-setup-*.rpt')}==CORNERS,'missing timing corner'
  timing={}
  for corner in sorted(CORNERS):
   timing[corner]={}
   for mode in ('setup','hold','mpw'):
    text=(c/f'bank-{mode}-{corner}.rpt').read_text();a=re.search(r'\((\d+) violated\)\.\s+Worst case slack is (-?[\d.]+)',text)
    assert a and int(a[1])==0 and float(a[2])>=0,('timing violation/missing path',variant,corner,mode)
    timing[corner][mode]=float(a[2])
   for mode in ('recovery','removal'):assert(c/f'bank-{mode}-{corner}.rpt').read_text().strip().endswith('Nothing to report.'),'unexpected async path'
   for label in ('Illegal Clocks','Unconstrained Clocks','Unconstrained Input Ports','Unconstrained Input Port Paths','Unconstrained Output Ports','Unconstrained Output Port Paths'):
    assert re.search('; '+label+r'\s*; 0\s*; 0\s*;',(c/f'bank-ucp-{corner}.rpt').read_text()),'unconstrained '+label
  warnings={}
  for phase in ('map','fit'):
   text=(c/(phase+'.log')).read_text();assert 'Critical Warning'not in text,'critical warning';warnings[phase]=dict(Counter(re.findall(r'Warning \((\d+)\)',text)))
  result=dict(resources=resources,timing=timing,warnings=warnings)
  if variant=='snapshot':
   result['state_registers']=state_registers((c/'snapshot_fit.map.rpt').read_text(errors='replace'),m)
   assert '; Fitter RAM Summary'not in(c/'snapshot_fit.fit.rpt').read_text(errors='replace'),'unexpected candidate block RAM'
  else:result['immutable_rom_placement']=baseline_roms((c/'snapshot_fit.fit.rpt').read_text(errors='replace'))
  results[variant]=result
 for phase in ('map','fit'):
  extra=Counter(results['snapshot']['warnings'][phase])-Counter(results['baseline']['warnings'][phase]);assert not(set(extra)-{'10036','13024','13410'}),('unreviewed warning increase',phase,extra)
 return dict(status='PASS',scope=p['scope'],results=results,minimum_snapshot_slack_ns=min(x for r in results['snapshot']['timing'].values()for x in r.values()),
  physical_scope='All18mutable files remain logic registers; all141state helper synthesis counts checked. Baseline packs immutable ROMs into5M10Ks; candidate implements PROMs in LUTs, with identical assets. Counts include shell/admin/pruning and do not budget enabled whole-core snapshots. No FPGA machine save/load or calibrated timer/speed claim.')


def controls(root):
 rows=[]
 for name,path,before,after in [('source','snapshot/DskEth_snapshot.sv','state_hold && state_frozen && state_request','state_request'),('monitor','snapshot/dorado_receive_monitor_snapshot.sv','!state_hold && state_advance','state_advance'),('macro','snapshot/snapshot_fit.qsf','SNAPSHOT_BANK_EXPERIMENT=1','SNAPSHOT_BANK_EXPERIMENT=0'),('constraint','snapshot/bank.sdc','20.000','18.000'),('hold','snapshot/bank-hold-7_slow_1100mv_-40c.rpt','(0 violated).','(1 violated).'),('prom','snapshot/verilog/proms/packages/DskEth-d05.mem',None,None),('placement','baseline/snapshot_fit.fit.rpt','M10K_X','MLAB_X'),('file_register','snapshot/snapshot_fit.map.rpt','85 (85)','84 (84)'),('ff_register','snapshot/snapshot_fit.map.rpt','9 (9)','8 (8)'),('missing_corner','snapshot/bank-setup-7_slow_1100mv_100c.rpt',None,None),('warning','snapshot/map.log',None,None)]:
  with tempfile.TemporaryDirectory(prefix='dorado-dsketh-fault-',dir='/tmp')as tmp:
   d=Path(tmp)/'evidence';shutil.copytree(root,d,ignore=shutil.ignore_patterns('db','incremental_db'));f=d/path
   if name=='prom':f.write_bytes(b'0\n')
   elif name=='missing_corner':f.unlink()
   elif name=='warning':f.write_text(f.read_text()+'\nWarning (10999): unreviewed inference\n')
   elif name in ('file_register','ff_register'):
    text=f.read_text();lines=text.splitlines(keepends=True);token='|cell_F10145A_snapshot:u_d08|'if name=='file_register'else'|cell_F10000_snapshot:u_f09|'
    for i,line in enumerate(lines):
     if line.startswith('; ')and token in line and before in line:lines[i]=line.replace(before,after,1);break
    else:raise AssertionError('missing register fault anchor:'+name)
    f.write_text(''.join(lines))
   else:
    text=f.read_text(errors='replace');assert before in text;f.write_text(text.replace(before,after))
   try:audit(d)
   except AssertionError as e:rows.append(dict(case=name,rejected=True,diagnostic=str(e)))
   else:raise AssertionError('damaged DskEth fit accepted:'+name)
 return rows

if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path);p.add_argument('--output',type=Path,required=True);p.add_argument('--negative-controls',action='store_true');a=p.parse_args();q=audit(a.directory.resolve())
 if a.negative_controls:q['failure_controls']=controls(a.directory.resolve())
 a.output.write_text(json.dumps(q,indent=2)+'\n');print(json.dumps(q,indent=2))
