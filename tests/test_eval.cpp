#include <catch2/catch_test_macros.hpp>
#include <formula.hpp>
using namespace formula;
#include <math.h>

#define PRECISION 5e-11

template<typename T1,typename T2>
double evalFormula(std::string formula, T1 x, T2 y)
{
    Formula f = formula;
    return f(x, y);
}

TEST_CASE("Eval in int formula", "[eval]")
{
    REQUIRE(abs(evalFormula("sin(x)^2 + 0.65*y + 3 / tan(pi/4)", 3, -5) - (-0.2300851433)) <= PRECISION);
}