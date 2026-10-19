#!/usr/bin/env python3
"""Read-only eight-socket EPROM fit audit: exact firmware, block RAM and timing."""
import argparse
from collections import Counter
import hashlib,json,re,shutil,tempfile
from pathlib import Path

import generate_i2716 as gen
CELL_SHA=gen.SHA
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def normalized(text):
    return '\n'.join(line for line in text.splitlines()if line.strip()and line!='set_global_assignment -name LAST_QUARTUS_VERSION "17.0.2 Lite Edition"')


def firmware(path):
    text=re.sub(r'//[^\n]*','',path.read_text());values=[]
    for token in text.split():
        assert re.fullmatch(r'[0-9a-fA-F]{2}',token),'unreviewed readmemh token'
        values.append(int(token,16))
    assert len(values)==2048,'firmware image length differs'
    return bytes(values)


def synthesized_image(path):
    text=path.read_text();assert re.search(r'WIDTH\s*=\s*8;',text)and re.search(r'DEPTH\s*=\s*2048;',text),'synthesized image geometry differs'
    assert 'ADDRESS_RADIX=UNS;'in text and'DATA_RADIX=BIN;'in text,'synthesized image radix differs'
    words={}
    for a,b in re.findall(r'(?m)^\s*(\d+)\s*:\s*([01]{8});\s*$',text):
        a=int(a);assert a not in words,'duplicate synthesized address';words[a]=int(b,2)
    assert set(words)==set(range(2048)),'incomplete/extra synthesized image words'
    return bytes(words[i]for i in range(2048))


def inferred_memories(text):
    headers=list(re.finditer(r'(?m)^;\s*Parameter Settings for (?:User|Inferred) Entity Instance:\s*([^\n;]+)',text));rows=[]
    for i,h in enumerate(headers):
        if 'altsyncram:'not in h[1]or not re.search(r'\brom_\w+\|',h[1]):continue
        pkg=re.search(r'\brom_(\w+)\|',h[1])[1];assert pkg in gen.IMAGES,'empty/unknown socket synthesized as ROM'
        section=text[h.end():headers[i+1].start()if i+1<len(headers)else len(text)];values={}
        for key,wanted in [('OPERATION_MODE','ROM'),('WIDTH_A','8'),('NUMWORDS_A','2048')]:
            actual=re.findall(r'; '+key+r'\s*; (\w+)\s*;',section);assert actual==[wanted],('EPROM macro differs',pkg,key,actual);values[key]=wanted
        rows.append(dict(socket=pkg,instance=h[1].strip(),parameters=values))
    assert len(rows)==4 and{r['socket']for r in rows}==set(gen.IMAGES),'missing/extra EPROM macro'
    return rows


def physical_ram(case):
    text=(case/'snapshot_fit.fit.rpt').read_text(encoding='latin1');start=text.find('; Fitter RAM Summary');assert start>=0,'missing physical EPROM table'
    table=text[start:].split('\n\n',1)[0];header=None;rows=[]
    for line in table.splitlines():
        if not line.startswith('; '):continue
        f=[x.strip()for x in line.split(';')[1:-1]]
        if f and f[0]=='Name':header=f
        elif f and'rom_'in f[0]:
            assert header and len(header)==len(f),'malformed physical EPROM row';rows.append(dict(zip(header,f)))
    assert len(rows)==4,'physical EPROM row count differs';seen=set();sites=set()
    for row in rows:
        pkg=re.search(r'\brom_(\w+)\|',row['Name'])[1]
        assert pkg in gen.IMAGES and pkg not in seen,'duplicate/empty/foreign EPROM socket';seen.add(pkg)
        for key,wanted in {'Type':'AUTO','Mode':'ROM','Port A Depth':'2048','Port A Width':'8','Port B Depth':'--','Port B Width':'--',
          'Size':'16384','Implementation Bits':'16384','M10K blocks':'2','MLAB cells':'0','Clock Mode':'Single Clock',
          'Port A Input Registers':'yes','Port A Output Registers':'no'}.items():
            assert row.get(key)==wanted,('physical EPROM differs',pkg,key,row.get(key))
        locations=row['Location'].split(', ');assert len(locations)==2 and all(re.fullmatch(r'M10K_X\d+_Y\d+_N0',x)for x in locations),'physical EPROM sites differ'
        assert not(sites&set(locations)),'independent EPROM banks share a site';sites.update(locations)
        image=case/'verilog/proms/eprom'/('BaseBd-'+pkg+'.mem');assert digest(image)==gen.IMAGES[pkg],'firmware bytes differ'
        assert synthesized_image(case/row['MIF'])==firmware(image),'synthesized firmware bytes differ'
    assert len(sites)==8,'eight independent physical EPROM sites required'
    return rows


def audit(directory):
    record=json.loads((directory/'provenance.json').read_text());results={}
    assert record['sockets']==list(gen.SOCKETS)and record['image_hashes']==gen.IMAGES,'socket/firmware map differs'
    assert record['manifest']['logical_state_bits']==9 and record['manifest']['original_sha256']==gen.SHA
    assert record['manifest']['fields']==[dict(name='d',width=8,lsb=0),dict(name='en_q',width=1,lsb=8)],'pipeline schema differs'
    for name,expected in record['compiled_files'].items():
        if not name.endswith('.qsf'):assert digest(directory/name)==expected,'compiled input differs: '+name
    corners={'7_slow_1100mv_-40c','7_slow_1100mv_100c','MIN_fast_1100mv_-40c','MIN_fast_1100mv_100c'}
    for variant in ('baseline','snapshot'):
        case=directory/variant
        assert digest(case/'cell_i2716.v')==CELL_SHA,'unreviewed original cell'
        assert digest(case/'cell_i2716_snapshot.sv')==record['manifest']['generated_sha256'],'adapter differs'
        assert normalized((case/'snapshot_fit.qsf').read_text())==normalized(record['expected_qsf'][variant]),'project source/macro/SDC changed'
        summary=(case/'snapshot_fit.fit.summary').read_text();assert 'Fitter Status : Successful'in summary
        resources={}
        for key,label in [('alms',r'Logic utilization \(in ALMs\)'),('registers','Total registers'),('ram_bits','Total block memory bits'),('ram_blocks','Total RAM Blocks')]:
            m=re.search(label+r' : ([\d,]+)',summary);assert m,'missing resource '+key
            resources[key]=int(m[1].replace(',',''))
        assert resources['ram_bits']==65536 and resources['ram_blocks']==8,'RAM geometry/packing differs'
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
        row=dict(resources=resources,timing=timing,warnings=warnings,physical_ram=physical_ram(case),rom_macros=inferred_memories((case/'snapshot_fit.map.rpt').read_text(encoding='latin1')))
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
        ('source','snapshot/cell_i2716.v','reg [7:0] d;','reg [6:0] d;'),
        ('adapter','snapshot/cell_i2716_snapshot.sv',"restored_d<=state_data[7:0];","restored_d<=8'b0;"),
        ('constraint','snapshot/bank.sdc','20.000','18.000'),
        ('macro','snapshot/snapshot_fit.qsf','SNAPSHOT_BANK_EXPERIMENT=1','SNAPSHOT_BANK_EXPERIMENT=0'),
        ('macro_mode','snapshot/snapshot_fit.map.rpt','ROM','RAM'),
        ('physical_geometry','snapshot/snapshot_fit.fit.rpt','16384','8192'),
        ('physical_bank','snapshot/snapshot_fit.fit.rpt','rom_c61|','rom_b61|'),
        ('ram_count','snapshot/snapshot_fit.fit.summary','Total RAM Blocks : 8','Total RAM Blocks : 0'),
        ('firmware','snapshot/verilog/proms/eprom/BaseBd-b61.mem',None,None),
        ('synthesized_firmware',None,None,None),
        ('hold','snapshot/bank-hold-7_slow_1100mv_-40c.rpt','(0 violated).','(1 violated).'),
        ('warning','snapshot/map.log',None,None),
    ]:
        with tempfile.TemporaryDirectory(prefix='dorado-word-control-',dir='/tmp')as temp:
            out=Path(temp)/'evidence';shutil.copytree(directory,out)
            if name=='synthesized_firmware':
                row=physical_ram(out/'snapshot')[0];p=out/'snapshot'/row['MIF'];text=p.read_text();m=re.search(r'(?m)^(\s*2047\s*:\s*)([01])',text);assert m;text=text[:m.start(2)]+str(1-int(m[2]))+text[m.end(2):]
            else:p=out/path;text=p.read_text(encoding='latin1')
            if name=='firmware':text+='\n00\n'
            elif name=='synthesized_firmware':pass
            elif before:assert before in text; text=text.replace(before,after)
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
    a.output.write_text(json.dumps(r,indent=2)+'\n');print('I2716 FIT AUDIT PASS;minimum slack',r['minimum_snapshot_slack_ns'])
