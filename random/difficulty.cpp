#include <iostream>
#include <cstdlib>
#include <ctime>

#include "difficulty.h"
#include "../class/game/gameStruct.h"

data::Resource resource;

void foodDifficulty(data::Resource& resource, int diff){
    double foodx = 0;
    double foody = 20;
    if (diff == 1){
        foodx = foody * 0.7;
    }
    if (diff == 2){
        foodx = foody * 0.65;
        foody = foody * 0.8;
    }
    if (diff == 3){
        foodx = foody * 0.55;
        foody = foody * 0.7;
    }
    if (diff == 4){
        foodx = foody * 0.4;
        foody = foody * 0.55;
    }
    if (diff == 5){
        foodx = foody * 0.3;
        foody = foody * 0.2;
    }
    int food;
    int range = foody - foodx +1;
    srand(time(NULL));
    food = rand() % range + foodx;
    resource.food = food;

}

//have to adjust the difficulty

int main(){
    foodDifficulty(resource,3);
    manpowerdifficulty(resource,3);
    campdifficulty(resource,3);
    equipdifficulty(resource,3);
    manpowerdifficulty(resource,3);
    baselandDifficulty(resource,3);
}
