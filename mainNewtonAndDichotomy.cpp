// ===== libraries =====
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <iomanip>
using namespace std;



// ===== main =====
int main()
{
    double f(double);
    double dichotomyMethod( double, double, double);
    double fNewton( double, double);

    double a=0;
    double b=1;
    double epsilonNewton=0.000001;
    double epsilonDichotomy=0.000001;
    
    double resultDichotomy = dichotomyMethod(a,b,epsilonDichotomy);
    double resultNewton = fNewton(a,epsilonNewton);
    
    cout << resultNewton << "\n" << resultDichotomy;
    
}



// ===== libraries =====
#include <math.h>



// ===== functions =====
double f(double x)
{
    double yf = x - cos(x); // input f(x)
    return yf;
}



// ===== libraries =====
#include <stdlib.h>
#include <iostream>
#include <math.h>
using namespace std;

double fNewton(double xInitial, double epsilonNewton)
{
    // functions
    double f(double);
    double fDerivative(double);

    // variables
    double xVar; // variable x  
    double correction = (f(xInitial) / fDerivative(xInitial));

    // loop
    while (abs(correction) > epsilonNewton)
    {
        xVar = xVar - correction;
        correction = f(xVar) / fDerivative(xVar);
    }

    // output
    return xVar;
}



// ===== libraries =====
#include <stdlib.h>
#include <iostream>
#include <math.h>
using namespace std;

double dichotomyMethod(double a, double b, double epsilonDichotomy)
{
    // ===== functions =====
    double f(double);

    // variables
    double middle; // middle of interval
    double fLeft = f(a); // left value of function
    double fMiddle;
    
    // loop
    while (abs(b-a) > epsilonDichotomy)
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
    return middle;
}



// ===== libraries =====
#include<math.h>

double fDerivative(double x)
{
    double yfDerivative = 1 + sin(x); // input f'(x)
    return yfDerivative; 
}
