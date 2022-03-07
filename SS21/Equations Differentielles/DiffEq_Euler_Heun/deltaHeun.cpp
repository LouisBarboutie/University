#include<math.h>
double deltaHeun(double yHeun, double yExact)
{
  double resultat;
  resultat = abs (((yHeun - yExact) / yExact) * 100);
  return (resultat);
}