#include "ParticuleChargee.h"
#include "Particule.h"
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <math.h>
using namespace std;

ParticuleChargee::ParticuleChargee(float a, float b, float c, float d):Particule(a,b,c)
{
    charge = d;
}

vector<float> ParticuleChargee::deplace(float E, float dt, float x0, float x1, float moveTime)
{

    xPos.push_back(x0);
    xPos.push_back(x1);
    
    for (int i = 2; i <= moveTime; i++)
    {
        xPos.push_back(charge * E / masse * pow(dt,2) + 2*xPos[i-1] - xPos[i-2]);
    }
    
    return xPos;
}

vector<float> ParticuleChargee::getVelocity( vector<float> a, float dt, float moveTime)
{
    vector<float> velocity;
    for (int i = 0; i <= moveTime; i++)
    {
        velocity.push_back((a[i+1]-a[i])/dt/3e8);
    }
    return velocity;
}