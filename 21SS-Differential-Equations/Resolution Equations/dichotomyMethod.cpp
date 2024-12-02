// ===== libraries =====
#include <stdlib.h>
#include <iostream>
#include <math.h>
using namespace std;



// ===== functions =====
double f(double x)
{
    double yf = x - cos(x); // input f(x)
    return yf;
}



// ===== main =====
int main(void)
{
    // functions
    double f(double);

    // variables
    double epsilon = 0.001; // precision
    double a = 0 ; // left x value
    double b = 10; // right x value
    double middle; // middle of interval
    double fLeft = f(a); // left value of function
    double fMiddle;

    // loop
    while (abs(b-a) > epsilon)
    {
        middle = ( a + b ) / 2;
        fMiddle = f(middle);

        if (fLeft * fMiddle < 0)
        {
            b = middle;
        }

        else
        {
            a = middle;
            fLeft = fMiddle;
        }
    }

    // output
    cout << middle;
}



