/*****************************************************************   
** Function :  单链表的增删改查
** Author   :  SubiU
** Date     :  2020.02.04
******************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node_t {
    int val;
	struct node_t *next;
}NODE;

/** Initialize your data structure here. */
NODE* node_create(int val) {
    NODE *node = NULL;
	node = (NODE *)malloc(sizeof(NODE));
    if(node != NULL) {
		node->val = val;
        node->next = NULL;
    }
    
    return node;
}

void node_free(NODE **node)
{
    if(node!=NULL && *node!=NULL) {
        free(*node);
		*node = NULL;
	}
}

void list_destroy(NODE **head)
{
    NODE *cur = NULL;
	if(head!=NULL && *head!=NULL) {
        cur = *head;
		while(cur) {
            cur = cur->next;
			node_free(head);
		    *head = cur;
		}		
	}
}

/** Get the value of the index-th node in the linked list. If the index is invalid, return -1. */
int list_get_index(NODE *head, int index)
{
    NODE *cur = NULL;
	NODE *pre = NULL;
	int count = 0;
	
    if(head==NULL)
		return -1;

    cur = head;
	while(cur) {
        if(count == index)
			return cur->val;
		cur = cur->next;
		count++;
	}
	if(cur==NULL)
		return -1;
}

/** Add a node of value val before the first element of the linked list. After the insertion, the new node will be the first node of the linked list. */
int link_head_add(NODE **head, int val)
{
    NODE *node = NULL;
    if(head == NULL)
		return -1;

	node = node_create(val);
	if(node == NULL)
		return -1;

    node->next = *head;
	*head = node;
	
	return 0;
}

/** Append a node of value val to the last element of the linked list. */
int link_tail_add(NODE **head, int val)
{
    NODE *node = NULL;
    NODE *cur = NULL;

    if(head == NULL)
		return -1;

	node = node_create(val);
	if(node == NULL)
		return -1;
	
	cur = *head;
	while(cur->next)
        cur = cur->next;
	cur->next = node;

	return 0;
}

/** Add a node of value val before the index-th node in the linked list. If index equals to the length of linked list, the node will be appended to the end of linked list. If index is greater than the length, the node will not be inserted. */
int link_add_at_index(NODE **head, int index, int val)
{
    NODE *node = NULL;
	NODE *pre = NULL;
	NODE *cur = NULL;
	int count = 0;
	
	if(head==NULL || *head==NULL)
		return -1;

	if(index == 0)
		link_head_add(head, val);
	else{
		cur = *head;
        while(cur) {
            if(count == index) {
                node = node_create(val);
				if(node == NULL)
					return -1;
				pre->next = node;
				node->next = cur;
			    return 0;
			}
            pre = cur;
			cur = cur->next;
			count++;
		}
	}
	if(index == count)
		link_tail_add(head, val);

	return 0;
}

/** Delete the index-th node in the linked list, if the index is valid. */
int link_delete_at_index(NODE **head, int index)
{
    NODE *pre = NULL;
    NODE *cur = NULL;
	int count = 0;

	if(head == NULL || *head==NULL)
		return -1;

    cur = *head;
	pre = *head;
    if(index == 0) {
        cur = cur->next;
	    node_free(head);
		*head = cur;
	}
	else {
    	while(cur) {
            if(count == index) {
    			pre->next = cur->next;
				node_free(&cur);
    			break;
    		}
    		pre = cur;
    		cur = cur->next;
    		count++;
    	}
    
        if(cur == NULL)
    		return -1;
    }

	return 0;
}

void list_show(NODE *head)
{
    NODE *cur = NULL;
	if(head==NULL) {
        printf("The list is NULL.\n");
		return;
    }

	cur = head;
	while(cur) {
        printf("%d(%p) ",cur->val,cur);
		cur = cur->next;
	}
	printf("\n");
}

int main(void)
{
    int val = 0;
    NODE *head = NULL;

    link_head_add(&head, 7);
    link_head_add(&head, 2);
    link_head_add(&head, 1);
    link_add_at_index(&head, 3, 0);
	link_delete_at_index(&head, 2);	
	
    link_head_add(&head, 6);	
    link_tail_add(&head, 4);      
	list_show(head);         /* 6 1 2 0 4 */
	val = list_get_index(head, 4);  
    printf("listGet index(4): %d\r\n",val); /* 4 */
    link_head_add(&head, 4);
    link_add_at_index(&head, 5, 0);
    link_head_add(&head, 6);
	list_show(head);        /* 6 4 6 1 2 0 0 4 */
	
	list_destroy(&head);
	list_show(head);

    return 0;
}
