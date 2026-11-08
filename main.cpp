/*
Имя нашей функции: addition
Аргумент функции: a (имеет тип int) и b (имеет тип int)
Функция вохвращает с (имеет тип int) - результат сложения двух чисел 
тип_возврата имя_функции(аргумент_функции)
{
    тело функции
}
*/
#include <iostream>

int addition(int *a, int *b)
{
    return *a + *b;
}

int substraction(int *a, int *b)
{
    return *a - *b;
}

int multiplication(int *a, int *b)
{
    return *a * *b;
}

int division(int *a, int *b, int* c)
{
    if (*b == 0)
    {
        return -1;
    }
    *c = *a / *b;
    return 0;
}

int power(int* a, int* b)
{
    int c = 1;
    for (int i = 0; i < *b; ++i)
        
        c *= *a;
    
    return c;
}

void printResult(int* a, int* b, int* c, char operation)
{
    std::cout << *a << ' ' << operation << ' ' << *b << " = " << *c << '\n';
}

int main()
{
    int a = 4;
    int b = 2;
    int c = 0;
    int status = 0;

    status = division(&a, &b, &c);
    if (status == 0)
    {
        printResult(&a, &b, &c, '/');
    }
    else if(status == -1)
    {
        std::cout << "Error! Division by zero is not allowed";
    }
    else
    {
        std::cout << "Unknown error at division operation";
    }
}
