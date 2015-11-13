#include <stdlib.h>
#include "linkedlist.h"

List *list_new(void)
{
    List *list;

    list = malloc(sizeof *list);
    return list ? list_init(list) : NULL;
}

List *list_init(List *list)
{
    // list is doubly-linked and circular
    list->dnode.next = list->dnode.prev = &list->dnode;
    list->len = 0;

    return list;
}

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

void list_delink_all(List *list)
{
	ListNode *cnode = list->dnode.next;

    while (cnode != &list->dnode) {
        ListNode *next = cnode->next;
        list_unlink_node(list, cnode);
        cnode = next;
    }
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

int list_is_node_present(const List *list, const ListNode *currnode)
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
