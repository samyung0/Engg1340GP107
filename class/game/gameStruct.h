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

private:
  double milliRemain = 0;
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
  BattleUnit(std::mutex &lg_, std::string country_, std::string region_, BattleTroopWrapper *mikata_, BattleTroopWrapper *foe_, double terrainDebuff_, bool &isEncircled_) : terrainDebuff(terrainDebuff_), country(country_), region(region_), mikata(mikata_), foe(foe_), lg(lg_), isEncircled(isEncircled_)
  {
    for (auto i : foe->totalTroop)
    {
      totalFoeSoftAttack += i->getSoftAttack();
      totalFoeHardAttack += i->getHardAttack();
      totalFoeAirAttack += i->getAirAttack();
    }
    for (auto i : foe->totalArmy)
      for (auto j : i->formation)
        for (auto k : j)
          if (k != NULL)
          {
            totalFoeSoftAttack += k->getSoftAttack();
            totalFoeHardAttack += k->getHardAttack();
            totalFoeAirAttack += k->getAirAttack();
          }
  };

  std::string country;
  std::string region;
  double terrainDebuff;
  bool isEncircled;

  BattleTroopWrapper *mikata;
  BattleTroopWrapper *foe;

  std::unordered_map<std::string, int> armorTroop = {{"armoredCar", 1}, {"tank1", 1}, {"tank2", 1}, {"tankOshimai", 1}};
  std::unordered_map<std::string, int> airTroop = {{"cas", 1}, {"fighter", 1}, {"bomber", 1}, {"kamikaze", 1}};
  std::random_device rd;
  std::vector<std::function<std::vector<double>(std::random_device &)>> randArmy = {
      [](std::random_device &) -> std::vector<double> { return {1}; }, &randArmy2, &randArmy3, &randArmy4};


  std::mutex &lg;

  // stats
  int duration = 0;
  Damage *damageDealt = new Damage();
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
  std::unordered_map<std::string, int> deathCount = {
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
      {"kamikaze", 0},
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
    double foodRatio = std::min(1.0, resource->food / troop->totalFoodRequired);
    double equipmentRatio = std::min(1.0, resource->equipment / troop->totalEquipmentRequired);
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
    for (auto i : this->mikata->totalTroop)
    {
      i->pivotalStrength = std::pow(1.2 - std::exp(-1.5 * (i->getHealth() / i->getBaseHealth()) + std::log(0.2) + 1.5), 1 - i->getHealth() / i->getBaseHealth());
      i->subsequentialStrength = (foodRatio + equipmentRatio) / 2;
      i->attackDebuff = this->terrainDebuff * (1 - std::min(1.0, i->getSpeed() / 10.0));
      totalFriendlyAirAttack += i->getAirAttack() * equipmentRatio;
      this->totalFriendlyDisruption += i->getDisruption();
      if (i->type != "suicideBomber" && i->type != "kamikaze")
        nonSpecialTroop++;
      else
        specialTroop++;
      unitFd++;
      if (this->armorTroop.count(i->type) != 0)
        armorTroopFd++;
      if (this->airTroop.count(i->type) != 0)
      {
        totalConspicuousnessFd += i->getConspicuousness();
        airTroopFd++;
      }
    }
    for (auto i : this->foe->totalTroop)
    {
      i->pivotalStrength = std::pow(1.2 - std::exp(-1.5 * (i->getHealth() / i->getBaseHealth()) + std::log(0.2) + 1.5), 1 - i->getHealth() / i->getBaseHealth());
      i->subsequentialStrength = this->isEncircled ? 0.5 : 1;
      i->attackDebuff = 0;
      totalFoeAirAttack += i->getAirAttack();
      this->totalFoeDisruption += i->getDisruption();
      unitFoe++;
      if (this->armorTroop.count(i->type) != 0)
        armorTroopFoe++;
      if (this->airTroop.count(i->type) != 0)
      {
        totalConspicuousnessFoe += i->getConspicuousness();
        airTroopFoe++;
      }
    }
    for (auto i : this->mikata->totalArmy)
    {
      double armyFoodRatio = std::min(1.0, (foodRatio * i->totalBaseFoodRequired) / i->totalFoodRequired);
      double armyEquipmentRatio = std::min(1.0, (equipmentRatio * i->totalBaseEquipmentRequired) / i->totalEquipmentRequired);
      bool isEmpty = true;
      for (auto j : i->formation)
        for (auto k : j)
          if (k != NULL)
          {
            isEmpty = false;
            k->pivotalStrength = std::pow(1.2 - std::exp(-1.5 * (k->getHealth() / k->getBaseHealth()) + std::log(0.2) + 1.5), 1 - k->getHealth() / k->getBaseHealth());
            k->subsequentialStrength = (armyFoodRatio + armyEquipmentRatio) / 2;
            k->attackDebuff = this->terrainDebuff * (1 - std::min(1.0, (k->getSpeed() + i->speedBoostPerLand) / 10.0));
            totalFriendlyAirAttack += k->getAirAttack() * armyEquipmentRatio;
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
          }
      if (!isEmpty)
        unitFd++;
    }
    for (auto i : this->foe->totalArmy)
    {
      bool isEmpty = true;
      for (auto j : i->formation)
        for (auto k : j)
          if (k != NULL)
          {
            isEmpty = false;
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
          }
      if (!isEmpty)
        unitFoe++;
    }
    this->totalFriendlyDisruption = 0.5 - std::exp(-0.1 * (this->totalFriendlyDisruption - 10 * std::log(0.5)));
    this->totalFoeDisruption = 0.5 - std::exp(-0.1 * (this->totalFoeDisruption - 10 * std::log(0.5)));
    airSupremacy = (totalFoeAirAttack == 0 ? 1 : 1 - std::exp((std::log(0.1) * totalFriendlyAirAttack) / (1.3 * totalFoeAirAttack)));
    normalizedAirSurpremacy = airSupremacy * 0.3 + 0.7;

    // damage structs
    Damage *foe = new Damage();
    Damage *nakama = new Damage();
    for (auto i : this->mikata->totalTroop)
    {
      if (nonSpecialTroop < specialTroop * 2 && (i->type == "suicideBomber" || i->type == "kamikaze"))
        continue;
      i->giveDamage(this->totalFriendlyDisruption, normalizedAirSurpremacy, resource->baseLandTroopMul, resource->baseAirTroopMul, resource->baseTankMul, resource->baseAirToopMul2, *nakama);
    }
    for (auto i : this->foe->totalTroop)
    {
      i->giveDamage(this->totalFriendlyDisruption, normalizedAirSurpremacy, 1, 1, 1, 1, *foe);
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
            k->giveDamage(this->totalFriendlyDisruption, normalizedAirSurpremacy, 1, 1, 1, 1, *foe);
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
    this->damageDealt->softAttack += nakama->softAttack;
    this->damageDealt->hardAttack += nakama->hardAttack;
    this->damageDealt->airAttack += nakama->airAttack;
    this->totalSoftAttack = nakama->softAttack;
    this->totalHardAttack = nakama->hardAttack;
    this->totalAirAttack = nakama->airAttack;

    double perUnit = foe->softAttack / unitFd;
    double perArmor = foe->hardAttack / armorTroopFd;
    double perUnitFoe = nakama->softAttack / unitFoe;
    double perArmorFoe = nakama->hardAttack / armorTroopFoe;
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
          damage = std::min(0., (perUnit - i->getDefense()) / 4) + std::min(0., (perArmor - i->getArmor()) * 1.1);
        }
        else
        {
          damage = perUnit - i->getDefense();
        }
      }
      damage = std::min(damage, 0.);
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
          damage = std::min(0., (perUnitFoe - i->getDefense()) / 4) + std::min(0., (perArmorFoe - i->getArmor()) * 1.1);
        }
        else
        {
          damage = perUnitFoe - i->getDefense();
        }
      }
      damage = std::min(damage, 0.);
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
        columnDistribution[1] = 0.3 + acc;
      else
        acc += 0.3;
      if (landPresent[2] != 0)
        columnDistribution[2] = 0.1 + acc;
      else
        acc += 0.1;
      columnDistribution[3] = acc;
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
              damage = foe->airAttack * i->formation[j][k]->getConspicuousness() / totalConspicuousnessFd;
            else
            {
              if (this->armorTroop.count(i->formation[j][k]->type) != 0)
                damage = std::min(0., (perUnit * columnDistribution[j] * distribution[count] - i->formation[j][k]->getDefense()) / 4) + std::min(0., (perArmor - i->formation[j][k]->getArmor()) * 1.1);
              else
                damage = perUnit * columnDistribution[j] * distribution[count] - i->formation[j][k]->getDefense();
              count++;
            }
            damage = std::min(damage, 0.);
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
        columnDistribution[1] = 0.3 + acc;
      else
        acc += 0.3;
      if (landPresent[2] != 0)
        columnDistribution[2] = 0.1 + acc;
      else
        acc += 0.1;
      columnDistribution[3] = acc;
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
                damage = std::min(0., (perUnitFoe * columnDistribution[j] * distribution[count] - i->formation[j][k]->getDefense()) / 4) + std::min(0., (perArmorFoe - i->formation[j][k]->getArmor()) * 1.1);
              else
                damage = perUnitFoe * columnDistribution[j] * distribution[count] - i->formation[j][k]->getDefense();
              count++;
            }
            damage = std::min(damage, 0.);
            i->formation[j][k]->takeDamage(damage);
          }
      }
    }

    this->duration++;
    this->lg.unlock();
  }
  void regen(data::Resource *resource)
  {
    for (auto i : this->mikata->totalTroop)
      i->increaseHealth((resource->baseRecovery + resource->baseRecoveryDiff) * (1 - totalFoeDisruption / 2) * std::pow(foodRatio, 1.5));
    for (auto i : this->mikata->totalArmy)
      for (auto j : i->formation)
        for (auto k : j)
          k->increaseHealth((resource->baseRecovery + resource->baseRecoveryDiff) * (1 - totalFoeDisruption / 2) * std::pow(std::min(1.0, (foodRatio * i->totalBaseFoodRequired) / i->totalFoodRequired), 1.5));
    for (auto i : this->foe->totalTroop)
      i->increaseHealth(resource->baseRecovery * (1 - totalFriendlyDisruption / 2) * (this->isEncircled ? 0 : 1));
    for (auto i : this->foe->totalArmy)
      for (auto j : i->formation)
        for (auto k : j)
          k->increaseHealth(resource->baseRecovery * (1 - totalFriendlyDisruption / 2) * (this->isEncircled ? 0 : 1));
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
        for (int i = 0; i < troop->allTroop.size(); i++)
          if (troop->allTroop[i] == this->mikata->totalTroop[i])
          {
            index = i;
            break;
          }
        assert(index != -1);

        troop->totalFoodRequired -= this->mikata->totalTroop[i]->getFood();
        troop->totalEquipmentRequired -= this->mikata->totalTroop[i]->getEquipment();
        troop->totalTroops--;

        this->friendCount[this->mikata->totalTroop[i]->type]--;
        this->deathCount[this->mikata->totalTroop[i]->type]++;
        this->totalFriendly--;

        troop->helper2[this->mikata->totalTroop[i]->type](3, -1);
        troop->helper2[this->mikata->totalTroop[i]->type](0, -1);
        delete this->mikata->totalTroop[i];
        this->mikata->totalTroop.erase(this->mikata->totalTroop.begin() + i);
        troop->allTroop.erase(troop->allTroop.begin() + index);
      }
    }
    for (int i = this->foe->totalTroop.size() - 1; i >= 0; i--)
    {
      if (this->foe->totalTroop[i]->getHealth() <= 0)
      {
        assert(this->foe->totalTroop[i]->reference.size() == 0);

        this->killCount[this->foe->totalTroop[i]->type]++;

        delete this->foe->totalTroop[i];
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
            this->deathCount[k->type]++;
            this->totalFriendly--;

            troop->helper2[k->type](3, -1);
            troop->helper2[k->type](1, -1);
            troop->helper2[k->type](0, -1);
            delete k;
            i->formation[x][y] = NULL;
            troop->allTroop.erase(troop->allTroop.begin() + index);
          }
        }
    for (auto i : this->mikata->totalArmy)
      for (int x = 0; x < 4; x++)
        for (int y = 0; y < 4; y++)
        {
          TroopUnit *k = i->formation[x][y];
          if (k != NULL && k->getHealth() <= 0)
          {
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

            this->killCount[k->type]--;

            delete k;
            i->formation[x][y] = NULL;
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
  Block(data::Troop *troop_, data::Resource *resource_, data::Battle *battler_, int &capturedLand_, bool &capitulated_, int &battlingRegions_, int &totalLand_, int &defeated_, std::vector<std::vector<Block *>> &map_, int coordX_, int coordY_) : troop(troop_), resource(resource_), battler(battler_), capturedLand(capturedLand_), capitulated(capitulated_), battlingRegions(battlingRegions_), totalLand(totalLand_), defeated(defeated_), map(map_), coordX(coordX_), coordY(coordY_) {}
  data::Troop *troop;
  data::Resource *resource;
  data::Battle *battler;

  std::string country;
  std::string name;
  std::string terrain;

  int coordX;
  int coordY;

  std::unordered_map<std::string, int> terrainToDebuff = {
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
  std::vector<std::vector<Block *>> &map;

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
  void reinforce(TroopUnit *reinforcement)
  {
    assert(reinforcement != NULL);

    this->lg.lock();
    if (!this->battling)
    {
      this->battle.push_back(new BattleUnit(this->lg, this->country, this->name, new BattleTroopWrapper({}, {}), new BattleTroopWrapper(this->totalFoeArmy, this->totalFoeTroop), this->terrainToDebuff[this->terrain], this->isEncircled));
      this->battling = true;
      battler->inBattle = true;
      battler->countryBattling = country;
      this->battlingRegions++;
    }

    troop->helper2[reinforcement->type](0, -1);
    troop->helper2[reinforcement->type](3, 1);

    reinforcement->state["battle"] = true;
    reinforcement->reference.push_back(true);
    reinforcement->isReferenced = true;

    battle.back()->mikata->totalTroop.push_back(reinforcement);
    battle.back()->totalFriendly++;
    battle.back()->friendCount[reinforcement->type]++;

    this->lg.unlock();
  }
  void reinforce(ArmyUnit *reinforcement)
  {
    assert(reinforcement != NULL);
    this->lg.lock();
    if (!this->battling)
    {
      this->battle.push_back(new BattleUnit(this->lg, this->country, this->name, new BattleTroopWrapper({}, {}), new BattleTroopWrapper(this->totalFoeArmy, this->totalFoeTroop), this->terrainToDebuff[this->terrain], this->isEncircled));
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
          battle.back()->friendCount[j->type]++;
          battle.back()->totalFriendly++;
          troop->helper2[j->type](3, 1);
          j->state["battle"] = true;
        }
      }
    battle.back()->mikata->totalArmy.push_back(reinforcement);
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
    retreating->isReferenced = retreating->reference.size() != 0;
    this->battle.back()->totalFriendly--;
    this->battle.back()->friendCount[retreating->type]--;
    if (userInitiated)
      this->checkEndBattleLose(battle);
    this->lg.unlock();
  }
  void retreat(ArmyUnit *retreating, data::Battle *battle, bool userInitiated = true)
  {
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
    for (auto i : this->battle.back()->mikata->totalTroop)
      this->retreat(i, battle, userInitiated);
    for (auto i : this->battle.back()->mikata->totalArmy)
      this->retreat(i, battle, userInitiated);
  }

  void cycle(data::Troop *troop, data::Resource *resource, data::Building *building, data::Battle *battle, std::function<void(std::string type, int time, std::function<void(data::Resource &)> &callBack, std::string desc, double land, int amount)> buildBase)
  {
    this->lg.lock();
    this->battle.back()->fight(troop, resource);
    this->battle.back()->clean(troop, resource);
    this->battle.back()->regen(resource);
    this->lg.unlock();
    this->checkEndBattle(resource, building, battle, buildBase);
  }

private:
  // only called when the enemy has lost the battle
  void endBattle(data::Resource *resource, data::Building *building, data::Battle* battle, std::function<void(std::string type, int time, std::function<void(data::Resource &)> &callBack, std::string desc, double land, int amount)> buildBase)
  {
    BattleUnit *ptr = this->battle.back();
    if (this->battling)
    {
      this->battlingRegions--;
      this->captured = true;
      this->battling = false;
    }
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

    this->retreatAll(battle, false);
    
    for (auto i : ptr->foe->totalTroop)
      delete i;
    for (auto i : ptr->foe->totalArmy)
    {
      for (auto j : i->formation)
        for (auto k : j)
          delete k;
      delete i;
    }
    delete ptr->foe;
    ptr->foe = NULL;
    delete ptr->mikata;
    ptr->mikata = NULL;
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
      retreatAll(battle, false);
      BattleUnit *ptr = this->battle.back();
      delete ptr->foe;
      ptr->foe = NULL;
      delete ptr->mikata;
      ptr->mikata = NULL;
      this->battling = false;

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
  void checkEndBattle(data::Resource *resource, data::Building *building, data::Battle *battle, std::function<void(std::string type, int time, std::function<void(data::Resource &)> &callBack, std::string desc, double land, int amount)> buildBase)
  {
    BattleUnit *ptr = this->battle.back();
    bool nakamaShinda = false;
    bool tekiShinda = false;

    bool nonEmptyarmynakama = false;
    bool nonEmptyarmyteki = false;

    for (auto i : ptr->mikata->totalArmy)
      for (auto j : i->formation)
        for (auto k : j)
          if (k != NULL)
          {
            nonEmptyarmynakama = true;
            break;
          }
    for (auto i : ptr->foe->totalArmy)
      for (auto j : i->formation)
        for (auto k : j)
          if (k != NULL)
          {
            nonEmptyarmyteki = true;
            break;
          }
    nakamaShinda = ptr->mikata->totalTroop.size() == 0 && !nonEmptyarmynakama;
    tekiShinda = ptr->foe->totalTroop.size() == 0 && !nonEmptyarmyteki;

    if (tekiShinda)
    {
      this->capturedLand++;
      if (this->capturedLand / this->totalLand >= 0.7)
      {
        this->capitulated = true;
        this->defeated++;
        battle->inBattle = false;
        battle->countryBattling = "";
        for (auto i : this->map)
          for (auto j : i)
            if (j != NULL)
              j->endBattle(resource, building, battle, buildBase);
      }
      else
      {
        this->endBattle(resource, building, battle, buildBase);
        for (auto i : this->map)
          for (auto j : i)
            if (j != NULL)
            {
              j->isAttackable = j->isAttackable || std::find(j->attackable.begin(), j->attackable.end(), std::make_pair(this->coordX, this->coordY)) != j->attackable.end();
              if (!j->isEncircled)
              {
                bool allCaptured = true;
                for (auto k : j->encircled)
                  if (!this->map[k.first][k.second]->captured)
                  {
                    allCaptured = false;
                    break;
                  }
                if (allCaptured)
                  j->isEncircled = true;
              }
              break;
            }
      }
    }
    else if (nakamaShinda)
    {
      retreatAll(battle, false);
      BattleUnit *ptr = this->battle.back();
      delete ptr->foe;
      ptr->foe = NULL;
      delete ptr->mikata;
      ptr->mikata = NULL;
      this->battling = false;

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
    int defeated = 0;
  };

}
#endif