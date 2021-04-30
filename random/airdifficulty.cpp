#include <iostream>
#include <cstdlib>
#include <ctime>

#include "../class/game/gameStruct.h"

data::Resource resource;

void airdifficulty(data::Resource &resource, int diff)
{
    double airpx = 0;

    double airpy = 20;

    if (diff == 1)
    {
        airpx = airpy * 0.7;
    }


    if (diff == 2)
    {
        airpx = airpy * 0.65;
        airpy = airpy * 0.8;
    }


    if (diff == 3)
    {
        airpx = airpy * 0.55;
        airpy = airpy * 0.7;
    }


    if (diff == 4)
    {
        airpx = airpy * 0.4;
        airpy = airpy * 0.55;
    }


    if (diff == 5)
    {
        airpx = airpy * 0.3;
        airpy = airpy * 0.2;
    }


    int airp;
    int range = airpy - airpx + 1;
    srand(time(NULL));
    airp = rand() % range + airpx;
    resource.airport = airp;
}





