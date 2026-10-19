#!/usr/bin/env python3
"""Ground all32 actual native BaseBoard 2125 slices in the original wire list.

Identical literal address/CE/WE pins prove four byte groups. No combinational
rewrites, CPU byte-order assumptions, board freeze or whole-state claim.
"""
import argparse,hashlib,json,re,sys,tempfile
from pathlib import Path
from audit_contb_im import instances

ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'tools'))
from sil_netlist import load_board
from sil_to_verilog import vname
SIL=ROOT/'chm/sil/BaseBd-Rev-Am.dm!1_/BaseBd-Rev-Am.wl'
SIL_SHA='ca1a3acdc926f56f6da5ebceb46209bf45473b5ddcf102860358813dfdcd6152'
ADDRESS=(13,12,11,10,9,6,5,4,3,2)
CELL_SHA='6eb55772966be2b159da55d2495a57dd66de73f10137479f7f417002aaa157c9'
CORE_SHA='d5e0649f392cfdbc116de9bbc5b615ba923526704357ddfb0888ace111fea5ca'


def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def audit(source_root,text=None,cell=None,wirelist=None):
 source=source_root/'build/rtl-executive/native16-tx-cable/BaseBd_nominal.v'
 text=source.read_text()if text is None else text
 cell=(source_root/'verilog/cells/cell_i2125.v').read_bytes()if cell is None else cell
 if hashlib.sha256(cell).hexdigest()!=CELL_SHA:raise ValueError('unreviewed 2125 cell')
 if digest(Path(__file__).parent/'f10415_word_bank.sv')!=CORE_SHA:raise ValueError('qualified RAM transport changed')
 wirelist=SIL if wirelist is None else wirelist
 if digest(wirelist)!=SIL_SHA:raise ValueError('unreviewed original wire list')
 board=load_board(str(wirelist));expected={p for p,r in board.packages.items()if r['type']=='i2125'}
 pins={(p['pkg'],p['pin']):n for n,net in board.nets.items()for p in net['pins']}
 ram=[(name,p)for kind,name,p in instances(text)if kind=='cell_i2125']
 if len(ram)!=32 or len({name for name,_ in ram})!=32 or {name[2:]for name,_ in ram}!=expected:raise ValueError('all32 unique original 2125 packages required')
 groups={};outputs=set()
 for name,p in ram:
  pkg=name[2:]
  if p.get('sys_clk')!='sys_clk':raise ValueError('2125 clock changed:'+pkg)
  for pin in ADDRESS+(1,14,15):
   if p.get('p'+str(pin))!=vname(pins[(pkg,pin)]):raise ValueError('wire-list pin differs:%s.%d'%(pkg,pin))
  out=vname(pins[(pkg,7)])+'__'+pkg+'_7'
  if p.get('p7')!=out or out in outputs:raise ValueError('2125 output omitted/aliased:'+pkg)
  outputs.add(out)
  cs=pins[(pkg,1)];m=re.fullmatch(r"RS([0-3])'",cs)
  if not m:raise ValueError('unreviewed 2125 select:'+cs)
  index=int(m[1]);we="WE'AC"if index<2 else"WE'BD"
  if pins[(pkg,14)]!=we:raise ValueError('2125 bank WE differs')
  data=p['p15'];m=re.fullmatch(r'MCD_([0-7])',data)
  if not m:raise ValueError('unreviewed 2125 data:'+data)
  bit=int(m[1]);key=tuple(p['p'+str(n)]for n in ADDRESS)+(p['p1'],p['p14'])
  group=groups.setdefault(index,dict(index=index,address=list(key[:10]),ce=p['p1'],we=p['p14'],literal_key=key,members=[]))
  if group['literal_key']!=key:raise ValueError('group address/control pins differ')
  group['members'].append(dict(package=pkg,instance=name,bit=bit,data=data,output=out))
 if set(groups)!=set(range(4)):raise ValueError('four original banks required')
 for g in groups.values():
  g.pop('literal_key');g['members'].sort(key=lambda x:x['bit'])
  if [r['bit']for r in g['members']]!=list(range(8)):raise ValueError('missing/aliased byte plane')
  if g['address']!=['RA'+str(i)for i in range(9,-1,-1)]:raise ValueError('2125 address order differs')
 return dict(status='PASS',scope=__doc__,groups=[groups[i]for i in range(4)],
  logical_ram_bits=32768,registered_read_bits=32,logical_state_bits=32800,
  source=str(source.relative_to(source_root)),source_sha256=hashlib.sha256(text.encode()).hexdigest(),
  cell_sha256=CELL_SHA,shared_transport_sha256=CORE_SHA,wirelist=str(SIL.relative_to(ROOT)),wirelist_sha256=digest(wirelist),
  tool_sources={str(p.relative_to(ROOT)):digest(p)for p in [Path(__file__).resolve(),Path(__file__).parent/'audit_contb_im.py',ROOT/'tools/sil_netlist.py',ROOT/'tools/sil_to_verilog.py']},
  bit_order='Word bit i is original MCD_i wire; no assumption about 6502 byte significance')


def controls(source_root):
 original=(source_root/'build/rtl-executive/native16-tx-cable/BaseBd_nominal.v').read_text();rows=[]
 anchor=re.search(r'\bcell_i2125\s+u_b11\s*\(.*?\);',original,re.S);assert anchor
 for name,before,after in [('address','.p2(RA0)','.p2(RA1)'),('select','.p1(RS0_p_)','.p1(RS1_p_)'),
  ('write_enable','.p14(WE_p_AC)','.p14(WE_p_BD)'),('data','.p15(MCD_7)','.p15(MCD_6)'),
  ('output','.p7(MCD_7__b11_7)','.p7(MCD_7__b12_7)'),('clock','.sys_clk(sys_clk)',".sys_clk(1'b0)")]:
  block=anchor[0];assert before in block
  changed=original[:anchor.start()]+block.replace(before,after,1)+original[anchor.end():]
  try:audit(source_root,changed)
  except ValueError as e:rows.append(dict(case=name,rejected=True,diagnostic=str(e)))
  else:raise AssertionError('damaged BaseBoard grouping accepted:'+name)
 for name,changed in [('missing',original[:anchor.start()]+original[anchor.end():]),('duplicate',original+anchor[0])]:
  try:audit(source_root,changed)
  except ValueError as e:rows.append(dict(case=name,rejected=True,diagnostic=str(e)))
  else:raise AssertionError('damaged 2125 package count accepted:'+name)
 try:audit(source_root,cell=b'reg hidden;')
 except ValueError as e:rows.append(dict(case='changed_cell',rejected=True,diagnostic=str(e)))
 else:raise AssertionError('changed 2125 semantics accepted')
 changed=original
 for name,pins in [(n,p)for k,n,p in instances(original)if k=='cell_i2125']:
  anchor=re.search(r'\bcell_i2125\s+'+name+r'\s*\(.*?\);',changed,re.S);assert anchor
  block=anchor[0].replace('.p2(RA0)','.p2(RA1)').replace('.p3(RA1)','.p3(RA0)')
  changed=changed[:anchor.start()]+block+changed[anchor.end():]
 try:audit(source_root,changed)
 except ValueError as e:rows.append(dict(case='common_address_swap',rejected=True,diagnostic=str(e)))
 else:raise AssertionError('common address corruption accepted')
 with tempfile.TemporaryDirectory(prefix='dorado-baseboard-wirelist-',dir='/tmp')as tmp:
  p=Path(tmp)/'changed.wl';p.write_bytes(SIL.read_bytes()+b'\n')
  try:audit(source_root,wirelist=p)
  except ValueError as e:rows.append(dict(case='wirelist_source',rejected=True,diagnostic=str(e)))
  else:raise AssertionError('unreviewed wire-list source accepted')
 return rows


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--frozen-root',type=Path,required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--negative-controls',action='store_true')
 a=p.parse_args();q=audit(a.frozen_root.resolve())
 if a.negative_controls:q['failure_controls']=controls(a.frozen_root.resolve())
 a.output.write_text(json.dumps(q,indent=2)+'\n');print('BaseBoard2125 grouping PASS:four1024x8 groups/32800state bits')
