#include <cstdio>
#include <cstdlib>
#include <string>
#include <regex>
#include <cmath>

/* list of supported operations */
enum operations
{
    none,
    addition,
    subtraction,
    multiplication,
    division,
    modulo
};

void error_exit()
{
    printf("ERROR: <value1> [=-x/] <value2>\n");
    exit(0);
}

enum operations get_operator(std::string_view x)
{
    switch (x[0])
    {
    case '+':
        return operations::addition;
    case '-':
        return operations::subtraction;
    case 'x':
        return operations::multiplication;
    case '/':
        return operations::division;
    case '%':
        return operations::modulo;
    default:
        error_exit();
        return operations::none;
    }
}

template<typename T>
T calculate(T x, T y, enum operations op)
{
    switch(op)
    {
        case operations::addition:
            return x + y;
        case operations::subtraction:
            return x - y;
        case operations::multiplication:
            return x * y;
        case operations::division:
            return x / y;
        case operations::modulo:
            return std::fmod(x,y);
        default:
            return 0;
    }
}

int main(int argc, char **argv)
{
    /* create a single string from all the passed parameters 
     * NOTE: if using parentheses on the expression you must use "" or '' 
     * around the expression or escape the open and close parenthesis characters */
    std::string input{};
    for(int index{1}; index < argc; index++)
    {
        input += argv[index];
        input += (index == argc - 1 ? "" : " "); 
    }

    std::string v1_str{};
    std::string op_str{};
    std::string v2_str{};

    /* use regex to do exact matches of the string and determine the types of the operands */
    std::regex regex_all{R"(\(?\s*((?:\d+)(?:\.\d+)?)\s*\)?\s*([+-x/%])\s*\(?\s*((?:\d+)(?:\.\d+)?)\s*\)?)"};
    std::smatch results;
    if(std::regex_match(input, results, regex_all))
    {
        v1_str = results[1].str();
        op_str = results[2].str();
        v2_str = results[3].str();
    }
    else
    {
        printf("ERR: could not match any regex patterns\n");
        error_exit();
    }

    v1_str = results[1].str();
    op_str = results[2].str();
    v2_str = results[3].str();

    auto op = get_operator(op_str);
    auto x = std::stod(v1_str);
    auto y = std::stod(v2_str);
    auto result = calculate(x, y, op);
    printf("%lf %s %lf = %lf\n", x, op_str.c_str(), y, result);

    return 0;
}