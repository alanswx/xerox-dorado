#!/usr/bin/env python3
"""gen_scan.py -- the state scan chain of dorado_rtl_{cpu,mem,io}.

The checkpoint files (init_*/checkpoint_{cpu,mem,io}.svh, written by
sim_cedar.cpp CEDAR_DUMP_INIT) name every register a saved machine state
sets. This writes, for each module, scan_<m>.svh: one shift of all those
registers as a single chain, in the files' order, most significant bit
first -- the bit shifted in at scan_in ends, after as many shifts as the
chain is long, where the first register's top bit is. And
scan_<m>_widths.svh, which prints each register's width in simulation
(+scanmap), from which the host's map (hps/scan_map.h) is made:

    tools/gen_scan.py                   # the three .svh pairs, from init_lisp
    tools/gen_scan.py --map LOGFILE     # hps/scan_map.h from a +scanmap run
    tools/gen_scan.py --regs INIT_DIR OUT   # a world's registers as one file for
                                        # the host (<snapshot>.regs): the three
                                        # checkpoint files under @cpu, @mem, @io
    tools/gen_scan.py --rams INIT_DIR OUT   # and its memories (<snapshot>.rams):
                                        # the load records of dorado_rtl_mailbox,
                                        # 64 bits each, low byte first, from the
                                        # directory's .hex files

The registers are shifted in chunks, last chunk first, each handing its
old top bit to the chunk before it (sc[k]); a disk register is in a chunk
of its own kind, compiled out with the controller (DORADO_NO_DISK).
"""
import re, sys, os
HERE = os.path.dirname(os.path.abspath(__file__))
RTL = os.path.dirname(HERE)
CHUNK = 32
# Not in the chain:
#  - the cache's row arrays: a restored machine starts with an empty cache
#    (reset leaves every line invalid); whoever writes the checkpoint puts
#    the dirty lines into storage first. 6,400 bits that cost a logic cell
#    each as a chain, and the cache words need not be loaded either.
#  - ddc_hram, the display's horizontal RAM (4,096 bits): the RTL display
#    does not read it (its timing is fixed), so a restored core leaves it 0.
#  - task_md_ready: the Md-ready time of the hold model; 0 (ready) will do.
SKIP = ('cache_tag', 'cache_lru', 'cache_valid', 'cache_dirty', 'cache_wp', 'cache_vacant',
        'cache_being_loaded', 'ddc_hram', 'task_md_ready',
        # written and never read (counts and last-reference records for the
        # simulation's eyes): a restored machine does not need them
        'ifu_dispatch_count', 'ddc_frame_count', 'ddc_scan_pixel',
        'last_fault_va', 'last_ref_va', 'last_ref_b')

# Registers declared wider than what they hold (an int for a small index or
# -1): only the low bits are in the chain, the top one of them the sign, and
# the rest of the register follows it. The bits out of the chain then all
# have one source and cost one cell between them.
NARROW = {'ifu_dispatch_rbase': 9, 'mapbuf_busy_slot': 5, 'mapbuf_busy_cycles': 5,
          'pipe_head': 5, 'last_cache_row': 7, 'last_cache_way': 3, 'last_ref_latency': 6,
          'eth_rx_count': 14, 'eth_rx_pos': 14, 'ddc_fifo_a_head': 7, 'ddc_fifo_a_tail': 7,
          'ddc_fifo_b_head': 7, 'ddc_fifo_b_tail': 7, 'ddc_scan_line': 12}
def cut(n): return f'{n}[{NARROW[n] - 1}:0]' if n in NARROW else n

# A register file of ARRAY_MIN or more entries (BR, the per-task files, the
# Pipe, ...) is not in the chain either: each entry is a load record
# (dorado_rtl_mailbox; kind 8 cpu, 9 mem, 10 io; the file's number in bits
# 59:52, the entry in 47:32, the value in 31:0), written by one assignment
# for the whole file. The record ROTATES the file one entry (entry i takes
# entry i + 1, the last takes the record's value), so a file's entries are
# sent first to last; and the same record with the file's entry 0 as its
# value (fr_q, scan_<m>_rd.svh) reads the file out and leaves it as it was
# after as many records as it has entries -- a save.
ARRAY_MIN = 16
LD_KIND = {'cpu': 8, 'mem': 9, 'io': 10}

# Registers of the RTL alone (no checkpoint file names them; a checkpoint
# made from the C machine starts them at 0, as reset does) that a save of a
# running core must carry: the terminal's serializer, which is part way
# through a 32-bit message (HM Table 24) for most of the time -- restored
# at bit 0 while the microcode holds half a message, the next keyboard word
# is assembled wrong and the world sees keys go down.
EXTRA = {'io': ['ddc_term_msg_bit', 'ddc_term_msg_word', 'ddc_term_msg_type',
                'ddc_term_msg_body', 'ddc_term_msg_active']}
NARROW.update({'ddc_term_msg_bit': 7, 'ddc_term_msg_word': 4, 'ddc_term_msg_type': 4})

def all_names(mod):
    out = []
    for line in open(os.path.join(RTL, 'init_lisp', f'checkpoint_{mod}.svh')):
        m = re.match(r'\s*([A-Za-z_][\w\[\]]*)\s*=\s*64\'h[0-9a-fA-F]+;', line)
        if m and re.sub(r'\[.*', '', m.group(1)) not in SKIP: out.append(m.group(1))
    # (before the disk's registers, which end the io chain in chunks of their own)
    k = next((i for i, n in enumerate(out) if n.startswith('disk_')), len(out))
    return out[:k] + EXTRA.get(mod, []) + out[k:]

def arrays(mod):
    """[(name, entries)] of the files loaded by record, in the files' order"""
    n = {}
    for x in all_names(mod):
        m = re.match(r'(\w+)\[(\d+)\]$', x)
        if m: n[m.group(1)] = max(n.get(m.group(1), 0), int(m.group(2)) + 1)
    return [(k, v) for k, v in n.items() if v >= ARRAY_MIN]

def names(mod):
    arr = {k for k, _ in arrays(mod)}
    return [x for x in all_names(mod) if re.sub(r'\[.*', '', x) not in arr]

def chunks(ns):
    """runs of at most CHUNK names, never mixing disk and other registers"""
    res, cur, kind = [], [], None
    for n in ns:
        k = n.startswith('disk_')
        if cur and (k != kind or len(cur) >= CHUNK):
            res.append((kind, cur)); cur = []
        kind = k; cur.append(n)
    if cur: res.append((kind, cur))
    return res

def gen(mod):
    ns = names(mod)
    ch = chunks(ns)
    K = len(ch)
    with open(os.path.join(RTL, f'scan_{mod}.svh'), 'w') as f:
        f.write(f'// generated by rtl_core/tools/gen_scan.py; do not edit ({len(ns)} registers, {K} chunks)\n')
        f.write(f'sc[{K}] = scan_in;\n')
        for k in range(K - 1, -1, -1):
            disk, c = ch[k]
            body = ', '.join(cut(n) for n in c)
            if disk: f.write('`ifndef DORADO_NO_DISK\n')
            f.write(f'{{sc[{k}], {body}}} = {{{body}, sc[{k+1}]}};\n')
            if disk: f.write(f'`else\nsc[{k}] = sc[{k+1}];\n`endif\n')
        for n in ns:
            if n in NARROW: f.write(f"{n} = $bits({n})'($signed({cut(n)}));\n")
        f.write('scan_out = sc[0];\n')
    with open(os.path.join(RTL, f'scan_{mod}_decl.svh'), 'w') as f:
        f.write(f'// generated by rtl_core/tools/gen_scan.py; do not edit\nlogic [{K}:0] sc;\n')
    with open(os.path.join(RTL, f'scan_{mod}_widths.svh'), 'w') as f:
        f.write('// generated by rtl_core/tools/gen_scan.py; do not edit\n')
        for n in ns:
            if n.startswith('disk_'): f.write('`ifndef DORADO_NO_DISK\n')
            f.write(f'$display("SCANMAP {mod} {n} %0d", {NARROW[n] if n in NARROW else f"$bits({n})"});\n')
            if n.startswith('disk_'): f.write('`endif\n')
    with open(os.path.join(RTL, f'scan_{mod}_ld.svh'), 'w') as f:
        f.write('// generated by rtl_core/tools/gen_scan.py; do not edit\ncase (ld_d[59:52])\n')
        for i, (a, n) in enumerate(arrays(mod)):
            if a.startswith('disk_'): f.write('`ifndef DORADO_NO_DISK\n')
            f.write(f"8'd{i}: begin for (int i = 0; i < {n - 1}; i++) {a}[i] = {a}[i + 1]; {a}[{n - 1}] = $bits({a}[0])'(ld_d[31:0]); end\n")
            if a.startswith('disk_'): f.write('`endif\n')
        f.write('default: ;\nendcase\n')
    with open(os.path.join(RTL, f'scan_{mod}_rd.svh'), 'w') as f:
        f.write('// generated by rtl_core/tools/gen_scan.py; do not edit\ncase (ld_d[59:52])\n')
        for i, (a, n) in enumerate(arrays(mod)):
            if a.startswith('disk_'): f.write('`ifndef DORADO_NO_DISK\n')
            f.write(f"8'd{i}: fr_q = 32'({a}[0]);\n")
            if a.startswith('disk_'): f.write('`endif\n')
        f.write("default: fr_q = 32'd0;\nendcase\n")
    return len(ns)

def ldfile():
    with open(os.path.join(RTL, 'hps', 'scan_ld.h'), 'w') as f:
        f.write('// generated by rtl_core/tools/gen_scan.py; do not edit\n')
        f.write('// The register files loaded by record: {module, file, kind, number, entries}.\n')
        rows = [(m, a, LD_KIND[m], i, n) for m in ('cpu', 'mem', 'io') for i, (a, n) in enumerate(arrays(m))]
        f.write(f'#define SCAN_LD_FILES {len(rows)}\n')
        f.write('static const struct { const char *mod, *name; int kind, id, n; } scan_ld[SCAN_LD_FILES] = {\n')
        for r in rows: f.write(f'    {{"{r[0]}", "{r[1]}", {r[2]}, {r[3]}, {r[4]}}},\n')
        f.write('};\n')

def mapfile(log):
    rows = []
    for line in open(log, errors='replace'):
        m = re.match(r'SCANMAP (\w+) (\S+) (\d+)', line)
        if m: rows.append((m.group(1), m.group(2), int(m.group(3))))
    # the chain: the mailbox feeds io, io feeds mem, mem feeds cpu; so the
    # stream is cpu's registers, then mem's, then io's
    order = {'cpu': 0, 'mem': 1, 'io': 2}
    rows.sort(key=lambda r: order[r[0]])     # stable: file order within a module
    # each module hands its bit on through a register (scan_out): one more
    # place in the chain after mem's registers and after io's
    out = []
    for i, r in enumerate(rows):
        if i and r[0] != rows[i - 1][0]: out.append(('pad', rows[i - 1][0] + '.scan_out', 1))
        out.append(r)
    rows = out
    total = sum(r[2] for r in rows)
    with open(os.path.join(RTL, 'hps', 'scan_map.h'), 'w') as f:
        f.write('// generated by rtl_core/tools/gen_scan.py --map; do not edit\n')
        f.write('// The state scan chain, in stream order: {module, register, bits}.\n')
        f.write(f'#define SCAN_BITS {total}\n#define SCAN_REGS {len(rows)}\n')
        f.write('static const struct { const char *mod, *name; int bits; } scan_map[SCAN_REGS] = {\n')
        for r in rows: f.write(f'    {{"{r[0]}", "{r[1]}", {r[2]}}},\n')
        f.write('};\n')
    print(f'{len(rows)} registers, {total} bits')

def regs(d, out):
    with open(out, 'w') as f:
        for m in ('cpu', 'mem', 'io'):
            f.write(f'@{m}\n')
            for line in open(os.path.join(d, f'checkpoint_{m}.svh')):
                mm = re.match(r"\s*([A-Za-z_][\w\[\]]*)\s*=\s*64'h([0-9a-fA-F]+);", line)
                if not mm: continue
                n, v = mm.group(1), int(mm.group(2), 16) & 0xffffffff
                if n in NARROW:      # the value must be the sign extension of its low bits
                    lo = v & ((1 << NARROW[n]) - 1)
                    if lo >> (NARROW[n] - 1): lo |= 0xffffffff & ~((1 << NARROW[n]) - 1)
                    if lo != v: sys.exit(f'{n} = {v:#x} does not fit the {NARROW[n]} bits the chain carries')
                f.write(f'{mm.group(1)} {mm.group(2)}\n')
            for n in EXTRA.get(m, []): f.write(f'{n} 0\n')

def rams(d, out):
    import struct
    def hexf(n): return [int(x, 16) for x in open(os.path.join(d, n + '.hex')).read().split()]
    recs = []
    w0, w1, w2, pr = hexf('im_iw0'), hexf('im_iw1'), hexf('im_iw2'), hexf('im_present')
    recs += [1 << 60 | a << 48 | w0[a] << 32 | w1[a] << 16 | w2[a] for a in range(4096) if pr[a]]
    lo, hi, pr = hexf('ifum_lo'), hexf('ifum_hi'), hexf('ifum_present')
    for a in range(1024):
        if pr[a]: recs += [2 << 60 | a << 16 | lo[a], 3 << 60 | a << 16 | hi[a]]
    rm, stk = hexf('rm'), hexf('stk')
    recs += [4 << 60 | a << 16 | rm[a] for a in range(256)]
    recs += [4 << 60 | (256 + a) << 16 | stk[a] for a in range(256)]
    al, pr = hexf('alufm'), hexf('alufm_present')
    recs += [5 << 60 | a << 16 | al[a] for a in range(16) if pr[a]]
    rp, wp, di, rf = hexf('map_rp'), hexf('map_wp'), hexf('map_dirty'), hexf('map_ref')
    recs += [6 << 60 | a << 32 | wp[a] << 18 | di[a] << 17 | rf[a] << 16 | rp[a] for a in range(len(rp))]
    rx, at = hexf('eth_rx_words'), hexf('eth_rx_attention')
    recs += [7 << 60 | at[a] << 32 | a << 16 | rx[a] for a in range(4096)]
    recs.append(15 << 60)
    with open(out, 'wb') as f:
        for r in recs: f.write(struct.pack('<Q', r))
    print(f'{len(recs)} load records')

if len(sys.argv) > 2 and sys.argv[1] == '--map': mapfile(sys.argv[2])
elif len(sys.argv) > 3 and sys.argv[1] == '--rams': rams(sys.argv[2], sys.argv[3])
elif len(sys.argv) > 3 and sys.argv[1] == '--regs': regs(sys.argv[2], sys.argv[3])
else:
    for m in ('cpu', 'mem', 'io'): print(m, gen(m))
    ldfile()
