
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define DEF_FUNC(x) 0x01
#define DEF_FUNC_CANCEL(x) 0x00

#define OUTPUT 0
#define printf_f(x) { if(OUTPUT) { printf("[%s:%d]: ",__func__,__LINE__); printf x;} }

int numberOf1_optimization(int n)
{
	int count = 0;
	while(n) {
		count++;
		n = n&(n-1);
	}
	
	return count;
}

int numberOf1(int n)
{
	unsigned int flag = 1;
	int count = 0;
	while(flag>0) {
		if(n&flag)
			count++;
		flag <<= 1;
	}

	return count;
}

bool power_of_2(int n)
{
	n = n&(n-1);
	if(n==0)
		return true;
	else
		return false;
}

int main(void)
{
	int n = 1024;
	int count = 0;
//	count = numberOf1(n);
	count = numberOf1_optimization(n);
	
	printf("Numberof1 of %d: %d\n",n,count);
	printf("%d %s power of 2\n",n,(power_of_2(n)? "is":"isn't"));

	int integer1 = 10, integer2 = 13;
	int integer = integer^integer2;
	count = numberOf1_optimization(integer);
	printf("%d becomes %d,needs to be changed %d times\n",integer1,integer2,count);
	
	return 0;
}
