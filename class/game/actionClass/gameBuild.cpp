#include <iostream>
#include <future>
#include <string>

#include "../game.h"
#include "../gameStruct.h"

// push to track progress (type, id of process by uuid lib)
// push to async progress to store async object
// start progress by pushing new Progress class to progress (finish after time)
// call callBack function as the effect of finishing the building
// release memory and remove coorelated items in tracking and async
// input: type of building, time to build, callback, description to be printed

// overloaded version is called by upgrading building since the callBack is a lambda wrapped around 2 lambdas and does not require any arguments
// also not passed by reference since it does not have a persistent memory address

void Game::buildBase(std::string type, int time, std::function<void(data::Resource &)> &callBack, std::string desc, double land)
{
  this->lg3.lock();
  this->resource->manpowerInUse++;
  this->resource->usedLand += land;
  std::string id = this->uuid();
  this->building->progressTrack.push_back(std::make_tuple(type, id, desc));
  this->building->progressAsync[id] = std::async(std::launch::async, [this, id, time, callBack, type]() {
    this->building->progress[id] = new Progress(time, this->setting["speed"]);
    this->lg3.unlock();
    this->building->progress[id]->start(this->lg3, this->lg3high);

    // high priority lock (should update before printing)
    this->lg3.lock();

    callBack(*this->resource);
    delete this->building->progress[id];
    this->building->progress.erase(id);
    int index = 0;
    for (int j = 0; j < this->building->progressTrack.size(); j++)
      if (std::get<1>(this->building->progressTrack[j]) == id)
      {
        index = j;
        break;
      }
    this->building->progressTrack.erase(this->building->progressTrack.begin() + index);

    this->resource->manpowerInUse--;
    
    // workaround resource deadlock error (caused by deleting the map values within the map value I think)
    std::thread temp([this, id]() { this->building->progressAsync.erase(id);});
    // must be detached for not blocking And progressAsync to be deleted properly
    temp.detach();

    this->lg3.unlock();
  });
  
}

void Game::buildBase(std::string type, int time, std::function<void()> callBack, std::string desc, double land)
{
  this->lg3.lock();
  this->resource->manpowerInUse++;
  this->resource->usedLand += land;
  std::string id = this->uuid();
  this->building->progressTrack.push_back(std::make_tuple(type, id, desc));
  this->building->progressAsync[id] = std::async(std::launch::async, [this, id, time, callBack, type]() {
    this->building->progress[id] = new Progress(time, this->setting["speed"]);
    this->lg3.unlock();
    this->building->progress[id]->start(this->lg3, this->lg3high);
    this->lg3.lock();
    callBack();
    delete this->building->progress[id];
    this->building->progress.erase(id);
    int index = 0;
    for (int j = 0; j < this->building->progressTrack.size(); j++)
      if (std::get<1>(this->building->progressTrack[j]) == id)
      {
        index = j;
        break;
      }
    this->building->progressTrack.erase(this->building->progressTrack.begin() + index);

    this->resource->manpowerInUse--;
    
    std::thread temp([this, id]() { this->building->progressAsync.erase(id);});
    temp.detach();

    this->lg3.unlock();
  });
  
}