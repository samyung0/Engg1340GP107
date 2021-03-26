#ifndef GAMEUNIT_H
#define GAMEUNIT_H
#include <string>
#include <utility>
#include <vector>
#include <functional>
#include <map>
#include <unordered_map>

#include "../../data/troop/troop.h"
class ArmyUnit
{
public:
  // name should be unique among other armies (used as key in army struct map)
  std::string name;

  bool inBattle = false;
  // country name, region coordinate
  std::pair<std::string, std::string> battleRegion = {NULL, NULL};

  bool battlePlanAssigned = false;
  std::string battlePlanName = NULL;

  double calsualtyCount = 0;
  // calculated by dividing lost troops/ total troops x 100
  double casualtyPercentage = 0;

  // pointer to dynamically created troops
  std::vector<Troop**> columnA = {NULL, NULL, NULL, NULL};
  std::vector<Troop**> columnB = {NULL, NULL, NULL, NULL};
  std::vector<Troop**> columnC = {NULL, NULL, NULL, NULL};
  std::vector<Troop**> columnD = {NULL, NULL, NULL, NULL};

  ArmyUnit(std::string);
};

// used to pass into battleUnit as a wrapper of how many troops are present (applicable to both enemy side and your side)
class battleTroopWrapper{
  public:
    // army, singular troop
    battleTroopWrapper(std::vector<std::string>, std::vector<std::pair<std::string, int>>);

    // army
    battleTroopWrapper(std::vector<std::string>);

    // singular troop
    battleTroopWrapper(std::vector<std::pair<std::string, int>>);
};

class BattleUnit{
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