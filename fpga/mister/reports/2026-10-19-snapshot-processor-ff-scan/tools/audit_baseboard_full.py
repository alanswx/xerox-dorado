#!/usr/bin/env python3
"""Read-only complete BaseBoard fit audit: source, state, RAM/ROM bytes and timing."""
import argparse,hashlib,json,re,shutil,tempfile
from collections import Counter
from pathlib import Path
import generate_baseboard_full as gen
from audit_i2716 import firmware,synthesized_image
CORNERS={'7_slow_1100mv_-40c','7_slow_1100mv_100c','MIN_fast_1100mv_-40c','MIN_fast_1100mv_100c'}


def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def normalized(t):return '\n'.join(l for l in t.splitlines()if l.strip()and l!='set_global_assignment -name LAST_QUARTUS_VERSION "17.0.2 Lite Edition"')


def table(text,title,key):
 start=text.find('; '+title);assert start>=0,'missing '+title
 section=text[start:].split('\n\n',1)[0];header=None;rows=[]
 for line in section.splitlines():
  if not line.startswith('; '):continue
  f=[x.strip()for x in line.split(';')[1:-1]]
  if f and f[0]==key:header=f
  elif f and header:
   assert len(header)==len(f),'malformed '+title;rows.append(dict(zip(header,f)))
 return rows


def memories(case,variant,m):
 rows=table((case/'snapshot_fit.fit.rpt').read_text(encoding='latin1'),'Fitter RAM Summary','Name')
 rams=[x for x in rows if x['Mode']=='Simple Dual Port'];roms=[x for x in rows if x['Mode']=='ROM']
 assert len(rows)==len(rams)+len(roms)and len(rams)==(32 if variant=='baseline'else 4)and len(roms)==4,'unexpected physical memory rows'
 all_sites=set();ram_sites=set();seen=set();members={x['instance']:r for r in m['packages']if r['type']=='SRAM'for x in r['group']['members']};groups={}
 for row in rams:
  if variant=='baseline':
   hit=re.search(r'cell_i2125:(u_\w+)\|',row['Name']);assert hit and hit[1]in members,'unknown original SRAM';key=hit[1];group=members[key]['index']-30;width=1
  else:
   hit=re.search(r'snapshot_sram_([0-3])\|',row['Name']);assert hit,'unknown candidate SRAM';group=int(hit[1]);key=group;width=8
  assert key not in seen,'duplicate physical SRAM';seen.add(key)
  for k,v in {'Mode':'Simple Dual Port','Clock Mode':'Single Clock','Port A Depth':'1024','Port B Depth':'1024','Port A Width':str(width),'Port B Width':str(width),
   'Size':str(width*1024),'Implementation Bits':str(width*1024),'M10K blocks':'1','MLAB cells':'0','Port A Input Registers':'yes','Port A Output Registers':'no','Port B Input Registers':'yes','Port B Output Registers':'no','Mixed Width RDW Mode':'Old data'}.items():assert row[k]==v,('SRAM geometry/collision differs',key,k,row[k])
  site=row['Location'];assert re.fullmatch(r'M10K_X\d+_Y\d+_N0',site),'non-M10K SRAM location'
  ram_sites.add(site);groups.setdefault(group,set()).add(site);all_sites.add(site)
 assert len(ram_sites)==4 and set(groups)==set(range(4))and all(len(s)==1 for s in groups.values()),'byte SRAM packing differs'
 rom_seen=set();rom_sites=set()
 for row in roms:
  hit=re.search(r'cell_i2716(?:_snapshot)?:(u_\w+)\|',row['Name']);assert hit,'unknown EPROM macro';p=hit[1];pkg=p[2:]
  assert pkg in gen.eprom.IMAGES and pkg not in rom_seen,'duplicate/empty/foreign EPROM';rom_seen.add(pkg)
  for k,v in {'Mode':'ROM','Clock Mode':'Single Clock','Port A Depth':'2048','Port A Width':'8','Port B Depth':'--','Port B Width':'--','Size':'16384','Implementation Bits':'16384','MLAB cells':'0','Port A Input Registers':'yes','Port A Output Registers':'no'}.items():assert row[k]==v,('EPROM physical geometry differs',pkg,k,row[k])
  locations=row['Location'].split(', ');assert len(locations)==int(row['M10K blocks'])and all(re.fullmatch(r'M10K_X\d+_Y\d+_N0',x)for x in locations),'EPROM site list differs'
  rom_sites.update(locations);all_sites.update(locations)
  image=case/'verilog/proms/eprom'/('BaseBd-'+pkg+'.mem');assert digest(image)==gen.eprom.IMAGES[pkg],'firmware asset differs'
  assert synthesized_image(case/row['MIF'])==firmware(image),'synthesized firmware differs'
 assert not(rom_sites&ram_sites),'ROM and mutable SRAM share a site'
 return dict(rows=rows,unique_m10k_sites=sorted(all_sites),sram_sites=sorted(ram_sites),eprom_sites=sorted(rom_sites))


def retention(text,m):
 rows=table(text,'Analysis & Synthesis Resource Utilization by Entity','Compilation Hierarchy Node');by_path={r['Full Hierarchy Name']:r for r in rows};retained=[]
 for p in m['packages']:
  if p['type']!='FF':continue
  hits=[r for r in rows if re.search(r'\b'+re.escape(p['kind']+'_snapshot:'+p['package'])+r'\|?$',r['Full Hierarchy Name'])]
  assert len(hits)==1,'missing/duplicate FF state hierarchy:'+p['package'];r=hits[0];n=p['logical_state_bits']
  assert r['Dedicated Logic Registers']==f'{n} ({n})','FF state pruned/added:'+p['package']
  retained.append(dict(package=p['package'],bits=n,hierarchy=r['Full Hierarchy Name']))
 assert sum(r['bits']for r in retained)==279
 # Every CPU node can be independently imported/read back. Private response
 # and common freeze registers may merge or prune across the board.
 cpu_rows=[r for r in rows if r['Entity Name']=='chip_6502_snapshot'];assert len(cpu_rows)==1,'missing CPU node hierarchy'
 cpu_regs=int(cpu_rows[0]['Dedicated Logic Registers'].split()[0]);assert cpu_regs>=1725,'CPU nodes pruned'
 riot_rows=[r for r in rows if r['Entity Name']=='M6532_snapshot'];assert len(riot_rows)==5,'five RIOTs required'
 for r in riot_rows:assert int(r['Dedicated Logic Registers'].split()[0])>=1097,'RIOT state pruned'
 return dict(ff=retained,cpu_synthesis_registers=cpu_regs,riot_synthesis_registers=[int(r['Dedicated Logic Registers'].split()[0])for r in riot_rows])


def audit(directory,root):
 record=json.loads((directory/'provenance.json').read_text());m=record['manifest'];_,expected,_=gen.generate(root);assert m==expected,'unreviewed complete board source/schema'
 assert m['logical_state_bits']==40366 and len(m['packages'])==48
 for n,h in record['compiled_files'].items():
  if not n.endswith('.qsf'):assert digest(directory/n)==h,'compiled input differs:'+n
 results={}
 for variant in('baseline','snapshot'):
  case=directory/variant
  assert normalized((case/'snapshot_fit.qsf').read_text())==normalized(record['expected_qsf'][variant]),'project source/macro/SDC differs'
  assert digest(case/'BaseBd_original.v')==gen.BOARD_SHA and digest(case/'BaseBd_snapshot.sv')==m['generated_sha256'],'board source differs'
  for n,h in m['original_sources'].items():assert digest(case/Path(n).name)==h,'original cell/vendor differs'
  for n,h in m['cloned_sources'].items():assert digest(case/n)==h,'qualified helper differs'
  summary=(case/'snapshot_fit.fit.summary').read_text();assert 'Fitter Status : Successful'in summary
  resources={}
  for k,label in [('alms',r'Logic utilization \(in ALMs\)'),('registers','Total registers'),('ram_bits','Total block memory bits'),('ram_blocks','Total RAM Blocks')]:
   hit=re.search(label+r' : ([\d,]+)',summary);assert hit,'missing resources';resources[k]=int(hit[1].replace(',',''))
  assert resources['ram_bits']==98304,'SRAM/firmware bits differ'
  memory=memories(case,variant,m);assert resources['ram_blocks']==len(memory['unique_m10k_sites']),'physical M10K packing/count differs'
  assert {p.name[len('bank-setup-'):-4]for p in case.glob('bank-setup-*.rpt')}==CORNERS,'missing/extra corner'
  timing={}
  for corner in sorted(CORNERS):
   timing[corner]={}
   for mode in('setup','hold','mpw'):
    t=(case/f'bank-{mode}-{corner}.rpt').read_text();hit=re.search(r'\((\d+) violated\)\.\s+Worst case slack is (-?[\d.]+)',t)
    assert hit and int(hit[1])==0 and float(hit[2])>=0,'timing violation/missing path:'+mode+'/'+corner;timing[corner][mode]=float(hit[2])
   for mode in('recovery','removal'):assert (case/f'bank-{mode}-{corner}.rpt').read_text().strip().endswith('Nothing to report.'),'unexpected asynchronous path'
   t=(case/f'bank-ucp-{corner}.rpt').read_text()
   for label in('Illegal Clocks','Unconstrained Clocks','Unconstrained Input Ports','Unconstrained Input Port Paths','Unconstrained Output Ports','Unconstrained Output Port Paths'):assert re.search('; '+label+r'\s*; 0\s*; 0\s*;',t),'illegal/unconstrained:'+label
  mapping=(case/'snapshot_fit.map.rpt').read_text(encoding='latin1');warnings={}
  for phase in('map','fit'):
   t=(case/(phase+'.log')).read_text();assert 'Critical Warning'not in t,'critical warning';warnings[phase]=dict(Counter(re.findall(r'Warning \((\d+)\)',t)))
  # Actual board/divider bindings, not defaults in uninstantiated source.
  for kind,pkg,param,wanted in [(gen.MODULE if variant=='snapshot'else gen.ORIGINAL_MODULE,'board','SYSPER',4),('cell_CLOCKGEN_snapshot'if variant=='snapshot'else'cell_CLOCKGEN','u_clockgen','N',4)]:
   h=re.search(r'; Parameter Settings for User Entity Instance: [^\n;]*\b'+kind+r':[^\n]*\b'+pkg+r'\s*;(.*?)(?:Note:)',mapping,re.S);assert h and re.search('; '+param+r'\s*; '+str(wanted)+r'\s*;',h[1]),'actual parameter binding differs:'+kind
  row=dict(resources=resources,memory=memory,timing=timing,warnings=warnings)
  if variant=='snapshot':row['state_retention']=retention(mapping,m)
  results[variant]=row
 for phase in('map','fit'):
  new=Counter(results['snapshot']['warnings'][phase])-Counter(results['baseline']['warnings'][phase]);assert not(set(new)-{'10036','13024','13410'}),('unreviewed warnings',phase,new)
 return dict(status='PASS',scope=record['scope'],results=results,delta={k:results['snapshot']['resources'][k]-results['baseline']['resources'][k]for k in('alms','registers','ram_bits','ram_blocks')},minimum_snapshot_slack_ns=min(v for c in results['snapshot']['timing'].values()for v in c.values()))


def controls(directory,root):
 rows=[]
 for name,path,before,after in [
  ('board','snapshot/BaseBd_snapshot.sv','state_hold && state_frozen && state_request','state_request'),
  ('helper','snapshot/cell_MCS6502_snapshot.sv','.state_index(state_index)',".state_index(7'b0)"),
  ('constraint','snapshot/bank.sdc','20.000','18.000'),('macro','snapshot/snapshot_fit.qsf','SNAPSHOT_BANK_EXPERIMENT=1','SNAPSHOT_BANK_EXPERIMENT=0'),
  ('state','snapshot/snapshot_fit.map.rpt','25 (25)','24 (24)'),('collision','snapshot/snapshot_fit.fit.rpt','Old data','New data'),
  ('hold','snapshot/bank-hold-7_slow_1100mv_-40c.rpt','(0 violated).','(1 violated).'),('warning','snapshot/map.log',None,None)]:
  with tempfile.TemporaryDirectory(prefix='dorado-baseboard-fit-',dir='/tmp')as tmp:
   out=Path(tmp)/'evidence';shutil.copytree(directory,out);p=out/path;t=p.read_text(encoding='latin1')
   if before:assert before in t;t=t.replace(before,after)
   else:t+='\nWarning (10999): unreviewed inference\n'
   p.write_text(t,encoding='latin1')
   try:audit(out,root)
   except AssertionError as e:rows.append(dict(case=name,rejected=True,diagnostic=str(e)))
   else:raise AssertionError('damaged complete BaseBoard fit accepted:'+name)
 return rows


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path);p.add_argument('--frozen-root',type=Path,required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--negative-controls',action='store_true');a=p.parse_args();r=audit(a.directory.resolve(),a.frozen_root.resolve())
 if a.negative_controls:r['failure_controls']=controls(a.directory.resolve(),a.frozen_root.resolve())
 a.output.write_text(json.dumps(r,indent=2)+'\n');print('FULL BASEBOARD FIT AUDIT PASS;minimum slack',r['minimum_snapshot_slack_ns'])
