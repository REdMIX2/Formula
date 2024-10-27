#ifndef __FORMULA_EXCEPTIONS_H__
#define __FORMULA_EXCEPTIONS_H__

#include <exception>
#include <string>

#include <token.hpp>

namespace formula
{
    class Exception : public std::exception
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

        Exception(Type code = UNKNOWN, const std::string &_message = "", double _value = 0.0, const std::string &_interval = "");
        virtual ~Exception() = default;
        const char *what() const throw();
        std::string message() const;
        Type type() const;

    private:
        Type m_type;
        std::string m_message;
    };
};

#endif // __FORMULA_EXCEPTIONS_H__