#!/usr/bin/env python3
"""Extend an immutable Initial fixture through Ethernet and the Pilot disk.

This is a component diagnostic, not a native BaseBoard/SDRAM/ARM boot gate.
It retains the parent's original-ROM direct-entry setup, including its bench
display/terminal setup. Original microcode reads the serial disk cable; no
guest instructions or IOCBs are completed by the host. The pack is read-only
and bench disk writes are volatile. Use the same inputs for candidate/control.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path
import re
import shutil
import subprocess
import time

ROOT = Path(__file__).resolve().parents[1]
SEED = 'f3bce2a8566b3bac3dd793b2016910c71b0c8d7b061b9d48c14261665802df51'
OBSERVER = r'''
  // Read-only phase-by-phase operands. PCT fires at a CIA change, before
  // the operand muxes have settled, and cannot prove a consumed MD value.
  longint boot_diag_settled_from=-1;
  initial void'($value$plusargs("settled-from=%d",boot_diag_settled_from));
  always @(negedge sys_clk)
    if(boot_diag_settled_from>=0 && n_cyc2>=boot_diag_settled_from &&
       n_cyc2<boot_diag_settled_from+6000)
      $display("BOOT_DIAG_PHASE sample=%0d CIA=%h task=%h clocks=%b%b%b A=%h B=%h ALU=%h MD=%h VA=%h RM00=%h BP=%h Hold=%b",
        n_cyc2,ring_cia,pct_ctask,m.b_ContA.clk0_p_Ca,m.b_ContA.clk1_p_Ca,
        m.b_ContA.clk2_p_Ca,brc_alua,brc_alub,brc_alu,bootio_md,w_procva,
        bootio_rm(8'h00),bootio_rm(8'h11),m.Hold);
  // Passive breadcrumbs for the germ -> Mesa boundary. Never drives state.
  reg boot_diag_clock=0,boot_diag_world=0,boot_diag_germ=0;
  reg boot_diag_input_n=1;
  integer boot_diag_traps=0,boot_diag_inputs=0;
  always @(negedge sys_clk) begin
    if(paritysources_arm && boot_diag_input_n && !m.b_DskEth.bIOin_p_ &&
       et_tioa==8'o15 && boot_diag_inputs<640)begin
      boot_diag_inputs=boot_diag_inputs+1;
      $display("BOOT_DIAG_RX_CPU sample=%0d n=%0d task=%h CIA=%h IOB=%h EOP=%b T=%h R=%h MD=%h VA=%h",
        n_cyc2,boot_diag_inputs,pct_ctask,ring_cia,el2_iob,m.b_DskEth.EthData_18,
        brc_t,brc_r,bootio_md,w_procva);
    end
    boot_diag_input_n=m.b_DskEth.bIOin_p_;
    if(!boot_diag_clock && m.b_ContA.clk0_p_Ca && paritysources_arm)begin
      if(!boot_diag_world && ring_cia<12'hc00)begin
        boot_diag_world=1;
        $display("BOOT_DIAG_WORLD sample=%0d CIA=%h task=%h",n_cyc2,ring_cia,pct_ctask);
      end
      if(pct_ctask==0 && ring_cia==12'h7e0 && !boot_diag_germ)begin
        boot_diag_germ=1;
        $display("BOOT_DIAG_GERM sample=%0d T=%h R=%h MD=%h VA=%h",n_cyc2,brc_t,brc_r,bootio_md,w_procva);
      end
      if(boot_diag_world && pct_ctask==0 && ring_cia==12'h221 && boot_diag_traps<128)begin
        boot_diag_traps=boot_diag_traps+1;
        $display("BOOT_DIAG_TRAP sample=%0d n=%0d T=%h R=%h MD=%h VA=%h Link=%h Hold=%b",
          n_cyc2,boot_diag_traps,brc_t,brc_r,bootio_md,w_procva,ring_link,m.Hold);
      end
    end
    boot_diag_clock=m.b_ContA.clk0_p_Ca;
  end
'''


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def prepare(parent, native_io, out, cycles, inspect_at, probe_only=False, trace_from=None, trace_cycles=20000, settled_from=None, native_memory=False, handoff_only=False):
    assert not(probe_only and handoff_only),'Choose probe or original-image handoff gate'
    provenance = json.loads((parent/'provenance.json').read_text())
    state = json.loads((parent/'run-state.json').read_text())
    assert state['status'] in ('PASS_ACTUAL_NATIVE_BOARDS_INITIAL_COMPONENT',
                               'PASS_MC1668_INITIAL_COMPONENT')
    frozen = parent/'executed-inputs'
    for name, digest in provenance['inputs'].items():
        assert sha(frozen/name) == digest, name
    out.mkdir(parents=True, exist_ok=False)
    inputs = out/'executed-inputs'
    shutil.copytree(frozen, inputs)
    tb = inputs/'verilog/verilator/tb_exec.sv'
    body = tb.read_text()
    assert body.count('module tb_exec;') == 1
    # Older immutable parents predate the runtime limit option. Install the
    # observer's own explicit control instead of assuming today's root bench.
    control='''
  integer boot_diag_limit_fd,boot_diag_limit_rc,boot_diag_limit;
'''
    loop='''    for (j2 = 0; j2 < runcycles; j2 = j2 + 1) begin
      @(posedge sys_clk);'''
    assert body.count(loop)==1
    body=body.replace(loop,loop+'''
      if(j2%1000000==0)begin
        boot_diag_limit_fd=$fopen("'''+str(out/'cycle-limit.txt')+'''","r");
        if(boot_diag_limit_fd!=0)begin
          boot_diag_limit_rc=$fscanf(boot_diag_limit_fd,"%d",boot_diag_limit);
          $fclose(boot_diag_limit_fd);
          if(boot_diag_limit_rc==1 && boot_diag_limit>0)
            runcycles=boot_diag_limit>j2 ? boot_diag_limit : j2+1;
        end
      end''')
    tb.write_text(body.replace('module tb_exec;', 'module tb_exec;\n'+control+OBSERVER))
    # The generic DskEth fixture does not establish Cedar receive timing.
    # Bind the exact deployed normal-operation board/helpers for BOTH arms.
    # Its administration remains inactive, as in the actual native backplane.
    native=json.loads((native_io/'r-front-ready-native.json').read_text())
    assert native['state_admin']=='INACTIVE' and native['frequency_mhz']==50
    io_sources={}
    for name,digest in native['source_hashes'].items():
        if (name.startswith('src/fpga/mister/snapshot_native') and
            (name.startswith('src/fpga/mister/snapshot_native_dsketh/') or '/cell_' in name) and
            Path(name).suffix in ('.v','.sv')):
            assert sha(native_io/name)==digest,name
            relative=name[4:]
            target=inputs/relative
            target.parent.mkdir(parents=True,exist_ok=True)
            target.write_bytes((native_io/name).read_bytes())
            io_sources[relative]=digest
    board='fpga/mister/snapshot_native_dsketh/DskEth_full_snapshot.sv'
    assert board in io_sources
    top=inputs/'verilog/generated/dorado_full.v'
    body=top.read_text()
    needle='DskEth_m_Rev_m_Cf_rxmonitor #(.SYSPER(SYSPER)) b_DskEth ('
    assert body.count(needle)==1
    body=body.replace(needle,'''DskEth_full_snapshot_experiment #(.SYSPER(SYSPER)) b_DskEth (
    .state_host_clk(sys_clk),.state_guest_advance(1'b1),
    .state_freeze(1'b0),.state_guest_held(1'b0),.state_request(1'b0),
    .state_write(1'b0),.state_index(13'b0),.state_data(18'b0),
    .state_frozen(),.state_done(),.state_error(),.state_result(),''')
    memory_sources={}
    if native_memory:
        # Couple the same deployed memory controllers to the existing raw
        # storage-array model. This still omits native BaseBoard/SDRAM/ARM.
        # Keep all administrative inputs inactive, as in the speed core.
        for memory_board in ['MemC','MemD','MemX']:
            name='fpga/mister/snapshot_native_'+memory_board.lower()+'/'+memory_board+'_full_snapshot.sv'
            source=native_io/'src'/name
            digest=hashlib.sha256(source.read_bytes()).hexdigest()
            assert digest==native['source_hashes']['src/'+name]
            target=inputs/name;target.parent.mkdir(parents=True,exist_ok=True)
            target.write_bytes(source.read_bytes());memory_sources[name]=digest
            pattern=r'\b'+memory_board+r'_m_Rev_m_\w+(?: #\([^\n;]+\))? b_'+memory_board+r' \('
            assert len(re.findall(pattern,body))==1,memory_board
            body=re.sub(pattern,memory_board+'''_full_snapshot_experiment b_'''+memory_board+''' (
    .state_host_clk(sys_clk),.state_guest_advance(1'b1),
    .state_freeze(1'b0),.state_guest_held(1'b0),.state_request(1'b0),
    .state_write(1'b0),.state_index('0),.state_data(18'b0),
    .state_frozen(),.state_done(),.state_error(),.state_result(),''',body)
        for source_name,digest in native['source_hashes'].items():
            if any(source_name.startswith('src/fpga/mister/snapshot_native_'+b+'/')
                   for b in ['memc','memd','memx']) and Path(source_name).suffix in ('.sv','.v'):
                assert sha(native_io/source_name)==digest
                name=source_name[4:];target=inputs/name
                target.parent.mkdir(parents=True,exist_ok=True)
                target.write_bytes((native_io/source_name).read_bytes());memory_sources[name]=digest
        # Preserve the bench's existing cache seed in the native packed RAMs.
        # Derive each destination from original output-pin wiring and the
        # actual generated bank alias, rather than an outdated slot manifest.
        generic=(inputs/'verilog/generated/MemD-Rev-Ca.v').read_text()
        packed=(inputs/'fpga/mister/snapshot_native_memd/MemD_full_snapshot.sv').read_text()
        bench=tb.read_text();seed_map=[]
        def cache_seed(match):
            package,bit=match.groups()
            instance=re.search(r'cell_F10470 '+package+r' \((.*?)\);',generic,re.S)
            assert instance,package
            output=re.search(r'\.p1\((\w+)\)',instance[1])[1]
            alias=re.search(r'assign '+output+r'=state_bank_q_(\d+)\[(\d+)\];',packed)
            assert alias,(package,output)
            bank,lane=alias.groups();seed_map.append(dict(package=package,bank=int(bank),lane=int(lane),word_bit=int(bit)))
            return 'm.b_MemD.snapshot_bank_'+bank+'.mem[ci]['+lane+'] = cw['+bit+'];'
        bench=re.sub(r'm\.b_MemD\.(u_\w+)\.mem\[ci\] = cw\[(\d+)\];',cache_seed,bench)
        assert len(seed_map)==36,len(seed_map)
        tb.write_text(bench)
        (out/'native-cache-seed-map.json').write_text(json.dumps(seed_map,indent=2)+'\n')
    top.write_text(body)
    (inputs/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    pack = ROOT/'build/rtl-executive/cedar-work-native-hints.pack'
    assert sha(pack) == SEED
    eb = ROOT/'chm/dorado/CedarDorado.eb!6'
    replies = ROOT/'build/rtl-executive/cedar113-bol-replies.txt'
    compile_command = list(state['actions'][0]['command'])
    assert compile_command[0] == 'verilator'
    compile_command[compile_command.index('--Mdir')+1] = str(out/'obj')
    compile_command.remove('verilog/generated/DskEth-Rev-Cf.v')
    compile_command += [name for name in sorted(io_sources) if name not in compile_command]
    compile_command += [name for name in sorted(memory_sources) if name not in compile_command]
    if native_memory:
        qsf=native_io/'core/Dorado.qsf'
        assert sha(qsf)==native['source_hashes']['core/Dorado.qsf']
        assert 'VERILOG_MACRO "DORADO_STORAGE_INPUT_DELAY=2"' in qsf.read_text()
        assert '+define+DORADO_STORAGE_INPUT_DELAY=2' not in compile_command
        compile_command += ['+define+DORADO_STORAGE_INPUT_DELAY=2']
        saved=inputs/'native-build/Dorado.qsf';saved.parent.mkdir(parents=True,exist_ok=True)
        saved.write_bytes(qsf.read_bytes())
    compile_command += ['+define+PILOT_PACK']
    if '-j' not in compile_command:
        compile_command += ['-j', '4']
    args = list(state['actions'][1]['command'])
    args[0] = str(out/'obj/test')
    args = [a for a in args if not a.startswith('+cycles=') and a!='+fastwait']
    limit = out/'cycle-limit.txt'
    limit.write_text(str(cycles)+'\n')
    args += [f'+cycles={cycles}', '+cycle-limit-file='+str(limit),
             '+terminal-key=21', '+etherhost=34', '+largeformat',
             '+loaded-eb='+str(eb), '+rcvplay='+str(replies), '+rcvdelay=100000',
             '+pack='+str(pack), '+check-cedar-mapwrites', '+frame',
             '+bootio-germ-copy', '+bootiofrom=0', '+rxtrace']
    bench_text=tb.read_text()
    assert '"inspect-state=%s"' in bench_text and '"inspect-at=%d"' in bench_text
    args += ['+inspect-state='+str(out/'register-map-inspection.txt'),
             '+inspect-at='+str(inspect_at)]
    if trace_from is not None:
        assert '"pcfrom=%d"' in bench_text and '"pclen=%d"' in bench_text
        args += ['+pcfrom='+str(trace_from),'+pclen='+str(trace_cycles)]
    if probe_only:
        args=[arg for arg in args if not arg.startswith('+loaded-eb=') and
              arg not in ('+check-cedar-mapwrites',)]
    if handoff_only:
        args=[arg for arg in args if arg!='+check-cedar-mapwrites']
    if settled_from is not None:
        args += ['+settled-from='+str(settled_from)]
    record = dict(scope=__doc__,parent=str(parent),
                  parent_provenance_sha256=sha(parent/'provenance.json'),
                  parent_status=state['status'], commands=[compile_command,args],
                  probe_only=probe_only,trace_from=trace_from,trace_cycles=trace_cycles,
                  handoff_only=handoff_only,
                  settled_from=settled_from,
                  native_io_source=str(native_io),native_io_inputs=io_sources,
                  native_memory_inputs=memory_sources,
                  native_io_manifest_sha256=sha(native_io/'r-front-ready-native.json'),
                  external_inputs={str(p.relative_to(ROOT)):sha(p)
                                   for p in (pack, eb, replies)},
                  inputs={str(p.relative_to(inputs)):sha(p)
                          for p in inputs.rglob('*') if p.is_file()})
    with gzip.open(out/'provenance.json.gz', 'wt') as stream:
        json.dump(record, stream, indent=2)
    print('Prepared Pilot component diagnostic',out,flush=True)


def run(out):
    with gzip.open(out/'provenance.json.gz','rt') as stream:
        record=json.load(stream)
    inputs=out/'executed-inputs'
    for name,digest in record['inputs'].items():
        assert sha(inputs/name)==digest,name
    for name,digest in record['external_inputs'].items():
        assert sha(ROOT/name)==digest,name
    path=out/'run-state.json'
    assert not path.exists(),'Inspect the existing process; never blindly restart'
    state=dict(status='RUNNING',driver_pid=__import__('os').getpid(),actions=[])
    try:
        for command,label in zip(record['commands'],('compile','test')):
            with (out/(label+'.log')).open('x') as stream:
                process=subprocess.Popen(command,cwd=inputs,stdout=stream,
                                         stderr=subprocess.STDOUT)
                action=dict(stage=label,pid=process.pid,command=command,started=time.time())
                state['actions'].append(action)
                path.write_text(json.dumps(state,indent=2)+'\n')
                code=process.wait()
                action.update(returncode=code,finished=time.time())
                path.write_text(json.dumps(state,indent=2)+'\n')
                assert code==0,(label,out/(label+'.log'))
        text=(out/'test.log').read_text()
        if record.get('probe_only'):
            assert (out/'register-map-inspection.txt').exists(),'Read-only inspection missing'
            assert 'BOOT_DIAG_RX_CPU ' in text,'No CPU receive samples'
        else:
            assert 'LOADED EB PASS:' in text,'Original IM/IFUM readback did not pass'
        state.update(status='TERMINAL_RECEIVE_FILTER_PROBE' if record.get('probe_only') else
                     'TERMINAL_PILOT_COMPONENT_DIAGNOSTIC',
                     native_firmware_qualified=False,desktop_qualified=False,
                     germ_entry_observed='BOOT_DIAG_GERM ' in text,
                     trap_entries=text.count('BOOT_DIAG_TRAP '))
    except BaseException:
        state['status']='TERMINAL_FAILED'
        raise
    finally:
        path.write_text(json.dumps(state,indent=2)+'\n')
    print(state['status'],flush=True)


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    mode=parser.add_mutually_exclusive_group(required=True)
    mode.add_argument('--prepare',type=Path)
    mode.add_argument('--run',type=Path)
    parser.add_argument('--parent',type=Path)
    parser.add_argument('--native-io',type=Path)
    parser.add_argument('--cycles',type=int,default=300000000)
    parser.add_argument('--inspect-at',type=int,default=60000000)
    parser.add_argument('--probe-only',action='store_true',help='Bounded receive diagnostic; never a Cedar boot qualification')
    parser.add_argument('--trace-from',type=int)
    parser.add_argument('--trace-cycles',type=int,default=20000)
    parser.add_argument('--settled-from',type=int)
    parser.add_argument('--native-memory',action='store_true')
    parser.add_argument('--handoff-only',action='store_true',help='Require original IM/IFUM readback and entry; no Cedar map-write gate')
    args=parser.parse_args()
    if args.prepare:
        assert args.parent and args.native_io
        prepare(args.parent.resolve(),args.native_io.resolve(),args.prepare.resolve(),args.cycles,args.inspect_at,
                args.probe_only,args.trace_from,args.trace_cycles,args.settled_from,args.native_memory,args.handoff_only)
    else:
        run(args.run.resolve())
