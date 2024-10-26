#ifndef __FORMULA_H__
#define __FORMULA_H__

#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <exception>
#include <functional>


#ifdef _MSC_VER
class __declspec(dllexport) Formula
#else
class Formula
#endif
{
public:
    Formula();
	Formula(const std::string& str);
	Formula(const char* str);

    Formula& operator =(const std::string& str);
    Formula& operator =(const char* str);
    Formula& input(const std::string& str_promt);
	
	void clear();
	bool empty()const;
	void check()const;

	double eval(const std::unordered_map<std::string, double>& variables);
    double eval(const std::vector<double>& variables);
	template<typename ... DataTypes>
	double eval(DataTypes ... rest);

	double operator ()(const std::unordered_map<std::string, double>& variables);
    double operator ()(const std::vector<double>& variables);
	template<typename ... DataTypes>
	double operator ()(DataTypes ... rest);

	void define(const std::string& var_name, double value);
	void define(const std::string& func_name, const std::function<double(double)>& f);

	friend std::ostream& operator <<(std::ostream& out_stream, const Formula& f);
	friend std::istream& operator >>(std::istream& in_stream, Formula& f);

public:
	class Exception: public std::exception
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

		Exception(Type code = UNKNOWN, const std::string& _message = "", double _value = 0.0, const std::string& _interval = "");
        const char* what() const throw();
        std::string message()const;
        Type type()const;

    private:
        Type m_type;
		std::string m_message;
	};

private:
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
		Token(const std::string& _type);
	    Token(char ch);
		Token(double _data);
		Token(Type _type, const std::string& _name, double _data);
		int innerPriority()const;
		int outerPriority()const;
	};

private:
    static void preprocess(std::string& str);
    static Token getNumber(const std::string& str, int& i);
    Token getWord(const std::string& str, int& i);
    Token getToken(const std::string& str, int& i);
    void generatePostfix();

private:
	static const std::unordered_set<char> s_supported_chars;
	static const std::unordered_map<std::string, double> s_built_in_variables;
	static const std::unordered_map<std::string, std::function<double(double)> > s_built_in_functions;

	std::vector<Token> m_postfix;
	std::string m_formula_string;

	std::set<std::string> m_found_variables;

	std::unordered_map<std::string, double> m_defined_variables;
    std::unordered_map<std::string, std::function<double(double)> > m_defined_functions;
};

std::vector<double> varargin2vector();

template<typename ... DataTypes>
std::vector<double> varargin2vector(double first, DataTypes ... rest)
{
	std::vector<double> variables(1, first), variables_rest;
	variables_rest = varargin2vector(rest...);
	variables.insert(variables.end(), variables_rest.begin(), variables_rest.end());

	return variables;
}

template<typename ... DataTypes>
double Formula::eval(DataTypes... varargin)
{
    std::vector<double> variables = varargin2vector(varargin...);
    return eval(variables);
}

template<typename ... DataTypes>
double Formula::operator ()(DataTypes... varargin)
{
    return eval(varargin...);
}
#endif // !_MSC_VER