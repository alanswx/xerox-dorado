#!/usr/bin/env python3
"""Read-only actual-width2125 fit audit: hashes, geometry, collision mode, timing."""
import argparse
from collections import Counter
import hashlib,json,re,shutil,tempfile
from pathlib import Path

WIDTHS=(8,8,8,8)
CELL_SHA='6eb55772966be2b159da55d2495a57dd66de73f10137479f7f417002aaa157c9'
CORE_SHA='d5e0649f392cfdbc116de9bbc5b615ba923526704357ddfb0888ace111fea5ca'
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def normalized(text):
    return '\n'.join(line for line in text.splitlines()if line.strip()and line!='set_global_assignment -name LAST_QUARTUS_VERSION "17.0.2 Lite Edition"')


def inferred_memories(text):
    headers=list(re.finditer(r'(?m)^;\s*Parameter Settings for (?:User|Inferred) Entity Instance:\s*([^\n;]+)',text));rows=[]
    for i,header in enumerate(headers):
        if 'altsyncram:'not in header[1]or not re.search(r'\bbank_[0-3]\|',header[1]):continue
        section=text[header.end():headers[i+1].start()if i+1<len(headers)else len(text)]
        number=int(re.search(r'\bbank_([0-3])\|',header[1])[1]);values={}
        for key,wanted in [('OPERATION_MODE','DUAL_PORT'),('RAM_BLOCK_TYPE','M10K'),('WIDTH_A',str(WIDTHS[number])),('WIDTH_B',str(WIDTHS[number])),
                           ('NUMWORDS_A','1024'),('NUMWORDS_B','1024'),('READ_DURING_WRITE_MODE_MIXED_PORTS','OLD_DATA')]:
            actual=re.findall(r'; '+key+r'\s*; (\w+)\s*;',section)
            assert actual==[wanted],('RAM inference differs',number,key,actual)
            values[key]=wanted
        rows.append(dict(bank=number,instance=header[1].strip(),parameters=values))
    assert len(rows)==4 and {r['bank']for r in rows}==set(range(4)),'missing/extra width-correct RAM macro'
    return rows


def physical_ram(text,variant):
    start=text.find('; Fitter RAM Summary');assert start>=0,'missing physical RAM table'
    table=text[start:].split('\n\n',1)[0];header=None;rows=[]
    for line in table.splitlines():
        if not line.startswith('; '):continue
        f=[x.strip()for x in line.split(';')[1:-1]]
        if f and f[0]=='Name':header=f
        elif f and re.search(r'(?:bank_[0-3]|original_[0-3])',f[0]):
            assert header and len(header)==len(f),'malformed physical RAM row'
            rows.append(dict(zip(header,f)))
    assert len(rows)==(4 if variant=='snapshot'else 32),'physical SRAM row count differs'
    width=8 if variant=='snapshot'else 1
    locations={}
    for row in rows:
        pattern=r'\bbank_([0-3])\|'if variant=='snapshot'else r'\boriginal_([0-3])\[([0-7])\]\.bit_cell\|'
        m=re.search(pattern,row['Name']);assert m,'unreviewed SRAM instance'
        key=int(m[1])if variant=='snapshot'else(int(m[1]),int(m[2]))
        assert key not in locations,'duplicate physical SRAM bank/plane'
        assert re.fullmatch(r'M10K_X\d+_Y\d+_N0',row['Location']),'not a physical M10K location'
        locations[key]=row['Location']
        assert row['Type']==('M10K block'if variant=='snapshot'else'AUTO'),'RAM type differs'
        for key,wanted in {'Mode':'Simple Dual Port','Port A Depth':'1024','Port B Depth':'1024',
          'Port A Width':str(width),'Port B Width':str(width),'Size':str(width*1024),
          'Implementation Bits':str(width*1024),'M10K blocks':'1','MLAB cells':'0',
          'Clock Mode':'Single Clock','Port A Input Registers':'yes','Port A Output Registers':'no',
          'Port B Input Registers':'yes','Port B Output Registers':'no','Mixed Width RDW Mode':'Old data'}.items():
            assert row.get(key)==wanted,('physical SRAM differs',variant,key,row.get(key))
    assert len(set(locations.values()))==4,'four distinct physical SRAM sites required'
    if variant=='snapshot':assert set(locations)==set(range(4)),'missing/extra SRAM bank'
    else:
        assert set(locations)=={(i,b)for i in range(4)for b in range(8)},'missing/extra original SRAM plane'
        assert all(len({locations[i,b]for b in range(8)})==1 for i in range(4)),'original byte grouping differs'
    return rows


def audit(directory):
    record=json.loads((directory/'provenance.json').read_text());results={}
    assert record['widths']==list(WIDTHS)and record['modeled_state_bits']==32800
    for name,expected in record['compiled_files'].items():
        if not name.endswith('.qsf'):assert digest(directory/name)==expected,'compiled input differs: '+name
    corners={'7_slow_1100mv_-40c','7_slow_1100mv_100c','MIN_fast_1100mv_-40c','MIN_fast_1100mv_100c'}
    for variant in ('baseline','snapshot'):
        case=directory/variant
        assert digest(case/'cell_i2125.v')==CELL_SHA,'unreviewed original cell'
        assert digest(case/'f10415_word_bank.sv')==CORE_SHA,'qualified transport changed'
        assert normalized((case/'snapshot_fit.qsf').read_text())==normalized(record['expected_qsf'][variant]),'project source/macro/SDC changed'
        summary=(case/'snapshot_fit.fit.summary').read_text();assert 'Fitter Status : Successful'in summary
        resources={}
        for key,label in [('alms',r'Logic utilization \(in ALMs\)'),('registers','Total registers'),('ram_bits','Total block memory bits'),('ram_blocks','Total RAM Blocks')]:
            m=re.search(label+r' : ([\d,]+)',summary);assert m,'missing resource '+key
            resources[key]=int(m[1].replace(',',''))
        assert resources['ram_bits']==32768 and resources['ram_blocks']==4,'RAM geometry/packing differs'
        assert {p.name[len('bank-setup-'):-4]for p in case.glob('bank-setup-*.rpt')}==corners,'missing corner'
        timing={}
        for corner in sorted(corners):
            timing[corner]={}
            for mode in ('setup','hold','mpw'):
                text=(case/f'bank-{mode}-{corner}.rpt').read_text();m=re.search(r'\((\d+) violated\)\.\s+Worst case slack is (-?[\d.]+)',text)
                assert m and int(m[1])==0 and float(m[2])>=0,'timing violation/missing path '+mode+'/'+corner
                timing[corner][mode]=float(m[2])
            for mode in ('recovery','removal'):
                assert (case/f'bank-{mode}-{corner}.rpt').read_text().strip().endswith('Nothing to report.'),'unexpected async path'
            text=(case/f'bank-ucp-{corner}.rpt').read_text()
            for label in ('Illegal Clocks','Unconstrained Clocks','Unconstrained Input Ports','Unconstrained Input Port Paths','Unconstrained Output Ports','Unconstrained Output Port Paths'):
                assert re.search('; '+label+r'\s*; 0\s*; 0\s*;',text),'unconstrained '+label
        warnings={}
        for phase in ('map','fit'):
            text=(case/(phase+'.log')).read_text();assert 'Critical Warning'not in text,'critical warning'
            warnings[phase]=dict(Counter(re.findall(r'Warning \((\d+)\)',text)))
        row=dict(resources=resources,timing=timing,warnings=warnings,physical_ram=physical_ram((case/'snapshot_fit.fit.rpt').read_text(errors='replace'),variant))
        if variant=='snapshot':row['sram_macros']=inferred_memories((case/'snapshot_fit.map.rpt').read_text(errors='replace'))
        results[variant]=row
    for phase in ('map','fit'):
        new=Counter(results['snapshot']['warnings'][phase])-Counter(results['baseline']['warnings'][phase])
        assert not(set(new)-{'10036','13024','13410'}),('unreviewed warning increase',phase,new)
    return dict(status='PASS',scope=record['scope'],results=results,
        delta={key:results['snapshot']['resources'][key]-results['baseline']['resources'][key]for key in ('alms','registers','ram_bits','ram_blocks')},
        minimum_snapshot_slack_ns=min(v for row in results['snapshot']['timing'].values()for v in row.values()))


def controls(directory):
    rows=[]
    for name,path,before,after in [
        ('source','snapshot/f10415_word_bank.sv','saved_q<=state_data[WIDTH-1:0];',"saved_q<='0;"),
        ('wrapper','snapshot/i2125_byte_bank.sv',"we_n ? bank_q : 8'b0",'bank_q'),
        ('constraint','snapshot/bank.sdc','20.000','18.000'),
        ('macro','snapshot/snapshot_fit.qsf','SNAPSHOT_BANK_EXPERIMENT=1','SNAPSHOT_BANK_EXPERIMENT=0'),
        ('old_data','snapshot/snapshot_fit.map.rpt','OLD_DATA','NEW_DATA'),
        ('physical_rdw','snapshot/snapshot_fit.fit.rpt','Old data','New data'),
        ('physical_bank','snapshot/snapshot_fit.fit.rpt','bank_1|','bank_0|'),
        ('ram_count','snapshot/snapshot_fit.fit.summary','Total RAM Blocks : 4','Total RAM Blocks : 5'),
        ('hold','snapshot/bank-hold-7_slow_1100mv_-40c.rpt','(0 violated).','(1 violated).'),
        ('warning','snapshot/map.log',None,None),
    ]:
        with tempfile.TemporaryDirectory(prefix='dorado-word-control-',dir='/tmp')as temp:
            out=Path(temp)/'evidence';shutil.copytree(directory,out);p=out/path;text=p.read_text(encoding='latin1')
            if before:assert before in text; text=text.replace(before,after)
            else:text+='\nWarning (10999): unreviewed inference\n'
            p.write_text(text,encoding='latin1')
            try:audit(out)
            except AssertionError as error:rows.append(dict(case=name,rejected=True,diagnostic=str(error)))
            else:raise AssertionError('damaged RAM fit evidence accepted: '+name)
    return rows


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path);p.add_argument('--output',type=Path,required=True);p.add_argument('--negative-controls',action='store_true')
    a=p.parse_args();r=audit(a.directory.resolve())
    if a.negative_controls:r['failure_controls']=controls(a.directory.resolve())
    a.output.write_text(json.dumps(r,indent=2)+'\n');print('I2125 FIT AUDIT PASS;minimum slack',r['minimum_snapshot_slack_ns'])
