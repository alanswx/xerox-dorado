#!/usr/bin/env python3
"""Independent BaseBoard serial/parallel source, state, memory/firmware and timing audit."""
import argparse
from collections import Counter
import json
from pathlib import Path
import re
import shutil
import tempfile
import audit_baseboard_full as qualified
import fit_baseboard_full as reference_fit
import fit_baseboard_ff_scan as fit
import generate_baseboard_ff_scan as gen


def memories(case,manifest):
    """Reconstruct only named firmware MIF paths; never archive Quartus DBs."""
    report=(case/'snapshot_fit.fit.rpt').read_bytes()
    rows=qualified.table(report.decode('latin1'),'Fitter RAM Summary','Name')
    images=[r['MIF'] for r in rows if r['Mode']=='ROM']
    assert len(images)==4 and len({Path(n).name for n in images})==4
    assert {p.name for p in (case/'synthesized-images').iterdir()}=={Path(n).name for n in images},'synthesized image inventory differs'
    with tempfile.TemporaryDirectory(prefix='baseboard-scan-memory-',dir='/tmp')as tmp:
        out=Path(tmp)
        (out/'snapshot_fit.fit.rpt').write_bytes(report)
        for n in images:
            q=out/n;q.parent.mkdir(parents=True,exist_ok=True)
            shutil.copyfile(case/'synthesized-images'/Path(n).name,q)
        for row in manifest['packages']:
            if row['type']=='EPROM' and row['init_file']:
                q=out/row['init_file'];q.parent.mkdir(parents=True,exist_ok=True)
                shutil.copyfile(case/row['init_file'],q)
        return qualified.memories(out,'snapshot',manifest)


def physical(mapping,manifest,variant):
    rows=qualified.table(mapping,'Analysis & Synthesis Resource Utilization by Entity','Compilation Hierarchy Node')
    def entity(name):
        return [r for r in rows if r['Entity Name']==name]
    ff={r['package']:r for r in manifest['packages'] if r['type']=='FF'}
    seen=set()
    for row in rows:
        match=re.search(r'\|(cell_\w+_snapshot):(u_\w+)\|?$',row['Full Hierarchy Name'])
        if not match or match[2] not in ff:continue
        name=match[2];assert name not in seen;seen.add(name)
        wanted=ff[name]['logical_state_bits']
        assert match[1]==ff[name]['kind']+'_snapshot'
        assert row['Dedicated Logic Registers']==f'{wanted} ({wanted})','physical FF retention differs: '+name
    assert seen==set(ff)
    cpu=entity('chip_6502_snapshot');assert len(cpu)==1
    wanted=1744 if variant=='baseline' else 1725
    assert cpu[0]['Dedicated Logic Registers']==f'{wanted} ({wanted})','CPU node/response retention differs'
    riots=entity('M6532_snapshot');assert len(riots)==5
    for row in riots:
        assert row['Dedicated Logic Registers']=='1115 (1115)','RIOT physical/response retention differs'
        assert row['Block Memory Bits']=='0','RIOT RAM policy differs'
    wrappers=entity('cell_MCS6532_snapshot');assert len(wrappers)==5
    for row in wrappers:assert row['Dedicated Logic Registers']=='1118 (3)','RIOT phase/response retention differs'
    eproms=entity('cell_i2716_snapshot');assert len(eproms)==8
    for row in eproms:assert row['Dedicated Logic Registers']=='19 (19)','EPROM canonical pipeline/response retention differs'
    srams=entity('dorado_snapshot_f10415_word_bank');assert len(srams)==4
    for row in srams:
        match=re.search(r'snapshot_sram_([0-3])\|',row['Full Hierarchy Name']);assert match
        # Exact hierarchy ownership of saved_q and shared private flags.
        # Read latency and raw pipeline semantics are checked separately.
        own=11 if match[1]=='0' else 10 if variant=='snapshot' and match[1]=='3' else 9
        assert row['Dedicated Logic Registers']==f'{own} ({own})','SRAM saved pipeline/transport retention differs'
        assert row['Block Memory Bits']=='8192'
    parent=entity(gen.MODULE);assert len(parent)==1
    own=27 if variant=='baseline' else 11
    assert re.fullmatch(r'\d+ \('+str(own)+r'\)',parent[0]['Dedicated Logic Registers']),'board response/boundary retention differs'
    controllers=entity('dorado_ff_scan_controller')
    assert len(controllers)==(1 if variant=='snapshot' else 0)
    # first_bit=index*18 always has bit0=0: 64 XML bits become 63 FFs.
    if controllers:assert controllers[0]['Dedicated Logic Registers']=='63 (63)','scan controller retention differs'
    assert re.findall(r'; READ_DURING_WRITE_MODE_MIXED_PORTS\s*; (\w+)\s*;',mapping)==['OLD_DATA']*4+['DONT_CARE']*4,'RAM/ROM macro collision differs'
    return dict(physical_ff_bits=279,physical_cpu_nodes=1725,physical_riot_bits=5490,
        physical_sram_bits=32800,physical_eprom_pipeline_bits=72,
        cpu=cpu,riots=riots,riot_wrappers=wrappers,eproms=eproms,srams=srams,parent=parent,controllers=controllers)


def audit(directory,root):
    record=json.loads((directory/'provenance.json').read_text())
    assert record['scan_driver_sha256']==fit.sha(Path(fit.__file__)),'executed runner differs'
    for n,h in record['compiled_files'].items():
        if not n.endswith('.qsf'):assert fit.sha(directory/n)==h,'compiled input differs: '+n
    old,old_manifest,old_clones=gen.reference(root);new,manifest,clones=gen.generate(root)
    assert record['manifest']==manifest and record['reference_manifest']==old_manifest
    results={}
    with tempfile.TemporaryDirectory(prefix='baseboard-scan-reference-',dir='/tmp')as tmp:
        expected=Path(tmp)/'fit';reference_fit.prepare(root,expected)
        original_shell=(expected/'baseline/top.sv').read_text()
        for variant,board,helpers in [('baseline',old,old_clones),('snapshot',new,clones)]:
            case=directory/variant
            qsf=(expected/variant/'snapshot_fit.qsf').read_text()
            if variant=='baseline':qsf+='set_global_assignment -name VERILOG_MACRO "SNAPSHOT_BANK_EXPERIMENT=1"\n'
            else:qsf+='set_global_assignment -name SYSTEMVERILOG_FILE ff_scan_controller.sv\n'
            for port in ('system_reset','scan_hold_required','scan_fault'):qsf+='set_instance_assignment -name VIRTUAL_PIN ON -to '+port+'\n'
            assert qualified.normalized((case/'snapshot_fit.qsf').read_text())==qualified.normalized(qsf),'project policy differs'
            assert (case/'top.sv').read_text()==fit.shell(original_shell,variant=='snapshot'),'clock/shell differs'
            for n in ('bank.sdc','audit.tcl'):assert (case/n).read_bytes()==(expected/variant/n).read_bytes(),'timing policy differs'
            assert (case/'BaseBd_snapshot.sv').read_bytes()==board.encode(),'board source differs'
            assert fit.sha(case/'BaseBd_original.v')==gen.qualified.BOARD_SHA
            for n,h in manifest['original_sources'].items():assert fit.sha(case/Path(n).name)==h,'original cell/vendor differs'
            for n,s in helpers.items():assert (case/n).read_bytes()==gen.raw_bytes(s),'helper differs: '+n
            summary=(case/'snapshot_fit.fit.summary').read_text();assert 'Fitter Status : Successful' in summary
            resources={}
            for key,label in [('alms',r'Logic utilization \(in ALMs\)'),('registers','Total registers'),('ram_bits','Total block memory bits'),('ram_blocks','Total RAM Blocks')]:
                match=re.search(label+r' : ([\d,]+)',summary);assert match
                resources[key]=int(match[1].replace(',',''))
            assert resources['ram_bits']==98304,'mutable SRAM/firmware bits differ'
            # BOTH variants use the qualified grouped SRAM and shared RIOT.
            memory=memories(case,old_manifest)
            assert resources['ram_blocks']==len(memory['unique_m10k_sites']),'M10K count differs'
            mapping=(case/'snapshot_fit.map.rpt').read_text(encoding='latin1')
            state=physical(mapping,old_manifest,variant)
            assert {p.name[len('bank-setup-'):-4] for p in case.glob('bank-setup-*.rpt')}==qualified.CORNERS
            timing={}
            for corner in sorted(qualified.CORNERS):
                timing[corner]={}
                for mode in ('setup','hold','mpw'):
                    text=(case/f'bank-{mode}-{corner}.rpt').read_text(encoding='latin1')
                    match=re.search(r'\((\d+) violated\)\.\s+Worst case slack is ([-\d.]+)',text)
                    assert match and int(match[1])==0 and float(match[2])>=0,'timing violation: '+mode
                    timing[corner][mode]=float(match[2])
                for mode in ('recovery','removal'):assert (case/f'bank-{mode}-{corner}.rpt').read_text().strip().endswith('Nothing to report.')
                text=(case/f'bank-ucp-{corner}.rpt').read_text()
                for name in ('Illegal Clocks','Unconstrained Clocks','Unconstrained Input Ports','Unconstrained Input Port Paths','Unconstrained Output Ports','Unconstrained Output Port Paths'):
                    assert re.search('; '+name+r'\s*; 0\s*; 0\s*;',text),'unconstrained coverage: '+name
                assert re.search(r'; host_clock\s*; Base\s*; 20\.000\s*;',
                                 (case/f'bank-clocks-{corner}.rpt').read_text()),'host clock differs'
            for kind,pkg,param,value in [(gen.MODULE,'board','SYSPER',4),('cell_CLOCKGEN_snapshot','u_clockgen','N',4)]:
                match=re.search(r'; Parameter Settings for User Entity Instance: [^\n;]*\b'+kind+r':[^\n]*\b'+pkg+r'\s*;(.*?)(?:Note:)',mapping,re.S)
                assert match and re.search('; '+param+r'\s*; '+str(value)+r'\s*;',match[1]),'actual clock parameter differs'
            warnings={}
            for phase in ('map','fit'):
                text=(case/(phase+'.log')).read_text();assert 'Critical Warning' not in text
                warnings[phase]=dict(Counter(re.findall(r'Warning \((\d+)\)',text)))
            results[variant]=dict(resources=resources,state_retention=state,memory=memory,timing=timing,warnings=warnings)
    for phase in ('map','fit'):
        extra=Counter(results['snapshot']['warnings'][phase])-Counter(results['baseline']['warnings'][phase])
        assert not(set(extra)-{'10036'}),'unreviewed warning increase: '+str(extra)
    return dict(status='PASS_ISOLATED_FIT',scope=__doc__,results=results,
        delta={k:results['snapshot']['resources'][k]-results['baseline']['resources'][k] for k in ('alms','registers','ram_bits','ram_blocks')},
        minimum_snapshot_slack_ns=min(v for c in results['snapshot']['timing'].values() for v in c.values()),
        limits='Isolated BaseBoard only; whole-core capacity/timing, global hold, machine snapshots and original60ns clocks remain unqualified.')


def controls(directory,root):
    cases=[('source','snapshot/BaseBd_snapshot.sv',' && !scan_busy','','compiled input differs'),
        ('cpu','snapshot/chip_6502_snapshot.sv','q[1724:1]','q[1724:2]','compiled input differs'),
        ('constraint','snapshot/bank.sdc','20.000','18.000','compiled input differs'),
        ('policy','snapshot/snapshot_fit.qsf','SNAPSHOT_BANK_EXPERIMENT=1','SNAPSHOT_BANK_EXPERIMENT=0','policy'),
        ('ff_retention','snapshot/snapshot_fit.map.rpt','25 (25)','24 (24)','physical FF retention'),
        ('cpu_retention','snapshot/snapshot_fit.map.rpt','1725 (1725)','1724 (1724)','CPU node'),
        ('riot_retention','snapshot/snapshot_fit.map.rpt','1115 (1115)','1114 (1114)','RIOT physical'),
        ('eprom_pipeline','snapshot/snapshot_fit.map.rpt','19 (19)','18 (18)','EPROM canonical'),
        ('hold','snapshot/bank-hold-7_slow_1100mv_-40c.rpt','(0 violated).','(1 violated).','timing violation'),
        ('old_data','snapshot/snapshot_fit.fit.rpt','Old data','New data','SRAM geometry'),
        ('old_data_macro','snapshot/snapshot_fit.map.rpt','; OLD_DATA','; NEW_DATA','RAM/ROM macro collision'),
        ('memory_site','snapshot/snapshot_fit.fit.rpt','M10K_X','MLAB_X','non-M10K'),
        ('clock','snapshot/top.sv','posedge host_clk','negedge host_clk','compiled input differs')]
    image=sorted((directory/'snapshot/synthesized-images').iterdir())[0]
    text=image.read_text();match=re.search(r'(?m)^\s*\d+\s*:\s*([01]{8});',text);assert match
    before=match[0];value=match[1];after=before.replace(value,('1' if value[0]=='0' else '0')+value[1:])
    cases.append(('synthesized_firmware',str(image.relative_to(directory)),before,after,'synthesized firmware differs'))
    results=[]
    for name,path,before,after,why in cases:
        with tempfile.TemporaryDirectory(prefix='baseboard-scan-bad-',dir='/tmp')as tmp:
            out=Path(tmp)/'fit';shutil.copytree(directory,out,ignore=shutil.ignore_patterns('db','incremental_db'))
            p=out/path;text=p.read_text(encoding='latin1');assert before in text,(name,before)
            p.write_text(text.replace(before,after),encoding='latin1')
            try:audit(out,root)
            except AssertionError as error:
                assert why in str(error),(name,error)
                results.append(dict(case=name,rejected=True,diagnostic=str(error)))
            else:raise AssertionError('damaged BaseBoard fit accepted: '+name)
    return results


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('directory',type=Path);p.add_argument('--source-root',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True);p.add_argument('--negative-controls',action='store_true')
    a=p.parse_args();result=audit(a.directory.resolve(),a.source_root.resolve())
    if a.negative_controls:result['failure_controls']=controls(a.directory.resolve(),a.source_root.resolve())
    a.output.write_text(json.dumps(result,indent=2)+'\n')
    print('BASEBOARD SERIAL FIT AUDIT PASS',result['delta'],'minimum slack',result['minimum_snapshot_slack_ns'])
