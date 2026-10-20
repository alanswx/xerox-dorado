#!/usr/bin/env python3
"""New board-only ABI2 schema for actual 1650-bit compact MSA members.

Unit stride remains256 addresses; words0..91 are mapped, 92..255 reject. Dense
payload has no per-member padding. All original eleven board maps are retained.
"""
import argparse
import hashlib
import json
from pathlib import Path
import serial_state_format_msa as prior

old=prior.old;native=prior.native;ROOT=old.ROOT
FORMAT='dorado-serial-boards-msa-compact-v1'


def msa_runs():
    return [row for unit in range(8)for row in (
        dict(word=unit<<8,count=91,width=18,offset=unit*1650),
        dict(word=(unit<<8)+91,count=1,width=12,offset=unit*1650+1638))]


def build_catalog(reports):
    base=old.build_catalog(reports);root=reports/'2026-10-19-snapshot-msa-compact'
    manifest,msha=old.receipt(root,'msa-v2/manifest.json');result,rsha=old.receipt(root,'msa-v2/results.json')
    classification,csha=old.receipt(root,'classification-v1/classification.json')
    audit,asha=old.receipt(root,'msa-compact-source-audit-v1-20261019.json')
    fit,fsha=old.receipt(root,'msa-compact-fit-review-v1-20261019.json')
    if (result['status'],manifest['logical_state_bits'],manifest['ff_bits'],manifest['frontend_bits'],
        classification['status'],classification['totals']['PHYSICAL DUT'],audit['status'],audit['actual_total_bits'],fit['status'])!=(
        'PASS',1650,866,784,'PASS_ALL_COMPACT_AND_REFERENCE_STATE',1650,'PASS_COMPACT_SOURCE_SCOPE',1650,'PASS_MATCHED_COMPONENT_SAVING'):
        raise ValueError('unqualified compact MSA receipt')
    if manifest['scan']!=dict(state_bits=1650,words=92,word_bits=18,last_word_bits=12,completion_edges=1650):
        raise ValueError('qualified compact MSA geometry differs')
    board=dict(id=11,name='MSA',index_bits=11,bits=13200,words=736,runs=msa_runs(),
        members=list(native.INSTANCES),member_bits=1650,member_stride=256,
        receipt=dict(directory=root.name,version='msa-v2',manifest_sha256=msha,results_sha256=rsha,
                     classification_sha256=csha,source_audit_sha256=asha,matched_fit_sha256=fsha))
    catalog=dict(format=FORMAT,abi=2,scope='boards-only',encoding='dense-lsb0',response_limit=4096,
        service_domains=5,board_bits=1957944,boards=[*base['boards'],board],predecessor_catalog_sha256=base['catalog_sha256'])
    digest=hashlib.sha256(old.canonical(catalog)).hexdigest();catalog.update(catalog_sha256=digest,schema_id=digest[:16])
    validate_catalog(catalog);return catalog


def validate_catalog(catalog):
    payload={k:v for k,v in catalog.items()if k not in ('catalog_sha256','schema_id')}
    digest=hashlib.sha256(old.canonical(payload)).hexdigest()
    if catalog.get('catalog_sha256')!=digest or catalog.get('schema_id')!=digest[:16]:raise ValueError('catalog fingerprint differs')
    if (catalog['format'],catalog['abi'],catalog['scope'],catalog['encoding'],catalog['response_limit'],catalog['service_domains'])!=(
        FORMAT,2,'boards-only','dense-lsb0',4096,5):raise ValueError('unsupported compact catalog/transport')
    if len(catalog['boards'])!=12:raise ValueError('twelve slots required')
    for identity,(board,spec)in enumerate(zip(catalog['boards'][:11],old.SPECS)):
        if (board['id'],board['name'],board['index_bits'],board['bits'])!=(identity,*spec[:3]):raise ValueError('main-board identity differs')
        old.validate_runs(board['runs'],board['bits'],board['index_bits'])
        if board['words']!=sum(r['count']for r in board['runs']):raise ValueError('main-board words differ')
    board=catalog['boards'][11]
    if (board['id'],board['name'],board['index_bits'],board['bits'],board['words'],board['members'],board['member_bits'],board['member_stride'])!=(
        11,'MSA',11,13200,736,list(native.INSTANCES),1650,256):raise ValueError('compact member identity/geometry differs')
    if board['runs']!=msa_runs():raise ValueError('compact indexed map/padding differs')
    old.validate_runs(board['runs'],board['bits'],board['index_bits'])
    if catalog['board_bits']!=1957944 or catalog['board_bits']!=sum(b['bits']for b in catalog['boards']):raise ValueError('physical total differs')


class SerialStateFormat(prior.SerialStateFormat):
    __slots__=()

    def __init__(self,catalog):
        validate_catalog(catalog);self._tag=int(catalog['schema_id'],16)
        if not self._tag:raise ValueError('zero schema tag')
        self._prefix=(0xb04d,2,*[(self._tag>>(16*i))&0xffff for i in range(4)])
        self.catalog_sha256=catalog['catalog_sha256'];boards=[]
        for board in catalog['boards']:
            runs=tuple(sorted((r['word'],r['count'],r['width'])for r in board['runs']))
            boards.append((tuple(r[0]for r in runs),runs))
        self._boards=tuple(boards)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--reports',type=Path,default=ROOT/'fpga/mister/reports')
    p.add_argument('--output',type=Path,required=True);a=p.parse_args();catalog=build_catalog(a.reports.resolve())
    a.output.write_text(json.dumps(catalog,indent=2)+'\n');print(catalog['schema_id'],catalog['board_bits'])
