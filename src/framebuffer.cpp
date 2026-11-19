#include "nova/framebuffer.hpp"
#include "nova/renderer.hpp"
#if defined(__linux__)
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstdint>
#include <cstring>
#include <algorithm>
#endif
namespace nova {
FramebufferBackend::~FramebufferBackend(){close();}
void FramebufferBackend::close() noexcept{
#if defined(__linux__)
 if(mapping_&&mapping_!=MAP_FAILED){munmap(mapping_,mappingLength_);}
 if(fd_>=0)::close(fd_);
#endif
 fd_=-1;mapping_=nullptr;mappingLength_=0;width_=height_=0;lastError_.clear();
}
bool FramebufferBackend::isOpen()const noexcept{return fd_>=0&&mapping_!=nullptr;}
bool FramebufferBackend::openDevice(const std::string& path){
 close();
#if defined(__linux__)
 fd_=::open(path.c_str(),O_RDWR|O_CLOEXEC);
 if(fd_<0){lastError_="open failed for "+path;return false;}
 fb_fix_screeninfo fixed{};fb_var_screeninfo variable{};
 if(ioctl(fd_,FBIOGET_FSCREENINFO,&fixed)<0||ioctl(fd_,FBIOGET_VSCREENINFO,&variable)<0){lastError_="framebuffer ioctl failed";close();return false;}
 if(variable.xres==0||variable.yres==0||variable.bits_per_pixel%8!=0||variable.bits_per_pixel<16||variable.bits_per_pixel>32){lastError_="unsupported framebuffer format";close();return false;}
 mappingLength_=fixed.smem_len;
 if(mappingLength_==0){lastError_="framebuffer reports zero memory size";close();return false;}
 mapping_=mmap(nullptr,mappingLength_,PROT_READ|PROT_WRITE,MAP_SHARED,fd_,0);
 if(mapping_==MAP_FAILED){mapping_=nullptr;lastError_="framebuffer mmap failed";close();return false;}
 width_=static_cast<int>(variable.xres);height_=static_cast<int>(variable.yres);bitsPerPixel_=variable.bits_per_pixel;lineLength_=fixed.line_length;xOffset_=variable.xoffset;yOffset_=variable.yoffset;
 redOffset_=variable.red.offset;redLength_=variable.red.length;greenOffset_=variable.green.offset;greenLength_=variable.green.length;blueOffset_=variable.blue.offset;blueLength_=variable.blue.length;transpOffset_=variable.transp.offset;transpLength_=variable.transp.length;
 return true;
#else
 (void)path;lastError_="framebuffer backend is only supported on Linux";return false;
#endif
}
bool FramebufferBackend::present(const Canvas& canvas){
#if defined(__linux__)
 if(!isOpen()){lastError_="device is not open";return false;}
 const unsigned int bytesPerPixel=bitsPerPixel_/8;
 const int copyWidth=std::min(width_,canvas.width()),copyHeight=std::min(height_,canvas.height());
 const auto& source=canvas.pixels();
 auto pack=[](std::uint8_t channel,unsigned int offset,unsigned int length)->std::uint32_t{
  if(length==0)return 0;const std::uint64_t maxValue=(length>=32)?0xffffffffULL:((1ULL<<length)-1ULL);
  return static_cast<std::uint32_t>((static_cast<std::uint64_t>(channel)*maxValue/255ULL)<<offset);
 };
 auto* base=static_cast<std::uint8_t*>(mapping_);
 for(int y=0;y<copyHeight;++y){for(int x=0;x<copyWidth;++x){
  const Color c=source[static_cast<std::size_t>(y)*canvas.width()+x];
  const std::uint32_t value=pack(c.r,redOffset_,redLength_)|pack(c.g,greenOffset_,greenLength_)|pack(c.b,blueOffset_,blueLength_)|pack(255,transpOffset_,transpLength_);
  const std::size_t pos=static_cast<std::size_t>(y+yOffset_)*lineLength_+static_cast<std::size_t>(x+xOffset_)*bytesPerPixel;
  if(pos+bytesPerPixel>mappingLength_){lastError_="framebuffer bounds check failed";return false;}
  for(unsigned int byte=0;byte<bytesPerPixel;++byte)base[pos+byte]=static_cast<std::uint8_t>((value>>(byte*8))&0xff);
 }}
 lastError_.clear();return true;
#else
 (void)canvas;lastError_="framebuffer backend is only supported on Linux";return false;
#endif
}
}
