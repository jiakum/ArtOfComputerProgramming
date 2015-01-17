#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

struct link {
	unsigned char *data;
	struct link *next;
};
static unsigned char *data[] = {
	"1234",
	"2134",
	"9999",
	"1111",
	"3234",
	"4234",
	"1534",
	"1634",
	"1734",
	"1284",
	"1294",
	"1239",
	"1230",
	"7234",
	"6734",
	"7834",
	"8734",
	"5634",
	"4534",
	"2222"
};

int main()
{
	int k,i,j,n,m;
	struct link *p,*top[10],botm[10],*total;

	n = sizeof(data)/sizeof(data[0]);
	total = malloc(n*sizeof(*total));
	memset(total, 0, n*sizeof(*total));
	j = n-1;
	p = &total[n-1-j];
	p->data = data[j];
	m = 4;

	for(k=1; k<=m; k++) {
		for(i=0;i<10;i++) {
			top[i] = &botm[i];
			botm[i].next = NULL;
		}
		while(1) {
			i = p->data[m-k] - '0';
			if(top[i])
				top[i]->next = p;
			top[i] = p;
			if((k==1) && j) {
				j--;
				p = &total[n-1-j];
				p->data = data[j];
			} else {
				p = p->next;
				if(!p)
					break;
			}
		}

		i = 0;
		while(1) {
			p = top[i];
			while(++i<10) {
				if(botm[i].next)
					break;
			}
			if(i >= 10)
				break;
			p->next = botm[i].next;
		}
		p = botm[0].next;
	}

	while(p) {
		printf("%s\n",p->data);
		p = p->next;
	}

	return 0;
}
