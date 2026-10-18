set pagination off
set confirm off
set language c++
python
assert 'video_fb_enable' in gdb.execute('info symbol 0xac61c',to_string=True)
assert 'user_io_status_set' in gdb.execute('info symbol 0x9d71c',to_string=True)
end
tbreak input_poll
continue
call ((void(*)(int,int))0xac61d)(0,0)
call ((void(*)(const char*,unsigned int,int))0x9d71d)("[8:5]",0,0)
call ((void(*)(const char*,unsigned int,int))0x9d71d)("[10:9]",1,0)
detach
