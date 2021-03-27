#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <future>

#include "game.h"
#include "../../lib/uuid/uuid.hpp"
#include "../../io/io.h"

ArmyUnit::ArmyUnit(std::string name_) : name(name_) {}

// decrement remain by 1 every interval until 0
Progress::Progress(int time, int interval_)
    : remain(time), interval(interval_)
{
  while (this->remain > 0)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(this->interval));
    this->remain--;
  }
}

Game::~Game() {}
Game::Game(const std::unordered_map<std::string, int> &setting_) : setting(setting_) {}

void Game::start()
{

  this->timerThread = new std::thread(&Game::timer, this, this->setting["speed"]);

  char input;

  // this->print[this->gamePhase] returns the memory address of the class function inside class
  // therefore cant just use *this->print[this->gamePhase] and execute it, must specify that the memory address comes from class by using this->
  (this->*this->print[this->gamePhase])(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->loopPrintStatus(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  while (1)
  {

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
      this->prevGamePhase.push_back(this->gamePhase);
      this->gamePhase = this->map[this->gamePhase][this->gamePhaseSelect[0]][this->gamePhaseSelect[1]];
      this->gamePhaseSelect[0] = 0;
      this->gamePhaseSelect[1] = 0;
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

    if(this->gamePhase == 0) this->loopPrintStatus(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    else this->stopLoopPrintStatus();

    if (this->gamePhase > 0)
    {
      (this->*this->print[this->gamePhase])(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    }
    else if(this->gamePhase < 0)
    {
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