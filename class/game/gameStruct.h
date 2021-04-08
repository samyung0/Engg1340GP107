#ifndef GAMESTRUCT_H
#define GAMESTRUCT_H
#include <string>
#include <utility>
#include <vector>
#include <functional>
#include <map>
#include <unordered_map>
#include <future>
#include <tuple>
#include <mutex>
#include <cassert>
#include <algorithm>

#include "../../data/troop/troop.h"

class Progress
{
public:
  int remain;
  int interval;
  Progress(int, int);
  void start(std::mutex &);
};

namespace data
{

  struct Resource
  {
    // to be updated in game fetch
    double food = 0;
    double equipment = 0;
    int manpower = 5;
    int manpowerInUse = 0;
    double baseLand = 100;
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

    double baseRecovery = 4;
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

    // time required to build each building
    std::vector<int> farmT = {10, 10, 10};
    std::vector<int> civilianFactoryT = {10, 15, 15};
    std::vector<int> militaryFactoryT = {15, 30, 60};
    std::vector<int> trainingCampT = {10};
    std::vector<int> airportT = {45};

    // land required to build each building
    std::vector<int> farmL = {3, 5, 6};
    std::vector<int> civilianFactoryL = {1, 1, 2};
    std::vector<int> militaryFactoryL = {2, 3, 5};
    std::vector<int> trainingCampL = {1};
    std::vector<int> airportL = {1};

    // upgradable
    std::vector<bool> farmU = {true, true, true};
    std::vector<bool> civilianFactoryU = {true, false, false};
    std::vector<bool> militaryFactoryU = {true, false, false};
    std::vector<bool> trainingCampU = {true};
    std::vector<bool> airportU = {true};

    // key: Building name, value: vector of functions which increment the Building count (capturing by ref)
    std::unordered_map<std::string, std::vector<std::function<void(Resource &)>>> effect = {
        {"farm",
         {[&](Resource &a) {
            farm[0]++;
            a.food += 8;
          },
          [&](Resource &a) {
            farm[1]++;
            a.food += 20;
          },
          [&](Resource &a) {
            farm[2]++;
            a.food += 28;
          }}},
        {"civilianFactory",
         {[&](Resource &a) {
            civilianFactory[0]++;
            a.manpower += 1;
          },
          [&](Resource &a) {
            civilianFactory[1]++;
            a.manpower += 3;
          },
          [&](Resource &a) {
            civilianFactory[2]++;
            a.manpower += 5;
          }}},
        {"militaryFactory",
         {[&](Resource &a) {
            militaryFactory[0]++;
            a.equipment += 10;
          },
          [&](Resource &a) {
            militaryFactory[1]++;
            a.equipment += 30;
          },
          [&](Resource &a) {
            militaryFactory[2]++;
            a.equipment += 100;
          }}},
        {"trainingCamp",
         {[&](Resource &a) {
           trainingCamp[0]++;
           a.camp++;
         }}},
        {"airport",
         {[&](Resource &a) {
           airport[0]++;
           a.airport++;
         }}},
    };
    std::unordered_map<std::string, std::vector<std::function<void(Resource &)>>> reverseEffect = {
        {"farm",
         {[&](Resource &a) {
            a.food -= 8;
          },
          [&](Resource &a) {
            a.food -= 20;
          },
          [&](Resource &a) {
            a.food -= 28;
          }}},
        {"civilianFactory",
         {[&](Resource &a) {
            a.manpower -= 1;
          },
          [&](Resource &a) {
            a.manpower -= 3;
          },
          [&](Resource &a) {
            a.manpower -= 5;
          }}},
        {"militaryFactory",
         {[&](Resource &a) {
            a.equipment -= 10;
          },
          [&](Resource &a) {
            a.equipment -= 30;
          },
          [&](Resource &a) {
            a.equipment -= 100;
          }}},
        {"trainingCamp",
         {[&](Resource &a) {
           a.camp--;
         }}},
        {"airport",
         {[&](Resource &a) {
           a.airport--;
         }}},
    };

    // type, id, desc
    std::vector<std::tuple<std::string, std::string, std::string>> progressTrack = {};
    std::unordered_map<std::string, Progress *> progress;
    std::unordered_map<std::string, std::future<void>> progressAsync;
  };
  struct Troop
  {
    std::vector<TroopUnit *> allTroop;

    // indices: free, in Army, in Battle plan, in Battle

    // update: I dont know why i didnt use a map but too late to change,
    // so I made a helper function that returns the value when queried
    std::vector<int> infantry = {0, 0, 0, 0};
    std::vector<int> calvary = {0, 0, 0, 0};
    std::vector<int> suicideBomber = {0, 0, 0, 0};
    std::vector<int> artillery = {0, 0, 0, 0};
    std::vector<int> logistic = {0, 0, 0, 0};
    std::vector<int> armoredCar = {0, 0, 0, 0};
    std::vector<int> tank1 = {0, 0, 0, 0};
    std::vector<int> tank2 = {0, 0, 0, 0};
    std::vector<int> tankOshimai = {0, 0, 0, 0};
    std::vector<int> cas = {0, 0, 0, 0};
    std::vector<int> fighter = {0, 0, 0, 0};
    std::vector<int> bomber = {0, 0, 0, 0};
    std::vector<int> kamikaze = {0, 0, 0, 0};

    std::unordered_map<std::string, std::function<int(int)>> helper = {
        {"infantry", [&](int index) -> int { return infantry[index]; }},
        {"calvary", [&](int index) -> int { return calvary[index]; }},
        {"suicideBomber", [&](int index) -> int { return suicideBomber[index]; }},
        {"artillery", [&](int index) -> int { return artillery[index]; }},
        {"logistic", [&](int index) -> int { return logistic[index]; }},
        {"armoredCar", [&](int index) -> int { return armoredCar[index]; }},
        {"tank1", [&](int index) -> int { return tank1[index]; }},
        {"tank2", [&](int index) -> int { return tank2[index]; }},
        {"tankOshimai", [&](int index) -> int { return tankOshimai[index]; }},
        {"cas", [&](int index) -> int { return cas[index]; }},
        {"fighter", [&](int index) -> int { return fighter[index]; }},
        {"bomber", [&](int index) -> int { return bomber[index]; }},
        {"kamikaze", [&](int index) -> int { return kamikaze[index]; }}};

    std::unordered_map<std::string, std::function<void(int, int)>> helper2 = {
        {"infantry", [&](int index, int value) { infantry[index] += value; }},
        {"calvary", [&](int index, int value) { calvary[index] += value; }},
        {"suicideBomber", [&](int index, int value) { suicideBomber[index] += value; }},
        {"artillery", [&](int index, int value) { artillery[index] += value; }},
        {"logistic", [&](int index, int value) { logistic[index] += value; }},
        {"armoredCar", [&](int index, int value) { armoredCar[index] += value; }},
        {"tank1", [&](int index, int value) { tank1[index] += value; }},
        {"tank2", [&](int index, int value) { tank2[index] += value; }},
        {"tankOshimai", [&](int index, int value) { tankOshimai[index] += value; }},
        {"cas", [&](int index, int value) { cas[index] += value; }},
        {"fighter", [&](int index, int value) { fighter[index] += value; }},
        {"bomber", [&](int index, int value) { bomber[index] += value; }},
        {"kamikaze", [&](int index, int value) { kamikaze[index] += value; }}};

    // type, id
    std::vector<std::tuple<std::string, std::string>> progressTrack = {};
    std::unordered_map<std::string, Progress *> progress;
    std::unordered_map<std::string, std::future<void>> progressAsync;

    int totalTroops = 0;
    int totalFoodRequired = 0;
    int totalEquipmentRequired = 0;
  };
  
}

class ArmyUnit
{
public:
  ArmyUnit(std::string);
  ArmyUnit(std::string, std::vector<std::tuple<int, int, TroopUnit *>>);

  // name should be unique among other armies (used as key in army struct map)
  std::string name;
  int troopCount = 0;

  bool inBattle = false;
  // country name, region coordinate
  std::pair<std::string, std::string> battleRegion = {};


  double calsualtyCount = 0;
  // calculated by dividing lost troops/ total troops x 100
  double casualtyPercentage = 0;

  // pointer to troops
  std::vector<std::vector<TroopUnit *>> formation = {
      {NULL, NULL, NULL, NULL},
      {NULL, NULL, NULL, NULL},
      {NULL, NULL, NULL, NULL},
      {NULL, NULL, NULL, NULL}};

  int totalBaseFoodRequired = 0;
  int totalBaseEquipmentRequired = 0;

  int foodReductionPer = 0;
  int equipmentReductionPer = 0;

  int speedBoostPerLand = 0;

  int logisticCount = 0;

  // total required = totalbase - totalreduced
  // total supplied = totalbase * total food production / total food required (capped at totalbase)
  double totalFoodRequired = 0;
  double totalEquipmentRequired = 0;

  // supplied/ required
  // double foodRatio = 1;
  // double equipmentRatio = 1;

  // stored as decimal, displayed as percentage
  // all troops within army should have same subsequential strength
  // double subsequentialStrength = 1.0;

  // calc and deal damage to all troops within the army during battle
  void calcDamage(Damage &);

  // remove troops that have 0 health or below
  void cleanUp();

  void addTroop(int y, int x, std::string, data::Troop *troop, data::Resource *resource);
  void removeTroop(int y, int x, data::Troop *troop, data::Resource *resource);
};

// used to pass into battleUnit as a wrapper of how many troops are present (applicable to both enemy side and your side)
class BattleTroopWrapper
{
public:
  // army, singular troop
  BattleTroopWrapper(std::vector<ArmyUnit *> army, std::vector<TroopUnit *> troop) : totalArmy(army), totalTroop(troop){};

  std::vector<ArmyUnit *> totalArmy;
  std::vector<TroopUnit *> totalTroop;
};

class BattleUnit
{
public:
  // country, region, your side troop, enemy side troop
  BattleUnit(std::string country_, std::string region_, BattleTroopWrapper *mikata_, BattleTroopWrapper *foe_) : country(country_), region(region_), mikata(mikata_), foe(foe_){};

  std::string country;
  std::string region;

  BattleTroopWrapper *mikata;
  BattleTroopWrapper *foe;

  // stats
  int duration = 0;
  double damageDealt = 0;
  std::unordered_map<std::string, int> killCount = {
      {"infantry", 0},
      {"calvary", 0},
      {"artillery", 0},
      {"logistic", 0},
      {"armoredCar", 0},
      {"tank1", 0},
      {"tank2", 0},
      {"tankOshimai", 0},
      {"cas", 0},
      {"fighter", 0},
      {"Bomber", 0},
  };
};

namespace data{
  struct Battle
  {
    std::vector<BattleUnit *> total;
    bool inBattle = false;
    std::string countryBattling = "";
  };
}

class Block
{
public:
  Block(data::Troop *troop_, data::Resource *resource_, data::Battle *battler_, std::string country_, std::string name_, std::vector<std::string> attackable_, std::vector<std::string> encircled_, std::unordered_map<std::string, int> acquirable_, int total_, std::string terrain_) : name(name_), attackable(attackable_), encircled(encircled_), acquirable(acquirable_), troop(troop_), resource(resource_), country(country_), total(total_), terrain(terrain_), battler(battler_) {}
  data::Troop *troop;
  data::Resource *resource;
  data::Battle *battler;

  std::string country;
  std::string name;

  std::string terrain;

  bool captured = false;
  bool battling = false;

  std::vector<std::string> attackable;
  std::vector<std::string> encircled;

  std::unordered_map<std::string, int> acquirable;

  std::vector<BattleUnit *> battle;

  std::vector<ArmyUnit *> totalFoeArmy;
  std::vector<TroopUnit *> totalFoeTroop;

  int total = 0;

  void reinforce(TroopUnit *reinforcement)
  {
    assert(reinforcement != NULL);

    if (!this->battling)
    {
      this->battle.push_back(new BattleUnit(this->country, this->name, new BattleTroopWrapper({}, {}), new BattleTroopWrapper(this->totalFoeArmy, this->totalFoeTroop)));
      this->battling = true;
      battler->inBattle = true;
      battler->countryBattling = country;
    }

    troop->helper2[reinforcement->type](0, -1);
    troop->helper2[reinforcement->type](3, 1);

    reinforcement->state["battle"] = true;
    reinforcement->reference.push_back(true);
    reinforcement->isReferenced = true;

    battle.back()->mikata->totalTroop.push_back(reinforcement);
  }
  void reinforce(ArmyUnit *reinforcement)
  {
    assert(reinforcement != NULL);

    if (!this->battling)
    {
      this->battle.push_back(new BattleUnit(this->country, this->name, new BattleTroopWrapper({}, {}), new BattleTroopWrapper(this->totalFoeArmy, this->totalFoeTroop)));
      this->battling = true;
      battler->inBattle = true;
      battler->countryBattling = country;
    }

    reinforcement->inBattle = true;
    reinforcement->battleRegion = std::make_pair(this->country, this->name);
    for (auto i : reinforcement->formation)
      for (auto j : i)
      {
        troop->helper2[j->type](3, 1);
        j->state["battle"] = true;
      }

    battle.back()->mikata->totalArmy.push_back(reinforcement);
  }
  void retreat(TroopUnit *retreating)
  {
    auto ptr = std::find(battle.back()->mikata->totalTroop.begin(), battle.back()->mikata->totalTroop.end(), retreating);
    assert(ptr != battle.back()->mikata->totalTroop.end());

    troop->helper2[retreating->type](3, -1);
    retreating->state["battle"] = false;
    assert(!retreating->state["army"]);
    assert(!retreating->state["battlePlan"]);
    retreating->state["free"] = true;
    troop->helper2[retreating->type](0, 1);

    battle.back()->mikata->totalTroop.erase(ptr);
    retreating->reference.pop_back();
    retreating->isReferenced = retreating->reference.size() != 0;

    if (battle.back()->mikata->totalTroop.size() == 0 && battle.back()->mikata->totalArmy.size() == 0)
      this->endBattle();
  }
  void retreat(ArmyUnit *retreating)
  {
    auto ptr = std::find(battle.back()->mikata->totalArmy.begin(), battle.back()->mikata->totalArmy.end(), retreating);
    assert(ptr != battle.back()->mikata->totalArmy.end());

    retreating->inBattle = false;
    retreating->battleRegion = {};
    for (auto i : retreating->formation)
      for (auto j : i)
      {
        troop->helper2[j->type](3, -1);
        j->state["battle"] = false;
      }

    battle.back()->mikata->totalArmy.erase(ptr);

    if (battle.back()->mikata->totalTroop.size() == 0 && battle.back()->mikata->totalArmy.size() == 0)
      this->endBattle();
  }
  void retreatAll()
  {
    for (auto i : battle.back()->mikata->totalTroop)
      this->retreat(i);
    for (auto i : battle.back()->mikata->totalArmy)
      this->retreat(i);
  }

  void capturedF()
  {
  }

private:
  void endBattle()
  {
    delete battle.back()->mikata;
    delete battle.back()->foe;
    this->battling = false;

    battler->inBattle = false;
    battler->countryBattling = "";
  }
};

class Enemy
{
public:
  Enemy(std::string name_, std::vector<std::vector<Block *>> map_) : name(name_), map(map_) {}
  std::string name;

  int totalLand = 0;
  int capturedLand = 0;
  bool capitulated = false;

  std::vector<std::vector<Block *>> map;
};

namespace data
{

  struct Army
  {
    // using map instead of unordered_map so armies' name will be printed according to lexicographical order
    // max 10 armies
    std::map<std::string, ArmyUnit *> total;
  };

  struct Research
  {

    std::vector<bool> farm = {true, true, true};
    std::vector<bool> divisionOfLabor = {true, false, false};
    std::vector<bool> productionLine = {true, false, false};
    std::vector<bool> landDoctrine = {true, false, false};
    std::vector<bool> airDoctrine = {true, false, false};
    std::vector<bool> urbanization = {true, false, false};
    std::vector<bool> weapon = {true, false, false};
    std::vector<bool> training = {true, false, false};
    std::vector<bool> recovery = {true, false, false};

    std::vector<int> farmT = {40, 40};
    std::vector<int> divisionOfLaborT = {20, 30};
    std::vector<int> productionLineT = {15, 120};
    std::vector<int> landDoctrineT = {40, 40};
    std::vector<int> airDoctrineT = {70, 100};
    std::vector<int> urbanizationT = {50, 70};
    std::vector<int> weaponT = {20, 70};
    std::vector<int> trainingT = {30, 50};
    std::vector<int> recoveryT = {30, 50};

    // key: research name, value: vector of functions which process the changes brought by the research (input all other struts, capturing by ref)
    std::unordered_map<std::string, std::vector<std::function<void(Resource &, Building &, Troop &, Army &)>>> effect = {
        {"farm",
         {[&](Resource &a, Building &b, Troop &c, Army &d) {
            b.farmU[1] = true;
            farm[1] = true;
          },
          [&](Resource &a, Building &b, Troop &c, Army &d) {
            b.farmU[2] = true;
            farm[2] = true;
          }}},
        {"divisionOfLabor",
         {[&](Resource &a, Building &b, Troop &c, Army &d) {
            b.civilianFactoryU[1] = true;
            divisionOfLabor[1] = true;
          },
          [&](Resource &a, Building &b, Troop &c, Army &d) {
            b.civilianFactoryU[2] = true;
            divisionOfLabor[2] = true;
          }}},
        {"productionLine",
         {[&](Resource &a, Building &b, Troop &c, Army &d) {
            b.militaryFactoryU[1] = true;
            productionLine[1] = true;
          },
          [&](Resource &a, Building &b, Troop &c, Army &d) {
            b.militaryFactoryU[2] = true;
            productionLine[2] = true;
          }}},
        {"landDoctrine",
         {[&](Resource &a, Building &b, Troop &c, Army &d) {
            a.baseLandTroopMul = 1.2;
            landDoctrine[1] = true;
          },
          [&](Resource &a, Building &b, Troop &c, Army &d) {
            a.baseLandTroopMul = 1.3;
            landDoctrine[2] = true;
          }}},
        {"airDoctrine",
         {[&](Resource &a, Building &b, Troop &c, Army &d) {
            a.baseAirTroopMul = 1.4;
            airDoctrine[1] = true;
          },
          [&](Resource &a, Building &b, Troop &c, Army &d) {
            a.baseAirTroopMul = 1.6;
            airDoctrine[2] = true;
          }}},
        {"urbanization",
         {[&](Resource &a, Building &b, Troop &c, Army &d) {
            a.baseLandMul = 1.1;
            urbanization[1] = true;
          },
          [&](Resource &a, Building &b, Troop &c, Army &d) {
            a.baseAirTroopMul = 1.3;
            urbanization[2] = true;
          }}},
        {"weapon",
         {[&](Resource &a, Building &b, Troop &c, Army &d) {
            a.baseTankMul = 1.05;
            weapon[1] = true;
          },
          [&](Resource &a, Building &b, Troop &c, Army &d) {
            a.baseTankMul = 1.1;
            a.baseAirToopMul2 = 1.1;
            weapon[2] = true;
          }}},
        {"training",
         {[&](Resource &a, Building &b, Troop &c, Army &d) {
            a.baseTrainingTimeMul = 0.9;
            training[1] = true;
          },
          [&](Resource &a, Building &b, Troop &c, Army &d) {
            a.baseTrainingTimeMul = 0.8;
            training[2] = true;
          }}},
        {"recovery",
         {[&](Resource &a, Building &b, Troop &c, Army &d) {
            a.baseRecoveryDiff = 6;
          },
          [&](Resource &a, Building &b, Troop &c, Army &d) {
            a.baseRecoveryDiff = 8;
          }}},
    };

    // desc, id
    std::tuple<std::string, std::string> progressTrack = {};
    std::unordered_map<std::string, Progress *> progress;
    std::unordered_map<std::string, std::future<void>> progressAsync;
  };

  struct Enemies
  {
    std::vector<Enemy *> totalEnemies;
  };

}
#endif