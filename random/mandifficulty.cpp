#include <iostream>
#include <cstdlib>
#include <ctime>

#include "../class/game/gameStruct.h"

data::Resource resource;

void manpowerdifficulty(data::Resource& resource, int diff){
    double manpx = 0;
    double manpy = 20;
    if (diff == 1){
        manpx = manpy * 0.7;
    }
    if (diff == 2){
        manpx = manpy * 0.65;
        manpy = manpy * 0.8;
    }
    if (diff == 3){
        manpx = manpy * 0.55;
        manpy = manpy * 0.7;
    }
    if (diff == 4){
        manpx = manpy * 0.4;
        manpy = manpy * 0.55;
    }
    if (diff == 5){
        manpx = manpy * 0.3;
        manpy = manpy * 0.2;
    }
    int manp;
    int range = manpy - manpx +1;
    srand(time(NULL));
    manp = rand() % range + manpx;
    resource.manpower = manp;
