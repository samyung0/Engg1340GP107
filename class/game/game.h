#ifndef GAME_H
#define GAME_H
#include <string>
#include <utility>
#include <vector>
#include <functional>
#include <map>
#include <unordered_map>
#include <mutex>

#include "gameStruct.h"

class Game
{
public:
  ~Game();
  Game(const std::unordered_map<std::string, int> &);
  void start();

private:
  void printStatus();
  void timer(int);
  bool terminate = false;

  int day = 1;
  int totalTroops = 0;
  int totalFoodRequired = 0;
  int totalEquipmentRequired = 0;

  std::unordered_map<std::string, int> setting;
  data::Resource resource;
  data::Building building;
  data::Troop troop;
  data::Army army;
  data::BattlePlan battlePlan;
  data::Research research;
  std::mutex lg;
  std::vector<Troop *> allTroop;

  // similar to how menu phase works, we progress the phase number according to use actions
  std::vector<std::vector<std::vector<int>>> map{
      {{1, 2, 3, 4, 5, 6},
       {7, 8, 9, 10, 11}}};
  int gamePhase = 0;
  int prevGamePhase = 0;

  // format researches when printing, return string
  std::string helper(std::vector<bool> level)
  {
    return (level[2] ? "advanced" : level[1] ? "intermediate" : "rudimentary");
  }
};
#endif