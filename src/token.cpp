#include <token.hpp>

using namespace std;
namespace formula
{
    namespace token
    {
        const unordered_set<char> Token::s_supported_chars =
            {
                'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
                'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
                'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
                'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
                '+', '-', '*', '/', '^', '(', ')', '0', '1', '2', '3', '4', '5',
                '6', '7', '8', '9', '0', '.', ' ', '\t', '\r', '\n'};

        bool Token::isNumber(char ch)
        {
            return ((ch >= '0' && ch <= '9') || ch == '.');
        }

        bool Token::isOperator(char ch)
        {
            return (ch == '+' || ch == '-' || ch == '*' || ch == '/' ||
                    ch == '^' || ch == '#' || ch == '(' || ch == ')');
        }
        bool Token::isNeedChar(char ch)
        {
            switch (ch)
            {
            case ' ':
            case '\t':
            case '\n':
            case '\r':
                return false;
            default:
                return true;
            }
        }
        Token::Token() : type(Token::Error),
                         data(0.0) {}

        Token::Token(Token::Type _type, const string &_name, double _data) : type(_type),
                                                                             name(_name),
                                                                             data(_data) {}

        Token::Token(char ch)
        {
            if (isOperator(ch))
            {
                type = Token::Operator;
                name.push_back(ch);
                data = 0.0;
            }
            else
            {
                type = Token::Error;
                data = 0.0;
            }
        }

        Token::Token(double _data) : type(Token::Number),
                                     data(_data) {}

        Token::Token(const string &_name) : name(_name)
        {
            if (name.empty())
            {
                type = Token::Error;
                data = 0.0;
                return;
            }

            if (isOperator(name[0]))
            {
                type = Token::Operator;
            }
            else
            {
                type = Token::Variable;
            }
            data = 0.0;
        }

        int Token::innerPriority() const
        {
            if (name.empty() || type == Token::Error || type == Token::Number || type == Token::Variable)
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

        int Token::outerPriority() const
        {
            if (name.empty() || type == Token::Error || type == Token::Number || type == Token::Variable)
            {
                return 0;
            }

            switch (name[0])
            {
            case '#':
                return 0;
            case '+':
            case '-':
                return 2;
            case '*':
            case '/':
                return 4;
            case '^':
                return 6;
            case '(':
                return 10;
            case ')':
                return 1;

            // Function
            default:
                return 8;
            }
        }
    }
};