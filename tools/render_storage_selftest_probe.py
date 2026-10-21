#!/usr/bin/env python3
"""Render Main-owned readout of pre-guest raw-SDRAM test pages20..23."""
import argparse
from pathlib import Path
from render_boot_fault_probe import TEMPLATE
START=TEMPLATE.index('def decode_sources(value):')
END=TEMPLATE.index('\nfinally:\n try:',START)
BODY='''def metadata(row):
 value=row['observation']
 assert value>>16==0xb157,('Actual raw-memory diagnostic marker absent',row)
 return dict(ready=bool(value&1),failed=bool(value&2),pending=bool(value&4),phase=(value>>3)&7)
try:
 select('[8:5]',0);select('[10:9]',0)
 select('[15:11]',20);before=metadata(read(0))
 select('[15:11]',21);address=read(1)
 select('[15:11]',22);failure=read(2)
 select('[15:11]',23);data=read(3)
 select('[15:11]',20);after=metadata(read(0))
 result.update(before=before,after=after,current_address=address['observation'],full_raw_cells=8388608,raw_cell_bits=12)
 assert not(after['ready'] and after['failed'])
 if before['failed'] and after['failed']:
  value=data['observation']
  result.update(first_failure_address=failure['observation'],first_failure_phase=value>>29,first_expected=(value>>17)&4095,first_actual=(value>>5)&4095)
 result['verified_zero_release_observed']=before['ready'] and after['ready']
'''
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__)
 p.add_argument('--pid',type=int,required=True)
 for name in ['deployment','remote-output','output']:p.add_argument('--'+name,type=Path,required=True)
 a=p.parse_args();assert a.pid>1 and a.deployment.is_absolute() and a.remote_output.is_absolute() and not a.output.exists()
 text=(TEMPLATE[:START]+BODY+TEMPLATE[END:]).replace('BOOT_FAULT_OBSERVATION ','STORAGE_SELFTEST_OBSERVATION ').replace('Main-owned passive boot-error pages; live page and retained event are separate observations','Main-owned raw-storage selftest pages; progress pages are separate observations')
 text=text.replace('@PID@',str(a.pid)).replace('@DEPLOYMENT@',repr(str(a.deployment))).replace('@OUTPUT@',repr(str(a.remote_output)))
 for block in text.split('\npython\n')[1:]:compile(block.split('\nend\n',1)[0],str(a.output),'exec')
 a.output.write_text(text)
