#!/usr/bin/env python3
"""Classify the exact enabled-native MSA sequential inventory without exclusions.

All eight boards' original FF/strobe history and logical frontend state require
save/restore. This records required state, not implemented board/frontend access,
quiesce, a native integration, external SDRAM contents or a complete snapshot.
"""
import argparse
import copy
from collections import Counter
import hashlib
import json
from pathlib import Path
import sys
import generate_msa_ff as gen
from generate_serial_boards import raw_receipt

sys.path.insert(0,str(gen.ROOT/'tools'))
from sil_state_inventory import inventory

INSTANCES=('b_msa','b_msaOdd','b_msa1','b_msa1Odd','b_msa2','b_msa2Odd','b_msa3','b_msa3Odd')
PACKAGES={
 'cell_MC10141': 'a01 a26 l01 l26'.split(),
 'cell_MC10176': 'b01 b26 c01 c26 e02 f01 f26 h02 j01 j26 k01 k26'.split(),
 'cell_F10016': 'g01 g26'.split(),
 'cell_SN74166': 'a13 a14 b13 b14 c13 c14 d13 d14 e03 e24 f03 f24 g13 g14 h13 h14 i13 i14 j13 j14'.split(),
 'cell_SN74S174': 'a03 a04 a05 a22 a23 a24 b03 b24 c03 c24 d03 d24 g03 g24 h03 h24 i03 i24 j03 j04 j05 j22 j23 j24'.split(),
 'cell_MK4096P_addr': ['dram_addr_'+str(n)for n in range(16)],
}
FIELDS={
 'cell_MC10141': dict(q=4,d_pre=4,s_pre=2,dr_pre=1,dl_pre=1,ck_d=1),
 'cell_MC10176': dict(q=6,d_pre=6,ck_d=1),
 'cell_F10016': dict(q=4,setup_pre=6,ck_d=1),
 **{k:dict(gen.fields(k))for k in gen.FIELDS},
}


def expected_fields():
    fields={}
    for board in INSTANCES:
        prefix='dorado_native_machine.m.'+board+'.'
        for kind,packages in PACKAGES.items():
            for package in packages:
                for name,width in FIELDS[kind].items():
                    fields[prefix+'u_'+package+'.'+name]=dict(width=width,owner='ORIGINAL BOARD FF/HISTORY',source=kind+'.v')
        for group in range(16):
            width=12 if group in (0,1,14,15)else 8
            saved=dict(cache_addr=16,cache_data=16,cache_valid=1,cas_d=1,data_out=width,
                       old_data=16,read_pipe=width,saved_addr=16,saved_data=16,saved_write=1,
                       waiting=1,we_d=1,write_data=width,write_done=1,write_pending=1)
            for name,bits in saved.items():
                owner='GUEST-CLOCK FRONTEND'if name in ('cas_d','we_d','write_pending','write_data','read_pipe','data_out')else'HOST-CLOCK FRONTEND'
                fields[prefix+f'u_storage_{group}.'+name]=dict(width=bits,owner=owner,source='dorado_storage_group.sv')
    return fields


def classify(data):
    expected=expected_fields();decisions=[]
    for row in data['candidates']:
        if row['category']!='sequential':continue
        if not any(row['path'].startswith('dorado_native_machine.m.'+board+'.')for board in INSTANCES):continue
        wanted=expected.pop(row['path'],None)
        if wanted is None:raise ValueError('extra/duplicate MSA state: '+row['path'])
        width=wanted['width']
        if (row['width'],row['bits'],row['unpacked_dimensions'],Path(row['source']).name)!=(width,width,[],wanted['source']):
            raise ValueError('MSA field source/geometry differs: '+row['path'])
        decisions.append(dict(path=row['path'],bits=width,decision='SAVE',owner=wanted['owner'],
                              access='PENDING MSA BOARD/FRONTEND INTEGRATION'))
    if expected:raise ValueError('missing MSA state: '+str(list(expected)[:3]))
    boards={}
    for board in INSTANCES:
        rows=[r for r in decisions if r['path'].startswith('dorado_native_machine.m.'+board+'.')]
        original=sum(r['bits']for r in rows if r['owner']=='ORIGINAL BOARD FF/HISTORY')
        frontend=sum(r['bits']for r in rows if r['owner']!='ORIGINAL BOARD FF/HISTORY')
        if(original,frontend,len(rows))!=(866,1824,458):raise ValueError('MSA aggregate geometry differs')
        boards[board]=dict(original_ff_history_bits=original,frontend_bits=frontend,required_bits=original+frontend,fields=len(rows))
    return dict(status='CLASSIFICATION_PASS_ACCESS_PENDING',scope=__doc__,required_bits=21520,
                sequential_exclusions=[],boards=boards,decisions=decisions,xml_sha256=data['xml_sha256'])


def controls(data):
    results=[]
    prefix='dorado_native_machine.m.b_msa.'
    for case in ('missing','extra','duplicate','width','dimensions','source','history_not_sequential','frontend_not_sequential','missing_odd_board'):
        broken=copy.deepcopy(data);rows=broken['candidates']
        target=next(r for r in rows if r['path']==prefix+'u_dram_addr_0.ras_d')
        if case=='missing':rows.remove(target)
        elif case in ('extra','duplicate'):
            added=copy.deepcopy(target)
            if case=='extra':added['path']+='_hidden'
            rows.append(added)
        elif case=='width':target['width']+=1
        elif case=='dimensions':target['unpacked_dimensions']=[[0,1]]
        elif case=='source':target['source']='unreviewed.v'
        elif case=='history_not_sequential':target['category']='initial_only_candidate'
        elif case=='frontend_not_sequential':
            next(r for r in rows if r['path']==prefix+'u_storage_0.old_data')['category']='initial_only_candidate'
        else:broken['candidates']=[r for r in rows if not r['path'].startswith('dorado_native_machine.m.b_msa3Odd.')]
        try:classify(broken)
        except ValueError as error:results.append(dict(case=case,rejected=True,diagnostic=str(error)))
        else:raise AssertionError('damaged native MSA classification accepted: '+case)
    return results


def run(reports,out):
    out.mkdir(parents=True,exist_ok=False)
    receipt_root=reports/'2026-10-19-snapshot-serial-board-global'
    provenance=json.loads(raw_receipt(receipt_root,'native-support-review/provenance.json'))
    verified={}
    for name,digest in provenance['sources'].items():
        raw=raw_receipt(receipt_root,'native-support-source/'+name)
        if hashlib.sha256(raw).hexdigest()!=digest:raise ValueError('native source receipt differs: '+name)
        verified[name]=digest
        if Path(name).stem in gen.SHA:
            if digest!=gen.SHA[Path(name).stem]:raise ValueError('new primitive differs from exact native source')
    old=reports/'2026-10-19-snapshot-enabled-capacity'
    raw=raw_receipt(old,'machine-support-v3/native-machine.xml')
    support=json.loads(raw_receipt(receipt_root,'native-support-review/machine-support-review.json'))
    if hashlib.sha256(raw).hexdigest()!=support['xml_sha256']:raise ValueError('native XML identity differs')
    full=inventory(raw)
    # Compare the recomputed full XML's MSA candidates with the archived review.
    select=lambda r:any(r['path'].startswith('dorado_native_machine.m.'+b+'.')for b in INSTANCES)
    if [r for r in full['candidates']if select(r)]!=[r for r in support['candidates']if select(r)]:
        raise ValueError('native MSA inventory replay differs')
    result=classify(full);result['failure_controls']=controls(full)
    result['verified_native_sources']=verified
    result['new_primitive_coverage']=dict(bits_per_board=636,total_bits=5088,instances=8,
        remaining='230 original bits per board reuse existing adapters; all 1824 frontend bits and board integration still pending')
    result['barrier_requirements']=[
        'Freeze original FF data and all edge histories together.',
        'Drain external memory handshakes before final frontend hold; preserve write_pending/write_done/old_data and read pipelines.',
        'Quiesce/capture the raw arbiter interface while physical SDRAM refresh continues.',
        'Include external 16MiB data/ECC memory and other backend state in the machine bundle.',
        'Frontend host and guest clock processes need separate faithful advance/hold treatment; a guest clock stop alone is insufficient.',
    ]
    rows=[r for r in full['candidates']if select(r)]
    counts=Counter(r['category']for r in rows);bits=Counter()
    for row in rows:bits[row['category']]+=row['bits']
    scoped=dict(full,candidates=rows,candidate_counts=dict(counts),candidate_bits=dict(bits),
                scope='Only the eight exact native MSA hierarchies; parent/native backend/external RAM excluded',
                instances=len({r['path'].rsplit('.',1)[0]for r in rows}))
    for name,value in [('classification.json',result),('inventory.json',scoped)]:
        (out/name).write_text(json.dumps(value,indent=2)+'\n')
    print(json.dumps(dict(status=result['status'],required_bits=21520,boards=8,fields=len(result['decisions']),
                         source_receipts=len(verified),failure_controls=len(result['failure_controls']))),flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--reports',type=Path,default=gen.ROOT/'fpga/mister/reports');p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();run(a.reports.resolve(),a.output.resolve())
