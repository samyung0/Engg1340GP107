#include <iostream>
#include <cstdlib>
#include <ctime>



#include "../class/game/gameStruct.h"

data::Resource resource;

void equipdifficulty(data::Resource& resource, int diff){

    double equipx = 0;

    double equipy = 20;

    if (diff == 1)
    {
        equipx = equipy * 0.7;
    }


    if (diff == 2)
    {
        equipx = equipy * 0.65;
        equipy = equipy * 0.8;
    }


    if (diff == 3)
    {
        equipx = equipy * 0.55;
        equipy = equipy * 0.7;
    }


    if (diff == 4)
    {
        equipx = equipy * 0.4;
        equipy = equipy * 0.55;
    }


    if (diff == 5)
    {
        equipx = equipy * 0.3;
        equipy = equipy * 0.2;
    }


    int equip;

    int range = equipy - equipx +1;

    srand(time(NULL));

    equip = rand() % range + equipx;

    resource.equipment = equip;




    
    