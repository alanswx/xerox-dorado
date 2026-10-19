#!/usr/bin/env python3
"""Fresh actual display XML state coverage, explicit nonaliased word/bit map."""
import argparse,copy,hashlib,json,re,subprocess,sys
from pathlib import Path
import generate_dispy_full as gen
sys.path.insert(0,str(gen.ROOT/'tools'))
from sil_state_inventory import inventory


def classify(report,m,root):
    if m!=gen.generate(root)[1]:raise ValueError('unreviewed display schema/source')
    prefix=gen.ORIGINAL_MODULE+'.';expected={};excluded={};allocated=set()
    def key(package,name):return prefix+(package+'.'if package else'')+name
    def add(package,name,width,dimensions,word,lsb=0,access=None):
        depth=dimensions[0][1]-dimensions[0][0]+1 if dimensions else 1
        access=access or[dict(word=word,lsb=lsb,width=width,field_lsb=0,depth=depth)]
        if key(package,name)in expected:raise ValueError('duplicate display field')
        field_bits=set()
        for c in access:
            if c['lsb']+c['width']>18 or c['field_lsb']+c['width']>width or c['depth']!=depth:raise ValueError('bad display transport mapping')
            for a in range(depth):
                for bit in range(c['width']):
                    target=(c['word']+a,c['lsb']+bit);source=(a,c['field_lsb']+bit)
                    if target in allocated or source in field_bits:raise ValueError('aliased display transport field')
                    allocated.add(target);field_bits.add(source)
        if len(field_bits)!=width*depth:raise ValueError('missing display transport bits')
        expected[key(package,name)]=dict(width=width,bits=width*depth,dimensions=dimensions,access=access)
    for r in m['ff']:
        for f in r['fields']:add(r['package'],f['name'],f['width'],[],r['word'],f['lsb'])
    for r in m['memories']:
        k=r['kind'];p=r['package'];word=r['word']
        if k=='cell_K1115A':
            add(p,'acc',32,[],word,access=[dict(word=word,lsb=0,width=18,field_lsb=0,depth=1),dict(word=word+1,lsb=0,width=14,field_lsb=18,depth=1)])
            add(p,'osc',1,[],word+1,14)
        else:
            add(p,'mem',r['width'],[[0,r['words']-1]],word)
            if k=='cell_F10415A':add(p,'dout_r',1,[],word+1024)
            elif k=='cell_F10145A':
                for f in r['write_window']['fields']:add(p,f['name'],f['width'],[],word+16,f['lsb'])
                for n,w in {'a_first':4,'d_first':4,'in_we':1,'n_amoved':32,'n_dmoved':32,'n_writes':32,'ramwe_dbg':1}.items():excluded[key(p,n)]=w
    for f in m['phase']['fields']:add('',f['name'],f['width'],[],m['phase']['word'],f['lsb'])
    roms={key(p['package'],'mem'):p for p in m['immutable_proms']}
    modules={gen.ORIGINAL_MODULE:(root/gen.SOURCE).read_text(),**{Path(n).stem:(root/n).read_text()for n in m['original_sources']}}
    procedural={r['kind']for r in m['ff']}|{r['kind']for r in m['memories']}|{gen.ORIGINAL_MODULE}
    derived={'ck_en','cka_en','ckb_en','clka','clkb','clk','d_a','d_b','d','a','addr','q','pe_n','ce_n',
             'load_data','d_use','s_use','dr_use','dl_use','sel','we','ja','ka','jb','kb','_unused'}
    seen=set();decisions=[]
    for r in report['candidates']:
        path=r['path'];category=r['category'];name=r['name']
        if path in seen:raise ValueError('duplicate display XML candidate')
        seen.add(path)
        if path in expected:
            w=expected.pop(path)
            if (category,r['width'],r['bits'],r['unpacked_dimensions'])!=('sequential',w['width'],w['bits'],w['dimensions']):raise ValueError('display state geometry differs:'+path)
            decisions.append(dict(path=path,decision='SAVE',bits=r['bits'],access=w['access']));continue
        if path in excluded:
            w=excluded.pop(path);wanted='initial_only_candidate'if name=='ramwe_dbg'else'sequential'
            if (category,r['bits'],r['unpacked_dimensions'])!=(wanted,w,[]):raise ValueError('display diagnostic differs')
            decisions.append(dict(path=path,decision='EXCLUDE_SIMULATION_ONLY',bits=w,reason='Original checked translate_off block, no use outside it'));continue
        module=r['module'].split('__')[0]
        if module not in modules:raise ValueError('unreviewed display module:'+module)
        if module=='cell_MCM10149':
            if path in roms:
                prom=roms.pop(path)
                if (category,r['width'],r['bits'],r['unpacked_dimensions'])!=('initial_only_candidate',4,1024,[[0,255]]):raise ValueError('display PROM changed shape/mutability')
                decisions.append(dict(path=path,decision='IMMUTABLE_BUILD_ASSET',bits=1024,asset=prom));continue
            if category!='combinational_review':raise ValueError('unclassified display PROM state')
        else:
            source=gen.ff.masked_comments(modules[module])
            if module not in procedural and re.search(r'\b(always\w*|initial|reg|logic)\b',source):raise ValueError('unreviewed procedural display cell')
            if category=='initial_only_candidate':
                if not re.search(r'\bwire\s+(?:\[[^\]]+\]\s*)?'+re.escape(name)+r'\s*(?:[;=])',source):raise ValueError('unclassified initial display state:'+path)
            elif category=='combinational_review':
                if module in procedural and module!=gen.ORIGINAL_MODULE and not re.fullmatch('p[0-9]+',name)and name not in derived:raise ValueError('unreviewed display derived field:'+path)
            else:raise ValueError('unexported display state:'+path)
        decisions.append(dict(path=path,decision='RECOMPUTE',bits=r['bits'],reason='Original source-pinned continuous combinational/constant expression'))
    if expected or excluded or roms:raise ValueError('missing display state/assets:'+str(list(expected)[:3]))
    bits=sum(x['bits']for x in decisions if x['decision']=='SAVE')
    if bits!=13993 or len(allocated)!=bits:raise ValueError('incomplete display state transport')
    return dict(status='PASS',modeled_state_bits=bits,excluded_simulator_bits=318,immutable_prom_bits=2048,xml_sha256=report['xml_sha256'],decisions=decisions,
        scope='Complete modeled actual native DispY_field only; not physical/global snapshot or timer migration')


def controls(report,m,root):
    rows=[]
    for name in ('lost_phase','lost_acc_high','lost_osc','lost_pipeline','lost_window','lost_ram','ram_depth','wrong_width','new_state','duplicate','socket_alias','prom_hash','mutable_prom','source_hash'):
        r,s=copy.deepcopy(report),copy.deepcopy(m)
        suffix={'lost_phase':'.clk1_p_Aa','lost_acc_high':'.u_a05.acc','lost_osc':'.u_a05.osc','lost_pipeline':'.u_h14.dout_r','lost_window':'.'+next(x['package']for x in m['memories']if x['kind']=='cell_F10145A')+'.we_d','lost_ram':'.u_h14.mem'}
        if name in suffix:
            before=len(r['candidates']);r['candidates']=[x for x in r['candidates']if not x['path'].endswith(suffix[name])];assert len(r['candidates'])<before,('missing fault target',name)
        elif name=='ram_depth':next(x for x in r['candidates']if x['path'].endswith('.u_h14.mem'))['unpacked_dimensions']=[[0,511]]
        elif name=='wrong_width':next(x for x in r['candidates']if x['path'].endswith('.u_a05.acc'))['width']=31
        elif name=='new_state':
            x=copy.deepcopy(next(x for x in r['candidates']if x['category']=='sequential'));x['path']+='_hidden';r['candidates'].append(x)
        elif name=='duplicate':r['candidates'].append(copy.deepcopy(r['candidates'][0]))
        elif name=='socket_alias':s['memories'][1]['word']=s['memories'][0]['word']
        elif name=='prom_hash':s['immutable_proms'][0]['sha256']='0'*64
        elif name=='mutable_prom':next(x for x in r['candidates']if x['path'].endswith('.u_i15.mem'))['category']='sequential'
        else:s['original_sources']['verilog/cells/cell_K1115A.v']='0'*64
        try:classify(r,s,root)
        except ValueError as e:rows.append(dict(case=name,rejected=True,diagnostic=str(e)))
        else:raise AssertionError('damaged display state map accepted:'+name)
    return rows

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--source-root',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args();out=a.output.resolve();out.mkdir(parents=True,exist_ok=False);root=a.source_root.resolve()
    _,m,_=gen.generate(root);cmd=['verilator','--xml-only','--top-module',gen.ORIGINAL_MODULE,'-Wno-fatal','-DDORADO_F10145_LOGIC','-DDORADO_F10415_M10K','--xml-output',str(out/'display.xml'),str(root/gen.SOURCE),*[str(root/n)for n in m['original_sources']]]
    (out/'command.json').write_text(json.dumps(cmd,indent=2)+'\n')
    with(out/'elaborate.log').open('w')as f:subprocess.run(cmd,cwd=root,stdout=f,stderr=subprocess.STDOUT,check=True)
    r=inventory((out/'display.xml').read_bytes());q=classify(r,m,root);q['failure_controls']=controls(r,m,root)
    for name,x in [('inventory.json',r),('manifest.json',m),('classification.json',q)]:(out/name).write_text(json.dumps(x,indent=2)+'\n')
    print('DISPLAY CLASSIFICATION PASS:13993mutable/2048immutable bits,14damaged schema controls rejected')
