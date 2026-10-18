set pagination off
set confirm off
python
import gdb
try:
 f=gdb.newest_frame()
 while f is not None and f.name()!='main': f=f.older()
 assert f is not None
 f.select()
 e=gdb.parse_and_eval('eth')
 expected={'last_boot_offset':0o113,'ftp_ctx_cur':4,'ftp_ctx_valid':1,'ftp_open':1,'ftp_tx_next':0x14434b,'ftp_tx_last_end':0x14434b,'ftp_last_ack':0x144f94,'ftp_tx_mode':2,'ftp_tx_step':0,'ftp_file_pos':0,'ftp_file_size':2204,'ftp_client_net_host':0x122,'ftp_client_sock_hi':0x12,'ftp_client_sock_lo':0x801c}
 for name,want in expected.items():
  got=int(e[name]);print('ASSERT',name,hex(got),hex(want));assert got==want,(name,got,want)
 c=e['ftp_ctx'][4]
 for name,want in {'used':1,'open':1,'tx_next':0x14434b,'tx_last_end':0x14434b,'last_ack':0x144f94,'tx_mode':2,'tx_step':0,'file_pos':0,'file_size':2204,'client_net_host':0x122,'client_sock_hi':0x12,'client_sock_lo':0x801c}.items():
  assert int(c[name])==want,(name,int(c[name]),want)
 # The wire trace proves that the receiver ACKed all bytes through 144f94.
 # Restore only the old ARM producer cursor, which a replay moved backwards.
 for target in ['eth.ftp_tx_next','eth.ftp_tx_last_end','eth.ftp_ctx[4].tx_next','eth.ftp_ctx[4].tx_last_end']:
  gdb.execute('set var '+target+' = 0x144f94')
  print('REPAIRED',target,hex(int(gdb.parse_and_eval(target))))
 print('ASSISTED_BOOT: ARM BSP cursor repair only; no guest CPU/memory/IOCB changes')
finally:
 gdb.execute('detach')
end
