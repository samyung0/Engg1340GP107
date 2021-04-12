#include <iostream>
#include <future>
#include <string>
#include <algorithm>

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

void Game::buildBase(std::string type, int time, std::function<void(data::Resource &)> &callBack, std::string desc, double land, int amount)
{
  this->lg3.lock();
  this->resource->manpowerInUse += amount;
  this->resource->usedLand += land * amount;

  std::vector<std::string> idStore;
  for (int i = 0; i < amount; i++)
  {
    idStore.push_back(this->uuid());
    this->building->progressTrack.push_back(std::make_tuple(type, idStore.back(), desc));
  }

  this->building->progressAsync[idStore[0]] = std::async(std::launch::async, [this, idStore, time, callBack, type]() {

    // here no matter how much the amount is, only 1 progress will be created so the time remaining stays in sync
    this->building->progress[idStore[0]] = new Progress(time, this->setting["speed"], this->fps, this->paused);
    for (int i = 1; i < idStore.size(); i++)
      this->building->progress[idStore[i]] = this->building->progress[idStore[0]];

    this->lg3.unlock();
    
    this->building->progress[idStore[0]]->start(this->lg3);

    this->lg3.lock();

    for (int i = 0; i < idStore.size(); i++)
      callBack(*this->resource);

    delete this->building->progress[idStore[0]];
    for (int i = 0; i < idStore.size(); i++)
      this->building->progress.erase(idStore[i]);

    for (int j = this->building->progressTrack.size() - 1; j >= 0; j--)
    {
      if (std::find(idStore.begin(), idStore.end(), std::get<1>(this->building->progressTrack[j])) != idStore.end())
      {
        this->building->progressTrack.erase(this->building->progressTrack.begin() + j);
      }
    }
    this->resource->manpowerInUse -= idStore.size();

    // workaround resource deadlock error (caused by deleting the map values within the map value I think)
    std::thread temp([this, idStore]() { this->building->progressAsync.erase(idStore[0]); });
    // must be detached for not blocking And progressAsync to be deleted properly
    temp.detach();

    this->lg3.unlock();
  });
}

void Game::buildBase(std::string type, int time, std::function<void()> callBack, std::string desc, double land, int amount)
{
  this->lg3.lock();
  this->resource->manpowerInUse += amount;
  this->resource->usedLand += land * amount;

  std::vector<std::string> idStore;
  for (int i = 0; i < amount; i++)
  {
    idStore.push_back(this->uuid());
    this->building->progressTrack.push_back(std::make_tuple(type, idStore.back(), desc));
  }

  this->building->progressAsync[idStore[0]] = std::async(std::launch::async, [this, idStore, time, callBack, type]() {
    this->building->progress[idStore[0]] = new Progress(time, this->setting["speed"], this->fps, this->paused);
    for (int i = 1; i < idStore.size(); i++)
      this->building->progress[idStore[i]] = this->building->progress[idStore[0]];

    this->lg3.unlock();
    this->building->progress[idStore[0]]->start(this->lg3);
    this->lg3.lock();

    for (int i = 0; i < idStore.size(); i++)
      callBack();

    delete this->building->progress[idStore[0]];
    for (int i = 0; i < idStore.size(); i++)
      this->building->progress.erase(idStore[i]);

    for (int j = this->building->progressTrack.size() - 1; j >= 0; j--)
    {
      if (std::find(idStore.begin(), idStore.end(), std::get<1>(this->building->progressTrack[j])) != idStore.end())
      {
        this->building->progressTrack.erase(this->building->progressTrack.begin() + j);
      }
    }
    this->resource->manpowerInUse -= idStore.size();

    std::thread temp([this, idStore]() { this->building->progressAsync.erase(idStore[0]); });
    temp.detach();

    this->lg3.unlock();
  });
}