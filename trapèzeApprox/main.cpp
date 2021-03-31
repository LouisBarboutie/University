#include <stdlib.h>
#include <iostream>
using namespace std;
#include <vector>
#include <math.h>
#include <iomanip>

/* 
 * Algo:
 * use f=x^2 from 0 to 1
 * set h to xMax-xMin / m
 * set m amount of points
 * set integral limits
 * get f from 0 to m
 * do calc
 */

 int main(void)
 {
    /*
     * functions
     */
    
    double f(double);
    
    /*
     * initialisation
     */
    
    double lowerLimit = 0, upperLimit = 1, pointAmount = 10000, integralValue;
    double h = ((upperLimit - lowerLimit) / pointAmount);
    vector<double> xVariable;

    xVariable.push_back (lowerLimit);

    /*
     * calculations
     */

    integralValue = h * 0.5 * ( f(upperLimit) + f(lowerLimit));

    for ( int i = 1; i < pointAmount; i++)
    {
        xVariable.push_back (xVariable[i-1]+h) ;
        integralValue = ( integralValue + h * f(xVariable[i]));
    }

    /*
     * output
     */

     cout << integralValue ;

 }