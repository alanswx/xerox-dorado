#!/usr/bin/env python3
"""Board-only ABI2 catalog including the eight MSAs in indexed slot11.

The first eleven address spaces retain their qualified maps. MSA unit u uses
index (u<<8)|word, word0..149; dense payload offsets have no per-unit padding.
This is not a full machine bundle, native service barrier or disk pairing.
"""
import argparse
from bisect import bisect_right
import hashlib
import json
from pathlib import Path
import serial_state_format as old
import classify_native_msa as native

ROOT=old.ROOT
FORMAT='dorado-serial-boards-msa-v1'


def msa_runs():
    return [row for unit in range(8)for row in (
        dict(word=unit<<8,count=149,width=18,offset=unit*2690),
        dict(word=(unit<<8)+149,count=1,width=8,offset=unit*2690+2682))]


def build_catalog(reports):
    base=old.build_catalog(reports)
    root=reports/'2026-10-19-snapshot-msa-board'
    manifest,msha=old.receipt(root,'simulation-v4/manifest.json')
    result,rsha=old.receipt(root,'simulation-v4/results.json')
    classification,csha=old.receipt(root,'classification/classification.json')
    audit,asha=old.receipt(root,'source-audit.json')
    if(result['status'],manifest['logical_state_bits'],manifest['ff_bits'],manifest['frontend_bits'],
       classification['status'],classification['physical_bits_per_side'],audit['status'])!=(
       'PASS',2690,866,1824,'PASS',2690,'PASS_SOURCE_ONLY'):
        raise ValueError('unqualified MSA receipt')
    if manifest['scan']!=dict(state_bits=2690,words=150,word_bits=18,last_word_bits=8,completion_edges=2690):
        raise ValueError('qualified MSA scan geometry differs')
    board=dict(id=11,name='MSA',index_bits=11,bits=21520,words=1200,runs=msa_runs(),
        members=list(native.INSTANCES),member_bits=2690,member_stride=256,
        receipt=dict(directory=root.name,version='simulation-v4',manifest_sha256=msha,results_sha256=rsha,
                     classification_sha256=csha,source_audit_sha256=asha))
    catalog=dict(format=FORMAT,abi=2,scope='boards-only',encoding='dense-lsb0',response_limit=4096,
                 service_domains=5,board_bits=1966264,boards=[*base['boards'],board],
                 predecessor_catalog_sha256=base['catalog_sha256'])
    digest=hashlib.sha256(old.canonical(catalog)).hexdigest()
    catalog.update(catalog_sha256=digest,schema_id=digest[:16]);validate_catalog(catalog)
    return catalog


def validate_catalog(catalog):
    payload={k:v for k,v in catalog.items()if k not in('catalog_sha256','schema_id')}
    digest=hashlib.sha256(old.canonical(payload)).hexdigest()
    if catalog.get('catalog_sha256')!=digest or catalog.get('schema_id')!=digest[:16]:raise ValueError('catalog fingerprint differs')
    if(catalog['format'],catalog['abi'],catalog['scope'],catalog['encoding'],catalog['response_limit'],catalog['service_domains'])!=(
        FORMAT,2,'boards-only','dense-lsb0',4096,5):raise ValueError('unsupported MSA catalog/transport')
    if len(catalog['boards'])!=12:raise ValueError('twelve complete slots required')
    for identity,(board,spec)in enumerate(zip(catalog['boards'][:11],old.SPECS)):
        if(board['id'],board['name'],board['index_bits'],board['bits'])!=(identity,*spec[:3]):raise ValueError('main-board identity differs')
        old.validate_runs(board['runs'],board['bits'],board['index_bits'])
        if board['words']!=sum(r['count']for r in board['runs']):raise ValueError('main-board word count differs')
    board=catalog['boards'][11]
    if(board['id'],board['name'],board['index_bits'],board['bits'],board['words'],board['members'],board['member_bits'],board['member_stride'])!=(
        11,'MSA',11,21520,1200,list(native.INSTANCES),2690,256):raise ValueError('MSA member identity/geometry differs')
    if board['runs']!=msa_runs():raise ValueError('MSA indexed map/padding differs')
    old.validate_runs(board['runs'],board['bits'],board['index_bits'])
    if catalog['board_bits']!=1966264 or catalog['board_bits']!=sum(b['bits']for b in catalog['boards']):raise ValueError('complete physical bit total differs')


class SerialStateFormat:
    __slots__=('_tag','_prefix','_boards','catalog_sha256')

    def __init__(self,catalog):
        validate_catalog(catalog);self._tag=int(catalog['schema_id'],16)
        if not self._tag:raise ValueError('zero schema tag')
        self._prefix=(0xb04d,2,*[(self._tag>>(16*i))&0xffff for i in range(4)])
        self.catalog_sha256=catalog['catalog_sha256'];boards=[]
        for board in catalog['boards']:
            runs=tuple(sorted((r['word'],r['count'],r['width'])for r in board['runs']))
            boards.append((tuple(r[0]for r in runs),runs))
        self._boards=tuple(boards)

    def control_words(self,hold):
        if type(hold)is not bool:raise ValueError('hold must be boolean')
        return [0x77,*self._prefix,int(hold)]

    def request_words(self,board,index,write=False,value=0):
        if type(board)is not int or not(0<=board<12)or type(index)is not int or type(write)is not bool:raise ValueError('invalid slot/request')
        starts,runs=self._boards[board];selected=bisect_right(starts,index)-1
        if selected<0:raise ValueError('unmapped address')
        start,count,width=runs[selected]
        if index>=start+count or type(value)is not int or not(0<=value<1<<width)or(not write and value):raise ValueError('unmapped address/value/padding')
        return [0x78,*self._prefix,int(write),board,index&0xffff,index>>16,value&0xffff,value>>16]

    def parse_status(self,words):
        if len(words)!=18 or any(type(w)is not int or not(0<=w<=65535)for w in words):raise ValueError('invalid status framing')
        tag=sum(words[12+i]<<(16*i)for i in range(4))
        if(words[0],words[1],words[3],tag,words[17])!=(0xd0b0,2,12,self._tag,4096):raise ValueError('status capability/schema differs')
        if not(words[2]&0x100)or words[2]>>11 or words[4]>>12 or words[8]>>2 or words[9]>6 or words[16]>>5:
            raise ValueError('status scope/padding differs')
        return dict(hold=bool(words[2]&1),busy=bool(words[2]&8),complete=bool(words[2]&16),
                    fault=bool(words[2]&64),ready=bool(words[2]&128),frozen=words[4],sequence=words[5]|words[6]<<16,
                    result=words[7]|words[8]<<16,error=words[9],control_errors=words[10]|words[11]<<16,services_held=words[16])


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--reports',type=Path,default=ROOT/'fpga/mister/reports')
    p.add_argument('--output',type=Path,required=True);a=p.parse_args();catalog=build_catalog(a.reports.resolve())
    a.output.write_text(json.dumps(catalog,indent=2)+'\n');print(catalog['schema_id'],catalog['board_bits'],len(catalog['boards']))
