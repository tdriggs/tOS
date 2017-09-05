#include "console.h"

void sweet();

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

	// char * string = "Taylor Driggs";
	// draw_string(400, 300, string, 255, 255, 255);

	sweet();

	while(1) {

	}
}
