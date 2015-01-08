#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "data.c"

#define M 9

struct {
	int start,end;
} *stack;
static int ptr = 0;
#define PUSH(l,r) 	do { \
						if(l < r) { \
							stack[ptr].start = l;\
							stack[ptr].end = r;\
							ptr++;\
						 }\
					}while(0);
static int inline PULL(int *l,int *r) {
	if(ptr) {
		ptr--;
		*l = stack[ptr].start;
		*r = stack[ptr].end;
		return 1; // success
	}
	return 0;
}

int main()
{
	int i,j,k,r,l,n,tmp;

	n = sizeof(data)/sizeof(data[0]);
	stack = malloc(n * sizeof(*stack));
	l = 0; r = n - 1;

	while(1) {
		if(r-l<M && !PULL(&l,&r))
			break;

		k = data[l]; j=r+1; i=l;
		while(1) {
			while(--j > l) {
				if(data[j] <= k)
					break;
			}
			while(++i < j) {
				if(data[i] >= k)
					break;
			}

			if(i < j){
				tmp = data[j];
				data[j] = data[i];
				data[i] = tmp;
				continue;
			} else {
				tmp = data[j];
				data[j] = data[l];
				data[l] = tmp;
				break;
			}
		}

		if(r - j > M) { 
			if(j - l > M) {
				PUSH(l,j-1);
				r = j - 1;
			} else 
				l = j+1;
			continue;
		} else if(j - l > M) {
			PUSH(j+1, r);
			l = j+1;
			continue;
		} else if(!PULL(&l,&r))
			break;
	}

	for(j=1; j<n; j++) {
		if(data[j-1] <= data[j])
			continue;
		k = data[j];
		for(i=j-1; i>=0; i--) {
			if(data[i] <= k)
				break;
			data[i+1] = data[i];
		}	
		data[i+1] = k;	
	}

	for(i=0; i<n; i++)
		printf("    %d\n",data[i]);

	return 0;
}
