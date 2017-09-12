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
