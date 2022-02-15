# Formula -- A math expression evaluation tool for C++

**Formula** can convert math expression(with variables) string into callable object. Firstly, let's see an example:
```c++
#include "formula.h"

using namespace std;

int main()
{
	// set f with an expression without variables
	Formula f = "sin(pi/12)^2 + 0.65*(-8.32 + 9) + 3 / tan(pi/4)";
	cout << f() << endl;

	// set f with an expression with 2 varialbes x and y
	f = "sin(x)^2 + 0.65*y + 3 / tan(pi/4)";
	cout << f(3, -5) << endl;

	return 0;
}
```

Let's see full usage of **Formula**

## Get a formula from string
You can convert string to evaluatable `Formula` in following way:
* Assign `std::string` or `char *` to formula:
	```c++
	Formula f = "x + a0";
	```
* Get formula from user input: 
	```c++
	Formula f;
	cin >> f;
	```
	or
	```c++
	Formula f;
	f.input("f(x) = ");
	```
The string can contains variables and one-parameter-functions.

## Evaluate a formula
* Call `Formula` object `f` with positional arguments, it will return a double result. And arguments order should follow variabel names' dictionary order.
	```c++
	double result;
	result = f(); // if Formula f has no variables
	result = f(0.3, 2.9); // if Formula f has 2 variabels
	```
* Call `Formula` object `f` with `std::vector<double>`, it will return a double result. And vector elements order should follow variabel names' dictionary order.
	```c++
	double result = f({0.3, 2.9});
	```
* Call `Formula` object `f` with `std::unordered_map<std::string, double>` to set each variable's value, it will return a double result.
	```c++
	double result = f({"x": 0.3, "y": 2.9});
	```
* Use `eval` methods just like call the object, it will return a double result:
	```c++
	double result;
	result = f.eval();
	result = f.eval(0.3, 2.9);
	result = f.eval({0.3, 2.9});
	result = f.eval({"x": 0.3, "y": 2.9});
	```

## Supported operators and functions
**Formula** only support following operators: `+ - * / ^` just like pure math do.

And by default, **Formula** support following one-parameter-functions:
* sin, cos, tan, csc, sec, cot
* asin, acos, atan, acsc, asec, acot
* arcsin, arccos, arctan, arccsc, arcsec, arccot
* sinh, cosh, tanh, csch, sech, coth
* asinh, acosh, atanh, acsch, asech, acoth
* arcsinh, arccosh, arctanh, arccsch, arcsech, arccoth
* exp, log, lg, log10, ln, log2
* sqrt, abs, fabs, sign, sgn

But you can define your own function by using following method:
```c++
void Formula::define(const std::string& function_name, const std::function<double(double)>& func)
```

In the same way, if you want to pre-define a variable for `Formula` to use, please call following method:
```c++
void Formula::define(const std::string& variable_name, double value);
```

By the way, there are 3 built-in constante:
* `PI` and `pi` is defined as `4*atan(1)`;
* `e` is defined as `exp(1)`;

For example,
```c++
Formula f = "sinc(x)/tau";

std::function<double(double)> sinc = [=](double x)->double
{
	return sin(x)/x;
};

f.define("sinc", sinc);
f.define("tau", 0.5);
double result = f(0.2);
```

## Assistant methods
* Use `bool Formula::empty()const` method to check a `Formula` object `f` is valid or not, it will return `true` if `f` is not a valid `Formula`;
* Use `void Formula::check()const` method to throw exception if `Formula` object `f` is not valid;
* Use `void Formula::clear()` method to clear a `Formula` object;
* Use `cout << f;` to print a `Formula` object `f`'s expression string.

## Example
Let's make a calculator with `Formula`:
```c++
#include "formula.h"

#include <iostream>
#include <string>

using namespace std;

int main()
{
	Formula f;
	while(true)
	{
		cout << ">> ";
		string formula_str;
		getline(cin, formula_str);
		if(formula_str == "exit")
		{
			break;
		}

		try
		{
			f = formula_str;
			cout << f.eval() << endl;
		}
		catch(const Formula::Exception& e)
		{
			cout << e.message() << endl;
		}
	}

	return 0;
}
```
Very easy to use, right?

## User Function Interface

Public functions:

`Formula::Formula()`
Construct an empty `Formula` object.

`Formula::Formula(const std::string& str)`
Construct a `Formula` object with expression string `str`.

`Formula::Formula(const char* str)`
Construct a `Formula` object with expression string `str`.

`Formula& Formula::operator =(const std::string& str)`
Assign expression string to current `Formula` object. This will cover old formula content.

`Formula& Formula::operator =(const char* str)`
Assign expression string to current `Formula` object. This will cover old formula content.

`Formula& Formula::input(const std::string& prompt)`
Cover current `Formula` object with terminal user input. And `prompt` is the input prompt.

`void Formula::clear()`
Clear current `Formula` object.

`bool Formula::empty()const`
If current `Formula` object is not a valid formula, it will return `true`, otherwise return `false`.

`void Formula::check()const`
If current `Formula` object is not a valid formula, it will throw an instance of `Formula::Exception`.

`double Formula::eval(const std::unordered_map<std::string, double>& variables)`
Evaluate current `Formula` object with variable setting as `variables` defined.

`double Formula::eval(const std::vector<double>& variables)`
Evaluate current `Formula` object with variable setting as `variables` defined. The vector `variables`' order must follow variables in expression string's dictionary order.

`template<typename ... DataTypes> double Formula::eval(DataTypes ... variables)`
Evaluate current `Formula` object with variable setting as `variables` defined. The order of double list `variables` must follow variables in expression string's dictionary order.

`double Formula::operator ()(const std::unordered_map<std::string, double>& variables)`
Evaluate current `Formula` object with variable setting as `variables` defined.

`double Formula::operator ()(const std::vector<double>& variables)`
Evaluate current `Formula` object with variable setting as `variables` defined. The vector `variables`' order must follow variables in expression string's dictionary order.

`template<typename ... DataTypes> double Formula::operator ()(DataTypes ... variables)`
Evaluate current `Formula` object with variable setting as `variables` defined. The order of double list `variables` must follow variables in expression string's dictionary order.

`void Formula::define(const std::string& var_name, double value)`
Pre-define a variable with name `var_name` and value `value`. When evaluate the `Formula` object, you won't need to set this variable again.

`void Formula::define(const std::string& func_name, const std::function<double(double)>& f)`
Define a function with name `func_name` and real content `f`. When evaluate the `Formula` object, the word `func_name` will be parsed correctly as a function name and will work just like `f` defines.