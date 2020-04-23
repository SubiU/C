/*****************************************************************   
** Function :  单向链表操作 
**             删除链表的倒数第n个节点(一次遍历).
**             反转链表、奇偶链表、回文链表、旋转链表
******************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEF_FUNC(x) 0x01
#define DEF_FUNC_CANCEL(x) 0x00

#define OUTPUT 0	
#define printf_f(x) { if(OUTPUT) { printf("[%s:%d]: ",__func__,__LINE__); printf x;} }

enum {
	FALSE = 0,
	TRUE =1
};

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
//      printf("%d(%p) ",cur->val,cur);
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
#endif

#if DEF_FUNC("LeetCode_19. 删除链表的倒数第n个节点(一次遍历)")
#if 1
void list_remove_nth_from_end(List *list, int n)
{
	Node *cur = NULL;
	Node *p = NULL;
	Node *pre = NULL;
	
	if(list==NULL)
		return;
	
	printf("list remove %dth-node from end:\n",n);
	cur = p = pre = list->head;
	while(n>0) {
		cur = cur->next;
	    n--;
	}

	while(cur) {
		pre = p;
		p = p->next;
    	cur = cur->next;
	}
	if(p!=NULL) {
		if(p==list->head) {
            cur = p->next;
			node_free_by_level2_pointer(&(list->head));
		    list->head = cur;
		}else {
    		pre->next = p->next;
    	    node_free_by_level2_pointer(&p);
		}
	}else {
		printf("list_remove_nth_from_end failed.\n");
	}
}
#else  /* 使用虚拟链表头节点 */
void list_remove_nth_from_end(List *list, int n)
{
	Node *cur = NULL;
	Node *p = NULL;
	Node *pre = NULL;
	Node *virtualHead = NULL;
	
	if(list==NULL)
		return;
	
	printf("list remove %dth-node from end:\n",n);
	virtualHead = (Node *)malloc(sizeof(Node));
	if(virtualHead==NULL)
		return;
	memset(virtualHead, 0, sizeof(Node));
	virtualHead->next = list->head;
	list->head = virtualHead;
	
	cur = p = pre = list->head;
	while(n>0) {
		cur = cur->next;
	    n--;
	}

	while(cur) {
		pre = p;
		p = p->next;
    	cur = cur->next;
	}
	if(p!=NULL) {
		pre->next = p->next;
	    node_free_by_level2_pointer(&p);
	}else {
		printf("list_remove_nth_from_end failed.\n");
	}

	cur = list->head->next;
	free(list->head);
	list->head = cur;
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

#if DEF_FUNC("获取链表的倒数第n个节点(一次遍历)")
Node* node_kth_to_tail(List *list, unsigned int k)
{
	if(list==NULL || list->head==NULL || k==0)
		return NULL;
	Node *p1 = list->head, *p2 = list->head;
	unsigned int i = 0;
	for(i=0; i<k-1; i++) {
		if(p1->next != NULL) {
			p1 = p1->next;
		}
		else {
			return NULL;
		}
	}

	while(p1->next != NULL) {
		p1 = p1->next;
		p2 = p2->next;
	}

	return p2;
}
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

void list_reverse_iteration(List *list)
{
	Node *cur = NULL;
	Node *next = NULL;
    if(list==NULL)
		return;

	cur = list->head->next;
	list->head->next = NULL;
	while(cur) {
		next = cur->next;
		cur->next = list->head;
		list->head = cur;
		cur = next;
	}
}

Node* list_reverse_recursive(Node *head)
{
	Node *headNew = NULL;

	if(head==NULL || head->next==NULL)
		return head;
	headNew = list_reverse_recursive(head->next);
	head->next->next = head;
	head->next = NULL;
	
	return headNew;
}

Node* list_odd_even(Node* head)
{
    Node *cur = NULL, *p = NULL, *q = NULL;
    
    if(head==NULL)
        return NULL;
    cur = p = q = head;
    while(p && p->next && p->next->next) {
        p = p->next;
        q = p->next;
        p->next = q->next;
        q->next = cur->next;
        cur->next = q;
        cur = q;
    }
    return head;
}

int list_is_palindrome(List *list)
{
	Node *cur = NULL;
	Node *headNew = NULL;
	int count = 0, median = 0, index = 0;
	if(list==NULL || list->head==NULL || list->head->next==NULL)
		return TRUE;
	cur = list->head;
	while(cur) {
		cur = cur->next;
		count++;
	}
    median = count/2+count%2-1;
	if(count%2)
		median -= 1;
	cur = list->head;
	while(cur) {
		if(index==median) {
			if(count%2)
				headNew = cur->next->next;
			else
				headNew = cur->next;
			cur->next = NULL;
			break;
		}
		cur = cur->next;
		index++;
	}	
	headNew = list_reverse_recursive(headNew);
	cur = list->head;	
	while(cur && headNew) {
		if(cur->val != headNew->val)
			return FALSE;
		cur = cur->next;
		headNew = headNew->next;
	}

	return TRUE;
}

List* merge_two_list(List *l1, List *l2)
{
	Node *pre = NULL;
	Node *next = NULL;
	Node *head1 = NULL;
	Node *head2 = NULL;

	if(l1==NULL && l2==NULL)
		return NULL;
    else if(l1==NULL && l2!=NULL)
		return l2;
	else if(l1!=NULL && l2==NULL)
		return l1;

	pre = head1 = l1->head;
	head2 = l2->head;

	while(head1 && head2) {
		if(head1->val > head2->val) {
			next = head2->next;
			head2->next = head1;
			if(head1 == l1->head)
				l1->head = head2;
			else
				pre->next = head2;
			pre = head2;
			head2 = next;
		}else {
			pre = head1;
			head1 = head1->next;
		}
	}
	if(head1==NULL && head2!=NULL)
		pre->next = head2;

    return l1;
}

Node* two_list_numbers_add(List *list1, List *list2)
{
    Node *head1 = NULL;
	Node *head2 = NULL;
	Node *node = NULL;
	Node *pre = NULL;
	Node *cur = NULL;
	int carry = 0, val = 0;
	if(list1==NULL || list2==NULL)
		return NULL;
    else if(list1->head!=NULL && list2->head==NULL)
        return list1->head;
    else if(list1->head==NULL && list2->head!=NULL)
        return list2->head;

	head1 = list1->head;
	head2 = list2->head;

    while(head1 && head2) {
		head1 = head1->next;
		head2 = head2->next;
	}
	if(head1!=NULL) {
		head1 = list1->head;
		head2 = list2->head;
	}
	else {
		head1 = list2->head;
		head2 = list1->head;
	}
	
	cur = head1;
	while(head1 && head2) {
		val = head1->val+head2->val+carry;
		carry = 0;
		if(val>=10) {
			carry = 1;
			val -=10;
		}
		head1->val = val;
		pre = head1;
		head1 = head1->next;
		head2 = head2->next;
	}
	while(head1) {
		val = head1->val+carry;
		carry = 0;
		if(val>=10) {
			carry = 1;
			val -=10;
		}
		head1->val = val;
		pre = head1;
		head1 = head1->next;
	}
	if(carry==1) {
		node = node_create(1);
		if(node==NULL)
			return NULL;
		if(pre!=NULL)
			pre->next = node;
	}
	return cur;
}

Node* list_rotate_right(Node *head, int k)
{
    Node *cur = NULL;
    Node *pre = NULL;
    Node *tmp = NULL;
    int count = 0, index = 0;
    if(head==NULL || k==0)
        return head;
    cur = head;
    while(cur->next) {
        cur = cur->next;
        count++;
    }
    count += 1;
    if(k%count==0)
        return head;
    index = count-(k%count)-1;
    count = 0;
    cur = head;
    while(cur) {
        if(count==index) {
            tmp = cur;
        }
        pre = cur;
        cur = cur->next;
        count++;
    }
    if(tmp!=NULL) {   
        pre->next = head;
        head = tmp->next;
        tmp->next = NULL;
    }
    return head;	
}

int main(void)
{
    int ret = 0;
    List *list = list_create();
    if(list == NULL)
	return -1;

    list_head_add(list, 7);
    list_head_add(list, 7);
    list_head_add(list, 4);

    list_tail_add(list, 4);
    list_tail_add(list, 2);
    list_tail_add(list, 1);
    list_index_add(list, 0, 1);
    list_index_add(list, 1, 2);                   
    list_show(list);   /* 1 2 4 7 7 4 2 1 */

    list_index_del(list, 0);
    list_show(list);
	
#if DEF_FUNC("删除链表的倒数第n个节点")	
	Node *nodeK = node_kth_to_tail(list, 4);
	printf("node-kth(4): %d\n", nodeK->val);
	list_remove_nth_from_end(list, 4);
    list_show(list);

	nodeK = node_kth_to_tail(list, 6);
	printf("node-kth(6): %d\n", nodeK->val);
	list_remove_nth_from_end(list, 6);
    list_show(list);

	nodeK = node_kth_to_tail(list, 1);
	printf("node-kth(1): %d\n", nodeK->val);
	list_remove_nth_from_end(list, 1);
    list_show(list);
#endif

#if DEF_FUNC_CANCEL("链表反转")	
    list_reverse_iteration(list);
	list_show(list);
	list->head = list_reverse_recursive(list->head);
    list_show(list);
#endif

#if DEF_FUNC_CANCEL("奇偶链表")
    list->head = list_odd_even(list->head);
    list_show(list);
#endif

#if DEF_FUNC_CANCEL("回文链表")
	ret = list_is_palindrome(list);
	if(ret==FALSE)
		printf("The list is not palindrome.\n");
	else
		printf("The list is palindrome.\n");
#endif

#if DEF_FUNC_CANCEL("合并两个有序链表")
	List *list1 = list_create();
	if(list1 == NULL)
		return -1;
	List *list2 = list_create();
	if(list2 == NULL)
		return -1;

	list_index_add(list1, 0, 1);
	list_index_add(list1, 1, 2); 
	list_index_add(list1, 2, 4);
	list_index_add(list1, 3, 8);	
	list_index_add(list1, 4, 9);
	list_index_add(list1, 5, 20);  				  
	list_show(list1);

	list_index_add(list2, 0, 1);
	list_index_add(list2, 1, 3); 
	list_index_add(list2, 2, 4);
	list_index_add(list2, 3, 10);	
	list_index_add(list2, 4, 11);
	list_show(list2);

	list1 = merge_two_list(list1, list2);
	list_show(list1);

	list_destroy_by_level2_pointer(&list1);
	list_destroy_by_level2_pointer(&list2);
#endif

#if DEF_FUNC_CANCEL("两数相加")
	List *list1 = list_create();
	if(list1 == NULL)
		return -1;
	List *list2 = list_create();
	if(list2 == NULL)
		return -1;

	list_index_add(list1, 0, 1);
#if 0	
	list_index_add(list1, 1, 2); 
	list_index_add(list1, 2, 4);
	list_index_add(list1, 3, 8);	
	list_index_add(list1, 4, 7);
	list_index_add(list1, 5, 9);
#endif
	list_show(list1);

	list_index_add(list2, 0, 9);
	list_index_add(list2, 1, 9);
#if 0	
	list_index_add(list2, 2, 4);
	list_index_add(list2, 3, 8);	
	list_index_add(list2, 4, 9);
#endif
	list_show(list2);

	list1->head = two_list_numbers_add(list1, list2);
	list_show(list1);

	list_destroy_by_level2_pointer(&list1);
	list_destroy_by_level2_pointer(&list2);
#endif

#if DEF_FUNC_CANCEL("旋转链表")
	list->head = list_rotate_right(list->head,2);
    printf("list rorate righe 2 nodes:\n");
	list_show(list);
	list->head = list_rotate_right(list->head,2);
    printf("list rorate righe 2 nodes:\n");
	list_show(list);
	list->head = list_rotate_right(list->head,8);
    printf("list rorate righe 8 nodes:\n");
	list_show(list);
#endif


#if 1
	list_destroy_by_level2_pointer(&list);
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
