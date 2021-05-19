#include "Particule.h"
#include "ParticuleChargee.h"
#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <vector>
using namespace std;

int main(void)
{
    float x0 = 0;
    float x1 = 1;
    float dt = 0.00001;
    float E = 1;
    vector<float> xElectron, xProton, xVelocityElectron, xVelocityProton;
    
    float moveTime = 10;
    ParticuleChargee electron;
    ParticuleChargee proton(x0,0,9.1e-31 * 1836,1.6e-19);
    
    xElectron = electron.deplace(E,dt,x0,x1,moveTime);
    xProton = proton.deplace(E,dt,x0,x1,moveTime);
    
    xVelocityElectron = electron.getVelocity(xElectron, dt, moveTime);
    xVelocityProton = proton.getVelocity(xProton, dt, moveTime);
    
    cout << "temps" << "    " << "xElectron" << "           " << "xProton" << "         " << "xVelocityElectron" << "           " << "xVelocityProton" << "\n";

    for (int i=0; i<=moveTime; i++)
    {
        cout << fixed << setprecision(9) << "t = " << i << "\t" << xElectron[i] << "            " << xProton[i] << "            " << xVelocityElectron[i] << "         " << xVelocityProton[i] << "\n";
    }
    //cout << electron.getx() << "\n" << electron.gety()<< "\n";
    //cout << proton.getx() << "\n" << proton.gety();
    
    ;
}