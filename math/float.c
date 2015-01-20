#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define p (127u)
typedef struct {
	int f:24;
	unsigned int e:8;
} f_t;

static f_t f_plus(f_t u, f_t v)
{
	int f;
	f_t w;

	if(u.e < v.e) {
		w = u;
		u = v;
		v = w;
	}

	if(u.e - v.e >= p+2)
		w = u;
	else {
		w.e = u.e;
		f = u.f + (v.f>>(u.e-v.e));
		if(f & (1 << 24)) {
			f >>= 1;
			w.e <<= 1;
		} else {
			while(!(f&(1<<23))) {
				f <<= 1;
				w.e >>= 1;
			}
		}
		w.f = f;
	}

	return w;
}

int main()
{
	f_t u,v;

	u.e = 10;
	u.f = 0x07F0000;
	v.e = 10;
	v.f = 0x07F0000;

	u = f_plus(u,v);
	printf("  0x%x\n",*(uint32_t*)&u);

	return 0;
}
