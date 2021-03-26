#ifndef GAMESTRUCT_H
#define GAMESTRUCT_H
#include <string>
#include <utility>
#include <vector>
#include <functional>
#include <map>
#include <unordered_map>

#include "game.h"
namespace data
{

  struct Resource
  {
    // to be updated in game fetchData
    int food = 0;
    int equipment = 0;
    int manpower = 0;
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

  struct Building
  {
    // index 0 means no upgrade, 1 and 2 for upgraded
    std::vector<int> farm = {0, 0, 0};
    std::vector<int> civilianFactory = {0, 0, 0};
    std::vector<int> militaryFactory = {0, 0, 0};
    std::vector<int> trainingCamp = {0};
    std::vector<int> airport = {0};

    // captured Buildings
    std::vector<int> farmC = {0, 0, 0};
    std::vector<int> civilianFactoryC = {0, 0, 0};
    std::vector<int> militaryFactoryC = {0, 0, 0};
    std::vector<int> trainingCampC = {0};
    std::vector<int> airportC = {0};

    // build time (to be updated during game fetchData)
    std::vector<int> farmT = {10, 0, 0};
    std::vector<int> civilianFactoryT = {0, 0, 0};
    std::vector<int> militaryFactoryT = {0, 0, 0};
    std::vector<int> trainingCampT = {0};
    std::vector<int> airportT = {0};

    // upgradable
    std::vector<bool> farmU = {true, false, false};
    std::vector<bool> civilianFactoryU = {true, false, false};
    std::vector<bool> militaryFactoryU = {true, false, false};
    std::vector<bool> trainingCampU = {true};
    std::vector<bool> airportU = {true};

    // key: Building name, value: vector of functions which increment the Building count (capturing by ref)
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

    std::vector<Progress*> progress;
    std::vector<std::future<void>> progressAsync;
  };
  struct Troop
  {
    // indices: total, in Army, in Battle plan, in Battle
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

    // order: camp required, time to train, food required, equipment required, soft attack, hard attack, air attack, defense, armor, speed, disruption, hp
    // to be changed in game fetchData
    std::vector<int> infantryStat = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::vector<int> calvaryStat = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::vector<int> suicideBomberStat = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::vector<int> artilleryStat = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::vector<int> logisticStat = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::vector<int> armoredCarStat = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::vector<int> tank1Stat = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::vector<int> tank2Stat = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::vector<int> casStat = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::vector<int> fighterStat = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::vector<int> bomberStat = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::vector<int> kamikazeStat = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    std::vector<Progress> progress;
  };

  struct Army
  {
    // using map instead of unordered_map so armies' name will be printed according to lexicographical order
    std::map<std::string, ArmyUnit> total;
  };

  struct BattlePlan
  {
    std::vector<BattlePlanUnit> total;
  };

  struct Battle
  {
    std::vector<BattleUnit> total;
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
    std::unordered_map<std::string, std::vector<std::function<void(Resource, Building, Troop, Army, BattlePlan, Battle)>>> effect = {
        {"farm",
         {[&](Resource a, Building b, Troop c, Army d, BattlePlan e, Battle f) {
            b.farmU[1] = true;
            farm[1] = true;
          },
          [&](Resource a, Building b, Troop c, Army d, BattlePlan e, Battle f) {
            b.farmU[2] = true;
            farm[2] = true;
          }}},
        {"divisionOfLabor",
         {[&](Resource a, Building b, Troop c, Army d, BattlePlan e, Battle f) {
            b.civilianFactoryU[1] = true;
            divisionOfLabor[1] = true;
          },
          [&](Resource a, Building b, Troop c, Army d, BattlePlan e, Battle f) {
            b.civilianFactoryU[2] = true;
            divisionOfLabor[2] = true;
          }}},
        {"productionLine",
         {[&](Resource a, Building b, Troop c, Army d, BattlePlan e, Battle f) {
            b.militaryFactoryU[1] = true;
            productionLine[1] = true;
          },
          [&](Resource a, Building b, Troop c, Army d, BattlePlan e, Battle f) {
            b.militaryFactoryU[2] = true;
            productionLine[2] = true;
          }}},
        {"landDoctrine",
         {[&](Resource a, Building b, Troop c, Army d, BattlePlan e, Battle f) {
            a.baseLandTroopMul = 1.2;
            landDoctrine[1] = true;
          },
          [&](Resource a, Building b, Troop c, Army d, BattlePlan e, Battle f) {
            a.baseLandTroopMul = 1.3;
            landDoctrine[2] = true;
          }}},
        {"airDoctrine",
         {[&](Resource a, Building b, Troop c, Army d, BattlePlan e, Battle f) {
            a.baseAirTroopMul = 1.4;
            airDoctrine[1] = true;
          },
          [&](Resource a, Building b, Troop c, Army d, BattlePlan e, Battle f) {
            a.baseAirTroopMul = 1.6;
            airDoctrine[2] = true;
          }}},
        {"urbanization",
         {[&](Resource a, Building b, Troop c, Army d, BattlePlan e, Battle f) {
            a.baseLandMul = 1.1;
            urbanization[1] = true;
          },
          [&](Resource a, Building b, Troop c, Army d, BattlePlan e, Battle f) {
            a.baseAirTroopMul = 1.3;
            urbanization[2] = true;
          }}},
        {"weapon",
         {[&](Resource a, Building b, Troop c, Army d, BattlePlan e, Battle f) {
            a.baseTankMul = 1.05;
            weapon[1] = true;
          },
          [&](Resource a, Building b, Troop c, Army d, BattlePlan e, Battle f) {
            a.baseTankMul = 1.1;
            a.baseAirToopMul2 = 1.1;
            weapon[2] = true;
          }}},
        {"training",
         {[&](Resource a, Building b, Troop c, Army d, BattlePlan e, Battle f) {
            a.baseTrainingTimeMul = 0.9;
            training[1] = true;
          },
          [&](Resource a, Building b, Troop c, Army d, BattlePlan e, Battle f) {
            a.baseTrainingTimeMul = 0.8;
            training[2] = true;
          }}},
        {"recovery",
         {[&](Resource a, Building b, Troop c, Army d, BattlePlan e, Battle f) {
            a.baseRecoveryDiff = 6;
          },
          [&](Resource a, Building b, Troop c, Army d, BattlePlan e, Battle f) {
            a.baseRecoveryDiff = 8;
          }}},
    };

    std::vector<Progress> progress;
  };

}
#endif