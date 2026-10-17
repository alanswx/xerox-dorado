// Interactive front end for the actual Sil-derived RTL machine.
// The screen is a read-only rendering of its DRAM/cache display list.
// All keyboard transitions go through dorado_keyboard -> dorado_terminal.
#include "verilated.h"
#include "Vtb_exec.h"
#include "Vtb_exec__Syms.h"
#include "rtl_memory_view.h"
#include "rtl_state.h"
#include <SDL.h>
#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <deque>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <set>
#include <string>
#include <vector>

static constexpr unsigned W=608, H=808;
static const uint8_t letters[26]={0x1c,0x32,0x21,0x23,0x24,0x2b,0x34,0x33,0x43,0x3b,0x42,0x4b,0x3a,0x31,0x44,0x4d,0x15,0x2d,0x1b,0x2c,0x3c,0x2a,0x1d,0x22,0x35,0x1a};
static const uint8_t digits[10]={0x45,0x16,0x1e,0x26,0x25,0x2e,0x36,0x3d,0x3e,0x46};
static unsigned ps2(SDL_Scancode k){
 if(k>=SDL_SCANCODE_A&&k<=SDL_SCANCODE_Z)return letters[k-SDL_SCANCODE_A];
 if(k>=SDL_SCANCODE_1&&k<=SDL_SCANCODE_9)return digits[k-SDL_SCANCODE_1+1];
 switch(k){
 case SDL_SCANCODE_0:return digits[0]; case SDL_SCANCODE_RETURN:return 0x5a;
 case SDL_SCANCODE_ESCAPE:return 0x76;case SDL_SCANCODE_BACKSPACE:return 0x66;
 case SDL_SCANCODE_TAB:return 0x0d;case SDL_SCANCODE_SPACE:return 0x29;
 case SDL_SCANCODE_MINUS:return 0x4e;case SDL_SCANCODE_EQUALS:return 0x55;
 case SDL_SCANCODE_LEFTBRACKET:return 0x54;case SDL_SCANCODE_RIGHTBRACKET:return 0x5b;
 case SDL_SCANCODE_BACKSLASH:return 0x5d;case SDL_SCANCODE_SEMICOLON:return 0x4c;
 case SDL_SCANCODE_APOSTROPHE:return 0x52;case SDL_SCANCODE_GRAVE:return 0x0e;
 case SDL_SCANCODE_COMMA:return 0x41;case SDL_SCANCODE_PERIOD:return 0x49;
 case SDL_SCANCODE_SLASH:return 0x4a;case SDL_SCANCODE_LSHIFT:return 0x12;
 case SDL_SCANCODE_RSHIFT:return 0x59;case SDL_SCANCODE_LCTRL:return 0x14;
 case SDL_SCANCODE_RCTRL:return 0x14;case SDL_SCANCODE_CAPSLOCK:return 0x58;
 case SDL_SCANCODE_DELETE:return 0x171;case SDL_SCANCODE_F1:return 0x05;
 case SDL_SCANCODE_F2:return 0x06;case SDL_SCANCODE_F3:return 0x04;
 default:return 0;
 }
}
struct Key {unsigned code;bool down;};
static void text_keys(const std::string&text,std::deque<Key>&q){
 for(unsigned char c:text){
  bool shift=false;unsigned code=0;
  if(c>='a'&&c<='z')code=letters[c-'a'];
  else if(c>='A'&&c<='Z'){code=letters[c-'A'];shift=true;}
  else if(c>='0'&&c<='9')code=digits[c-'0'];
  else switch(c){
   case '\r':case '\n':code=0x5a;break;case ' ':code=0x29;break;case '\t':code=0x0d;break;
   case '.':code=0x49;break;case ',':code=0x41;break;case '-':code=0x4e;break;
   case '/':code=0x4a;break;case '?':code=0x4a;shift=true;break;
   case ':':code=0x4c;shift=true;break;case ';':code=0x4c;break;
   case '=':code=0x55;break;case '+':code=0x55;shift=true;break;
   default:throw std::runtime_error("unsupported scripted character");
  }
  if(shift)q.push_back({0x12,true});q.push_back({code,true});q.push_back({code,false});if(shift)q.push_back({0x12,false});
 }
}
static std::array<uint32_t,W*H> render(Vtb_exec__Syms*s){
 std::array<uint32_t,W*H>fb;fb.fill(0xffffffff);unsigned dl=rtl_visible_word(s,0420),y=0;std::array<bool,65536>seen{};
 for(unsigned n=0;n<128&&dl>1&&dl<65532&&y<H&&!seen[dl];n++){
  seen[dl]=true;unsigned c=rtl_visible_word(s,dl+1),sa=rtl_visible_word(s,dl+2),slc=rtl_visible_word(s,dl+3);
  unsigned nw=c&255,ht=(c>>8)&63,lines=2*slc;bool inv=c&0x4000;
  if(nw>64||!lines||lines>H)break;
  for(unsigned row=0;row<lines&&y<H;row++,y++)for(unsigned j=0;j<nw;j++){
   unsigned bits=rtl_visible_word(s,(sa+row*nw+j)&65535);
   // Alto video convention: one is black ink, zero is white background.
   for(unsigned b=0;b<16;b++){unsigned x=(ht+j)*16+b;if(x<W)fb[y*W+x]=(((bits>>(15-b))&1)^inv)?0xff000000:0xffffffff;}
  }
  dl=rtl_visible_word(s,dl);
 }
 return fb;
}
static void pgm(const std::string&path,Vtb_exec__Syms*s){
 auto fb=render(s);std::ofstream f(path,std::ios::binary);f.exceptions(std::ios::failbit|std::ios::badbit);
 unsigned ink=0;
 f<<"P5\n"<<W<<" "<<H<<"\n255\n";for(auto x:fb){uint8_t v=x&255;ink+=v==0;f.write(reinterpret_cast<char*>(&v),1);}
 std::printf("BITMAP @%llu ink=%u DCB=%04x %s\n",(unsigned long long)s->TOP.tb_exec__DOT__n_cyc2,ink,rtl_visible_word(s,0420),path.c_str());
}
static void media_write(std::ostream&f,Vtb_exec___024root*r){
 auto&m=r->tb_exec__DOT__u_pack__DOT__written_words;uint64_t n=m.size();uint16_t d=m.atDefault();f.write(reinterpret_cast<char*>(&n),8);f.write(reinterpret_cast<char*>(&d),2);
 QData k;for(int ok=m.first(k);ok;ok=m.next(k)){uint64_t key=k;uint16_t val=m.at(k);f.write(reinterpret_cast<char*>(&key),8);f.write(reinterpret_cast<char*>(&val),2);}
}
static void media_read(std::istream&f,Vtb_exec___024root*r){
 uint64_t n=0;uint16_t d=0;f.read(reinterpret_cast<char*>(&n),8);f.read(reinterpret_cast<char*>(&d),2);
 if(n>32000000)throw std::runtime_error("invalid snapshot media length");
 std::vector<std::pair<uint64_t,uint16_t>>pairs;for(uint64_t i=0;i<n;i++){uint64_t k;uint16_t v;f.read(reinterpret_cast<char*>(&k),8);f.read(reinterpret_cast<char*>(&v),2);pairs.emplace_back(k,v);}
 auto&m=r->tb_exec__DOT__u_pack__DOT__written_words;m.clear();m.atDefault()=d;for(auto[k,v]:pairs)m.at(k)=v;
}
static void save_state(const std::string&path,Vtb_exec__Syms*s){
 auto temporary=path+".tmp";
 std::ofstream f(temporary,std::ios::binary);f.exceptions(std::ios::failbit|std::ios::badbit);f.write("DORRTL1\0",8);f.write(rtl_state_fingerprint,64);auto*base=reinterpret_cast<char*>(s);
 for(auto&r:rtl_state_ranges)f.write(base+r[0],r[1]);media_write(f,&s->TOP);
 f.close();if(std::rename(temporary.c_str(),path.c_str()))throw std::runtime_error("cannot replace snapshot "+path);
 std::printf("Saved RTL state %s at %llu\n",path.c_str(),(unsigned long long)s->TOP.tb_exec__DOT__n_cyc2);
}
static void load_state(const std::string&path,Vtb_exec__Syms*s,bool raw){
 std::ifstream f(path,std::ios::binary);f.exceptions(std::ios::failbit|std::ios::badbit);std::vector<char>bytes(rtl_state_size);
 if(raw){f.read(bytes.data(),bytes.size());std::ifstream mf(path+".media",std::ios::binary);if(mf){mf.exceptions(std::ios::failbit|std::ios::badbit);media_read(mf,&s->TOP);}}
 else{
  char magic[8],fingerprint[64];f.read(magic,8);f.read(fingerprint,64);
  if(std::memcmp(magic,"DORRTL1\0",8)||std::memcmp(fingerprint,rtl_state_fingerprint,64))throw std::runtime_error("snapshot belongs to a different RTL build");
  for(auto&r:rtl_state_ranges)f.read(bytes.data()+r[0],r[1]);media_read(f,&s->TOP);
 }
 auto*base=reinterpret_cast<char*>(s);for(auto&r:rtl_state_ranges)std::memcpy(base+r[0],bytes.data()+r[0],r[1]);
 // Host run controls are independent of restored guest execution.
 s->TOP.tb_exec__DOT__runcycles=0x3fffffffffffffffULL;s->TOP.tb_exec__DOT__has_cycle_limit_file=0;s->TOP.tb_exec__DOT__bootio=0;
 std::printf("Restored RTL state %s at %llu\n",path.c_str(),(unsigned long long)s->TOP.tb_exec__DOT__n_cyc2);
}
int main(int argc,char**argv){try{
 bool headless=false,raw=false;std::string restore,save,out,dump,text,capture;uint64_t stop=0,saveat=0,saveevery=0,typeat=0,gap=4000000,every=2000000;
 std::vector<std::string>args{argv[0]};
 for(int i=1;i<argc;i++){
  std::string a=argv[i];auto val=[&](){if(++i>=argc)throw std::runtime_error("missing value for "+a);return std::string(argv[i]);};
  if(a=="--help"){
   std::puts("Dorado RTL simulator (run from the repository root)\n"
    "  --restore FILE       Resume an exact-build RTL snapshot\n"
    "  --headless           Run without an SDL window\n"
    "  --stop-at SAMPLES    Stop at this absolute fabric sample\n"
    "  --save-state FILE    Save RTL state, including disk writes\n"
    "  --save-at SAMPLES    Save at this sample instead of exit\n"
    "  --save-every N       Replace the checkpoint every N samples and at exit\n"
    "  --out FILE           Write the guest DCB bitmap as PGM at exit\n"
    "  --capture-prefix P   Write periodic PGM images as P-SAMPLE.pgm\n"
    "  --capture-every N    Fabric samples between images (default 2000000)\n"
    "  --dump-memory FILE   Write 64K visible guest words at exit\n"
    "  --type TEXT          Queue text through the PS/2 keyboard\n"
    "  --type-at SAMPLES    Start queued text at this sample\n"
    "  --key-gap SAMPLES    Space key transitions (default 4000000)\n"
    "  +NAME[=VALUE]        Pass a testbench option to Verilator\n"
    "  --import-raw FILE    Diagnostic import of exact-layout POD state\n"
    "The display is rendered read-only from actual RTL memory.\n"
    "Simulation currently uses an assisted Initial/AEmu boot.");return 0;
  }
  if(a=="--headless")headless=true;else if(a=="--restore")restore=val();else if(a=="--import-raw"){restore=val();raw=true;}
  else if(a=="--save-state")save=val();else if(a=="--save-at")saveat=std::stoull(val());else if(a=="--stop-at")stop=std::stoull(val());
  else if(a=="--save-every"){saveevery=std::stoull(val());if(!saveevery)throw std::runtime_error("--save-every must be positive");}
  else if(a=="--out")out=val();else if(a=="--dump-memory")dump=val();else if(a=="--type")text=val();else if(a=="--type-at")typeat=std::stoull(val());else if(a=="--key-gap")gap=std::stoull(val());
  else if(a=="--capture-prefix")capture=val();else if(a=="--capture-every")every=std::stoull(val());
  else if(a.starts_with("+"))args.push_back(a);else throw std::runtime_error("unknown option "+a);
 }
 if(!gap)throw std::runtime_error("--key-gap must be positive");
 if(!every)throw std::runtime_error("--capture-every must be positive");
 if(saveat&&save.empty())throw std::runtime_error("--save-at requires --save-state");
 if(saveevery&&save.empty())throw std::runtime_error("--save-every requires --save-state");
 if(saveevery&&saveat)throw std::runtime_error("choose --save-at or --save-every");
 auto add=[&](const char*arg){std::string key=arg;auto pos=key.find('=');if(pos!=std::string::npos)key.resize(pos+1);for(auto&a:args)if(a.starts_with(key))return;args.emplace_back(arg);};
 for(auto a:{"+preload=verilog/verilator/obj_bwdisk/im.txt","+worldms","+bootchain","+start=3904","+enterat=c92","+enterworld=23e","+frame","+mb0","+nostuck","+terminal","+pack=chm/diskpacks/games-trident.pack","+cycles=2000000000","+pendulum","+hasram","+ioreset","+fastwait","+bbword=0000","+hold-limit=100000"})add(a);
 if(restore.empty())add("+bbfeed");
 std::vector<const char*>av;for(auto&a:args)av.push_back(a.c_str());VerilatedContext ctx;ctx.threads(1);ctx.commandArgs(av.size(),av.data());Vtb_exec top(&ctx);auto*r=top.rootp;auto*s=r->vlSymsp;
 SDL_Window*window=nullptr;SDL_Renderer*renderer=nullptr;SDL_Texture*texture=nullptr;
 if(!headless){if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS))throw std::runtime_error(SDL_GetError());window=SDL_CreateWindow("Dorado RTL",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,W,H,SDL_WINDOW_RESIZABLE);if(!window)throw std::runtime_error(SDL_GetError());renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);if(!renderer)renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_SOFTWARE);if(!renderer)throw std::runtime_error(SDL_GetError());SDL_RenderSetLogicalSize(renderer,W,H);texture=SDL_CreateTexture(renderer,SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_STREAMING,W,H);if(!texture)throw std::runtime_error(SDL_GetError());}
 bool restored=restore.empty(),saved=false,typed=false,quit=false;std::deque<Key>keys;std::set<unsigned>held;uint64_t nextkey=0,nextcapture=every,nextsave=saveevery;unsigned toggle=0;
 auto stamp=SDL_GetTicks64();
 while(!ctx.gotFinish()&&!quit){
  top.eval();uint64_t n=r->tb_exec__DOT__n_cyc2;
  if(!restored&&n==5000&&r->tb_exec__DOT__sys_clk){load_state(restore,s,raw);restored=true;n=r->tb_exec__DOT__n_cyc2;toggle=r->tb_exec__DOT__terminal_ps2_key&0x400;}
  if(restored&&r->tb_exec__DOT__sys_clk){
   if(!capture.empty()&&n>=nextcapture){pgm(capture+"-"+std::to_string(n)+".pgm",s);nextcapture=(n/every+1)*every;}
   if(!typed&&!text.empty()&&n>=typeat){text_keys(text,keys);typed=true;}
   if(!keys.empty()&&n>=nextkey){auto k=keys.front();keys.pop_front();toggle^=0x400;r->tb_exec__DOT__terminal_ps2_key=toggle|(k.down?0x200:0)|k.code;nextkey=n+gap;if(k.down)held.insert(k.code);else held.erase(k.code);std::printf("KEY @%llu code=%03x %s\n",(unsigned long long)n,k.code,k.down?"down":"up");}
   if(!save.empty()&&!saved&&saveat&&n>=saveat){save_state(save,s);saved=true;}
   if(saveevery&&n>=nextsave){save_state(save,s);nextsave=(n/saveevery+1)*saveevery;}
   if(stop&&n>=stop)break;
  }
  if(!headless&&SDL_GetTicks64()-stamp>=50){
   stamp=SDL_GetTicks64();SDL_Event e;while(SDL_PollEvent(&e)){
    if(e.type==SDL_QUIT){quit=true;break;}
    if((e.type==SDL_KEYDOWN||e.type==SDL_KEYUP)&&!e.key.repeat){auto c=ps2(e.key.keysym.scancode);if(c)keys.push_back({c,e.type==SDL_KEYDOWN});}
    if(e.type==SDL_WINDOWEVENT&&e.window.event==SDL_WINDOWEVENT_FOCUS_LOST){keys.clear();for(auto c:held)keys.push_back({c,false});}
   }
   if(restored){auto fb=render(s);SDL_UpdateTexture(texture,nullptr,fb.data(),W*4);SDL_RenderClear(renderer);SDL_RenderCopy(renderer,texture,nullptr,nullptr);SDL_RenderPresent(renderer);}
  }
  if(!top.eventsPending())break;ctx.time(top.nextTimeSlot());
 }
 if(!restored)throw std::runtime_error("RTL initialization did not reach snapshot restore point");
 if(!out.empty())pgm(out,s);
 if(!dump.empty()){std::ofstream f(dump,std::ios::binary);f.exceptions(std::ios::failbit|std::ios::badbit);for(unsigned a=0;a<65536;a++){uint16_t v=rtl_visible_word(s,a);f.write(reinterpret_cast<char*>(&v),2);}}
 if(!save.empty()&&!saved)save_state(save,s);
 if(texture)SDL_DestroyTexture(texture);if(renderer)SDL_DestroyRenderer(renderer);if(window)SDL_DestroyWindow(window);if(!headless)SDL_Quit();top.final();return 0;
 }catch(const std::exception&e){std::fprintf(stderr,"dorado-executive: %s\n",e.what());return 1;}}
