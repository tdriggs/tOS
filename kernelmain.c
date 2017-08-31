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

	unsigned int color_first = 0xFF;
	unsigned int color_second = 0x07;

	while(1)
	{
		wait(1000000);
		color_first++;
		color_second++;
		draw_initials(400, 300, 10, color_first, color_second);
	}
}

// Red :: 00011111 00000000 :: 0x1F00
// Green :: 11100000 00000111 :: 0xE007
// Blue :: 0000000 11111000 :: 0x00F8
// Yellow :: 11111111 00000111 :: 0xFF07