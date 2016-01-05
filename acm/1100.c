#include <stdio.h>
#include <sdtlib.h>
#include <string.h>

#define OPERATOR_PLUS (0)
#define OPERATOR_MINUS (1)
#define OPERATOR_MULTI (2)
#define OPERATOR_NUM (3)

struct node {
	struct node *left,*right;
	unsigned int val;
};

int main()
{

}
