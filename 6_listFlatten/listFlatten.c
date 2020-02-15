/*****************************************************************   
** Function : 双向链表操作. 
**            扁平化多级双向链表.
******************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEF_FUNC(x) 0x01
#define DEF_FUNC_CANCEL(x) 0x00

#define OUTPUT 0	
#define printf_f(x) { if(OUTPUT) { printf("[%s:%d]: ",__func__,__LINE__); printf x;} }

typedef struct Node_t{
    int val;
    struct Node_t *prev;
    struct Node_t *next;
	struct Node_t *child;
}Node;

void list_show(Node *head)
{
    Node *cur = head;
	if(head==NULL)
		printf("The list is NULL.\n");
	while(cur) {
        printf("%d ",cur->val);
	    cur = cur->next;
	}
	
	printf("\n\n");
}

Node *node_create(int val)
{
    Node *node = NULL;
    node = (Node *)malloc(sizeof(Node));
    if(node) {
        node->val = val;
		node->prev = NULL;
		node->next = NULL;
    }
	
    return node;
}

#if 0 /* 一级指针操作链表 */
Node* list_head_add(Node *head, int val)
{	
	Node *node = NULL;
	
    node = node_create(val);
    if(node==NULL)
        return NULL;

    if(head!=NULL) {
	    node->next = head;
		head->prev = node;
	    head = node;
    }else {
		head = node;
	}
	
	return head;
}
#else /* 二级指针操作链表 */
void list_head_add(Node **head, int val)
{	
	Node *node = NULL;
	if(head==NULL)
		return;
	
    node = node_create(val);
    if(node==NULL)
        return;
	
    if((*head)!=NULL) {
	    node->next = (*head);
		(*head)->prev = node;
	    *head = node;
    }else {
		*head = node;
	}	
}
#endif

#if DEF_FUNC("MEMORY-FREE")
#if 0
void node_free_by_level2_pointer(Node **node)
{
    if(node!=NULL && *node!=NULL) {
        free(*node);
	*node = NULL;
    }
}
void list_destroy_by_level2_pointer(Node **head)
{
    Node *cur = NULL, *p = NULL;	
    if(head!=NULL && *head!=NULL) {	
    	cur = *head;
        while(cur)
        {
            p = cur->next;
            node_free_by_level2_pointer(&cur); /* 释放了链表成员,但没有将成员指针置为NULL,只是将cur置为NULL */
    	    cur = p;
    	}
	list_show(*head);    /* 此时访问链表成员会访问到随机值 */
    }
}
#else /* 改进 */
void node_free_by_level2_pointer(Node **node)
{
    if(node!=NULL && *node!=NULL) {	
        free(*node);
	    *node = NULL;
    }
}
void list_destroy_by_level2_pointer(Node **head)
{
    Node *cur = NULL, *p = NULL;	
    if(head!=NULL && *head!=NULL) {	
    	cur = (*head);
        while(cur)
        {
            cur = cur->next;
            node_free_by_level2_pointer(head);
    	    *head = cur;
    	}
    }  
}
#endif

#if 0
void node_free_by_level1_pointer(Node *node)
{
    if(node!= NULL) {
        free(node);
/* 此处释放内存,但node是值传递,没能将node置为NULL,外部还能够访问到该释放内存里的随机值 */
	node = NULL;  /* node局部变量,并不会置外部相关变量为NULL */
    }
}
Node* list_destroy_by_level1_pointer(Node *head)
{
    Node *cur = NULL, *p = NULL;
    if(head!=NULL) {
    	cur = head;
        while(cur)
        {
            p = cur->next;
            node_free_by_level1_pointer(cur); /* 释放了链表成员,但没有将其置为NULL */
    	    cur = p;
    	}
	list_show(head);    /* 此时访问链表成员会访问到随机值 */
    }	
	return head;
}
#else /* 改进 */
void node_free_by_level1_pointer(Node *node)
{
    if(node!= NULL) {
        free(node);
	node = NULL;
    }
}

Node* list_destroy_by_level1_pointer(Node *head)
{
    Node *cur = NULL, *p = NULL;
    if(head!=NULL) {
    	cur = head;
        while(cur)
        {
            cur = cur->next;
            node_free_by_level1_pointer(head); /* 释放了链表成员,但没有将其置为NULL */
    	    head = cur;      /* 最终此处将head置为NULL */
    	}
		list_show(head);    /* 此时访问链表成员不会访问到随机值 */
    }
	return head;
}
#endif
#endif

#if 0
Node* list_tail_add(Node *head, int val)
{
    Node *tail = NULL;
    Node *cur = NULL;
	
    tail = node_create(val);
    if(tail==NULL)
		return head;

    if(head==NULL)
		return tail;
    cur = head;
    while(cur->next)
		cur = cur->next;
    
    cur->next = tail;
    tail->prev = cur;

	return head;
}
#else
void list_tail_add(Node **head, int val)
{
    Node *tail = NULL;
    Node *cur = NULL;
	
    tail = node_create(val);
    if(tail==NULL)
		return;

    if(head==NULL || *head==NULL) {
        *head = tail;
		return;
	}
    cur = *head;
    while(cur->next)
		cur = cur->next;
    
    cur->next = tail;
    tail->prev = cur;
}
#endif

#if 0
Node* list_index_add(Node *head, int index, int val)
{
    Node *node = NULL;
    Node *pre = NULL;
    Node *cur = NULL;
    int count = 0;
	
    if(index == 0) {
        head = list_head_add(head, val);
		return head;
    }
    cur = head;	
    while(cur) {
	if(index == count) {
	    node = node_create(val);
	    if(node==NULL)
			return head;
		node->prev = cur->prev;
		node->next = cur;
		cur->prev->next = node;
		cur->prev = node;

	    return head;
	}
	cur = cur->next;
	count++;
    }

    if(count == index) {
        head = list_tail_add(head, val);
        return head;
    }
    else {
	printf("list_index_add failed\n");
        return head;
    }
}
#else
void list_index_add(Node **head, int index, int val)
{
    Node *node = NULL;
    Node *pre = NULL;
    Node *cur = NULL;
    int count = 0;

	if(head==NULL)
			return;
	
    if(index == 0) {
        list_head_add(head, val);
		return;
    }
    cur = *head;	
    while(cur) {
	if(index == count) {
	    node = node_create(val);
	    if(node==NULL)
			return;
		node->prev = cur->prev;
		node->next = cur;
		cur->prev->next = node;
		cur->prev = node;
	    return;
	}
	cur = cur->next;
	count++;
    }

    if(count == index) {
        list_tail_add(head, val);
    }
    else {
	printf("list_index_add failed\n");
    }
}
#endif

#if 0
Node* list_index_del(Node *head, int index)
{
    Node *node = NULL;
    Node *cur = NULL;
    int count = 0;

    if(head==NULL)
       return head;

    cur = head;	
    if(index == 0) {
		cur = cur->next;
//        node_free_by_level1_pointer(list->head);
	    node_free_by_level2_pointer(&head);
		cur->prev = NULL;
        head = cur; 
        return head;
    }
			
    while(cur) {
	    if(index == count) {
			if(cur->next != NULL) {
				cur->prev->next = cur->next;
	            cur->next->prev = cur->prev;
			}else {
				cur->prev->next = NULL;
			}				
//    	    node_free_by_level1_pointer(cur);
            node_free_by_level2_pointer(&cur);
    	    return head;
		}
		cur = cur->next;
		count++;		
    }

    printf("list_index_del failed\n");

	return head;
}
#else
void list_index_del(Node **head, int index)
{
    Node *node = NULL;
    Node *cur = NULL;
    int count = 0;

    if(head==NULL || *head==NULL)
       return;

    cur = *head;	
    if(index == 0) {
		cur = cur->next;
//        node_free_by_level1_pointer(list->head);
	    node_free_by_level2_pointer(head);
		cur->prev = NULL;
        *head = cur; 
        return;
    }
			
    while(cur) {
	    if(index == count) {
			if(cur->next != NULL) {
				cur->prev->next = cur->next;
	            cur->next->prev = cur->prev;
			}else {
				cur->prev->next = NULL;
			}				
//    	    node_free_by_level1_pointer(cur);
            node_free_by_level2_pointer(&cur);
    	    return;
		}
		cur = cur->next;
		count++;		
    }

    printf("list_index_del failed\n");
}
#endif

int list_get_by_index(Node *head, int index) {
    Node *cur = NULL;
    int count = 0;
    if(head == NULL)
        return -1;
    
    cur = head;
    while(cur)
    {
        if(count == index)
            return cur->val;
        cur = cur->next;
        count++;
    }

    return -1;
}

void list_index_child_add(Node *head, int index, Node *child)
{
	Node *cur = NULL;
	int count = 0;

	if(index==0) {
		head->child = child;
		return;
	}

	cur = head;
	while(cur) {
		if(count==index) {
			cur->child = child;
			return;
		}
		cur = cur->next;
		count++;
	}
	if(index>count)
		printf("list_index_child_add failed.\n");
}

Node* list_node_has_child_find(Node *head)
{
	Node *cur = NULL;
	
	if(head==NULL)
		return NULL;
	cur = head;
	while(cur) {
		if(cur->child!=NULL) {
			return cur;
		}
		cur = cur->next;
	}
	return NULL;
}

Node* list_flatten(Node *head)
{
	Node *cur = NULL;
	Node *prev = NULL;
	Node *tmp = NULL;
	Node *child = NULL;
	Node *childEnd = NULL;
	
    if(head==NULL)
		return NULL;

	cur = list_node_has_child_find(head);
	if(cur==NULL) {
		printf_f(("cur==NULL\n"));
		return head;
	}
    else {
		prev = cur;
		tmp = cur->next;
		child = cur->child;		
		printf_f(("tmp(%d) child(%d) cur(%d)\n",tmp->val,child->val,cur->val));
		cur = list_flatten(child);		
		printf_f(("tmp(%d) child(%d) cur(%d)\n",tmp->val,child->val,cur->val));
		if(cur==child) {
			childEnd = child;
			while(childEnd->next)
				childEnd = childEnd->next;			
			printf_f(("childEnd(%d)\n",childEnd->val));
			child->prev = prev;
			childEnd->next = tmp;
			prev->child = NULL;
			prev->next = child;
			if(tmp!=NULL)
				tmp->prev = childEnd;
		}
	}
	
	return head;
}

int main(void)
{
    int ret = 0;
    Node *head = NULL;
	Node *child1 = NULL;
	Node *child2 = NULL;

#if DEF_FUNC_CANCEL("双向链表操作")

#if DEF_FUNC_CANCEL("一级指针操作链表")
    head = list_head_add(head, 6);
    head = list_head_add(head, 7);
    head = list_head_add(head, 4);

    list_show(head);        /* 4 7 6 */

    head = list_tail_add(head, 5);
    head = list_tail_add(head, 3);
    head = list_tail_add(head, 2);
    list_show(head);                  /* 4 7 6 5 3 2 */
    head = list_index_add(head, 0, 1);
    head = list_index_add(head, 1, 2);
    list_show(head);                  /* 1 2 4 7 6 5 3 2 */
	
    head = list_index_del(head, 0);
    list_show(head);         /* 2 4 7 6 5 3 2 */
    head = list_index_del(head, 6);
    list_show(head);         /* 2 4 7 6 5 3 */
    head = list_index_del(head, 1); 
    list_show(head);         /* 2 7 6 5 3 */
    head = list_index_add(head, 5, 5);
    list_show(head);         /* 2 7 6 5 3 5 */
#endif

#if DEF_FUNC_CANCEL("二级指针操作链表") 
    list_head_add(&head, 6);
    list_head_add(&head, 7);
    list_head_add(&head, 4);

    list_show(head);        /* 4 7 6 */

    list_tail_add(&head, 5);
    list_tail_add(&head, 3);
    list_tail_add(&head, 2);
    list_show(head);                  /* 4 7 6 5 3 2 */
    list_index_add(&head, 0, 1);
    list_index_add(&head, 1, 2);
    list_show(head);                  /* 1 2 4 7 6 5 3 2 */
	
    list_index_del(&head, 0);
    list_show(head);         /* 2 4 7 6 5 3 2 */
    list_index_del(&head, 6);
    list_show(head);         /* 2 4 7 6 5 3 */
    list_index_del(&head, 1); 
    list_show(head);         /* 2 7 6 5 3 */
    list_index_add(&head, 5, 5);
    list_show(head);         /* 2 7 6 5 3 5 */
#endif

#if 1
    list_destroy_by_level2_pointer(&head);
    list_show(head);
#else /* or */
    head = list_destroy_by_level1_pointer(head);
    list_show(head);
#endif
#endif

#if DEF_FUNC("扁平化多级双向链表")
/*******************************************************
	1--2--3--4--5--6--NULL
		  |
		  7--8--9--10--NULL
		  	 |
		  	 11--12--NULL

	1-2-3-7-8-11-12-9-10-4-5-6-NULL
*******************************************************/
    list_head_add(&head, 1);
    list_index_add(&head, 1, 2);	
    list_index_add(&head, 2, 3);
    list_index_add(&head, 3, 4);
    list_index_add(&head, 4, 5);
    list_index_add(&head, 5, 6);
	printf("list:\n");
    list_show(head);              /* 1 2 3 4 5 6 */

    list_head_add(&child1, 7);
    list_index_add(&child1, 1, 8);	
    list_index_add(&child1, 2, 9);
    list_index_add(&child1, 3, 10);
	printf("child1(child of list):\n");
    list_show(child1);            /* 7 8 9 10 */

    list_head_add(&child2, 11);
    list_index_add(&child2, 1, 12); 
	printf("child2(child of child1):\n");
	list_show(child2);              /* 11 12 */

	list_index_child_add(head, 2, child1);
	list_index_child_add(child1, 1, child2);

	head = list_flatten(head);
	printf("flatten:\n");
	list_show(head);   /* 1 2 3 7 8 11 12 9 10 4 5 6 */
#endif

    return 0;
}
