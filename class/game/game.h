#include <string>
#include <utility>
#include <vector>

#include "../menuWrapper/menuWrapper.h"
#ifndef GAME_H
#define GAME_H

// using namespace data;
class Game
{
public:
  ~Game();
  Game(const MenuWrapper &gameStats);
  void start();

private:
  void timer(int interval);
  MenuWrapper copyStats;
};

class Progress
{
private:
  int remain;

public:
  Progress(int);
  int get();
  void minus(int);
};

class armyUnit
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

  armyUnit(std::string);
};

class battlePlanUnit
{
public:
  std::vector<std::string> armyAssigned = {};
  std::vector<std::string> activated = {};

  std::string target = NULL;
  // region coordinate to be attacked in order
  std::vector<std::string> order = {};
};

#endif