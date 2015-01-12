#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "data.c"

int main()
{
	int i,j,k,r,l,n,tmp;

	n = sizeof(data)/sizeof(data[0]);
	l = ((n-1)>>1) + 1; r = n - 1;

	while(1) {
		if(l > 0) {
			l -= 1;
			k = data[l];
		} else if(r > 0) {
			k = data[r];
			data[r] = data[l];
			data[l] = k;
			r--;
		} else
			break;

		i = l; j = i << 1;
		while(j < r) {
			if(data[j] < data[j+1])
				j += 1;
			if(data[j] <= k)
				break;
			data[i] = data[j];
			i = j; j <<= 1;
		}
		if((j == r) && (data[j] > k)) {
			data[i] = data[j];
			i = j; j <<= 1;
		}
		data[i] = k;
	}

	for(i=0; i<n; i++)
		printf("    %d\n",data[i]);

	return 0 ;
}
