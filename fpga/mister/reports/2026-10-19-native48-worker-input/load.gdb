set pagination off
set confirm off
set language c++
python
from pathlib import Path
import hashlib
assert gdb.selected_inferior().pid==3781
assert hashlib.sha256(Path('/proc/3781/exe').read_bytes()).hexdigest()=='21e67a3f5588a875320437133df316b7e8f87061eb06b6dcaf63ecb139e29b60'
assert 'offload_try_add_work' in gdb.execute('info symbol 0x992c8',to_string=True)
end
tbreak input_poll
continue
set $probe_handle = (void *)dlopen("/tmp/dorado48-input-probe.so",2)
python
assert int(gdb.parse_and_eval('$probe_handle')) != 0
end
set $probe_entry = (void *)dlsym($probe_handle,"dorado_queue_slow_fsync")
python
assert int(gdb.parse_and_eval('$probe_entry')) != 0
end
set $probe_rc = ((int (*)(unsigned int,int,const char *,const char *,unsigned int,unsigned int))$probe_entry)(0x992c9,20,"/tmp/dorado48-input-worker.arm","/tmp/dorado48-input-worker.jsonl",4200,30000)
python
assert int(gdb.parse_and_eval('$probe_rc')) == 0
end
detach
