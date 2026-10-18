#!/usr/bin/env python3
"""Build Main's actual DDR lifecycle controller against its C daemon service."""
import os
import argparse
from pathlib import Path
import subprocess
import tempfile
from contextlib import nullcontext

here=Path(__file__).resolve().parent
main=here.parent
server=main.parent/'server'
parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--main-source',type=Path,help='Also compile the actual patched Main lifecycle prologues')
parser.add_argument('--emit',type=Path,help='Keep generated hooks and binaries for cross compilation')
parser.add_argument('--no-run',action='store_true')
parser.add_argument('--static',action='store_true')
args=parser.parse_args()
with (nullcontext(str(args.emit.resolve())) if args.emit else tempfile.TemporaryDirectory(prefix='dorado-ddr-main-build-',dir='/tmp')) as tmp:
    out=Path(tmp)
    out.mkdir(parents=True,exist_ok=True)
    extra=[]
    if args.main_source:
        hooks=[]
        # Keep each exact prologue up to its first mutation/reset. Replacing
        # the hardware action with a counter permits testing failed barriers
        # without programming an FPGA or rebooting the host.
        for filename,start,end,unused,tail in (
            ('fpga_io.cpp','int fpga_load_rbf(', '\tOsdDisable();','(void)name;(void)cfg;(void)xml;','return 0;'),
            ('fpga_io.cpp','void reboot(', '\tsync();','(void)cold;',''),
            ('fpga_io.cpp','void app_restart(', '\tsync();','(void)path;(void)xml;(void)exe;',''),
            ('user_io.cpp','void user_io_init(', '\tchar *name;','(void)path;(void)xml;',''),
        ):
            source=(args.main_source/filename).read_text();i=source.index(start);j=source.index(end,i)
            prefix=source[i:j]
            assert 'dorado_net_barrier(' in prefix
            assert prefix.index('dorado_net_barrier(')<prefix.index('dorado_disk_')
            hooks.append(prefix+unused+'++lifecycle_mutations;'+tail+'}\n')
        (out/'lifecycle_hooks.inc').write_text(''.join(hooks))
        extra=['-DDORADO_TEST_LIFECYCLE','-I'+str(out)]
    objects=[]
    for name in ('dorado_ddr_ring','dorado_ddr_control'):
        obj=out/(name+'.o');objects.append(str(obj))
        subprocess.run([os.environ.get('CC','cc'),'-std=c11','-O2','-g','-Wall','-Wextra','-Werror','-c',str(server/(name+'.c')),'-o',str(obj)],check=True)
    exe=out/'test'
    subprocess.run([os.environ.get('CXX','c++'),'-std=c++14','-O2','-g','-Wall','-Wextra','-Werror','-pthread',
                    '-I'+str(server),'-I'+str(here),str(main/'dorado_ddr.cpp'),str(main/'dorado_control_client.cpp'),
                    str(here/'test_ddr_transport.cpp'),*extra,*objects,*( ['-static'] if args.static else []),'-o',str(exe)],check=True)
    if not args.no_run:subprocess.run([str(exe)],check=True,timeout=20)
