#include "Homme.h"
#include <iostream>
#include <stdlib.h>
using namespace std;

Homme::Homme(int a)
{
    age = a;
}

void Homme::vieillir(void)
{
    age = age + 1;
}

int Homme::quelAge()
{
    return age;
}
