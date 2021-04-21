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
#include <cmath>
#include <random>

#include "../../data/troop/troop.h"
#include "../../random/random.h"
#include "../../io/io.h"

const bool dev = false;

class Progress
{
public:
  int remain;
  int &interval;
  int fps;
  bool &paused;
  Progress(int, int &, int, bool &);
  Progress(double, int &, int, bool &);
  void start(std::mutex &);
  int milliRemain = 0;
};

namespace data
{
  struct Resource
  {
    // to be updated in game fetch
    double food = 0;
    double equipment = 0;
    int manpower = 0;
    int manpowerInUse = 0;
    double baseLand = 0;
    double usedLand = 0;
    double capturedLand = 0;
    int camp = 0;
    int campUsed = 0;
    int airport = 0;
    int airportUsed = 0;

    double baseLandMul = 1;
    double baseLandTroopMul = 1;
    double baseAirTroopMul = 1;
    double baseTankMul = 1;
    double baseAirToopMul2 = 1;
    double baseTrainingTimeMul = 1;

    double baseRecovery = 1;
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

    // again not sure why I didnt use a map to store the building data
    // but its too late to change so I made a herlp function to return the buildings data
    std::unordered_map<std::string, std::function<int(int)>> helper = {
        {"farm", [&](int index) -> int { return farmL[index]; }},
        {"militaryFactory", [&](int index) -> int { return militaryFactoryL[index]; }},
        {"civilianFactory", [&](int index) -> int { return civilianFactoryL[index]; }},
        {"trainingCamp", [&](int index) -> int { return trainingCampL[index]; }},
        {"airport", [&](int index) -> int { return airportL[index]; }}};

    // type, id, desc
    std::vector<std::tuple<std::string, std::string, std::string>> progressTrack = {};
    std::unordered_map<std::string, Progress *> progress;
    std::unordered_map<std::string, std::future<void>> progressAsync;
  };
  struct Troop
  {
    std::vector<TroopUnit *> allTroop;

    // indices: free, in Army, null, in Battle (battle plan scrapped)

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

  // name should be unique among other armies (used as key in army struct map)
  std::string name;
  int troopCount = 0;
  int historicTroopCount = 0;

  bool inBattle = false;
  // country name, region coordinate
  std::pair<std::string, std::string> battleRegion = {};

  // double calsualtyCount = 0;
  // calculated by dividing lost troops/ total troops
  // double casualtyPercentage = 0;

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

  double historicCasualtyCount = 0;
  double casualtyPercentage = 0;

  // supplied/ required
  // double foodRatio = 1;
  // double equipmentRatio = 1;

  void addTroop(int y, int x, std::string, data::Troop *troop, data::Resource *resource);
  void removeTroop(int y, int x, data::Troop *troop, data::Resource *resource);

  void addTroopM(int y, int x, TroopUnit *instance);
  void removeTroopM(int y, int x);
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
  BattleUnit(std::function<void(ArmyUnit *)> retreatArmy_, std::vector<TroopUnit *> &totalFoeTroop_, std::vector<ArmyUnit *> &totalFoeArmy_, std::unordered_map<std::string, int> &foeCount_, std::mutex &lg_, std::string country_, std::string region_, BattleTroopWrapper *mikata_, BattleTroopWrapper *foe_, double terrainDebuff_, bool &isEncircled_) : terrainDebuff(terrainDebuff_), country(country_), region(region_), mikata(mikata_), foe(foe_), lg(lg_), isEncircled(isEncircled_), foeCount(foeCount_), totalFoeArmy(totalFoeArmy_), totalFoeTroop(totalFoeTroop_), retreatArmy(retreatArmy_){};

  std::string country;
  std::string region;
  double terrainDebuff;
  bool isEncircled;

  std::vector<std::string> log;

  BattleTroopWrapper *mikata;
  BattleTroopWrapper *foe;
  std::vector<ArmyUnit *> &totalFoeArmy;
  std::vector<TroopUnit *> &totalFoeTroop;

  std::unordered_map<std::string, int> armorTroop = {{"armoredCar", 1}, {"tank1", 1}, {"tank2", 1}, {"tankOshimai", 1}};
  std::unordered_map<std::string, int> airTroop = {{"cas", 1}, {"fighter", 1}, {"bomber", 1}, {"kamikaze", 1}};
  std::random_device rd;
  std::vector<std::function<std::vector<double>(std::random_device &)>> randArmy = {
      [](std::random_device &) -> std::vector<double> { return {1}; }, &randArmy2, &randArmy3, &randArmy4};

  std::mutex &lg;
  std::function<void(ArmyUnit *)> retreatArmy;

  // stats
  int duration = 0;
  std::unordered_map<std::string, int> &foeCount;
  std::unordered_map<std::string, int> friendCount = {
      {"infantry", 0},
      {"calvary", 0},
      {"suicideBomber", 0},
      {"artillery", 0},
      {"logistic", 0},
      {"armoredCar", 0},
      {"tank1", 0},
      {"tank2", 0},
      {"tankOshimai", 0},
      {"cas", 0},
      {"fighter", 0},
      {"Bomber", 0},
      {"Kamikaze", 0},
  };
  int totalFriendlyDeathCount = 0;
  int totalFoeDeathCount = 0;
  int totalFriendly = 0;
  double totalSoftAttack = 0;
  double totalHardAttack = 0;
  double totalAirAttack = 0;
  double totalFoeSoftAttack = 0;
  double totalFoeHardAttack = 0;
  double totalFoeAirAttack = 0;
  double totalFriendlyDisruption = 0;
  double totalFoeDisruption = 0;
  double foodRatio = 1;
  double equipmentRatio = 1;

  // may the better Hoi player win
  void fight(data::Troop *troop, data::Resource *resource)
  {
    this->lg.lock();

    // calculate the stats for both sides
    this->foodRatio = std::min(1.0, resource->food / troop->totalFoodRequired);
    this->equipmentRatio = std::min(1.0, resource->equipment / troop->totalEquipmentRequired);
    double avgFoodRatio = 0;
    double avgEquipmentRatio = 0;
    double avgAttackDebuff = 0;
    double avgPivotalStrength = 0;
    double avgSubsequentialStrength = 0;
    double totalFriendlyAirAttack = 0;
    double totalFoeAirAttack = 0;
    double airSupremacy = 1;
    double normalizedAirSurpremacy = 0;
    int nonSpecialTroop = 0;
    int specialTroop = 0;
    int unitFd = 0;
    int armorTroopFd = 0;
    int airTroopFd = 0;
    int unitFoe = 0;
    int armorTroopFoe = 0;
    int airTroopFoe = 0;
    int totalConspicuousnessFd = 0;
    int totalConspicuousnessFoe = 0;
    int totalTroopFd = 0;
    int totalTroopFoe = 0;
    for (auto i : this->mikata->totalTroop)
    {
      i->pivotalStrength = std::pow(1.2 - std::exp(-1.5 * (i->getHealth() / i->getBaseHealth()) + std::log(0.2) + 1.5), 1 - i->getHealth() / i->getBaseHealth());
      i->subsequentialStrength = (this->foodRatio + this->equipmentRatio) / 2;
      i->attackDebuff = this->terrainDebuff * (std::max(0., 1 - std::min(1.0, i->getSpeed() / 10.0)));
      totalFriendlyAirAttack += i->getAirAttack();
      this->totalFriendlyDisruption += i->getDisruption();
      if (i->type != "suicideBomber" && i->type != "kamikaze")
        nonSpecialTroop++;
      else
        specialTroop++;
      if (this->armorTroop.count(i->type) != 0)
        armorTroopFd++;
      if (this->airTroop.count(i->type) != 0)
      {
        totalConspicuousnessFd += i->getConspicuousness();
        airTroopFd++;
      }
      else
      {
        unitFd++;
      }
      totalTroopFd++;
      avgFoodRatio += this->foodRatio;
      avgEquipmentRatio += this->equipmentRatio;
      avgAttackDebuff += i->attackDebuff;
      avgPivotalStrength += i->pivotalStrength;
      avgSubsequentialStrength += i->subsequentialStrength;
    }
    for (auto i : this->foe->totalTroop)
    {
      i->pivotalStrength = std::pow(1.2 - std::exp(-1.5 * (i->getHealth() / i->getBaseHealth()) + std::log(0.2) + 1.5), 1 - i->getHealth() / i->getBaseHealth());
      i->subsequentialStrength = this->isEncircled ? 0.5 : 1;
      i->attackDebuff = 0;
      totalFoeAirAttack += i->getAirAttack();
      this->totalFoeDisruption += i->getDisruption();
      if (this->armorTroop.count(i->type) != 0)
        armorTroopFoe++;
      if (this->airTroop.count(i->type) != 0)
      {
        totalConspicuousnessFoe += i->getConspicuousness();
        airTroopFoe++;
      }
      else
      {
        unitFoe++;
      }
      totalTroopFoe++;
    }
    for (auto i : this->mikata->totalArmy)
    {
      double armyFoodRatio = i->totalFoodRequired == 0 ? 1 : std::min(1.0, (this->foodRatio * i->totalBaseFoodRequired) / i->totalFoodRequired);
      double armyEquipmentRatio = i->totalEquipmentRequired == 0 ? 1 : std::min(1.0, (this->equipmentRatio * i->totalBaseEquipmentRequired) / i->totalEquipmentRequired);
      bool hasLandTroop = false;
      for (auto j : i->formation)
        for (auto k : j)
          if (k != NULL)
          {
            k->pivotalStrength = std::pow(1.2 - std::exp(-1.5 * (k->getHealth() / k->getBaseHealth()) + std::log(0.2) + 1.5), 1 - k->getHealth() / k->getBaseHealth());
            k->subsequentialStrength = (armyFoodRatio + armyEquipmentRatio) / 2;
            k->attackDebuff = this->terrainDebuff * (std::max(0., 1 - std::min(1.0, (k->getSpeed() + i->speedBoostPerLand) / 10.0)));
            totalFriendlyAirAttack += k->getAirAttack();
            this->totalFriendlyDisruption += k->getDisruption();
            if (k->type != "suicideBomber" && k->type != "kamikaze")
              nonSpecialTroop++;
            else
              specialTroop++;

            if (this->armorTroop.count(k->type) != 0)
              armorTroopFd++;
            if (this->airTroop.count(k->type) != 0)
            {
              totalConspicuousnessFd += k->getConspicuousness();
              airTroopFd++;
            }
            else
              hasLandTroop = true;
            avgFoodRatio += armyFoodRatio;
            avgEquipmentRatio += armyEquipmentRatio;
            avgAttackDebuff += k->attackDebuff;
            avgPivotalStrength += k->pivotalStrength;
            avgSubsequentialStrength += k->subsequentialStrength;
            totalTroopFd++;
          }
      if (hasLandTroop)
        unitFd++;
    }
    for (auto i : this->foe->totalArmy)
    {
      bool hasLandTroop = false;
      for (auto j : i->formation)
        for (auto k : j)
          if (k != NULL)
          {
            k->pivotalStrength = std::pow(1.2 - std::exp(-1.5 * (k->getHealth() / k->getBaseHealth()) + std::log(0.2) + 1.5), 1 - k->getHealth() / k->getBaseHealth());
            k->subsequentialStrength = this->isEncircled ? 0.5 : 1;
            k->attackDebuff = 0;
            totalFoeAirAttack += k->getAirAttack();
            this->totalFoeDisruption += k->getDisruption();
            if (this->armorTroop.count(k->type) != 0)
              armorTroopFoe++;
            if (this->airTroop.count(k->type) != 0)
            {
              totalConspicuousnessFoe += k->getConspicuousness();
              airTroopFoe++;
            }
            else
              hasLandTroop;
            totalTroopFoe++;
          }
      if (hasLandTroop)
        unitFoe++;
    }
    totalConspicuousnessFd = std::max(1, totalConspicuousnessFd);
    totalConspicuousnessFoe = std::max(1, totalConspicuousnessFoe);
    this->totalFriendlyDisruption = 0.5 - std::exp(-0.1 * (this->totalFriendlyDisruption - 10 * std::log(0.5)));
    this->totalFoeDisruption = 0.5 - std::exp(-0.1 * (this->totalFoeDisruption - 10 * std::log(0.5)));
    airSupremacy = (totalFoeAirAttack == 0 && totalAirAttack == 0 ? 0 : totalAirAttack == 0  ? 0
                                                                    : totalFoeAirAttack == 0 ? 1
                                                                                             : 1 - std::exp((std::log(0.1) * totalFriendlyAirAttack) / (1.3 * totalFoeAirAttack)));
    double airSupremacyFoe = totalFoeAirAttack == 0 && totalAirAttack == 0 ? 0 : 1 - airSupremacy;
    normalizedAirSurpremacy = airSupremacy * 0.3 + 0.7;
    double normalizedAirSurpremacyFoe = airSupremacyFoe * 0.3 + 0.7;

    if (dev)
    {
      std::cout << (std::string("----- Enemy encircled: ") + (this->isEncircled ? "true" : "false") + " -----") << std::endl;
      std::cout << ("Total friendly troops: " + std::to_string(totalTroopFd)) << std::endl;
      std::cout << ("Total friendly armored troops: " + std::to_string(armorTroopFd)) << std::endl;
      std::cout << ("Total friendly planes: " + std::to_string(airTroopFd)) << std::endl;
      std::cout << ("Friendly Disruption (on enemy): " + std::to_string((int)std::round(this->totalFriendlyDisruption * 100)) + "%") << std::endl;
      std::cout << ("Total enemy troops: " + std::to_string(totalTroopFoe)) << std::endl;
      std::cout << ("Total enemy armored troops: " + std::to_string(armorTroopFoe)) << std::endl;
      std::cout << ("Total enemy planes: " + std::to_string(airTroopFoe)) << std::endl;
      std::cout << ("Enemy disruption (on friendly): " + std::to_string((int)std::round(this->totalFoeDisruption * 100)) + "%") << std::endl;
    }
    else
    {
      log.push_back(std::string("----- Enemy encircled: ") + (this->isEncircled ? "true" : "false") + " -----");
      log.push_back("Total friendly troops: " + std::to_string(totalTroopFd));
      log.push_back("Total friendly armored troops: " + std::to_string(armorTroopFd));
      log.push_back("Total friendly planes: " + std::to_string(airTroopFd));
      log.push_back("Friendly Disruption (on enemy): " + std::to_string((int)std::round(this->totalFriendlyDisruption * 100)) + "%");
      log.push_back("Total enemy troops: " + std::to_string(totalTroopFoe));
      log.push_back("Total enemy armored troops: " + std::to_string(armorTroopFoe));
      log.push_back("Total enemy planes: " + std::to_string(airTroopFoe));
      log.push_back("Enemy disruption (on friendly): " + std::to_string((int)std::round(this->totalFoeDisruption * 100)) + "%");
    }

    // damage structs
    Damage *foe = new Damage();
    Damage *nakama = new Damage();
    foe->airAttack = totalFoeAirAttack;
    nakama->airAttack = totalFriendlyAirAttack;
    for (auto i : this->mikata->totalTroop)
    {
      if (nonSpecialTroop < specialTroop * 2 && (i->type == "suicideBomber" || i->type == "kamikaze"))
        continue;
      i->giveDamage(this->totalFriendlyDisruption, normalizedAirSurpremacy, resource->baseLandTroopMul, resource->baseAirTroopMul, resource->baseTankMul, resource->baseAirToopMul2, *nakama);
    }
    for (auto i : this->foe->totalTroop)
    {
      i->giveDamage(this->totalFriendlyDisruption, normalizedAirSurpremacyFoe, 1, 1, 1, 1, *foe);
    }
    for (auto i : this->mikata->totalArmy)
    {
      for (auto j : i->formation)
        for (auto k : j)
          if (k != NULL)
          {
            if (nonSpecialTroop < specialTroop * 2 && (k->type == "suicideBomber" || k->type == "kamikaze"))
              continue;
            k->giveDamage(this->totalFriendlyDisruption, normalizedAirSurpremacy, resource->baseLandTroopMul, resource->baseAirTroopMul, resource->baseTankMul, resource->baseAirToopMul2, *nakama);
          }
    }
    for (auto i : this->foe->totalArmy)
    {
      for (auto j : i->formation)
        for (auto k : j)
          if (k != NULL)
          {
            k->giveDamage(this->totalFriendlyDisruption, normalizedAirSurpremacyFoe, 1, 1, 1, 1, *foe);
          }
    }
    if (armorTroopFoe == 0)
    {
      nakama->softAttack += nakama->hardAttack;
      nakama->hardAttack = 0;
    }
    if (armorTroopFd == 0)
    {
      foe->softAttack += foe->hardAttack;
      foe->hardAttack = 0;
    }
    int nonAirFd = totalTroopFd - airTroopFd;
    int nonAirFoe = totalTroopFoe - airTroopFoe;
    double dropoffFactorFd = (nonAirFd == 0 ? 1. : std::max(0., std::min(1., std::pow(1.5, airTroopFd / nonAirFd - 0.33) - 1.0)));
    double dropoffFactorFoe = (nonAirFoe == 0 ? 1. : std::max(0., std::min(1., std::pow(1.5, airTroopFoe / nonAirFoe - 0.33) - 1.0)));
    nakama->softAttack *= (1. - dropoffFactorFd);
    nakama->hardAttack *= (1. - dropoffFactorFd);
    nakama->airAttack *= (1. - dropoffFactorFd);
    foe->softAttack *= (1. - dropoffFactorFoe);
    foe->hardAttack *= (1. - dropoffFactorFoe);
    foe->airAttack *= (1. - dropoffFactorFoe);
    this->totalSoftAttack = nakama->softAttack;
    this->totalHardAttack = nakama->hardAttack;
    this->totalAirAttack = nakama->airAttack;
    this->totalFoeSoftAttack = foe->softAttack;
    this->totalFoeHardAttack = foe->hardAttack;
    this->totalFoeAirAttack = foe->airAttack;
    if (dev)
    {
      std::cout << ("Total droppoff factor: " + std::to_string((int)std::round(dropoffFactorFd * 100)) + "%") << std::endl;
      std::cout << ("Total conspicuousness: " + std::to_string((int)totalConspicuousnessFd)) << std::endl;
      std::cout << ("Food ratio (avg): " + std::to_string((int)std::round(avgFoodRatio / totalTroopFd * 100)) + "%") << std::endl;
      std::cout << ("Equipment ratio (avg): " + std::to_string((int)std::round(avgEquipmentRatio / totalTroopFd * 100)) + "%") << std::endl;
      std::cout << ("Raw air supremacy: " + std::to_string((int)std::round(airSupremacy * 100)) + "%") << std::endl;
      std::cout << ("Adjusted air supremacy: " + std::to_string((int)std::round(normalizedAirSurpremacy * 100)) + "%") << std::endl;
      std::cout << ("Attack debuff (avg): " + std::to_string((int)std::round(avgAttackDebuff / totalTroopFd * 100)) + "%") << std::endl;
      std::cout << ("****************") << std::endl;
      std::cout << ("Total friendly soft attack: " + std::to_string(nakama->softAttack)) << std::endl;
      std::cout << ("Total friendly hard attack: " + std::to_string(nakama->hardAttack)) << std::endl;
      std::cout << ("Total friendly air attack: " + std::to_string(nakama->airAttack)) << std::endl;
      std::cout << ("Total enemy soft attack: " + std::to_string(foe->softAttack)) << std::endl;
      std::cout << ("Total enemy hard attack: " + std::to_string(foe->hardAttack)) << std::endl;
      std::cout << ("Total enemy air attack: " + std::to_string(foe->airAttack)) << std::endl;
    }
    else
    {
      log.push_back("Total droppoff factor: " + std::to_string((int)std::round(dropoffFactorFd * 100)) + "%");
      log.push_back("Total conspicuousness: " + std::to_string((int)totalConspicuousnessFd));
      log.push_back("Food ratio (avg): " + std::to_string((int)std::round(avgFoodRatio / totalTroopFd * 100)) + "%");
      log.push_back("Equipment ratio (avg): " + std::to_string((int)std::round(avgEquipmentRatio / totalTroopFd * 100)) + "%");
      log.push_back("Raw air supremacy: " + std::to_string((int)std::round(airSupremacy * 100)) + "%");
      log.push_back("Adjusted air supremacy: " + std::to_string((int)std::round(normalizedAirSurpremacy * 100)) + "%");
      log.push_back("Attack debuff (avg): " + std::to_string((int)std::round(avgAttackDebuff / totalTroopFd * 100)) + "%");
      log.push_back("****************");
      log.push_back("Total friendly soft attack: " + std::to_string(nakama->softAttack));
      log.push_back("Total friendly hard attack: " + std::to_string(nakama->hardAttack));
      log.push_back("Total friendly air attack: " + std::to_string(nakama->airAttack));
      log.push_back("Total enemy soft attack: " + std::to_string(foe->softAttack));
      log.push_back("Total enemy hard attack: " + std::to_string(foe->hardAttack));
      log.push_back("Total enemy air attack: " + std::to_string(foe->airAttack));
    }

    double perUnit = foe->softAttack / unitFd;
    double perArmor = 0;
    if (armorTroopFd != 0)
      perArmor = foe->hardAttack / armorTroopFd;
    double perUnitFoe = nakama->softAttack / unitFoe;
    double perArmorFoe = 0;
    if (armorTroopFoe != 0)
      perArmorFoe = nakama->hardAttack / armorTroopFoe;

    double avgFd = 0;
    double avgFoe = 0;
    for (auto i : this->mikata->totalTroop)
    {
      double damage = 0;
      if (this->airTroop.count(i->type) != 0)
      {
        damage = foe->airAttack * i->getConspicuousness() / totalConspicuousnessFd;
      }
      else
      {
        if (this->armorTroop.count(i->type) != 0)
        {
          damage = std::max(0., (perUnit - i->getDefense()) / 4) + std::max(0., (perArmor - i->getArmor()) * 1.1);
        }
        else
        {
          damage = perUnit - i->getDefense();
        }
      }
      damage = std::max(damage, 0.);
      avgFd += damage;
      i->takeDamage(damage);
    }
    for (auto i : this->foe->totalTroop)
    {
      double damage = 0;
      if (this->airTroop.count(i->type) != 0)
      {
        damage = nakama->airAttack * i->getConspicuousness() / totalConspicuousnessFoe;
      }
      else
      {
        if (this->armorTroop.count(i->type) != 0)
        {
          damage = std::max(0., (perUnitFoe - i->getDefense()) / 4) + std::max(0., (perArmorFoe - i->getArmor()) * 1.1);
        }
        else
        {
          damage = perUnitFoe - i->getDefense();
        }
      }
      damage = std::max(damage, 0.);
      avgFoe += damage;
      i->takeDamage(damage);
    }
    for (auto i : this->mikata->totalArmy)
    {
      std::vector<int> landPresent = {0, 0, 0, 0};
      for (int j = 0; j < 4; j++)
        for (int k = 0; k < 4; k++)
        {
          if (i->formation[j][k] != NULL)
          {
            if (this->airTroop.count(i->formation[j][k]->type) == 0)
              landPresent[j]++;
          }
        }
      std::vector<double> columnDistribution = {0, 0, 0, 0};
      double acc = 0;
      if (landPresent[0] != 0)
        columnDistribution[0] = 0.6;
      else
        acc = 0.6;
      if (landPresent[1] != 0)
      {
        columnDistribution[1] = 0.3 + acc;
        acc = 0;
      }
      else
        acc += 0.3;
      if (landPresent[2] != 0)
      {
        columnDistribution[2] = 0.1 + acc;
        acc = 0;
      }
      else
        acc += 0.1;
      if (landPresent[3] != 0)
      {
        columnDistribution[3] = acc;
        acc = 0;
      }
      if (acc != 0)
      {
        if (landPresent[0] != 0)
          columnDistribution[0] += acc;
        else if (landPresent[1] != 0)
          columnDistribution[1] += acc;
      }
      // for (auto i : landPresent)
      //   std::cout << "land present: " << i << std::endl;
      for (int j = 0; j < 4; j++)
      {
        std::vector<double> distribution = {};
        if(landPresent[j] > 0)
        distribution = this->randArmy[landPresent[j] - 1](this->rd);
        int count = 0;
        for (int k = 0; k < 4; k++)
          if (i->formation[j][k] != NULL)
          {

            double damage = 0;
            if (this->airTroop.count(i->formation[j][k]->type) != 0)
              damage = foe->airAttack * i->formation[j][k]->getConspicuousness() / totalConspicuousnessFd;
            else
            {
              if (this->armorTroop.count(i->formation[j][k]->type) != 0)
              {
                // for (auto i : columnDistribution)
                //   std::cout << "column: " << i << std::endl;
                // for (auto i : distribution)
                //   std::cout << "distribution: " << i << std::endl;
                // std::cout << perUnit << std::endl;
                // std::cout << perArmor << std::endl;
                damage = std::max(0., (perUnit * columnDistribution[j] * distribution[count] - i->formation[j][k]->getDefense()) / 4) + std::max(0., (perArmor - i->formation[j][k]->getArmor()) * 1.1);
              }
              else
                damage = perUnit * columnDistribution[j] * distribution[count] - i->formation[j][k]->getDefense();
              count++;
            }
            damage = std::max(damage, 0.);
            avgFd += damage;
            i->formation[j][k]->takeDamage(damage);
          }
      }
    }
    for (auto i : this->foe->totalArmy)
    {
      std::vector<int> landPresent = {0, 0, 0, 0};
      for (int j = 0; j < 4; j++)
        for (int k = 0; k < 4; k++)
        {
          if (i->formation[j][k] != NULL)
          {
            if (this->airTroop.count(i->formation[j][k]->type) == 0)
              landPresent[j]++;
          }
        }
      std::vector<double> columnDistribution = {0, 0, 0, 0};
      double acc = 0;
      if (landPresent[0] != 0)
        columnDistribution[0] = 0.6;
      else
        acc = 0.6;
      if (landPresent[1] != 0)
      {
        columnDistribution[1] = 0.3 + acc;
        acc = 0;
      }
      else
        acc += 0.3;
      if (landPresent[2] != 0)
      {
        columnDistribution[2] = 0.1 + acc;
        acc = 0;
      }
      else
        acc += 0.1;
      if (landPresent[3] != 0)
      {
        columnDistribution[3] = acc;
        acc = 0;
      }
      if (acc != 0)
      {
        if (landPresent[0] != 0)
          columnDistribution[0] += acc;
        else if (landPresent[1] != 0)
          columnDistribution[1] += acc;
      }
      for (int j = 0; j < 4; j++)
      {
        std::vector<double> distribution = {};
        if (columnDistribution[j] != 0)
          distribution = this->randArmy[landPresent[j] - 1](this->rd);
        int count = 0;

        for (int k = 0; k < 4; k++)
          if (i->formation[j][k] != NULL)
          {
            double damage = 0;
            if (this->airTroop.count(i->formation[j][k]->type) != 0)
              damage = nakama->airAttack * i->formation[j][k]->getConspicuousness() / totalConspicuousnessFoe;
            else
            {
              if (this->armorTroop.count(i->formation[j][k]->type) != 0)
                damage = std::max(0., (perUnitFoe * columnDistribution[j] * distribution[count] - i->formation[j][k]->getDefense()) / 4) + std::max(0., (perArmorFoe - i->formation[j][k]->getArmor()) * 1.1);
              else
                damage = perUnitFoe * columnDistribution[j] * distribution[count] - i->formation[j][k]->getDefense();
              count++;
            }
            damage = std::max(damage, 0.);
            avgFoe += damage;
            i->formation[j][k]->takeDamage(damage);
          }
      }
    }
    if (dev)
    {
      std::cout << ("****************") << std::endl;
      std::cout << ("Friendly health decreased (avg): " + std::to_string((int)std::round(avgFd / totalTroopFd))) << std::endl;
      std::cout << ("Enemy health decreased (avg): " + std::to_string((int)std::round(avgFoe / totalTroopFoe))) << std::endl;
    }
    else
    {
      log.push_back("****************");
      log.push_back("Friendly health decreased (avg): " + std::to_string((int)std::round(avgFd / totalTroopFd)));
      log.push_back("Enemy health decreased (avg): " + std::to_string((int)std::round(avgFoe / totalTroopFoe)));
    }
    delete foe;
    foe = NULL;
    delete nakama;
    nakama = NULL;
    this->duration++;
    this->lg.unlock();
  }
  void regen(data::Resource *resource)
  {
    this->lg.lock();
    for (auto i : this->mikata->totalTroop)
      i->increaseHealth((resource->baseRecovery + resource->baseRecoveryDiff) * (1 - this->totalFoeDisruption / 2) * std::pow(this->foodRatio, 1.5));

    for (auto i : this->mikata->totalArmy)
      for (auto j : i->formation)
        for (auto k : j)
          if (k != NULL)
            k->increaseHealth((resource->baseRecovery + resource->baseRecoveryDiff) * (1 - this->totalFoeDisruption / 2) * std::pow(i->totalFoodRequired == 0 ? 1 : std::min(1.0, (this->foodRatio * i->totalBaseFoodRequired) / i->totalFoodRequired), 1.5));

    for (auto i : this->foe->totalTroop)
      i->increaseHealth(resource->baseRecovery * (1 - this->totalFriendlyDisruption / 2) * (this->isEncircled ? 0 : 1));

    for (auto i : this->foe->totalArmy)
      for (auto j : i->formation)
        for (auto k : j)
          if (k != NULL)
            k->increaseHealth(resource->baseRecovery * (1 - this->totalFriendlyDisruption / 2) * (this->isEncircled ? 0 : 1));
    this->lg.unlock();
  }
  void clean(data::Troop *troop, data::Resource *resource)
  {
    for (int i = this->mikata->totalTroop.size() - 1; i >= 0; i--)
    {
      if (this->mikata->totalTroop[i]->getHealth() <= 0)
      {
        this->mikata->totalTroop[i]->reference.pop_back();
        assert(this->mikata->totalTroop[i]->reference.size() == 0);

        int index = -1;
        for (int j = 0; j < troop->allTroop.size(); j++)
          if (troop->allTroop[j] == this->mikata->totalTroop[i])
          {
            index = j;
            break;
          }
        assert(index != -1);

        troop->totalFoodRequired -= this->mikata->totalTroop[i]->getFood();
        troop->totalEquipmentRequired -= this->mikata->totalTroop[i]->getEquipment();
        troop->totalTroops--;

        this->friendCount[this->mikata->totalTroop[i]->type]--;
        this->totalFriendlyDeathCount++;

        troop->helper2[this->mikata->totalTroop[i]->type](3, -1);
        delete this->mikata->totalTroop[i];
        this->mikata->totalTroop[i] = NULL;
        this->mikata->totalTroop.erase(this->mikata->totalTroop.begin() + i);
        troop->allTroop.erase(troop->allTroop.begin() + index);
      }
    }
    for (int i = this->foe->totalTroop.size() - 1; i >= 0; i--)
    {
      if (this->foe->totalTroop[i]->getHealth() <= 0)
      {
        assert(this->foe->totalTroop[i]->reference.size() == 0);

        this->totalFoeDeathCount++;
        this->foeCount[this->foe->totalTroop[i]->type]--;

        int index = -1;
        for (int j = 0; j < this->totalFoeTroop.size(); j++)
          if (this->totalFoeTroop[j] == this->foe->totalTroop[i])
          {
            index = j;
            break;
          }
        assert(index != -1);
        this->totalFoeTroop.erase(this->totalFoeTroop.begin() + index);

        delete this->foe->totalTroop[i];
        this->foe->totalTroop[i] = NULL;
        this->foe->totalTroop.erase(this->foe->totalTroop.begin() + i);
      }
    }
    for (auto i : this->mikata->totalArmy)
      for (int x = 0; x < 4; x++)
        for (int y = 0; y < 4; y++)
        {
          TroopUnit *k = i->formation[x][y];
          if (k != NULL && k->getHealth() <= 0)
          {
            k->reference.pop_back();
            assert(k->reference.size() == 0);

            int index = -1;
            for (int i = 0; i < troop->allTroop.size(); i++)
              if (troop->allTroop[i] == k)
              {
                index = i;
                break;
              }
            assert(index != -1);

            troop->totalFoodRequired -= k->getFood();
            troop->totalEquipmentRequired -= k->getEquipment();
            troop->totalTroops--;
            i->totalBaseFoodRequired -= k->getFood();
            i->totalBaseEquipmentRequired -= k->getEquipment();
            i->troopCount--;
            if (k->type == "logistic")
            {
              i->logisticCount--;
              i->foodReductionPer--;
              i->speedBoostPerLand -= 1;
            }
            i->totalFoodRequired = i->totalBaseFoodRequired - i->foodReductionPer * i->troopCount;
            i->totalEquipmentRequired = i->totalBaseEquipmentRequired - i->equipmentReductionPer * i->troopCount;
            i->historicCasualtyCount++;
            i->casualtyPercentage = i->historicCasualtyCount / i->historicTroopCount;

            this->friendCount[k->type]--;
            this->totalFriendlyDeathCount++;

            troop->helper2[k->type](3, -1);
            troop->helper2[k->type](1, -1);
            delete k;
            i->formation[x][y] = NULL;
            troop->allTroop.erase(troop->allTroop.begin() + index);
          }
        }
    for (auto i : this->foe->totalArmy)
      for (int x = 0; x < 4; x++)
        for (int y = 0; y < 4; y++)
        {
          TroopUnit *k = i->formation[x][y];
          if (k != NULL && k->getHealth() <= 0)
          {
            k->reference.pop_back();
            assert(k->reference.size() == 0);

            i->totalBaseFoodRequired -= k->getFood();
            i->totalBaseEquipmentRequired -= k->getEquipment();
            i->troopCount--;
            if (k->type == "logistic")
            {
              i->logisticCount--;
              i->foodReductionPer--;
              i->speedBoostPerLand -= 1;
            }
            i->totalFoodRequired = i->totalBaseFoodRequired - i->foodReductionPer * i->troopCount;
            i->totalEquipmentRequired = i->totalBaseEquipmentRequired - i->equipmentReductionPer * i->troopCount;
            this->totalFoeDeathCount++;
            this->foeCount[k->type]--;

            delete k;
            i->formation[x][y] = NULL;
          }
        }
    for (int j = this->foe->totalArmy.size() - 1; j >= 0; j--)
    {
      auto i = this->foe->totalArmy[j];
      if (i->troopCount == 0)
      {
        this->foeCount["army"]--;
        int index = -1;
        for (int j = 0; j < this->totalFoeArmy.size(); j++)
          if (this->totalFoeArmy[j] == i)
          {
            index = j;
            break;
          }
        assert(index != -1);
        this->totalFoeArmy.erase(this->totalFoeArmy.begin() + index);
        delete i;
        i = NULL;
        this->foe->totalArmy.erase(this->foe->totalArmy.begin() + j);
      }
    }
    for (int j = this->mikata->totalArmy.size() - 1; j >= 0; j--)
    {
      auto i = this->mikata->totalArmy[j];
      if (i->troopCount == 0)
      {
        i->historicCasualtyCount = 0;
        i->historicTroopCount = 0;
        i->casualtyPercentage = 0;
        this->retreatArmy(i);
      }
    }
  }
};

namespace data
{
  struct Battle
  {
    bool inBattle = false;
    std::string countryBattling = "";
  };
}

class Block
{
public:
  Block(data::Troop *troop_, data::Resource *resource_, data::Battle *battler_, int &capturedLand_, bool &capitulated_, int &battlingRegions_, int &totalLand_, int &defeated_, int coordX_, int coordY_) : troop(troop_), resource(resource_), battler(battler_), capturedLand(capturedLand_), capitulated(capitulated_), battlingRegions(battlingRegions_), totalLand(totalLand_), defeated(defeated_), coordX(coordX_), coordY(coordY_) {}
  data::Troop *troop;
  data::Resource *resource;
  data::Battle *battler;

  std::string country;
  std::string name;
  std::string terrain;

  int coordX;
  int coordY;

  std::unordered_map<std::string, double> terrainToDebuff = {
      {"plain", 0},
      {"hill", 0.02},
      {"river", 0.04},
      {"mountian", 0.05},
      {"snowland", 0.08},
      {"desert", 0.1}};

  int &capturedLand;
  bool &capitulated;
  int &battlingRegions;
  int &totalLand;
  int &defeated;
  std::vector<std::vector<Block *>> map;

  bool captured = false;
  bool battling = false;
  bool isEncircled = false;
  bool isAttackable = false;

  std::vector<std::pair<int, int>> attackable;
  std::vector<std::pair<int, int>> encircled;

  std::unordered_map<std::string, int> acquirable;

  std::vector<BattleUnit *> battle;

  std::vector<ArmyUnit *> totalFoeArmy;
  std::vector<TroopUnit *> totalFoeTroop;
  std::unordered_map<std::string, int> foeCount = {
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
      {"bomber", 0},
      {"army", 0}};

  std::mutex lg;

  int totalFoe = 0;
  void regen(data::Resource *resource)
  {
    for (auto i : this->totalFoeTroop)
      i->increaseHealth(resource->baseRecovery);

    for (auto i : this->totalFoeArmy)
      for (auto j : i->formation)
        for (auto k : j)
          if (k != NULL)
            k->increaseHealth(resource->baseRecovery);
  }
  void reinforce(std::mutex &lguser, std::function<void()> endGame, bool &gameOver, int enemySize, TroopUnit *reinforcement, data::Resource *resource, data::Building *building, data::Battle *battle, std::function<void(std::string type, int time, std::function<void(data::Resource &)> &callBack, std::string desc, double land, int amount)> buildBase)
  {
    assert(reinforcement != NULL);
    assert(reinforcement->reference.size() == 0);

    if (this->captured)
      return;
    this->lg.lock();
    if (this->totalFoe == 0)
    {
      this->capturedLand++;
      if (1.0 * this->capturedLand / this->totalLand >= 0.7)
      {
        this->capitulated = true;
        this->defeated++;
        battle->inBattle = false;
        battle->countryBattling = "";
        for (auto i : this->map)
          for (auto j : i)
            if (j != NULL && !j->captured)
              j->endBattle(resource, building, battle, buildBase);
        if (this->defeated == enemySize)
        {
          gameOver = true;
        }
      }
      else
      {
        bool stillInBattle = false;
        this->endBattle(resource, building, battle, buildBase);
        for (auto i : this->map)
          for (auto j : i)
            if (j != NULL)
            {
              if (j->battling)
                stillInBattle = true;
              j->isAttackable = j->isAttackable || std::find(j->attackable.begin(), j->attackable.end(), std::make_pair(this->coordX, this->coordY)) != j->attackable.end();
              if (!j->isEncircled)
              {
                bool allCaptured = true;
                for (auto k : j->encircled)
                {
                  if (!this->map[k.first][k.second]->captured)
                  {
                    allCaptured = false;
                    break;
                  }
                }
                if (allCaptured)
                  j->isEncircled = true;
              }
            }
        if (!stillInBattle)
        {
          battle->inBattle = false;
          battle->countryBattling = "";
        }
      }
      this->lg.unlock();
      return;
    }
    if (!this->battling)
    {
      this->battle.push_back(new BattleUnit([&](ArmyUnit *army) { this->retreat(army, battle, false); }, totalFoeTroop, totalFoeArmy, foeCount, this->lg, this->country, this->name, new BattleTroopWrapper({}, {}), new BattleTroopWrapper(this->totalFoeArmy, this->totalFoeTroop), this->terrainToDebuff[this->terrain], this->isEncircled));
      this->battling = true;
      battler->inBattle = true;
      battler->countryBattling = country;
      this->battlingRegions++;
    }

    troop->helper2[reinforcement->type](0, -1);
    troop->helper2[reinforcement->type](3, 1);

    reinforcement->state["battle"] = true;
    reinforcement->state["free"] = false;
    reinforcement->reference.push_back(true);
    reinforcement->isReferenced = true;

    this->battle.back()->mikata->totalTroop.push_back(reinforcement);
    this->battle.back()->totalFriendly++;
    this->battle.back()->friendCount[reinforcement->type]++;

    this->lg.unlock();
  }
  void reinforce(std::mutex &lguser, std::function<void()> endGame, bool &gameOver, int enemySize, ArmyUnit *reinforcement, data::Resource *resource, data::Building *building, data::Battle *battle, std::function<void(std::string type, int time, std::function<void(data::Resource &)> &callBack, std::string desc, double land, int amount)> buildBase)
  {
    assert(reinforcement != NULL);
    if (this->captured)
      return;
    this->lg.lock();
    if (this->totalFoe == 0)
    {
      this->capturedLand++;
      if (1.0 * this->capturedLand / this->totalLand >= 0.7)
      {
        this->capitulated = true;
        this->defeated++;
        battle->inBattle = false;
        battle->countryBattling = "";
        for (auto i : this->map)
          for (auto j : i)
            if (j != NULL && !j->captured)
              j->endBattle(resource, building, battle, buildBase);
        if (this->defeated == enemySize)
        {
          gameOver = true;
        }
      }
      else
      {
        bool stillInBattle = false;
        this->endBattle(resource, building, battle, buildBase);
        for (auto i : this->map)
          for (auto j : i)
            if (j != NULL)
            {
              if (j->battling)
                stillInBattle = true;
              j->isAttackable = j->isAttackable || std::find(j->attackable.begin(), j->attackable.end(), std::make_pair(this->coordX, this->coordY)) != j->attackable.end();
              if (!j->isEncircled)
              {
                bool allCaptured = true;
                for (auto k : j->encircled)
                {
                  if (!this->map[k.first][k.second]->captured)
                  {
                    allCaptured = false;
                    break;
                  }
                }
                if (allCaptured)
                  j->isEncircled = true;
              }
            }
        if (!stillInBattle)
        {
          battle->inBattle = false;
          battle->countryBattling = "";
        }
      }
      this->lg.unlock();
      return;
    }
    if (!this->battling)
    {
      this->battle.push_back(new BattleUnit([&](ArmyUnit *army) { this->retreat(army, battle, false); }, totalFoeTroop, totalFoeArmy, foeCount, this->lg, this->country, this->name, new BattleTroopWrapper({}, {}), new BattleTroopWrapper(this->totalFoeArmy, this->totalFoeTroop), this->terrainToDebuff[this->terrain], this->isEncircled));
      this->battling = true;
      battler->inBattle = true;
      battler->countryBattling = country;
      this->battlingRegions++;
    }
    reinforcement->inBattle = true;
    reinforcement->battleRegion = std::make_pair(this->country, this->name);
    for (auto i : reinforcement->formation)
      for (auto j : i)
      {
        if (j != NULL)
        {
          assert(j->reference.size() == 1);
          this->battle.back()->friendCount[j->type]++;
          this->battle.back()->totalFriendly++;
          troop->helper2[j->type](3, 1);
          j->state["battle"] = true;
        }
      }
    this->battle.back()->mikata->totalArmy.push_back(reinforcement);
    this->lg.unlock();
  }
  void retreat(TroopUnit *retreating, data::Battle *battle, bool userInitiated = true)
  {
    if (retreating == NULL)
      return;
    this->lg.lock();
    auto ptr = std::find(this->battle.back()->mikata->totalTroop.begin(), this->battle.back()->mikata->totalTroop.end(), retreating);
    assert(ptr != this->battle.back()->mikata->totalTroop.end());

    troop->helper2[retreating->type](3, -1);
    retreating->state["battle"] = false;
    assert(!retreating->state["army"]);
    assert(!retreating->state["battlePlan"]);
    retreating->state["free"] = true;
    troop->helper2[retreating->type](0, 1);

    this->battle.back()->mikata->totalTroop.erase(ptr);
    retreating->reference.pop_back();
    assert(retreating->reference.size() == 0);
    retreating->isReferenced = false;
    this->battle.back()->totalFriendly--;
    this->battle.back()->friendCount[retreating->type]--;
    if (userInitiated)
      this->checkEndBattleLose(battle);
    this->lg.unlock();
  }
  void retreat(ArmyUnit *retreating, data::Battle *battle, bool userInitiated = true)
  {
    if (retreating == NULL)
      return;
    this->lg.lock();
    auto ptr = std::find(this->battle.back()->mikata->totalArmy.begin(), this->battle.back()->mikata->totalArmy.end(), retreating);
    assert(ptr != this->battle.back()->mikata->totalArmy.end());

    retreating->inBattle = false;
    retreating->battleRegion = {};
    for (auto i : retreating->formation)
      for (auto j : i)
      {
        if (j != NULL)
        {
          this->battle.back()->friendCount[j->type]--;
          this->battle.back()->totalFriendly--;
          troop->helper2[j->type](3, -1);
          j->state["battle"] = false;
        }
      }

    this->battle.back()->mikata->totalArmy.erase(ptr);

    if (userInitiated)
      this->checkEndBattleLose(battle);
    this->lg.unlock();
  }
  void retreatAll(data::Battle *battle, bool userInitiated = true)
  {
    int c = this->battle.back()->mikata->totalTroop.size() - 1;
    while (this->battle.back()->mikata != NULL && c >= 0)
    {
      this->retreat(this->battle.back()->mikata->totalTroop[c], battle, userInitiated);
      c--;
    }
    if (this->battle.back()->mikata)
    {
      c = this->battle.back()->mikata->totalArmy.size() - 1;
      while (this->battle.back()->mikata != NULL && c >= 0)
      {
        this->retreat(this->battle.back()->mikata->totalArmy[c], battle, userInitiated);
        c--;
      }
    }
  }

  void cycle(std::function<void()> endGame, bool &gameOver, int enemySize, data::Troop *troop, data::Resource *resource, data::Building *building, data::Battle *battle, std::function<void(std::string type, int time, std::function<void(data::Resource &)> &callBack, std::string desc, double land, int amount)> buildBase)
  {
    this->battle.back()->fight(troop, resource);
    this->battle.back()->clean(troop, resource);
    this->checkEndBattle(endGame, gameOver, enemySize, resource, building, battle, buildBase);
    if (this->battling)
      this->battle.back()->regen(resource);
  }

private:
  // only called when the enemy has lost the battle
  void endBattle(data::Resource *resource, data::Building *building, data::Battle *battle, std::function<void(std::string type, int time, std::function<void(data::Resource &)> &callBack, std::string desc, double land, int amount)> buildBase)
  {
    this->captured = true;
    resource->capturedLand += this->acquirable["land"];
    if (this->acquirable["farm"] > 0)
      buildBase("farm", 0, building->effect["farm"][0], "", building->helper["farm"](0), this->acquirable["farm"]);
    if (this->acquirable["civilianFactory"] > 0)
      buildBase("civilianFactory", 0, building->effect["civilianFactory"][0], "", building->helper["civilianFactory"](0), this->acquirable["civilianFactory"]);
    if (this->acquirable["militaryFactory"] > 0)
      buildBase("militaryFactory", 0, building->effect["militaryFactory"][0], "", building->helper["militaryFactory"](0), this->acquirable["militaryFactory"]);
    if (this->acquirable["trainingCamp"] > 0)
      buildBase("trainingCamp", 0, building->effect["trainingCamp"][0], "", building->helper["trainingCamp"](0), this->acquirable["trainingCamp"]);
    if (this->acquirable["airport"] > 0)
      buildBase("airport", 0, building->effect["airport"][0], "", building->helper["airport"](0), this->acquirable["airport"]);
    if (this->battling)
    {
      this->battlingRegions--;
      this->retreatAll(battle, false);
      BattleUnit *ptr = this->battle.back();
      for (auto i : ptr->foe->totalTroop)
      {
        delete i;
        i = NULL;
      }
      for (auto i : ptr->foe->totalArmy)
      {
        for (auto j : i->formation)
          for (auto k : j)
          {
            delete k;
            k = NULL;
          }
        delete i;
        i = NULL;
      }
      delete ptr->foe;
      ptr->foe = NULL;
      delete ptr->mikata;
      ptr->mikata = NULL;
    }
    this->battling = false;
  }
  void checkEndBattleLose(data::Battle *battle)
  {
    BattleUnit *ptr = this->battle.back();
    bool nakamaShinda = false;
    bool nonEmptyarmynakama = false;

    for (auto i : ptr->mikata->totalArmy)
      for (auto j : i->formation)
        for (auto k : j)
          if (k != NULL)
          {
            nonEmptyarmynakama = true;
            break;
          }
    nakamaShinda = ptr->mikata->totalTroop.size() == 0 && !nonEmptyarmynakama;
    if (nakamaShinda)
    {
      BattleUnit *ptr = this->battle.back();
      delete ptr->foe;
      ptr->foe = NULL;
      delete ptr->mikata;
      ptr->mikata = NULL;
      this->battling = false;
      this->battlingRegions--;
      bool remainingBattling = false;
      for (auto i : this->map)
        for (auto j : i)
          if (j != NULL && j->battling)
          {
            remainingBattling = true;
            break;
          }

      if (!remainingBattling)
      {
        battle->inBattle = false;
        battle->countryBattling = "";
      }
    }
  }
  void checkEndBattle(std::function<void()> endGame, bool &gameOver, int enemySize, data::Resource *resource, data::Building *building, data::Battle *battle, std::function<void(std::string type, int time, std::function<void(data::Resource &)> &callBack, std::string desc, double land, int amount)> buildBase)
  {
    BattleUnit *ptr = this->battle.back();
    bool nakamaShinda = false;
    bool tekiShinda = false;

    bool nonEmptyarmynakama = false;

    for (auto i : ptr->mikata->totalArmy)
      for (auto j : i->formation)
        for (auto k : j)
          if (k != NULL)
          {
            nonEmptyarmynakama = true;
            break;
          }
    nakamaShinda = ptr->mikata->totalTroop.size() == 0 && !nonEmptyarmynakama;
    tekiShinda = ptr->foe->totalTroop.size() == 0 && this->foeCount["army"] == 0;
    if (tekiShinda)
    {
      this->capturedLand++;
      if (1.0 * this->capturedLand / this->totalLand >= 0.7)
      {
        this->capitulated = true;
        this->defeated++;
        battle->inBattle = false;
        battle->countryBattling = "";
        for (auto i : this->map)
          for (auto j : i)
            if (j != NULL && !j->captured)
              j->endBattle(resource, building, battle, buildBase);
        if (this->defeated == enemySize)
        {
          gameOver = true;
        }
      }
      else
      {
        bool stillInBattle = false;
        this->endBattle(resource, building, battle, buildBase);
        for (auto i : this->map)
          for (auto j : i)
            if (j != NULL)
            {
              if (j->battling)
                stillInBattle = true;
              j->isAttackable = j->isAttackable || std::find(j->attackable.begin(), j->attackable.end(), std::make_pair(this->coordX, this->coordY)) != j->attackable.end();
              if (!j->isEncircled)
              {
                bool allCaptured = true;
                for (auto k : j->encircled)
                {
                  if (!this->map[k.first][k.second]->captured)
                  {
                    allCaptured = false;
                    break;
                  }
                }
                if (allCaptured)
                  j->isEncircled = true;
              }
            }
        if (!stillInBattle)
        {
          battle->inBattle = false;
          battle->countryBattling = "";
        }
      }
    }
    else if (nakamaShinda)
    {
      BattleUnit *ptr = this->battle.back();
      delete ptr->foe;
      ptr->foe = NULL;
      delete ptr->mikata;
      ptr->mikata = NULL;
      this->battling = false;
      this->battlingRegions--;
      bool remainingBattling = false;
      for (auto i : this->map)
        for (auto j : i)
          if (j != NULL && j->battling)
          {
            remainingBattling = true;
            break;
          }

      if (!remainingBattling)
      {
        battle->inBattle = false;
        battle->countryBattling = "";
      }
    }
  }
};

class Enemy
{
public:
  Enemy(std::string name_, int &defeated_) : name(name_), defeated(defeated_) {}
  std::string name;

  int totalLand = 0;
  int capturedLand = 0;
  bool capitulated = false;

  int battlingRegions = 0;
  int &defeated;

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
            a.baseLandMul = 1.3;
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
            a.baseRecoveryDiff = 2;
            recovery[1] = true;
          },
          [&](Resource &a, Building &b, Troop &c, Army &d) {
            a.baseRecoveryDiff = 2.5;
            recovery[2] = true;
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
    int defeated = 0;
  };

}
#endif