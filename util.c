#include "util.h"

void kmemcpy(void * d, void * s, int n) {
	char * dp = (char *)d;
	char * sp = (char *)s;
	while (n--) {
		*dp++ = *sp++;
	}
}

void kmemset(void * p, char v, int n) {
	char * pp = (char *)p;
	while(n--) {
		*pp++ = v;
	}
}

void panic(const char * msg) {
	kprintf("%s", msg);
	while(1);
}

int kdivide(int n, int d) {
	int Q = 0;
	int TUB = 0;

	int i;
	for (i = 0; i < 32; ++i) {
		TUB <<= 1;
		if (n & (0x80000000 >> i)) {
			TUB |= 1;
		}

		Q <<= 1;
		if (TUB >= d) {
			Q |= 1;
			TUB -= d;
		}
	}

	return Q;
}

int kmodulo(int n, int d) {
	int Q = 0;
	int TUB = 0;

	int i;
	for (i = 0; i < 32; ++i) {
		TUB <<= 1;
		if ((0x80000000 >> i) & n) {
			TUB |= 1;
		}

		Q <<= 1;
		if (TUB >= d) {
			Q |= 1;
			TUB -= d;
		}
	}

	return TUB;
}

void halt() {
	asm volatile(
        "mov r0,#0\n"
        "mcr p15,0,r0,c7,c0,4" 
        ::: "r0"
    );
}
