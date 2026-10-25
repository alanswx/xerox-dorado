#!/usr/bin/env python3
"""Turn the qualified read-only ALU view into ordinary synthesis connections.

Resolve physical package observations through their exact connected pins.
Export cross-module scalar observations explicitly instead of relying on
hierarchical references, which Quartus17 rejects. No state, delays, assumptions
or timing exceptions are added. The original projection remains the oracle.
"""
import argparse
import json
from pathlib import Path
import re

from test_external_r_packet import sha


def split_arguments(text):
    pieces=[];start=0;depth=0
    for i,c in enumerate(text):
        if c in '([{':depth+=1
        elif c in ')]}':depth-=1
        elif c==','and depth==0:pieces.append(text[start:i].strip());start=i+1
        assert depth>=0
    assert depth==0
    pieces.append(text[start:].strip())
    return pieces


def ports(header):
    result=[];direction=None;width=1
    for token in split_arguments(header):
        m=re.fullmatch(r'(?:(input|output)\s+(?:wire|reg)\s*(?:\[(\d+):0\])?\s*)?(\w+)',token)
        assert m,token
        if m[1]:direction=m[1];width=int(m[2])+1 if m[2]else 1
        assert direction
        result.append((direction,m[3],width))
    return result


def lower(raw):
    graphs={}
    for m in re.finditer(r'module (alu_projection_\w+)\((.*?)\);(.*?)\nendmodule',raw,re.S):
        name,header,body=m[1],m[2],m[3]
        instances={}
        for call in re.finditer(r'(?m)^ +(?P<kind>alu_\w+)(?: #\([^\n]+\))? (?P<name>\w+)\(',body):
            start=call.end();i=start;depth=1
            while depth:
                if body[i]=='(':depth+=1
                elif body[i]==')':depth-=1
                i+=1
            assert body[i]==';'
            instances[call['name']]=dict(kind=call['kind'],text=body[call.start():i+1],
                arguments=split_arguments(body[start:i-1]),extra={})
        graphs[name]=dict(original=m[0],header=header,body=body,ports=ports(header),instances=instances,
                          exports={},wires={},substitutions={})
    assert len(graphs)==7
    observations=[]

    def connections(instance):
        args=instance['arguments']
        if args[0].startswith('.'):
            result={}
            for arg in args:
                m=re.fullmatch(r'\.(\w+)\((.*)\)',arg,re.S);assert m,arg
                result[m[1]]=m[2]
            return result
        child=graphs[instance['kind']]
        assert len(args)==len(child['ports'])
        return dict(zip([n for _,n,_ in child['ports']],args))

    def observe(name,reference):
        graph=graphs[name]
        if '.'not in reference:
            assert re.fullmatch(r'\w+',reference),reference
            assert re.search(r'\bwire '+re.escape(reference)+r'\s*;',graph['body']),reference
            return reference
        instance_name,field=reference.split('.',1)
        instance=graph['instances'][instance_name];kind=instance['kind']
        if kind in graphs:
            child=graphs[kind]
            expression=observe(kind,field)
            if expression not in child['exports']:
                child['exports'][expression]='physical_observation_'+str(len(child['exports']))
            port=child['exports'][expression]
            if port not in instance['extra']:
                wire='physical_'+instance_name+'_'+port
                instance['extra'][port]=wire;graph['wires'][wire]=1
            return instance['extra'][port]
        pins=connections(instance)
        if field in ['ca','cb']:
            assert kind in ['alu_sampled_MC10231','alu_sampled_SE10231']
            value='('+pins['p9']+'|'+pins['p6'if field=='ca'else'p11']+')'
        elif field=='clk':
            assert kind in ['alu_sampled_F10000','alu_sampled_F10000_gated','alu_sampled_F10000_ready']
            value='('+pins['p4']+'|'+pins['p13']+')'
        elif re.fullmatch(r'state_q\[\d+\]',field):
            index=int(re.fullmatch(r'state_q\[(\d+)\]',field)[1])
            part=re.fullmatch(r'state_q\[(\d+)\+:(\d+)\]',pins['state_q']);assert part
            assert index<int(part[2]);value=f'state_q[{int(part[1])+index}]'
        else:
            assert re.fullmatch(r'p\d+',field),field
            value='('+pins[field]+')'
        observations.append(dict(module=name,reference=reference,connected_expression=value))
        return value

    pattern=r'\b\w+(?:\.\w+)+(?:\[\d+\])?'
    for name,graph in graphs.items():
        code=re.sub(r'//[^\n]*','',graph['body'])
        for reference in sorted(set(re.findall(pattern,code)),key=lambda value:(-len(value),value)):
            graph['substitutions'][reference]=observe(name,reference)
    for name,graph in graphs.items():
        header=graph['header'];body=graph['body']
        for expression,port in graph['exports'].items():header+=',output wire '+port
        for instance in graph['instances'].values():
            if not instance['extra']:continue
            child=graphs[instance['kind']];args=list(instance['arguments'])
            for port in child['exports'].values():
                # A port observed by another instance still needs an empty
                # connection here; positional lists must retain exact order.
                value=instance['extra'].get(port,'')
                args.append('.'+port+'('+value+')'if args[0].startswith('.')else value)
            call=instance['text'];opening=call.index('(')
            # Projection graph instances have no parameter list.
            assert '#'not in call
            new=call[:opening+1]+',\n '.join(args)+');'
            assert body.count(call)==1;body=body.replace(call,new,1)
        def replace_line(line):
            code,separator,comment=line.partition('//')
            code=re.sub(pattern,lambda m:graph['substitutions'].get(m[0],m[0]),code)
            assert not re.search(pattern,code),(name,code)
            return code+separator+comment
        body='\n'.join(replace_line(line)for line in body.split('\n'))
        body='\n'+''.join(' wire '+wire+';\n'for wire in graph['wires'])+body
        body+=''.join(' assign '+port+'='+expression+';\n'for expression,port in graph['exports'].items())
        replacement='module '+name+'('+header+');'+body+'\nendmodule'
        raw=raw.replace(graph['original'],replacement,1)
    start=' for(genvar i=0;i<16;i=i+1) begin: map_t_file\n'
    end=' end\n alu_projection_r_front_control_conta c('
    assert raw.count(start)==raw.count(end)==1
    raw=raw.replace(start,' genvar i;\n generate\n for(i=0;i<16;i=i+1) begin: map_t_file\n',1)
    raw=raw.replace(end,' end\n endgenerate\n alu_projection_r_front_control_conta c(',1)
    return raw,dict(primitive_observations=observations,
                    explicit_exports={n:g['exports']for n,g in graphs.items()},
                    no_state_or_timing_logic_added=True)


def generate(source,out):
    assert not out.exists();out.mkdir(parents=True)
    original=(source/'projection.sv').read_text();raw,details=lower(original)
    (out/'projection.sv').write_text(raw)
    record=dict(scope=__doc__,source=str(source),source_sha256=sha(source/'projection.sv'),
                output_sha256=sha(out/'projection.sv'),details=details,
                native_capture_or_physical_timing_qualified=False)
    (out/'manifest.json').write_text(json.dumps(record,indent=2)+'\n')
    return record


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();generate(a.source.resolve(),a.output.resolve())
