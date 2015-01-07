#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

static int data[] = { 10, 20, 30, 40, 20, 30, 9, 44, 99, 100};
static const int T = 1;
static const int H[] = { 1, 3};

int main(int argc,char *argv[])
{
	int h,t,i,j,n,k;

	n = sizeof(data)/sizeof(int);
	for(t=T; t>=0; t--) {
		h = H[t];
		for(i=h-1; i<n; i+=h) {
			int r = data[i];
			for(j=i-h; j>=0; j-=h) {
				if(data[j] >= r) {
					data[j+h] = r;
					break;
				} else {
					data[j+h] = data[j];
				}
			}
			data[j+h] = r;
		}
	}

	for(i=0; i<n; i++)
		printf("    %d\n",data[i]);
	printf("    %d\n",n);

	return 0;
}
