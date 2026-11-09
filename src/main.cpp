<<<<<<< HEAD
#include <iostream>

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

struct Task 
{
    int value1;
    char operation;
    int value2;
    int status;
    int result;
};

void parse(int argc, char** argv, Task& task)
{
    task.value1 = std::atoi(argv[1]);
    task.operation = *(argv[2]);
    task.value2 = std::atoi(argv[3]);
}

void calculate(Task& task)
{
    task.status = 0;
    switch(task.operation)
    {
        case '+':
            task.result = addition(task.value1, task.value2);
            break;
        case '-':
            task.result = subtraction(task.value1, task.value2);
            break;
        case '*':
            task.result = multiplication(task.value1, task.value2);
            break;
        case '/':
            task.status = division(task.value1, task.value2, task.result);
            break;
        case '^':
            task.result = power(task.value1, task.value2);
            break;
        default:
            task.status = 1;
    }
}

void output(Task task)
{
    if (task.status == 0)
    {
        std::cout << task.value1 << ' ' << task.operation << ' ' << task.value2 << " = " << task.result << '\n';
    }
    else if (task.status == -1)
    {
        std::cout << "Error! Division by zero!\n";
    }
    else if (task.status == 1)
    {
        std::cout << "Error! Unknown operation!\n";
    }
    else 
    {
        std::cout << "Unknown error\n";
    }
}

void run(int argc, char** argv)
{
    Task task;
    parse(argc, argv, task);
    calculate(task);
    output(task);
}

int main(int argc, char** argv)
{
    run(argc, argv);
}

=======
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
>>>>>>> 966b3e872dcff76262b4f93cd04c176dc434eead
