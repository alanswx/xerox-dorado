# Why Medley could not load from {DORADO} -- RESOLVED

2026-08-11. **Resolved: it was the `{DORADO} Login:` prompt, not a missing
protocol.** Answering it with `Guest` unblocks the library --
`STP_TRANSFER HRULE.LCOM (3125 bytes)` on the wire and +4,622 pixels on
screen.

The rest of this file is kept because the reasoning below is wrong in an
instructive way, and section 3 in particular must not be repeated.

**What was wrong.** I read `FTP_UNSERVED type=0o223` in the trace as "we
never answer this" and built a whole implementation plan on it. We do
answer it: `PUP_ADDRLOOKUP -> AddressIs "Dorado"` appears two lines later
in the same log. `FTP_UNSERVED` is logged partway through
`eth_tx_packet_done()`, and Mayday, EFTP-Ack, AddressLookup, GatewayInfo
and AltoTime are all handled *after* that point -- so the label was
lying. Fixed in `src/ethernet.c`: those five types are now skipped by the
FTP_UNSERVED log, with a comment saying why.

**What actually blocks.** Reading the guest's screen (rendering the
changed pixels as ASCII, because `tools/pgm_text.py` does not know
Medley's font and returns `?` for every glyph) showed the echoed command
followed by `Login` and `NoName`. `docs/lisp-leaf-handoff.md:140` already
said it: *"Any scripted run must answer it (`Guest` / `Guest`)."* The
Koto colour recipe does exactly that at `Makefile:459`.

**The lesson**: a trace label is a claim by our own code, not evidence.
Two independent signals -- the wire AND the screen -- agree only after
the login; either alone would have misled.

---

## Superseded analysis (kept for the protocol notes, which are correct)

The Pup facts below are accurate and worth keeping; only the conclusion
that we fail to serve 223b was wrong.

## 1. The observation

With `DORADO_FTP_TRACE=1`, driving `(LOAD '{DORADO}HRULE.LCOM)` in the
guest after the desktop is up:

```
FTP_NETDIR request  id=00000613 src=000442/1/124665 text="DORADO"
FTP_NETDIR lookup release host -> STP
FTP_QUEUE    type=0o221 id=0000018b bytes=6 d=000442/1/124665 s=000401/0/4
FTP_UNSERVED type=0o223 id=0000018c d=000400/0/4 s=000442/1/124665
FTP_UNSERVED body len=6 words: 000401 000000 000003  text="......"
```

Name resolution works: the guest asks for `DORADO`, we answer with a
NetDirReply (221b). The guest then sends **223b to socket 4** and we never
answer, so its file open never proceeds.

## 2. What 223b is, from Xerox's own source

`chm/altosource/netexec.dm!4_/NetExec1.bcpl` (Boggs, 1 July 1983):

```
psMiscServ      = 4
ptAddressLookup = 223b
ptAddressReply  = 224b
ptAddressError  = 222b
```

and the client, `GetName()`:

```
pbi>>PBI.pup.type       = ptAddressLookup
pbi>>PBI.pup.length     = pupOvBytes+6
pbi>>PBI.pup.dPort.socket^2 = psMiscServ
let port = lv pbi>>PBI.pup.bytes^1
port>>Port.net  = ndb>>NDB.localNet
port>>Port.host = ndb>>NDB.localHost
SimplePup(pbi, 100, 500, InstallName)
```

So **223b is the REVERSE of the name lookup we already serve**: NetDir
(220b/221b) is name -> address; AddressLookup is **address -> name**. The
6-byte body is a `Port` (net byte, host byte, socket 2 words).

Our trace body `000401 000000 000003` decodes as net 1, host 1, socket 3 --
which is our STP socket, the address we had just handed the guest in the
NetDir reply. Medley resolves `DORADO`, then asks us what that address is
*called*, and blocks until told.

`InstallName` accepts exactly two answers and retries on anything else:

```
case ptAddressReply:  copy the name bytes, done
case ptAddressError:  ourName = -1, done
default:              resultis true   // keep trying
```

`SimplePup(pbi, 100, 500, ...)` retransmits every 100 ticks and gives up
after 500 -- which is why this shows as a burst of identical unserved
packets rather than one.

## 3. Why our existing handler does not fire

We already implement this exchange -- `dorado/src/ethernet.c:3886`, with
`DORADO_PUP_TYPE_ADDRESS_LOOKUP 0223` / `ADDRESS_REPLY 0224` in
`include/ethernet.h`. It was written for NetExec's `GetName` and answers
the literal name "Dorado".

It is guarded on

```c
if (eth->tx_words[1] == DORADO_PUP_TYPE_ETHERNET &&
    eth->tx_words[3] == DORADO_PUP_TYPE_ADDRESS_LOOKUP) {
```

The Medley packet fails that guard and falls through the chain in
`eth_tx_packet` (RFC -> `eth_ftp_select_conn` -> `eth_leaf_handle` ->
`FTP_UNSERVED`). So this is **not a new protocol to write** -- it is an
existing handler that the Medley path does not reach.

## 4. What it takes

Small, and in one file.

1. **Find out why the guard fails.** Print `tx_words[0..3]` for the
   unserved 223b packet. Either `tx_words[1]` is not `01000` on this path
   (different encapsulation), or the packet is reaching `eth_tx_packet`
   through a route that has already consumed those words. One trace run
   answers it; no code change needed to find out.
2. **Move the AddressLookup test into the same fall-through chain** that
   Leaf uses, i.e. an `eth_addrlookup_handle(eth)` called next to
   `eth_leaf_handle(eth)`, keyed on the Pup type in `tx_words[3]` and
   destination socket 4, not on the encapsulation word. That makes it
   reachable from every path rather than only the raw one.
3. **Answer with the name the guest asked about**, not a constant. The
   body is the `Port` being queried; the reply's whole data section is the
   name bytes with **no leading string length** (`InstallName` computes
   `numChars` from `pup.length - pupOvBytes`). For net 1 / host 1 that is
   our STP server, so `"Dorado"` is the right answer today -- but keying
   it on the queried port is what makes it correct when there is more
   than one service.
4. **Implement 222b `ptAddressError`** for a port we do not recognise.
   Without it an unknown address costs the guest its full 500-tick retry
   budget instead of failing immediately. We do not currently define this
   constant at all.
5. **Gate it.** Mirror `make verify-lisp-leaf`: drive
   `(LOAD '{DORADO}<pkg>)` in Medley and assert on the trace that the
   server both saw the request for that file and served it -- a pixel
   count cannot tell "loaded" from "not found", and `tools/pgm_text.py`
   cannot read Medley's screen font (verified: it returns `?` for every
   glyph on this world).

## 5. Caveat worth keeping

Answering 223b unblocks the *name* step. Whether the file open then
succeeds is a separate question -- the next thing the guest sends may be
Leaf (0o260, which we serve) or old STP (which we serve), or something
else again. The honest expectation is that this clears one blocking step
and reveals the next; the trace will say which.

Note also this is why Lyric works and Medley does not: Lyric's INIT puts
`{DORADO}<>` in `DIRECTORIES` and reaches the file server without the
reverse-name step. Medley asks first.
