#!/usr/bin/env python3
"""Factor R sign/odd outputs into one read-window source mux.

The original three MC1668 outputs and all their state updates remain. During
an open read window exactly the selected RAM, Md or Pd bit contributes;
closed windows retain the AND of the three stored Qs. Binary equivalence
must be checked against the original capture and set-priority expressions.
"""
import hashlib

PARENT_SHA='d4809afa1d091ce88e5456db7980dc2616a42368cae829abccb0840de1fac719'
OLD=''' assign rm_flags_n={!(state_next[595] & state_next[594] & child_next[590]),
                    !(state_next[593] & state_next[592] & child_next[588])};'''
NEW=''' // Preserve all three physical latch Qs; factor only their output view.
 wire high_flag_bit=pins_after[148] ? (new_md[0] ? pins_after[92] : pins_after[76]) : pins_after[127];
 wire low_flag_bit=pins_after[150] ? (new_md[1] ? pins_after[107] : pins_after[91]) : pins_after[142];
 assign rm_flags_n={r_read_n[1] ? !(state_q[595]&state_q[594]&state_q[590]) : !low_flag_bit,
                    r_read_n[0] ? !(state_q[593]&state_q[592]&state_q[588]) : !high_flag_bit};'''


def rewrite(path):
    raw=path.read_text();assert hashlib.sha256(raw.encode()).hexdigest()==PARENT_SHA
    assert raw.count(OLD)==1;changed=raw.replace(OLD,NEW)
    assert changed.replace(NEW,OLD)==raw
    path.write_text(changed)
    return dict(parent_sha256=PARENT_SHA,output_sha256=hashlib.sha256(changed.encode()).hexdigest(),
                old=OLD,new=NEW,physical_latch_updates_unchanged=True,
                extra_architectural_state=False,native_integrated=False,scope=__doc__)
