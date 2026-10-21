#!/usr/bin/env python3
"""Match retained FPGA error pins to the original ROM's breakpoint table.

doradoboot.masm's BootBlocks include a count and big-endian address table.
SendAHunk deliberately selects bad parity for BOTH halves at those addresses.
This is static ROM/source inspection; no emulated CPU executes or imports state.
"""
import argparse
import hashlib
import json
from pathlib import Path


def inspect(dump, observation):
    rom={int(a,16):int(v,16) for a,v in (line.split() for line in dump.read_text().splitlines())}
    assert set(rom)==set(range(0xc000,0x10000)) and all(0<=v<256 for v in rom.values())
    word=lambda address:rom[address]|rom[address+1]<<8
    blocks=[]
    for header in [0xc000,0xc00b]:
        data,origin,hunks=word(header),word(header+2),word(header+4)
        count,table=rom[header+8],word(header+9)
        assert origin+hunks*4<=4096 and data+hunks*17<=65536
        addresses=[rom[table+2*i]<<8|rom[table+2*i+1] for i in range(count)]
        assert len(addresses)==len(set(addresses)) and addresses==sorted(addresses,reverse=True)
        assert all(origin<=a<origin+hunks*4 for a in addresses)
        blocks.append(dict(header=header,data=data,origin=origin,hunks=hunks,
                           breakpoint_table=table,breakpoints=addresses))
    captured=json.loads(observation.read_text())
    assert captured['capture_valid'] and captured['armed'] and captured['selector_restored']
    assert all(captured['first_sources']['enables'].values())
    flags=captured['first_sources']['raw_error_flags']
    assert flags['IM_LH'] and flags['IM_RH']
    assert not any(flags[n] for n in ['IO','RM_STK_T','MEMORY','MD'])
    cia=captured['first_CIA']
    matches=[b for b in blocks if cia in b['breakpoints']]
    assert len(matches)==1, 'First error address is not a declared ROM breakpoint'
    block=matches[0]
    index=cia-block['origin'];hunk,lane=divmod(index,4)
    position=block['data']+hunk*17+1+lane*4
    left=rom[position]<<8|rom[position+1]
    right=rom[position+2]<<8|rom[position+3]
    ff,jcn=right>>8,right&255
    assert jcn>>4==0, 'This proof expects an unconditional long branch'
    destination=ff<<4|jcn&15
    current=captured['pages'][-1]['cpu']
    assert destination==(current>>12)&4095, 'Stopped CPU is not at the breakpoint branch destination'
    return dict(status='PASS_ORIGINAL_ROM_INTENTIONAL_BREAKPOINT_MATCH',scope=__doc__,
                blocks=blocks,first_error_CIA=cia,raw_left=left,raw_right=right,
                FF=ff,JCN=jcn,branch_destination=destination,stopped_processor=current,
                inferred_source_label='InitialMain.mc MicrocodeBootFailed: Breakpoint, Branch[BootEmulator]',
                limitation='Proves deliberate failed-boot stop; does not distinguish packet acceptance, image checksum or raw-memory corruption',
                inputs={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in [dump,observation,Path(__file__)]})


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    for name in ['rom-dump','observation','output']:
        parser.add_argument('--'+name,type=Path,required=True)
    a=parser.parse_args();assert not a.output.exists()
    result=inspect(a.rom_dump.resolve(),a.observation.resolve())
    a.output.write_text(json.dumps(result,indent=2)+'\n')
    print(result['status'],'CIA',hex(result['first_error_CIA']),'->',hex(result['branch_destination']))
