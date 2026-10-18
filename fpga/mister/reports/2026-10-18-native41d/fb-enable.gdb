set pagination off
set confirm off
python
assert "video_fb_enable" in gdb.execute("info symbol 0xac61c",to_string=True)
end
tbreak input_poll
continue
call ((void (*)(int,int))0xac61d)(1,0)
detach
