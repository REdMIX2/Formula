#ifndef FORMULA_EXCEPTION_H
#define FORMULA_EXCEPTION_H

#include <string>
#include <exception>

static bool isOperator(char ch)
{
	return ( ch == '+' || ch == '-' || ch == '*' || ch == '/' ||
			 ch == '^' || ch == '#' || ch == '(' || ch == ')'   );
}


#ifdef _MSC_VER
class __declspec(dllexport) FormulaException : public std::exception
#else
class FormulaException : public std::exception
#endif
{
public:
    enum Type
    {
        UNKNOWN,
        NOT_SUPPORTED_TOKEN,
        NOT_ENOUGH_OPERANDS,
        INTERNAL_ERROR,
        NOT_DEFINED_FUNCTION,
        WRONG_FORMAT,
        NOT_DEFINED_VARIABLE,
        DIVIDIED_BY_ZERO,
        OUT_OF_RANGE,
        EMPTY_STRING,
        NOT_SUPPORTED_CHARACTER,
    };

    FormulaException(Type code = UNKNOWN, const std::string &_message = "", double _value = 0.0, const std::string &_interval = "");
    const char *what() const throw();
    std::string message() const;
    Type type() const;

private:
    Type m_type;
    std::string m_message;
};

#endif // FORMULA_EXCEPTION_H