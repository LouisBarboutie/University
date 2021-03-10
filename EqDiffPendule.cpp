// EqDiffPendule v0.0.1

#include <stdlib.h>
#include <iostream>
using namespace std;
#include <vector>
#include <math.h>
#include <iomanip>

int main()
{
    // ===== initialisation =====

    // functions
    double f( double, double, double);
    double g( double, double);
    
    // number of points
    int n=10;

    // step of x
    double h=0.1;
    
    // pi
    double pi = acos(-1);
    
    // initial conditions
    double theta0 = pi/4, omega0 = 0, angularFreq = 25; // theta is angle, omega is angular velocity, angularFreq is w

    // intermediate variables
    double k1, k2, k3, k4, l1, l2, l3, l4;

    // vectors
    vector<double> elapsedTime, theta, omega, zeta;

    // vector initialisation
    elapsedTime.push_back ( 0);
    theta.push_back ( theta0);
    omega.push_back ( omega0);
    zeta.push_back (g( theta0, omega0));

    // ===== calculations =====
    // rk4 BTW

    // main loop

    for( int i=1; i<=n; i++)
    {
        // x values
        elapsedTime.push_back ( elapsedTime[i-1] + h);

        // runge-kutta order 4
        k1 = f(elapsedTime[i-1], theta[i-1], angularFreq);
        k2 = f(elapsedTime[i-1]+0.5*h, theta[i-1]+0.5*h*k1, angularFreq);
        k3 = f(elapsedTime[i-1]+0.5*h, theta[i-1]+0.5*h*k2, angularFreq);
        k4 = f(elapsedTime[i-1]+h, theta[i-1]+h*k3, angularFreq);
        zeta.push_back (theta[i-1]+(h/6)*(k1+2*k2+2*k3+k4));
        
        l1 = g(elapsedTime[i-1], omega[i-1]);
        l2 = g(elapsedTime[i-1]+0.5*h, omega[i-1]+0.5*h*k1);
        l3 = g(elapsedTime[i-1]+0.5*h, omega[i-1]+0.5*h*k2);
        l4 = g(elapsedTime[i-1]+h, omega[i-1]+h*k3);
        omega.push_back (omega[i-1]+(h/6)*(k1+2*k2+2*k3+k4));

        //debug
        cout << fixed << setprecision(9) << "timestamp = " << elapsedTime[i] << "\t" << "zeta = u = " << zeta[i] << "\t" << "omega = theta =" << omega[i] << "\n";
    }
}

// ===== functions =====

#include <math.h>
double f( double theta, double omega, double w)
{
    double resf;
    resf = -pow( w, 2) * sin( theta);
    return resf;
}

#include <math.h>
double g( double theta, double omega)
{
    double resg;
    resg = omega;
    return resg;
}