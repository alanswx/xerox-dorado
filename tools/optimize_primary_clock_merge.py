#!/usr/bin/env python3
"""Prepare a parallel insertion merger without changing source clocks or state.

The four CPU/pixel records are already strictly ordered. Coalesce each added
crystal against those records, insert only distinct new timestamps, and route
records to fixed slots by their insertion ranks. Constant quantum scaling is
a twelve-entry truth table rather than inferred multipliers. This preparation
alone proves nothing; run the complete absolute-time and formal gates next.
"""
import argparse
import json
from pathlib import Path

from test_r_front_forward_view import sha

BODY = ''' reg [17:0] candidate_time[0:5];
 reg [5:0] candidate_mask[0:5];
 reg present[0:5];
 reg [2:0] rank[0:5];
 reg bb_matches,ether_matches,bb_insert,ether_insert;
 reg [5:0] joined,cursor;
 reg [17:0] timestamp;
 function automatic [17:0] scale_quantum(input [3:0] quantum);
  case(quantum)
SCALE_CASES
   default:scale_quantum=18'd0;
  endcase
 endfunction
 always @*begin
  event_count=0;offsets=0;masks=0;levels=0;
  cursor=levels_before;joined=0;timestamp=0;
  bb_matches=0;ether_matches=0;bb_insert=0;ether_insert=0;
  for(integer i=0;i<6;i++)begin
   candidate_time[i]=UNUSED;candidate_mask[i]=0;present[i]=0;rank[i]=0;
  end
  if(advance && !reset)begin
   for(integer i=0;i<4;i++)if(i<int'(inner_count))begin
    candidate_time[i]=scale_quantum(inner_offsets[4*i+:4]);
    candidate_mask[i]={2'b0,inner_masks[4*i+:4]};present[i]=1;
   end
   if(bb_due)begin candidate_time[4]=18'(bb_remaining);candidate_mask[4]=6'b010000;end
   if(ether_due)begin candidate_time[5]=ether_remaining;candidate_mask[5]=6'b100000;end
   for(integer i=0;i<4;i++)if(present[i])begin
    if(bb_due && candidate_time[i]==candidate_time[4])begin
     bb_matches=1;candidate_mask[i]=candidate_mask[i]|candidate_mask[4];
    end
    if(ether_due && candidate_time[i]==candidate_time[5])begin
     ether_matches=1;candidate_mask[i]=candidate_mask[i]|candidate_mask[5];
    end
   end
   bb_insert=bb_due && !bb_matches;
   ether_insert=ether_due && !ether_matches && !(bb_due && candidate_time[4]==candidate_time[5]);
   if(bb_insert && ether_due && candidate_time[4]==candidate_time[5])
    candidate_mask[4]=candidate_mask[4]|candidate_mask[5];
   present[4]=bb_insert;present[5]=ether_insert;
   for(integer i=0;i<4;i++)begin
    rank[i]=3'(i)+3'(bb_insert && candidate_time[4]<candidate_time[i])+
                       3'(ether_insert && candidate_time[5]<candidate_time[i]);
    if(present[i] && candidate_time[i]<candidate_time[4])rank[4]=rank[4]+3'd1;
    if(present[i] && candidate_time[i]<candidate_time[5])rank[5]=rank[5]+3'd1;
   end
   rank[4]=rank[4]+3'(ether_insert && candidate_time[5]<candidate_time[4]);
   rank[5]=rank[5]+3'(bb_insert && candidate_time[4]<candidate_time[5]);
   event_count=inner_count+3'(bb_insert)+3'(ether_insert);
   for(integer record=0;record<6;record++)begin
    joined=0;timestamp=0;
    for(integer i=0;i<6;i++)if(present[i] && rank[i]==3'(record))begin
     joined=joined|candidate_mask[i];timestamp=timestamp|candidate_time[i];
    end
    cursor=cursor^joined;
    if(record<int'(event_count))begin
     offsets[18*record+:18]=timestamp;
     masks[6*record+:6]=joined;
     levels[6*record+:6]=cursor;
    end
   end
  end
  levels_after=cursor;
 end
'''


def prepare(source, out):
    assert not out.exists()
    raw = source.read_text()
    start = raw.index(' reg [17:0] candidate_time')
    end = raw.index(' always @(posedge clk)begin', start)
    assert 'first=UNUSED;' in raw[start:end]
    cases = '\n'.join(f"   4'd{i}:scale_quantum=18'd{i*11765};" for i in range(1, 13))
    body = BODY.replace('SCALE_CASES', cases)
    candidate = raw[:start]+body+raw[end:]
    assert candidate[:start]+raw[start:end]+candidate[start+len(body):] == raw
    out.mkdir(parents=True)
    (out/'original.sv').write_text(raw)
    (out/'dorado_primary_clock_events.sv').write_text(candidate)
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    result = dict(status='PREPARED_PARALLEL_PRIMARY_CLOCK_INSERTION', source=str(source),
                  original_sha256=sha(source), state_and_source_clocks_byte_unchanged=True,
                  extra_saved_state_or_virtual_cycles=0, source_domain_changed=False,
                  formal_qualified=False, simulator_qualified=False,
                  timing_qualified=False, native_consumers_integrated=False,
                  files={p.name: sha(p) for p in out.iterdir() if p.is_file()})
    (out/'results.json').write_text(json.dumps(result, indent=2)+'\n')
    print(result['status'], flush=True)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ['source', 'output']:
        parser.add_argument('--'+name, type=Path, required=True)
    args = parser.parse_args()
    prepare(args.source.resolve(), args.output.resolve())
