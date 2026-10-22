#!/usr/bin/env python3
"""Review exact one-MSA ready prediction and all15ns timing conditions.

The fit has registered virtual services; no complete native/SDRAM-I/O or
hardware speed qualification follows from this component review.
"""
import argparse,collections,contextlib,hashlib,io,json,re,sys,tempfile
from pathlib import Path
import fit_storage_ready_prediction as fit
import prepare_storage_ready_prediction as prepare
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'fpga/mister/snapshot'))
from audit_native_conta import ucp
from sil_netlist import load_board
CORNERS={'7_slow_1100mv_100c','7_slow_1100mv_-40c','MIN_fast_1100mv_100c','MIN_fast_1100mv_-40c'}
def sha(b):return hashlib.sha256(b).hexdigest()
def read_json(p):return json.loads(p.read_text())
def checked_inputs(root,proof):
 for n,h in proof['inputs'].items():assert sha((root/'inputs'/n).read_bytes())==h,('Component input changed',n)
def path_report(p):
 t=p.read_text();m=re.search(r'Found (\d+) (?:setup|hold) paths.*Worst case slack is (-?[0-9.]+)',t)
 assert m and int(m[1])>0,('Missing timed paths',p.name)
 assert float(m[2])>=0,('Negative slack',p.name,m[2])
 return dict(report=p.name,paths=int(m[1]),slack_ns=float(m[2]),sha256=sha(p.read_bytes()))
def review(root,component,groups):
 cp=read_json(component/'provenance.json');cs=read_json(component/'run-state.json')
 assert cs['status']=='PASS_POST_EDGE_READY_ORIGINAL_MSA_COMPONENT' and len(cs['actions'])==8
 assert all(a['returncode']!=0 if a['stage']in ['stale-ready-test','old-address-test','ignore-write-test']else a['returncode']==0 for a in cs['actions'])
 checked_inputs(component,cp)
 parent=Path(cp['parent']);assert sha((parent/'provenance.json.gz').read_bytes())==cp['parent_provenance_sha256']
 assert (component/'inputs/dorado_storage_group.sv').read_bytes()==prepare.group_prediction((parent/'inputs/dorado_storage_group.sv').read_bytes())
 assert (component/'inputs/msa_ddram.v').read_bytes()==prepare.board_preview((parent/'inputs/msa_ddram.v').read_bytes())
 assert (component/'inputs/addr_preview.sv').read_bytes()==prepare.address_preview((parent/'inputs/cells/cell_MK4096P_addr.v').read_bytes())
 assert (component/'positive-raw.csv').read_bytes()==(parent/'burst-raw.csv').read_bytes(),'Host epochs or raw data changed'
 assert 'all 9437184 stored bits identical' in (component/'positive-test.log').read_text()
 gp=read_json(groups/'provenance.json');gs=read_json(groups/'run-state.json');checked_inputs(groups,gp)
 assert gs['status']=='PASS_READY_GROUP_TWO_SIMULATORS' and len(gs['actions'])==12
 assert gp['component_provenance_sha256']==sha((component/'provenance.json').read_bytes())
 assert (groups/'inputs/group.sv').read_bytes()==(component/'inputs/dorado_storage_group.sv').read_bytes()
 assert (groups/'inputs/preview.sv').read_bytes()==(component/'inputs/addr_preview.sv').read_bytes()
 for engine in ['icarus','verilator']:
  for width in [8,12]:
   t=(groups/(engine+'-w'+str(width)+'-test.log')).read_text();assert 'STORAGE_READY_GROUP PASS width='+str(width)in t
   assert re.search(r'checks=40000 steps=\d+ reads=\d+ writes=\d+ paused_responses=\d+ reset_cycles=\d+',t)
 assert all(a['returncode']!=0 if a['stage'].endswith('-stale-test')else a['returncode']==0 for a in gs['actions'])
 fp=read_json(root/'provenance.json');s=read_json(root/'run-state.json');terminal=read_json(root/'terminal-input-check.json')
 assert s['status']=='TERMINAL_READY_TIMING_REVIEW_REQUIRED' and len(s['actions'])==4 and all(a['returncode']==0 for a in s['actions'])
 assert set(terminal['absent_pids'])=={s['driver_pid']}|{a['pid']for a in s['actions']}
 assert fp['component_provenance_sha256']==sha((component/'provenance.json').read_bytes()) and fp['component_state_sha256']==sha((component/'run-state.json').read_bytes())
 with tempfile.TemporaryDirectory(prefix='dorado-ready-source-')as tmp:
  regenerated=Path(tmp)/'source'
  with contextlib.redirect_stdout(io.StringIO()):fit.prepare(component,regenerated)
  expected=read_json(regenerated/'provenance.json')
  assert expected==fp,'Fit source/configuration differs from qualified component recipe'
 tail=b'\nset_global_assignment -name LAST_QUARTUS_VERSION "17.0.2 Lite Edition"'
 for n,h in fp['files'].items():
  b=(root/n).read_bytes()
  if n=='ready_fit.qsf':
   assert b.endswith(tail)and sha(b[:-len(tail)])==h,'Unexpected fitted settings change'
  else:assert sha(b)==h,('Fitted input changed',n)
 assert set(terminal['changed'])=={'ready_fit.qsf'} and terminal['exact_settings_append'].encode()==tail
 assert not terminal['functional_or_timing_settings_changed']
 assert (root/'clock.sdc').read_text()==fit.SDC and '15.000'in fit.SDC
 log=(root/'audit.log').read_text();assert set(re.findall(r'^READY_CORNER (\S+)$',log,re.M))==CORNERS
 assert all(int(n)==17 for c,n in re.findall(r'^READY_REGISTERS (\S+) (\d+)$',log,re.M))
 names=re.findall(r'^READY_REGISTER (.+)$',log,re.M)
 expected_names={f'msa_ddram:u_msa|dorado_storage_group:u_storage_{i}|ready_q'for i in range(16)}|{'msa_ddram:u_msa|dorado_storage_group:u_storage_14|ready_q~DUPLICATE'}
 assert len(names)==68 and set(names)==expected_names and all(n==4 for n in collections.Counter(names).values()),'Ready bit/copy coverage differs'
 paths=[];min_pulse=[]
 for c in sorted(CORNERS):
  assert all(v==[0,0]for v in ucp((root/('ready-ucp-'+c+'.rpt')).read_text()).values()),'Unconstrained paths'
  clocks=(root/('ready-clocks-'+c+'.rpt')).read_text();assert re.search(r'; host_clock\s*; Base\s*; 15.000\s*;',clocks)
  for prefix in ['','predict-','gate-','step-']:
   for mode in ['setup','hold']:paths.append(path_report(root/('ready-'+prefix+mode+'-'+c+'.rpt')))
  for mode in ['recovery','removal']:assert 'Nothing to report.'in (root/('ready-'+mode+'-'+c+'.rpt')).read_text()
  t=(root/('ready-mpw-'+c+'.rpt')).read_text();m=re.search(r'Worst case slack is (-?[0-9.]+)',t);assert m and float(m[1])>=0;min_pulse.append(float(m[1]))
 summary=re.findall(r'Type\s*:\s*([^\n]+)\nSlack\s*:\s*([-+0-9.]+)',(root/'ready_fit.sta.summary').read_text());assert len(summary)==12 and all(float(v)>=0 for _,v in summary)
 warnings={}
 for name,allowed in [('map',{12473,10036,12011,12241,13024}),('fit',{12473,292013}),('sta',{12473}),('audit',{12473})]:
  t=(root/(name+'.log')).read_text();assert 'Critical Warning'not in t and not re.search(r'^Error',t,re.M)
  w=re.findall(r'^Warning \((\d+)\): (.+)$',t,re.M);assert {int(n)for n,_ in w}==allowed,(name,w);warnings[name]=w
 m=(root/'map.log').read_text();missing=re.findall(r'Net "msa_ddram:u_msa\|([^\"]+)" is missing source',m)
 assert set(missing)=={'msa01_sil_pl_2','msa02_sil_pl_7'} and len(missing)==2
 board=load_board(str(ROOT/'chm/sil/msa-Rev-Bg.dm!1_/msa.wl'));refs={}
 for name,pkg in [('msa01.sil+2','l02'),('msa02.sil+7','l25')]:
  net=board.nets[name];assert {(x['pkg'],x['pin'],x['dir'])for x in net['pins']}=={(pkg,n,'in')for n in [1,2,6,10]}
  assert board.packages[pkg]['type']=='MC10125';refs[name]=net
 unused=re.findall(r'^Warning \(10036\):.*object "([^\"]+)"',m,re.M)
 assert set(unused)=={'_unused_pins','GND192','GND58','VTT14','VTT8','ready_reference'} and unused.count('ready_reference')==2
 assert '97 hierarchies have connectivity warnings'in m
 connectivity=set(re.findall(r'Port Connectivity Checks: "([^"]+)"',(root/'ready_fit.map.rpt').read_text()))
 parent_msa=(parent/'inputs/msa_ddram.v').read_text();old_cells={f'msa_ddram:u_msa|cell_{typ}:u_{pkg}'for typ,pkg in re.findall(r'cell_(\w+)(?: #\([^\n]*\))? u_(\w+) \(',parent_msa)}
 assert len(connectivity)==117 and connectivity<=old_cells,'New connectivity warning outside unchanged original cells'
 stuck=re.findall(r'Pin "result\[(\d+)\]" is stuck at GND',m);expected_pad=set()
 for width,g in re.findall(r'dorado_storage_group #\(\.ADDR_BITS\(16\), \.WIDTH\((\d+)\)\) u_storage_(\d+)',(root/'msa_ddram.v').read_text()):expected_pad.update(35+16*int(g)+b for b in range(int(width),16))
 assert len(stuck)==len(set(stuck))==112 and {int(n)for n in stuck}==expected_pad,'Unexpected constant output'
 return dict(status='PASS_ONE_MSA_POST_EDGE_READY_15NS_COMPONENT',scope=__doc__,source_files=fp['files'],component_provenance_sha256=sha((component/'provenance.json').read_bytes()),group_provenance_sha256=sha((groups/'provenance.json').read_bytes()),timing=paths,summary=summary,minimum_setup_ns=min(x['slack_ns']for x in paths if '-setup-'in x['report']),minimum_hold_ns=min(x['slack_ns']for x in paths if '-hold-'in x['report']),minimum_pulse_ns=min(min_pulse),ready_logical_bits=16,ready_physical_registers=17,ready_copy_group=14,classified_warnings=warnings,original_VBB_reference_nets=refs,constant_output_padding_bits=112,unconstrained_paths=0,period_ns=15,native_qualified=False,hardware_qualified=False,hardware_speedup_qualified=False)
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__)
 for n in ['root','component','groups','output']:p.add_argument('--'+n,type=Path,required=True)
 a=p.parse_args();r=review(a.root.resolve(),a.component.resolve(),a.groups.resolve());a.output.write_text(json.dumps(r,indent=2)+'\n');print(r['status'],r['minimum_setup_ns'],r['minimum_hold_ns'])
