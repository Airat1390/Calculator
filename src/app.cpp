#include "app.h"

#include <errno.h>
#include <libmath.h>
#include <limits.h>
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
    printf("  -h         : Показать эту справку\n\n");
    printf("Пример запуска:\n");
    printf("  calculator -a 5 -b 2 -o +\n");
}

bool safeParseInt(const char* str, int& out_value)
{
    if (str == nullptr || *str == '\0')
        return false;

    char* endptr;
    errno = 0;
    long val = strtol(str, &endptr, 10);

    if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN)) ||
        (errno != 0 && val == 0))
    {
        return false;
    }
    if (*endptr != '\0')
    {
        return false;
    }
    if (val < INT_MIN || val > INT_MAX)
    {
        return false;
    }

    out_value = static_cast<int>(val);
    return true;
}

int makeTask(int argc, char** argv, Task& task)
{
    task.value1 = 0;
    task.value2 = 0;
    task.operation = '\0';
    task.result = 0;
    task.status = 0;

    bool has_a = false;
    bool has_b = false;
    bool has_o = false;

    int opt;
    optind = 1;
    opterr = 0;

    while ((opt = getopt(argc, argv, "a:b:o:h")) != -1)
    {
        switch (opt)
        {
            case 'a':
                if (!safeParseInt(optarg, task.value1))
                {
                    fprintf(
                        stderr,
                        "Error: Invalid or out-of-range value for -a: '%s'\n",
                        optarg);
                    return 1;
                }
                has_a = true;
                break;
            case 'b':
                if (!safeParseInt(optarg, task.value2))
                {
                    fprintf(
                        stderr,
                        "Error: Invalid or out-of-range value for -b: '%s'\n",
                        optarg);
                    return 1;
                }
                has_b = true;
                break;
            case 'o':
                if (optarg[0] == '\0' || optarg[1] != '\0')
                {
                    fprintf(
                        stderr,
                        "Error: Operator -o must be a single character: '%s'\n",
                        optarg);
                    return 1;
                }
                task.operation = optarg[0];
                has_o = true;
                break;
            case 'h':
                printHelp();
                return 0;
            case '?':
                if (optopt == 'a' || optopt == 'b' || optopt == 'o')
                {
                    fprintf(stderr, "Error: Option -%c requires an argument.\n",
                            optopt);
                }
                else
                {
                    fprintf(stderr, "Error: Unknown option -%c.\n", optopt);
                }
                return 1;
            default:
                return 1;
        }
    }

    if (!has_o)
    {
        fprintf(stderr, "Error: Missing required option: -o\n");
        return 1;
    }

    if (task.operation == '!')
    {
        if (!has_a)
        {
            fprintf(stderr, "Error: Unary operation '!' requires operand -a\n");
            return 1;
        }
    }
    else
    {
        if (!has_a || !has_b)
        {
            fprintf(stderr,
                    "Error: Operation '%c' requires both operands: -a and -b\n",
                    task.operation);
            return 1;
        }
    }

    return -1;
}

void makeCalculate(Task& task)
{
    switch (task.operation)
    {
        case '+':
            task.status =
                libmath::addition(task.value1, task.value2, task.result);
            break;
        case '-':
            task.status =
                libmath::subtraction(task.value1, task.value2, task.result);
            break;
        case '*':
            task.status =
                libmath::multiplication(task.value1, task.value2, task.result);
            break;
        case '/':
            task.status =
                libmath::division(task.value1, task.value2, task.result);
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

int printResult(Task task)
{
    if (task.status == 0)
    {
        if (task.operation == '!')
        {
            printf("%d! = %d\n", task.value1, task.result);
        }
        else
        {
            printf("%d %c %d = %d\n", task.value1, task.operation, task.value2,
                   task.result);
        }
        return 0;
    }

    if (task.status == -1)
    {
        fprintf(stderr, "Error: Division by zero!\n");
    }
    else if (task.status == -2)
    {
        fprintf(stderr, "Error: Degree must be non-negative!\n");
    }
    else if (task.status == -3)
    {
        fprintf(stderr, "Error: Factorial number must be non-negative!\n");
    }
    else if (task.status == -5)
    {
        fprintf(stderr, "Error: Integer overflow occurred!\n");
    }
    else
    {
        fprintf(stderr, "Error: Unknown math or operation error!\n");
    }

    return 1;
}

} // namespace

namespace app
{

int applicationRun(int argc, char** argv)
{
    Task task;
    int parse_status = makeTask(argc, argv, task);

    if (parse_status >= 0)
    {
        return parse_status;
    }

    makeCalculate(task);
    return printResult(task);
}

} // namespace app
