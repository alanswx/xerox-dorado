#!/usr/bin/env python3
"""Versioned compact board-state format from pinned, qualified address maps.

Offline encoding and packet formatting only. A catalog is not a complete
machine snapshot or a substitute for a matching build/disk/service bundle.
"""
import argparse
from bisect import bisect_right
from functools import lru_cache
import gzip
import hashlib
import json
from pathlib import Path

ROOT=Path(__file__).resolve().parents[3]
SPECS=[
 ('ContA',12,1353,'conta','simulation-v7'),
 ('ContB',15,147809,'contb','simulation-v1'),
 ('ProcH',16,5712,'processor','ProcH/simulation'),
 ('ProcL',16,5581,'processor','ProcL/simulation'),
 ('IFU',18,28653,'ifu','simulation-v1'),
 ('MemC',16,23718,'memc','simulation-v4'),
 ('MemD',20,296591,'memd','simulation-v2'),
 ('MemX',24,1378312,'memx','simulation-v1'),
 ('DskEth',13,2656,'dsketh','simulation-v1'),
 ('DispY',19,13993,'dispy','simulation-v3'),
 ('BaseBd',17,40366,'baseboard','simulation-v4')]


def canonical(value):
    return json.dumps(value,sort_keys=True,separators=(',',':')).encode()


@lru_cache(maxsize=16)
def receipt_index(root):
    p=root/'archive-manifest.json';p=p if p.exists() else p.with_suffix('.json.gz')
    raw=gzip.decompress(p.read_bytes()) if p.suffix=='.gz' else p.read_bytes()
    rows=json.loads(raw);result={r['raw_name']:r for r in rows}
    if len(result)!=len(rows):raise ValueError('duplicate receipt member')
    return result


def receipt(root,name):
    row=receipt_index(root)[name];p=root/row['stored']
    raw=gzip.decompress(p.read_bytes()) if p.suffix=='.gz' else p.read_bytes()
    if len(raw)!=row['raw_bytes'] or hashlib.sha256(raw).hexdigest()!=row['raw_sha256']:
        raise ValueError('qualified receipt bytes differ: '+name)
    return json.loads(raw),row['raw_sha256']


def runs_from_words(words,bits,index_bits):
    runs=[];cursor=0
    for word in words:
        address,width,offset=(word[k] for k in ('word','width','offset'))
        if any(type(x) is not int for x in (address,width,offset)) or not(0<=address<1<<index_bits and 1<=width<=18 and offset==cursor):
            raise ValueError('invalid/noncontiguous physical word map')
        if runs and runs[-1]['width']==width and runs[-1]['word']+runs[-1]['count']==address:
            runs[-1]['count']+=1
        else:runs.append(dict(word=address,count=1,width=width,offset=offset))
        cursor+=width
    if cursor!=bits:raise ValueError('incomplete physical state map')
    validate_runs(runs,bits,index_bits)
    return runs


def validate_runs(runs,bits,index_bits):
    cursor=0;intervals=[]
    for row in runs:
        if set(row)!={'word','count','width','offset'} or any(type(v) is not int for v in row.values()):
            raise ValueError('invalid run schema')
        word,count,width,offset=(row[k] for k in ('word','count','width','offset'))
        if not(count>0 and 1<=width<=18 and word>=0 and word+count<=1<<index_bits and offset==cursor):
            raise ValueError('invalid run geometry/physical coverage')
        cursor+=count*width;intervals.append((word,word+count))
    if cursor!=bits:raise ValueError('incomplete physical coverage')
    intervals.sort()
    if any(b[0]<a[1] for a,b in zip(intervals,intervals[1:])):raise ValueError('aliased transport address')


def complete_words(name,manifest,metadata):
    """Expand RAMs tested by separate physical/oracle buses, not flat probes."""
    if name=='ContB':
        rows=[dict(word=r['index'],width=r['width'],offset=r['lsb']) for r in manifest['scan']['word_map']]
    elif name=='IFU':rows=metadata['ff_rows']
    else:rows=metadata['state_words']
    cursor=0
    for row in rows:
        if row['offset']!=cursor:raise ValueError('qualified flat-probe layout differs')
        yield row;cursor+=row['width']
    banks=[]
    if name=='ContB':
        if len(manifest['groups'])!=8 or any(len(g['bits'])!=18 for g in manifest['groups']):raise ValueError('independent ContB IM groups differ')
        banks=[dict(word=g['index']<<11,width=18,count=1025) for g in manifest['groups']]
    elif name=='IFU':
        banks=[dict(word=g['word'],width=g['width'],count=g['ram_words']+1) for g in manifest['banks']]
        if any(g['pipeline_word']!=g['ram_words'] for g in manifest['banks']):raise ValueError('IFU raw pipeline address differs')
    elif name=='MemD':
        banks=[dict(word=g['word'],width=g['width'],count=g['ram_words']+1) for g in manifest['banks']]
        if any(g['pipeline_word']!=g['ram_words'] for g in manifest['banks']):raise ValueError('cache raw pipeline address differs')
    elif name=='MemX':
        banks=[dict(word=g['word'],width=g['width'],count=g['words']) for g in manifest['rams']]
        if len(banks)!=21 or any(g['count']!=65536 or g['width']!=1 for g in banks):raise ValueError('independent map planes differ')
    for bank in banks:
        for row in range(bank['count']):
            yield dict(word=bank['word']+row,width=bank['width'],offset=cursor);cursor+=bank['width']


def build_catalog(reports):
    boards=[]
    for identity,(name,index_bits,bits,folder,version) in enumerate(SPECS):
        root=reports/('2026-10-19-snapshot-'+folder+'-ff-scan')
        manifest,manifest_sha=receipt(root,version+'/manifest.json')
        result,result_sha=receipt(root,version+'/results.json')
        if result['status']!='PASS' or manifest['logical_state_bits']!=bits:raise ValueError('unqualified board receipt')
        if name=='ContB':
            metadata={};metadata_sha=manifest_sha
        else:
            metadata,metadata_sha=receipt(root,version+'/fixture-metadata.json')
        first=[]
        def checked_words():
            for i,row in enumerate(complete_words(name,manifest,metadata)):
                if i<manifest['scan']['words']:first.append((row['word'],row['width']))
                yield row
        runs=runs_from_words(checked_words(),bits,index_bits)
        scan=manifest['scan'];base=scan.get('base_word',0)
        expected=[(base+i,min(18,scan['state_bits']-18*i)) for i in range(scan['words'])]
        if first!=expected:raise ValueError('compact FF map differs')
        boards.append(dict(id=identity,name=name,index_bits=index_bits,bits=bits,
            words=sum(r['count'] for r in runs),runs=runs,
            receipt=dict(directory=root.name,version=version,manifest_sha256=manifest_sha,
                         results_sha256=result_sha,metadata_sha256=metadata_sha)))
    catalog=dict(format='dorado-serial-boards-v1',abi=2,scope='boards-only',encoding='dense-lsb0',
        response_limit=2048,service_domains=5,board_bits=sum(b['bits'] for b in boards),boards=boards)
    if catalog['board_bits']!=1944744:raise ValueError('complete board inventory differs')
    digest=hashlib.sha256(canonical(catalog)).hexdigest()
    catalog.update(catalog_sha256=digest,schema_id=digest[:16])
    validate_catalog(catalog)
    return catalog


def validate_catalog(catalog):
    payload={k:v for k,v in catalog.items() if k not in ('catalog_sha256','schema_id')}
    digest=hashlib.sha256(canonical(payload)).hexdigest()
    if catalog.get('catalog_sha256')!=digest or catalog.get('schema_id')!=digest[:16]:raise ValueError('catalog fingerprint differs')
    if (catalog['format'],catalog['abi'],catalog['scope'],catalog['encoding'],catalog['response_limit'],catalog['service_domains'])!=(
        'dorado-serial-boards-v1',2,'boards-only','dense-lsb0',2048,5):raise ValueError('unsupported format/transport scope')
    if len(catalog['boards'])!=11:raise ValueError('complete eleven-board catalog required')
    for identity,(board,spec) in enumerate(zip(catalog['boards'],SPECS)):
        if (board['id'],board['name'],board['index_bits'],board['bits'])!=(identity,*spec[:3]):raise ValueError('board identity/geometry differs')
        validate_runs(board['runs'],board['bits'],board['index_bits'])
        if board['words']!=sum(r['count'] for r in board['runs']):raise ValueError('board word count differs')
    if catalog['board_bits']!=sum(b['bits'] for b in catalog['boards']):raise ValueError('board total differs')


def iter_words(board):
    for run in board['runs']:
        for n in range(run['count']):yield run['word']+n,run['width'],run['offset']+n*run['width']


def encode_board(board,words):
    image=bytearray((board['bits']+7)//8);values=iter(words)
    for index,width,offset in iter_words(board):
        try:address,value=next(values)
        except StopIteration:raise ValueError('incomplete board image') from None
        if address!=index or type(value) is not int or not(0<=value<1<<width):raise ValueError('word order/value/padding differs')
        first,shift=divmod(offset,8);count=(width+shift+7)//8
        raw=int.from_bytes(image[first:first+count],'little')|(value<<shift)
        image[first:first+count]=raw.to_bytes(count,'little')
    try:next(values)
    except StopIteration:return bytes(image)
    raise ValueError('extra board words')


def decode_board(board,image):
    if len(image)!=(board['bits']+7)//8 or (board['bits']%8 and image[-1]>>(board['bits']%8)):
        raise ValueError('image length/padding differs')
    for index,width,offset in iter_words(board):
        first,shift=divmod(offset,8);count=(width+shift+7)//8
        yield index,(int.from_bytes(image[first:first+count],'little')>>shift)&((1<<width)-1)


class SerialStateFormat:
    """Validate once; immutable copied runs make per-word formatting bounded."""
    __slots__=('_tag','_prefix','_boards','catalog_sha256')

    def __init__(self,catalog):
        validate_catalog(catalog)
        self._tag=int(catalog['schema_id'],16)
        if not self._tag:raise ValueError('zero schema tag')
        self._prefix=(0xb04d,2,*[(self._tag>>(16*i))&0xffff for i in range(4)])
        self.catalog_sha256=catalog['catalog_sha256']
        self._boards=[]
        for board in catalog['boards']:
            runs=tuple(sorted((r['word'],r['count'],r['width']) for r in board['runs']))
            self._boards.append((tuple(r[0] for r in runs),runs))
        self._boards=tuple(self._boards)

    def control_words(self,hold):
        if type(hold) is not bool:raise ValueError('hold must be boolean')
        return [0x77,*self._prefix,int(hold)]

    def request_words(self,board,index,write=False,value=0):
        if type(board) is not int or not(0<=board<11) or type(index) is not int or type(write) is not bool:raise ValueError('invalid board/request')
        starts,runs=self._boards[board];selected=bisect_right(starts,index)-1
        if selected<0:raise ValueError('unmapped address')
        start,count,width=runs[selected]
        if index>=start+count or type(value) is not int or not(0<=value<1<<width) or (not write and value):raise ValueError('unmapped address/value/padding')
        return [0x78,*self._prefix,int(write),board,index&0xffff,index>>16,value&0xffff,value>>16]

    def parse_status(self,words):
        if len(words)!=18 or any(type(w) is not int or not(0<=w<=0xffff) for w in words):raise ValueError('invalid status framing')
        tag=sum(words[12+i]<<(16*i) for i in range(4))
        if (words[0],words[1],words[3],tag,words[17])!=(0xd0b0,2,11,self._tag,2048):raise ValueError('status capability/schema mismatch')
        if not(words[2]&0x100) or words[2]>>11 or words[4]>>11 or words[8]>>2 or words[9]>6 or words[16]>>5:
            raise ValueError('unsupported status scope/padding')
        return dict(hold=bool(words[2]&1),busy=bool(words[2]&8),complete=bool(words[2]&16),
            fault=bool(words[2]&64),ready=bool(words[2]&128),frozen=words[4],
            sequence=words[5]|words[6]<<16,result=words[7]|words[8]<<16,error=words[9],
            control_errors=words[10]|words[11]<<16,services_held=words[16])


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--reports',type=Path,default=ROOT/'fpga/mister/reports');p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();catalog=build_catalog(a.reports.resolve());a.output.write_text(json.dumps(catalog,indent=2)+'\n')
    print('Serial board catalog',catalog['schema_id'],'bits',catalog['board_bits'],'runs',sum(len(b['runs']) for b in catalog['boards']))
