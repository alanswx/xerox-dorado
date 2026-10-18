set pagination off
set confirm off
set language c++
python
assert 'dorado_disk_barrier' in gdb.execute('info symbol 0x66a0c',to_string=True)
end
tbreak input_poll
continue
set $barrier_ok = ((bool(*)(const char*))0x66a0d)("Native41d cold-remount readback")
python
assert bool(gdb.parse_and_eval('$barrier_ok'))
end
detach
