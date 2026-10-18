set pagination off
set confirm off
tbreak input_poll
continue
call ((void (*)(int,int))0xa9925)(1,0)
detach
