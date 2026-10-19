set pagination off
set confirm off
set language c++
python
from pathlib import Path
import hashlib
assert gdb.selected_inferior().pid==8321
assert hashlib.sha256(Path('/proc/8321/exe').read_bytes()).hexdigest()=='21e67a3f5588a875320437133df316b7e8f87061eb06b6dcaf63ecb139e29b60'
assert 'video_fb_enable' in gdb.execute('info symbol 0xad7fc',to_string=True)
assert 'user_io_status_set' in gdb.execute('info symbol 0x9e98c',to_string=True)
end
tbreak input_poll
continue
call ((void(*)(int,int))0xad7fd)(1,0)
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
assert words[:6]==[0xd0d4,1,0,0x1ff0,0,1] and words[6]==22 and not any(words[i]for i in (7,8,16,17)),words
assert words[9:13]!=[39413,40156,57640,50393],words
print('ACTUAL_MAIN_CAPABILITY '+json.dumps(words))
end
detach
