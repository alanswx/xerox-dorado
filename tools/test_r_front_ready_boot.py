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

from stage_r_front_ready_native import canonical_visible_r_state, expose_native_visible_r

ROOT = Path(__file__).resolve().parents[1]
SEED = 'f3bce2a8566b3bac3dd793b2016910c71b0c8d7b061b9d48c14261665802df51'
OBSERVER = r'''
  bit boot_diag_handoff_trace=0;
  longint boot_diag_handoff_from=-1;
  longint boot_diag_task_samples[0:15];
  initial begin
    boot_diag_handoff_trace=$test$plusargs("handoff-trace");
    for(integer t=0;t<16;t++)boot_diag_task_samples[t]=0;
  end
  always @(negedge sys_clk)if(boot_diag_handoff_trace && boot_diag_world)begin
    if(boot_diag_handoff_from<0)boot_diag_handoff_from=n_cyc2;
    boot_diag_task_samples[pct_ctask]++;
    if(n_cyc2<boot_diag_handoff_from+12000)
      $display("BOOT_DIAG_HANDOFF sample=%0d CIA=%h task=%h CTD=%h clocks=%b%b%b T=%h A=%h B=%h ALU=%h MD=%h VA=%h PEnc=%h BNT=%h StopTasks=%b TaskOffN=%b Hold=%b KReady=%b SayNotReady=%b IfuHold=%b Ref=%b Ack=%b",
        n_cyc2,ring_cia,pct_ctask,ring_ctd,m.b_ContA.clk0_p_Ca,m.b_ContA.clk1_p_Ca,
        m.b_ContA.clk2_p_Ca,brc_t,brc_alua,brc_alub,brc_alu,bootio_md,w_procva,
        w_penc,w_bnt,m.b_ContA.StopTasks,m.b_ContA.TaskingIsOff_p_,m.Hold,
        m.b_IFU.KReady,m.b_IFU.SayNotReady,m.IfuHold,m.b_IFU.IfuMemRef,m.b_IFU.IfuMemAck);
  end
  final if(boot_diag_handoff_trace)begin
    for(integer t=0;t<16;t++)begin
      $display("BOOT_DIAG_TASK_SAMPLES task=%h samples=%0d",t[3:0],boot_diag_task_samples[t]);
      $display("BOOT_DIAG_TPC_RAW task=%h l=%h i=%h j=%h k=%h",t[3:0],
        m.b_ContA.u_l13.mem[15-t],m.b_ContA.u_i13.mem[15-t],
        m.b_ContA.u_j13.mem[15-t],m.b_ContA.u_k13.mem[15-t]);
    end
  end
  // Read-only phase-by-phase operands. PCT fires at a CIA change, before
  // the operand muxes have settled, and cannot prove a consumed MD value.
  longint boot_diag_settled_from=-1;
  wire [15:0] boot_diag_r_value={R_VALUE_BITS};
  wire [15:0] boot_diag_dr_value={DR_VALUE_BITS};
  wire [15:0] boot_diag_drm_value={DRM_VALUE_BITS};
  initial void'($value$plusargs("settled-from=%d",boot_diag_settled_from));
  always @(negedge sys_clk)
    if(boot_diag_settled_from>=0 && n_cyc2>=boot_diag_settled_from &&
       n_cyc2<boot_diag_settled_from+6000)
      $display("BOOT_DIAG_PHASE sample=%0d CIA=%h task=%h clocks=%b%b%b T=%h A=%h B=%h ALU=%h MD=%h VA=%h RM00=%h BP=%h Hold=%b BMux=%h Mapbuf=%h written=%h MapLd=%b Pair=%b PairClk=%b MapDec=%b MapClk=%b MapEn=%b MapGate=%b MapC07=%b FH=%b preC1=%b MapWE=%b MapRAS=%b MapCAS=%b pending=%b addr=%h R=%h dR=%h dRm=%h RBypass=%b%b RReadClk=%b%b RVisibleClk=%b%b BClk=%b%b BMuxH=%b%b%b BMuxL=%b%b%b Root=%b",
        n_cyc2,ring_cia,pct_ctask,m.b_ContA.clk0_p_Ca,m.b_ContA.clk1_p_Ca,
        m.b_ContA.clk2_p_Ca,brc_t,brc_alua,brc_alub,brc_alu,bootio_md,w_procva,
        bootio_rm(8'h00),bootio_rm(8'h11),m.Hold,map_probe_b,map_probe_q,cedar_map_written,
        m.b_MemX.MapbufLd_p_a,m.Map_u_InPair_p_,m.b_MemC.LdPair_p_,m.b_MemC.Map_u__p_,
        m.b_MemX.Clk1_p_Aa,m.b_MemX.u_c02.ck_en,m.b_MemX.MapbufLd_p_c,m.b_MemX.u_c07.p3,
        m.b_MemX.preFH_p_A,m.b_MemX.preClk1_p_A,m.b_MemX.RTMapWE_p_a,
        m.b_MemX.RTMapRAS_p_a,m.b_MemX.RTMapCAS_p_a,m.b_MemX.u_a04.write_pending,
        m.b_MemX.u_a04.addr,boot_diag_r_value,boot_diag_dr_value,boot_diag_drm_value,
        m.b_ProcH.RbBypassDly,m.b_ProcL.RbBypassDly,
        m.b_ProcH.Clock1_p_Bb,m.b_ProcL.Clock1_p_Bb,
        m.b_ProcH.PreSHCP_p_B,m.b_ProcL.PreSHCP_p_B,
        m.b_ProcH.Clock1_p_Ac,m.b_ProcL.Clock1_p_Ac,
        m.b_ProcH.BmuxEn_p_,m.b_ProcH.Bmux1_p_,m.b_ProcH.Bmux0_p_,
        m.b_ProcL.BmuxEn_p_,m.b_ProcL.Bmux1_p_,m.b_ProcL.Bmux0_p_,m.CLK_ca_p_);
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
for field,net in [('DRM_VALUE_BITS','dRm'),('DR_VALUE_BITS','dR'),('R_VALUE_BITS','R')]:
    OBSERVER=OBSERVER.replace(field,','.join(
        f'm.b_Proc{"H" if i<8 else "L"}.{net}_{i:02d}' for i in range(16)))

MEMORY_LINE_OBSERVER = r'''
  // Read-only visibility at the boot image's 0E10/1E10/2E10/3E10 lines.
  // Keep a short tail so a request remains visible after shared VA changes.
  integer boot_diag_memory_tail=0,boot_diag_memory_rows=0;
  wire [15:0] boot_diag_bp=bootio_rm(8'h11);
  wire [167:0] boot_diag_cache_address={ADDRESS_BANKS};
  wire [13:0] boot_diag_cache_we={WRITE_BANKS};
  wire [13:0] boot_diag_cache_ce={SELECT_BANKS};
  wire [17:0] boot_diag_d0={D0_BITS},boot_diag_d1={D1_BITS};
  always @(negedge sys_clk)if(paritysources_arm && !boot_diag_world)begin
    if((w_procva[25:14]==0 && w_procva[11:4]==8'he1) ||
       (pct_ctask==7 && boot_diag_bp[15:14]==0 && boot_diag_bp[11:4]==8'he1))
      boot_diag_memory_tail=32;
    else if(boot_diag_memory_tail>0)boot_diag_memory_tail--;
    if(boot_diag_memory_tail>0)begin
      boot_diag_memory_rows++;
      if(boot_diag_memory_rows>200000)
        $fatal(1,"DIAGNOSTIC_TRACE_LIMIT_REACHED: memory-line observer exceeded200000 rows; no guest-failure conclusion");
      $display("BOOT_DIAG_MEMORY_LINE sample=%0d CIA=%h task=%h CTD=%h clocks=%b%b%b T=%h A=%h B=%h ALU=%h MD=%h VA=%h BP=%h RMar=%h Hold=%b MDHoldN=%b RefHoldN=%b NoRef=%b D0=%h D1=%h raw=%h delayed=%h cache_addr=%h cache_we=%b cache_ce=%b",
        n_cyc2,ring_cia,pct_ctask,ring_ctd,m.b_ContA.clk0_p_Ca,m.b_ContA.clk1_p_Ca,
        m.b_ContA.clk2_p_Ca,brc_t,brc_alua,brc_alub,brc_alu,bootio_md,w_procva,
        boot_diag_bp,startup_rmar,m.Hold,m.b_MemC.MDhold_p_,m.b_MemC.RefHold_p_,m.b_MemC.NoRef,
        boot_diag_d0,boot_diag_d1,m.b_MemD.storage_rx_raw,m.b_MemD.storage_rx_data,
        boot_diag_cache_address,boot_diag_cache_we,boot_diag_cache_ce);
    end
  end
  final $display("BOOT_DIAG_MEMORY_LINE_ROWS %0d",boot_diag_memory_rows);
'''
for field,port in [('ADDRESS_BANKS','address'),('WRITE_BANKS','we_n'),('SELECT_BANKS','ce_n')]:
    MEMORY_LINE_OBSERVER=MEMORY_LINE_OBSERVER.replace(field,','.join(
        f'm.b_MemD.snapshot_bank_{i}.{port}'for i in reversed(range(14))))
for field,bank in [('D0_BITS',0),('D1_BITS',1)]:
    MEMORY_LINE_OBSERVER=MEMORY_LINE_OBSERVER.replace(field,','.join(
        f'm.b_MemD.D{bank}in_{i:02d}'for i in range(18)))


def storage_observer(start,length):
    """Observe the actual generic DRAM address-latch pins and resolved words."""
    declarations=[];names=[]
    for board,tag in [('msa','even'),('msaOdd','odd')]:
        for field,port,width in [('address',None,14),('pins','a',7),('ras','p4',1),('cas','p15',1)]:
            name='boot_diag_'+tag+'_'+field;names.append(name)
            values=[f'm.b_{board}.dram_addr_{i}'if port is None else
                    f'm.b_{board}.u_dram_addr_{i}.{port}'for i in reversed(range(16))]
            declarations.append(f'  wire [{16*width-1}:0] {name}={{'+','.join(values)+'};')
    declarations += ['  integer boot_diag_storage_rows=0;',
        '  always @(negedge sys_clk)begin',
        f'    if(n_cyc2>={start} && n_cyc2<{start+length})begin',
        '      boot_diag_storage_rows++;',
        '      $display("BOOT_DIAG_STORAGE_ADDRESS sample=%0d CIA=%h task=%h clocks=%b%b%b T=%h ALU=%h VA=%h RMar=%h Hold=%b raw=%h delayed=%h e_addr=%h e_pins=%h e_ras=%h e_cas=%h o_addr=%h o_pins=%h o_ras=%h o_cas=%h MemWE=%b%b",',
        '        n_cyc2,ring_cia,pct_ctask,m.b_ContA.clk0_p_Ca,m.b_ContA.clk1_p_Ca,m.b_ContA.clk2_p_Ca,',
        '        brc_t,brc_alu,w_procva,startup_rmar,m.Hold,m.b_MemD.storage_rx_raw,m.b_MemD.storage_rx_data,',
        '        '+','.join(names)+',m.MemWEa,m.MemWEb);',
        '    end',
        '  end',
        '  final $display("BOOT_DIAG_STORAGE_ADDRESS_ROWS %0d",boot_diag_storage_rows);']
    return '\n'.join(declarations)+'\n'


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def prepare(parent, native_io, out, cycles, inspect_at, probe_only=False, trace_from=None, trace_cycles=20000, settled_from=None, native_memory=False, handoff_only=False, native_ifu=False, handoff_trace=False, native_cpu_baseline=False, memory_line_trace=False, storage_from=None, storage_samples=4000, native_r_visible_from=None, canonical_native_r=False):
    assert not(probe_only and handoff_only),'Choose probe or original-image handoff gate'
    assert not canonical_native_r or native_r_visible_from is not None
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
    assert not memory_line_trace or native_memory
    assert storage_from is None or(native_memory and probe_only and 0<storage_samples<=20000)
    tb.write_text(body.replace('module tb_exec;', 'module tb_exec;\n'+control+OBSERVER+
                              (MEMORY_LINE_OBSERVER if memory_line_trace else '')+
                              (storage_observer(storage_from,storage_samples)if storage_from is not None else'')))
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
    ifu_sources={}
    if native_ifu:
        for source_name,digest in native['source_hashes'].items():
            if source_name.startswith('src/fpga/mister/snapshot_native_ifu/') and Path(source_name).suffix in ('.sv','.v'):
                assert sha(native_io/source_name)==digest
                name=source_name[4:];target=inputs/name
                target.parent.mkdir(parents=True,exist_ok=True)
                target.write_bytes((native_io/source_name).read_bytes());ifu_sources[name]=digest
        pattern=r'\bIFU_m_Rev_m_\w+ b_IFU \('
        assert len(re.findall(pattern,body))==1
        body=re.sub(pattern,'''IFU_full_snapshot_experiment b_IFU (
    .state_host_clk(sys_clk),.state_guest_advance(1'b1),
    .state_freeze(1'b0),.state_guest_held(1'b0),.state_request(1'b0),
    .state_write(1'b0),.state_index(18'b0),.state_data(18'b0),
    .state_frozen(),.state_done(),.state_error(),.state_result(),''',body)
        generic=(inputs/'verilog/generated/IFU-Rev-Ch.v').read_text()
        packed=(inputs/'fpga/mister/snapshot_native_ifu/IFU_full_snapshot.sv').read_text()
        mapping={}
        def ifum_reference(match):
            package,address=match.groups()
            instance=re.search(r'cell_F10415A '+package+r' \((.*?)\);',generic,re.S)
            assert instance,package
            output=re.search(r'\.p1\((\w+)\)',instance[1])[1]
            alias=re.search(r'assign '+output+r'=state_bank_q_(\d+)\[(\d+)\];',packed)
            assert alias,(package,output)
            bank,lane=alias.groups();mapping[package]=dict(bank=int(bank),lane=int(lane),original_output=output)
            return 'm.b_IFU.snapshot_bank_'+bank+'.mem['+address+']['+lane+']'
        for target in [tb,inputs/'verilog/generated/im_preload.vh']:
            updated=re.sub(r'm\.b_IFU\.(u_\w+)\.mem\[([^\]]+)\]',ifum_reference,target.read_text())
            target.write_text(updated)
        assert len(mapping)==27,len(mapping)
        (out/'native-ifum-readback-map.json').write_text(json.dumps(mapping,indent=2)+'\n')
    baseline_cpu_sources={}
    if native_cpu_baseline:
        assert state['status']=='PASS_MC1668_INITIAL_COMPONENT'
        for cpu_board,folder in [('ContA','snapshot_native_conta'),('ProcH','snapshot_native_processor'),('ProcL','snapshot_native_processor')]:
            name='fpga/mister/'+folder+'/'+cpu_board+'_full_snapshot.sv'
            # These are the preserved qualified MC1668 wrappers from the
            # immutable parent, rather than the new central replacement.
            assert sha(inputs/name)==provenance['inputs'][name]
            baseline_cpu_sources[name]=sha(inputs/name)
            pattern=r'\b'+cpu_board+r'_m_Rev_m_\w+ b_'+cpu_board+r' \('
            assert len(re.findall(pattern,body))==1,cpu_board
            body=re.sub(pattern,cpu_board+'''_full_snapshot_experiment b_'''+cpu_board+''' (
    .state_host_clk(sys_clk),.state_guest_advance(1'b1),
    .state_freeze(1'b0),.state_guest_held(1'b0),.state_request(1'b0),
    .state_write(1'b0),.state_index('0),.state_data(18'b0),
    .state_frozen(),.state_done(),.state_error(),.state_result(),''',body)
        # The native cell helpers copied above must be exactly the ones
        # paired with these older wrappers; do not silently change the oracle.
        for name in io_sources:
            if '/cell_' in name and name in provenance['inputs']:
                assert io_sources[name]==provenance['inputs'][name],name
    visible_r_sources={}
    if native_r_visible_from is not None:
        # Diagnostic only: test the identified physical R-output epoch with
        # the four ORIGINAL visible-R latches. No forced bus or map contents.
        # Central predicted R state remains an internal diagnostic duplicate;
        # this is not yet a reviewed production storage representation.
        assert not native_cpu_baseline and native_memory and native_ifu
        reference=json.loads((native_r_visible_from/'provenance.json').read_text())
        reference_state=json.loads((native_r_visible_from/'run-state.json').read_text())
        assert reference_state['status']=='PASS_MC1668_INITIAL_COMPONENT'
        helpers=[name for name in reference['inputs'] if name.endswith('/cell_MC10173_snapshot.v')]
        assert len(helpers)==1,helpers
        assert sha(inputs/helpers[0])==reference['inputs'][helpers[0]],helpers[0]
        for board in ['ProcH','ProcL']:
            name='fpga/mister/snapshot_native_processor/'+board+'_full_snapshot.sv'
            original_path=native_r_visible_from/'executed-inputs'/name
            assert sha(original_path)==reference['inputs'][name],name
            original=original_path.read_text();target=inputs/name;changed=target.read_text()
            packages=[]
            for package in ['u_g06','u_g07']:
                pattern=r'^  cell_MC10173_snapshot '+package+r' \(\n.*?^  \); // MC10173\n'
                matches=list(re.finditer(pattern,original,re.M|re.S))
                assert len(matches)==1,(board,package)
                replacement=r'^  // Qualified central storage replaces '+package+r'\.\n(?:  assign[^\n]+\n)+'
                assert len(re.findall(replacement,changed,re.M))==1,(board,package)
                changed=re.sub(replacement,lambda _:matches[0][0],changed,flags=re.M)
                packages.append(package)
            if canonical_native_r:
                changed=expose_native_visible_r(changed,board)
            target.write_text(changed)
            visible_r_sources[name]=dict(original_sha256=sha(original_path),packages=packages,
                                         resulting_sha256=sha(target))
        (out/'native-visible-r-diagnostic.json').write_text(json.dumps(dict(
            scope='Four original native R-output latch packages; hypothesis test only',
            reference=str(native_r_visible_from),sources=visible_r_sources,
            internal_predicted_R_duplicate=not canonical_native_r,
            evaluator_owned_Q_bits=736 if canonical_native_r else 752,
            production_qualified=False),indent=2)+'\n')
    if canonical_native_r:
        body=canonical_visible_r_state(body)
    top.write_text(body)
    (inputs/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    (inputs/'stage_r_front_ready_native.py').write_bytes((ROOT/'tools/stage_r_front_ready_native.py').read_bytes())
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
    compile_command += [name for name in sorted(ifu_sources) if name not in compile_command]
    compile_command += [name for name in sorted(baseline_cpu_sources) if name not in compile_command]
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
    if handoff_trace:
        args+=['+handoff-trace']
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
                  native_ifu_inputs=ifu_sources,handoff_trace=handoff_trace,
                  native_cpu_baseline_inputs=baseline_cpu_sources,
                  native_visible_r_sources=visible_r_sources,
                  canonical_native_r=canonical_native_r,
                  memory_line_trace=memory_line_trace,
                  storage_window_from=storage_from,storage_window_samples=storage_samples,
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
            if record.get('storage_window_from')is not None:
                assert 'BOOT_DIAG_STORAGE_ADDRESS 'in text,'No original storage samples'
                assert 'BOOT_DIAG_STORAGE_ADDRESS_ROWS '+str(record['storage_window_samples'])in text
            elif record.get('settled_from')is not None:
                assert text.count('BOOT_DIAG_PHASE ')==6000,'Incomplete bounded phase probe'
            else:
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
    parser.add_argument('--native-ifu',action='store_true')
    parser.add_argument('--handoff-trace',action='store_true')
    parser.add_argument('--native-cpu-baseline',action='store_true')
    parser.add_argument('--memory-line-trace',action='store_true')
    parser.add_argument('--storage-from',type=int)
    parser.add_argument('--storage-samples',type=int,default=4000)
    parser.add_argument('--native-r-visible-from',type=Path,help='Diagnostic: preserve original visible-R latches from a qualified native parent')
    parser.add_argument('--canonical-native-r',action='store_true',help='Use original visible-R latches as sole state owner, without a predicted duplicate')
    args=parser.parse_args()
    if args.prepare:
        assert args.parent and args.native_io
        prepare(args.parent.resolve(),args.native_io.resolve(),args.prepare.resolve(),args.cycles,args.inspect_at,
                args.probe_only,args.trace_from,args.trace_cycles,args.settled_from,args.native_memory,args.handoff_only,args.native_ifu,args.handoff_trace,args.native_cpu_baseline,args.memory_line_trace,args.storage_from,args.storage_samples,
                args.native_r_visible_from.resolve() if args.native_r_visible_from else None,args.canonical_native_r)
    else:
        run(args.run.resolve())
