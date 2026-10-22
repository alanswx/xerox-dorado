#!/usr/bin/env python3
"""Review exact same-edge board reduction, functional proofs and15ns fit.

One MSA with registered virtual services only; no native/SDRAM-I/O/firmware
qualification or assembly permission follows from this component review.
"""
import argparse,collections,contextlib,hashlib,io,json,re,tempfile
from pathlib import Path
import fit_storage_board_ready as fit
from prepare_storage_board_ready import board_reduction,group_outputs
from review_storage_ready_prediction import CORNERS,path_report,ucp

def sha(b):return hashlib.sha256(b).hexdigest()
def js(p):return json.loads(p.read_text())
def review(root,component,groups,parent_component,parent_groups,parent_fit,parent_qualification):
 p=js(component/'provenance.json');s=js(component/'run-state.json')
 assert s['status']=='PASS_MSA_SAME_EDGE_BOARD_READY_REDUCTION'and len(s['actions'])==8
 for a in s['actions']:assert a['returncode']!=0 if a['stage']in ['stale-child-test','ignore-step-test','omit-group15-test']else a['returncode']==0
 for n,h in p['inputs'].items():assert sha((component/'inputs'/n).read_bytes())==h,n
 for n,fn in [('msa_ddram.v',board_reduction),('dorado_storage_group.sv',group_outputs)]:assert(component/'inputs'/n).read_bytes()==fn((parent_component/'inputs'/n).read_bytes())
 assert(component/'positive-raw.csv').read_bytes()==(parent_component/'positive-raw.csv').read_bytes()
 assert 'all 9437184 stored bits identical'in(component/'positive-test.log').read_text()
 gp=js(groups/'provenance.json');gs=js(groups/'run-state.json');assert gs['status']=='PASS_READY_GROUP_TWO_SIMULATORS'and len(gs['actions'])==12
 assert gp['component_provenance_sha256']==sha((component/'provenance.json').read_bytes())
 for n,h in gp['inputs'].items():assert sha((groups/'inputs'/n).read_bytes())==h,n
 assert(groups/'inputs/group.sv').read_bytes()==(component/'inputs/dorado_storage_group.sv').read_bytes()
 for a in gs['actions']:assert a['returncode']!=0 if a['stage'].endswith('-stale-test')else a['returncode']==0
 fp=js(root/'provenance.json');fs=js(root/'run-state.json');terminal=js(root/'terminal-input-check.json')
 assert fs['status']=='TERMINAL_BOARD_READY_TIMING_REVIEW_REQUIRED'and len(fs['actions'])==4 and all(a['returncode']==0 for a in fs['actions'])
 assert set(terminal['absent_pids'])=={fs['driver_pid']}|{a['pid']for a in fs['actions']}
 with tempfile.TemporaryDirectory(prefix='board-ready-review-')as t:
  expected=Path(t)/'source'
  with contextlib.redirect_stdout(io.StringIO()):fit.prepare(component,parent_component,parent_groups,parent_fit,parent_qualification,expected)
  assert js(expected/'provenance.json')==fp,'Component recipe differs'
 tail=b'\nset_global_assignment -name LAST_QUARTUS_VERSION "17.0.2 Lite Edition"'
 assert terminal['changed']==['ready_fit.qsf']and terminal['exact_settings_append'].encode()==tail and not terminal['functional_or_timing_settings_changed']
 for n,h in fp['files'].items():
  b=(root/n).read_bytes()
  if n=='ready_fit.qsf':assert b.endswith(tail);b=b[:-len(tail)]
  assert sha(b)==h,('Fitted input changed',n)
 assert terminal['prepared']==fp['files']and terminal['actual']=={n:sha((root/n).read_bytes())for n in fp['files']}
 log=(root/'audit.log').read_text();counts=re.findall(r'^READY_REGISTERS (\S+) (\d+)$',log,re.M);assert len(counts)==4 and {c for c,_ in counts}==CORNERS and all(n=='1'for _,n in counts)
 pruned=re.findall(r'^REDUNDANT_GROUP_READY (\S+) (\d+)$',log,re.M);assert len(pruned)==4 and {c for c,_ in pruned}==CORNERS and all(n=='0'for _,n in pruned)
 assert re.findall(r'^READY_REGISTER (.+)$',log,re.M)==['msa_ddram:u_msa|ready_q']*4
 paths=[]
 for c in sorted(CORNERS):
  assert all(v==[0,0]for v in ucp((root/('ready-ucp-'+c+'.rpt')).read_text()).values()),'Unconstrained paths'
  assert re.search(r'; host_clock\s*; Base\s*; 15.000\s*;',(root/('ready-clocks-'+c+'.rpt')).read_text())
  for prefix in ['','predict-','gate-','step-']:
   for mode in ['setup','hold']:paths.append(path_report(root/('ready-'+prefix+mode+'-'+c+'.rpt')))
  for mode in ['recovery','removal']:assert 'Nothing to report.'in(root/('ready-'+mode+'-'+c+'.rpt')).read_text()
  t=(root/('ready-mpw-'+c+'.rpt')).read_text();m=re.search(r'Worst case slack is (-?[0-9.]+)',t);assert m and float(m[1])>=0
 summary=re.findall(r'Type\s*:\s*([^\n]+)\nSlack\s*:\s*([-+0-9.]+)',(root/'ready_fit.sta.summary').read_text());assert len(summary)==12 and all(float(v)>=0 for _,v in summary)
 def normalized(row):
  code,t=row;t=re.sub(r'/home/alans/mister/Dorado_\w+_202610\d+','PROJECT',t);t=re.sub(r'(at [^ ()]+)\(\d+\)',r'\1(LINE)',t);t=re.sub(r' Line: \d+',' Line: LINE',t);return code,t
 for n in ['map','fit','sta','audit']:
  t=(root/(n+'.log')).read_text();assert 'Critical Warning'not in t and not re.search(r'^Error',t,re.M)
  old=collections.Counter(map(normalized,re.findall(r'^Warning \((\d+)\): (.+)$',(parent_fit/(n+'.log')).read_text(),re.M)))
  new=collections.Counter(map(normalized,re.findall(r'^Warning \((\d+)\): (.+)$',t,re.M)));assert old==new,('Warnings differ',n,old-new,new-old)
 return dict(status='PASS_ONE_MSA_SAME_EDGE_READY_REDUCTION_15NS',scope=__doc__,source_files=fp['files'],component_provenance_sha256=sha((component/'provenance.json').read_bytes()),groups_provenance_sha256=sha((groups/'provenance.json').read_bytes()),timing=paths,summary=summary,minimum_setup_ns=min(x['slack_ns']for x in paths if '-setup-'in x['report']),minimum_hold_ns=min(x['slack_ns']for x in paths if '-hold-'in x['report']),ready_registers=1,redundant_group_registers=0,assembly_permitted=False,native_qualified=False,hardware_speedup_qualified=False)

if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__)
 for n in ['root','component','groups','parent-component','parent-groups','parent-fit','parent-qualification','output']:p.add_argument('--'+n,type=Path,required=True)
 a=p.parse_args();r=review(a.root.resolve(),a.component.resolve(),a.groups.resolve(),a.parent_component.resolve(),a.parent_groups.resolve(),a.parent_fit.resolve(),a.parent_qualification.resolve());a.output.write_text(json.dumps(r,indent=2)+'\n');print(r['status'],r['minimum_setup_ns'],r['minimum_hold_ns'])
