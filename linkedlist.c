/*! @file
 */

#include <stdlib.h>
#include "linkedlist.h"

/*! \brief Allocate a new object of type \ref List and initialise it to an empty
 * list.
 *
 * \returns a pointer to the newly created list, or NULL if an error occurred.
 */
List *list_new(void)
{
	List *list;

	list = malloc(sizeof *list);
	return list ? list_init(list) : NULL;
}


/*! \brief Initialise an object of type \ref List so that it is an empty list.
 *
 * If \ref list_new is used to create the list, then calling this function is
 * not necessary. A usage for the function is where the list is not created
 * using \ref list_new but, for example, as a local or global object.
 *
 * \note This function does not check if \p is already or initalised or not.
 * Calling this function on a list that already has nodes associated with it
 * may lead to memory leaks.
 *
 * \returns \p list
 *
 * \pre list != NULL
 */
List *list_init(List *list)
{
	// list is doubly-linked and circular
	list->dnode.next = list->dnode.prev = &list->dnode;
	list->len = 0;

	return list;
}


/*! \brief Free all memory and nodes allocated and members of \p list
 *
 * Frees memory allocated for all nodes that are in the list (which must have
 * their memory allocated using malloc. After the nodes themselves have had
 * their memory release, the list pointed to by \p list is also released.
 *
 * \note if \p list was not created using \ref list_new then calling this
 * function results in undefined behaviour. Similarly if the nodes within the
 * list were not all created using malloc the behaviour is undefined.
 *
 * \pre list != NULL
 *
 * \sa list_delink_all
 */
void list_dispose(List *list)
{
	ListNode *cnode = list->dnode.next;

	while (cnode != &list->dnode) {
		ListNode *next = cnode->next;
		free(cnode);
		cnode = next;
	}

	free(list);
}


/*! \brief Unlinks all nodes in \p list so that they no longer belong to the list.
 *
 * The rationale for this function is so that a list created with
 * \ref list_init can contain nodes that were not allocated using malloc.
 * Unlike \ref list_dispose, this function does not free the memory for the
 * nodes. This is so that nodes in the list can be, for example, in a global
 * or static array (or other method where the lifetime of the nodes exist for
 * the same lifetime as the list).
 *
 * This function also, unlike \ref list_dispose does not attempt to free the
 * memory for \list itself (meaning that list need not be dynamically
 * allocated either). If \p list \e is dynamically allocated then releasing
 * its memory is the respsonsibility of the program that allocated the list.
 *
 * \pre list != NULL
 */
void list_delink_all(List *list)
{
	ListNode *cnode = list->dnode.next;

	while (cnode != &list->dnode) {
		ListNode *next = cnode->next;
		list_unlink_node(list, cnode);
		cnode = next;
	}

	list->len = 0;
}


int list_is_empty(const List *list)
{
	return list->dnode.next == &list->dnode;
}

int list_is_head(const List *list, const ListNode *node)
{
	return list->dnode.next == node;
}

int list_is_tail(const List *list, const ListNode *node)
{
	return list->dnode.prev == node;
}

int list_contains_node(const List *list, const ListNode *currnode)
{
	ListNode *cnode = list->dnode.next;

	while (cnode != &list->dnode) {
		if (cnode == currnode)
			break;
	}
	return cnode != &list->dnode;
}

ListNode *list_head(const List *list)
{
	return list_is_empty(list) ? NULL : list->dnode.next;
}

ListNode *list_tail(const List *list)
{
	return list_is_empty(list) ? NULL : list->dnode.prev;
}

ListNode *list_append(List *list, ListNode *newnode)
{
	return list_insert_after(list, list->dnode.prev, newnode);
}

ListNode *list_prepend(List *list, ListNode *newnode)
{
	return list_insert_before(list, list->dnode.next, newnode);
}

ListNode *list_insert_after(List *list, ListNode *currnode, ListNode *newnode)
{
	newnode->next = currnode->next;
	newnode->prev = currnode;
	newnode->next->prev = newnode;
	newnode->prev->next = newnode;

	list->len++;

	return currnode;
}

ListNode *list_insert_before(List *list, ListNode *currnode, ListNode *newnode)
{
	newnode->next = currnode;
	newnode->prev = currnode->prev;
	newnode->next->prev = newnode;
	newnode->prev->next = newnode;

	list->len++;

	return currnode;
}

ListNode *list_unlink_node(List *list, ListNode *currnode)
{
	if (currnode == &list->dnode)
		return NULL;

	// unlink
	currnode->prev->next = currnode->next;
	currnode->next->prev = currnode->prev;
	list->len--;

	return currnode;
}

void list_delete_node(List *list, ListNode *currnode)
{
	if (currnode == &list->dnode)
		return;

	// unlink
	currnode->prev->next = currnode->next;
	currnode->next->prev = currnode->prev;
	list->len--;

	free(currnode);
}

ListNode *list_next(const List *list, const ListNode *currnode)
{
	ListNode *node = currnode->next;
	return node == &list->dnode ? NULL : node;
}

ListNode *list_prev(const List *list, const ListNode *currnode)
{
	ListNode *node = currnode->prev;
	return node == &list->dnode ? NULL : node;
}


size_t list_traverse(const List *list, void (*callback)(const ListNode *))
{
	ListNode *cnode = list->dnode.next;
	size_t i = 0;

	while (cnode != &list->dnode) {
		callback(cnode);
		i++;
		cnode = cnode->next;
	}

	return i;
}
