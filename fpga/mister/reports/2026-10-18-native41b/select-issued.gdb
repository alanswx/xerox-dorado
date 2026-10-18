set pagination off
set confirm off
python
s=gdb.execute("info symbol 0x9aa74",to_string=True)
print(s)
assert "user_io_status_set" in s
end
tbreak input_poll
continue
call ((void (*)(const char *, unsigned int, int))0x9aa75)("[8:5]",0,0)
call ((void (*)(const char *, unsigned int, int))0x9aa75)("[10:9]",1,0)
detach
