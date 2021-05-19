#include "Particule.h"
#include "ParticuleChargee.h"
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <math.h>
using namespace std;

Particule::Particule(float a, float b, float c)
{
    x = a;
    y = b;
    masse = c;
}

//float Particule::getx(void)
//{
//    return x;
//}

//float Particule::gety(void)
//{
//    return y;
//}