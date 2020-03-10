/*****************************************************************   
** Function : 
**			  排序算法
**            冒泡排序、选择排序、插入排序、希尔排序
**            归并排序、快速排序
**
** 			  查找算法
**			  二分查找
******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DEF_FUNC(x) 0x01
#define DEF_FUNC_CANCEL(x) 0x00

#define OUTPUT 1	
#define printf_f(x) { if(OUTPUT) { printf("[%s:%d]: ",__func__,__LINE__); printf x;} }

#define SIZE 10

void array_show(int *array, int size)
{
    int i = 0;
	
	if(array==NULL)
		return;
	for(i=0; i<size; i++)
	{
		printf("%d ",array[i]);
	}
	printf("\n");
}

void swap(int *array, int index1, int index2)
{
	int tmp = 0;
	if(array==NULL)
		return;
	tmp = array[index1];
	array[index1] = array[index2];
	array[index2] = tmp;
}

void bubble_sort(int *array, int size)
{
	int i = 0, j = 0;
	int flag = 0;

	if(array==NULL || size<=1)
		return;
	
	for(i=0; i<size-1; i++) {
		for(j=size-1; j>i; j--) {
			if(array[j]<array[j-1]) { /* sort from small to large */
				swap(array, j, j-1);
				flag = 1;
			}
		}
		if(flag==0)   /* 没有数据交换,提前退出 */
			break;
	}
}

#if 1
void insert_sort(int *array, int size)
{
    int i = 0, j = 0;
	int tmp = 0;
	int index = 0;
	
	if(array==NULL)
		return;
 	for(i=1; i<size; i++) {
 		tmp = array[i];
 		for(j=0; j<i; j++) {
 			if(tmp<array[j]) {
				for(index=i; index>j; index--)
					array[index] = array[index-1];
				array[j] = tmp;
 				break;
 			}
 		}
 	}
}
#else 
void insert_sort(int *array, int size)
{
    int i = 0, j = 0;
	int tmp = 0;
	int index = 0;
	
	if(array==NULL)
		return;
 	for(i=1; i<size; i++) {
 		tmp = array[i];
		j = i;
		while(j>0 && tmp<array[j-1]) {
			array[j] = array[j-1];
			j--;
 		}
		if(j!=i)
			array[j] = tmp;
 	}
}

#endif

void select_sort(int *array, int size)
{
    int i = 0, j = 0;
	int select_index = 0;
	int select_flag = 0;

	if(array==NULL)
		return;
	
	for(i=0; i<size-1; i++) {
		select_index = i;
		for(j=i+1; j<size; j++) {
			if(array[select_index]>array[j])
				select_index = j;
	    }
		if(select_index!=i) {
			swap(array, i, select_index);
		}	
	}	
}

void merge(int *array, int start1, int end1, int start2, int end2)
{
    int tmp[20] = {0};
	int index = 0;
    int count1 = end1-start1+1, count2 = end2-start2+1;
	int *p = NULL, *q = NULL;

	if(array==NULL)
		return;
	p = &array[start1];
	q = &array[start2];

#if 0 /* debug */
	printf_f(("start1(%d) end1(%d) start2(%d) end2(%d)\n",start1,end1,start2,end2));
	printf_f(("count1(%d) count2(%d)\n",count1,count2));
	printf("*p(%d) *q(%d)\n",*p,*q);
	int i = 0;
	printf("array:");
	for(i=start1; i<=end2; i++) {
		printf("%d ",array[i]);
	}
	printf("\n");
#endif

	while(count1!=0 && count2!=0) {
		if(*p<=*q) {
			tmp[index++] = *p;
			p++;
		 	count1--;
		}else {
			tmp[index++] = *q;
			q++;
		 	count2--;
		}
	}

	while(count1!=0) {
		tmp[index++] = *p;
		p++;
		count1--;	
	}
	while(count2!=0) {
		tmp[index++] = *q;
		q++;
		count2--;	
	}

	memcpy(&array[start1], tmp, sizeof(int)*index);

#if 0 /* debug */
	printf("array(%d):",index);
	for(i=start1; i<=end2; i++) {
		printf("%d ",array[i]);
	}
	printf("\n");
#endif
}

void merge_sort(int *array, int start, int end)
{
	int median = 0;
	
	if(array==NULL)
		return;

    if(start>=end) {
//		printf(" start(%d)>=end(%d)\n",start,end);
		return;
    }
    median = (start+end)/2;
//	printf("median(%d)\n",median);
	merge_sort(array, start, median);
	merge_sort(array, median+1, end);
	
	merge(array, start, median, median+1, end);
}

void insert(int *array, int gap, int index)
{
    int i = 0;
	int inserted = array[index];
	
	printf_f(("gap(%d) index(%d)\n",gap,index));
	
    for(i=index-gap; i>=0&&(inserted<array[i]); i-=gap) {
		printf_f(("i(%d) \n",i))
        array[i+gap] = array[i];
    }
	printf_f(("i(%d) \n",i))
    array[i+gap] = inserted;
}

void shell_sort(int *array, int len)
{
    int  i = 0;
	int gap = 0;
	
    for(gap=len/2; gap>0; gap/=2) {
        for(i=gap; i<len; i++) {
            insert(array, gap, i);
		}
	}
}

int partition(int *array, int start, int end)
{
	int i = start, j = start;
	int val = array[end];
	
	if(array==NULL)
		return -1;
	
#if 1
    for(j=start; j<end; j++) {
		if(array[j]<val) {
			swap(array, i, j);
			i++;
		}
    }
#else	
	while(i<end && j<end) {
		if(array[j]>val) {
			j++;
		}else {
			swap(array, i, j);
			i++;
			j++;
		}
	}
#endif
	swap(array, i, end);

	return i;
}

void quick_sort(int *array, int start, int end)
{
	int point = 0;
	if(array==NULL)
		return;

	if(start>=end)
		return;

    point = partition(array, start, end);
	if(point==-1)
		return;
	quick_sort(array, start, point-1);
	quick_sort(array, point+1, end);
}

int binary_search(int *array, int size, int value)
{
    int median = 0;
	int start = 0, end = size-1;
	if(array==NULL || size<0)
		return -1;

	while(start<=end) {
		median = start+((end-start)>>1); /* start+(end-start)/2 */
		printf("median: %d\n",median);
		if(array[median]==value) 
			return median;
		else if(array[median]<value)
			start = median+1;
		else
			end = median-1;
	}
	return -1;
}

int binary_search_recursive(int * array, int start, int end, int value)
{
	int median = 0;
	if(array==NULL)
		return -1;
	if(start>end)
		return -1;
	median = start+((end-start)>>1);
	if(array[median] == value)
		return median;
	else if(array[median]<value)
		return binary_search_recursive(array, median+1, end, value);
	else 
		return binary_search_recursive(array, start, median-1, value);
}

int binary_search_first_value(int *array, int size, int value)
{
    int median = 0;
	int start = 0, end = size-1;
	if(array==NULL || size<0)
		return -1;

#if 0
	while(start<=end) {  /* 查找第一个值等于给定值的元素 */
		median = start+((end-start)>>1); /* 直接使用 (start+end)/2, 如start和end数值太大可能导致数据溢出,优化使用 start+(end-start)/2 */
		if(array[median]<value)
			start = median+1;
		else if(array[median]>value)
			end = median-1;
		else {
			if(median==0 || array[median-1]!=value) 
				return median;
			else {
	 			end = median-1;
			}
		}
	}
#else     /* 查找第一个值大于等于给定值的元素 */
	while(start<=end) { 
		median = start+((end-start)>>1); /* start+(end-start)/2 */
		if(array[median]>=value) {
			if(median==0 || array[median-1]<value)
				return median;
			else
				end = median-1;
		}
		else
			start = median+1;
	}
#endif

	return -1;
}

int binary_search_last_value(int *array, int size, int value)
{
    int median = 0;
	int start = 0, end = size-1;
	if(array==NULL || size<0)
		return -1;

#if 0    /* 查找最后一个值等于给定值的元素 */
	while(start<=end) {
		median = start+((end-start)>>1); /* start+(end-start)/2 */
		if(array[median]<value)
			start = median+1;
		else if(array[median]>value)
			end = median-1;
		else {
			if(median==end || array[median+1]!=value)
				return median;
			else
				start = median+1;
		}
	}
#else   /* 查找最后一个值小于等于给定值的元素 */
	while(start<=end) {
		median = start+((end-start)>>1); /* start+(end-start)/2 */
		if(array[median]<=value) {
			if(median==end || array[median+1]>value)
				return median;
			else
				start = median+1;
		}else
			end = median-1;
	}
#endif

	return -1;
}

void age_sort(int *age, int length)
{
	if(age==NULL || length<=0)
		return;
	
	int oldestAge = 99; 
	int timesofAge[oldestAge+1];
	int i = 0, j = 0, size = 0;

	for(i=0; i<oldestAge+1; i++)
		timesofAge[i] = 0;
	
	for(i=0; i<length; i++) {
		if(age[i]<0 || age[i]>oldestAge)
			continue;
		++timesofAge[age[i]];
	}

	for(i=0; i<oldestAge+1; i++) {
		for(j=0; j<timesofAge[i]; j++) {
			age[size++] = i;
		}
	}
}

int mininorder(int *arr, int len)
{
	if(arr==NULL || len<=0)
		return -1;

	int i = 0, result = arr[0];
	for(i=1; i<len; i++) {
		if(result > arr[i])
			result = arr[i];
	}
	
	return result;
}

int min(int *arr, int len)
{
	if(arr==NULL || len<=0)
		return -1;
	
	int index1 = 0, index2 = len-1;
	int mid = index1;
	while(arr[index1]>=arr[index2]) {
		mid = (index1+index2)/2;
		if(index2-index1==1) {
			mid = index2;
			break;
		}

		if(arr[index1]==arr[index2] && arr[mid]==arr[index1])
			return mininorder(arr,len);
			
		if(arr[mid]>=arr[index1])
			index1 = mid;
		else if(arr[mid]<=arr[index2])
			index2 = mid;
	}

	return arr[mid];
}

int main(void)
{
	int array[SIZE] = {0};
	int i = 0;
	for(i=0; i<SIZE; i++)
		array[i] = rand()%100+1;

	printf("array: ");
	array_show(array, SIZE);
#if 0	
	for(i=0; i<10; i++)
		printf("%d ",array[i]);
	printf("\n");
#endif

#if DEF_FUNC_CANCEL("bubble_sort")
	bubble_sort(array, SIZE);

	printf("bubble sort: ");
	array_show(array, SIZE);
#endif

#if DEF_FUNC_CANCEL("insert_sort")
    insert_sort(array, SIZE);

	printf("insert sort: ");
	array_show(array, SIZE);
#endif

#if DEF_FUNC_CANCEL("select_sort")
	select_sort(array, SIZE);

	printf("select sort: ");
	array_show(array, SIZE);
#endif

#if DEF_FUNC_CANCEL("merge_sort")
	merge_sort(array, 0, SIZE-1);

	printf("merge sort: ");
	array_show(array, SIZE);
#endif

#if DEF_FUNC_CANCEL("shell_sort")
	shell_sort(array, SIZE);

	printf("shell sort: ");
	array_show(array, SIZE);
#endif

#if DEF_FUNC("quick_sort")
	quick_sort(array, 0, SIZE-1);

	printf("quick sort: ");
	array_show(array, SIZE);
#endif

#if DEF_FUNC_CANCEL("binary_search")
//	i = binary_search(array, SIZE, 87);
//	i = binary_search_recursive(array, 0, SIZE-1, 87);
//	i = binary_search_first_value(array, SIZE, 87);
	i = binary_search_last_value(array, SIZE, 87);
	
	printf("binary_search: index(%d)\n",i);
#endif

#if DEF_FUNC("min")
/*
旋转数组的最小数字
// 题目：把一个数组最开始的若干个元素搬到数组的末尾，我们称之为数组的旋转。
// 输入一个递增排序的数组的一个旋转，输出旋转数组的最小元素。例如数组
// {3, 4, 5, 1, 2}为{1, 2, 3, 4, 5}的一个旋转，该数组的最小值为1。
*/
//	int arr[5] = {3, 4, 5, 1, 2};
	int arr[5] = {1, 2, 3, 4, 5};
	array_show(arr, 5);

	int value = 0;
	value = min(arr, 5);
	printf("min: %d\n", value);
#endif

#if DEF_FUNC_CANCEL("age sort")
	int ages[100];
	int idx = 0;
	for(idx=0; idx<100; idx++)
		ages[idx] = rand()%100;
	printf("ages: \n");
	array_show(ages, 100);
	age_sort(ages, 100);
	printf("ages sort:\n");
	array_show(ages, 100);
#endif

	return 0;
}
