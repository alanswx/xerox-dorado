#!/usr/bin/env python3
"""Decode constant burst lanes before captured SDRAM data reaches registers.

Replace variable-position stores with constant slices and control-only enables.
Every condition, data value and capture edge is unchanged; no pipeline delay,
CAS/refresh/mask change or speculative response is introduced.
"""

def decode_capture(raw):
 s=raw.decode()
 old='    reg [2:0] command=NOP;';assert s.count(old)==1
 s=s.replace(old,'''    // Keep the actual RAS/CAS/WE bits in their output registers. Quartus
    // Standard Design Compilation guide3.4.11.1: syn_encoding="user"
    // retains the HDL values instead of adding a one-hot output decoder.
    (* syn_encoding = "user" *) reg [2:0] command=NOP;''')
 old='line_words[fill_line][16*fill_beat+:16]<=dq_capture;'
 assert s.count(old)==1,'Expected original variable-position BL8 store'
 s=s.replace(old,'''// Fixed slices let fill_line/fill_beat drive enables; SDRAM
            // data does not pass through a variable-position write mux.
            for(integer capture_line=0;capture_line<CACHE_LINES;capture_line=capture_line+1)
                for(integer capture_beat=0;capture_beat<8;capture_beat=capture_beat+1)
                    if(fill_line==capture_line && fill_beat==capture_beat)
                        line_words[capture_line][16*capture_beat+:16]<=dq_capture;''')
 old='first_burst_words[12*fill_beat+:12]<=dq_capture[11:0];'
 if 'first_burst_words'in s:
  assert s.count(old)==1,'Expected exact passive trace capture'
  s=s.replace(old,'''for(integer trace_beat=0;trace_beat<8;trace_beat=trace_beat+1)
                if(fill_beat==trace_beat)
                    first_burst_words[12*trace_beat+:12]<=dq_capture[11:0];''')
 return s.encode()

def allow_66mhz(raw):
 # AS4C32M16SB Rev1.4, p22 Table16: CL2 tCK>=10ns,
 # tAC<=6ns. This candidate is limited to the tested15ns host period.
 old=b'CLOCK_HZ>50000000';assert raw.count(old)==1
 return raw.replace(old,b'CLOCK_HZ>66666667')
