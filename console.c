#include "console.h"
#include "font.h"
#include "util.h"

volatile unsigned char * framebuffer = (unsigned char *)framebuffer_start;
volatile unsigned * serialport = (unsigned *) 0x16000000;
volatile unsigned * serialflags = (unsigned *) 0x16000018;

const int BYTES_IN_LINE = WIDTH * CHAR_HEIGHT * 2;
const int BYTES_ON_SCREEN = WIDTH * HEIGHT * 2;


void console_init() {
	haxis = 0x3f1f3f00 | (WIDTH / 4 - 4);
	vaxis = 0x80b6000 | (HEIGHT - 1);
	polarity = 0x067f3800;
	params = 0x1829;
	baseaddr1 = (unsigned)framebuffer;
}

void set_pixel(int x, int y, unsigned int r, unsigned int g, unsigned int b) {
	r &= 31;
	b &= 248;
	unsigned int g_r = g & 252;
	g_r = g_r << 3;
	unsigned int g_b = g & 224;
	g_b = g_b >> 5;

	unsigned int color_first = g_r | r;
	unsigned int color_second = b | g_b;
	framebuffer[WIDTH * 2 * y + 2 * x] = (unsigned char)(color_first);
	framebuffer[WIDTH * 2 * y + 2 * x + 1] = (unsigned char)(color_second);
}

void draw_block(int x, int y, int width, int height, unsigned int r, unsigned int g, unsigned int b) {
	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j) {
			set_pixel(x + i, y + j, r, g, b);
		}
	}
}

void draw_char(int x, int y, unsigned char c, unsigned int r, unsigned int g, unsigned int b) {
	for (int i = 0; i < CHAR_WIDTH; ++i) {
		for (int j = 0; j < CHAR_HEIGHT; ++j) {
			int pixel_on = (font_data[c][j] >> i) & 1;
			if (pixel_on) {
				set_pixel(CHAR_WIDTH - i + x, j + y, r, g, b);
			}
		}
	}
}

void draw_string(int x, int y, const char * c, unsigned int r, unsigned int g, unsigned int b) {
	int i = 0;
	while (c[i]) {
		draw_char(x + (CHAR_WIDTH * i), y, c[i], r, g, b);
		i++;
	}
}

void console_putc(unsigned char c) {
	static int row = 0;
	static int column = 0;
	static unsigned char prev_char = '\0';
	static int backspace_pressed = 0;

	if (c == '\t') {
		while (1) {
			++row;

			if (row >= WIDTH / CHAR_WIDTH) {
				++column;
				row = 0;

				if (column >= (HEIGHT / CHAR_HEIGHT) - 1) {
					kmemcpy((void *)framebuffer, (void *)(framebuffer + BYTES_IN_LINE), BYTES_ON_SCREEN - BYTES_IN_LINE);
					--column;
				}
			}

			if (row % 8 == 0) {
				break;
			}
		}
		backspace_pressed = 0;
	}
	else if (c == '\n') {
		++column;
		row = 0;

		if (column >= (HEIGHT / CHAR_HEIGHT) - 1) {
			kmemcpy((void *)framebuffer, (void *)(framebuffer + BYTES_IN_LINE), BYTES_ON_SCREEN - BYTES_IN_LINE);
			--column;
		}
		backspace_pressed = 0;
	}
	else if (c == '\x7f') {
		--row;
		if (row < 0) {
			--column;
			row = (int)(WIDTH / CHAR_WIDTH) - 1;
		}
		draw_block(row * CHAR_WIDTH, column * CHAR_HEIGHT, CHAR_WIDTH, CHAR_HEIGHT, 0, 0, 0);
		backspace_pressed = 0;
	}
	else if (c == '\r') {
		row = 0;
		backspace_pressed = 0;
	}
	else if (c == '\b') {
		--row;
		if (row < 0) {
			--column;
			row = (int)(WIDTH / CHAR_WIDTH);
		}
		backspace_pressed = 1;
	}
	else if (c == '\f') {
		row = 0;
		column = 0;
		draw_block(0, 0, WIDTH, HEIGHT, 0, 0, 0);
		backspace_pressed = 0;
	}
	else {
		if (backspace_pressed && c == prev_char) {
			draw_char(row * CHAR_WIDTH + 2, column * CHAR_HEIGHT, c, 255, 255, 255);
		}
		else {
			draw_block(row * CHAR_WIDTH, column * CHAR_HEIGHT, CHAR_WIDTH, CHAR_HEIGHT, 0, 0, 0);
			draw_char(row * CHAR_WIDTH, column * CHAR_HEIGHT, c, 255, 255, 255);
		}
		prev_char = c;
		++row;
		if (row >= WIDTH / CHAR_WIDTH) {
			++column;
			row = 0;

			if (column >= (HEIGHT / CHAR_HEIGHT) - 1) {
				kmemcpy((void *)framebuffer, (void *)(framebuffer + BYTES_IN_LINE), BYTES_ON_SCREEN - BYTES_IN_LINE);
				kmemset((void *)(framebuffer + BYTES_ON_SCREEN - BYTES_IN_LINE), '\0', BYTES_IN_LINE);
				--column;
			}

		}
		backspace_pressed = 0;
	}

	while(*serialflags & (1 << 5)) {}
	*serialport = c;
}
