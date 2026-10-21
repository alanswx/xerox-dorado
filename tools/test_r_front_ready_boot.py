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
import shutil
import subprocess
import time

ROOT = Path(__file__).resolve().parents[1]
SEED = 'f3bce2a8566b3bac3dd793b2016910c71b0c8d7b061b9d48c14261665802df51'
OBSERVER = r'''
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


def prepare(parent, native_io, out, cycles, inspect_at):
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
    compile_command += ['+define+PILOT_PACK']
    if '-j' not in compile_command:
        compile_command += ['-j', '4']
    args = list(state['actions'][1]['command'])
    args[0] = str(out/'obj/test')
    args = [a for a in args if not a.startswith('+cycles=')]
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
    record = dict(scope=__doc__,parent=str(parent),
                  parent_provenance_sha256=sha(parent/'provenance.json'),
                  parent_status=state['status'], commands=[compile_command,args],
                  native_io_source=str(native_io),native_io_inputs=io_sources,
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
        assert 'LOADED EB PASS:' in text,'Original IM/IFUM readback did not pass'
        state.update(status='TERMINAL_PILOT_COMPONENT_DIAGNOSTIC',
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
    args=parser.parse_args()
    if args.prepare:
        assert args.parent and args.native_io
        prepare(args.parent.resolve(),args.native_io.resolve(),args.prepare.resolve(),args.cycles,args.inspect_at)
    else:
        run(args.run.resolve())
