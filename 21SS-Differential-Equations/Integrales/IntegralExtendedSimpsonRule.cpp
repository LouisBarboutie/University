// ===== main =====
#include <stdlib.h>
#include <iostream>
using namespace std;
#include <vector>
#include <math.h>
#include <iomanip>

int main(void)
{
    /*
     * functions
     */

    double f(double);

    /*
     * variables
     */

    double lowerLimit = 0 , upperLimit = 1; // integral limits
    double integralValue = 0; // calculated integral
    double n=10; // point amount
    double xVar; // x variable
    double I, J;


    /*
     * calculations
     */

    // step value
    double h = (upperLimit - lowerLimit) / n;

    // Loop 2n
    xVar = lowerLimit;
    for ( int i=2; i <= n-1; i = i+2 )
    {
        xVar = xVar + 2*h;
        I = ( I + f(xVar));
    }

    // Loop 2n+1
    xVar = lowerLimit - h;
    for ( int i=1; i <= n-1; i = i+2 )
    {
        xVar = xVar + 2*h;
        J = ( J + f(xVar));
    }
    
    // integral
    integralValue = h * ( f(lowerLimit) + f(upperLimit) + 4.*J + 2.*I ) / 3.;

    /*
     * output
     */ 

    cout << h << "\n" << integralValue;
}

// ===== functions =====
#include <stdlib.h>
#include <iostream>
using namespace std;
#include <vector>
#include <math.h>
#include <iomanip>
  
double f( double x)
{
    double resf = pow(x,2);
    return resf;
}