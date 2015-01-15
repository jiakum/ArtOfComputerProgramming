#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "queued_data.c"

static int delta[36] = {0};
int main()
{
	int n,i,j,success = 0;

	n = sizeof(data)/sizeof(data[0]);
	for(i=1;i<sizeof(delta)/sizeof(delta[0]);i++)
		delta[i] = (n + (1<<(i-1)))>>i;

	for(i=delta[1],j=2; delta[j]!=0;) {
		if(search > data[i])
			i += delta[j];
		else if(search < data[i])
			i -= delta[j];
		else {
			success = 1;
			break;
		}
		j++;
	}

	if(success)
		printf("    search success!\n    i:  %d,search:  %d  =  %d\n",i,search,data[i]);
	else
		printf("    search failed!\n    search:%d\n",search);

	return 0;
}
