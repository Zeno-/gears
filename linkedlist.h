#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#define DECLARE_AS_LIST_NODE ListNode linkage;

typedef struct listnode {
    struct listnode *prev, *next;
} ListNode;

typedef struct {
    ListNode dnode; // dummy head
    size_t len;
} List;

List *list_new(void);
List *list_init(List *list);
void list_dispose(List *list);
void list_delink_all(List *list);

int list_is_empty(const List *list);
int list_is_head(const List *list, const ListNode *currnode);
int list_is_tail(const List *list, const ListNode *currnode);

ListNode *list_head(const List *list);
ListNode *list_tail(const List *list);
ListNode *list_append(List *list, ListNode *newnode);
ListNode *list_prepend(List *list, ListNode *newnode);
ListNode *list_insert_after(List *list, ListNode *currnode, ListNode *newnode);
ListNode *list_insert_before(List *list, ListNode *currnode, ListNode *newnode);
ListNode *list_unlink_node(List *list, ListNode *currnode);
void list_delete_node(List *list, ListNode *currnode);
ListNode *list_next(const List *list, const ListNode *currnode);
ListNode *list_prev(const List *list, const ListNode *currnode);

size_t list_traverse(const List *list, void (*callback)(const ListNode *));

#endif // LINKED_LIST_H
