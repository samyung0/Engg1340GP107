#include <string>
#include <iostream>

#include "../game.h"
#include "../gameStruct.h"

// visit Game::buildBase for documentation
void Game::researchBase(std::string desc, std::function<void(data::Resource &, data::Building &, data::Troop &, data::Army &, data::BattlePlan &, data::Battle &)> & callBack, int time){
  this->lg3.lock();
  
  std::string id = this->uuid();
  this->research->progressTrack = std::make_tuple(desc, id);
  this->research->progressAsync[id] = std::async(std::launch::async, [this, id, callBack, time](){
    this->research->progress[id] = new Progress(time, this->setting["speed"]);
    this->lg3.unlock();

    this->research->progress[id]->start(this->lg3);

    this->lg3.lock();
    callBack(*this->resource, *this->building, *this->troop, *this->army, *this->battlePlan, *this-> battle);

    delete this->research->progress[id];
    this->research->progress.erase(id);
    this->research->progressTrack = {};

    std::thread temp([this, id]() { this->building->progressAsync.erase(id);});
    temp.detach();

    this->lg3.unlock();
  });

}
  