set pagination off
set confirm off
set language c++
python
assert 'video_fb_enable' in gdb.execute('info symbol 0xad7fc',to_string=True)
assert 'user_io_status_set' in gdb.execute('info symbol 0x9e98c',to_string=True)
end
tbreak input_poll
continue
call ((void(*)(int,int))0xad7fd)(0,0)
call ((void(*)(const char*,unsigned int,int))0x9e98d)("[8:5]",0,0)
call ((void(*)(const char*,unsigned int,int))0x9e98d)("[10:9]",1,0)
python
import json
words=[]
try:
 words.append(int(gdb.parse_and_eval('((unsigned short(*)(unsigned short))0x9ca91)(0x74)')))
 for i in range(17):words.append(int(gdb.parse_and_eval('((unsigned short(*)(unsigned short))0x701a5)(0)')))
finally:gdb.execute('call ((void(*)())0x9c8b5)()')
print('ACTUAL_MAIN_CAPABILITY '+json.dumps(words))
end
detach
