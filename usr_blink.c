#define WIDTH 800
#define HEIGHT 600
#define framebuffer_start ((0x07ffffff - WIDTH * HEIGHT * 2) & 0xfffffff0)

unsigned char * framebuffer = (unsigned char *)framebuffer_start;

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

int main(int argc, char* argv[])
{
    int i,j;
    for(i=0;i<HEIGHT;++i){
        for(j=0;j<WIDTH;++j){
            set_pixel(j, i, 255, 0, 0);
        }
    }

    asm volatile("bkpt");

    return 0;
}