set pagination off
tbreak input_poll
continue
call ((unsigned short (*)(unsigned short))0x98b79)(0x23)
set $video_word = ((unsigned short (*)(unsigned short))0x6c34d)(0)
printf "VIDEO_WORD_00=%u\n", $video_word
set $video_word = ((unsigned short (*)(unsigned short))0x6c34d)(0)
printf "VIDEO_WORD_01=%u\n", $video_word
set $video_word = ((unsigned short (*)(unsigned short))0x6c34d)(0)
printf "VIDEO_WORD_02=%u\n", $video_word
set $video_word = ((unsigned short (*)(unsigned short))0x6c34d)(0)
printf "VIDEO_WORD_03=%u\n", $video_word
set $video_word = ((unsigned short (*)(unsigned short))0x6c34d)(0)
printf "VIDEO_WORD_04=%u\n", $video_word
set $video_word = ((unsigned short (*)(unsigned short))0x6c34d)(0)
printf "VIDEO_WORD_05=%u\n", $video_word
set $video_word = ((unsigned short (*)(unsigned short))0x6c34d)(0)
printf "VIDEO_WORD_06=%u\n", $video_word
set $video_word = ((unsigned short (*)(unsigned short))0x6c34d)(0)
printf "VIDEO_WORD_07=%u\n", $video_word
set $video_word = ((unsigned short (*)(unsigned short))0x6c34d)(0)
printf "VIDEO_WORD_08=%u\n", $video_word
set $video_word = ((unsigned short (*)(unsigned short))0x6c34d)(0)
printf "VIDEO_WORD_09=%u\n", $video_word
set $video_word = ((unsigned short (*)(unsigned short))0x6c34d)(0)
printf "VIDEO_WORD_10=%u\n", $video_word
set $video_word = ((unsigned short (*)(unsigned short))0x6c34d)(0)
printf "VIDEO_WORD_11=%u\n", $video_word
set $video_word = ((unsigned short (*)(unsigned short))0x6c34d)(0)
printf "VIDEO_WORD_12=%u\n", $video_word
set $video_word = ((unsigned short (*)(unsigned short))0x6c34d)(0)
printf "VIDEO_WORD_13=%u\n", $video_word
set $video_word = ((unsigned short (*)(unsigned short))0x6c34d)(0)
printf "VIDEO_WORD_14=%u\n", $video_word
set $video_word = ((unsigned short (*)(unsigned short))0x6c34d)(0)
printf "VIDEO_WORD_15=%u\n", $video_word
set $video_word = ((unsigned short (*)(unsigned short))0x6c34d)(0)
printf "VIDEO_WORD_16=%u\n", $video_word
set $video_word = ((unsigned short (*)(unsigned short))0x6c34d)(0)
printf "VIDEO_WORD_17=%u\n", $video_word
set $video_word = ((unsigned short (*)(unsigned short))0x6c34d)(0)
printf "VIDEO_WORD_18=%u\n", $video_word
set $video_word = ((unsigned short (*)(unsigned short))0x6c34d)(0)
printf "VIDEO_WORD_19=%u\n", $video_word
set $video_word = ((unsigned short (*)(unsigned short))0x6c34d)(0)
printf "VIDEO_WORD_20=%u\n", $video_word
set $video_word = ((unsigned short (*)(unsigned short))0x6c34d)(0)
printf "VIDEO_WORD_21=%u\n", $video_word
set $video_word = ((unsigned short (*)(unsigned short))0x6c34d)(0)
printf "VIDEO_WORD_22=%u\n", $video_word
call ((void (*)())0x9899d)()
detach
