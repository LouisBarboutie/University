#include <math.h>
double yNext(double yBarre, double x, double y, double h)
{
  double f (double, double);
  double resultat;
  resultat = y + h * 0.5 * (f (x, y) + f (x + h, yBarre));
  return (resultat);
}
