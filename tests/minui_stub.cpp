#include "minui/minui.h"
#include <vector>
struct Fill { int x1,y1,x2,y2; unsigned char r,g,b,a; };
static std::vector<Fill> fills;
static Fill current{};
static int flipCount=0,exitCount=0;
static int screenWidth=8,screenHeight=4;
int gr_init(){return 0;}
void gr_exit(){++exitCount;}
int gr_fb_width(){return screenWidth;}
int gr_fb_height(){return screenHeight;}
void gr_flip(){++flipCount;}
void gr_color(unsigned char r,unsigned char g,unsigned char b,unsigned char a){current.r=r;current.g=g;current.b=b;current.a=a;}
void gr_fill(int x1,int y1,int x2,int y2){fills.push_back({x1,y1,x2,y2,current.r,current.g,current.b,current.a});}
void reset_minui_stub(){fills.clear();flipCount=exitCount=0;}
const std::vector<Fill>& minui_fills(){return fills;}
int minui_flip_count(){return flipCount;}
int minui_exit_count(){return exitCount;}
