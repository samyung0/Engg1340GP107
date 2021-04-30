#include <iostream>
#include <cstdlib>
#include <ctime>

#include "../class/game/gameStruct.h"

data::Resource resource;

void baselandDifficulty(data::Resource& resource, int diff){

    double baselx = 0;

    double basely = 20;

    if (diff == 1)
    {
        baselx = basely * 0.7;
    }


    if (diff == 2)
    {
        baselx = basely * 0.65;
        basely = basely * 0.8;
    }

    
    if (diff == 3)
    {
        baselx = basely * 0.55;
        basely = basely * 0.7;
    }


    if (diff == 4)
    {
        baselx = basely * 0.4;
        basely = basely * 0.55;
    }


    if (diff == 5)
    {
        baselx = basely * 0.3;
        basely = basely * 0.2;
    }


    int basel;

    int range = basely - baselx +1;

    
    srand(time(NULL));
    basel = rand() % range + baselx;
    resource.baseLand = basel;
}