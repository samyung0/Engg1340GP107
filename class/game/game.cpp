#include <iostream>
#include <vector>
#include <unordered_map>
#include <functional>
#include <string>
#include <map>
#include <utility>
#include <thread>
#include <chrono>
#include <mutex>
#include <future>

#include "game.h"

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
Game::Game(const MenuWrapper &gameStats) : stats(gameStats) {}

void Game::start()
{

  mutex lg;


  Building.progressAsync.push_back(std::async(std::launch::async, [&](){
    int index = Building.progress.size();
    Building.progress.push_back(new Progress(Building.farmT[0], 1000));
    Building.effect["farm"][0]();

    // lock to avoid racing of progressAsync and progress
    lg.lock();
    Building.progress.erase(Building.progress.begin()+index-1);
    Building.progressAsync.erase(Building.progress.begin()+index-1);
    lg.unlock();
  }));
}