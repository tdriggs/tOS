#include "console.h"

volatile unsigned char* framebuffer = (unsigned char*)((0x07ffffff - WIDTH * HEIGHT * 2) & 0xfffffff0);

void console_init()
{
	haxis = 0x3f1f3f00 | (WIDTH / 4 - 4);
	vaxis = 0x80b6000 | (HEIGHT - 1);
	polarity = 0x067f3800;
	params = 0x1829;
	baseaddr1 = (unsigned)framebuffer;
}

void set_pixel(int x, int y)
{
	framebuffer[WIDTH * 2 * y + 2 * x] = 0x0f;
	framebuffer[WIDTH * 2 * y + 2 * x + 1] = 0x00;
}