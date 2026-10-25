#!/usr/bin/env python3
"""Source-bind a native boot register/FIFO read without executing a guest CPU.

Decode current physical RM bytes and the retained receive FIFO. Match a whole
last packet/CRC/status before interpreting it. Consumed FIFO slots are stale;
this cannot establish how the first packet was handled or a complete RX history.
Initial.mb provides RM names only, with four runtime anchors; it is not the
embedded ROM's CIA symbol table. No writes to the running process or medium.
"""
import argparse
import json
from pathlib import Path
import re
import shutil
import subprocess

from review_late_hold_and_input_reset import retain
from sil_netlist import load_board
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1]


def crc_word(crc,word):
    for bit in range(15,-1,-1):
        feedback=((crc>>15)^(word>>bit))&1
        crc=((crc<<1)&65535)^(0x8005 if feedback else 0)
    return crc


def analyze(inspection,out):
    assert not out.exists();out.mkdir(parents=True)
    record=json.loads((inspection/'result.json').read_text());native=Path(record['source'])
    assert record['read_only_guest_inspection']
    assert f"Process {record['pid']} detached" in (inspection/'lldb.log').read_text()
    assert sha(native/'obj/native-cedar')==record['binary_sha256']
    assert sha(native/'obj/Vtb_native_cedar___024root.h')==record['header_sha256']
    banks=record['translated_front']['physical_9bit_banks'];masks={};bindings={}
    for half,board in [('h','ProcH-Rev-Ce'),('l','ProcL-Rev-Ci')]:
        name='chm/sil/'+board+'.dm!1_/'+board+'.wl'
        physical=load_board(str(ROOT/name))
        pins={(p['pkg'],p['pin']):n for n,row in physical.nets.items()for p in row['pins']}
        mask=0
        for bit in range(8):
            net=pins['h06',7+bit]
            assert net in ['RbAdr.'+str(bit),'RbAdr.'+str(bit)+"'"]
            if net.endswith("'"):mask|=1<<(7-bit)
            assert pins['j06',7+bit]==net
        for package,offset in [('h06',0),('j06',4)]:
            assert [pins[package,pin]for pin in [19,18,17,16]]==[
                f'dRm.{(0 if half=="h" else 8)+offset+i:02d}'for i in range(4)]
        masks[half]=mask;bindings[board]=dict(source=name,sha256=sha(ROOT/name),physical_address_xor=mask)
        dest=out/'inputs'/name;dest.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(ROOT/name,dest)
    assert masks=={'h':15,'l':255}
    port=native/'inputs/src/verilog/rtl/dorado_r_file_ports.sv';raw=port.read_text()
    assert 'assign read_data={ql[7:0],qh[7:0]};'in raw
    words=[]
    for address in range(256):
        wire=(banks['rm_h'][address^masks['h']]&255)|((banks['rm_l'][address^masks['l']]&255)<<8)
        words.append(int(f'{wire:016b}'[::-1],2))
    # A static .mb reader supplies names, without linking cpu.c or executing
    # microcode. Keep exact source and output for independent reproduction.
    symbols=ROOT/'build/rtl-executive/native-initial-symbols-v1-20261024'
    for source,target in [(ROOT/'dorado/src/mb.c','mb.c'),
                          (ROOT/'dorado/include/mb.h','mb.h'),(port,'dorado_r_file_ports.sv')]:
        shutil.copy2(source,out/target)
    image=ROOT/'chm/dorado/expanded/bootstrap.dm!20_/Initial.mb'
    shutil.copy2(image,out/'Initial.mb')
    reader=(symbols/'registers.c').read_text()
    before='int main(void){';after='int main(int argc,char **argv){if(argc!=2)return 2;'
    operand='"chm/dorado/expanded/bootstrap.dm!20_/Initial.mb"'
    assert reader.count(before)==reader.count(operand)==1
    actual=reader.replace(before,after,1).replace(operand,'argv[1]',1)
    assert actual.replace('argv[1]',operand,1).replace(after,before,1)==reader
    (out/'registers.c').write_text(actual)
    command=['cc','-O2','-I'+str(out),str(out/'registers.c'),str(out/'mb.c'),'-o',str(out/'symbols')]
    subprocess.run(command,check=True,capture_output=True)
    listing=subprocess.check_output([str(out/'symbols'),str(out/'Initial.mb')],cwd=out,text=True)
    (out/'register-symbols.log').write_text(listing)
    names={name:int(address)for name,address in re.findall(r'^(\w+) memory=RM .*address_decimal=(\d+)$',listing,re.M)}
    anchors={'R400':0o400,'BOOTDATAPTR':0o1000,'LHOST':0o42,'BFN':0o113}
    for name,value in anchors.items():assert words[names[name]]==value,(name,words[names[name]])
    selected={name:dict(address=names[name],value=words[names[name]],octal=f'{words[names[name]]:06o}')
              for name in ['R400','BOOTDATAPTR','LHOST','BFN','SEQNO','RHOST','RETRIES','EIPTR','EICNT']}
    expected=record['network_buffer']['first32_words'][:record['counters']['network_wire_rlength']]
    assert len(expected)==13 and expected[3]==0o265 and expected[5]==0o76
    slots=record['receiver_FIFO']['slots'];matches=[]
    crc=0x8005  # Independent serial CRC includes the leading sync bit.
    for word in expected:crc=crc_word(crc,word)
    for start in range(16):
        if all(slots[(start+i)%16]['logical_word']==word and not slots[(start+i)%16]['EOP']
               for i,word in enumerate(expected)):
            check=slots[(start+len(expected))%16];status=slots[(start+len(expected)+1)%16]
            assert not check['EOP']and check['logical_word']==crc and crc_word(crc,check['logical_word'])==0
            assert status['EOP']and status['logical_word']==0
            matches.append(dict(start=start,CRC_address=check['physical_address'],CRC_word=crc,
                                EOP_address=status['physical_address'],status_word=0))
    assert len(matches)==1,matches
    retain(inspection,out/inspection.name)
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    result=dict(status='REVIEWED_LAST_NATIVE_RX_PACKET_AND_BOOT_REGISTERS',scope=__doc__,
                inspection_sha256=sha(inspection/'result.json'),pid=record['pid'],
                host_ticks=record['counters']['host_ticks'],guest_ticks=record['counters']['guest_ticks'],
                source_bindings=bindings,anchors=anchors,registers=selected,
                static_symbol_compile_command=command,symbol_image_sha256=sha(out/'Initial.mb'),
                initial_reader_parent_sha256=sha(symbols/'registers.c'),
                final_packet_words=expected,whole_packet_CRC_and_zero_EOP_match=matches,
                pointer_write=record['counters']['receiver_pointer_l09'],
                pointer_read=record['counters']['receiver_pointer_l12'],
                FIFO_empty=record['counters']['receiver_RxFifoEmpty'],
                FIFO_full=record['counters']['receiver_RxFifoFull'],
                first_packet_rejection_reason_established=False,complete_RX_history=False,
                native_or_hardware_boot_qualified=False,hardware_changed=False,
                source_MIR_or_clock_changes=False)
    result['files']={str(p.relative_to(out)):sha(p)for p in out.rglob('*')if p.is_file()and p.name!='symbols'}
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n')
    print(result['status'],json.dumps(selected),flush=True)


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--inspection',type=Path,required=True);parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args();analyze(args.inspection.resolve(),args.output.resolve())
