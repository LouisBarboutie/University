#ifndef PARTICULE_H
#define PARTICULE_H
#include <iostream>
#include <stdlib.h>
#include <vector>
using namespace std;

class Particule
{
    protected: 
    float x,y;
    float masse;
    

    public: 
    Particule(float = 0, float = 0, float = 9.1e-31);
    //float getx(void);
    //float gety(void);
};

#endif