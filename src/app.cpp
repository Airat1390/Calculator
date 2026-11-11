#include "app.h"

#include <errno.h> // NOLINT
#include <getopt.h>
#include <libmath.h>
#include <limits.h> // NOLINT
#include <stdint.h> // NOLINT
#include <stdio.h>  // NOLINT
#include <stdlib.h> // NOLINT
#include <unistd.h>

namespace
{

constexpr int kBaseTen = 10;

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
    // NOLINTNEXTLINE
    printf("Калькулятор. Доступные опции:\n");
    // NOLINTNEXTLINE
    printf("  -a <число>   : Первое число\n");
    // NOLINTNEXTLINE
    printf("  -b <число>   : Второе число (игнорируется для !)\n");
    // NOLINTNEXTLINE
    printf("  -o <знак>    : Операция (+, -, *, /, ^, !)\n");
    // NOLINTNEXTLINE
    printf("  -h, --help   : Показать эту справку\n\n");
    // NOLINTNEXTLINE
    printf("Пример запуска:\n");
    // NOLINTNEXTLINE
    printf("  calculator -a 5 -b 2 -o +\n");
}

bool safeParseInt(const char* str, int& outValue)
{
    if (str == nullptr || *str == '\0')
    {
        return false;
    }

    char* endptr = nullptr;
    errno = 0;
    const int64_t val = strtol(str, &endptr, kBaseTen);

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

    outValue = static_cast<int>(val);
    return true;
}

bool validateOptions(bool hasA, bool hasB, char operation)
{
    if (operation == '!')
    {
        if (!hasA)
        {
            // NOLINTNEXTLINE
            fprintf(stderr, "Error: Unary operation '!' requires operand -a\n");
            return false;
        }
    }
    else
    {
        if (!hasA || !hasB)
        {
            // NOLINTNEXTLINE
            fprintf(stderr,
                    "Error: Operation '%c' requires both operands: -a and -b\n",
                    operation);
            return false;
        }
    }
    return true;
}

int makeTask(int argc, char** argv, Task& task)
{
    task.value1 = 0;
    task.value2 = 0;
    task.operation = '\0';
    task.result = 0;
    task.status = libmath::kSuccess;

    bool hasA = false;
    bool hasB = false;
    bool hasO = false;

    // NOLINTNEXTLINE
    const option longOptions[] = {{"help", no_argument, nullptr, 'h'},
                                  {nullptr, 0, nullptr, 0}};

    int opt = 0;
    optind = 1;
    opterr = 0;

    // NOLINTNEXTLINE
    while ((opt = getopt_long(argc, argv, "a:b:o:h", longOptions, nullptr)) !=
           -1)
    {
        switch (opt)
        {
            case 'a':
                if (!safeParseInt(optarg, task.value1))
                {
                    // NOLINTNEXTLINE
                    fprintf(
                        stderr,
                        "Error: Invalid or out-of-range value for -a: '%s'\n",
                        optarg);
                    return 1;
                }
                hasA = true;
                break;
            case 'b':
                if (!safeParseInt(optarg, task.value2))
                {
                    // NOLINTNEXTLINE
                    fprintf(
                        stderr,
                        "Error: Invalid or out-of-range value for -b: '%s'\n",
                        optarg);
                    return 1;
                }
                hasB = true;
                break;
            case 'o':
                // NOLINTNEXTLINE
                if (optarg[0] == '\0' || optarg[1] != '\0')
                {
                    // NOLINTNEXTLINE
                    fprintf(
                        stderr,
                        "Error: Operator -o must be a single character: '%s'\n",
                        optarg);
                    return 1;
                }
                // NOLINTNEXTLINE
                task.operation = optarg[0];
                hasO = true;
                break;
            case 'h':
                printHelp();
                return 0;
            case '?':
                if (optopt == 'a' || optopt == 'b' || optopt == 'o')
                {
                    // NOLINTNEXTLINE
                    fprintf(stderr, "Error: Option -%c requires an argument.\n",
                            optopt);
                }
                else
                {
                    // NOLINTNEXTLINE
                    fprintf(stderr, "Error: Unknown option.\n");
                }
                return 1;
            default:
                return 1;
        }
    }

    if (!hasO)
    {
        // NOLINTNEXTLINE
        fprintf(stderr, "Error: Missing required option: -o\n");
        return 1;
    }

    if (!validateOptions(hasA, hasB, task.operation))
    {
        return 1;
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

int printResult(const Task& task)
{
    if (task.status == libmath::kSuccess)
    {
        if (task.operation == '!')
        {
            // NOLINTNEXTLINE
            printf("%d! = %d\n", task.value1, task.result);
        }
        else
        {
            // NOLINTNEXTLINE
            printf("%d %c %d = %d\n", task.value1, task.operation, task.value2,
                   task.result);
        }
        return 0;
    }

    if (task.status == libmath::kDivisionByZero)
    {
        // NOLINTNEXTLINE
        fprintf(stderr, "Error: Division by zero!\n");
    }
    else if (task.status == libmath::kNegativeDegree)
    {
        // NOLINTNEXTLINE
        fprintf(stderr, "Error: Degree must be non-negative!\n");
    }
    else if (task.status == libmath::kNegativeFactorial)
    {
        // NOLINTNEXTLINE
        fprintf(stderr, "Error: Factorial number must be non-negative!\n");
    }
    else if (task.status == libmath::kOverflow)
    {
        // NOLINTNEXTLINE
        fprintf(stderr, "Error: Integer overflow occurred!\n");
    }
    else
    {
        // NOLINTNEXTLINE
        fprintf(stderr, "Error: Unknown math or operation error!\n");
    }

    return 1;
}

} // namespace

namespace app
{

int applicationRun(int argc, char** argv)
{
    Task task{};
    const int parseStatus = makeTask(argc, argv, task);

    if (parseStatus >= 0)
    {
        return parseStatus;
    }

    makeCalculate(task);
    return printResult(task);
}

} // namespace app
