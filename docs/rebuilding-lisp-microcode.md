# Rebuilding a Dorado Lisp microcode from archive source

2026-08-12. `mb2eb` can reproduce a shipped `DORADOLISPMC.EB`
**byte-for-byte** from the archive's `.MB`, which means a microcode that
exists only as source -- Carol's and Fugue.6's 18-Feb-1984 build has no
`.eb` anywhere -- can be built and run.

This file is the procedure and, more importantly, the three things that
have to be right. Each was wrong on the first attempt and only one of
them announces itself.

## 1. The recipe

```sh
cd dorado

# 1. fetch the .MB (resolve the real filename from the directory index --
#    the cross-reference's name column does NOT match the live path, and
#    the server is case-sensitive)
curl -sS "https://xeroxparcarchive.computerhistory.org/eris/lisp/harmony/basics/.index.html" \
  | grep -io 'href="[^"]*"' | sed 's/href="//;s/"//' | grep -iE '^[A-Za-z][A-Za-z0-9]*LISP\.MB!'
curl -sS "https://xeroxparcarchive.computerhistory.org/eris/lisp/harmony/basics/DoradoLisp.MB!1" \
  -o ../chm/lisp/mb-by-release/harmony-DoradoLisp.MB

# 2. build it
./build/mb2eb --loadrampage --stamp 12004,21000,110400 --name DoradoLispMc.EB \
    ../chm/lisp/mb-by-release/harmony-DoradoLisp.MB out.eb 01070

# 3. prove it: every word but the creation date must match the original
cmp out.eb '../chm/lisp/ucode-by-release/harmony-DORADOLISPMC.EB'
```

The only differing words are **3 and 4, the creation date**, which is
deliberately not fabricated. Everything else -- all 4,526 items, the
checksum, the start address -- is identical.

## 2. The three things that must be right

**a. `--stamp RAM,BCPL,LISP` (header words 64-66).** The version numbers
are NOT in the `.MB`. The real toolchain runs LoadMB and then a SEPARATE
`StampVersions` pass (`StampD1UCode.cm`: `StampVersions DoradoLispMc.eb
12004 21000 110400`). `LISP.RUN` reads those words BEFORE loading the
microcode, so an unstamped rebuild is version 0 and gets refused with
"Microcode too old for this lisp.run" -- which reads exactly like a real
version relationship and is not. Initial's own LoadRam ignores the
header, which is why every non-Lisp world we build works without it.

**b. `--loadrampage`.** LoadMB emits the IM items for real addresses
`7600..7677`; LoadRam then skips them itself (`LoadRam.mc` `LRTypeIM`:
"Branch if would overwrite LoadRam"). They are functionally inert -- 64
items, exactly the 512-byte difference from an archive `.eb` -- but
without them you cannot do the byte comparison that catches (c).

**c. The start address is `01070`, not `01076`.** `01076` is the Dorado
default (InitMap) and is what every other world here uses; the Lisp
microcode's entry point is `01070`. This one is NOT inert: it is the
address the loader jumps to. A build with `01076` passes the version
gate, transfers the whole sysout, prints no error -- **and Lisp never
starts** (zero instruction-set-1 dispatches). It is invisible unless you
diff against the original, because the End item is the last four words of
the file.

Item ORDER also has to match to make the comparison work: LoadMB walks
the `.MB` in **image (source) order** emitting each entry's **real**
placement address, so an archive `.eb` runs 0, 1, 13, 17, 23... not
0, 1, 2, 3. LoadRam is order-independent, so this only matters for
diffing -- but the diff is what finds (c).

## 3. Getting a release's own version numbers

They are compiled into the microcode as constants, so they can be read
out of the `.MB` with no external information. `LISP0.mc`, in
`InitLispRegs`:

```
   T_ and[RamVersion, 177400]c;
   T_ T + (and[RamVersion, 377]c);        * pushed for the loader
   ... same for RamMinBcplVersion, RamMinLispVersion
```

Those six constants sit at **image 0o2032-0o2041** as `BSEL=FF,,0`
(value = FF<<8) then `BSEL=0,,FF` (value = FF) pairs:

```sh
./build/mbdis -m IM -d <file.MB> | grep -A1 -E '^ *(2032|2033|2035|2036|2040|2041):'
```

Extracting harmony's gives **12004 / 21000 / 110400**, which is exactly
its shipped stamp -- that agreement is what validates the method. Carol's
Feb-1984 `.MB` gives **12002 / 21000 / 110400**.

## 4. What is where

| file | what |
|---|---|
| `chm/lisp/mb-by-release/` | the fetched `.MB` sources, one per release |
| `build/ucode-built/` | microcodes we built (gitignored; rebuild with the above) |
| `chm/lisp/ucode-by-release/` | the shipped `.EB`s, named by release |

**Identify a microcode by CRC32, never by size or directory name.**
`lispcore-gc` and `koto` are both 36,696 bytes and different builds;
`koto` and `jcai` are byte-identical; `harmony` == `current/`; `lyric` ==
the generically named `chm/lisp/DORADOLISPMC.EB!1`. Seven distinct builds
behind ten paths. The per-release stamp table is in
`docs/lisp-distributions.md` section 4f.

## 5. Releases whose microcode exists only as source

`carol/basics` and `fugue.6/basics` hold `DORADOLISP.MB` crc `2795b272`
(18-Feb-1984) with **no matching `.EB` in the archive** -- the `.EB`
sitting beside it is the later `ee7a2f70` (05-Mar-1984), one file copied
across six release directories. So this build is reconstructable only by
the procedure above:

```sh
./build/mb2eb --loadrampage --stamp 12002,21000,110400 --name DoradoLispMc.EB \
    '../chm/lisp/mb-by-release/carol-DORADOLISP.MB!1' \
    build/ucode-built/carol-exact.eb 01070
```

To run it, the microcode must be **on the boot pack** -- `LISP.RUN` loads
`DORADOLISPMC.EB` from disk and `--boot-file 112=` is inert for a
`lisp.run/M` boot:

```sh
make lisp-dist-pack REL=carol-1983 \
     LISP_DISTPACK_UCODE=build/ucode-built/carol-exact.eb \
     LISP_DISTPACK_PREFIX=build/mx/c-exact
```
