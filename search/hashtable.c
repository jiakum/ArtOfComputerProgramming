#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "data.c"

struct tree{
	struct tree *next;
	int data;
} tree;

struct tree_mem{
	struct tree data;
	struct tree_mem *next;
};

#define NEW_TREE_COUNT (1024)
struct large_tree_mem {
	struct tree_mem data[NEW_TREE_COUNT];
	struct large_tree_mem *next,*prev;
};

static struct tree_mem *avail = NULL;
static struct large_tree_mem *ltm_stack = NULL;

static struct tree *alloc_tree()  //left and right will be set zero
{
	static const int newcount = 1024;
	struct tree_mem *m;

	if(!avail) {
		struct large_tree_mem *head;

		if(!(head = (struct large_tree_mem *)malloc(sizeof(*ltm_stack)))) {
			printf("fatal error!Can not alloc more memory\n    ptr:  %p\n",avail);
			return NULL;
		} else {
			int i;

			memset(head, 0, sizeof(*ltm_stack));
			avail = (struct tree_mem *)head;

			for(i=0; i<NEW_TREE_COUNT - 1; i++)
				(avail+i)->next = avail+i+1;
			(avail+i)->next = NULL;

			if(!ltm_stack)
				ltm_stack = head;
			else {
				ltm_stack->prev = head;
				head->next = ltm_stack;
				ltm_stack = head;
			}
		}
	}

	m = avail;
	avail = avail->next;
	memset(m, 0, sizeof(*m));

	return &(m->data);
}

static void free_tree(struct tree *p)
{
	if(p) {
		struct tree_mem *m = (struct tree_mem *)p;
		m->next = avail;
		avail = m;
	}
}

static void free_alltrees()
{
	struct large_tree_mem *next;

	while(ltm_stack) {
		next = ltm_stack->next;
		free(ltm_stack);
		ltm_stack = next;
	}
}

#define MAX (256)
static struct tree *table[MAX];

static inline unsigned int h(int val) 
{
	unsigned int ret;

	ret = val;
	ret %= MAX;
	return ret;
}

static struct tree* insert(int val, int *inserted)
{
	unsigned int i;
	struct tree *p,*s = NULL;

	i = h(val);
	p = table[i];
	while(p) {
		if(p->data == val) {
			if(inserted)
				*inserted = 0;
			return p;
		}
		s = p;
		p = p->next;
	}

	p = alloc_tree();
	p->data = val;
	if(s)
		s->next = p;
	if(!table[i])
		table[i] = p;
	if(inserted)
		*inserted = 1;

	return p;
}

int main()
{
	int i,n,j;
	struct tree *p;

	n = sizeof(data)/sizeof(data[0]);
	for(i=0; i<n; i++)
		insert(data[i], 0);
	insert(0, &i);
	if(!i)
		printf("found 0\n");
	for(i=0; i<MAX; i++) {
		j = 0;
		p = table[i];
		while(p) {
			j++;
			p = p->next;
		}
		printf(" table[%d]:  %d\n",i,j);
	}
	free_alltrees();

	return 0;
}
