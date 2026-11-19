#include "nova/logger.hpp"
#include "nova/renderer.hpp"
#include <iostream>
int main(){
 nova::Logger::info("NOVA Core starting"); nova::Logger::info("Renderer initialized (host canvas)");
 nova::Canvas canvas(320,240); canvas.clear(nova::colors::Black);
 canvas.fillRect(0,0,320,3,nova::colors::Cyan);
 canvas.drawText(16,18,"NOVA RECOVERY",nova::colors::White,2);
 canvas.drawText(16,44,"RENDERER TEST",nova::colors::Cyan);
 canvas.fillRect(16,68,288,1,nova::colors::Gray);
 canvas.drawText(16,82,"LOG SYSTEM: OK",nova::colors::White);
 canvas.drawText(16,98,"CANVAS: 320X240",nova::colors::White);
 canvas.fillRect(16,130,120,18,nova::colors::Blue);
 canvas.drawText(20,135,"BOOT",nova::colors::White);
 canvas.drawText(16,168,"DISPLAY BACKEND",nova::colors::Gray);
 canvas.drawText(16,180,"NOT CONNECTED",nova::colors::Gray);
 if(canvas.getPixel(0,0)!=nova::colors::Cyan||canvas.getPixel(20,135)!=nova::colors::Blue){nova::Logger::error("Renderer smoke test failed");return 1;}
 nova::Logger::info("Renderer smoke test passed");
 std::cout<<"NOVA host rendering smoke test: PASS\nCanvas: "<<canvas.width()<<"x"<<canvas.height()<<"\n";return 0;
}
