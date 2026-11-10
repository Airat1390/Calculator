#include "app.h"
#include <libmath.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

namespace
{

struct Task
{
    int value1;
    int value2;
    char operation;
    int result;
    int status;
};

void printHelp()
{
    printf("Калькулятор. Доступные опции:\n");
    printf("  -a <число> : Первое число\n");
    printf("  -b <число> : Второе число (игнорируется для !)\n");
    printf("  -o <знак>  : Операция (+, -, *, /, ^, !)\n");
    printf("  -h         : Показать эту справку\n");
}

void makeTask(int argc, char** argv, Task& task)
{
    task.value1 = 0;
    task.value2 = 0;
    task.operation = '\0';
    task.result = 0;
    task.status = 0;

    int opt;
    optind = 1;

    while ((opt = getopt(argc, argv, "a:b:o:h")) != -1)
    {
        switch (opt)
        {
            case 'a':
                task.value1 = atoi(optarg);
                break;
            case 'b':
                task.value2 = atoi(optarg);
                break;
            case 'o':
                task.operation = optarg[0];
                break;
            case 'h':
            default:
                printHelp();
                exit(0);
        }
    }

    if (task.operation == '\0')
    {
        printHelp();
        exit(1);
    }
}

void makeCalculate(Task& task)
{
    switch (task.operation)
    {
        case '+':
            task.status = libmath::addition(task.value1, task.value2, task.result);
            break;
        case '-':
            task.status = libmath::subtraction(task.value1, task.value2, task.result);
            break;
        case '*':
            task.status = libmath::multiplication(task.value1, task.value2, task.result);
            break;
        case '/':
            task.status = libmath::division(task.value1, task.value2, task.result);
            break;
        case '^':
            task.status = libmath::power(task.value1, task.value2, task.result);
            break;
        case '!':
            task.status = libmath::factorial(task.value1, task.result);
            break;
        default:
            task.status = 1;
            break;
    }
}

void printResult(Task task)
{
    if (task.status == 0)
    {
        if (task.operation == '!')
        {
            printf("%d! = %d\n", task.value1, task.result);
        }
        else
        {
            printf("%d %c %d = %d\n", task.value1, task.operation, task.value2, task.result);
        }
    }
    else if (task.status == -1)
    {
        printf("Error! Division by zero!\n");
    }
    else if (task.status == -2)
    {
        printf("Error! Degree must be non-negative!\n");
    }
    else if (task.status == -3)
    {
        printf("Error! Factorial number must be non-negative!\n");
    }
    else if (task.status == -5)
    {
        printf("Error! Integer overflow occurred!\n");
    }
    else if (task.status == 1)
    {
        printf("Error! Unknown operation!\n");
    }
    else
    {
        printf("Unknown error\n");
    }
}

} // namespace

namespace app
{

void applicationRun(int argc, char** argv)
{
    Task task;
    makeTask(argc, argv, task);
    makeCalculate(task);
    printResult(task);
}

} // namespace app

