#pragma once
#include <cstdint>
#include <string>
#include <vector>
namespace nova {
struct Color {
 std::uint8_t r,g,b;
 constexpr Color(std::uint8_t red=0,std::uint8_t green=0,std::uint8_t blue=0):r(red),g(green),b(blue){}
 constexpr bool operator==(const Color& other) const noexcept{return r==other.r&&g==other.g&&b==other.b;}
 constexpr bool operator!=(const Color& other) const noexcept{return !(*this==other);}
};
class Canvas {
public:
 Canvas(int width,int height);
 int width()const noexcept{return width_;} int height()const noexcept{return height_;}
 const std::vector<Color>& pixels()const noexcept{return pixels_;}
 void clear(Color); void pixel(int,int,Color); Color getPixel(int,int)const;
 void fillRect(int,int,int,int,Color); void drawText(int,int,const std::string&,Color,int scale=1);
private:int width_,height_;std::vector<Color> pixels_;
};
namespace colors {inline constexpr Color Black{0,0,0},White{255,255,255},Cyan{0,210,230},Blue{35,100,230},Gray{130,130,130},Red{230,60,60};}
}
