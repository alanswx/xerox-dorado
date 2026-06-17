#!/usr/bin/env python3
"""tioga2txt - extract clean plaintext source from Xerox PARC Tioga files.

Tioga is the Cedar text editor's rich-text on-disk format.  A Tioga file is
NOT plain text: the readable source sits in a leading "text block", but the
document structure (node nesting -> indentation) and the comments live in
two further blocks at the end, followed by a small fixed trailer.  Just
cat-ing the file therefore (a) prints a binary trailer of formatting junk
and (b) DROPS every comment, because comments are stored separately from the
code text.  This tool reconstructs the document in reading order.

================================================================================
TIOGA ON-DISK FORMAT  (reverse-engineered from Rochus Keller's TiogaReader.cpp,
which is itself a port of the 1993 Xerox Cedar reader)
================================================================================

A Tioga file has four parts laid out end to end:

    [ TEXT block ][ COMMENT block ][ CONTROL block ][ TRAILER ]

  TRAILER  - the last 14 bytes.  Layout:
       2 bytes  trailer id = 0x85 0x97
       4 bytes  propLen     (length of property data; usually 0)
       4 bytes  textLen     (byte length of the TEXT block)
       4 bytes  totalSize   (must equal the whole file length -> sanity check)

  Length fields are 4 bytes in a quirky order (NOT plain big/little endian):
       value = b[0]<<8 | b[1] | b[2]<<24 | b[3]<<16

  TEXT block    bytes [0, textLen).  Raw characters of all *code* nodes,
                concatenated in document order, each node terminated by a CR
                (0x0D).  This is where "naive" extraction stops - but it is
                missing every comment.

  COMMENT block bytes [textLen, textLen+commentLen).
                2-byte id 0x00 0x00, then a 4-byte length, then the characters
                of all *comment* nodes, again CR-terminated.

  CONTROL block bytes [textLen+commentLen, fileLen-14).
                2-byte id 0x9d 0xca, then a 4-byte length, then a stream of
                one-byte opcodes that re-assemble the document: they start and
                end nodes (the nesting depth = indentation), and pull text out
                of the TEXT or COMMENT block in the right order.

  Control opcodes that matter for plaintext:
       endOfFile (0)            end of stream
       startNode/startNodeFirst.. , terminalTextNode/terminalTextNodeFirst..
                                begin a node (terminal = leaf, no children).
                                A node start *pushes* onto the level stack;
                                stack depth drives indentation.
       endNode                  pop the level stack
       rope    (152)            text for current node lives in TEXT block:
                                read GetInt() chars (+1 trailing CR) from TEXT
       comment (153)            same, but text lives in the COMMENT block
       runs/looks/look1..3      character formatting (fonts/bold) - SKIPPED,
                                irrelevant to plaintext, but their operands
                                must still be consumed to stay in sync
       prop/propShort           node properties - consumed and ignored

  Two integer encodings appear in the control stream:
       GetInt : variable length, 7 bits per byte, low byte first,
                high bit (0x80) = "more bytes follow"
       GetStr : one length byte, then that many raw chars

  Character fix-ups (original Cedar charset -> ASCII/Unicode):
       0xD3        -> ©   (copyright sign)
       0xAC, '_'   -> ←   (the Mesa/Cedar assignment arrow; '_' was *displayed*
                           as a left arrow and is the assignment operator, not
                           an identifier char in Cedar source)
       CR (0x0D)   -> LF  (line separator)

If the file does not parse as Tioga (bad trailer / lengths), we fall back to
emitting the bytes verbatim with only the charset fix-ups applied.
================================================================================
"""

import sys

# --- control opcode constants (see TiogaReader.cpp enum tioga_ControlOp) ---
NUM_FORMATS = 70
NUM_LOOKS = 50
endOfFile = 0
startNode = 1
startNodeFirst = 2
startNodeLast = startNodeFirst + NUM_FORMATS            # 72
terminalTextNode = startNodeLast + 1                    # 73
terminalTextNodeFirst = terminalTextNode + 1            # 74
terminalTextNodeLast = terminalTextNodeFirst + NUM_FORMATS  # 144
otherNode = terminalTextNodeLast + 1                    # 145
otherNodeShort = otherNode + 1
otherNodeSpecs = otherNodeShort + 1
otherNodeSpecsShort = otherNodeSpecs + 1
prop = otherNodeSpecsShort + 1                          # 149
propShort = prop + 1                                    # 150
endNode = propShort + 1                                 # 151
rope = endNode + 1                                      # 152
comment = rope + 1                                      # 153
runs = comment + 1                                      # 154
looks = runs + 1                                        # 155
looksFirst = looks + 1                                  # 156
looksLast = looksFirst + NUM_LOOKS                      # 206
look1 = looksLast + 1                                   # 207
look2 = look1 + 1                                       # 208
look3 = look2 + 1                                       # 209

TRAILER_LEN = 2 + 3 * 4
HEADER_LEN = 2 + 4  # id + length, for comment and control blocks


def _getlen(b, p):
    return (b[p] << 8) | b[p + 1] | (b[p + 2] << 24) | (b[p + 3] << 16)


def fixchar(ch):
    if ch == 0xD3:
        return '©'      # ©
    if ch == 0xAC or ch == ord('_'):
        return '←'      # ←
    if ch == 0x0D or ch == 0x0A:
        return '\n'      # CR (Tioga) or LF (already-plain dumps) -> newline
    if ch == 0x09:
        return '\t'
    if 0x20 <= ch <= 0x7E:
        return chr(ch)
    # Other Latin-1 / control bytes: keep printable Latin-1, drop the rest.
    if ch >= 0xA0:
        return chr(ch)
    return ''


def to_text(buf):
    return ''.join(fixchar(c) for c in buf)


class Reader:
    def __init__(self, buf):
        self.buf = buf
        n = len(buf)
        if n < TRAILER_LEN:
            raise ValueError("too short")
        p = n - TRAILER_LEN
        if buf[p] != 0x85 or buf[p + 1] != 0x97:
            raise ValueError("bad trailer id")
        propLen = _getlen(buf, p + 2)
        textLen = _getlen(buf, p + 6)
        total = _getlen(buf, p + 10)
        if total != n or propLen > n or textLen > n:
            raise ValueError("bad trailer lengths")
        if buf[textLen] != 0 or buf[textLen + 1] != 0:
            raise ValueError("bad comment id")
        commentLen = _getlen(buf, textLen + 2)
        cp = textLen + commentLen
        if buf[cp] != 0x9D or buf[cp + 1] != 0xCA:
            raise ValueError("bad control id")
        controlLen = _getlen(buf, cp + 2)
        if n != textLen + commentLen + controlLen:
            raise ValueError("section lengths inconsistent")

        # stream cursors: (pos, limit)
        self.text_p, self.text_lim = 0, textLen
        self.com_p = textLen + HEADER_LEN
        self.com_lim = textLen + commentLen
        self.ctl_p = textLen + commentLen + HEADER_LEN
        self.ctl_lim = n - TRAILER_LEN

        self.formats = [None]      # index 0 = null format
        self.out = []
        # Two counters, exactly as in the reference reader:
        #   stack_size - size of the node stack (terminal + non-terminal);
        #                drives indentation as (stack_size-2)*4 spaces.
        #   level      - count of OPEN non-terminal nodes; drives the
        #                endNode / endOfFile balancing.
        self.stack_size = 0
        self.level = 0

    # --- control-stream primitives ---
    def get_op(self):
        if self.ctl_p < self.ctl_lim:
            v = self.buf[self.ctl_p]
            self.ctl_p += 1
            return v
        return endOfFile

    def get_byte(self):
        if self.ctl_p < self.ctl_lim:
            v = self.buf[self.ctl_p]
            self.ctl_p += 1
            return v
        return 0

    def get_int(self):
        result = 0
        nbits = 0
        while self.buf[self.ctl_p] & 0x80:
            result |= (self.buf[self.ctl_p] & 0x7F) << nbits
            self.ctl_p += 1
            nbits += 7
        result |= (self.buf[self.ctl_p] & 0x7F) << nbits
        self.ctl_p += 1
        return result

    def get_str(self):
        n = self.buf[self.ctl_p]
        self.ctl_p += 1
        s = self.buf[self.ctl_p:self.ctl_p + n]
        self.ctl_p += n
        return bytes(s)

    def add_format(self, name):
        if not name:
            return 0
        for i in range(1, len(self.formats)):
            if self.formats[i] == name:
                return i
        if len(self.formats) < NUM_FORMATS:
            self.formats.append(name)
            return len(self.formats) - 1
        return 0

    def sget_text(self, which, length):
        if which == 'text':
            s = self.buf[self.text_p:self.text_p + length]
            self.text_p += length
        else:
            s = self.buf[self.com_p:self.com_p + length]
            self.com_p += length
        return bytes(s)

    def insert_text(self, raw, length, is_comment):
        # raw has length+1 bytes (trailing CR); emit only `length` chars.
        body = to_text(raw[:length])
        indent = ' ' * max(0, (self.stack_size - 2) * 4)
        lines = body.split('\n')
        first = True
        for line in lines:
            text = line
            if is_comment:
                trimmed = line.strip()
                if trimmed and not trimmed.startswith('--'):
                    text = '-- ' + line
                if self.stack_size == 1 and not trimmed:
                    return
            self.out.append((indent if first else '') + text)
            first = False

    def run(self):
        terminal_node = False
        last_was_terminal = False
        op = self.get_op()
        while True:
            iformat = 0
            if terminalTextNodeFirst <= op <= terminalTextNodeLast:
                iformat = op - terminalTextNodeFirst
                if iformat >= len(self.formats):
                    iformat = 0
                terminal_node = True
            elif startNodeFirst <= op <= startNodeLast:
                iformat = op - startNodeFirst
                if iformat >= len(self.formats):
                    iformat = 0
                terminal_node = False
            else:
                if op == endNode:
                    self.level -= 1
                    if self.stack_size > 0:
                        self.stack_size -= 1   # EndNode() pops the stack
                    op = self.get_op()
                    continue
                elif op == startNode:
                    iformat = self.add_format(self.get_str())
                    terminal_node = False
                elif op == terminalTextNode:
                    iformat = self.add_format(self.get_str())
                    terminal_node = True
                elif op == rope or op == comment:
                    length = self.get_int()
                    raw = self.sget_text('text' if op == rope else 'com',
                                         length + 1)
                    self.insert_text(raw, length, op == comment)
                    op = self.get_op()
                    continue
                elif op == runs:
                    nruns = self.get_int()
                    for _ in range(nruns):
                        op = self.get_op()
                        if looksFirst <= op <= looksLast:
                            pass
                        elif look1 <= op <= look3:
                            for _ in range(op - look1 + 1):
                                self.get_byte()
                        elif op == looks:
                            for _ in range(4):
                                self.get_byte()
                        self.get_int()  # run length
                    op = self.get_op()
                    continue
                elif op == prop:
                    self.get_str()
                    length = self.get_int()
                    self.ctl_p += length  # skip prop rope (from control stream)
                    op = self.get_op()
                    continue
                elif op == propShort:
                    self.get_byte()
                    length = self.get_int()
                    self.ctl_p += length
                    op = self.get_op()
                    continue
                elif op == endOfFile:
                    if self.level == 0:
                        break
                    op = endNode      # supply missing endNode ops
                    continue
                else:
                    op = self.get_op()
                    continue

            if self.level == 0 and op == endOfFile:
                break
            # start a new node
            if last_was_terminal and self.stack_size > 0:
                self.stack_size -= 1          # EndNode() the prior terminal
            last_was_terminal = terminal_node
            self.stack_size += 1              # StartNode() pushes
            if not terminal_node:
                self.level += 1
            op = self.get_op()

        return '\n'.join(self.out) + '\n'


def convert(buf):
    try:
        return Reader(buf).run()
    except Exception:
        # Not a (valid) Tioga file - emit verbatim with charset fix-ups.
        return to_text(buf)


def main(argv):
    if len(argv) != 2:
        sys.stderr.write("usage: tioga2txt.py <tioga-file>\n")
        return 2
    with open(argv[1], 'rb') as f:
        buf = f.read()
    out = convert(buf)
    sys.stdout.write(out)
    return 0


if __name__ == '__main__':
    raise SystemExit(main(sys.argv))
