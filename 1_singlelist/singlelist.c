/*****************************************************************   
** Function :  单链表的增删改查
** Author   :  SubiU
** Date     :  2020.02.04
******************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef DEF_FUNC
#define DEF_FUNC(x) 0x01
#endif

typedef struct Node_t{
    int val;
    struct Node_t *next;
}Node;

typedef struct {
    Node *head;
}List;

void list_show(List *list)
{
    if(list == NULL) {
        printf("The list is NULL\n");
        return;
    }

    Node *cur = list->head;
	while(cur) {
		printf("%d ",cur->val);
//            printf("%d(%p) ",cur->val,cur);
	    cur = cur->next;
	}
	
	printf("\n\n");
}

#if DEF_FUNC("MEMORY-FREE")
#if 0
void node_free_by_level2_pointer(Node **node)
{
    if(node!=NULL && *node!=NULL) {
        free(*node);
	*node = NULL;
    }
}
void list_destroy_by_level2_pointer(List **list)
{
    Node *cur = NULL, *p = NULL;	
    if(list!=NULL && *list!=NULL) {	
    	cur = (*list)->head;
        while(cur)
        {
            p = cur->next;
            node_free_by_level2_pointer(&cur); /* 释放了链表成员,但没有将成员指针置为NULL,只是将cur置为NULL */
    	    cur = p;
    	}
	list_show(*list);    /* 此时访问链表成员会访问到随机值 */
	free(*list);
	*list = NULL;
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
void list_destroy_by_level2_pointer(List **list)
{
    Node *cur = NULL, *p = NULL;	
    if(list!=NULL && *list!=NULL) {	
    	cur = (*list)->head;
        while(cur)
        {
            cur = cur->next;
            node_free_by_level2_pointer(&((*list)->head));
    	    (*list)->head = cur;
    	}
        free(*list);
    	*list = NULL;
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
void list_destroy_by_level1_pointer(List *list)
{
    Node *cur = NULL, *p = NULL;
    if(list!=NULL) {
    	cur = list->head;
        while(cur)
        {
            p = cur->next;
            node_free(cur); /* 释放了链表成员,但没有将其置为NULL */
    	    cur = p;
    	}
	list_show(list);    /* 此时访问链表成员会访问到随机值 */

#if 0  /* 此处能够释放内存,但list是值传递,置list为NULL后,执行完该函数后函数外的list值并不会改变,可能会导致二次释放内存出错 */
		if(list!=NULL) {
		printf("[%s:%d]: list %p\n",__func__,__LINE__,list);
    		free(list);
    		list = NULL; /* list局部变量,并不会置外部相关变量为NULL */
	    }
#endif
    }	
}
#else /* 改进 */
void node_free_by_level1_pointer(Node *node)
{
    if(node!= NULL) {
        free(node);
/* 此处释放内存,但node是值传递,没能将node置为NULL,外部还能够访问到该释放内存里的随机值 */
	node = NULL;  /* node局部变量,并不会置外部相关变量为NULL */
    }
}
void list_destroy_by_level1_pointer(List *list)
{
    Node *cur = NULL, *p = NULL;
    if(list!=NULL) {
    	cur = list->head;
        while(cur)
        {
            cur = cur->next;
            node_free_by_level1_pointer(list->head); /* 释放了链表成员,但没有将其置为NULL */
    	    list->head = cur;      /* 最终此处将head置为NULL */
    	}
	list_show(list);    /* 此时访问链表成员不会访问到随机值 */

#if 0  /* 此处能够释放内存,但list是值传递,置list为NULL后,执行完该函数后函数外的list值并不会改变,函数外仍能通过list访问(但访问的内存是随机值或非法访问),可能会导致二次释放内存出错. */
		if(list!=NULL) {
		    printf("[%s:%d]: list %p\n",__func__,__LINE__,list);
    		    free(list);
    		    list = NULL; /* list局部变量,并不会置外部相关变量为NULL */
	    }
#endif
    }	
}
#endif
#endif

List *list_create(void)
{
    List *list = NULL;
    list = (List *)malloc(sizeof(List));	
    if(list)
        memset(list, 0, sizeof(List));
	
    return list;
}

Node *node_create(int val)
{
    Node *node = NULL;
    node = (Node *)malloc(sizeof(Node));
    if(node) {
        node->val = val;
	node->next = NULL;
    }
	
    return node;
}

void list_head_add(List *list, int val)
{
    Node *head = NULL;
	
    if(list==NULL)
        return;

    head = node_create(val);
    if(head==NULL)
        return;

    head->next = list->head;
    list->head = head;
}

void list_tail_add(List *list, int val)
{
    Node *tail = NULL;
    Node *pre = list->head;
    Node *cur = list->head;
	
    if(list==NULL)
        return;

    tail = node_create(val);
    if(tail==NULL)
	return;

    while(cur) {
        pre = cur;
	cur = cur->next;
    }
    
    if(pre)
        pre->next = tail;
    else
        list->head = tail;
}

short list_index_add(List *list, int index, int val)
{
    Node *node = NULL;
    Node *pre = NULL;
    Node *cur = list->head;
    int count = 0;
	
    if(list==NULL)
        return -1;

    if(index == 0) {
        list_head_add(list, val);
	return 0;
     }
			
    while(cur) {
	if(index == count) {
	    node = node_create(val);
	    if(node==NULL)
		return -1;
	    node->next = cur;
	    pre->next = node;
	    return 0;
	}
        pre = cur;
	cur = cur->next;
	count++;
    }

    if(count == index) {
        list_tail_add(list, val);
        return 0;
    }
    else {
	printf("list_index_add failed\n");
        return -1;
    }
}

void list_index_del(List *list, int index)
{
    Node *node = NULL;
    Node *pre = NULL;
    Node *cur = NULL;
    int count = 0;

    if(list==NULL)
       return;

    cur = list->head;	
    if(index == 0) {
		cur = cur->next;
//        node_free_by_level1_pointer(list->head);
	    node_free_by_level2_pointer(&(list->head));
        list->head = cur; 
        return;
    }
			
    while(cur) {
	    if(index == count) {
            pre->next = cur->next;
//    	    node_free_by_level1_pointer(cur);
            node_free_by_level2_pointer(&cur);
    	    return;
	}
	pre = cur;
	cur = cur->next;
	count++;
    }

    printf("list_index_del failed\n");
}

int main(void)
{
    int ret = 0;
    List *list = list_create();
    if(list == NULL)
	return -1;

    list_head_add(list, 6);
    list_head_add(list, 7);
    list_head_add(list, 4);

    list_tail_add(list, 5);
    list_tail_add(list, 3);
    list_tail_add(list, 2);
    list_index_add(list, 0, 1);
    list_index_add(list, 1, 2);
    list_show(list);         /* 1 2 4 7 6 5 3 2 */
    list_index_add(list, 7, 7);
    list_show(list);         /* 1 2 4 7 6 5 3 7 2 */
    list_index_add(list, 9, 9);
    list_show(list);         /* 1 2 4 7 6 5 3 7 2 9 */
	
    list_index_del(list, 0); 
    list_show(list);    	 /* 2 4 7 6 5 3 7 2 9 */
    list_index_del(list, 6); 
    list_show(list);         /* 2 4 7 6 5 3 2 9 */
    list_index_del(list, 1); 
    list_show(list);         /* 2 7 6 5 3 2 9 */

#if 1
    list_destroy_by_level2_pointer(&list);
    list_show(list);
#else /* or */
    list_destroy_by_level1_pointer(list);
    if(list!=NULL) {
	free(list);
	list = NULL;
    }
    list_show(list);
#endif

    return 0;
}
