#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "linkedlist.h"

struct intnode {
	DECLARE_AS_LIST_NODE
	int v;
};

void print_intnode(const ListNode *node)
{
	struct intnode *cn = (struct intnode *)node;
	printf("%d\n", cn->v);
}

int main(void)
{
	List *mylist = list_new();
	ListNode *cnode;
	int i;
	int r = 0;

	if (!mylist)
		return 1;

#if 1
	for (i = 0; i < 15; i++) {
		struct intnode *newnode = malloc(sizeof(*newnode));
		if (!newnode) {
			r = 1;
			goto mallocerror;
		}
		newnode->v = i;
		list_prepend(mylist, (ListNode *)newnode);
		assert(list_is_node_present(mylist, (ListNode *)newnode));
	}


#else
	for (i = 0; i < 5; i++) {
		struct intnode *newnode = malloc(sizeof(*newnode));
		if (!newnode) {
			r = 1;
			goto mallocerror;
		}
		newnode->v = i;
		list_append(mylist, (ListNode *)newnode);
	}


#endif

	printf("List length: %lu\n", (unsigned long)mylist->len);

#if 0
	for (cnode = list_head(mylist); cnode != NULL; cnode = list_next(mylist, cnode)) {
		printf("%d\n", ((struct intnode *)cnode)->v);
	}
#else
	list_traverse(mylist, print_intnode);
#endif

mallocerror:

	list_dispose(mylist);

	return r;
}

