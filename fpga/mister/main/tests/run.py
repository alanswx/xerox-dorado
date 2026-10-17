#!/usr/bin/env python3
import os
import pathlib
import subprocess
import sys
import tempfile
import time

server_exe, test_exe, root = (pathlib.Path(p).resolve() for p in sys.argv[1:])
with tempfile.TemporaryDirectory(prefix="dorado-main-", dir="/tmp") as tmp:
    path = str(pathlib.Path(tmp) / "server.sock")
    proc = subprocess.Popen([str(server_exe), path,
                             str(root / "dorado/worlds/aemu.eb"),
                             str(root / "chm/bootfiles/CRTTEST.BOOT!1")])
    try:
        for _ in range(100):
            if pathlib.Path(path).exists():
                break
            assert proc.poll() is None, "server startup failed"
            time.sleep(.01)
        env = dict(os.environ, DORADO_NET_SOCKET=path)
        subprocess.run([str(test_exe), str(root / "dorado/worlds/aemu.eb")],
                       env=env, timeout=10, check=True)
    finally:
        proc.terminate()
        proc.wait(timeout=5)
