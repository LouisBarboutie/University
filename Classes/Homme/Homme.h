#ifndef HOMME_H
#define HOMME_H

class Homme
{
    private: 
    int age;

    public: 
    Homme(int=20);
    void vieillir(void);
    int quelAge();
};

#endif