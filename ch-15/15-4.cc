#include <cstdio>
#include <cstdlib>
#include <string>
#include <regex>

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
            return x % y;
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
    std::regex regex_int{R"(\(?\s*(\d+)\s*\)?\s*([+-x/])\s*\(?\s*(\d+)\s*\)?)"};
    std::regex regex_float{R"(\(?\s*(\d+\.\d+)\s*\)?\s*([+-x/])\s*\(?\s*(\d+\.\d+)\s*\)?)"};
    std::smatch results;
    bool is_float = false;
    if(!std::regex_match(input, results, regex_int))
    {
        if(std::regex_match(input, results, regex_float))
            is_float = true;
        else
        {
            printf("ERR: could not match any regex patterns\n");
            error_exit();
        }
    }

    v1_str = results[1].str();
    op_str = results[2].str();
    v2_str = results[3].str();

    printf("v1 = %s\n", v1_str.c_str());
    printf("op = %s\n", op_str.c_str());
    printf("v2 = %s\n", v2_str.c_str());

    int val{}, i_val2{}, i_result{};
    double d_val1{}, d_val2{}, d_result{};
    auto op = get_operator(op_str);

    if(!is_float)
    {
        auto val1 = std::stoi(v1_str);
        auto val2 = std::stoi(v2_str);
        auto result = calculate(val1, val2, op);
        printf("%d %s %d = %d\n", val1, op_str.c_str(), val2, result);
    }
    else
    {
        auto val1 = std::stod(v1_str);
        auto val2 = std::stod(v2_str);
        auto result = calculate(val1, val2, op);
        printf("%lf %s %lf = %lf\n", val1, op_str.c_str(), val2, result);
    }

    return 0;
}