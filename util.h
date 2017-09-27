#ifndef UTIL_H
#define UTIL_H

#include "kprintf.h"

void kmemcpy(void * d, void * s, int n);

void kmemset(void * p, char v, int n);

void panic(const char * msg);

int kdivide(int n, int d);

int kmodulo(int n, int d);

#endif
