#include "console.h"

void wait(int ticks)
{
	while (ticks > 0)
	{
		ticks -= 1;
	}
}

void kernel_main()
{
	console_init();

	unsigned int color = 0x0000;
	// draw_block(400, 300, 10, 0x0000);
	// set_pixel(400, 300);

	while(1)
	{
		wait(1000000);
		color += 1;
		draw_block(400, 300, 10, color);
	}
}