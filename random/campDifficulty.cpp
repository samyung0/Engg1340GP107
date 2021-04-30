#include <iostream>
#include <cstdlib>
#include <ctime>

#include "../class/game/gameStruct.h"

data::Resource resource;

void campdifficulty(data::Resource& resource, int diff){
    double campx = 0;
    double campy = 20;
    if (diff == 1){
        campx = campy * 0.7;
    }
    if (diff == 2){
        campx = campy * 0.65;
        campy = campy * 0.8;
    }
    if (diff == 3){
        campx = campy * 0.55;
        campy = campy * 0.7;
    }
    if (diff == 4){
        campx = campy * 0.4;
        campy = campy * 0.55;
    }
    if (diff == 5){
        campx = campy * 0.3;
        campy = campy * 0.2;
    }
    int camp;
    int range = campy - campx +1;
    srand(time(NULL));
    camp = rand() % range + campx;
    resource.camp = camp;;