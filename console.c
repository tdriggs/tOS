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

void set_pixel(int x, int y, unsigned int color)
{
	framebuffer[WIDTH * 2 * y + 2 * x] = (unsigned char)(color >> 4);
	framebuffer[WIDTH * 2 * y + 2 * x + 1] = (unsigned char)(color & 0xf);
}

void draw_block(int centerX, int centerY, int size, unsigned int color)
{
	int i;
	int j;
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			set_pixel(centerX + i - (size / 2), centerY + j - (size / 2), color);
		}
	}
}