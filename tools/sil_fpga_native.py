#!/usr/bin/env python3
"""Assemble a cold-start FPGA machine from Sil boards, without saved state.

The BaseBoard owns processor clocks, power-on code, CP bus and IM loading.
External models are a terminal, serial Trident medium, nominal analog
comparators and raw DDR or SDRAM storage. No guest CPU or memory state is imported.
"""
import argparse
import json
from pathlib import Path
import re
import subprocess
import sys
from sil_backplane import MACHINE_CONFIG_TIES
from sil_to_verilog import vname


def sdram_wrapper(wrapper):
    """Replace only the raw backend in a canonical or frozen native wrapper."""
    for marker in ('    input wire ddram_busy,ddram_rvalid,',
                   '  wire backing_cache_hit;', '  assign rr=',
                   '  dorado_storage_ddram #(', '  dorado_native_backplane #('):
        if wrapper.count(marker) != 1:
            raise ValueError(f'Expected one native wrapper marker: {marker!r}')
    wrapper = wrapper.replace('    input wire ddram_busy,ddram_rvalid,', '''    output wire [12:0] sdram_a,
    output wire [1:0] sdram_ba,
    output wire sdram_ncs,sdram_nras,sdram_ncas,sdram_nwe,sdram_dqml,sdram_dqmh,sdram_cke,
    output wire [15:0] sdram_dq_out,
    output wire sdram_dq_oe,
    input wire [15:0] sdram_dq_in,
    input wire ddram_busy,ddram_rvalid,''')
    start = wrapper.index('  wire backing_cache_hit;') + len('  wire backing_cache_hit;\n')
    end = wrapper.index('  assign rr=', start)
    wrapper = wrapper[:start] + '''  wire physical_read,physical_write,read_return,sdram_initialized;
  // Same passive slots now count SDRAM commands/returns. The physical DDR
  // port stays idle. Raw counts still exclude power-on zero-fill.
  assign storage_events={1'b0,(sv && sr),(rv && !rr),1'b0,
    read_return,physical_write,physical_read,(rv && rr && rw),(rv && rr && !rw)};
  assign ddram_burstcount=0;assign ddram_addr=0;
  assign ddram_read=0;assign ddram_write=0;assign ddram_wdata=0;assign ddram_be=0;
''' + wrapper[end:]
    start = wrapper.index('  dorado_storage_ddram #(')
    end = wrapper.index('  dorado_native_backplane #(', start)
    wrapper = wrapper[:start] + '''`ifdef DORADO_HOST_CLOCK_HZ
  localparam integer STORAGE_CLOCK_HZ=`DORADO_HOST_CLOCK_HZ;
`else
  localparam integer STORAGE_CLOCK_HZ=33333333;
`endif
  dorado_storage_sdram #(.ADDR_BITS(RAW_BITS),.CLOCK_HZ(STORAGE_CLOCK_HZ)) u_sdram(
    .clk(clk_host),.reset(reset),
    .req_valid(storage_initialized ? rv : init_valid),.req_ready(backend_ready),
    .req_write(storage_initialized ? rw : 1'b1),
    .req_addr(storage_initialized ? ra : init_address),.req_data(storage_initialized ? wd : 16'd0),
    .rsp_valid(backend_response),.rsp_ready(storage_initialized ? sr : init_response_ready),.rsp_data(sd),
    .initialized(sdram_initialized),.cache_hit(backing_cache_hit),
    .physical_read(physical_read),.physical_write(physical_write),.read_return(read_return),
    .sdram_a(sdram_a),.sdram_ba(sdram_ba),.sdram_ncs(sdram_ncs),
    .sdram_nras(sdram_nras),.sdram_ncas(sdram_ncas),.sdram_nwe(sdram_nwe),
    .sdram_dqml(sdram_dqml),.sdram_dqmh(sdram_dqmh),.sdram_cke(sdram_cke),
    .dq_out(sdram_dq_out),.dq_oe(sdram_dq_oe),.dq_in(sdram_dq_in));
''' + wrapper[end:]
    return wrapper


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('-o', '--output', type=Path, required=True)
    parser.add_argument('--storage-modules', type=int, choices=range(1, 5), default=4)
    parser.add_argument('--storage-chip-kbits', type=int, choices=(16, 64), default=16)
    parser.add_argument('--map-kwords', type=int, choices=(16, 64), default=16)
    parser.add_argument('--storage-line-cache-bits', type=int, choices=range(0, 11), default=0,
                        help='optional raw DDR line buffer index bits; 0 disables (default)')
    parser.add_argument('--storage-backend', choices=('ddr', 'sdram'), default='ddr',
                        help='raw storage backend; SDRAM requires the128MB MiSTer module')
    parser.add_argument('--receive-monitor', action='store_true', help='passive DskEth receive-status observation')
    parser.add_argument('--instruction-monitor', action='store_true',
                        help='expose passive original ContA instruction-progress signals')
    parser.add_argument('--pilot-disk', action='store_true',
                        help='Pilot format: 28 usable of 29 slots, 10 label words, 256 data words')
    args = parser.parse_args()
    if args.storage_backend == 'sdram' and args.storage_line_cache_bits:
        parser.error('SDRAM has no DDR line buffer; use --storage-line-cache-bits 0')
    chip_address_bits = 14 if args.storage_chip_kbits == 16 else 16
    out = args.output
    out.mkdir(parents=True, exist_ok=True)
    boards = ['ProcH', 'ProcL', 'ContA', 'ContB', 'IFU', 'MemC', 'MemD', 'MemX',
              'DskEth', 'DispY', 'BaseBd', 'msa', 'msaOdd']
    for slot in range(1, args.storage_modules):
        boards += [f'msa{slot}', f'msa{slot}Odd']
    commands = [
        ['tools/sil_backplane.py', '--emit', str(out/'dorado_native_backplane.v'),
         '--module', 'dorado_native_backplane', '--boards', ','.join(boards),
         '--external-storage', '--nominal-environment', '--processor-monitor', '--display-field',
         '--export-net', "XmtData'"],
        ['tools/sil_to_verilog.py', 'chm/sil/BaseBd-Rev-Am.dm!1_/BaseBd-Rev-Am.wl',
         '--lc', 'chm/sil/BaseBd-Rev-Am.dm!1_/BaseBd-Rev-Am.lc',
         '--dict', 'chm/sil/msa-Rev-Bg.dm!1_/ecldict.analyze', '--nominal-environment',
         '-o', str(out/'BaseBd_nominal.v')],
        ['tools/sil_to_verilog.py', 'chm/sil/ContA-Rev-Cd.dm!1_/ContA-Rev-Cd.wl',
         '--lc', 'chm/sil/ContA-Rev-Cd.dm!1_/ContA-Rev-Cd.lc',
         '--dict', 'chm/sil/msa-Rev-Bg.dm!1_/ecldict.analyze', '--processor-monitor',
         '-o', str(out/'ContA_monitor.v')],
        ['tools/sil_to_verilog.py', 'chm/sil/DispY-Rev-Cl.dm!1_/DispY-Rev-Cl.wl',
         '--lc', 'chm/sil/DispY-Rev-Cl.dm!1_/DispY-Rev-Cl.lc',
         '--dict', 'chm/sil/msa-Rev-Bg.dm!1_/ecldict.analyze', '--display-field',
         '-o', str(out/'DispY_field.v')],
        ['tools/sil_storage_ddram.py', '-o', str(out/'msa_ddram.v')],
    ]
    if args.receive_monitor:
        commands[0].append('--receive-monitor')
        commands.append(['tools/sil_to_verilog.py', 'chm/sil/DskEth-Rev-Cf.dm!1_/DskEth-Rev-Cf.wl',
                         '--lc', 'chm/sil/DskEth-Rev-Cf.dm!1_/DskEth-Rev-Cf.lc',
                         '--dict', 'chm/sil/msa-Rev-Bg.dm!1_/ecldict.analyze',
                         '--receive-monitor', '-o', str(out/'DskEth_monitor.v')])
    if args.instruction_monitor:
        commands[0].append('--instruction-monitor')
        commands[2].append('--instruction-monitor')
    if chip_address_bits != 14:
        commands[0] += ['--storage-address-bits', str(chip_address_bits)]
        commands[4] += ['--chip-address-bits', str(chip_address_bits)]
    if args.map_kwords != 16:
        commands.append(['tools/sil_to_verilog.py',
                         'chm/sil/MemX-Rev-Ch.dm!1_/MemX-Rev-Ch.wl',
                         '--lc', 'chm/sil/MemX-Rev-Ch.dm!1_/MemX-Rev-Ch.lc',
                         '--dict', 'chm/sil/msa-Rev-Bg.dm!1_/ecldict.analyze',
                         '--map-kwords', str(args.map_kwords), '-o', str(out/'MemX_density.v')])
    for command in commands:
        subprocess.run([sys.executable, *command], check=True)
    source = (out/'dorado_native_backplane.v').read_text().split(');', 1)[0]
    ports = re.findall(r'^    (input|output)\s+wire\s+(\w+)\s*[,\n]', source, re.M)
    # Vector storage ports are connected separately below.
    bindings = {vname(name): value for name, value in MACHINE_CONFIG_TIES.items()}
    if chip_address_bits == 16:
        bindings[vname('ChipsAre256/16K')] = "1'b0"
        bindings[vname('ChipsAre64K')] = "1'b1"
    bindings.update(sys_clk='sys_clk', baseboard_boot_n='baseboard_boot_n',
        storage_clk='clk_host', CLK_InBase='base_clock_out', CLK_OutBase_p_='base_clock_out',
        PixelClkVCO='pixel_clk', RawPixelClk='pixel_clk', TermIsLF='large_format',
        OISData='term_ois', OISData_p_='~term_ois', RcvData='ethernet_rx', XmtData_p_='ethernet_tx_n',
        AltoTTLVideo='video', AltoHSync='hsync', AltoVSync_p_='vsync_n', HBlank='hblank',
        VBlank='vblank', HalfLine='halfline',
        ClockP0__in='drv_cp', ClockM0__in='drv_cm',
        DataP0__in="pack_writing ? 1'b0 : drv_dp", DataM0__in="pack_writing ? 1'b0 : drv_dm",
        DataP0='write_data_p', DataM0='write_data_m',
        TtlReady_p_='drv_ready_n', TtlOnLine_p_='drv_online_n', TtlTerm_p_='drv_term_n',
        TtlIndex_p_='drv_index_n', TtlReadOnly_p_='~pack_readonly', TtlEndOfCyl_p_='pack_eoc_n',
        SecIndx0_p_='drv_sec_n', Selected0_p_='drv_sel_n', Select0_p_='disk_sel_n',
        CylinderTag_p_='cyltag_n', HeadTag_p_='headtag_n', DriveTag_p_='drivetag_n', ContTag_p_='conttag_n')
    for i, name in enumerate(['000', '00', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9']):
        bindings[f'TagBus_{name}_p_'] = f'tag_n[{11-i}]'
    for i in range(8):
        bindings[f'Host_{i}'] = f'ethernet_host[{7-i}]'
    for i in range(1, 4):
        bindings[f'SecIndx{i}_p_'] = bindings[f'Selected{i}_p_'] = "1'b1"
    for name in list(bindings):
        if name.startswith('Mb') and name[2:].isdigit() and int(name[2:]) >= args.storage_modules:
            del bindings[name]
    connections = []
    for direction, name in ports:
        if name in bindings:
            connections.append(f'    .{name}({bindings[name]})')
        elif direction == 'input':
            # External control processor is absent: its active-low lines idle high.
            value = "1'b1" if name.startswith(('ACPABus_', 'ACPBus_', 'ACPStrb_')) else "1'b0"
            connections.append(f'    .{name}({value})')
    # A board added inside the backplane can consume a cable output, hiding
    # it from the default port heuristic. Never emit a native wrapper with
    # one of its required external device connections silently absent.
    required = {'XmtData_p_', 'RcvData', 'AltoTTLVideo', 'AltoHSync',
                'AltoVSync_p_', 'HBlank', 'VBlank', 'HalfLine', 'OISData',
                'DataP0', 'DataM0', 'Select0_p_', 'CylinderTag_p_',
                'HeadTag_p_', 'DriveTag_p_', 'ContTag_p_'}
    missing = required - {name for _, name in ports}
    if missing:
        raise ValueError(f'native device ports missing from backplane: {sorted(missing)}')
    groups = args.storage_modules*32
    address_bits = chip_address_bits + (groups-1).bit_length()
    connections += ['    .baseboard_debug(baseboard_debug)', '    .processor_debug(processor_debug)',
                    '    .control_debug(control_debug)',
                    '    .video_field(video_field)']
    if args.instruction_monitor:
        connections.append('    .instruction_signals(instruction_signals)')
    connections += [f'    .storage_{n}({v})' for n, v in [
        ('reset', 'reset || !storage_initialized'), ('ready', 'board_ready'),
        ('req_valid', 'valid'), ('req_write', 'wr'), ('req_ready', 'accept'),
        ('req_addr', 'addresses'), ('req_data', 'wdata'), ('rsp_valid', 'response'),
        ('rsp_ready', 'response_ready'), ('rsp_data', 'rdata')]]
    wrapper = '''// Generated by tools/sil_fpga_native.py; no checkpoint/state initializers.
`default_nettype none
module dorado_native_machine #(parameter integer SYSPER=4) (
    input wire sys_clk,clk_host,reset,baseboard_boot_n,large_format,
    input wire [7:0] ethernet_host,
    input wire [10:0] ps2_key,
    input wire [15:0] mouse_keys,
    input wire signed [8:0] mouse_dx,mouse_dy,
    input wire mouse_pending,
    output wire mouse_taken,
    output wire signed [8:0] mouse_sent_dx,mouse_sent_dy,
    input wire ethernet_rx,
    output wire ethernet_tx_n,
    input wire pack_attached,pack_readonly,pack_mounted,
    input wire sd_ack,sd_buff_wr,
    input wire [12:0] sd_buff_addr,
    input wire [15:0] sd_buff_dout,
    output wire [15:0] sd_buff_din,
    output wire [31:0] sd_lba,
    output wire [5:0] sd_blk_cnt,
    output wire sd_rd,sd_wr,pack_ready,pack_fault,pack_dirty,
    output wire [31:0] pack_fault_debug,pack_fault_extra,
    output wire [2:0] pack_host_phase,
    output wire [31:0] pack_reads,pack_writes,
    output wire [8:0] storage_events,
    input wire ddram_busy,ddram_rvalid,
    input wire [63:0] ddram_rdata,
    output wire [7:0] ddram_burstcount,ddram_be,
    output wire [28:0] ddram_addr,
    output wire ddram_read,ddram_write,
    output wire [63:0] ddram_wdata,
    output wire storage_ready,storage_initialized,
    output wire [31:0] baseboard_debug,processor_debug,
`ifdef DORADO_RX_MONITOR
    output wire [31:0] receive_debug,
`endif
    output wire [9:0] control_debug,
    output wire video,hsync,vsync_n,hblank,vblank,halfline,pixel_clk,video_field
);
  localparam integer GROUPS=GROUP_COUNT,RAW_BITS=ADDRESS_BITS;
  wire base_clock_out,term_ois;
  cell_K1115A #(.FREQ_KHZ(50000),.SYSPER(2*SYSPER)) u_pixclk
    (.sys_clk(sys_clk),.p8(pixel_clk),.p7(1'b0),.p14(1'b1));
  wire [15:0] kw0,kw1,kw2,kw3;
  dorado_keyboard u_keyboard(.sys_clk(sys_clk),.reset(reset),.ps2_key(ps2_key),
    .key_word0(kw0),.key_word1(kw1),.key_word2(kw2),.key_word3(kw3));
  reg hblank_d=0;
  always @(posedge sys_clk)hblank_d<=hblank;
  dorado_terminal u_terminal(.sys_clk(sys_clk),.reset(reset),.line_tick(hblank && !hblank_d),
    .key_word0(kw0),.key_word1(kw1),.key_word2(kw2),.key_word3(kw3),
    .key_word4(mouse_keys),.mouse_dx(mouse_dx),.mouse_dy(mouse_dy),.mouse_pending(mouse_pending),
    .mouse_taken(mouse_taken),.mouse_sent_dx(mouse_sent_dx),.mouse_sent_dy(mouse_sent_dy),
    .OISData(term_ois),.OISData_n());
  wire drv_cp,drv_cm,drv_dp,drv_dm,drv_ready_n,drv_online_n,drv_term_n,drv_index_n,drv_sec_n,drv_sel_n,disk_sel_n;
  wire [15:0] drv_sector;
  wire [11:0] tag_n;
  wire cyltag_n,headtag_n,drivetag_n,conttag_n,pack_eoc_n,pack_writing;
  wire write_data_p,write_data_m,drv_adv,media_bit;
  dorado_pack_hps #(.SECTORS_PER_TRACK(DISK_SECTORS),.LABEL_WORDS(DISK_LABEL_WORDS)) u_media(
    .clk_sys(clk_host),.machine_clk(sys_clk),.reset(reset),
    .attached(pack_attached),.readonly(pack_readonly),.remounted(pack_mounted),
    .tagbus_n(tag_n),.cyltag_n(cyltag_n),.headtag_n(headtag_n),.drivetag_n(drivetag_n),.conttag_n(conttag_n),
    .selected(~disk_sel_n),.data_adv(drv_adv),.pulse(drv_sector),
    .write_data_p(write_data_p),.write_data_m(write_data_m),
    .data_bit(media_bit),.write_gate(pack_writing),.endofcyl_n(pack_eoc_n),
    .ready(pack_ready),.fault(pack_fault),.dirty(pack_dirty),
    .fault_debug(pack_fault_debug),.fault_extra(pack_fault_extra),
    .host_phase(pack_host_phase),.reads(pack_reads),.writes(pack_writes),
    .sd_lba(sd_lba),.sd_blk_cnt(sd_blk_cnt),.sd_rd(sd_rd),.sd_wr(sd_wr),.sd_ack(sd_ack),
    .sd_buff_addr(sd_buff_addr),.sd_buff_dout(sd_buff_dout),.sd_buff_wr(sd_buff_wr),.sd_buff_din(sd_buff_din));
  dorado_trident #(.SYSPER(2*SYSPER),.SECTORS_PER_REV(DISK_PULSES),.WORDS_PER_TRACK(10080)) u_drive(
    .sys_clk(sys_clk),.reset(reset),.attached(pack_attached),.selected(~disk_sel_n),
    .data_bit(media_bit),.data_adv(drv_adv),.sector_out(drv_sector),
    .TtlReady_n(drv_ready_n),.TtlOnLine_n(drv_online_n),.TtlTerm_n(drv_term_n),
    .Selected_n(drv_sel_n),.SecIndx_n(drv_sec_n),.TtlIndex_n(drv_index_n),
    .DataP(drv_dp),.DataM(drv_dm),.ClockP(drv_cp),.ClockM(drv_cm));
  wire board_ready;
  wire [GROUPS-1:0] valid,wr,accept,response,response_ready;
  wire [GROUPS*14-1:0] addresses;
  wire [GROUPS*16-1:0] wdata,rdata;
  wire rv,rr,rw,sv,sr;
  wire [RAW_BITS-1:0] ra,init_address;
  wire [15:0] wd,sd;
  wire init_valid,init_accept,init_response,init_response_ready;
  assign storage_ready=storage_initialized && board_ready;
  dorado_storage_init #(.ADDR_BITS(RAW_BITS)) u_initialize(
    .clk(clk_host),.reset(reset),.ready(storage_initialized),
    .req_valid(init_valid),.req_ready(init_accept),.req_addr(init_address),
    .rsp_valid(init_response),.rsp_ready(init_response_ready));
  dorado_storage_arb #(.GROUPS(GROUPS),.GROUP_ADDR_BITS(14),.RAW_ADDR_BITS(RAW_BITS)) u_arb(
    .clk(clk_host),.reset(reset || !storage_initialized),
    .req_valid(valid),.req_write(wr),.req_ready(accept),.req_addr(addresses),.req_data(wdata),
    .rsp_valid(response),.rsp_ready(response_ready),.rsp_data(rdata),
    .raw_req_valid(rv),.raw_req_ready(rr),.raw_req_write(rw),.raw_req_addr(ra),.raw_req_data(wd),
    .raw_rsp_valid(sv),.raw_rsp_ready(sr),.raw_rsp_data(sd));
  wire backend_ready,backend_response;
  wire backing_cache_hit;
  // Passive host-clock events. Raw request counts exclude initialization;
  // DDR command counts include it so initialization traffic remains visible.
  assign storage_events={backing_cache_hit,
    (sv && sr),(rv && !rr),(ddram_busy && (ddram_read || ddram_write)),
    ddram_rvalid,(ddram_write && !ddram_busy),(ddram_read && !ddram_busy),
    (rv && rr && rw),(rv && rr && !rw)};
  assign rr=storage_initialized && backend_ready;
  assign sv=storage_initialized && backend_response;
  assign init_accept=!storage_initialized && backend_ready;
  assign init_response=!storage_initialized && backend_response;
  dorado_storage_ddram #(.ADDR_BITS(RAW_BITS),.LINE_CACHE_BITS(LINE_BUFFER_BITS),.GROUP_ADDR_BITS(14)) u_ddram(
    .clk(clk_host),.reset(reset),
    .req_valid(storage_initialized ? rv : init_valid),.req_ready(backend_ready),
    .req_write(storage_initialized ? rw : 1'b1),
    .req_addr(storage_initialized ? ra : init_address),.req_data(storage_initialized ? wd : 16'd0),
    .rsp_valid(backend_response),.rsp_ready(storage_initialized ? sr : init_response_ready),.rsp_data(sd),
    .ddram_busy(ddram_busy),.ddram_burstcount(ddram_burstcount),.ddram_addr(ddram_addr),
    .ddram_read(ddram_read),.ddram_write(ddram_write),.ddram_wdata(ddram_wdata),.ddram_be(ddram_be),
    .cache_hit(backing_cache_hit),
    .ddram_rdata(ddram_rdata),.ddram_rvalid(ddram_rvalid));
  dorado_native_backplane #(.SYSPER(SYSPER)) m (
CONNECTIONS
  );
RECEIVE_MONITOR
endmodule
`default_nettype wire
'''.replace('GROUP_COUNT', str(groups)).replace('ADDRESS_BITS', str(address_bits)).replace('LINE_BUFFER_BITS', str(args.storage_line_cache_bits))
    if chip_address_bits != 14:
        wrapper = wrapper.replace('[GROUPS*14-1:0]', f'[GROUPS*{chip_address_bits}-1:0]')
        wrapper = wrapper.replace('.GROUP_ADDR_BITS(14)', f'.GROUP_ADDR_BITS({chip_address_bits})')
    wrapper = wrapper.replace('DISK_SECTORS', '29')
    wrapper = wrapper.replace('DISK_LABEL_WORDS', '10' if args.pilot_disk else '8')
    wrapper = wrapper.replace('DISK_PULSES', '117')
    if args.receive_monitor:
        connections.append('    .receive_debug(receive_debug)')
    wrapper = wrapper.replace('CONNECTIONS', ',\n'.join(connections))
    wrapper = wrapper.replace('RECEIVE_MONITOR', '')
    if args.instruction_monitor:
        wrapper = wrapper.replace('    output wire [31:0] baseboard_debug,processor_debug,',
            '    output wire [4:0] instruction_signals,\n'
            '    output wire [31:0] baseboard_debug,processor_debug,')
    if args.storage_backend == 'sdram':
        wrapper = sdram_wrapper(wrapper)
    (out/'dorado_native_machine.sv').write_text(wrapper)
    sources = [str(out/n) for n in ['dorado_native_backplane.v', 'BaseBd_nominal.v',
                                   'msa_ddram.v', 'ContA_monitor.v', 'DispY_field.v',
                                   'dorado_native_machine.sv']]
    if args.receive_monitor:
        sources.append(str(out/'DskEth_monitor.v'))
    if args.map_kwords != 16:
        sources.append(str(out/'MemX_density.v'))
    for name in boards:
        if args.receive_monitor and name == 'DskEth':
            continue
        if args.map_kwords != 16 and name == 'MemX':
            continue
        if name.startswith('msa') or name in ('BaseBd','ContA','DispY'):
            continue
        candidates = list(Path('verilog/generated').glob(name+'-Rev-*.v'))
        if len(candidates)!=1:
            raise ValueError(f'ambiguous board source: {name}')
        sources.append(str(candidates[0]))
    sources += [str(p) for p in sorted(Path('verilog/cells').glob('cell_*.v'))]
    sources += [str(Path('verilog/cells')/n) for n in ['dorado_keyboard.v', 'dorado_terminal.v', 'dorado_trident.v']]
    sources += [str(p) for p in sorted(Path('verilog/vendor/6502').glob('*.v'))]
    sources += [str(p) for p in sorted(Path('verilog/vendor/riot').glob('*.sv'))]
    sources += ['fpga/mister/'+n+'.sv' for n in ['dorado_receive_monitor','dorado_pack_hps','dorado_storage_group',
                'dorado_storage_arb','dorado_storage_ddram','dorado_storage_init']]
    if args.storage_backend == 'sdram':
        sources.append('fpga/mister/dorado_storage_sdram.sv')
    manifest = {'native': True, 'storage_modules': args.storage_modules, 'pilot_disk': args.pilot_disk, 'receive_monitor': args.receive_monitor,
        'storage_line_cache_bits': args.storage_line_cache_bits,
                                               'control_error_monitor': True, 'pack_fault_monitor': True,
        'display_field': True, 'control_clock_monitor': True,
        'hps_pack': True, 'ethernet_wire': True, 'ethernet_host_port': True}
    manifest['storage_backend'] = args.storage_backend
    if args.instruction_monitor:
        manifest['instruction_monitor'] = True
    if args.storage_backend == 'sdram':
        manifest['required_sdram_bytes'] = 128*1024*1024
    if args.storage_chip_kbits != 16 or args.map_kwords != 16:
        manifest.update(storage_chip_kbits=args.storage_chip_kbits, map_kwords=args.map_kwords,
                        memory_geometry_experimental=True)
    (out/'manifest.json').write_text(json.dumps(manifest, indent=2)+'\n')
    (out/'sources.f').write_text('\n'.join(sources)+'\n')
    print(f'Generated cold-start machine: {args.storage_modules} storage modules, {groups} raw groups')


if __name__=='__main__':
    main()
