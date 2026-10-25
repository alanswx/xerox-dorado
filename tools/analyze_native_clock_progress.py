#!/usr/bin/env python3
"""Compare read-only native clock candidates and inventory saved branch flags.

The .mb supplies register names, not the embedded ROM's instruction map. Decode
the exact EPROM payload and bind its timeout operands before interpreting RTC.
Preserve the standalone register candidates to expose their different layout.
No CPU emulator, guest state writes or timer calibration is involved.
"""
import argparse
import json
from pathlib import Path
import re
import shutil
import subprocess
from unittest.mock import patch

from native_branch_flag_sources import bindings, BOARD, WIRELIST
from inspect_initial_rom_wait import run as inspect_rom_wait
from review_late_hold_and_input_reset import retain
from sil_netlist import load_board
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1]


def analyze(inspections,out):
    assert not out.exists() and len(inspections)>=2
    out.mkdir(parents=True)
    original=ROOT/'build/rtl-executive/native-initial-symbols-v1-20261024/registers.c'
    reader=original.read_text()
    assert reader.count('int main(void){')==1 and reader.count('s->mem_id==m.rm_id && s->addr<48')==1
    operand='"chm/dorado/expanded/bootstrap.dm!20_/Initial.mb"'
    assert reader.count(operand)==1
    reader=reader.replace('int main(void){','int main(int argc,char **argv){if(argc!=2)return 2;',1)
    reader=reader.replace(operand,'argv[1]',1).replace('s->mem_id==m.rm_id && s->addr<48','s->mem_id==m.rm_id',1)
    (out/'registers.c').write_text(reader)
    files={'mb.c':ROOT/'dorado/src/mb.c','mb.h':ROOT/'dorado/include/mb.h',
           'Initial.mb':ROOT/'chm/dorado/expanded/bootstrap.dm!20_/Initial.mb',
           'InitialJunk.mc':ROOT/'chm/dorado/expanded/BootstrapSources.dm/InitialJunk.mc',
           'InitialEther.mc':ROOT/'chm/dorado/expanded/BootstrapSources.dm/InitialEther.mc',
           Path(__file__).name:Path(__file__),
           'native_branch_flag_sources.py':ROOT/'tools/native_branch_flag_sources.py',
           'inspect_initial_rom_wait.py':ROOT/'tools/inspect_initial_rom_wait.py'}
    for name,path in files.items():shutil.copy2(path,out/name)
    command=['cc','-O2','-I'+str(out),str(out/'registers.c'),str(out/'mb.c'),'-o',str(out/'symbols')]
    subprocess.run(command,check=True,capture_output=True)
    listing=subprocess.check_output([str(out/'symbols'),str(out/'Initial.mb')],text=True)
    (out/'register-symbols.log').write_text(listing)
    names={name:int(a) for name,a in re.findall(r'^(\w+) memory=RM .*address_decimal=(\d+)$',listing,re.M)}
    assert {n:names[n] for n in ['RTCLOCK','RTCDELTALO','RTC430']}==dict(RTCLOCK=176,RTCDELTALO=177,RTC430=178)
    masks={};pin_sources={}
    for half,board in [('h','ProcH-Rev-Ce'),('l','ProcL-Rev-Ci')]:
        path=ROOT/'chm/sil'/(board+'.dm!1_')/(board+'.wl')
        physical=load_board(str(path));pins={(p['pkg'],p['pin']):n for n,r in physical.nets.items() for p in r['pins']}
        mask=0
        for bit in range(8):
            net=pins['h06',7+bit]
            assert net in ['RbAdr.'+str(bit),'RbAdr.'+str(bit)+"'"]
            assert pins['j06',7+bit]==net
            if net.endswith("'"):mask|=1<<(7-bit)
        masks[half]=mask;pin_sources[board]=dict(sha256=sha(path),address_xor=mask)
        dest=out/'wirelists'/path.name;dest.parent.mkdir(exist_ok=True);shutil.copy2(path,dest)
    assert masks==dict(h=15,l=255)
    observations=[];native=None;branch=None
    for directory in inspections:
        record=json.loads((directory/'result.json').read_text());source=Path(record['source'])
        assert record['pid']==69736 and record['read_only_guest_inspection']
        assert 'Process 69736 detached' in (directory/'lldb.log').read_text()
        assert record['binary_sha256']==sha(source/'obj/native-cedar')
        assert record['header_sha256']==sha(source/'obj/Vtb_native_cedar___024root.h')
        assert native is None or native==source
        native=source;banks=record['translated_front']['physical_9bit_banks']
        def word(address):
            wire=(banks['rm_h'][address^masks['h']]&255)|((banks['rm_l'][address^masks['l']]&255)<<8)
            return int(f'{wire:016b}'[::-1],2)
        anchors={'R400':0o400,'BOOTDATAPTR':0o1000,'LHOST':0o42,'BFN':0o113}
        assert all(word(names[n])==v for n,v in anchors.items())
        selected={n:dict(address=names[n],value=word(names[n]),octal=f'{word(names[n]):06o}')
                  for n in ['RTCLOCK','RTCDELTALO','RTC430','SEQNO','RETRIES','RHOST','EICNT','EIPTR']}
        rom_out=out/('ROM-'+directory.name)
        inspect_rom_wait(source,directory,rom_out)
        rom=json.loads((rom_out/'results.json').read_text())
        assert rom['actual_RTC430_address']==179 and rom['standalone_RTC430_address']==178
        counters=record['counters']
        observations.append(dict(inspection=directory.name,inspection_sha256=sha(directory/'result.json'),
                                 host_ticks=counters['host_ticks'],enabled_machine_ticks=counters['guest_ticks'],
                                 register_candidates=selected,transmitted_packets=counters['network_tx_good'],
                                 ROM_wait=dict(actual_RTC430_address=rom['actual_RTC430_address'],
                                               RTC430_value=rom['RTC430_value'],
                                               emulator_task_T_threshold=rom['emulator_task_T_threshold'],
                                               timeout_subtraction=rom['timeout_subtraction'],
                                               timeout_subtraction_is_negative=rom['timeout_subtraction_is_negative'],
                                               evidence_sha256=sha(rom_out/'results.json'))))
        if 'branch_flag_source_binding' in record:
            branch=record['branch_flag_source_binding']
            assert branch==json.loads(json.dumps(bindings(source)))
            for name,digest in branch['source_hashes'].items():
                assert sha(directory/'branch-source'/name)==digest
        retain(directory,out/directory.name)
    assert branch is not None
    observations.sort(key=lambda row:row['host_ticks'])
    intervals=[]
    for before,after in zip(observations,observations[1:]):
        a=before['register_candidates'];b=after['register_candidates']
        ticks=after['enabled_machine_ticks']-before['enabled_machine_ticks']
        delta=(b['RTCLOCK']['value']-a['RTCLOCK']['value'])&65535
        assert ticks>0 and delta>0
        intervals.append(dict(enabled_machine_ticks=ticks,low_word_delta_mod65536=delta,
                              low_word_numerically_decreased=b['RTCLOCK']['value']<a['RTCLOCK']['value'],
                              machine_ticks_per_modulo_increment=ticks/delta,
                              standalone_RTC430_candidate_delta=(b['RTC430']['value']-a['RTC430']['value'])&65535,
                              actual_RTC430_delta=(after['ROM_wait']['RTC430_value']-before['ROM_wait']['RTC430_value'])&65535,
                              upper_register_ROM_mapping_qualified=True))
    # Pin-corruption controls must reach the physical binding check. File bytes
    # remain unchanged: only the checker sees altered text during this audit.
    board_path=native/'inputs'/BOARD;original_text=board_path.read_text();read_text=Path.read_text
    controls=[]
    for label,old,new in [('wrong_task_address','.p10(CurrLast_0_p_)','.p10(wrong_task_address)'),
                          ('wrong_flag_clock','.p7(Clock1_p_Cb)','.p7(wrong_flag_clock)'),
                          ('wrong_g18_carry','assign ProcH11_sil_pl_13=r_front_q[91];','assign ProcH11_sil_pl_13=r_front_q[92];')]:
        if label=='wrong_task_address':
            start=original_text.index('  cell_F10145A_snapshot u_e13 (');end=original_text.index('  );',start)
            body=original_text[start:end];assert body.count(old)==1
            changed=original_text[:start]+body.replace(old,new,1)+original_text[end:]
        else:
            assert original_text.count(old)==1,(label,original_text.count(old))
            changed=original_text.replace(old,new,1)
        def read(path,*args,**kwargs):return changed if path==board_path else read_text(path,*args,**kwargs)
        with patch.object(Path,'read_text',read):
            try:bindings(native)
            except AssertionError as error:controls.append(dict(case=label,rejected=True,diagnostic=str(error)))
            else:raise AssertionError('Wrong physical binding accepted: '+label)
    summary=dict(status='REVIEWED_NATIVE_CLOCK_CANDIDATES_AND_BRANCH_FLAG_SEAM',scope=__doc__,
                 source_run=str(native),pid=69736,source_bindings=pin_sources,
                 original_low_bank_anchors=anchors,observations=observations,intervals=intervals,
                 branch_flag_source_binding=branch,rejected_binding_controls=controls,
                 static_reader_compilation=command,reader_parent_sha256=sha(original),
                 upper_RTC_register_mapping_bound_to_runtime_ROM=True,
                 standalone_RTC430_register_would_falsely_suggest_a_missing_carry=True,
                 clock_stall_or_missing_carry_established=False,first_packet_rejection_established=False,
                 next_action='Qualify the saved flag write/capture windows and normal multiword carry across task switches before selecting native paired phases; preserve the original boot run and normal retries.',
                 branch_family_integrated_into_common_owner=False,native_timing_qualified=False,
                 hardware_changed=False,speed_gain_measured=False,full_goal_active=True)
    summary['files']={str(p.relative_to(out)):sha(p) for p in out.rglob('*') if p.is_file() and p.name!='symbols'}
    (out/'results.json').write_text(json.dumps(summary,indent=2)+'\n')
    print(summary['status'],len(observations),'read-only observations;',len(controls),'bad pin bindings rejected')
    return summary


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--inspection',type=Path,action='append',required=True)
    parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args();analyze([p.resolve() for p in args.inspection],args.output.resolve())
