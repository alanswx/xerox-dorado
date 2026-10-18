from pathlib import Path
import shutil, subprocess
old=Path('/home/alans/mister/Dorado_Native28_20261018')
new=Path('/home/alans/mister/Dorado_Native29_20261018')
assert not new.exists(), 'Do not replace an existing candidate'
new.mkdir()
shutil.copytree(old/'src', new/'src')
shutil.copytree(old/'core', new/'core', ignore=shutil.ignore_patterns('db', 'incremental_db', 'output_files', '*.rpt', '*.summary', '*.log', 'c5_pin_model_dump.txt'))
for p in (new/'core').rglob('*'):
    if p.is_file() and p.suffix in ('.qsf', '.qip', '.sdc', '.tcl', '.qpf'):
        p.write_text(p.read_text().replace(str(old), str(new)))
p=new/'core/rtl/pll/pll_0002.v'
s=p.read_text()
assert s.count('output_clock_frequency0("33.333333 MHz")')==1
p.write_text(s.replace('output_clock_frequency0("33.333333 MHz")', 'output_clock_frequency0("40.000000 MHz")'))
shutil.copy2(new/'core/native26-before-review.sdc', new/'core/native_timing.sdc')
print('Native29: Native28 logic unchanged; 40 MHz PLL / SYSPER4. Requires fresh STA/clock/hold review and hardware test.', flush=True)
q=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin')
for exe,args,name in [('quartus_map',['Dorado'],'map'),('quartus_fit',['Dorado'],'fit'),('quartus_asm',['Dorado'],'asm'),('quartus_sta',['Dorado'],'sta'),('quartus_sta',['-t','audit_native_clocks.tcl'],'audit')]:
    print('Native29',name,flush=True)
    with (new/(name+'.log')).open('w') as f:
        subprocess.run([str(q/exe),*args],cwd=new/'core',stdout=f,stderr=subprocess.STDOUT,check=True)
print('Native29 fit complete; review required before deployment',flush=True)
