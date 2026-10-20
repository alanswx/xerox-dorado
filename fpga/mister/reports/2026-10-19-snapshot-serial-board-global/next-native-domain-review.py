import hashlib,json
from collections import Counter,defaultdict
from pathlib import Path
ROOT=Path('/Users/alans/dev2/Dorado');B=ROOT/'build/rtl-executive';I=B/'enabled60-support-inventory-v3-20261019'
provenance=json.loads((I/'provenance.json').read_text());build=Path(provenance['build'])
for name,digest in provenance['sources'].items():assert hashlib.sha256((build/name).read_bytes()).hexdigest()==digest,'original support source changed:'+name
inventory=json.loads((I/'machine-support-review.json').read_text())
seq=[r for r in inventory['candidates']if r['category']=='sequential']
assert sum(r['bits']for r in seq)==154037
msa=[r for r in seq if r['path'].startswith('dorado_native_machine.m.b_msa')]
by_instance=Counter();by_source=defaultdict(Counter)
for row in msa:
 instance='.'.join(row['path'].split('.')[:3]);by_instance[instance]+=row['bits']
 by_source[Path(row['source']).name]['bits']+=row['bits'];by_source[Path(row['source']).name]['fields']+=1
assert len(by_instance)==8 and set(by_instance.values())=={2690}
report=dict(status='REVIEW_ONLY_SOURCE_REVALIDATED',scope='Not classification/access/hold implementation; candidates must not be silently excluded.',
 support_xml_sha256=inventory['xml_sha256'],source_files_verified=len(provenance['sources']),support_candidate_bits=154037,
 msa_candidate_bits=sum(by_instance.values()),msa_instances=dict(by_instance),msa_sequential_sources={k:dict(v)for k,v in by_source.items()},
 groups={k:dict(bits=v['candidate_bits'].get('sequential',0),fields=v['candidate_counts'].get('sequential',0))for k,v in inventory['groups'].items()if v['candidate_bits'].get('sequential',0)},
 next=['Eight MSA storage-board instances are outside the eleven main-board adapters and require explicit state classification/access/hold, including original latches/address histories and storage request/front-end state.',
 'Classify disk buffer/host transfer, raw controller/arbiter, drive/pixel/keyboard/terminal/wrapper state; add top-level input/video/network and16MiBrawRAM separately.',
 'SDRAM refresh must continue while guest state is held; quiesce/capture the logical memory interface rather than blindly stopping the physical controller.',
 'Bind SYSTEM reset to a genuine machine reseed/lifecycle event; parser reset or a soft reset that leaves shifted guest state cannot release quarantine.'])
p=B/'native-hold-domain-review-20261019.json';p.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(dict(status=report['status'],source_files_verified=report['source_files_verified'],support_candidate_bits=154037,msa_instances=8,msa_candidate_bits=21520)))
