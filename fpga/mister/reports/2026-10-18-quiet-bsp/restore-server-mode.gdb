set pagination off
set confirm off
set args /tmp/dorado-net.sock /media/fat/games/DoradoRTL/System_20261017/dorado/worlds/aemu.eb /media/fat/games/DoradoRTL/System_20261017/chm/bootfiles/NETEXEC.BOOT!8 /media/fat/games/DoradoRTL/System_20261017/chm/cedar/stp-root
break poll
run
up
python
assert 'main' in gdb.newest_frame().older().name() or gdb.selected_frame().name()=='main'
end
print/x eth.last_boot_offset
set var eth.last_boot_offset = 0113
print/x eth.last_boot_offset
detach
