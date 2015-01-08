#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define M 2

struct {
	int start,end;
} *stack;
static int ptr = 0;
#define PUSH(l,r) 	do { \
						stack[ptr].start = l;\
						stack[ptr].end = r;\
						ptr++;\
					}while(0);
static int inline PULL(int *l,int *r) {
	if(ptr) {
		ptr--;
		*l = stack[ptr].start;
		*r = stack[ptr].end;
		return 1; // success
	}
	return 0; // failed
}

static int data[] = { 10, 20, 30, 40, 20, 30, 9, 44, 99, 100};

int main()
{
	int i,j,k,r,l,n,tmp,b;

	n = sizeof(data)/sizeof(data[0]);
	stack = malloc(sizeof(data[0]) * 8 * sizeof(*stack));
	l = 0; r = n - 1; b = 1 << 8;
	 i = j = 0;

	while(1) {
		if(i >= j)
			i = l; j = r;
		while(1) {
			if(i > j)
				break;;
			if(data[i] & b)
				break;
			i++;
		}
		while(1) {
			if(j < i)
				break;
			if(!(data[j] & b))
				break;
			j--;
		}

		if(i < j) {
			tmp = data[j];
			data[j] = data[i];
			data[i] = tmp;
			continue;
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
		printf("    %d\n",data[i]);

	return 0;
}
