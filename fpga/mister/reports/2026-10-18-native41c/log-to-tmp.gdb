set pagination off
set confirm off
python
assert 'input_poll' in gdb.execute('info symbol 0x84310',to_string=True)
end
tbreak input_poll
continue
call (int)fflush((void *)0)
set $dorado_log_fd = (int)open("/tmp/dorado41c-main.log", 1217, 384)
python
assert int(gdb.parse_and_eval('$dorado_log_fd')) >= 3
end
call (int)dup2($dorado_log_fd,1)
call (int)dup2($dorado_log_fd,2)
call (int)close($dorado_log_fd)
detach
