#ifndef PARTICULE_H
#define PARTICULE_H

class Particule
{
    private: 
    float x,y;
    float masse;

    public: 
    Particule(float = 0, float = 0, float = 9.1e-31);
    void deplace(float = 1., float = 1.);
    float getx(void);
    float gety(void);
};

#endif