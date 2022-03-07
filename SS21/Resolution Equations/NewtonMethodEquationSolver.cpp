// ===== libraries =====
#include <stdlib.h>
#include <iostream>
#include <math.h>
using namespace std;



// ===== functions =====
double f(double x)
{
    double yf =  // input f(x)
    return yf;
}

double fDerivative(double x)
{
    double yfDerivative = // input f'(x)
    return yfDerivative; 
}




// ===== main =====
int main(void)
{
    // functions
    double f(double);
    double fDerivative(double);  

    // variables
    double n = 10; // amount of steps
    double x0 = 0; // starting value
    double xVar; // variable x
    
    // loop
    for(int i = 1; i <= n; i++ )
    {
        xVar = xVar - ( f(xVar) / fDerivative(xVar) ); // xn+1 = xn - f/f'
    }

    // output
    cout << xVar;
}