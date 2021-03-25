#include <iostream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <functional>
#include <string>
#include <map>
#include <utility>

#include "game.h"

armyUnit::armyUnit(std::string name_) : name(name_) {}

namespace data
{

  struct resource
  {
    // to be updated in game init
    unsigned int food = 0;
    unsigned int equipment = 0;
    unsigned int manpower = 0;
    double baseLand = 0;
    double usedLand = 0;
    double capturedLand = 0;

    double baseLandMul = 1;
    double baseLandTroopMul = 1;
    double baseAirTroopMul = 1;
    double baseTankMul = 1;
    double baseAirToopMul2 = 1;
    double baseTrainingTimeMul = 1;

    double baseRecoveryDiff = 0;
  };

  struct building
  {
    // index 0 means no upgrade, 1 and 2 for upgraded
    std::vector<unsigned int> farm = {0, 0, 0};
    std::vector<unsigned int> civilianFactory = {0, 0, 0};
    std::vector<unsigned int> militaryFactory = {0, 0, 0};
    std::vector<unsigned int> trainingCamp = {0};
    std::vector<unsigned int> airport = {0};

    // captured buildings
    std::vector<unsigned int> farmC = {0, 0, 0};
    std::vector<unsigned int> civilianFactoryC = {0, 0, 0};
    std::vector<unsigned int> militaryFactoryC = {0, 0, 0};
    std::vector<unsigned int> trainingCampC = {0};
    std::vector<unsigned int> airportC = {0};

    // build time (to be updated during game init)
    std::vector<unsigned int> farmT = {0, 0, 0};
    std::vector<unsigned int> civilianFactoryT = {0, 0, 0};
    std::vector<unsigned int> militaryFactoryT = {0, 0, 0};
    std::vector<unsigned int> trainingCampT = {0};
    std::vector<unsigned int> airportT = {0};

    // upgradable
    std::vector<bool> farmU = {true, false, false};
    std::vector<bool> civilianFactoryU = {true, false, false};
    std::vector<bool> militaryFactoryU = {true, false, false};
    std::vector<bool> trainingCampU = {true};
    std::vector<bool> airportU = {true};

    // key: building name, value: vector of functions which increment the building count (capturing by ref)
    std::unordered_map<std::string, std::vector<std::function<void()>>> effect = {
        {"farm",
         {[&]() {
            farm[0]++;
          },
          [&]() {
            farm[1]++;
          },
          [&]() {
            farm[2]++;
          }}},
        {"civilianFactory",
         {[&]() {
            civilianFactory[0]++;
          },
          [&]() {
            civilianFactory[1]++;
          },
          [&]() {
            civilianFactory[2]++;
          }}},
        {"militaryFactory",
         {[&]() {
            militaryFactory[0]++;
          },
          [&]() {
            militaryFactory[1]++;
          },
          [&]() {
            militaryFactory[2]++;
          }}},
        {"trainingCamp",
         {[&]() {
           trainingCamp[0]++;
         }}},
        {"airport",
         {[&]() {
           airport[0]++;
         }}},
    };

    std::vector<Progress> progress;
  };
  struct troop
  {
    // indices: total, in army, in battle plan, in battle
    std::vector<int> infantry = {0, 0, 0, 0};
    std::vector<int> calvary = {0, 0, 0, 0};
    std::vector<int> suicideBomber = {0, 0, 0, 0};
    std::vector<int> artillery = {0, 0, 0, 0};
    std::vector<int> logistic = {0, 0, 0, 0};
    std::vector<int> armoredCar = {0, 0, 0, 0};
    std::vector<int> tank1 = {0, 0, 0, 0};
    std::vector<int> tank2 = {0, 0, 0, 0};
    std::vector<int> cas = {0, 0, 0, 0};
    std::vector<int> fighter = {0, 0, 0, 0};
    std::vector<int> bomber = {0, 0, 0, 0};
    std::vector<int> kamikaze = {0, 0, 0, 0};

    // troop training time (to be updated in game init)
    unsigned int infantryT = 0;
    unsigned int calvaryT = 0;
    unsigned int suicideBomberT = 0;
    unsigned int artilleryT = 0;
    unsigned int logisticT = 0;
    unsigned int armoredCarT = 0;
    unsigned int tank1T = 0;
    unsigned int tank2T = 0;
    unsigned int casT = 0;
    unsigned int fighterT = 0;
    unsigned int bomberT = 0;
    unsigned int kamikazeT = 0;

    // troop health (to be updated in game init)
    unsigned int infantryH = 0;
    unsigned int calvaryH = 0;
    unsigned int suicideBomberH = 0;
    unsigned int artilleryH = 0;
    unsigned int logisticH = 0;
    unsigned int armoredCarH = 0;
    unsigned int tank1H = 0;
    unsigned int tank2H = 0;
    unsigned int casH = 0;
    unsigned int fighterH = 0;
    unsigned int bomberH = 0;
    unsigned int kamikazeH = 0;

    // camps it take to train per troop (to be updated in game init)
    unsigned int infantryC = 0;
    unsigned int calvaryC = 0;
    unsigned int suicideBomberC = 0;
    unsigned int artilleryC = 0;
    unsigned int logisticC = 0;
    unsigned int armoredCarC = 0;
    unsigned int tank1C = 0;
    unsigned int tank2C = 0;
    unsigned int casC = 0;
    unsigned int fighterC = 0;
    unsigned int bomberC = 0;
    unsigned int kamikazeC = 0;

    // food required per troop (to be updated in game init)
    unsigned int infantryF = 0;
    unsigned int calvaryF = 0;
    unsigned int suicideBomberF = 0;
    unsigned int artilleryF = 0;
    unsigned int logisticF = 0;
    unsigned int armoredCarF = 0;
    unsigned int tank1F = 0;
    unsigned int tank2F = 0;
    unsigned int casF = 0;
    unsigned int fighterF = 0;
    unsigned int bomberF = 0;
    unsigned int kamikazeF = 0;

    // equipment required per troop (to be updated in game init)
    unsigned int infantryE = 0;
    unsigned int calvaryE = 0;
    unsigned int suicideBomberE = 0;
    unsigned int artilleryE = 0;
    unsigned int logisticE = 0;
    unsigned int armoredCarE = 0;
    unsigned int tank1E = 0;
    unsigned int tank2E = 0;
    unsigned int casE = 0;
    unsigned int fighterE = 0;
    unsigned int bomberE = 0;
    unsigned int kamikazeE = 0;

    std::vector<Progress> progress;
  };

  struct army
  {
    // using map instead of unordered_map so armies' name will be printed according to lexicographical order
    std::map<std::string, armyUnit> total;
  };

  struct battlePlan
  {
    std::vector<battlePlanUnit> total;
  };

  struct battle
  {
  };

  struct research
  {

    std::vector<bool> farm = {true, false, false};
    std::vector<bool> divisionOfLabor = {true, false, false};
    std::vector<bool> productionLine = {true, false, false};
    std::vector<bool> landDoctrine = {true, false, false};
    std::vector<bool> airDoctrine = {true, false, false};
    std::vector<bool> urbanization = {true, false, false};
    std::vector<bool> weapon = {true, false, false};
    std::vector<bool> training = {true, false, false};
    std::vector<bool> recovery = {true, false, false};

    // key: research name, value: vector of functions which process the changes brought by the research (input all other struts, capturing by ref)
    std::unordered_map<std::string, std::vector<std::function<void(resource, building, troop, army, battlePlan, battle)>>> effect = {
        {"farm",
         {[&](resource a, building b, troop c, army d, battlePlan e, battle f) {
            b.farmU[1] = true;
            farm[1] = true;
          },
          [&](resource a, building b, troop c, army d, battlePlan e, battle f) {
            b.farmU[2] = true;
            farm[2] = true;
          }}},
        {"divisionOfLabor",
         {[&](resource a, building b, troop c, army d, battlePlan e, battle f) {
            b.civilianFactoryU[1] = true;
            divisionOfLabor[1] = true;
          },
          [&](resource a, building b, troop c, army d, battlePlan e, battle f) {
            b.civilianFactoryU[2] = true;
            divisionOfLabor[2] = true;
          }}},
        {"productionLine",
         {[&](resource a, building b, troop c, army d, battlePlan e, battle f) {
            b.militaryFactoryU[1] = true;
            productionLine[1] = true;
          },
          [&](resource a, building b, troop c, army d, battlePlan e, battle f) {
            b.militaryFactoryU[2] = true;
            productionLine[2] = true;
          }}},
        {"landDoctrine",
         {[&](resource a, building b, troop c, army d, battlePlan e, battle f) {
            a.baseLandTroopMul = 1.2;
            landDoctrine[1] = true;
          },
          [&](resource a, building b, troop c, army d, battlePlan e, battle f) {
            a.baseLandTroopMul = 1.3;
            landDoctrine[2] = true;
          }}},
        {"airDoctrine",
         {[&](resource a, building b, troop c, army d, battlePlan e, battle f) {
            a.baseAirTroopMul = 1.4;
            airDoctrine[1] = true;
          },
          [&](resource a, building b, troop c, army d, battlePlan e, battle f) {
            a.baseAirTroopMul = 1.6;
            airDoctrine[2] = true;
          }}},
        {"urbanization",
         {[&](resource a, building b, troop c, army d, battlePlan e, battle f) {
            a.baseLandMul = 1.1;
            urbanization[1] = true;
          },
          [&](resource a, building b, troop c, army d, battlePlan e, battle f) {
            a.baseAirTroopMul = 1.3;
            urbanization[2] = true;
          }}},
        {"weapon",
         {[&](resource a, building b, troop c, army d, battlePlan e, battle f) {
            a.baseTankMul = 1.05;
            weapon[1] = true;
          },
          [&](resource a, building b, troop c, army d, battlePlan e, battle f) {
            a.baseTankMul = 1.1;
            a.baseAirToopMul2 = 1.1;
            weapon[2] = true;
          }}},
        {"training",
         {[&](resource a, building b, troop c, army d, battlePlan e, battle f) {
            a.baseTrainingTimeMul = 0.9;
            training[1] = true;
          },
          [&](resource a, building b, troop c, army d, battlePlan e, battle f) {
            a.baseTrainingTimeMul = 0.8;
            training[2] = true;
          }}},
        {"recovery",
         {[&](resource a, building b, troop c, army d, battlePlan e, battle f) {
            a.baseRecoveryDiff = 6;
          },
          [&](resource a, building b, troop c, army d, battlePlan e, battle f) {
            a.baseRecoveryDiff = 8;
          }}},
    };

    std::vector<Progress> progress;
  };

}

Game::~Game() {}
Game::Game(const MenuWrapper &gameStats) : copyStats(gameStats) {}
void Game::start() { std::cout << "Game has started." << std::endl; }

Progress::Progress(int remain_) : remain(remain_) {}
int Progress::get() { return remain; }
void Progress::minus(int amount) { remain -= amount; }