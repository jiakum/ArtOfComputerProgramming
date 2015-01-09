#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "data.c"

static int T = 1;
static const int H_LOWER[] = { 1, 3, 7, 16, 36, 81, 183, 412, 927, 2086};
static const int H_HIGHER[] = { 1, 3, 7, 16, 36, 81, 183, 412, 927, 2086};

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
		if(H[i+2] > n) {
			t = i; 
			break;
		}				
	}

	for(t=T; t>=0; t--) {
		h = H[t];
		for(i=h-1; i<n; i+=h) {
			int r = data[i];
			for(j=i-h; j>=0; j-=h) {
				if(data[j] <= r)
					break;
				else {
					data[j+h] = data[j];
				}
			}
			data[j+h] = r;
		}
	}

	for(i=0; i<n; i++)
		printf("    %d\n",data[i]);

	return 0;
}
