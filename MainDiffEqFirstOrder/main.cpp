/* MainDiffEqFirstOrder v0.0.1
 * language: C++
 * author: Louis-Hendrik Barboutie
 * date: 10/03/2021
 * license: CC BY-NC 4.0
 *
 * original code by Marie Hagenbourger, no license provided
 *
 * Changelog:
 * rearranged code for better readability and structure, uniformized variable names in functions
 * redundant variable "ySuiv" deleted, as it is just the value you push into "y" 
 * renamed "y" to "yEuler", "y_exact" to "yExact", "f_exact" to "fExact", "y_suiv" to "yNext", "y_barre" to "yNextTemp", "ecart_rel_Euler" to "deltaEuler", "ecart_rel_Heun" to "deltaHeun", "ecart" to "yEulerDelta", "ySuiv_ecart" to "yHeunDelta"
 */

#include <stdlib.h>
#include <iostream>
using namespace std;
#include <vector>
#include <math.h>
#include <iomanip>

int main(void)
{
  //  ===== initialisation =====
  
  // functions
  double f (double, double);
  double fExact (double, double, double);
  double deltaEuler (double, double);
  double deltaHeun (double, double);
  double yNext (double, double, double, double);

  // number of points
  int n = 10;			

  // step of x
  double h = 0.1;
  
  // initial conditions
  double x0=0, y0=1, yBarre;
  
  // vector definition
  vector<double> x, yHeun, yExact, yEuler, yHeunDelta, yEulerDelta;
  
  // vector initialisation
  x.push_back (x0);
  yHeun.push_back (y0);
  yExact.push_back (y0);
  yEuler.push_back (y0);
  yHeunDelta.push_back (y0);
  yEulerDelta.push_back (y0);
  
  

  // ===== calculations =====
  
  // main loop
  for (int i = 1; i <= n; i++)
    {
      // x values
      x.push_back (x[i - 1] + h);
      
      // exact solution
      yExact.push_back(fExact(x[i], x0, y0)); // important to place after x.push_back, if before, argument of x needs to be i+1 (untested)
      
      // old school euler
      yEuler.push_back (yEuler[i - 1] + h * f (x[i - 1], yEuler[i - 1])); 
      
      // rock'n roll euler
      yBarre = yHeun[i-1]+h*f(x[i - 1], yHeun[i - 1]);
      yHeun.push_back (yHeun[i-1]+h*0.5*(f(x[i - 1], yHeun[i - 1]) + f( x[i], yBarre)));
      
      // relative difference
      yHeunDelta.push_back (deltaHeun (yHeun[i], yExact[i]));
      yEulerDelta.push_back (deltaEuler (yEuler[i], yExact[i]));
      
      // debug
      cout << fixed << setprecision(8) << "\t" <<  "yExact = " << yExact[i] << "\t" << "yEuler = " << yEuler[i] << "\t" << "yHeun = " << yHeun[i] << "\t" << "yHeunDelta = " << yHeunDelta[i] << "\t" << "yEulerDelta = " << yEulerDelta[i] << "\n";
    }
}



// ===== functions =====

/*
 * double f (double x, double y)
 * {
 *    double resultat;
 *    resultat = x + y;
 *    return (resultat);
 * }
 *
 * ===== ===== =====
 *
 * #include <math.h>
 * double fExact(double x, double x0, double y0)
 * {
 * double resultat;
 * resultat = -x-1+(1+x0+y0)*exp(x-x0);
 * return (resultat);
 * }
 *
 * ===== ===== =====
 *
 * #include <math.h>
 * double yNextTemp(double x_i, double y_i, double h)
 * {
 * double f (double, double);
 * double resultat;
 * resultat = y_i + h * f (x_i, y_i);
 * return (resultat);
 * }
 *
 * ===== ===== =====
 *
 * #include <math.h>
 * double yNext(double yBarre, double x, double y, double h)
 * {
 * double f (double, double);
 * double resultat;
 * resultat = y + h * 0.5 * (f (x, y) + f (x + h, yBarre));
 * return (resultat);
 * }
 *
 * ===== ===== =====
 *
 * #include<math.h>
 * double deltaHeun(double yHeun, double yExact)
 * {
 * double resultat;
 * resultat = abs (((yHeun - yExact) / yExact) * 100);
 * return (resultat);
 * }
 *
 * ===== ===== =====
 *
 * #include<math.h>
 * double deltaEuler(double yEuler, double yExact)
 * {
 * double resultat;
 * resultat = abs (((yEuler - yExact) / yExact) * 100);
 * return (resultat);
 * }
 */

