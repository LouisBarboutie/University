#include<math.h>
double deltaEuler(double yEuler, double yExact)
{
  double resultat;
  resultat = abs (((yEuler - yExact) / yExact) * 100);
  return (resultat);
}