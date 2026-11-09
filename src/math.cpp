#include "math.h"

namespace math
{

int addition(int a, int b)
{
	return a + b;
}

int subtraction(int a, int b)
{
	return a - b;
}

int multiplication(int a, int b)
{
	return a * b;
}

int division(int a, int b, int& c)
{
	if (b == 0)
	{
		return -1;
	}
	c = a / b;
	return 0;
}

int power(int a, int b)
{
	int c = 1;
	for (int i = 0; i < b; ++i)
	{
		c = c * a;
	}
	return c;
}

}
