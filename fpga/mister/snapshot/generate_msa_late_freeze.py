#!/usr/bin/env python3
"""Separate MSA board/FF hold from its sixteen raw-memory frontend holds.

Generate from the qualified complete chip-width MSA. No physical board or
frontend state, normal equation, pin or serial layout changes. A coordinator
must stop guest advances, let raw work drain, then assert frontend hold.
"""
import copy
import hashlib
import generate_msa_chipwidth as parent


def transform(board):
    board=parent.base.once(board,
        'input wire state_global_hold,state_global_abort,state_system_reset,',
        'input wire state_frontend_hold,state_global_hold,state_global_abort,state_system_reset,')
    anchor='  reg state_boundary_ready=1\'b0;'
    board=parent.base.once(board,anchor,
        '  wire frontend_hold=state_frontend_hold || state_scan_hold_required;\n'+anchor)
    before='.state_guest_advance(state_guest_advance),.state_hold(state_hold),'
    assert board.count(before)==16
    return board.replace(before,'.state_guest_advance(state_guest_advance),.state_hold(frontend_hold),')


def inverse(board):
    before='.state_guest_advance(state_guest_advance),.state_hold(frontend_hold),'
    assert board.count(before)==16
    board=board.replace(before,'.state_guest_advance(state_guest_advance),.state_hold(state_hold),')
    board=parent.base.once(board,
        '  wire frontend_hold=state_frontend_hold || state_scan_hold_required;\n','')
    return parent.base.once(board,
        'input wire state_frontend_hold,state_global_hold,state_global_abort,state_system_reset,',
        'input wire state_global_hold,state_global_abort,state_system_reset,')


def generate():
    old,manifest,helpers=parent.generate();board=transform(old);assert inverse(board)==old
    manifest=copy.deepcopy(manifest)
    manifest.update(scope=__doc__,generated_sha256=hashlib.sha256(board.encode()).hexdigest(),
        late_frontend_hold=True,new_board_state_bits=0,
        limits='Component only; real raw service drain and full native integration remain pending.')
    return board,manifest,helpers
