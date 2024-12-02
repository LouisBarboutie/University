#include "Homme.h"
#include <iostream>
#include <stdlib.h>
using namespace std;

int main(void)
{
    Homme Donald;
    Homme Robert(30);
    Donald.vieillir();
    cout << Robert.quelAge() << "\n";
    cout << Donald.quelAge();
}