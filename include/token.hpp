#ifndef __TOKEN_H__
#define __TOKEN_H__

#include <string>
#include <unordered_set>

namespace formula
{
    namespace token
    {
        struct Token
        {
            enum Type
            {
                Error,
                Operator,
                Number,
                Function,
                Variable
            };

            Type type;
            std::string name;
            double data;

        public:
            Token();
            Token(const std::string &_type);
            Token(char ch);
            Token(double _data);
            Token(Type _type, const std::string &_name, double _data);
            int innerPriority() const;
            int outerPriority() const;
            static bool isNumber(char ch);
            static bool isOperator(char ch);
            static bool isNeedChar(char ch);

        public:
            static const std::unordered_set<char> s_supported_chars;
        };

    };
};

#endif // __TOKEN_H__