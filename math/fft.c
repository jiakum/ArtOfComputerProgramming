#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define pi (3.141592654)
static int inv;
static int npt = 16;

typedef struct {
	double real;
	double imag;
} compleX;
static compleX buf[16];

static void fft()
{
	int sign;
	long m,item,l,le,le1,k,ip,i,j;
	double ur,ui,wr,wi,tr,ti,temp;

	j = 1;
	for(i=1; i<npt; ++i) {
		if(i<j) {
			tr = buf[j].real; ti = buf[j].imag;
			buf[j].real = buf[i].real;
			buf[j].imag = buf[i].imag;
			buf[i].real = tr; buf[i].imag = ti;
			k = npt >> 1;
			while(k < j) {
				j -= k;
				k >>= 1;
			};
		} else {
			k = npt >> 1;
			while(k < j) {
				j -= k;
				k >>= 1;
			};
		}
		j += k;
	}

	m = 0; item = npt;
	while(item > i) {
		item >>= 1;
		m++;
	}
	if(inv == 1)
		sign = 1;
	else 
		sign = -1;

	for(l=1; l<=m; l++) {
		le = pow(2,l);
		le1 = le >> 1;
		ur = 1.0; ui = 0;
		wr = cos(pi/le1);
		wi = sign * sin(pi/le1);
		for(j=1; j<le1; ++j) {
			i = j;
			while(i <= npt) {
				ip = i + le1;
				tr = buf[ip].real*ur - buf[ip].imag*ui;
				ti = buf[ip].imag*ur + buf[ip].real*ui;
				buf[ip].real -= tr;
				buf[ip].imag -= ti;
				buf[j].real += tr;
				buf[j].imag += ti;
				i += le;
			}
			temp = ur*wr - ui*wi;
			ui = ui*wr + ur*wi;
			ur = temp;
		}
	}
	if(inv == 1) {
		for(i =1; i<= npt; ++i) {
			buf[i].real /= npt;
			buf[i].imag /= npt;
		}
	}
}

int main()
{
	printf("select type of transfrom\n");
	printf("\n");
	printf("0    for forward DFT\n");
	printf("1    for inverse DFT\n");
	scanf("%d",inv);
	read_data();
	fft();
	save_data();

	return 0;
}
