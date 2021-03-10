#include <math.h>
double yNextTemp(double x_i, double y_i, double h)
{
  double f (double, double);
  double resultat;
  resultat = y_i + h * f (x_i, y_i);
  return (resultat);
}