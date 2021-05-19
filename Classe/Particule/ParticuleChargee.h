#ifndef PARTICULECHARGEE_H
#define PARTICULECHARGEE_H
#include "Particule.h"

class ParticuleChargee:public Particule
{
    private: 
    float charge;
    vector<float> xPos;

    public: 
    ParticuleChargee(float = 0, float = 0, float = 9.1e-31, float = -1.6e-19);
    vector<float> deplace(float = 1., float = 1., float = 1., float = 1., float = 1.);
    vector<float> getVelocity( vector<float>, float, float);
};

#endif