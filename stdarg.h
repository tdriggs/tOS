#ifndef STDARG_H
#define STDARG_H

typedef struct _va_list {
	char * p;
} va_list;

#define va_start(L, x) _va_start(&L, &x, sizeof(x))

static void _va_start(va_list * L, void * x, int sz) {
	L->p = ((char *)x + sz);
}

#define va_arg(L, T) (*(T*)(_va_arg(&L, sizeof(T))))

static void * _va_arg(va_list * L, int sz) {
	char * result = L->p;
	L->p += sz;
	return result;
}

static void va_end(va_list L) {}

#endif
