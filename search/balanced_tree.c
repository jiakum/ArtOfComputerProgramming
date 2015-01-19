#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "data.c"

struct tree{
	struct tree *left,*right;
	int key,B;
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

#define LINK(a,s) (((a)>0)?(s->right):(s->left))
static struct tree *top;
static struct tree* insert_and_search(int k)
{
	int key,a,tmp;
	struct tree *t,*r,*s,*p,*q;
	
	t = top;s = top->right;p = top->right;
	while(1) {
		key = p->key;
		if(k < key) {
			q = p->left;
			if(!q) {
				q = alloc_tree();
				p->left = q;
				break;
			} else {
				if(q->B) {
					t = p; 
					s = q;
				}
				p = q;
				continue;
			}
		} else if(k > key) {
			q = p->right;
			if(!q) {
				q = alloc_tree();
				p->right = q;
				break;
			} else {
				if(q->B) {
					t = p; 
					s = q;
				}
				p = q;
				continue;
			}
		} else {
			printf("found :%d\n",k);
			return p;
		}
	}

	q->key = k;
	q->B = 0;
	if(k < s->key)
		a = -1;
	else
		a = 1;
	p = LINK(a,s); r = p;
	while(p!=q) {
		if(k < p->key) {
			p->B = -1;
			p = p->left;
		} else if(k > p->key) {
			p->B = 1;
			p = p->right;
		}
	}

	if(!s->B) {
		s->B = a;
		top->B++;
	} else if(s->B == -a) {
		s->B = 0;
		return q;
	} else {
		if(r->B == a) {
			p = r;
			if(a<0) {
				s->left = r->right;
				r->right = s;
			} else {
				s->right = r->left;
				r->left = s;
			} 
			s->B = 0; r->B = 0;
		} else if(r->B == -a) {
			if(a<0) {
				p = r->right;
				r->right = p->left;
				p->left = r;
				s->left = p->right;
				p->right = s;
			} else {
				p = r->left;
				r->left = p->right;
				p->right = r;
				s->right = p->left;
				p->left = s;
			}

			if(p->B == a) {
				s->B = -a;
				r->B = 0;
			} else if(p->B == 0) {
				s->B = 0;
				r->B = 0;
			} else {
				s->B = 0;
				r->B = a;
			}
			p->B = 0;
		}
		if(s == t->right)
			t->right = p;
		else
			t->left = p;
	}

}

int main()
{
	int i,n;
	struct tree *p;

	top = alloc_tree();
	top->key = 0x7FFFFFFF;
	p = alloc_tree();
	p->key = 0;
	p->B = 0;
	top->right = p;
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
			printf("     key:  %d,B:  %d\n",p->key,p->B);
			p = p->right;
		}
	}

	free_alltrees();

	return 0;
}
