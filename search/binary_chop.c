#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "queued_data.c"

int main()
{
	int n,l,r,success,i,k;

	n = sizeof(data)/sizeof(data[0]);
	l = 0; r = n-1; success = 0;

	while(1) {
		if(r < l)
			break;
		i = (r + l)>>1;
		k = data[i];
		if(k > search)
			r = i - 1;
		else if(k < search)
			l = i + 1;
		else {
			success = 1;
			break;
		}
	}

	if(success)
		printf("    search success!\n    i:  %d,search:  %d  =  %d\n",i,search,data[i]);
	else
		printf("    search failed!\n    search:%d\n",search);

	return 0;
}
