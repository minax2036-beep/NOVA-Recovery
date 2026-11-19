#include "nova/renderer.hpp"
#include <algorithm>
#include <array>
#include <stdexcept>
namespace nova { namespace {
using Glyph=std::array<std::uint8_t,7>;
Glyph glyph(char input){char c=(input>='a'&&input<='z')?static_cast<char>(input-'a'+'A'):input;
switch(c){
case 'A':return{14,17,17,31,17,17,17}; case 'B':return{30,17,17,30,17,17,30}; case 'C':return{14,17,16,16,16,17,14}; case 'D':return{30,17,17,17,17,17,30}; case 'E':return{31,16,16,30,16,16,31}; case 'F':return{31,16,16,30,16,16,16}; case 'G':return{14,17,16,23,17,17,15}; case 'H':return{17,17,17,31,17,17,17}; case 'I':return{31,4,4,4,4,4,31}; case 'J':return{7,2,2,2,18,18,12}; case 'K':return{17,18,20,24,20,18,17}; case 'L':return{16,16,16,16,16,16,31}; case 'M':return{17,27,21,21,17,17,17}; case 'N':return{17,25,21,19,17,17,17}; case 'O':return{14,17,17,17,17,17,14}; case 'P':return{30,17,17,30,16,16,16}; case 'Q':return{14,17,17,17,21,18,13}; case 'R':return{30,17,17,30,20,18,17}; case 'S':return{15,16,16,14,1,1,30}; case 'T':return{31,4,4,4,4,4,4}; case 'U':return{17,17,17,17,17,17,14}; case 'V':return{17,17,17,17,17,10,4}; case 'W':return{17,17,17,21,21,21,10}; case 'X':return{17,17,10,4,10,17,17}; case 'Y':return{17,17,10,4,4,4,4}; case 'Z':return{31,1,2,4,8,16,31};
case '0':return{14,17,19,21,25,17,14}; case '1':return{4,12,4,4,4,4,14}; case '2':return{14,17,1,2,4,8,31}; case '3':return{30,1,1,14,1,1,30}; case '4':return{2,6,10,18,31,2,2}; case '5':return{31,16,16,30,1,1,30}; case '6':return{14,16,16,30,17,17,14}; case '7':return{31,1,2,4,8,8,8}; case '8':return{14,17,17,14,17,17,14}; case '9':return{14,17,17,15,1,1,14};
case ':':return{0,4,4,0,4,4,0}; case '.':return{0,0,0,0,0,6,6}; case ',':return{0,0,0,0,6,6,4}; case '-':return{0,0,0,31,0,0,0}; case '_':return{0,0,0,0,0,0,31}; case '/':return{1,2,2,4,8,8,16}; case '!':return{4,4,4,4,4,0,4}; case '?':return{14,17,1,2,4,0,4}; case '+':return{0,4,4,31,4,4,0}; case '=':return{0,31,0,31,0,0,0}; case '(':return{2,4,8,8,8,4,2}; case ')':return{8,4,2,2,2,4,8}; case '[':return{14,8,8,8,8,8,14}; case ']':return{14,2,2,2,2,2,14}; case ' ':return{0,0,0,0,0,0,0}; default:return{0,0,0,0,0,0,0};}}
}
Canvas::Canvas(int w,int h):width_(w),height_(h){if(w<=0||h<=0)throw std::invalid_argument("Canvas dimensions must be positive");pixels_.resize(static_cast<std::size_t>(w)*h);clear(colors::Black);}
void Canvas::clear(Color c){std::fill(pixels_.begin(),pixels_.end(),c);}
void Canvas::pixel(int x,int y,Color c){if(x<0||y<0||x>=width_||y>=height_)return;pixels_[static_cast<std::size_t>(y)*width_+x]=c;}
Color Canvas::getPixel(int x,int y)const{if(x<0||y<0||x>=width_||y>=height_)return colors::Black;return pixels_[static_cast<std::size_t>(y)*width_+x];}
void Canvas::fillRect(int x,int y,int w,int h,Color c){if(w<=0||h<=0)return;int l=std::max(0,x),t=std::max(0,y),r=std::min(width_,x+w),b=std::min(height_,y+h);for(int py=t;py<b;++py)for(int px=l;px<r;++px)pixel(px,py,c);}
void Canvas::drawText(int x,int y,const std::string& text,Color c,int scale){if(scale<=0)return;int cx=x;for(char ch:text){Glyph g=glyph(ch);for(int row=0;row<7;++row)for(int col=0;col<5;++col)if(g[row]&(1u<<(4-col)))fillRect(cx+col*scale,y+row*scale,scale,scale,c);cx+=6*scale;}}
}
