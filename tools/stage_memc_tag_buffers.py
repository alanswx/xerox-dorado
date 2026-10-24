#!/usr/bin/env python3
"""Replicate existing Aad inversions at individual asynchronous cache-tag RAMs.

The fitted critical path has a shared inverted address driving320 loads.
Keep one combinational inverter per RAM address pin instead. No state, port,
capture window, clock, RAM function or timing exception is added. Formal checks
all affected RAM address vectors for arbitrary original latch states; a wrong
polarity must have a counterexample. Physical replication/timing remain open.
"""
import argparse
import json
import os
from pathlib import Path
import re
import subprocess

from test_r_front_forward_view import sha

MEMC='src/fpga/mister/snapshot_native_memc/MemC_full_snapshot.sv'
CELL='src/fpga/mister/snapshot_native_processor/cell_MC1668_snapshot.v'
PACKAGES=['a14','a15','b14','b15','c14','c15','d14','d15']


def pins(body):return dict(re.findall(r'\.p(\d+)\((\w+)\)',body))


def stage(parent,out,project,yosys):
    assert not out.exists()and project.is_absolute()
    p=json.loads((parent/'r-front-control-native.json').read_text())
    assert p['frequency_hz']==57142857 and p['sysper']==4 and p['owned_caller_slice15ns_passed']
    assert not p['assembly_permitted']
    for name,digest in p['source_hashes'].items():assert sha(parent/name)==digest,name
    cell=(parent/CELL).read_text()
    assert 'assign p2 =  qa;  assign p3  = ~qa;'in cell
    assert 'assign p15 = qb;  assign p14 = ~qb;'in cell
    text=(parent/MEMC).read_text();positive={};inverted={}
    for index,package in enumerate(PACKAGES):
        body=re.search(r'cell_MC1668_snapshot u_'+package+r' \(.*?\n  \);',text,re.S)[0]
        row=pins(body)
        for offset,true_pin,inverse_pin in [(0,'2','3'),(1,'15','14')]:
            true=row[true_pin];inv=row[inverse_pin]
            positive[true]=2*index+offset;inverted[inv]=true
    assert len(positive)==len(inverted)==16
    changed=text;bindings=[];declarations=[];original_addresses=[];next_addresses=[]
    pattern=r'cell_MB7071H_snapshot u_(\w+) \(.*?\n  \);'
    for match in re.finditer(pattern,text,re.S):
        package=match[1];row=pins(match[0]);updates={}
        for pin in map(str,range(7,15)):
            net=row.get(pin)
            if net in inverted:
                local='tag_address_'+package+'_'+pin
                source=inverted[net]
                declarations.append(f'  (* keep = "true" *) wire {local} = ~{source};')
                bindings.append(dict(package=package,pin=int(pin),original=net,source=source,local=local))
                updates[pin]=local
        if not updates:continue
        replacement=match[0]
        for pin,local in updates.items():
            old=f'.p{pin}({row[pin]})';assert replacement.count(old)==1
            replacement=replacement.replace(old,f'.p{pin}({local})',1)
        assert changed.count(match[0])==1;changed=changed.replace(match[0],replacement,1)
        original_addresses.extend(row[str(pin)]for pin in range(7,15))
        next_addresses.extend(pins(replacement)[str(pin)]for pin in range(7,15))
    assert len(bindings)==84 and len(original_addresses)==168
    marker='  cell_MB7071H_snapshot '
    index=changed.index(marker);changed=changed[:index]+'\n'.join(declarations)+'\n'+changed[index:]
    restored=changed
    for row in bindings:
        restored=restored.replace(f'.p{row["pin"]}({row["local"]})',f'.p{row["pin"]}({row["original"]})',1)
    restored=restored.replace('\n'.join(declarations)+'\n','',1)
    assert restored==text
    out.mkdir(parents=True)
    proof=out/'address-proof';proof.mkdir()
    common=['module tag_address_equivalence(input wire [15:0] owners,output wire equivalent);']
    common += [f' wire {name}=owners[{index}];'for name,index in positive.items()]
    common += [f' wire {name}=~{source};'for name,source in inverted.items()]
    common += declarations
    common += [' wire [167:0] original_address={'+','.join(original_addresses)+'};',
               ' wire [167:0] next_address={'+','.join(next_addresses)+'};',
               ' assign equivalent=original_address==next_address;','endmodule']
    original='\n'.join(common)+'\n';rows=[]
    for variant in ['positive','wrong-polarity']:
        directory=proof/variant;directory.mkdir()
        actual=original
        if variant!='positive':
            first=bindings[0]
            old=f'wire {first["local"]} = ~{first["source"]};'
            assert actual.count(old)==1
            actual=actual.replace(old,old.replace(' = ~',' = '),1)
        (directory/'actual.sv').write_text(actual)
        script='read_verilog -sv /work/actual.sv\nhierarchy -check -top tag_address_equivalence\nproc\nflatten\nopt\ncheck -assert\n'
        script+='sat -prove equivalent 1 -'+('verify'if variant=='positive'else'falsify')+' -show-ports -dump_json /work/counterexample.json\n'
        (directory/'proof.ys').write_text(script)
        env=os.environ.copy();env['YOWASP_MOUNT']='/work='+str(directory)
        env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache')
        with(directory/'proof.log').open('x')as log:
            command=[str(yosys),'-Q','-T','-s','/work/proof.ys']
            code=subprocess.run(command,env=env,stdout=log,stderr=subprocess.STDOUT).returncode
        log=(directory/'proof.log').read_text()
        expected='SAT proof finished - no model found: SUCCESS!'if variant=='positive'else'SAT proof finished - model found: FAIL!'
        assert code==0 and expected in log,(variant,code)
        rows.append(dict(variant=variant,returncode=code,proof_log_sha256=sha(directory/'proof.log'),
                         source_sha256=sha(directory/'actual.sv'),command=command))
    for name in p['source_hashes']:
        raw=(parent/name).read_bytes()
        if name==MEMC:raw=changed.encode()
        if Path(name).suffix in ['.qsf','.qpf','.qip','.tcl','.sdc']:
            raw=raw.replace(p['project'].encode(),str(project).encode())
        dest=out/name;dest.parent.mkdir(parents=True,exist_ok=True);dest.write_bytes(raw)
    changes=[name for name in p['source_hashes']if(out/name).read_bytes().replace(str(project).encode(),p['project'].encode())!=(parent/name).read_bytes()]
    assert changes==[MEMC]
    record={**p,'project':str(project),'scope':__doc__,'parent_stage':str(parent),
            'parent_manifest_sha256':sha(parent/'r-front-control-native.json'),
            'cache_tag_address_buffers':dict(kept_inversions=84,affected_RAMs=21,
                compared_address_bits=168,bindings=bindings,proofs=rows,
                capture_edges_or_RAM_ports_changed=False,extra_state=False,
                actual_physical_duplication_qualified=False),
            'source_hashes':{name:sha(out/name)for name in p['source_hashes']},
            'physical_timing_qualified':False,'firmware_qualified':False,
            'speedup_qualified':False,'assembly_permitted':False,'hardware_changed':False}
    (out/'r-front-control-native.json').write_text(json.dumps(record,indent=2)+'\n')
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    print('STAGED_84_TAG_ADDRESS_INVERSIONS_WITH_168_BIT_EQUIVALENCE',flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for name in ['parent','output','project','yosys']:p.add_argument('--'+name,type=Path,required=True)
    a=p.parse_args();stage(a.parent.resolve(),a.output.resolve(),a.project,a.yosys.resolve())
