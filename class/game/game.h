#ifndef GAME_H
#define GAME_H
#include <string>
#include <utility>
#include <vector>
#include <functional>
#include <map>
#include <unordered_map>

#include "../menuWrapper/menuWrapper.h"
#include "gameStruct.h"

class Game
{
public:
  ~Game();
  Game(const MenuWrapper &gameStats);
  void start();
  void fetchData(Resource&, Building&, Troop&, Army&, BattlePlan&)
  MenuWrapper stats;
  Resource resource;
  Building building;
  Troop troop;
  Army army;
  BattlePlan battlePlan;

private:
  void timer(int interval);
  
};

class Progress
{
public:
  int remain;
  int interval;
  Progress(int, int);
};

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

  // pair: troop type, health remaining
  std::vector<std::pair<std::string, int>> columnA = {{NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0}};
  std::vector<std::pair<std::string, int>> columnB = {{NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0}};
  std::vector<std::pair<std::string, int>> columnC = {{NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0}};
  std::vector<std::pair<std::string, int>> columnD = {{NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0}};

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
}

class battleUnit{
  public:
    // country, region, your side troop, enemy side troop
    battleUnit(std::string, std::string, battleTroopWrapper, battleTroopWrapper);
}

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