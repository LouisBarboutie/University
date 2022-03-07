#include "ParticuleChargee.h"
#include "Particule.h"
#include <iostream>
#include <stdlib.h>
using namespace std;

ParticuleChargee::ParticuleChargee(float a, float b, float c, float d):Particule(a,b,c)
{
    charge = d;
}