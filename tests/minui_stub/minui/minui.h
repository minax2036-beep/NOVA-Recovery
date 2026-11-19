#pragma once
int gr_init();
void gr_exit();
int gr_fb_width();
int gr_fb_height();
void gr_flip();
void gr_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void gr_fill(int x1, int y1, int x2, int y2);
