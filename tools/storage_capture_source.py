"""Strict source transforms for corrected SDRAM capture and historical replay.

The checked-in controllers use the physically qualified CL2 capture. The
inverse transform exists only to reconstruct the preserved late-capture
receipts; it is never a default simulation configuration.
"""

LEGACY = b'delay_count<=3;state<=READ_WAIT;'
CORRECT = b'delay_count<=2;state<=READ_WAIT;'
COMMENTS = [(b'fourth following falling edge', b'third following falling edge'),
            (b'fourth rising edge', b'third rising edge')]


def correct_capture(raw):
    assert raw.count(LEGACY) == 1 and raw.count(CORRECT) == 0
    raw = raw.replace(LEGACY, CORRECT)
    for old, new in COMMENTS:
        assert raw.count(old) == 1 and raw.count(new) == 0
        raw = raw.replace(old, new)
    return raw


def require_correct_capture(raw):
    assert raw.count(CORRECT) == 1 and raw.count(LEGACY) == 0, 'Default controller must use the physically qualified capture'
    for old, new in COMMENTS:
        assert raw.count(old) == 0 and raw.count(new) == 1
    return raw


def legacy_capture(raw):
    require_correct_capture(raw)
    raw = raw.replace(CORRECT, LEGACY)
    for old, new in COMMENTS:
        raw = raw.replace(new, old)
    return raw


def legacy_read_dqm_model(raw):
    # AS4C32M16SB section17/Figure31: two receiver-clock mask latency.
    # Launch precedes the receiver edge, so the corrected model uses the
    # previous input (pipe[0]). Reconstruct old model bytes only for receipts.
    old = b'read_dqm  = dqm_pipe[1];'
    new = b'read_dqm  = dqm_pipe[0];'
    assert raw.count(new) == 1 and raw.count(old) == 0
    return raw.replace(new, old)
