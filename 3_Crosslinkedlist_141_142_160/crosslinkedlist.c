/*****************************************************************   
** Function：
**    链表存在环检测和环入口节点获取.
**    相交链表的相交节点获取.
**
** ??? 链表相交,相交部分重复释放是否存在问题? 
******************************************************************/

#include <stdio.h>
#include <stdlib.h>

#define DEF_FUNC(x) 0x01
#define DEF_FUNC_CANCEL(x) 0x00

enum {
    DISABLE = 0,
    ENABLE = 1
};

#define OUTPUT 0	
#define printf_f(x) { if(OUTPUT) { printf("[%s:%d]: ",__func__,__LINE__); printf x;} }

typedef struct node {
    int val;
	struct node *next;
}Node;

typedef struct {
    Node *head;
}List;

typedef enum {
    structInt = 1,
	structNode
}StructType;

List* listCreate(void)
{
    List *list = NULL;
	list = (List *)malloc(sizeof(List));
	if(list)
		list->head = NULL;
	
	return list;
}

Node* nodeCreate(int val)
{
    Node *node = NULL;
	node = (Node *)malloc(sizeof(Node));
	if(node) {
        node->val = val;
		node->next = NULL;
	}
	
	return node;
}

/** Add a node of value val before the first element of the linked list. After the insertion, the new node will be the first node of the linked list. */
void listAddHead(List *list, void *data, StructType structType) 
{
    Node *newNode = NULL;
	
    if((list == NULL) || (data == NULL)) 
		return;

    if(structType == structInt) {
    	newNode = nodeCreate(*(int *)data);	    
    }else if(structType == structNode){
        newNode = (Node *)data;
	}

	if(newNode) {
        newNode->next = list->head;
		list->head = newNode;
	}
}

/** Append a node of value val to the last element of the linked list. */
void listAddTail(List *list, int val)
{
    Node *tail = NULL;
	Node *cur = NULL;
	
    if(list == NULL) 
		return;

    cur = list->head;
	tail = list->head;

    Node *newNode = nodeCreate(val);
	if(newNode == NULL) 
		return;
	
	while(cur) {
        tail = cur;
		cur = cur->next;
	}

	if(tail)
		tail->next = newNode;
	else 
		list->head = newNode;
}

void listTailPointtoIndex(List *list, int index)
{
    int idx = 0;
	Node *cur = NULL;
    Node *point = NULL;
    Node *tail = NULL;

	if(list==NULL)
		return;
	
    cur = list->head;
	while(cur) {
	    if(idx == index)
			point = cur;
    
        tail = cur;
		cur = cur->next;
		idx++;
	}

    if(tail)
	    tail->next = point;
}

void listTailPointtoIndexCancel(List *list, Node *firstNode, int index)
{
    int idx = 0;
	Node *cur = NULL;

	if(list==NULL)
		return;
	
    cur = list->head;
	while(cur) {
		if(cur->next==firstNode && idx>index) {
            cur->next = NULL;
			return;
		}			
		cur = cur->next;
		idx++;
	}
}


/* Two list has a intersecting node */
void listPointtoListByIndex(List *listA, List *listB, int index)
{
    int idx = 0;
	Node *cur = NULL;
    Node *point = NULL;
    Node *tail = NULL;

	if((listA==NULL) || (listB==NULL)) 
		return;
    cur = listB->head;
	while(cur) {
	    if(idx == index) {
	        point = cur;
		    break;
	    }
		cur = cur->next;
		idx++;
	}

	cur = listA->head;
    while(cur) {
		tail = cur;
        cur = cur->next;
	}

    if(tail && point)
	    tail->next = point;
}


int listNodeIndex(List *list, Node *node)
{
    int index = 0;
	
    if((list == NULL) || (list->head == NULL))
		return -1;

    Node *cur = list->head;
    while(cur) {
        if(cur == node) {
           return index;
		}

		cur = cur->next;
		index++;
	}

	return -1;
}

/** Add a node of value val before the index-th node in the linked list. If index equals to the length of linked list, the node will be appended to the end of linked list. If index is greater than the length, the node will not be inserted. */
void listIndexAdd(List *list, int index, int val)
{
    Node *cur = NULL;
	Node *pre = NULL;
	int idx = 0;
	
    if(list == NULL) 
		return;

	cur = list->head;
	pre = list->head;

	if(index == 0) {
		listAddHead(list, (void *)&val, structInt);
        return;
	}
	
	while(cur) {
        if(idx == index) {
			Node *newNode = nodeCreate(val);
			if(newNode) {
                newNode->next = cur;
				pre->next = newNode;
			}
			return;            
		}

		pre = cur;
		cur = cur->next;	
		idx++;
	}

	if(idx == index) {
		listAddTail(list, val);
    }
}

/** Delete the index-th node in the linked list, if the index is valid. */
void listIndexDelete(List *list, int index)
{
    Node *cur = NULL;
	Node *pre = NULL;
	int idx = 0;
	
    if(list == NULL) 
		return;

	cur = list->head;
	pre = list->head;

	if(index == 0) {
        list->head = cur->next;
	    free(cur);
	    cur = NULL;
		return;
	}
	
	while(cur) {
        if(idx == index) {			
            pre->next = cur->next;
			free(cur);
	        cur = NULL;
 			return;            
		}

		pre = cur;
		cur = cur->next;
		idx++;
	}
}

/** Get the value of the index-th node in the linked list. If the index is invalid, return -1. */
int listGet(List *list, int index)
{
    int idx = 0;
	
    if(list == NULL)
		return -1;

	Node *cur = list->head;

	while(cur) {
        if(idx == index) {
            return cur->val;
		}

		cur = cur->next;
		idx++;
	}

	return -1;
}

Node* detectCycle(List *list)
{
    if((list == NULL) || (list->head == NULL))
		return NULL;

	Node *pFast = list->head;
	Node *pSlow = list->head;
	
    while(pFast!=NULL) {
		if(pFast->next!=NULL && pFast->next->next!=NULL) {
        	pFast = pFast->next->next;	
        	pSlow = pSlow->next;
		}else {
            return NULL;
		}
    	if(pFast == pSlow) {
    		return pFast;
    	}
    }
}

#if 0
Node* listIntersectionNodeDetect(List *listA, List *listB)
{
    Node *curA = NULL;
	Node *curB = NULL;

	if(listA==NULL || listB==NULL)
		return NULL;

	curA = listA->head;
	curB = listB->head;
	while(curA!=NULL && curB!=NULL) {
       curA = curA->next;
	   curB = curB->next;
	}
	if(curA==NULL) {
		curA = listB->head;
		while(curB!=NULL) {
			curB = curB->next;
			curA = curA->next;
		}
		curB = listA->head;
		while(!(curA==NULL || curB==NULL || curA==curB)) {
            curA = curA->next;
			curB = curB->next;
		}
		if(curA!=NULL)
    		return curA;
		else
			return NULL;
	}
	else if(curB==NULL) {
		curB = listA->head;
		while(curA!=NULL) {
			curA = curA->next;
			curB = curB->next;
		}
		curA = listB->head;
		while(!(curA==NULL || curB==NULL || curA==curB)) {
            curA = curA->next;
			curB = curB->next;
		}
		if(curA!=NULL)
    		return curA;
		else
			return NULL;
	}

	return NULL;
}
#else   /* 优化 */
Node* listIntersectionNodeDetect(List *listA, List *listB)
{
    Node *curA = NULL;
	Node *curB = NULL;
	int countA = 0, countB = 0, count = 0;

	if(listA==NULL || listB==NULL)
		return NULL;

	curA = listA->head;
	curB = listB->head;
	while(curA!=NULL) {
       curA = curA->next;
       countA++;
	}	
	while(curB!=NULL) {
       curB = curB->next;
       countB++;
	}
	
	curA = listA->head;
	curB = listB->head;
	if(countA>=countB) {
        count = countA-countB;
		while(count--)
			curA = curA->next;
	}else if(countA<=countB) {
        count = countB-countA;
		while(count--)
			curB = curB->next;		
	}
	
	while(curA!=curB) {
        curA = curA->next;
	    curB = curB->next;
	}
	if(curA!=NULL)
		return curA;

	return NULL;
}
#endif

Node* cycleFirstNode(List *list, Node *node)
{
    if((list == NULL) || (list->head == NULL))
		return NULL;

    Node *p = list->head;
	Node *q = node;
	
    while(p) {
        p = p->next;
		q = q->next;
	    if(p == q)
			return p;
	}
}

void nodeFree(Node **node)
{
    if((node!=NULL) && (*node!=NULL)) {
		printf_f((" %d(%p)\n",(*node)->val,*node));
        free(*node);
	    printf_f((" %d(%p)\n",(*node)->val,*node));
		*node = NULL;
	}
}

#if 1
void listFree(List **list)
{
    if(list!=NULL && *list!=NULL && (*list)->head!=NULL) {
    	Node *cur = (*list)->head;
    	while(cur) {
    		cur = cur->next;
			printf_f(("%d(%p) %p list(%p)\n",((*list)->head)->val,(*list)->head,&((*list)->head),*list));
		    if((*list)->head!=NULL) {
    	        nodeFree(&((*list)->head));
		    }
			printf_f(("(%p) %p list(%p)\n",(*list)->head,&((*list)->head),*list));
    		(*list)->head = cur;
//			listShow(*list);
    	}   

    	free(*list);
    	*list = NULL;
    }
}
#else
void listFree(List *list)
{
    if(list!=NULL && list->head!=NULL) {
    	Node *cur = list->head;
    	while(cur) {
    		cur = cur->next;
    	    nodeFree(&(list->head));
    		list->head = cur;
    	} 
    }
}
#endif

void listShow(List *list)
{
    int idx = 0;
	
    if(list == NULL) {
        printf("The list is NULL.\n\n");
		return;
	}

	Node *cur = list->head;
//    printf("[%s:%d]: cur: %p\n",__func__,__LINE__,cur);
	while(cur) {
		if(idx > 20)
			break;
        printf("%d(%p) ",cur->val,cur);
	    cur = cur->next;
		idx++;
	}
	printf("\n\n");
}

int main(void)
{
    int index = 0;
	Node *firstNode = NULL;
	int param = 0;
	
    List *list = listCreate();
	if(list == NULL)
		return -1;
	
    listAddHead(list, (void *)&param, structInt);
    listAddTail(list, 7);
    listIndexAdd(list, 1, 1);
    listIndexAdd(list, 2, 2);
    listIndexAdd(list, 3, 3);
    listIndexAdd(list, 4, 4);	
    listIndexAdd(list, 5, 5);
    listIndexAdd(list, 6, 6);
	listShow(list); /* 0 1 2 3 4 5 6 7 */
	
#if DEF_FUNC_CANCEL("链表存在环检测和环入口节点获取")
		listTailPointtoIndex(list,2);
		listShow(list);
	
		Node *cycleEnd = detectCycle(list);  /* 快慢指针第一次相遇的节点 */
		printf("cycleEnd=%d\r\n",cycleEnd->val);
		if(cycleEnd != NULL) {
			printf("This list has Cycle.\r\n");
			firstNode = cycleFirstNode(list, cycleEnd); /* 环入口节点 */	
			printf("firstNode=%d\r\n",firstNode->val);
			if(firstNode != NULL) {
				index = listNodeIndex(list, firstNode);
				if(index != -1)
					printf("The first-cycle-index of this list is %d\r\n",index);
			}
		}
	
#if 1
        if(cycleEnd!=NULL)
    		listTailPointtoIndexCancel(list, firstNode, index);
		listFree(&list);
#else
		listFree(list);
		if(list!=NULL) {
			free(list);
			list = NULL;
		}
#endif
		listShow(list);
#endif

#if DEF_FUNC("相交链表的相交节点获取")
	Node *intersectionNode = NULL;
    List *listB = listCreate();
	if(listB == NULL)
		return -1;
	
    List *listC = listCreate();
	if(listC == NULL)
		return -1;
		
    param = 10;
    listAddHead(listB, (void *)&param, structInt);
    listAddTail(listB, 17);
    listIndexAdd(listB, 1, 11);
    listIndexAdd(listB, 2, 12);
	
	listPointtoListByIndex(listB, list, 3);
	listShow(listB); /* 10 11 12 17 3 4 5 6 7 */

    param = 1;
    listAddHead(listC, (void *)&param, structInt);
	
	listPointtoListByIndex(listC, list, 3);
	listShow(listC); /* 1 3 4 5 6 7 */

    index = 0;
	intersectionNode = listIntersectionNodeDetect(list, listB);
	if(intersectionNode!=NULL) {
        printf("listIntersectionNodeDetect: %d\n",intersectionNode->val);
	}else {
        printf("The list has no IntersectionNodeDetect.\n");
	}

/*****************************************************
链表相交:
   依次释放链表分配的堆内存,节点"3"后链表成员重复释放了3次,编译执行没有出错, Why?

   list:	  0 - 1 - 2 
					   \ 
   listB: 10-11-12-17 - 3 - 4 - 5 - 6 - 7 
					   /	   
   listC:			  1  
*****************************************************/

	listFree(&list); 
//	listShow(list);
	
	listFree(&listB);
//	listShow(list);

	listFree(&listC);
//	listShow(listC);
#endif


#if DEF_FUNC_CANCEL("内存释放")
	int a=10;
    int *q = NULL;
	int *p=(int *)malloc(sizeof(int));
	*p=200;
	q = p;
    free(p);
	p=NULL;

	if(q!=NULL) {
    	free(q);  /* 对内存二次释放出错 */
		q = NULL;
	}
#endif

    return 0;
}
