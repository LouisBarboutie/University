#include "Particule.h"
#include <iostream>
#include <stdlib.h>
using namespace std;

Particule::Particule(float a, float b, float c)
{
    x = a;
    y = b;
    masse = c;
}

void Particule::deplace(float a, float b)
{
    x = x + a;
    y = y + b;
}

float Particule::getx(void)
{
    return x;
}

float Particule::gety(void)
{
    return y;
}