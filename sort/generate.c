#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#include <string.h>
#include <math.h>

static int generate_nums(int max)
{
	int fd,num,data,i = 0;
	FILE *fp;
	char *buf;

	if((!(fp = fopen("data.c", "wb"))) || ((fd = open("/dev/urandom", O_RDONLY)) < 0)) {
		printf("open file failed\n");
		return -1;
	}
	buf ="/* \n"
		 "* This file contains data to be sorted. \n"
		 "* It is shared by every sort function. \n"
		 "* Don't delete it. \n"
		 "*/ \n"
		 "static int data[] = {  \n    ";
	fwrite(buf, strlen(buf), 1, fp);

	while(i++ < max) {
		read(fd, &data, sizeof(data));
		fprintf(fp, " %d,", data);
		if(!(i%10))
			fprintf(fp, " \n    ");
	}
	fprintf(fp, " %d};", 0);
	close(fd);fclose(fp);

	return 0;
}

static int H[64] = {1};
static int generate_h(int max)
{
	int i;
	double result,s;

	for(i=1; i< sizeof(H)/sizeof(H[0]); i++) {
		s = i;
		if(i&0x01)
			result = 8*pow(2, s) - 6*pow(2, (s+1)/2) + 1.0;
		else
			result = 9*pow(2, s) - 9*pow(2, s/2) + 1.0;
		H[i] = (result+0.5);
		printf(" %d,",H[i]);
		if(!(i%10))
			printf(" \n");
		if(result > max)
			break;
	}
	printf(" \n");
	printf("  %d",0x7FFFFFFF);
	printf(" \n");
	return 0;
}

int main()
{
	if(generate_nums(100000) < 0)
		printf("generate numbers failed\n");
	else
		printf("generate numbers success\n");
	generate_h(0x7FFFFFFF);
	return 0;
}
