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

double fDerivative(double x)
{
    double yfDerivative = 1 + sin(x); // input f'(x)
    return yfDerivative; 
}




// ===== main =====
int main(void)
{
    // functions
    double f(double);
    double fDerivative(double);

    // variables
    double EPSILON = 0.001; // precision in decimal places
    double x0 = 0; // starting value
    double xVar; // variable x  
    double correction = (f(x0) / fDerivative(x0));

    // loop
    while (abs(correction) > EPSILON)
    {
        xVar = xVar - correction;
        correction = f(xVar) / fDerivative(xVar);
    }

    // output
    cout << xVar;
}