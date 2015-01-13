#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "data.c"

#define M 2

struct {
	int start;
	int32_t end;
} *stack;
static int ptr = 0;
#define PUSH(l,r) 	do { \
						stack[ptr].start = l;\
						stack[ptr].end = r;\
						ptr++;\
					}while(0);
static int inline PULL(int *l,uint32_t *r) {
	if(ptr) {
		ptr--;
		*l = stack[ptr].start;
		*r = stack[ptr].end;
		return 1; // success
	}
	return 0; // failed
}

int main()
{
	int i,j,r,l,n,tmp;
	uint32_t b;

	n = sizeof(data)/sizeof(data[0]);
	stack = malloc(sizeof(data[0]) * sizeof(*stack));
	l = 0; r = n - 1; b = 1 << 31;  // Attention!Change value b if neccesary
	i = j = 0;

	while(1) {
		if(l == r) {
			l = r +1;
			if(!PULL(&r,&b))
				break;
		}
		if(i >= j)
			i = l, j = r;

		while(i <= j) {
			if(data[i] & b)
				break;
			i++;
		}
		while(j >= i) {
			if(!(data[j] & b))
				break;
			j--;
		}

		if(i < j) { 
			tmp = data[j];
			data[j] = data[i];
			data[i] = tmp;
			continue; /* Attention! In next loop,the value of i and j will not change*/
		}

		if(b >>= 1) {
			if(j == r || j < l)
				continue;
			else if(j == l) {
				l += 1;
				continue;
			}
			PUSH(r,b);
			r = j;
		} else {
			l = r +1;
			if(!PULL(&r,&b))
				break;
		}
	}

	for(i=0; i<n; i++)
		printf("    %u\n",data[i]);

	return 0;
}
