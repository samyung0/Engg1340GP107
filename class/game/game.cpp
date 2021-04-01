#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <future>
#include <mutex>

#include "gameStruct.h"
#include "game.h"
#include "../../lib/uuid/uuid.hpp"
#include "../../io/io.h"
#include "../../data/troop/troop.h"

ArmyUnit::ArmyUnit(std::string name_) : name(name_) {}

// decrement remain by 1 every interval until 0
Progress::Progress(int time, int interval_)
    : remain(time), interval(interval_)
{
}
void Progress::start(std::mutex &lg3)
{
  while (this->remain > 0)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    lg3.lock();
    this->remain--;
    lg3.unlock();
  }
}

Game::~Game() {}
Game::Game(const std::unordered_map<std::string, int> &setting_) : setting(setting_) {}

void Game::start()
{
  resource = new data::Resource();
  building = new data::Building();
  troop = new data::Troop();
  army = new data::Army();
  battlePlan = new data::BattlePlan();
  research = new data::Research();
  battle = new data::Battle();
  this->timerThread = new std::thread(&Game::timer, this, this->setting["speed"]);

  char input;

  (this->*this->print[this->gamePhase])(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  // this->loopPrintStatus(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  while (1)
  {
    int prevGamePhase = this->gamePhase;

    // same method used in menuPhase
    input = getch();
    if (input == '\033')
    {
      getch();
      switch (getch())
      {
      case 'A':
        this->gamePhaseSelect[0] = (this->gamePhaseSelect[0] + -1 + this->map[this->gamePhase].size()) % this->map[this->gamePhase].size();
        this->gamePhaseSelect[1] = (this->gamePhaseSelect[1]) % this->map[this->gamePhase][this->gamePhaseSelect[0]].size();
        break;
      case 'B':
        this->gamePhaseSelect[0] = (this->gamePhaseSelect[0] + 1) % this->map[this->gamePhase].size();
        this->gamePhaseSelect[1] = (this->gamePhaseSelect[1]) % this->map[this->gamePhase][this->gamePhaseSelect[0]].size();
        break;
      case 'C':
        this->gamePhaseSelect[0] = (this->gamePhaseSelect[0]) % this->map[this->gamePhase].size();
        this->gamePhaseSelect[1] = (this->gamePhaseSelect[1] + 1) % this->map[this->gamePhase][this->gamePhaseSelect[0]].size();
        break;
      case 'D':
        this->gamePhaseSelect[0] = (this->gamePhaseSelect[0]) % this->map[this->gamePhase].size();
        this->gamePhaseSelect[1] = (this->gamePhaseSelect[1] + -1 + this->map[this->gamePhase][this->gamePhaseSelect[0]].size()) % this->map[this->gamePhase][this->gamePhaseSelect[0]].size();
        break;
      }
    }
    // progress game phase
    else if (input == '\n')
    {
      this->gamePhase = this->map[this->gamePhase][this->gamePhaseSelect[0]][this->gamePhaseSelect[1]];
      if (gamePhase > 0)
      {
        this->prevGamePhase.push_back(prevGamePhase);
        this->gamePhaseSelect[0] = 0;
        this->gamePhaseSelect[1] = 0;
      }
      else if (gamePhase == 0)
      {
        this->gamePhaseSelect[0] = 0;
        this->gamePhaseSelect[1] = 0;
      }
    }
    // default shortcut key for back
    else if (input == ' ')
    {
      if (this->prevGamePhase.size() > 0)
      {
        this->gamePhase = this->prevGamePhase.back();
        this->prevGamePhase.pop_back();
        this->gamePhaseSelect[0] = 0;
        this->gamePhaseSelect[1] = 0;
      }
    }

    this->stopLoopPrintStatus();
    this->stopLoopPrintBuild();
    this->stopLoopPrintResearch();
    this->stopLoopPrintTroop();
    
    if (this->gamePhase >= 0)
    {
      (this->*this->print[this->gamePhase])(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    }
    else
    {
      this->lguser.lock();
      (this->*this->action[-this->gamePhase])(this->gamePhase, prevGamePhase);
      this->lguser.unlock();
    }
  }

  // terminate timer thread
  this->terminate = true;
  timerThread->join();

  // sole::uuid1()

  // building.progressAsync.push_back(std::async(std::launch::async, [&](){
  //   int index = building.progress.size();
  //   building.progress.push_back(new Progress(building.farmT[0], 1000));
  //   building.effect["farm"][0]();

  //   // lock to avoid racing of progressAsync and progress
  //   lg.lock();
  //   building.progress.erase(building.progress.begin()+index-1);
  //   building.progressAsync.erase(building.progress.begin()+index-1);
  //   lg.unlock();
  // }));
}