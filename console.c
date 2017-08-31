#include "console.h"
#include "font.h"

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

void draw_char(int x, int y, unsigned char c) {
	int i;
	int j;
	for (i = 0; i < CHAR_WIDTH; i++) {
		for (j = 0; j < CHAR_HEIGHT; j++) {
			int pixel_on = (font_data[c][i] >> j) & 1;
			if (pixel_on) {
				set_pixel(CHAR_WIDTH - j + x, i + y, 0x1f, 0x00);
			}
		}
	}
}

void draw_string(int x, int y, int num_chars, const char * c) {
	int i;
	for (i = 0; i < num_chars; i++) {
		draw_char(x + (CHAR_WIDTH * i), y, c[i]);
	}
}
