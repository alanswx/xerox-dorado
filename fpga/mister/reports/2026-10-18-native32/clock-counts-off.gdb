set pagination off
set confirm off
info address input_poll
info symbol 0x9aa74
tbreak input_poll
continue
call ((void (*)(const char *, unsigned int, int))0x9aa75)("[5]",0,0)
detach
