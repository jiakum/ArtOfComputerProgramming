#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "data.c"

struct tree{
	struct tree *left,*right;
	int key;
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

struct {
	void *p;
} *stack;
static int ptr = 0;
#define PUSH(p) 	do { \
						stack[ptr].p = p;\
						ptr++;\
					}while(0);
static int inline PULL(void **p) {
	if(ptr) {
		ptr--;
		*p = stack[ptr].p;
		return 1; // success
	}
	return 0;
}

int main()
{
	int i,n,k,key;
	struct tree *top,*p,*tmp;

	top = alloc_tree();
	stack = malloc(n * sizeof(*stack));
	top->key = 0;

	n = sizeof(data)/sizeof(data[0]);
	for(i=0; i<n; i++) {
		k = data[i];
		p = top;
		while(1) {
			key = p->key;
			if(k < key) {
				if(!p->left) {
					tmp = alloc_tree();
					tmp->key = k;
					p->left = tmp;
					break;
				} else {
					p = p->left;
					continue;
				}
			} else if(k > key) {
				if(!p->right) {
					tmp = alloc_tree();
					tmp->key = k;
					p->right = tmp;
					break;
				} else {
					p = p->right;
					continue;
				}
			} else {
				printf("  k:%d found\n",k);
				break;
			}
		}
	}

	p = top;
	while(1) {
		if(p) {
			PUSH(p);
			p = p->left;
			continue;
		} else {
			if(!PULL((void **)&p))
				break;
			printf("     %d\n",p->key);
			p = p->right;
		}
	}

	free_alltrees();

	return 0;
}
