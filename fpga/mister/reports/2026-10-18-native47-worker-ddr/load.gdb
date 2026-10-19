set pagination off
set confirm off
set language c++
python
assert 'offload_try_add_work' in gdb.execute('info symbol 0x992c8',to_string=True)
end
tbreak input_poll
continue
set $probe_handle = (void *)dlopen("/tmp/dorado47-loader-probe.so",2)
python
assert int(gdb.parse_and_eval('$probe_handle')) != 0
end
set $probe_entry = (void *)dlsym($probe_handle,"dorado_queue_slow_fsync")
python
assert int(gdb.parse_and_eval('$probe_entry')) != 0
end
set $probe_rc = ((int (*)(unsigned int,int,const char *,const char *,unsigned int,unsigned int))$probe_entry)(0x992c9,20,"/tmp/dorado47-loader-worker.arm","/tmp/dorado47-loader-worker.jsonl",4200,30000)
python
assert int(gdb.parse_and_eval('$probe_rc')) == 0
end
detach
