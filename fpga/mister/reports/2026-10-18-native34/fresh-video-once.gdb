set pagination off
set confirm off
tbreak input_poll
continue
call ((void (*)(int))0xa8881)(1)
printf "FRESH_WIDTH=%u\n", *(unsigned int*)0x9715ec
printf "FRESH_HEIGHT=%u\n", *(unsigned int*)(0x9715ec+4)
printf "FRESH_SOURCE_VTIME=%u\n", *(unsigned int*)(0x9715ec+12)
printf "FRESH_HDMI_VTIME=%u\n", *(unsigned int*)(0x9715ec+24)
detach
