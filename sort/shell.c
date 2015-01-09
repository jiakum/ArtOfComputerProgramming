#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "data.c"

static const int H_LOWER[] = { 1, 3, 7, 16, 36, 81, 183, 412, 927, 2086};
static const int H_HIGHER[] = { 1,  5, 19, 41, 109, 209, 505, 929, 2161, 3905, 8929,
								16001, 36289, 64769, 146305, 260609, 587521, 1045505, 2354689, 4188161, 9427969,
								16764929, 37730305, 67084289, 150958081, 268386305, 603906049, 1073643521, 2147483647};

int main(int argc,char *argv[])
{
	int h,t,i,j,n,k;
	const int *H;

	n = sizeof(data)/sizeof(int);
	if(n < 1000)
		H = H_LOWER;
	else 
		H = H_HIGHER;
	for(i=0;;i++) {
		if(H[i+2] >= n) {
			t = i; 
			break;
		}				
	}

	for(; t>=0; t--) {
		h = H[t];
		for(i=h-1; i<n; i+=h) {
			int r = data[i];
			for(j=i-h; j>=0; j-=h) {
				if(data[j] <= r)
					break;
				else
					data[j+h] = data[j];
			}
			data[j+h] = r;
		}
	}

	for(i=0; i<n; i++)
		printf("    %d\n",data[i]);

	return 0;
}
