#!/usr/bin/env python3
"""Read local FPGA device ACKs separately from board-only host permissions.

An ACK can authorize the next coordinator step, never whole-machine admission.
The ordinary ABI2 formatter continues to validate every original status field.
"""
LOCAL_STATUS_FLAG=1<<11
LOCAL_STATUS_VERSION=0xac01


def parse_service_status(client,words):
    if len(words)!=21 or any(type(w)is not int or not 0<=w<=65535 for w in words):
        raise ValueError('invalid local-service status framing')
    if not words[2]&LOCAL_STATUS_FLAG or words[20]!=LOCAL_STATUS_VERSION:
        raise ValueError('local-service status extension unavailable')
    available,local=words[19],words[18]
    if not available or available>>5 or local&~available:
        raise ValueError('unsupported local-service mask or ACK bits')
    ordinary=list(words[:18]);ordinary[2]&=~LOCAL_STATUS_FLAG
    result=client.parse_status(ordinary)
    result.update(local_services_held=local,local_services_available=available,
                  local_status_version=LOCAL_STATUS_VERSION)
    return result
