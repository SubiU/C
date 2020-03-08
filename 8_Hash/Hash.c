
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define DEF_FUNC(x) 0x01
#define DEF_FUNC_CANCEL(x) 0x00

#define HASH_TABLE_CAPACITY_INIT 999

typedef int keyType;
typedef int valueType;

typedef struct hash_node {
	keyType key;
	valueType value;
	struct hash_node *next;
}HashNode;

typedef struct {
	HashNode **node;
	int nodeSize;
	int capacity;
}HashTable;

void hash_table_show(HashTable *table)
{
	int i = 0;
	HashNode *cur = NULL;

	printf("Hash table show:\n");
	if(table==NULL)
		return;
	
	for(i=0; i<table->capacity; i++) {
		if(table->node && table->node[i]) {
			cur = table->node[i];
			while(cur) {
				printf("index(%d)\t[%d]->%d\n",i,cur->key,cur->value);
				cur =  cur->next;
			}
		}
	}
	printf("\n");
}

int hash_func(keyType key, int capacity)
{
	return key%capacity;
}

void hash_table_init(HashTable *table, int capacity)
{
	table->node = (HashTable **)malloc(sizeof(HashNode *)*capacity);   /* */
	memset(table->node, 0, sizeof(HashNode *)*capacity);
	table->nodeSize = 0;
	table->capacity = capacity;
}

HashNode* hash_node_create(keyType key, valueType value)
{
	HashNode *node = NULL;
	node = (HashNode *)malloc(sizeof(HashNode));
	if(node) {
		node->key = key;
		node->value = value;
	}
	return node;
}

short hash_table_node_search(HashTable *table, keyType key, valueType value)
{
	int index = 0;
	HashNode *cur = NULL;
	
	if(table==NULL || table->node==NULL)
		return -1;
	index = hash_func(key, table->capacity);
	if(table->node[index]) {	
		cur = table->node[index];
		while(cur) {
			if(cur->value == value)
				return 0;
			cur = cur->next;
		}
	}
	return -1;	
}

void hash_table_node_insert(HashTable *table, keyType key, valueType value)
{
	short ret = 0;
	int index = 0, i = 0;
	HashNode *node = NULL;
	HashTable tableNew;
	HashNode *cur = NULL;
	HashNode *next = NULL;
	
	ret = hash_table_node_search(table, key, value);
	if(ret != 0) {		
		if(table->nodeSize*10/table->capacity > 8) {  /* 装载因子, 动态扩容策略 */
			hash_table_init(&tableNew, table->capacity*2);
			for(i=0; i<table->capacity; i++) {
				if(table->node!=NULL && table->node[i]!=NULL) {
					cur = table->node[i];
					while(cur) {
						index = hash_func(cur->key, tableNew.capacity);
						next = cur->next;	
						cur->next = tableNew.node[index]; /* 将原表中node节点直接转移到新表,避免内存的频繁删除与创建操作 */
						tableNew.node[index] = cur;			
						tableNew.nodeSize++;
						cur = next;
					}
				}
			}
			
			node = hash_node_create(key, value);
			if(node) {
				index = hash_func(key, tableNew.capacity);
				node->next = tableNew.node[index];
				tableNew.node[index] = node;
				tableNew.nodeSize++;
			}

            free(table->node);
			table->node = tableNew.node;	
			table->nodeSize = tableNew.nodeSize;
			table->capacity = tableNew.capacity;
		}else { 
			node = hash_node_create(key, value);
			if(node) {
				index = hash_func(key, table->capacity);
				node->next = table->node[index];
				table->node[index] = node;
				table->nodeSize++;
			}
		}
	}
}

void hash_table_node_detele(HashTable *table, keyType key, valueType value)
{	
	int index = 0;
	HashNode *cur = NULL;
	HashNode *prev = NULL;
	
	if(table==NULL || table->node==NULL)
		return;
	index = hash_func(key, table->capacity);
	if(table->node[index]) {
		prev = table->node[index];
		cur = table->node[index];
		while(cur) {
			if(cur->value == value) {
				if(prev == cur) {
					table->node[index] = cur->next;
					free(prev);
					prev = NULL;
					table->nodeSize--;
					return;
				}else {
					prev->next = cur->next;
					free(cur);
					cur = NULL;
					table->nodeSize--;
					return;
				}
			}
			prev = cur;
			cur = cur->next;
		}
	}
	
	printf("The node with key(%d)-value(%d) does not exist\n",key, value);
}

#if 0
void hash_table_destroy(HashTable **table)
{
	int i = 0;
	HashNode *cur = NULL;
	
	if(table!=NULL && *table!=NULL) {
		for(i=0; i<(*table)->capacity; i++) {
			if((*table)->node!=NULL && (*table)->node[i]!=NULL) {
				cur = (*table)->node[i];
				while(cur) {
					cur = cur->next;
					free((*table)->node[i]);
					(*table)->node[i] = cur;
				}				
			}		
		}		
		free((*table)->node);
		(*table)->node = NULL;
		
		free((*table));
		*table = NULL;
	}
}
#endif
void hash_table_destroy(HashTable *table)
{
	int i = 0;
	HashNode *cur = NULL;
	
	if(table!=NULL && table->nodeSize>0) {
		for(i=0; i<table->capacity; i++) {
			if(table->node!=NULL && table->node[i]!=NULL) {
				cur = table->node[i];
				while(cur) {
					cur = cur->next;
					free(table->node[i]);
					table->node[i] = cur;
				}				
			}		
		}		
		free(table->node);
		table->node = NULL;
	}
}

int compare(const void *value1, const void *value2)
{
	return (*((int *)value1)-*((int *)value2));
}


bool contains_duplicate(int *array, int size)
{
#if DEF_FUNC("use hash table")
	int i = 0, num = 0;
    int index = 0;
    HashNode *node = NULL;
    HashNode *cur = NULL;

	HashTable table;
	hash_table_init(&table, HASH_TABLE_CAPACITY_INIT);
	
	for(i=0; i<size; i++) {
        num = array[i];
        if(num<0)
            index = (-num)%HASH_TABLE_CAPACITY_INIT;
        else 
            index = num%HASH_TABLE_CAPACITY_INIT;

        if(array[index])
        {
            cur = table.node[index];
            while(cur) {
                if(cur->key==num) {
					hash_table_destroy(&table);
					return true;
				}
                cur = cur->next;
            }            
        } 

        node = (HashNode *)malloc(sizeof(HashNode));
        if(node==NULL) {
			hash_table_destroy(&table);
            return false;
        }
        node->key = num;
        node->next = table.node[index];
        table.node[index] = node;
	}
	
	hash_table_destroy(&table);
	return false;
#endif

#if DEF_FUNC_CANCEL("use qsort")
    int i = 0;
	qsort(array, size, sizeof(int), compare);
	for(i=0; i<size-1; i++) {   /* size-1 */
		if(array[i]==array[i+1])
			return true;
	}
	return false;
#endif
}


#if 0
int single_number(int *array, int size)
{
	int i = 0;	
	if(array==NULL)
		return -1;
	qsort(array, size, sizeof(int), compare);
	
	for(i=0; i<size-1; i+=2) {
		if(array[i]!=array[i+1])
			return array[i];
	}

	if(size%2)
		return array[size-1];
	
	return -1;
}
#else   /* better */
int single_number(int *array, int size)
{
    int i = 0, res = 0;
	if(array==NULL)
		return -1;
	for(i=0; i<size; i++) {
		res ^= array[i];
	}
	return res;
}
#endif

#if 0
int* intersection(int* nums1, int nums1Size, int* nums2, int nums2Size, int* returnSize)
{
	int i = 0, j = 0;
	int size = 0;
    int *nums = NULL;

	if(nums1==NULL || nums2==NULL || returnSize==NULL || nums1Size<=0 || nums2Size<=0) {
		*returnSize = 0; /**/
		return NULL;
	}
	
	int arr[nums1Size+nums2Size];
	qsort(nums1, nums1Size, sizeof(int), compare);	
	qsort(nums2, nums2Size, sizeof(int), compare);	

	i = 0;
	j = 1;
	while(i<nums1Size) {
		while(j<nums1Size && nums1[i]==nums1[j])
			j++;
		arr[size++] = nums1[i];
		i = j;
		j += 1;
	}

	i = 0;
	j = 1;
	while(i<nums2Size) {
		while(j<nums2Size && nums2[i]==nums2[j])
			j++;
		arr[size++] = nums2[i];
		i = j;
		j += 1;
	}

    nums = (int *)malloc(sizeof(int)*size);
    if(nums==NULL)
        return NULL;
    memset(nums, 0, sizeof(int)*size);
	qsort(arr, size, sizeof(int), compare);	
	j = 0;
	for(i=0; i<size-1; i++) {
		if(arr[i]==arr[i+1])
			nums[j++] = arr[i];
	}
    *returnSize = j;
    
    return nums;
}
#else
int* intersection(int* nums1, int nums1Size, int* nums2, int nums2Size, int* returnSize)
{
	int hash[HASH_TABLE_CAPACITY_INIT] = {0};
	int i = 0, size = 0;

	if(nums1==NULL || nums2==NULL || returnSize==NULL || nums1Size<=0 || nums2Size<=0) {
		*returnSize = 0;
		return NULL;
	}
	
	int *nums = (int *)malloc(sizeof(nums1Size+nums2Size));
	if(nums==NULL) {
		*returnSize = 0;
		return NULL;
	}

	for(i=0; i<nums1Size; i++) {
		if(hash[nums1[i]]==0)
			hash[nums1[i]] = 1;
	}

	for(i=0; i<nums2Size; i++) {
		if(hash[nums2[i]]==1)
			hash[nums2[i]] = 2;
	}

	for(i=0; i<HASH_TABLE_CAPACITY_INIT; i++) {
		if(hash[i]==2)
			nums[size++] = i;
	}

	*returnSize = size;
	
	return nums;
}
#endif

int nextNum(int n) {
    int sum = 0;
    while(n) {
        sum += (n%10)*(n%10);
        n /= 10;
    }
    return sum;
}

bool isHappy(int n){
    int slow = n;
    int fast = n;
    do {
        slow = nextNum(slow);
        fast = nextNum(fast);
        fast = nextNum(fast);
    }while(slow!=fast);
    
    return slow==1;
}

int main(void)
{
#if DEF_FUNC_CANCEL("Hash table")
	HashTable table;
	hash_table_init(&table, HASH_TABLE_CAPACITY_INIT);
	hash_table_node_insert(&table, 1, 1);
	hash_table_node_insert(&table, 2, 2);
	hash_table_node_insert(&table, 5, 5);
	
	hash_table_node_insert(&table, 10, 10);
	hash_table_node_insert(&table, 21, 21);
	hash_table_node_insert(&table, 31, 31);
	hash_table_node_insert(&table, 12, 12);
	hash_table_node_insert(&table, 22, 22);
	hash_table_node_insert(&table, 32, 32);
	hash_table_node_insert(&table, 15, 15);
	hash_table_node_insert(&table, 25, 25);
	hash_table_show(&table);

	hash_table_node_detele(&table, 10, 10);
	hash_table_show(&table);
	hash_table_node_detele(&table, 31, 31);
	hash_table_show(&table);
	hash_table_node_detele(&table, 25, 25);
	hash_table_show(&table);
	hash_table_node_detele(&table, 32, 32);
	hash_table_show(&table);
	hash_table_node_detele(&table, 0, 0);
	hash_table_show(&table);
	hash_table_node_detele(&table, 10, 10);
	hash_table_show(&table);

	hash_table_destroy(&table);	
	hash_table_show(&table);
#endif

#if DEF_FUNC_CANCEL("Hash table")
	short ret = 0;
	HashTable table;
	hash_table_init(&table, HASH_TABLE_CAPACITY_INIT);
	hash_table_node_insert(&table, 1, 1);
	hash_table_node_insert(&table, 2, 2);
	ret = hash_table_node_search(&table, 1, 1);
	printf("key[1]->value[1] exist: %s\n", (ret? "false":"true"));
	ret = hash_table_node_search(&table, 3, 3);
	printf("key[3]->value[3] exist: %s\n", (ret? "false":"true"));

	hash_table_node_insert(&table, 2, 2);
	
	hash_table_show(&table);
	ret = hash_table_node_search(&table, 2, 2);
	printf("key[2]->value[2] exist: %s\n", (ret? "false":"true"));

	hash_table_node_detele(&table, 2, 2);
	
	hash_table_show(&table);
	ret = hash_table_node_search(&table, 2, 2);
	printf("key[2]->value[2] exist: %s\n", (ret? "false":"true"));
#endif

#if DEF_FUNC_CANCEL("contains duplicate") 
	bool rv = 0;
	int array[6] = {1, 3, 4, 4, 6, 8};
	
	rv = contains_duplicate(array, 6);
	if(rv)
		printf("The array is duplicate.\n");
	else 
		printf("The array is not duplicate.\n");
#endif

#if DEF_FUNC_CANCEL("single_number")
	int value = 0;
	int array[6] = {1, 1, 2};

	value = single_number(array, 3);
	if(value!=-1)
		printf("Single number %d exist.\n",value);
	else 
		printf("Single number does not exist.\n");
#endif

#if DEF_FUNC_CANCEL("intersection")
	int i = 0, size = 0;
	int arr1[10] = {8, 5, 5, 32, 10, 8, 11, 5, 32, 10};
	int arr2[6] = {56, 10, 7, 10, 10, 7};	
	int *arr_return = NULL;
	
	arr_return = intersection(arr1, 10, arr2, 6, &size);

	printf("arr return:\n");
	for(i=0; i<size; i++)
		printf("%d ",arr_return[i]);	
	printf("\n");

	free(arr_return);
	arr_return =  NULL;
#endif

#if DEF_FUNC("happy")
	printf("19 is happy? %s\n", (isHappy(19)? "true":"false"));
#endif

	return 0;
}
