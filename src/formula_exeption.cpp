#include "../include/formula_exeption.hpp"

FormulaException::FormulaException(FormulaException::Type _type, const std::string& _message, double _value, const std::string& _interval):
	m_type(_type)
{
	switch (_type)
    {
    case UNKNOWN: m_message = "Unknown error occured"; break;
    case NOT_SUPPORTED_TOKEN: m_message = ("Not suppored token: " + _message); break;
    case NOT_ENOUGH_OPERANDS:
    {
        if (!_message.empty() && isOperator(_message[0]))
        {
            m_message = ("Not enough operands for operator " + _message);
        }
        else
        {
            m_message = ("Not enough operands for function " + _message);
        }
        break;
    }
    case INTERNAL_ERROR: m_message = "A programming error occured. Tell developer to check code."; break;
    case NOT_DEFINED_FUNCTION: m_message = ("Not defined function: " + _message); break;
    case WRONG_FORMAT: m_message = "Formula format is wrong"; break;
    case NOT_DEFINED_VARIABLE: m_message = ("Not defined variable: " + _message); break;
    case DIVIDIED_BY_ZERO: m_message = "Divided by zero"; break;
    case OUT_OF_RANGE: m_message = ("Operand x = " + std::to_string(_value) + " is out of function " + _message + "'s domain: " + _interval); break;
    case EMPTY_STRING: m_message = "Empty string"; break;
    case NOT_SUPPORTED_CHARACTER: m_message = "Not suppored character: " + _message; break;
    default: m_message = "Unknown error occured"; break;
    }
}

const char* FormulaException::what() const throw()
{
    return m_message.c_str();
}

std::string FormulaException::message() const
{
    return m_message;
}

FormulaException::Type FormulaException::type()const
{
	return m_type;
}
