#include <formula.hpp>
// using namespace formula;
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