#include <iostream>

#include "../class/game/gameStruct.h"

namespace data{
    struct Resource
    {
        double food = 0;
        double equipment = 0;
        int manpower = 0;
        int manpowerInUse;
        double baseLand = 0;
        double usedLand = 0;
        double capturedLand = 0;
        int camp = 100;
        int campUsed = 0;
        int airport = 0;
        int airportUsed = 0;

        double baseLandMul = 1;
        double baseLandTroopMul = 1;
        double baseAirTroopMul = 1;
        double baseTankMul = 1;
        double baseAirToopMul2 = 1;
        double baseTrainingTimeMul = 1;

        double baseRecoveryDiff = 0;
    };
    
}

int main(int x, int y, data::Resource&){

}

