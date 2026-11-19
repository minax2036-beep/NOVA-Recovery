#include "nova/recovery_display.hpp"
#include "nova/renderer.hpp"
#include <cassert>
#include <vector>
struct Fill { int x1,y1,x2,y2; unsigned char r,g,b,a; };
void reset_minui_stub();
const std::vector<Fill>& minui_fills();
int minui_flip_count();
int minui_exit_count();
int main(){
    reset_minui_stub();
    nova::RecoveryDisplay display;
    assert(display.initialize());
    assert(display.isInitialized());
    assert(display.width()==8 && display.height()==4);
    nova::Canvas canvas(2,2);
    canvas.clear(nova::colors::Black);
    canvas.pixel(1,0,nova::colors::Cyan);
    canvas.pixel(0,1,nova::colors::Blue);
    assert(display.present(canvas));
    const auto& fills=minui_fills();
    assert(fills.size()==4);
    assert(fills[0].x1==0 && fills[0].y1==0 && fills[0].x2==4 && fills[0].y2==2);
    assert(fills[1].x1==4 && fills[1].y1==0 && fills[1].x2==8 && fills[1].y2==2);
    assert(fills[2].x1==0 && fills[2].y1==2 && fills[2].x2==4 && fills[2].y2==4);
    assert(fills[3].x1==4 && fills[3].y1==2 && fills[3].x2==8 && fills[3].y2==4);
    assert(minui_flip_count()==1);
    display.shutdown();
    assert(minui_exit_count()==1);
    return 0;
}
