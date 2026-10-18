from pathlib import Path
import shutil,hashlib
base=Path('/media/fat/games/DoradoRTL')
new=base/'Cedar35_20261018'
core=Path('/media/fat/_Computer/DoradoRTL_Cedar35_20261018')
assert not(new/'main.log').exists()
seed=base/'System_20261017/cedar-work-native-hints-f3bce2a8.pack'
assert hashlib.sha256(seed.read_bytes()).hexdigest()=='f3bce2a8566b3bac3dd793b2016910c71b0c8d7b061b9d48c14261665802df51'
assert not(new/'cedar-work.pack').exists()
shutil.copyfile(seed,new/'cedar-work.pack')
ini=Path('/media/fat/MiSTer.ini')
s=ini.read_text()
assert '[DoradoCedar35_20261018]' not in s
shutil.copyfile(ini,new/'MiSTer.ini.before')
s+='\n[DoradoCedar35_20261018]\nvideo_mode=1920,88,44,148,1080,4,5,36,74250,1,1\nvsync_adjust=0\nmain=/media/fat/games/DoradoRTL/System_20261017/MiSTer-1fb2e098e6f0dba6\n'
ini.write_text(s)
print('Fresh Native35 seed/INI prepared; no core load yet',flush=True)
