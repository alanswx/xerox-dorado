#!/usr/bin/env python3
"""Carry source-verified physical diagnostic inputs across native RTL staging.

Stage overlays replace board files; inheriting old provenance alone does not
preserve external jumper inputs. Reapply ONLY the documented SkipDiskWait' PB1
ground when the baseline actually contains that ground. Keep all other bytes.
"""
import hashlib

BASEBOARD='src/build/rtl-executive/native16-tx-cable/BaseBd_nominal.v'
DEFAULT=b"1'b0, TSetRun, SkipWait_p___drv, MCPStrb})"
FITTED=b"1'b0, TSetRun, 1'b0, MCPStrb})"


def digest(raw):return hashlib.sha256(raw).hexdigest()


def preserve(staged,baseline,metadata):
    if not metadata.get('skip_disk_wait_jumper_fitted',False):return staged,None
    assert baseline.count(FITTED)==1 and DEFAULT not in baseline,'Baseline jumper claim disagrees with actual PB1 input'
    assert staged.count(DEFAULT)+staged.count(FITTED)==1,'Cannot identify the staged original control-RIOT PB1 input'
    actual=staged.replace(DEFAULT,FITTED,1)
    assert actual.count(FITTED)==1 and DEFAULT not in actual
    if DEFAULT in staged:assert actual.replace(FITTED,DEFAULT,1)==staged
    else:assert actual==staged
    return actual,dict(path=BASEBOARD,input='control RIOT PB1 / SkipDiskWait active low',level=0,
        baseline_sha256=digest(baseline),staged_sha256=digest(staged),output_sha256=digest(actual),
        changed=actual!=staged,only_external_PB1_input_changed=True,original_ROM_and_clock_bytes_unchanged=True)
