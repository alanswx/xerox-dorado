#!/usr/bin/env python3
"""Bind a passive native wait observation to its actual EPROM microinstructions.

The standalone Initial.mb has different placement and an RTC register allocation
which differs from the ROM payload. Use its symbols only after checking the
actual ROM fields. This is static decoding/readback interpretation, never guest
execution, packet acceptance, timer calibration or a complete firmware gate.
"""
import argparse
import gzip
import json
from pathlib import Path
import re
import shutil
import subprocess

from baseboard_rom_im import decode
from test_r_front_forward_view import sha

ROOT=Path(__file__).resolve().parents[1]
FIELDS=['rstk','aluf','bsel','lc','asel','block','ff','jcn']


def run(source,readback,out):
    assert not out.exists()
    observed=json.loads((readback/'result.json').read_text())
    assert Path(observed['source'])==source and observed['read_only_guest_inspection']
    assert f"Process {observed['pid']} detached"in(readback/'lldb.log').read_text()
    assert sha(source/'obj/native-cedar')==observed['binary_sha256']
    assert sha(source/'obj/Vtb_native_cedar___024root.h')==observed['header_sha256']
    provenance=json.loads(gzip.decompress((source/'provenance.json.gz').read_bytes()))
    for name,digest in provenance['inputs'].items():assert sha(source/'inputs'/name)==digest,name
    out.mkdir(parents=True);inputs=out/'inputs';inputs.mkdir()
    copied={}
    def copy(path,name):
        dest=inputs/name;dest.parent.mkdir(parents=True,exist_ok=True)
        shutil.copy2(path,dest);copied[name]=sha(dest);return dest
    for name in ['mb','microcode','disasm']:
        copy(ROOT/f'dorado/src/{name}.c',f'dorado/src/{name}.c')
        copy(ROOT/f'dorado/include/{name}.h',f'dorado/include/{name}.h')
    helper=copy(ROOT/'tools/initial_rom_symbols.c','initial_rom_symbols.c')
    copy(Path(__file__),Path(__file__).name)
    copy(ROOT/'tools/baseboard_rom_im.py','baseboard_rom_im.py')
    mb=copy(ROOT/'chm/dorado/expanded/bootstrap.dm!20_/Initial.mb','Initial.mb')
    copy(readback/'result.json','readback.json');copy(readback/'lldb.log','readback-detach.log')
    copy(source/'provenance.json.gz','native-provenance.json.gz')
    for binding in observed['T_file']['source_bindings'].values():
        path=Path(binding['source']);assert sha(path)==binding['source_sha256']
        copy(path,'native/'+str(path.relative_to(source/'inputs')))
    for kind,suffix in [('F10145A','sv'),('MC10173','v')]:
        name=f'src/fpga/mister/snapshot_native_conta/cell_{kind}_snapshot.{suffix}'
        copy(source/'inputs'/name,'native/'+name)
    if 'Q_register'in observed:
        name='src/fpga/mister/snapshot_native_conta/cell_MC10141_snapshot.v'
        copy(source/'inputs'/name,'native/'+name)
    binary=out/'symbols'
    command=['cc','-O2','-I'+str(inputs/'dorado/include'),str(helper),
             *[str(inputs/f'dorado/src/{n}.c')for n in ['mb','microcode','disasm']],'-o',str(binary)]
    with(out/'compile.log').open('x')as log:
        subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
    text=subprocess.check_output([str(binary),str(mb)],text=True)
    (out/'symbols.log').write_text(text)
    symbols={};registers={};standalone={}
    for line in text.splitlines():
        f=line.split()
        if f[0]=='SYM':symbols[f[1]]=dict(image=int(f[2],16),real=int(f[3],16))
        elif f[0]=='RM':registers[f[1]]=int(f[2],16)
        else:
            assert f[0]=='IM'and len(f)==10
            standalone[int(f[1],16)]=dict(zip(FIELDS,map(lambda v:int(v,16),f[2:])))
    rom={}
    for socket,base in [('f60',0xc000),('e60',0xc800),('c61',0xf000),('b61',0xf800)]:
        path=source/f'inputs/src/verilog/proms/eprom/BaseBd-{socket}.mem'
        frozen=copy(path,'eprom/'+path.name)
        values=[int(v,16)for v in re.sub(r'//[^\n]*','',frozen.read_text()).split()]
        assert len(values)==2048 and all(0<=v<=255 for v in values)
        rom.update({base+i:int(f'{v:08b}'[::-1],2)for i,v in enumerate(values)})
    actual={};blocks=[]
    for header in [0xc000,0xc00b]:
        lines=list(decode(rom,header));blocks.append(lines[0])
        (out/f'rom-im-{header:04x}.txt').write_text('\n'.join(lines)+'\n')
        for line in lines[1:]:
            f=line.split();assert f[0]=='IM'and len(f)==12
            address=int(f[1],16);assert address not in actual
            actual[address]=dict(zip(FIELDS,[int(v,16)for v in f[2:10]]))
    wait=symbols['AWAITETHERBOOTREPLY']['real'];reset=symbols['RESETETHERBOOTTIMER']['real']
    matches=[a for a,row in actual.items()if row==standalone[wait]]
    assert len(matches)==1,('ROM wait binding is ambiguous',matches)
    delta=matches[0]-wait;assert delta==0x40
    # The actual wait reads the next RTC slot. Every other operand in these
    # two instructions and the four remaining wait/reset instructions agrees.
    bindings=[]
    for original,changed in [(wait,False),(reset,False),(0xc35,False),(0xc31,False),
                             (0xc26,True),(0xc2d,True)]:
        old=standalone[original];new=actual[original+delta]
        different=[k for k in FIELDS if old[k]!=new[k]]
        assert different==(['rstk']if changed else[]),(original,different)
        if changed:assert new['rstk']==old['rstk']+1
        bindings.append(dict(standalone_real=original,ROM_real=original+delta,
                             differing_fields=different,standalone=old,ROM=new))
    clock_step=actual[0xc26+delta];seq_step=actual[wait+delta]
    assert seq_step['ff']==0x8b and clock_step['ff']==0x81
    # HM Table11a: immediate FF RBase<-bank. The sequence compare switches
    # from bank1 to bankB; the following RTC subtraction switches back.
    sequence_address=((clock_step['ff']&15)<<4)|seq_step['rstk']
    clock_address=((seq_step['ff']&15)<<4)|clock_step['rstk']
    assert sequence_address==registers['SEQNO']==0x17
    assert clock_address==registers['RTC430']+1==0xb3
    reverse=lambda word:int(f'{word:016b}'[::-1],2)
    words=list(map(reverse,observed['register_file']['wire_order_words']))
    task_t=list(map(reverse,observed['T_file']['wire_order_words']))
    clock=words[clock_address];threshold=task_t[0]
    alu=(clock-threshold-1)&0xffff
    result=dict(status='PASS_ORIGINAL_ROM_WAIT_BINDING',scope=__doc__,blocks=blocks,
        standalone_symbols=symbols,standalone_RM_symbols=registers,
        instruction_bindings=bindings,ROM_relocation=delta,
        sequence_address=sequence_address,sequence_number=words[sequence_address],
        actual_RTC430_address=clock_address,standalone_RTC430_address=registers['RTC430'],
        RTC430_value=clock,emulator_task_T_threshold=threshold,
        timeout_subtraction=alu,timeout_subtraction_is_negative=bool(alu&0x8000),
        timeout_condition='ROM subtraction RTC430 - task0.T - 1; interpret sampled flags/branch timing separately',
        host_tick=observed['counters']['host_ticks'],guest_tick=observed['counters']['guest_ticks'],
        readback_sha256=sha(readback/'result.json'),decoder_command=command,
        input_hashes=copied,original_ROM_or_guest_state_changed=False,
        packet0_root_cause_established=False,firmware_or_clock_speed_qualified=False)
    if 'Q_register'in observed:
        result['global_Q']=reverse(observed['Q_register']['wire_order_word'])
        result['sequence_compare_Q_matches']=result['global_Q']==result['sequence_number']
    assert result['sequence_number']==0
    for name,digest in copied.items():assert sha(inputs/name)==digest,name
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps({k:result[k]for k in ['status','actual_RTC430_address','RTC430_value',
        'emulator_task_T_threshold','timeout_subtraction_is_negative']},indent=2))


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for name in ['source','readback','output']:p.add_argument('--'+name,type=Path,required=True)
    a=p.parse_args();run(a.source.resolve(),a.readback.resolve(),a.output.resolve())
