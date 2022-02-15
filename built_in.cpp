#include "formula.h"
#include <cmath>

static bool isZero(double x)
{
    return (fabs(x) < 1E-6);
}

static double _sign(double x)
{
    if (x > 0)
    {
        return 1;
    }
    else if (x < 0)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

static double _tan(double x)
{
	if(isZero(cos(x)))
	{
		throw Formula::Exception(Formula::Exception::OUT_OF_RANGE, "tan", x, "cos(x) != 0");
	}
	return tan(x);
}

static double _csc(double x)
{
	if(isZero(sin(x)))
	{
		throw Formula::Exception(Formula::Exception::OUT_OF_RANGE, "csc", x, "sin(x) != 0");
	}
	return 1.0 / sin(x);
}

static double _sec(double x)
{
	if(isZero(cos(x)))
	{
		throw Formula::Exception(Formula::Exception::OUT_OF_RANGE, "sec", x, "cos(x) != 0");
	}
	return 1.0 / cos(x);
}

static double _cot(double x)
{
	if(isZero(sin(x)))
	{
		throw Formula::Exception(Formula::Exception::OUT_OF_RANGE, "cot", x, "sin(x) != 0");
	}
	return cos(x) / sin(x);
}

static double _asin(double x)
{
	if(!(x >= -1 && x <= 1))
	{
		throw Formula::Exception(Formula::Exception::OUT_OF_RANGE, "asin", x, "x >= -1 && x <= 1");
	}
	return asin(x);
}

static double _acos(double x)
{
	if(!(x >= -1 && x <= 1))
	{
		throw Formula::Exception(Formula::Exception::OUT_OF_RANGE, "acos", x, "x >= -1 && x <= 1");
	}
	return acos(x);
}

static double _acsc(double x)
{
	if(!(x <= -1 || x >= 1))
	{
		throw Formula::Exception(Formula::Exception::OUT_OF_RANGE, "acsc", x, "x <= -1 || x >= 1");
	}
	return asin(1.0 / x);
}

static double _asec(double x)
{
	if(!(x <= -1 || x >= 1))
	{
		throw Formula::Exception(Formula::Exception::OUT_OF_RANGE, "asec", x, "x <= -1 || x >= 1");
	}
	return acos(1.0 / x);
}

static double _acot(double x)
{
	if(x < 0)
	{
		return 4*atan(1) + atan(1.0/x);
	}
	else if(x > 0)
	{
		return atan(1.0/x);
	}
	else
	{
		return 2*atan(1);
	}
}

static double _csch(double x)
{
	if(isZero(x))
	{
		throw Formula::Exception(Formula::Exception::OUT_OF_RANGE, "csch", x, "x != 0");
	}
	return 1 / sinh(x);
}

static double _sech(double x)
{
	return 1 / cosh(x);
}

static double _coth(double x)
{
	if(isZero(x))
	{
		throw Formula::Exception(Formula::Exception::OUT_OF_RANGE, "coth", x, "x != 0");
	}
	return 1 / tanh(x);
}

static double _acosh(double x)
{
	if(!(x >= 1))
	{
		throw Formula::Exception(Formula::Exception::OUT_OF_RANGE, "acosh", x, "x >= 1");
	}
	return acosh(x);
}

static double _atanh(double x)
{
	if(!(x > -1 && x < 1))
	{
		throw Formula::Exception(Formula::Exception::OUT_OF_RANGE, "atanh", x, "x > -1 && x < 1");
	}
	return atanh(x);
}

static double _acsch(double x)
{
	if(!(x > -1 && x < 1))
	{
		throw Formula::Exception(Formula::Exception::OUT_OF_RANGE, "acsch", x, "x > -1 && x < 1");
	}
	return log( (1 + _sign(x) * sqrt(1 + x*x)) / x );
}

static double _asech(double x)
{
	if(!(x > 0 && x <= 1))
	{
		throw Formula::Exception(Formula::Exception::OUT_OF_RANGE, "asech", x, "x > 0 && x <= 1");
	}
	return log( (1 + sqrt(1 - x*x)) / x );
}

static double _acoth(double x)
{
	if(!(x < -1 || x > 1))
	{
		throw Formula::Exception(Formula::Exception::OUT_OF_RANGE, "acoth", x, "x < -1 || x > 1");
	}
	return 0.5 * log( (x+1)/(x-1) );
}

static double _log(double x)
{
	if(!(x > 0))
	{
		throw Formula::Exception(Formula::Exception::OUT_OF_RANGE, "log", x, "x > 0");
	}
	return log(x);
}

static double _log10(double x)
{
	if(!(x > 0))
	{
		throw Formula::Exception(Formula::Exception::OUT_OF_RANGE, "log10", x, "x > 0");
	}
	return log10(x);
}

static double _log2(double x)
{
	if(!(x > 0))
	{
		throw Formula::Exception(Formula::Exception::OUT_OF_RANGE, "log2", x, "x > 0");
	}
	return log2(x);
}

static double _sqrt(double x)
{
	if(!(x >= 0))
	{
		throw Formula::Exception(Formula::Exception::OUT_OF_RANGE, "sqrt", x, "x >= 0");
	}
	return sqrt(x);
}

#define FUNCTION(func_name) [=](double x)->double{return func_name(x);}

const std::unordered_map<std::string, std::function<double(double)> > Formula::s_built_in_functions = 
{
	{"sin", FUNCTION(sin)},
	{"cos", FUNCTION(cos)},
	{"tan", FUNCTION(_tan)},
    {"csc", FUNCTION(_csc)},
    {"sec", FUNCTION(_sec)},
    {"cot", FUNCTION(_cot)},

	{"asin", FUNCTION(_asin)},
	{"acos", FUNCTION(_acos)},
	{"atan", FUNCTION(atan)},
	{"acsc", FUNCTION(_acsc)},
	{"asec", FUNCTION(_asec)},
	{"acot", FUNCTION(_acot)},

    {"arcsin", FUNCTION(_asin)},
    {"arccos", FUNCTION(_acos)},
    {"arctan", FUNCTION(atan)},
    {"arccsc", FUNCTION(_acsc)},
    {"arcsec", FUNCTION(_asec)},
    {"arccot", FUNCTION(_acot)},

	{"sinh", FUNCTION(sinh)},
	{"cosh", FUNCTION(cosh)},
	{"tanh", FUNCTION(tanh)},
	{"csch", FUNCTION(_csch)},
	{"sech", FUNCTION(_sech)},
	{"coth", FUNCTION(_coth)},

	{"asinh", FUNCTION(asinh)},
	{"acosh", FUNCTION(_acosh)},
	{"atanh", FUNCTION(_atanh)},
	{"acsch", FUNCTION(_acsch)},
	{"asech", FUNCTION(_asech)},
	{"acoth", FUNCTION(_acoth)},

    {"arcsinh", FUNCTION(asinh)},
    {"arccosh", FUNCTION(_acosh)},
    {"arctanh", FUNCTION(_atanh)},
    {"arccsch", FUNCTION(_acsch)},
    {"arcsech", FUNCTION(_asech)},
    {"arccoth", FUNCTION(_acoth)},

	{"exp", FUNCTION(exp)},
	{"log", FUNCTION(_log)},
	{"lg", FUNCTION(_log10)},
	{"log10", FUNCTION(_log10)},
	{"ln", FUNCTION(_log)},
	{"log2", FUNCTION(_log2)},

	{"sqrt", FUNCTION(_sqrt)},
	{"abs", FUNCTION(fabs)},
	{"fabs", FUNCTION(fabs)},
    {"sign", FUNCTION(_sign)},
    {"sgn", FUNCTION(_sign)},
};

#undef FUNCTION

const std::unordered_map<std::string, double> Formula::s_built_in_variables = 
{
	{"PI", 4*atan(1)},
	{"pi", 4*atan(1)},
	{"e", exp(1)}
};