set pagination off
set confirm off
tbreak poll
continue
set $packet=(void *)((void *(*)(unsigned))malloc)(26)
set $dest=(void *)((void *(*)(unsigned))malloc)(110)
python
import gdb
gdb.selected_inferior().write_memory(int(gdb.parse_and_eval("$packet")),bytes.fromhex("22010200001600110019b035012200129426010100000003ffff"))
gdb.selected_inferior().write_memory(int(gdb.parse_and_eval("$dest")),bytes.fromhex("01002f746d702f646f7261646f2d6d61696e2d363734382e736f636b00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"))
end
call ((int (*)(int,void *,unsigned,int,void *,unsigned))sendto)(3,$packet,26,0,$dest,110)
call ((void (*)(void *))free)($packet)
call ((void (*)(void *))free)($dest)
detach
