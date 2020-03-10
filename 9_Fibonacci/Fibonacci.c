#include <stdio.h>
#include <stdlib.h>

long long fibonacci(long long n)
{
	long long fibMinusOne = 1, fibMinusTwo = 0, fibN = 0;
	unsigned int i = 0;
	if(n<2)	
		return n;

	for(i=2; i<=n; i++)	{
		fibN = fibMinusOne + fibMinusTwo;
		fibMinusTwo = fibMinusOne;
		fibMinusOne = fibN;
	}
	return fibN;
}

int main(void)
{
	int ret1 = 0, ret2 = 0;
	long long n, result = 0;
	
	printf("Fibonacci of n:\n");
	while((ret1=scanf("%lld",&n))!=1 || n<0 || (ret2=getchar())!='\n') {
		if(ret1!=1 || ret2!='\n')
			while(getchar()!='\n');

		printf("Input invalid\n");
		printf("Fibonacci of n:\n");
	}
	
	result = fibonacci(n);
	printf("Result of Fibonacci(%lld): %lld\n", n,result);

	return 0;
}
