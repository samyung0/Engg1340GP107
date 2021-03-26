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

// Note timer only progress day variable, nothing else
// training troop/ building have their own async loops
void Game::timer(int time)
{
  // std::cout << "timer is" << time << std::endl;
  while (!this->terminate)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(time));

    this->lg.lock();
    day++;
    this->lg.unlock();
  }
}


void Game::start()
{
  std::thread timerThread(&Game::timer, this, this->setting["speed"]);
  while (1)
  {
    this->printStatus();
    getch();
    break;
  }

  // terminate timer thread
  this->terminate = true;
  timerThread.join();

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