set pagination off
set confirm off
set language c++
python
assert 'spi_uio_cmd_cont' in gdb.execute('info symbol 0x9ca90',to_string=True)
assert 'fpga_spi' in gdb.execute('info symbol 0x701a4',to_string=True)
assert 'DisableIO' in gdb.execute('info symbol 0x9c8b4',to_string=True)
end
tbreak input_poll
continue
x/bx 0x15539c
x/bx 0x1553a0
x/wx 0x1553c8
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
