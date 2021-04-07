#ifndef GAMEUNIT_H
#define GAMEUNIT_H
#include <string>
#include <utility>
#include <vector>
#include <functional>
#include <tuple>

#include "../../data/troop/troop.h"
#include "../damage/damage.h"

class ArmyUnit
{
public:
  ArmyUnit(std::string);
  ArmyUnit(std::string, std::vector<std::tuple<int ,int , TroopUnit *>>);

  // name should be unique among other armies (used as key in army struct map)
  std::string name;

  bool inBattle = false;
  // country name, region coordinate
  std::pair<std::string, std::string> battleRegion = {};

  bool battlePlanAssigned = false;
  std::string battlePlanName;

  double calsualtyCount = 0;
  // calculated by dividing lost troops/ total troops x 100
  double casualtyPercentage = 0;

  // pointer to troops
  std::vector<std::vector<TroopUnit *>> formation = {
      {NULL, NULL, NULL, NULL},
      {NULL, NULL, NULL, NULL},
      {NULL, NULL, NULL, NULL},
      {NULL, NULL, NULL, NULL}};

  // note when the army is gone is ways such as removed by user, it will not be deleted from the array
  // nor will it be removed when all troops inside it die
  // it will stay there so the array length does not shrink, so the randomly generated name will not collide (generated from the length)
  bool removed = false;

  int totalBaseFoodRequired = 0;
  int totalBaseEquipmentRequired = 0;

  int foodReductionPerLand = 0;
  int foodReductionPerAir = 0;

  int equipmentReductionPerLand = 0;
  int equipmentReductionPerAir = 0;

  int speedBoostPerLand = 0;

  int logisticCount = 0;

  // total required = totalbase - totalreduced
  // total supplied = totalbase * total food production / total food required (capped at totalbase)
  double totalFoodRequired = 0;
  double totalEquipmentRequired = 0;

  // supplied/ required
  double foodRatio = 1;
  double equipmentRatio = 1;

  // stored as decimal, displayed as percentage
  // all troops within army should have same subsequential strength
  double subsequentialStrength = 1.0;

  // is called when its either removed by user or lost in battle
  void disband();

  // calc and deal damage to all troops within the army during battle
  void calcDamage(Damage &);

  // remove troops that have 0 health or below
  void cleanUp();
};

// used to pass into battleUnit as a wrapper of how many troops are present (applicable to both enemy side and your side)
class battleTroopWrapper
{
public:
  // army, singular troop
  battleTroopWrapper(std::vector<std::string>, std::vector<std::pair<std::string, int>>);

  // army
  battleTroopWrapper(std::vector<std::string>);

  // singular troop
  battleTroopWrapper(std::vector<std::pair<std::string, int>>);
};

class BattleUnit
{
public:
  // country, region, your side troop, enemy side troop
  BattleUnit(std::string, std::string, battleTroopWrapper, battleTroopWrapper);
};

class BattlePlanUnit
{
public:
  std::vector<std::string> armyAssigned = {};
  std::vector<std::string> activated = {};

  std::string target = NULL;
  // region coordinate to be attacked in order
  std::vector<std::string> order = {};
};
#endif