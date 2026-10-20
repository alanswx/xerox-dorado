// Pauses host service and guest serial state at nine independently chosen
// transfer/CRC positions. The HPS peer drains only real accepted requests.
#ifdef DISK_PAUSE_TEST
#define DISK_DEFINE_PAUSE \
 take_snapshot=[&](){ \
  top.snapshot_hold=1; \
  auto held_clock=[&](bool vary){ \
   top.clk_host=top.sys_clk=0;top.eval();disk.before(top); \
   if(vary){top.sd_buff_addr=uint16_t((host*317)&8191);top.sd_buff_dout=uint16_t(host^0xa53c);} \
   top.eval();top.clk_host=1;top.eval();disk.after(top);ctx.timeInc(1);host++; \
  }; \
  unsigned limit=50000; \
  do{held_clock(false);}while(!top.snapshot_idle&&--limit); \
  if(!limit||disk.active||top.sd_rd||top.sd_wr||top.sd_ack||top.sd_buff_wr) \
   throw std::runtime_error("disk pause failed to drain HPS"); \
  std::array<uint32_t,DISK_STATE_WORDS> state; \
  for(unsigned i=0;i<state.size();i++)state[i]=top.observed_state[i]; \
  const auto paused_image=disk.media;const unsigned reads=disk.reads,writes=disk.writes; \
  for(unsigned clock=0;clock<1000;clock++){ \
   held_clock(true); \
   for(unsigned i=0;i<state.size();i++)if(state[i]!=top.observed_state[i]) \
    throw std::runtime_error("held disk state changed"); \
   if(!top.snapshot_idle||disk.active||disk.reads!=reads||disk.writes!=writes) \
    throw std::runtime_error("held disk image or request changed"); \
  } \
  if(disk.media!=paused_image)throw std::runtime_error("held disk image changed"); \
  top.snapshot_hold=0;limit=8; \
  do{held_clock(false);}while(top.snapshot_held&&--limit); \
  if(!limit)throw std::runtime_error("disk parser latch failed to release"); \
 };
#else
#define DISK_DEFINE_PAUSE
#endif
