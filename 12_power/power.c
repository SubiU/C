
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define DEF_FUNC(x) 0x01
#define DEF_FUNC_CANCEL(x) 0x00

#define OUTPUT 0
#define printf_f(x) { if(OUTPUT) { printf("[%s:%d]: ",__func__,__LINE__); printf x;} }

bool g_InvailInput = false;

double power_exponent(double base, int exponent)
{
	if(exponent==0)
		return 1;
	if(exponent==1)
		return base;

	double result = power_exponent(base, exponent>>1);
	result *= result;
	if(exponent&0x1 == 1)
		result *= base;

	return result;
}

double power(double base, int exponent)
{
	if(base==0.0 && exponent<0) {
		g_InvailInput = true;
		return 0.0;
	}
	
	unsigned int absExponent = 0;
	if(exponent<0)
		absExponent = (unsigned int)(-exponent);
	else 
		absExponent = (unsigned int)(exponent);

	double result = 0;
	result = power_exponent(base, absExponent);

    if(exponent<0)
		result = 1.0/result;
	
	return result;
}

int main(void)
{
	double result = power(1.4,-17);

	if(g_InvailInput == false)
		printf("pow(1.4,-17): %f\n",result);
	else
		printf("Failed\n");
	
	return 0;
}
