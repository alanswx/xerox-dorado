#!/usr/bin/env python3
"""Read back a Manchester replay and verify every microcode reply against its EB.

Wire-only diagnostic: valid transmitted packets do not prove CPU receipt or
correct memory timing. HM chapter 11 supplies the bit cell and CRC polynomial;
InitialEther.mc supplies the MicrocodeBootReply fields and empty EOF packet.
"""
import argparse
import hashlib
import json
from pathlib import Path

from decode_xmt import decode, words
from eb_replies import crc16, eb_words


def check(stream,eb,sysper):
    transitions=[tuple(map(int,line.split()))for line in stream.read_text().splitlines()]
    assert transitions and all(len(row)==2 and row[1]in(0,1)for row in transitions)
    assert all(a[0]<b[0]for a,b in zip(transitions,transitions[1:]))
    cell=340/(30/sysper);groups=[[]]
    for row in transitions:
        if groups[-1] and row[0]-groups[-1][-1][0]>2*cell:groups.append([])
        groups[-1].append(row)
    replies=[];other=[];payload=[]
    for index,group in enumerate(groups):
        candidates=set()
        for polarity in(0,1):
            for skip in(False,True):
                packets=decode(group,cell,polarity,skip)
                if len(packets)!=1:continue
                bits=packets[0];wl,remainder=words(bits)
                if bits[0]==1 and remainder==0 and crc16(bits)==0:
                    candidates.add(tuple(wl[:-1]))
        assert len(candidates)==1,('Ambiguous or invalid frame CRC',index,len(candidates))
        wl=candidates.pop()
        if len(wl)<4 or wl[1]!=0o1000 or wl[3]!=0o265:
            other.append(dict(frame=index,words=len(wl)));continue
        assert len(wl)>=13 and wl[2]>=0o26 and wl[2]%2==0
        n=(wl[2]-0o26)//2
        assert len(wl)==13+n and wl[-1]==0xffff,(index,'Pup framing')
        assert wl[4]==1 and wl[5]==len(replies),(index,'sequence')
        assert wl[6]==wl[0]>>8 and wl[7:9]==(1,1)
        assert wl[9]==(wl[0]&255) and wl[10:12]==(0,4)
        assert not replies or replies[-1]['payload_words']>0,'Data after EOF'
        replies.append(dict(frame=index,sequence=wl[5],payload_words=n,
                            source_host=wl[0]&255,destination_host=wl[0]>>8))
        payload.extend(wl[12:-1])
    assert replies and replies[-1]['payload_words']==0,'Missing empty EOF'
    assert payload==eb_words(eb),'Wire payload differs from original EB'
    assert sum(payload)&0xffff==0,'Original image checksum does not sum to zero'
    return dict(scope=__doc__,status='PASS_WIRE_MICROCODE_IMAGE_AND_CRC',
                sysper=sysper,frames=len(groups),non_microcode_frames=other,replies=replies,
                payload_words=len(payload),image_sum16=0,
                inputs={str(p):hashlib.sha256(p.read_bytes()).hexdigest()for p in
                    [stream,eb,Path(__file__),Path(__file__).with_name('decode_xmt.py'),
                     Path(__file__).with_name('eb_replies.py')]},CPU_receipt_qualified=False)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('stream',type=Path);p.add_argument('eb',type=Path)
    p.add_argument('--sysper',type=int,default=4);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();result=check(a.stream,a.eb,a.sysper)
    with a.output.open('x')as f:json.dump(result,f,indent=2);f.write('\n')
    print(result['status'],result['frames'],'frames,',result['payload_words'],'original image words')
