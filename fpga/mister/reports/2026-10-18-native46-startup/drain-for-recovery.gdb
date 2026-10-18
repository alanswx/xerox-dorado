set pagination off
set confirm off
set language c++
python
assert 'dorado_disk_barrier' in gdb.execute('info symbol 0x67b50',to_string=True)
end
tbreak input_poll
continue
print ((bool(*)(const char*))0x67b51)("full-board recovery after DDR stopped indication failure")
detach
