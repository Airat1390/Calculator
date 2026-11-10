#include "app.h"
#include <libmath.h>

#include <cerrno>
#include <climits>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>

namespace
{

constexpr int kBaseTen = 10;
constexpr int kErrorOverflow = -5;

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
    std::printf("Калькулятор. Доступные опции:\n"); // NOLINT
    std::printf("  -a <число> : Первое число\n"); // NOLINT
    std::printf("  -b <число> : Второе число (игнорируется для !)\n"); // NOLINT
    std::printf("  -o <знак>  : Операция (+, -, *, /, ^, !)\n"); // NOLINT
    std::printf("  -h         : Показать эту справку\n\n"); // NOLINT
    std::printf("Пример запуска:\n"); // NOLINT
    std::printf("  calculator -a 5 -b 2 -o +\n"); // NOLINT
}

bool safeParseInt(const char* str, int& outValue)
{
    if (str == nullptr || *str == '\0')
    {
        return false;
    }

    char* endptr = nullptr;
    errno = 0;
    const int64_t val = std::strtol(str, &endptr, kBaseTen);

    if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN)) || (errno != 0 && val == 0))
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
            std::fprintf(stderr, "Error: Unary operation '!' requires operand -a\n"); // NOLINT
            return false;
        }
    }
    else
    {
        if (!hasA || !hasB)
        {
            std::fprintf(stderr, "Error: Operation '%c' requires both operands: -a and -b\n", operation); // NOLINT
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
    task.status = 0;

    bool hasA = false;
    bool hasB = false;
    bool hasO = false;

    int opt = 0;
    optind = 1;
    opterr = 0; 

    while ((opt = getopt(argc, argv, "a:b:o:h")) != -1) // NOLINT
    {
        switch (opt)
        {
            case 'a':
                if (!safeParseInt(optarg, task.value1))
                {
                    std::fprintf(stderr, "Error: Invalid or out-of-range value for -a: '%s'\n", optarg); // NOLINT
                    return 1;
                }
                hasA = true;
                break;
            case 'b':
                if (!safeParseInt(optarg, task.value2))
                {
                    std::fprintf(stderr, "Error: Invalid or out-of-range value for -b: '%s'\n", optarg); // NOLINT
                    return 1;
                }
                hasB = true;
                break;
            case 'o':
                if (optarg[0] == '\0' || optarg[1] != '\0') // NOLINT
                {
                    std::fprintf(stderr, "Error: Operator -o must be a single character: '%s'\n", optarg); // NOLINT
                    return 1;
                }
                task.operation = optarg[0]; // NOLINT
                hasO = true;
                break;
            case 'h':
                printHelp();
                return 0;
            case '?':
                if (optopt == 'a' || optopt == 'b' || optopt == 'o')
                {
                    std::fprintf(stderr, "Error: Option -%c requires an argument.\n", optopt); // NOLINT
                }
                else
                {
                    std::fprintf(stderr, "Error: Unknown option -%c.\n", optopt); // NOLINT
                }
                return 1;
            default:
                return 1;
        }
    }

    if (!hasO)
    {
        std::fprintf(stderr, "Error: Missing required option: -o\n"); // NOLINT
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

int printResult(const Task& task)
{
    if (task.status == 0)
    {
        if (task.operation == '!')
        {
            std::printf("%d! = %d\n", task.value1, task.result); // NOLINT
        }
        else
        {
            std::printf("%d %c %d = %d\n", task.value1, task.operation, task.value2, task.result); // NOLINT
        }
        return 0; 
    }

    if (task.status == -1)
    {
        std::fprintf(stderr, "Error: Division by zero!\n"); // NOLINT
    }
    else if (task.status == -2)
    {
        std::fprintf(stderr, "Error: Degree must be non-negative!\n"); // NOLINT
    }
    else if (task.status == -3)
    {
        std::fprintf(stderr, "Error: Factorial number must be non-negative!\n"); // NOLINT
    }
    else if (task.status == kErrorOverflow)
    {
        std::fprintf(stderr, "Error: Integer overflow occurred!\n"); // NOLINT
    }
    else
    {
        std::fprintf(stderr, "Error: Unknown math or operation error!\n"); // NOLINT
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

