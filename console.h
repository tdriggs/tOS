#define WIDTH 800
#define HEIGHT 600

#define pl110 		((volatile unsigned*)0xc0000000)
#define haxis       (*(pl110+0))    //offset 0: horizontal axis
#define vaxis       (*(pl110+1))    //offset 4: vertical axis
#define polarity    (*(pl110+2))    //offset 8: clock+polarity
#define lineend     (*(pl110+3))    //offset 12: line end
#define baseaddr1   (*(pl110+4))    //offset 16: upper panel base address
#define baseaddr2   (*(pl110+5))    //offset 20: lower panel base address
#define intmask     (*(pl110+6))    //offset 24: interrupt mask
#define params      (*(pl110+7))    //offset 28: panel parameters

void console_init();

void set_pixel(int x, int y, unsigned int color);

void draw_block(int centerX, int centerY, int size, unsigned int color);