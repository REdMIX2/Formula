#include <benchmark/benchmark.h>
#include <string>
#include <exception>
enum Token
{
    Error,
    Operator,
    Number,
    Function,
    Variable
};

int innerPriorityOld(std::string name, Token type)
{
    if (name.empty() || type == Token::Error || type == Token::Number ||
        type == Token::Variable)
    {
        return 0;
    }

    switch (name[0])
    {
    case '#':
        return 0;
    case '+':
    case '-':
        return 3;
    case '*':
    case '/':
        return 5;
    case '^':
        return 7;
    case '(':
        return 1;
    case ')':
        return 10;

    // Function
    default:
        return 9;
    }
}

int innerPriorityOldThow(std::string name, Token type)
{
    if (name.empty() || type == Token::Error || type == Token::Number ||
        type == Token::Variable)
    {
        throw std::exception();
        return 0;
    }

    switch (name[0])
    {
    case '#':
        return 0;
    case '+':
    case '-':
        return 3;
    case '*':
    case '/':
        return 5;
    case '^':
        return 7;
    case '(':
        return 1;
    case ')':
        return 10;

    // Function
    default:
        return 9;
    }
}

int innerPriority(std::string name, Token type)
{
    switch (type)
    {
    case Token::Error:
    case Token::Number:
    case Token::Variable:
    {
        return 0;
    }
    default:
    {
        if (name.empty())
            return 0;
        switch (name[0])
        {
        case '#':
            return 0;
        case '+':
        case '-':
            return 3;
        case '*':
        case '/':
            return 5;
        case '^':
            return 7;
        case '(':
            return 1;
        case ')':
            return 10;

        // Function
        default:
            return 9;
        }
    }
    }
}

void BM_ParseTokenOld(benchmark::State &state)
{
    for (auto _ : state)
    {
        for (std::string ch : {"#", "+", "-", "*", "/", "^", "(", ")"})
        {
            for (auto t : {Token::Error, Token::Operator, Token::Number, Token::Function, Token::Variable})
            {
                try
                {
                    innerPriorityOld(ch, t);
                }
                catch (const std::exception &err)
                {
                }
            }
        }
    }
}

void BM_ParseTokenOldThow(benchmark::State &state)
{
    for (auto _ : state)
    {
        for (std::string ch : {"#", "+", "-", "*", "/", "^", "(", ")"})
        {
            for (auto t : {Token::Error, Token::Operator, Token::Number, Token::Function, Token::Variable})
            {
                try
                {
                    innerPriorityOldThow(ch, t);
                }
                catch (const std::exception &err)
                {
                }
            }
        }
    }
}


void BM_ParseToken(benchmark::State &state)
{
    for (auto _ : state)
    {
        for (std::string ch : {"#", "+", "-", "*", "/", "^", "(", ")"})
        {
            for (auto t : {Token::Error, Token::Operator, Token::Number, Token::Function, Token::Variable})
            {
                innerPriority(ch, t);
            }
        }
    }
}
BENCHMARK(BM_ParseTokenOldThow)->Iterations(1'000'000);
BENCHMARK(BM_ParseToken)->Iterations(1'000'000);
BENCHMARK(BM_ParseTokenOld)->Iterations(1'000'000);

BENCHMARK_MAIN();