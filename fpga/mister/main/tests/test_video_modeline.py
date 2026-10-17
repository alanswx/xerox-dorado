#!/usr/bin/env python3
"""Exercise the exact Main parser with its PLL programming boundary stubbed.

Pass video.cpp from the pinned Main checkout. The original parser must reach
the zero-clock PLL call; the patched parser must preserve raw PLL registers,
reject zero clocks, and keep the normal short modeline forms working.
"""
import argparse
import os
from pathlib import Path
import subprocess
import tempfile

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument("source", type=Path)
args = parser.parse_args()
original = args.source.read_text()
patch = Path(__file__).resolve().parents[1] / "main-video-mode-260912.patch"

prefix = r"""
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <strings.h>
using std::isfinite;
"""
stubs = r"""
static int pll_calls;
static void setPLL(double clock, vmode_custom_t *) {
    if (!(clock > 0) || !isfinite(clock)) std::exit(23);
    ++pll_calls;
}
static void cfg_error(const char *, ...) {}
template<size_t N> static char *strcpyz(char (&out)[N], const char *s) {
    std::snprintf(out, N, "%s", s); return out;
}
static int str_tokenize(char *s, const char *sep, char **out, int max) {
    int n = 0; char *state;
    for (char *p = strtok_r(s, sep, &state); p && n < max;
         p = strtok_r(nullptr, sep, &state)) out[n++] = p;
    return n;
}
static void video_calculate_cvt(int, int, float, int, vmode_custom_t *v) {
    v->Fpix = 148.5;
}
"""
cases = r"""
static int parse(const char *s, vmode_custom_t &v) {
    char text[1024]; std::snprintf(text, sizeof text, "%s", s);
    return parse_custom_video_mode(text, &v);
}
int main() {
    vmode_custom_t v{};
    assert(parse("8", v) == 8 && pll_calls == 0);
    assert(parse("1920,88,44,148,1080,4,5,36,74250", v) == -2);
    assert(v.Fpix == 74.25 && v.param.hact == 1920 && pll_calls == 1);
    assert(parse("1920,88,44,148,1080,4,5,36,148500,1,1", v) == -2);
    assert(v.Fpix == 148.5 && v.param.hpol == 1 && v.param.vpol == 1);
    assert(pll_calls == 2);
    // This exact raw form stalled Main during the FPGA boot.
    assert(parse("1,1280,110,40,220,720,5,5,20,4,1028,3,65536,5,771,9,2,8,7,7,3908420239,0,0,4,1,0", v) == -2);
    assert(pll_calls == 2 && v.Fpix == 0 && v.item[20] == 3908420239U);
    assert(v.param.hact == 1280 && v.param.vact == 720 && v.param.vic == 4);
    assert(parse("1920,88,44,148,1080,4,5,36,0", v) == -1);
    assert(pll_calls == 2);
    assert(parse("", v) == -1);
    std::puts("PASS raw PLL preservation, zero-clock rejection, 9/11-item modes");
}
"""


def excerpt(source):
    start = source.index("struct vmode_custom_param_t\n")
    types = source[start:source.index("static_assert(", start)]
    start = source.index("static int parse_custom_video_mode(")
    function = source[start:source.index("static int store_custom_video_mode(", start)]
    return prefix + types + stubs + function + cases


with tempfile.TemporaryDirectory(prefix="dorado-video-parser-") as tmp:
    root = Path(tmp)
    (root / "video.cpp").write_text(original)
    subprocess.run(["patch", "-p1", "-i", str(patch)], cwd=root, check=True,
                   capture_output=True, text=True)
    for name, source in [("original", original), ("patched", (root / "video.cpp").read_text())]:
        cpp, binary = root / (name + ".cpp"), root / name
        cpp.write_text(excerpt(source))
        subprocess.run([os.environ.get("CXX", "c++"), "-std=c++14", "-O2", str(cpp),
                        "-o", str(binary)], check=True)
        result = subprocess.run([str(binary)], capture_output=True, text=True, timeout=3)
        if name == "original":
            assert result.returncode == 23, result
            print("PASS mutation control: original parser reaches zero-clock PLL call")
        else:
            assert result.returncode == 0, result
            print(result.stdout.strip())
