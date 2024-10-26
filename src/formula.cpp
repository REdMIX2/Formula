#include <formula.hpp>

#include <stack>
#include <cmath>
#include <math.h>

using namespace std;

const unordered_set<char> Formula::s_supported_chars =
{
	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
	'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
	'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
	'+', '-', '*', '/', '^', '(', ')', '0', '1', '2', '3', '4', '5',
	'6', '7', '8', '9', '0', '.', ' ', '\t', '\r', '\n'
};

static bool isZero(double x)
{
    return (fabs(x) < 1E-6);
}

static bool isNumber(char ch)
{
    return ( (ch >= '0' && ch <= '9') || ch == '.');
}

static bool isOperator(char ch)
{
	return ( ch == '+' || ch == '-' || ch == '*' || ch == '/' ||
			 ch == '^' || ch == '#' || ch == '(' || ch == ')'   );
}

Formula::Exception::Exception(Formula::Exception::Type _type, const std::string& _message, double _value, const std::string& _interval):
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
    case OUT_OF_RANGE: m_message = ("Operand x = " + to_string(_value) + " is out of function " + _message + "'s domain: " + _interval); break;
    case EMPTY_STRING: m_message = "Empty string"; break;
    case NOT_SUPPORTED_CHARACTER: m_message = "Not suppored character: " + _message; break;
    default: m_message = "Unknown error occured"; break;
    }
}

const char* Formula::Exception::what() const throw()
{
    return m_message.c_str();
}

string Formula::Exception::message() const
{
    return m_message;
}

Formula::Exception::Type Formula::Exception::type()const
{
	return m_type;
}

vector<double> varargin2vector()
{
	return vector<double>();
}

Formula::Token::Token():
type(Token::Error),
data(0.0) {}

Formula::Token::Token(Token::Type _type, const string& _name, double _data):
type(_type),
name(_name),
data(_data) {}

Formula::Token::Token(char ch)
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

Formula::Token::Token(double _data):
type(Token::Number),
data(_data) {}

Formula::Token::Token(const string& _name):
name(_name)
{
	if(name.empty())
	{
		type = Token::Error;
		data = 0.0;
		return;
	}

	if(isOperator(name[0]))
	{
		type = Token::Operator;
	}
	else
	{
		type = Token::Variable;
	}
	data = 0.0;
}

int Formula::Token::innerPriority()const
{
	if(name.empty() || type == Token::Error || type == Token::Number || type == Token::Variable)
	{
		return 0;
	}

	switch(name[0])
	{
		case '#': return 0;
		case '+':
		case '-': return 3;
		case '*':
		case '/': return 5;
		case '^': return 7;
		case '(': return 1;
		case ')': return 10;

		// Function
		default:	 return 9;
	}
}

int Formula::Token::outerPriority()const
{
	if(name.empty() || type == Token::Error || type == Token::Number || type == Token::Variable)
	{
		return 0;
	}

	switch(name[0])
	{
		case '#': return 0;
		case '+':
		case '-': return 2;
		case '*':
		case '/': return 4;
		case '^': return 6;
		case '(': return 10;
		case ')': return 1;

		// Function
		default:	 return 8;
	}
}

Formula::Formula() {}

Formula::Formula(const string& str):
    m_formula_string(str)
{
	generatePostfix();
}

Formula::Formula(const char* str):
	m_formula_string(std::string(str))
{
	generatePostfix();
}

Formula& Formula::operator =(const string& str)
{
    Formula temp(str);
    m_postfix = temp.m_postfix;
    m_formula_string = temp.m_formula_string;
    m_found_variables = temp.m_found_variables;

    return *this;
}

Formula& Formula::operator =(const char* str)
{
	return (*this = string(str));
}

void Formula::check()const
{
	if( m_postfix.empty() )
	{
		return;
	}
	
	stack<double> operants;
	for(auto token = m_postfix.cbegin(); token != m_postfix.cend(); token++)
	{
		switch(token->type)
		{
			case Token::Error:
			{
				throw Exception(Exception::NOT_SUPPORTED_TOKEN, token->name);
			}
			case Token::Number:
			case Token::Variable:
			{
				operants.push(0);
				break;
			}
			case Token::Operator:
			{
				if(token->name != "+" &&
				   token->name != "-" &&
				   token->name != "*" &&
				   token->name != "/" &&
				   token->name != "^")
				{
					throw Exception(Exception::WRONG_FORMAT, token->name);
				}

				if(operants.size() < 2)
				{
					throw Exception(Exception::NOT_ENOUGH_OPERANDS, token->name);
				}
				
				operants.pop();
				break;
			}
			case Token::Function:
			{
				if(operants.empty())
				{
					throw Exception(Exception::NOT_ENOUGH_OPERANDS, token->name);
				}

				operants.pop();

				if(m_defined_functions.count(token->name) == 0)
				{
					throw Exception(Exception::NOT_DEFINED_FUNCTION, token->name);
				}
				operants.push(0);
				break;
			}
		}
	}

    if(operants.size() != 1)
	{
		throw Exception(Exception::WRONG_FORMAT);
	}
}

ostream& operator <<(ostream& o, const Formula& f)
{
	o << f.m_formula_string;
	return o;
}

istream& operator >>(istream& i, Formula& f)
{
	string str;

    getline(cin, str);

	f = str;
	f.check();

	return i;
}

Formula& Formula::input(const string& str_promt)
{
	string str;
	cout << str_promt;
	getline(cin, str);

	*this = str;
	check();

	return *this;
}

void Formula::clear()
{
    m_postfix.clear();
    m_formula_string.clear();
    m_found_variables.clear();
    m_defined_variables.clear();
    m_defined_functions.clear();
}

bool Formula::empty()const
{
	return m_postfix.empty();
}

void Formula::define(const string& var_name, double value)
{
	m_defined_variables[var_name] = value;
}

void Formula::define(const string& func_name, const std::function<double(double)>& f)
{
	m_defined_functions[func_name] = f;
}

double Formula::eval(const unordered_map<string, double>& variables)
{
    if (m_postfix.empty())
    {
        throw Exception(Exception::EMPTY_STRING);
    }

	stack<double> operants;

	double x, y;
	for(auto token = m_postfix.cbegin(); token != m_postfix.cend(); token++)
	{
		switch(token->type)
		{
			case Token::Error:
			{
				throw Exception(Exception::NOT_SUPPORTED_TOKEN, token->name);
			}
			case Token::Number:
			{
				operants.push(token->data);
				break;
			}
			case Token::Variable:
			{
				if(variables.count(token->name) != 0)
				{
					operants.push(variables.at(token->name));
				}
				else if(m_defined_variables.count(token->name) != 0)
				{
					operants.push(m_defined_variables.at(token->name));
				}
				else if(s_built_in_variables.count(token->name) != 0)
				{
					operants.push(s_built_in_variables.at(token->name));
				}
				else
				{
					throw Exception(Exception::NOT_DEFINED_VARIABLE, token->name);
				}
				break;
			}
			case Token::Operator:
			{
				if(token->name != "+" &&
				   token->name != "-" &&
				   token->name != "*" &&
				   token->name != "/" &&
				   token->name != "^")
				{
					throw Exception(Exception::WRONG_FORMAT);
				}

				if(operants.size() < 2)
				{
					throw Exception(Exception::NOT_ENOUGH_OPERANDS, token->name);
				}
				
				y = operants.top();
				operants.pop();

				x = operants.top();
				operants.pop();

				if(token->name == "+") operants.push(x + y);
				else if(token->name == "-") operants.push(x - y);
				else if(token->name == "*") operants.push(x * y);
				else if(token->name == "/")
				{
					if(isZero(y))
					{
						throw Exception(Exception::DIVIDIED_BY_ZERO);
					}
					operants.push(x / y);
				}
				else // if(token->name == "^")
				{
					if(isZero(x) && y < 0)
					{
                        throw Exception(Exception::DIVIDIED_BY_ZERO);
					}
					operants.push(pow(x, y));
				}

				break;
			}
			case Token::Function:
			{
				if(operants.empty())
				{
					throw Exception(Exception::NOT_ENOUGH_OPERANDS, token->name);
				}

				x = operants.top();
				operants.pop();

				if(m_defined_functions.count(token->name) != 0)
				{
					operants.push( m_defined_functions[token->name](x) );
				}
				else if(s_built_in_functions.count(token->name) != 0)
				{
					operants.push( s_built_in_functions.at(token->name)(x) );
				}
				else
				{
					throw Exception(Exception::NOT_DEFINED_FUNCTION, token->name);
				}
			}
		}
	}

    if(operants.size() != 1)
	{
		throw Exception(Exception::WRONG_FORMAT);
	}
	else
	{
		if( fabs( operants.top() ) <= 1E-6 )
		{
			return 0;
		}
		else
		{
			return operants.top();
		}
	}
}

double Formula::eval(const vector<double>& vector_variables)
{
    unsigned i = 0;
    unordered_map<string, double> variables;
    for (const string& name : m_found_variables)
    {
        if (!m_defined_variables.count(name))
        {
            if (i >= vector_variables.size())
            {
                break;
            }
            variables[name] = vector_variables[i];
            i++;
        }
    }

    return eval(variables);
}

double Formula::operator ()(const unordered_map<string, double>& variables)
{
    return eval(variables);
}

double Formula::operator ()(const vector<double>& vector_variales)
{
    return eval(vector_variales);
}

void Formula::preprocess(string& str)
{
    unsigned i = 0;

	// Delete all spaces in string.
	for(auto it = str.begin(); it != str.end();)
	{
		if(s_supported_chars.count(*it) == 0)
		{
			string error_char;
			error_char.push_back(*it);
			throw Exception(Exception::NOT_SUPPORTED_CHARACTER, error_char);
		}

		if(*it == ' ' || *it == '\t' || *it == '\n' || *it == '\r')
		{
			it = str.erase(it);
		}
		else
		{
			it++;
		}
	}

	// If the first char of this string is '-', add '0' before '-'.
	i = 0;
	if(str[i] == '-')
	{
		str.insert(i, "0");
	}

	// Change structure "(-" into "(0-".
	while(i != str.size())
	{
		if(i+1 < str.size() && (str[i] == '(' && str[i+1] == '-'))
		{
			str.insert(i+1, "0");
		}
		i++;
	}

	// Add "#" in the tail of string.
	str = str + "#";
}

Formula::Token Formula::getNumber(const string& str, int& i)
{
	int i0 = i;
	while( isNumber(str[i]) )
	{
		i++;
	}
	string num = str.substr(i0, i-i0);
	if(num.back() == '.')
	{
		num.push_back('0');
	}

	return Token( std::stod(num) );
}

Formula::Token Formula::getWord(const string& str, int& i)
{
	int i_start = i;
	while( !isOperator(str[i]) )
	{
		i++;
	}

	Token token(str.substr(i_start, i-i_start));
	if(str[i] == '(')
	{
		token.type = Token::Function;
	}
	else
	{
		token.type = Token::Variable;
		m_found_variables.insert(token.name);
	}
	return token;
}

Formula::Token Formula::getToken(const string& str, int& i)
{
	if( isNumber(str[i]) )
	{
		return getNumber(str, i);
	}
	else if( isOperator(str[i]) )
	{
        i++;
		return Token(str[i-1]);
	}
	else
	{
		return getWord(str, i);
	}
}

void Formula::generatePostfix()
{
    stack<Token> operators;

    preprocess(m_formula_string);

    operators.push( Token("#") );

	int n = m_formula_string.size();
	int i = 0;
	Token token = getToken(m_formula_string, i);

    while(i < n || operators.top().name != "#")
	{
		if(token.type == Token::Error)
		{
			throw Exception(Exception::NOT_SUPPORTED_TOKEN, token.name);
		}
		
		if(token.type == Token::Number || token.type == Token::Variable)
		{
			m_postfix.push_back(token);
			token = getToken(m_formula_string, i);
		}
		else
		{
			int outer_priority = token.outerPriority();
			int inner_priority = operators.top().innerPriority();
			if( outer_priority > inner_priority )
			{
				operators.push( token );
				token = getToken(m_formula_string, i);
			}
			else if( outer_priority < inner_priority )
			{
				m_postfix.push_back( operators.top() );
				operators.pop();
			}
			else //if( outer_priority == inner_priority )
			{
				Token token_temp = operators.top();
				operators.pop();
				if( token_temp.name == "(" )
				{
					token = getToken(m_formula_string, i);
				}
			}
		}
	}
}
