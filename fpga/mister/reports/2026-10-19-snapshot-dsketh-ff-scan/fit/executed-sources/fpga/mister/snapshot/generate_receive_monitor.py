#!/usr/bin/env python3
"""Pinned34bit passive receive-monitor state for full DskEth snapshots.

No clock or packet behavior change. Local access requires all consumers held;
complete qualification is performed with the full actual DskEth board.
"""
import hashlib,re
from pathlib import Path
import generate_snapshot_ff as ff
ROOT=Path(__file__).resolve().parents[3]
SOURCE='fpga/mister/dorado_receive_monitor.sv'
SHA='6f3e1b083016c1a5ea6cc1f33ac99ec1194a48a6b87714504d547c61b1620b46'
FIELDS=[('received',8),('consumed',8),('wire_status',8),('cpu_status',8),('input_d',1),('write_d',1)]
PROCESS='''always @(posedge clk) begin
        write_d<=fifo_write_n;
        input_d<=input_n;
        if(write_d && !fifo_write_n && fifo_eop) begin
            received<=received+1'b1;
            wire_status<=fifo_word[7:0];
        end
        if(input_d && !input_n && tioa==8'o15 && input_eop) begin
            consumed<=consumed+1'b1;
            cpu_status<=input_word[7:0];
        end
    end'''


def generate(original=None):
 s=(ROOT/SOURCE).read_text()if original is None else original
 if hashlib.sha256(s.encode()).hexdigest()!=SHA:raise ValueError('unreviewed receive monitor')
 text,process=ff.extract_once(s,PROCESS)
 if re.search(r'\b(always\w*|initial|final)\b',ff.masked_comments(text)):raise ValueError('extra monitor process')
 normal=re.sub(r'^always\s*@\s*\(\s*posedge\s+clk\s*\)\s*','',process,count=1)
 assert text.count('module dorado_receive_monitor (')==1 and text.count('    input wire clk,')==1
 text=text.replace('module dorado_receive_monitor (','module dorado_receive_monitor_snapshot (',1)
 text=text.replace('    input wire clk,','''    input wire state_host_clk,state_advance,state_freeze,state_guest_held,state_request,state_write,
    input wire [1:0] state_index,input wire [17:0] state_data,
    output reg state_frozen=1'b0,state_done=1'b0,state_error=1'b0,
    output reg [17:0] state_result,
    input wire clk,''',1)
 text=text.replace('endmodule','''
 wire state_hold=state_freeze && state_guest_held;
 wire state_accept=state_hold && state_frozen && state_request;
 wire state_valid=!state_index[1];
 always @(posedge state_host_clk)begin
  state_frozen<=state_hold;state_done<=state_accept;state_error<=state_accept && !state_valid;
  if(state_accept)begin
   state_result<=18'b0;
   if(state_valid)begin
    if(state_write)begin
     if(!state_index[0]){wire_status[1:0],consumed,received}<=state_data;
     else {write_d,input_d,cpu_status,wire_status[7:2]}<=state_data[15:0];
    end else if(!state_index[0])state_result<={wire_status[1:0],consumed,received};
    else state_result<={2'b0,write_d,input_d,cpu_status,wire_status[7:2]};
   end
  end
  if(!state_hold && state_advance)begin
'''+normal+'''
  end
 end
endmodule''',1)
 fields=[];lsb=0
 for n,w in FIELDS:fields.append(dict(name=n,width=w,lsb=lsb));lsb+=w
 return text,dict(module='dorado_receive_monitor_snapshot',logical_state_bits=34,fields=fields,
  words=[dict(index=0,width=18),dict(index=1,width=16)],original_sha256=SHA,
  generated_sha256=hashlib.sha256(text.encode()).hexdigest(),scope=__doc__)
