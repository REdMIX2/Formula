#include <stack>
#include <cmath>
#include <math.h>

#include <formula.hpp>
#include "built_in.cpp"

using namespace std;
namespace formula
{
	using namespace token;
	vector<double> varargin2vector()
	{
		return vector<double>();
	}

	Formula::Formula() {}

	Formula::Formula(const string &str) : m_formula_string(str)
	{
		generatePostfix();
	}

	Formula::Formula(const char *str) : m_formula_string(std::string(str))
	{
		generatePostfix();
	}

	Formula &Formula::operator=(const string &str)
	{
		Formula temp(str);
		m_postfix = temp.m_postfix;
		m_formula_string = temp.m_formula_string;
		m_found_variables = temp.m_found_variables;

		return *this;
	}

	Formula &Formula::operator=(const char *str)
	{
		return (*this = string(str));
	}

	void Formula::check() const
	{
		if (m_postfix.empty())
		{
			return;
		}

		stack<double> operants;
		for (auto token = m_postfix.cbegin(); token != m_postfix.cend(); token++)
		{
			switch (token->type)
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
				if (token->name != "+" &&
					token->name != "-" &&
					token->name != "*" &&
					token->name != "/" &&
					token->name != "^")
				{
					throw Exception(Exception::WRONG_FORMAT, token->name);
				}

				if (operants.size() < 2)
				{
					throw Exception(Exception::NOT_ENOUGH_OPERANDS, token->name);
				}

				operants.pop();
				break;
			}
			case Token::Function:
			{
				if (operants.empty())
				{
					throw Exception(Exception::NOT_ENOUGH_OPERANDS, token->name);
				}

				operants.pop();

				if (m_defined_functions.count(token->name) == 0)
				{
					throw Exception(Exception::NOT_DEFINED_FUNCTION, token->name);
				}
				operants.push(0);
				break;
			}
			}
		}

		if (operants.size() != 1)
		{
			throw Exception(Exception::WRONG_FORMAT);
		}
	}

	Formula &Formula::input(const string &str_promt)
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

	bool Formula::empty() const
	{
		return m_postfix.empty();
	}

	void Formula::define(const string &var_name, double value)
	{
		m_defined_variables[var_name] = value;
	}

	void Formula::define(const string &func_name, const std::function<double(double)> &f)
	{
		m_defined_functions[func_name] = f;
	}

	double Formula::eval(const unordered_map<string, double> &variables)
	{
		using namespace formula;
		if (m_postfix.empty())
		{
			throw Exception(Exception::EMPTY_STRING);
		}

		stack<double> operants;

		double x, y;
		for (auto token = m_postfix.cbegin(); token != m_postfix.cend(); token++)
		{
			switch (token->type)
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
				if (variables.count(token->name) != 0)
				{
					operants.push(variables.at(token->name));
				}
				else if (m_defined_variables.count(token->name) != 0)
				{
					operants.push(m_defined_variables.at(token->name));
				}
				else if (valid_functions::s_built_in_variables.count(token->name) != 0)
				{
					operants.push(valid_functions::s_built_in_variables.at(token->name));
				}
				else
				{
					throw Exception(Exception::NOT_DEFINED_VARIABLE, token->name);
				}
				break;
			}
			case Token::Operator:
			{
				if (token->name != "+" &&
					token->name != "-" &&
					token->name != "*" &&
					token->name != "/" &&
					token->name != "^")
				{
					throw Exception(Exception::WRONG_FORMAT);
				}

				if (operants.size() < 2)
				{
					throw Exception(Exception::NOT_ENOUGH_OPERANDS, token->name);
				}

				y = operants.top();
				operants.pop();

				x = operants.top();
				operants.pop();

				if (token->name == "+")
					operants.push(x + y);
				else if (token->name == "-")
					operants.push(x - y);
				else if (token->name == "*")
					operants.push(x * y);
				else if (token->name == "/")
				{
					if (valid_functions::isZero(y))
					{
						throw Exception(Exception::DIVIDIED_BY_ZERO);
					}
					operants.push(x / y);
				}
				else // if(token->name == "^")
				{
					if (valid_functions::isZero(x) && y < 0)
					{
						throw Exception(Exception::DIVIDIED_BY_ZERO);
					}
					operants.push(pow(x, y));
				}

				break;
			}
			case Token::Function:
			{
				if (operants.empty())
				{
					throw Exception(Exception::NOT_ENOUGH_OPERANDS, token->name);
				}

				x = operants.top();
				operants.pop();

				if (m_defined_functions.count(token->name) != 0)
				{
					operants.push(m_defined_functions[token->name](x));
				}
				else if (valid_functions::s_built_in_functions.count(token->name) != 0)
				{
					operants.push(valid_functions::s_built_in_functions.at(token->name)(x));
				}
				else
				{
					throw Exception(Exception::NOT_DEFINED_FUNCTION, token->name);
				}
			}
			}
		}

		if (operants.size() != 1)
		{
			throw Exception(Exception::WRONG_FORMAT);
		}
		else
		{
			if (fabs(operants.top()) <= 1E-6)
			{
				return 0;
			}
			else
			{
				return operants.top();
			}
		}
	}

	double Formula::eval(const vector<double> &vector_variables)
	{
		unsigned i = 0;
		unordered_map<string, double> variables;
		for (const string &name : m_found_variables)
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

	double Formula::operator()(const unordered_map<string, double> &variables)
	{
		return eval(variables);
	}

	double Formula::operator()(const vector<double> &vector_variales)
	{
		return eval(vector_variales);
	}

	void Formula::preprocess(string &str)
	{
		// TODO Refactor code
		unsigned i = 0;

		// Delete all spaces in string.
		for (auto it = str.begin(); it != str.end();)
		{
			if (Token::s_supported_chars.count(*it) == 0)
			{
				string error_char;
				error_char.push_back(*it);
				throw Exception(Exception::NOT_SUPPORTED_CHARACTER, error_char);
			}

			if (*it == ' ' || *it == '\t' || *it == '\n' || *it == '\r')
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
		if (str[i] == '-')
		{
			str.insert(i, "0");
		}

		// Change structure "(-" into "(0-".
		while (i != str.size())
		{
			if (i + 1 < str.size() && (str[i] == '(' && str[i + 1] == '-'))
			{
				str.insert(i + 1, "0");
			}
			i++;
		}

		// Add "#" in the tail of string.
		str = str + "#";
	}

	Token Formula::getNumber(const string &str, int &i)
	{
		int i0 = i;
		while (Token::isNumber(str[i]))
		{
			i++;
		}
		string num = str.substr(i0, i - i0);
		if (num.back() == '.')
		{
			num.push_back('0');
		}

		return Token(std::stod(num));
	}

	Token Formula::getWord(const string &str, int &i)
	{
		int i_start = i;
		while (!Token::isOperator(str[i]))
		{
			i++;
		}

		Token token(str.substr(i_start, i - i_start));
		if (str[i] == '(')
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

	Token Formula::getToken(const string &str, int &i)
	{
		if (Token::isNumber(str[i]))
		{
			return getNumber(str, i);
		}
		else if (Token::isOperator(str[i]))
		{
			i++;
			return Token(str[i - 1]);
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

		operators.push(Token("#"));

		int n = m_formula_string.size();
		int i = 0;
		Token token = getToken(m_formula_string, i);
		// Refactor code
		while (i < n || operators.top().name != "#")
		{
			if (token.type == Token::Error)
			{
				throw Exception(Exception::NOT_SUPPORTED_TOKEN, token.name);
			}

			if (token.type == Token::Number || token.type == Token::Variable)
			{
				m_postfix.push_back(token);
				token = getToken(m_formula_string, i);
			}
			else
			{
				int outer_priority = token.outerPriority();
				int inner_priority = operators.top().innerPriority();
				if (outer_priority > inner_priority)
				{
					operators.push(token);
					token = getToken(m_formula_string, i);
				}
				else if (outer_priority < inner_priority)
				{
					m_postfix.push_back(operators.top());
					operators.pop();
				}
				else // if( outer_priority == inner_priority )
				{
					Token token_temp = operators.top();
					operators.pop();
					if (token_temp.name == "(")
					{
						token = getToken(m_formula_string, i);
					}
				}
			}
		}
	}
	ostream &operator<<(ostream &o, const formula::Formula &f)
	{
		o << f.m_formula_string;
		return o;
	}

	istream &operator>>(istream &i, formula::Formula &f)
	{
		string str;

		getline(cin, str);

		f = str;
		f.check();

		return i;
	}
};
