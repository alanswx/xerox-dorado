set pagination off
set confirm off
python
import gdb
try:
 f=gdb.newest_frame()
 while f is not None and f.name()!='main': f=f.older()
 assert f is not None
 f.select()
 eth=gdb.parse_and_eval('eth')
 print('LIVE_SERVER_CONTEXT')
 for name in ['last_boot_offset','ftp_ctx_cur','ftp_ctx_valid','ftp_open','ftp_tx_next','ftp_tx_last_end','ftp_last_ack','ftp_tx_mode','ftp_tx_step','ftp_file_pos','ftp_file_size','ftp_client_net_host','ftp_client_sock_hi','ftp_client_sock_lo']:
  print(name, hex(int(eth[name])))
 slot=int(eth['ftp_ctx_cur'])
 c=eth['ftp_ctx'][slot]
 print('SAVED_SLOT',slot)
 for name in ['used','open','tx_next','tx_last_end','last_ack','tx_mode','tx_step','file_pos','file_size','client_net_host','client_sock_hi','client_sock_lo']:
  print(name,hex(int(c[name])))
finally:
 gdb.execute('detach')
end
