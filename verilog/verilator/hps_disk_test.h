// Testbench implementation of MiSTer's 16-bit virtual-disk buffer protocol.
// The production core uses hps_io and the mounted disk file instead.
#pragma once
#include <cstdint>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <vector>
struct HpsDiskTest {
 std::vector<uint8_t> media;
 bool active=false, closing=false, writing=false;
 unsigned index=0, count=0;
 uint64_t offset=0;
 unsigned reads=0, writes=0;
 explicit HpsDiskTest(const char*path){
  std::ifstream f(path,std::ios::binary);if(!f)throw std::runtime_error("cannot open test disk");
  media.assign(std::istreambuf_iterator<char>(f),{});
 }
 template<class T> void before(T&t){
  t.sd_buff_wr=0;t.sd_ack=0;
  if(closing){closing=false;active=false;return;}
  if(!active){
   if(!t.sd_rd&&!t.sd_wr)return;
   if(t.sd_rd&&t.sd_wr)throw std::runtime_error("simultaneous HPS read/write");
   active=true;writing=t.sd_wr;index=0;count=(t.sd_blk_cnt+1)*256;
   offset=uint64_t(t.sd_lba)*512;
   if(offset+count*2>media.size())throw std::runtime_error("HPS request outside image");
   if(writing)writes++;else reads++;
  }
  t.sd_ack=1;t.sd_buff_addr=index;
  if(!writing){t.sd_buff_wr=1;t.sd_buff_dout=media[offset+2*index]|(media[offset+2*index+1]<<8);}
 }
 template<class T> void after(T&t){
  if(!active||closing)return;
  if(writing){media[offset+2*index]=t.sd_buff_din;media[offset+2*index+1]=t.sd_buff_din>>8;}
  if(++index==count)closing=true;
 }
};
