#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "data.c"

int main()
{
	int p,q,i,n,s,t;
	int *L,*result;

	n = sizeof(data)/sizeof(int);
	L = malloc((n+2) * sizeof(int));
	result = malloc((n+1) * sizeof(int));
	for(i=1; i<n+1; i++)
		result[i] = data[i-1];

	for(i=1; i<n-1; i++)
		L[i] = -(i+2);
	L[0] =1; L[n+1]=2; L[n]=0; L[n-1]=0;

	while(1) {
		s=0; t=n+1; p=L[s]; q=L[t];
		if(q == 0)
			break;

		while(1) {
			if(result[p] <= result[q]) {
				L[s] = (L[s]<0) ? -p : p;
				s = p; p = L[p];
				if(p > 0)
					continue;
				L[s] = q; s = t;
				do{
					t = q;
					q = L[q];
				}while(q > 0);
			} else {
				L[s] = (L[s]<0) ? -q : q;
				s = q; q = L[q];
				if(q > 0)
					continue;
				L[s] = p; s = t;
				do{
					t = p;
					p = L[p];
				}while(p > 0);
			}

			if(q < 0) {
				p = -p; q = -q;
				continue;
			} else if(q == 0){
				L[s] = (L[s]<0) ? p : -p;
				L[t] = 0;
				break;
			}
		}
	}

	i = 0; s = 0;
	do {
		data[i] = result[L[s]];
		s = L[s];i++;
	}while(L[s] > 0); 
	for(i=0; i<n; i++)
		printf("    %d\n",data[i]);

	free(L);free(result);

	return 0;
}
