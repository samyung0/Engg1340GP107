#ifndef GAME_H
#define GAME_H
#include <string>
#include <vector>
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

  // similar to how menu phase works, we progress the phase number according to use actions
  std::vector<std::vector<std::vector<int>>> map{

      // building, researching, troops training, army editing, battle planning, battling
      {{1, 2, 3, 4, 5, 6},

       // set speed, pause (action), save as (action), restart (action), quit (action)
       {7, -1, -2, -3, -4}},

      // build (1-4), upgrade (5-8)
      // delete (1-2), upgraed (3 - 6)
      {
          {-5, -6, -7, -8, -9, -10, -11, -12},
          {-14, -15, -16, -17, -18, -19, -20, -21},
          {-23, -24, -25, -26, -27, -28, -29, -30},
          {-32, -33, -34, -35, -36, -37, -38, -39},
          {-41, -42, -43, -44, -45, -46, -47, -48},
          {-49, -50, -87, -88, -89, -90},
          {-58, -59},
          {-67, -68},
          {-76, -77},
          {-85, -86}}};
  int gamePhase = 0;
  int gamePhaseSelect[2] = {0, 0};

  // function to be executed according to gamePhase (printing)
  // input: x and y value
  std::vector<void (Game::*)(int, int)> print = {
      &Game::printStatus, &Game::printBuild};
  // , &Game::printResearch, &Game::printTroopTrain, &Game::printArmyEdit, &Game::printBattlePlan, &Game::printBattle};

  // function to be executed according to gamePhase (modifying)
  // std::vector<void (Game::*)()> action = {
  //     NULL, &Game::pause, &Game::saveAs, &Game::restart, &Game::quit};

private:
  void printStatus(int x, int y);
  void printBuild(int x, int y);
  void printResearch(int x, int y);
  void printTroopTrain(int x, int y);
  void printArmyEdit(int x, int y);
  void printBattlePlan(int x, int y);
  void printBattle(int x, int y);
  void printSetSpeed(int x, int y);

  void pause();
  void saveAs();
  void restart();
  void quit();

  void timer(int);
  bool terminate = false;

  int day = 1;

  std::unordered_map<std::string, int> setting;
  data::Resource resource;
  data::Building building;
  data::Troop troop;
  data::Army army;
  data::BattlePlan battlePlan;
  data::Research research;
  data::Battle battle;
  std::mutex lg;

  // format researches when printing, return string
  std::string helper(std::vector<bool> level)
  {
    return (level[2] ? "advanced" : level[1] ? "intermediate"
                                             : "rudimentary");
  }

  // format battling regions when printing, return string
  std::string helper2(std::vector<std::string> region)
  {
    std::string out;
    for (auto i : region)
      out += i + " ";
    return out.substr(0, out.size() - 1);
  }
};
#endif