#ifndef GAME_H
#define GAME_H
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <mutex>
#include <thread>
#include <chrono>
#include <future>
#include <condition_variable>

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
          {-49, -50, -51, -52},
          {-53, -54, -55, -56, -57, -58},
          {-59, -60, -61, -62, -63, -64},
          {-65, -66, -67, -68, -69, -70},
          {-71, -72},
          {-73, -74},
          {0}}};
  int gamePhase = 0;
  std::vector<int> prevGamePhase = {};
  int gamePhaseSelect[2] = {0, 0};

  // function to be executed according to gamePhase (printing)
  // input: x and y value
  std::vector<void (Game::*)(int, int)> print = {
      &Game::printStatus, &Game::printBuild};
  // , &Game::printResearch, &Game::printTroopTrain, &Game::printArmyEdit, &Game::printBattlePlan, &Game::printBattle};

  // function to be executed according to gamePhase (modifying)
  // length: 74
  std::vector<void (Game::*)()> action = {
      NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};

private:
  
  // separate timer thread to increment time only
  std::thread *timerThread;
  std::future<void> loopPrintStatusThread;

  // default to printing the base status every second
  // approach: call async to allocate non-blocking thread to tempLoopPrintStatus
  //           -> tempLoopPrintStatus waits 1 second for a conditional variable to notify instead of doing thread sleep (cannot wake thread when slept)
  //           -> async thread is terminated per user action (to update the x and y index value)
  //           -> terminated by first setting terminatePrint to false (stops while loop), then notifying the conditional variable (breaks wait_for)
  void loopPrintStatus(int x, int y)
  {
    if (this->loopPrintStatusThread.valid())
    {
      this->lgPrintStatus.lock();
      this->terminatePrint = true;
      this->lgPrintStatus.unlock();
      terminatePrintCV.notify_all();
      this->loopPrintStatusThread.get();
    }
    this->loopPrintStatusThread = std::async(std::launch::async, &Game::tempLoopPrintStatus, this, x, y);
  };
  void stopLoopPrintStatus()
  {
      this->lgPrintStatus.lock();
      this->terminatePrint = true;
      this->lgPrintStatus.unlock();
      terminatePrintCV.notify_all();
      this->loopPrintStatusThread.get();
  }
  void tempLoopPrintStatus(int x, int y){
    this->terminatePrint = false;
      while (!terminatePrint)
      {
        this->printStatus(x, y);
        std::unique_lock<std::mutex> lock(this->lgPrintStatus);
        terminatePrintCV.wait_for(lock, std::chrono::milliseconds(1000));
      }
  }

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

  // Note timer only progress day variable, nothing else
  // training troop/ building have their own async loops
  void timer(int time)
  {
    while (!this->terminate)
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(time));

      this->lg.lock();
      this->day++;
      this->lg.unlock();
    }
  };
  bool terminate = false;
  bool terminatePrint = false;
  std::condition_variable terminatePrintCV;

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
  std::mutex lgPrintStatus;

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