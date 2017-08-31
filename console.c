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

void set_pixel(int x, int y, unsigned int color_first, unsigned int color_second)
{
	framebuffer[WIDTH * 2 * y + 2 * x] = (unsigned char)(color_first);
	framebuffer[WIDTH * 2 * y + 2 * x + 1] = (unsigned char)(color_second);
}

void draw_block(int centerX, int centerY, int size, unsigned int color_first, unsigned int color_second)
{
	int i;
	int j;
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			set_pixel(centerX + i - (size / 2), centerY + j - (size / 2), color_first, color_second);
		}
	}
}

void draw_initials(int startX, int startY, int size, unsigned int color_first, unsigned int color_second) {
	// T
	draw_block(startX, startY, size, color_first, color_second);
	draw_block(startX, startY + size, size, color_first, color_second);
	draw_block(startX - size * 3, startY - size, size, color_first, color_second);
	draw_block(startX - size * 2, startY - size, size, color_first, color_second);
	draw_block(startX - size, startY - size, size, color_first, color_second);
	draw_block(startX, startY - size, size, color_first, color_second);
	draw_block(startX + size, startY - size, size, color_first, color_second);
	draw_block(startX + size * 2, startY - size, size, color_first, color_second);
	draw_block(startX + size * 3, startY - size, size, color_first, color_second);
	draw_block(startX, startY + size * 2, size, color_first, color_second);
	draw_block(startX, startY + size * 3, size, color_first, color_second);
	draw_block(startX, startY + size * 4, size, color_first, color_second);
	draw_block(startX, startY + size * 5, size, color_first, color_second);

	// D
	draw_block(startX + size * 5, startY - size, size, color_first, color_second);
	draw_block(startX + size * 5, startY, size, color_first, color_second);
	draw_block(startX + size * 5, startY + size, size, color_first, color_second);
	draw_block(startX + size * 5, startY + size * 2, size, color_first, color_second);
	draw_block(startX + size * 5, startY + size * 3, size, color_first, color_second);
	draw_block(startX + size * 5, startY + size * 4, size, color_first, color_second);
	draw_block(startX + size * 5, startY + size * 5, size, color_first, color_second);
	draw_block(startX + size * 6, startY + size * 5, size, color_first, color_second);
	draw_block(startX + size * 7, startY + size * 5, size, color_first, color_second);
	draw_block(startX + size * 8, startY + size * 5, size, color_first, color_second);
	draw_block(startX + size * 6, startY - size, size, color_first, color_second);
	draw_block(startX + size * 7, startY - size, size, color_first, color_second);
	draw_block(startX + size * 8, startY - size, size, color_first, color_second);
	draw_block(startX + size * 9, startY, size, color_first, color_second);
	draw_block(startX + size * 9, startY + size, size, color_first, color_second);
	draw_block(startX + size * 9, startY + size * 2, size, color_first, color_second);
	draw_block(startX + size * 9, startY + size * 3, size, color_first, color_second);
	draw_block(startX + size * 9, startY + size * 4, size, color_first, color_second);
}