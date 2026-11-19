#pragma once
#include <string>
namespace nova {
class Canvas;
class FramebufferBackend {
public:
 FramebufferBackend()=default;
 ~FramebufferBackend();
 FramebufferBackend(const FramebufferBackend&)=delete;
 FramebufferBackend& operator=(const FramebufferBackend&)=delete;
 bool openDevice(const std::string& path);
 void close() noexcept;
 bool isOpen()const noexcept;
 bool present(const Canvas& canvas);
 const std::string& lastError()const noexcept{return lastError_;}
 int width()const noexcept{return width_;} int height()const noexcept{return height_;}
private:
 int fd_=-1; void* mapping_=nullptr; unsigned long mappingLength_=0;
 int width_=0,height_=0; unsigned int bitsPerPixel_=0;
 unsigned int lineLength_=0; unsigned int xOffset_=0,yOffset_=0;
 unsigned int redOffset_=0,redLength_=0,greenOffset_=0,greenLength_=0,blueOffset_=0,blueLength_=0,transpOffset_=0,transpLength_=0;
 std::string lastError_;
};
}
