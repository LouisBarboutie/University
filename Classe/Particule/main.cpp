#include "Particule.h"
#include "ParticuleChargee.h"
#include <iostream>
#include <stdlib.h>
using namespace std;

int main(void)
{
    Particule electron;
    ParticuleChargee proton;
    electron.deplace(2,5);
    proton.deplace(6,7);
    cout << electron.getx() << "\n" << electron.gety()<< "\n";
    cout << proton.getx() << "\n" << proton.gety();
}