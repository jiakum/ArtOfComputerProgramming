#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ISLETTER(x) (((x>='A')&&(x<='Z')) ? 1 : 0)
#define ISDIGIT(x)  (((x>='0')&&(x<='9')) ? 1 : 0)
#define ISRAIL(x)   ((x=='-') ? 1 : 0)
static inline char letter2digit(char letter)
{
	char digit = 0;

	if((letter >= 'A') && (letter <= 'P'))
		digit = (letter - 'A')/3 + '2';
	else if((letter >= 'R') && (letter <= 'Y'))
		digit = (letter - 'Q')/3 + '7';

	return digit;
}

#define LINE_SIZE (32)
#define TELEPHONE_LEN (7)
#define ALIGN_INT_ADDRESS(p) (((unsigned long)p+0x3)&(~0x3))
#define FREQ_ADDRESS(p) ((int *)ALIGN_INT_ADDRESS((unsigned long)p+TELEPHONE_LEN+1))

static inline int print_telephone(char *buf)
{
	int freq;
	if(!buf)
		return 0 ;
	freq = *FREQ_ADDRESS(buf) + 1;
	if(freq > 1) {
		printf("%c%c%c-%c%c%c%c %d\n",buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6],freq);
		return 1;
	} else
		return 0;
}

int main()
{
	int total,i,j,len,ptr;
	int *freq;
	char **strings;

	scanf("%d\n",&total);
	if(!(strings = (char**)malloc(total*sizeof(char*)))) {
		fprintf(stderr,"fatal error!can not malloc %d memory.\n",(int)(total*sizeof(char*)));
		return -1;
	}
	for(i=0;i<total;i++) {
		char *str = (char*)malloc(LINE_SIZE);
		if(!str) {
			fprintf(stderr,"fatal error!can not malloc %d memory.\n",LINE_SIZE);
			return -1;
		}
		strings[i] = str;
		if(!fgets(str,LINE_SIZE,stdin)) {
			fprintf(stderr,"fgets error!i:%d\n",i);
			return -1;
		}

		len = -1; ptr = 0;
		while((++len < LINE_SIZE) && (ptr < TELEPHONE_LEN)){
			if(ISRAIL(str[len]))
				continue;
			else if(ISDIGIT(str[len])) 
				str[ptr++] = str[len];
			else if(ISLETTER(str[len])) 
				str[ptr++] = letter2digit(str[len]);
		}
		str[TELEPHONE_LEN] = '\0';
		freq = FREQ_ADDRESS(str);
		*freq = 0;
	}

	for(i=0;i<total;i++) {
		int cmp;
		char *temp;
		for(j=0;j<total;j++) {
			if(i == j)
				continue;
			if(!strings[i])
				break;
			if(!strings[j])
				continue;
			cmp = strncmp(strings[i],strings[j],TELEPHONE_LEN);
			if(cmp == 0) {
				freq = FREQ_ADDRESS(strings[i]);
				*freq += *FREQ_ADDRESS(strings[j])+1;
				free(strings[j]);
				strings[j] = NULL;
			} else if((cmp > 0) && (i < j)) {
				temp = strings[i];
				strings[i] = strings[j];
				strings[j] = temp;
			} else if((cmp < 0) && (i > j)) {
				temp = strings[i];
				strings[i] = strings[j];
				strings[j] = temp;
			}
		}
	}

	j = 0;
	for(i=0;i<total;i++) 
		j += print_telephone(strings[i]);
	if(j == 0)
		printf("No duplicates.\n");
	return 0;
}
