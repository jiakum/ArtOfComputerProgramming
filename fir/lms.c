#include <stdlib.h>
#include <stdio.h>

#define mu 0.04

void initlms(int N, double w[])
{
	int i;
	for(i=0; i<N; ++i) {
		w[i] = 0;
	}
}

double lmsflt(double w[], double x[], int N, double dk)
{
	int i;
	double uek,yk,ek;

	yk = 0;
	for(i=0; i<N; ++i)
		yk += w[i]*x[i];
	ek = dk - yk;

	uek = 2 * mu * ek;
	for(i=0; i<N; i++)
		w[i] += uek*x[i];

	return yk;
}
