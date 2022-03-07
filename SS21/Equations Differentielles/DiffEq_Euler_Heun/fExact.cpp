#include <math.h>
double fExact(double x, double x0, double y0)
{
  double resultat;
  resultat = -x-1+(1+x0+y0)*exp(x-x0);
  return (resultat);
}
