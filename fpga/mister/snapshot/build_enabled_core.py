#!/usr/bin/env python3
"""Complete Native59-derived board-state capacity experiment, never deploys an FPGA.

Every qualified board's administrative paths are wired to bounded diagnostic
SPI access. This is board-only scope, not wrapper/backend/input/ARM snapshots.
Original clocks, memory/ECC and timing constraints remain the qualified ones.
"""
import argparse,hashlib,json,re,shutil,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
BP='build/rtl-executive/native16-tx-cable/dorado_native_backplane.v'
MACHINE='build/rtl-executive/native16-tx-cable/dorado_native_machine.sv'
PARENT='/home/alans/mister/Dorado_Native59_20261019'
BOARDS=[('ContA',12),('ContB',15),('ProcH',16),('ProcL',16),('IFU',18),('MemC',16),('MemD',20),('MemX',24),('DskEth',13),('DispY',19),('BaseBd',17)]
TARGET=Path('fpga/mister/snapshot_enabled')
PORTS='''    input wire snapshot_freeze,snapshot_guest_held,snapshot_request,snapshot_write,
    input wire [3:0] snapshot_board,input wire [23:0] snapshot_index,input wire [17:0] snapshot_data,
    output wire snapshot_ready,snapshot_done,output wire [2:0] snapshot_error,
    output wire [17:0] snapshot_result,output wire [10:0] snapshot_frozen,output wire snapshot_fault,
'''
CONNECTIONS='''    .snapshot_freeze(snapshot_freeze),.snapshot_guest_held(snapshot_guest_held),
    .snapshot_request(snapshot_request),.snapshot_write(snapshot_write),.snapshot_board(snapshot_board),
    .snapshot_index(snapshot_index),.snapshot_data(snapshot_data),.snapshot_ready(snapshot_ready),
    .snapshot_done(snapshot_done),.snapshot_error(snapshot_error),.snapshot_result(snapshot_result),
    .snapshot_frozen(snapshot_frozen),.snapshot_fault(snapshot_fault),
'''


def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def once(t,a,b):
 if t.count(a)!=1:raise ValueError('unexpected source shape:'+a[:90])
 return t.replace(a,b,1)


def prepare(frozen,out,shared_riot=False):
 if shared_riot:import generate_baseboard_shared_riot as gen
 else:import generate_baseboard_full as gen
 board,m,clones=gen.generate(frozen);out.mkdir(parents=True,exist_ok=False)
 files={'BaseBd_full_snapshot.sv':board,**clones}
 for n in ('board_state_fabric.sv','board_state_uio.sv'):files[n]=(Path(__file__).parent/n).read_bytes()
 shared={}
 for n in clones:
  choices=[p for p in (frozen/'fpga/mister').glob('snapshot_native*/'+n)]
  if choices:
   if len(choices)!=1 or digest(choices[0])!=m['cloned_sources'][n]:raise ValueError('shared helper differs/ambiguous:'+n)
   shared[n]=str(choices[0].relative_to(frozen))
 for n,s in files.items():(out/n).write_bytes(s if isinstance(s,bytes)else s.encode())
 catalog=[]
 for i,(name,bits)in enumerate(BOARDS):
  if name=='BaseBd':manifest=m
  else:
   matches=[]
   for p in (frozen/'fpga/mister').glob('snapshot_native*/assets.json'):
    a=json.loads(p.read_text());value=a.get('manifests',{}).get(name)
    if name=='ContB':value=a.get('board_manifest')if p.parent.name=='snapshot_native'else value
    if name=='ContA':value=a.get('manifest')if p.parent.name=='snapshot_native_conta'else value
    if value:matches.append(value)
   if len(matches)!=1:raise ValueError('qualified board schema missing/ambiguous:'+name)
   manifest=matches[0]
  catalog.append(dict(id=i,board=name,index_bits=bits,module=manifest['module'],logical_state_bits=manifest['logical_state_bits']))
 record=dict(scope=__doc__,baseboard_manifest=m,catalog=catalog,assets={n:digest(out/n)for n in files},shared=shared,
  implementation_variant='shared_riot_ram_port'if shared_riot else'qualified_original_ports',
  expected_original_sources=m['original_sources']|{gen.SOURCE:m['original_sha256']},builder_sha256=digest(Path(__file__)),
  helper_scope='Board state only; all11adapters active, Main/wrapper/backends/input/server/disk generation not captured; no machine snapshot or deployment qualification.',
  nominal_clock='Same50MHz/SYSPER4, unchanged SDRAM/DDR/ECC and timing constraints')
 (out/'assets.json').write_text(json.dumps(record,indent=2)+'\n')
 print('Prepared all11boards,',sum(x['logical_state_bits']for x in catalog),'modeled board bits;',len(shared),'shared helpers reused')


def rewire_backplane(text,metadata):
 text=once(text,'    input wire contb_advance,','    input wire contb_advance,\n'+PORTS)
 for item in metadata['catalog']:
  name=item['board'];i=item['id'];width=item['index_bits'];module=item['module']
  if name=='BaseBd':
   anchor=re.search(r'\bBaseBd_m_Rev_m_Am_nominal\s*#\(\.SYSPER\(SYSPER\)\)\s+b_BaseBd\s*\(',text);assert anchor
   injection=module+''' #(.SYSPER(SYSPER)) b_BaseBd (
    .state_host_clk(storage_clk),.state_guest_advance(contb_advance),
    .state_freeze(1'b0),.state_guest_held(1'b0),.state_request(1'b0),.state_write(1'b0),
    .state_index(17'b0),.state_data(18'b0),.state_frozen(),.state_done(),.state_error(),.state_result(),'''
   text=text[:anchor.start()]+injection+text[anchor.end():]
  # Match the exact current instance, not a wildcard parameter spanning peers.
  marker=re.search(r'\b'+module+r'\b[^;]*?\bb_'+name+r'\s*\((.*?)\);',text,re.S);assert marker,'missing board instance:'+name
  old=marker[0];new=old
  values={'state_freeze':'snapshot_freeze','state_guest_held':'snapshot_guest_held','state_request':f'snapshot_board_request[{i}]',
   'state_write':'snapshot_write','state_index':f'snapshot_index[{width-1}:0]','state_data':'snapshot_data','state_frozen':f'snapshot_frozen[{i}]',
   'state_done':f'snapshot_board_done[{i}]','state_error':f'snapshot_board_error[{i}]','state_result':f'snapshot_board_result[{i*18}+:18]'}
  for port,value in values.items():
   pattern=r'\.'+port+r'\(([^)]*)\)';found=re.findall(pattern,new);assert len(found)==1,(name,port)
   if port=='state_index':assert found[0]==f"{width}'b0",'qualified index width differs'
   elif port in ('state_freeze','state_guest_held','state_request','state_write'):assert found[0]=="1'b0",'administrative input was active'
   elif port=='state_data':assert found[0]=="18'b0"
   else:assert found[0]==''
   new=re.sub(pattern,'.'+port+'('+value+')',new,count=1)
  assert '.state_host_clk(storage_clk)'in new and '.state_guest_advance(contb_advance)'in new
  text=text[:marker.start()]+new+text[marker.end():]
 end=text.index(');',text.index('module dorado_native_backplane'))+2
 fabric='''
  wire [10:0] snapshot_board_request,snapshot_board_done,snapshot_board_error;
  wire [197:0] snapshot_board_result;
  dorado_board_state_fabric u_snapshot_fabric(
   .clk(storage_clk),.reset(storage_reset),.freeze(snapshot_freeze),.guest_held(snapshot_guest_held),
   .request(snapshot_request),.write(snapshot_write),.board(snapshot_board),.index(snapshot_index),.data(snapshot_data),
   .ready(snapshot_ready),.done(snapshot_done),.error(snapshot_error),.result(snapshot_result),.fault(snapshot_fault),
   .board_frozen(snapshot_frozen),.board_done(snapshot_board_done),.board_error(snapshot_board_error),
   .board_result(snapshot_board_result),.board_request(snapshot_board_request));
'''
 return text[:end]+fabric+text[end:]


def rewire_wrapper(text):
 text=once(text,'    input wire contb_advance,','    input wire contb_advance,\n'+PORTS)
 anchor='  dorado_native_backplane #(.SYSPER(SYSPER)) m (\n';return once(text,anchor,anchor+CONNECTIONS)


def rewire_core(text):
 text=once(text,'wire [35:0] ethernet_ext_bus;','''wire [35:0] ethernet_ext_bus,board_state_ext_bus,dorado_ext_bus;
wire board_state_freeze,board_state_guest_held,board_state_request,board_state_write;
wire [3:0] board_state_board;wire [23:0] board_state_index;wire [17:0] board_state_data,board_state_result;
wire board_state_ready,board_state_done,board_state_fault;wire [2:0] board_state_error;wire [10:0] board_state_frozen;
assign ethernet_ext_bus[35:33]=dorado_ext_bus[35:33];assign ethernet_ext_bus[31:16]=dorado_ext_bus[31:16];
assign board_state_ext_bus[35:33]=dorado_ext_bus[35:33];assign board_state_ext_bus[31:16]=dorado_ext_bus[31:16];
assign dorado_ext_bus[32]=ethernet_ext_bus[32]|board_state_ext_bus[32];
assign dorado_ext_bus[15:0]=ethernet_ext_bus[32]?ethernet_ext_bus[15:0]:board_state_ext_bus[32]?board_state_ext_bus[15:0]:16'b0;''')
 text=once(text,'.EXT_BUS(ethernet_ext_bus)','.EXT_BUS(dorado_ext_bus)')
 text=once(text,'wire machine_enable = ~reset && pack_ready && storage_ready;','wire machine_enable = ~reset && pack_ready && storage_ready && !board_state_freeze;')
 anchor='dorado_native_machine #(.SYSPER(MACHINE_SYSPER)) u_native (\n'
 frontend='''// Board-only diagnostic boundary. Backend/input/ARM state is not snapshotted.
assign board_state_guest_held=board_state_freeze && !network_step;
dorado_board_state_uio u_board_state_uio(
 .clk(clk_sys),.reset(reset),.bus(board_state_ext_bus),.freeze(board_state_freeze),.guest_held(board_state_guest_held),
 .board_frozen(board_state_frozen),.request(board_state_request),.write(board_state_write),
 .board(board_state_board),.index(board_state_index),.data(board_state_data),.ready(board_state_ready),
 .done(board_state_done),.error(board_state_error),.result(board_state_result),.fault(board_state_fault),
 .busy(),.complete(),.txn_sequence(),.control_errors());
'''
 con=CONNECTIONS
 for name in('freeze','guest_held','request','write','board','index','data','ready','done','error','result','frozen','fault'):con=con.replace('('+f'snapshot_{name}'+')','('+f'board_state_{name}'+')')
 return once(text,anchor,frontend+anchor+con)


def stage(baseline,out,assets):
 a=json.loads((assets/'assets.json').read_text());assert digest(Path(__file__))==a['builder_sha256'],'prepared builder differs'
 for n,h in a['assets'].items():assert digest(assets/n)==h,'prepared asset differs'
 for n,h in a['expected_original_sources'].items():assert digest(baseline/'src'/n)==h,'original source differs'
 for n,path in a['shared'].items():assert digest(baseline/'src'/path)==a['assets'][n],'shared helper differs'
 assert (baseline/'core/Dorado.qsf').read_text().count('set_global_assignment -name VERILOG_MACRO "DORADO_STORAGE_INPUT_DELAY=2"')==1
 out.mkdir(parents=True,exist_ok=False);shutil.copytree(baseline/'src',out/'src');shutil.copytree(baseline/'core',out/'core',ignore=shutil.ignore_patterns('db','incremental_db','output_files','*.rpt','*.summary','*.log','c5_pin_model_dump.txt'))
 for p in(out/'core').rglob('*'):
  if p.is_file()and p.suffix in('.qsf','.qip','.sdc','.tcl','.qpf'):
   text=p.read_text()
   for parent in{str(baseline),PARENT}:text=text.replace(parent,str(out))
   p.write_text(text)
 bp=out/'src'/BP;bp.write_text(rewire_backplane(bp.read_text(),a));machine=out/'src'/MACHINE;machine.write_text(rewire_wrapper(machine.read_text()));core=out/'core/Dorado.sv';core.write_text(rewire_core(core.read_text()))
 target=out/'src'/TARGET;target.mkdir(exist_ok=False);new=[n for n in a['assets']if n not in a['shared']]
 for n in new:shutil.copyfile(assets/n,target/n)
 shutil.copyfile(assets/'assets.json',target/'assets.json')
 qsf=out/'core/Dorado.qsf';text=qsf.read_text()
 for n in new:text+='\nset_global_assignment -name SYSTEMVERILOG_FILE '+str(target/n)+'\n'
 qsf.write_text(text)
 for name in('Dorado.sdc','native_timing.sdc','sdram_timing.sdc','checkpoint_timing.sdc'):
  old=(baseline/'core'/name).read_text();actual=(out/'core'/name).read_text()
  for parent in{str(baseline),PARENT}:old=old.replace(parent,str(out))
  assert actual==old,'timing constraints changed:'+name
 changed=[];added=[]
 for p in(out/'src').rglob('*'):
  if not p.is_file():continue
  n=p.relative_to(out/'src');old=baseline/'src'/n
  if not old.exists():added.append(str(n))
  elif p.read_bytes()!=old.read_bytes():changed.append(str(n))
 assert set(changed)=={BP,MACHINE},'unintended source edits'
 assert set(added)=={str(TARGET/n)for n in new+['assets.json']},'unintended new sources'
 from_pattern='native59-';corners=out/'core/device-corners.tcl';corners.write_text(corners.read_text().replace(from_pattern,'enabled60-'))
 # New script reports every board boundary with unchanged SDC exceptions.
 crossing=(baseline/'core/audit-adapter-crossings.tcl').read_text().replace('native59-','enabled60-')
 crossing=once(crossing,'  {*|DskEth_full_snapshot_experiment:b_DskEth|*} dsketh','  {*|DskEth_full_snapshot_experiment:b_DskEth|*} dsketh\n  {*|BaseBd_full_snapshot_experiment:b_BaseBd|*} basebd')
 (out/'core/audit-adapter-crossings.tcl').write_text(crossing)
 tracked=[bp,machine,core,qsf,out/'core/device-corners.tcl',out/'core/audit-adapter-crossings.tcl',*[out/'src'/n for n in added]]
 record=dict(scope=a['helper_scope'],baseline=str(baseline),output=str(out),catalog=a['catalog'],board_state_bits=sum(x['logical_state_bits']for x in a['catalog']),
  implementation_variant=a.get('implementation_variant','qualified_original_ports'),
  source_changes=changed,core_changes=['Dorado.sv','Dorado.qsf','device-corners.tcl','audit-adapter-crossings.tcl'],added_sources=added,
  timing_changes='None; identical clocks/SDC/operating conditions',snapshot_admin='ENABLED through bounded76..79UIO diagnostics; not a whole-machine snapshot',
  source_hashes={str(p.relative_to(out)):digest(p)for p in tracked})
 (out/'enabled-integration.json').write_text(json.dumps(record,indent=2)+'\n');print('Enabled all11board state paths; staging terminal, not qualified',flush=True)


def run(out,quartus):
 r=json.loads((out/'enabled-integration.json').read_text())
 for n,h in r['source_hashes'].items():assert digest(out/n)==h,'staged source differs'
 assert not(out/'map.log').exists(),'inspect existing build;do not restart'
 for tool,args,label in [('quartus_map',['Dorado'],'map'),('quartus_fit',['Dorado'],'fit'),('quartus_sta',['Dorado'],'sta'),('quartus_sta',['-t','audit_native_clocks.tcl'],'audit'),('quartus_sta',['-t','device-corners.tcl'],'device-corners'),('quartus_sta',['-t','audit-adapter-crossings.tcl'],'adapter-crossings')]:
  print('Enabled60',label,flush=True)
  with(out/(label+'.log')).open('w')as f:subprocess.run([str(quartus/tool),*args],cwd=out/'core',stdout=f,stderr=subprocess.STDOUT,check=True)
 print('All-board enabled capacity/timing build terminal; independent audit still required. No RBF deployed.',flush=True)


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--prepare-assets',type=Path);p.add_argument('--frozen-root',type=Path);p.add_argument('--shared-riot',action='store_true');p.add_argument('--stage',type=Path);p.add_argument('--baseline',type=Path);p.add_argument('--assets',type=Path);p.add_argument('--run',type=Path);p.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'));a=p.parse_args()
 if a.prepare_assets and a.frozen_root:prepare(a.frozen_root.resolve(),a.prepare_assets.resolve(),a.shared_riot)
 elif a.stage and a.baseline and a.assets:stage(a.baseline.resolve(),a.stage.resolve(),a.assets.resolve())
 elif a.run:run(a.run.resolve(),a.quartus)
 else:p.error('Specify preparation, staging or run inputs')
