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

static struct tree *top;
static struct tree* insert_and_search(int k)
{
	int key;
	struct tree *p,*target = NULL;
	
	p = top;
	while(1) {
		key = p->key;
		if(k < key) {
			if(!p->left) {
				target = alloc_tree();
				target->key = k;
				p->left = target;
				break;
			} else {
				p = p->left;
				continue;
			}
		} else if(k > key) {
			if(!p->right) {
				target = alloc_tree();
				target->key = k;
				p->right = target;
				break;
			} else {
				p = p->right;
				continue;
			}
		} else {
			target = p;
			printf("  k:%d found\n",k);
			break;
		}
	}

	return target;
}

static void delete_one(struct tree *p)
{
	struct tree *f,*r;

	if(!p->right) {
		if(p->left) {
			f = p->left;
			memcpy(p, f, sizeof(*p));
		} else {
		}
	} else if(!p->left) {
		f = p->right;
		memcpy(p, f, sizeof(*p));
	} else {
		r = p->right;
	}
	free_tree(p);
}

int main()
{
	int i,n;
	struct tree *p;

	top = alloc_tree();
	top->key = 0;
	n = sizeof(data)/sizeof(data[0]);
	stack = malloc(n * sizeof(*stack));

	for(i=0; i<n; i++)
		insert_and_search(data[i]);

	p = top;i = 0;
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
