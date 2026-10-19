set pagination off
set confirm off
set language c++
python
import hashlib
from pathlib import Path
assert gdb.selected_inferior().pid==1509
assert hashlib.sha256(Path('/proc/1509/exe').read_bytes()).hexdigest()=='21e67a3f5588a875320437133df316b7e8f87061eb06b6dcaf63ecb139e29b60'
assert 'dorado_net_barrier' in gdb.execute('info symbol 0x69990',to_string=True)
assert 'spi_uio_cmd_cont' in gdb.execute('info symbol 0x9ca90',to_string=True)
assert 'fpga_spi' in gdb.execute('info symbol 0x701a4',to_string=True)
assert 'DisableIO' in gdb.execute('info symbol 0x9c8b4',to_string=True)
end
tbreak input_poll
continue
python
import json,time
from pathlib import Path
assert not Path('/tmp/dorado47-active-stop.json').exists()
def capability():
 words=[]
 try:
  words.append(int(gdb.parse_and_eval('((unsigned short(*)(unsigned short))0x9ca91)(0x74)')))
  for i in range(17):words.append(int(gdb.parse_and_eval('((unsigned short(*)(unsigned short))0x701a5)(0)')))
 finally:gdb.execute('call ((void(*)())0x9c8b5)()')
 assert words[:6]==[0xd0d4,1,0,0x1ff0,0,1],words
 return words
before=capability()
assert before[6]==0x16 and not any(before[i]for i in (7,8,16,17)),before
start=time.monotonic_ns()
result=int(gdb.parse_and_eval('((bool(*)(const char*))0x69991)("active-DDR lifecycle qualification after normal guest Boot")'))
after=capability();finished=time.monotonic_ns()
record=dict(main_pid=1509,server_pid=1506,before=before,barrier_result=bool(result),after=after,start_ns=start,finished_ns=finished)
Path('/tmp/dorado47-active-stop.json').write_text(json.dumps(record,indent=2)+'\n')
assert result and after[6]==1 and before[9:13]==after[9:13] and not any(after[i]for i in (7,8,16,17)),record
print('ACTUAL_ACTIVE_STOP '+json.dumps(record))
end
detach
