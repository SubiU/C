
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define DEF_FUNC(x) 0x01
#define DEF_FUNC_CANCEL(x) 0x00

#define OUTPUT 0
#define printf_f(x) { if(OUTPUT) { printf("[%s:%d]: ",__func__,__LINE__); printf x;} }

#if DEF_FUNC("动态规划")
int max_product_after_cutting(int len)
{
	if(len<2)
		return 0;
	else if(len==2)
		return 1;
	else if(len==3)
		return 2;

	int products[len+1];
	products[0] = 0;
	products[1] = 1;
	products[2] = 2;
	products[3] = 3;

	int i = 0, j = 0, product = 0, max = 0;
	for(i=4; i<=len; ++i) {
		for(j=1; j<=i/2; ++j) {
			product = products[j]*products[i-j];
			if(max<product)
				max = product;
			products[i] = max;
		}
	}

	return products[len];
}
#endif

#if DEF_FUNC("贪婪算法")
int max_product_after_cutting_solution(int len)
{
	if(len<2)
		return 0;
	else if(len==2)
		return 1;
	else if(len==3)
		return 2;

	int timesof2 = 0, timesof3 = len/3;
	if(len-timesof3*3==1)
		timesof3 -= 1;
	timesof2 = (len-timesof3*3)/2;

	return ((int)(pow(3,timesof3))*(int)(pow(2,timesof2)));
}
#endif

int main(void)
{
	int len = 50, max = 0;
	max = max_product_after_cutting(len);
	printf("max: %d\n", max);

	max = max_product_after_cutting_solution(len);
	printf("max: %d\n", max);
	
	return 0;
}
