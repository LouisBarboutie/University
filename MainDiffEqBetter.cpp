#include <iostream>
#include<math.h>
#include <vector> // tableau de nombres en C++
using namespace std;
int main()
{
    double f(double,double);
    int n=100; // nombre de points
    double h=0.1,x0=0,y0=0; // pas des x et conditions initiales 
    vector<double> x,y,z; // x est un vecteur de doubles
    x.push_back(0); // on "pousse" 0 dans x
    y.push_back(0); 
    z.push_back(0); // z solution exacte
    // le point signifie qu'on appelle la méthode push_back à l'objet y
    
    for (int i=1; i<=n; i++)
    {
        x.push_back(x[i-1]+h); // on met n valeurs dans le tableau x 
        y.push_back(y[i-1]+0.5*h*(f(x[i-1],y[i-1])+f(x[i-1]+h,y[i-1]+h*f(x[i-1],y[i-1]))));  // algo euler: y_(i+1) = y_i + h * f(x_i,y_i)
        z.push_back(-x[i]-1+(1+x0+y0)*exp(x[i]-x0));
        cout << "x[" << i << "] = " << x[i] << "\t" <<"y[" << i << "] = " << y[i] << "\t" << "z[" << i << "] = " << z[i] << "\t" << "erreur relative = " << (z[i]-y[i])/z[i] << "\n";
    }
}

/* 
 * y'-y=x
 * solution exacte: y=-x_i-1+(1+x_0+y_0)*exp(x_i-x_0)
 * méthode: séparer axe (Ox) en intervalles x_i
 * h = x_i+1 - x_i 
 * on s'arrête à 1, CI y(0)=0
 */