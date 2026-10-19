set pagination off
set confirm off
python
assert gdb.selected_inferior().pid==14717
s=gdb.execute("info symbol 0x9e98c",to_string=True)
print(s)
assert "user_io_status_set" in s
end
tbreak input_poll
continue
call ((void (*)(const char *, unsigned int, int))0x9e98d)("[8:5]",0,0)
call ((void (*)(const char *, unsigned int, int))0x9e98d)("[10:9]",1,0)
detach
