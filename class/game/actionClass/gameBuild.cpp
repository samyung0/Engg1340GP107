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
  this->resource->manpowerInUse+=amount;
  this->resource->usedLand += land*amount;

  std::vector<std::string> idStore;
  for(int i=0;i<amount;i++){
  idStore.push_back(this->uuid());
  this->building->progressTrack.push_back(std::make_tuple(type, idStore.back(), desc));
  }
  this->building->progressAsync[idStore[0]] = std::async(std::launch::async, [this, idStore, time, callBack, type]() {
    std::cout << "inside async" << std::endl;
    this->building->progress[idStore[0]] = new Progress(time, this->setting["speed"]);
    for(int i=1;i<idStore.size();i++) this->building->progress[idStore[i]] = this->building->progress[idStore[0]];

    this->lg3.unlock();
    this->building->progress[idStore[0]]->start(this->lg3, this->lg3high);


    this->lg3.lock();

    for(int i=0;i<idStore.size();i++)
    callBack(*this->resource);

    delete this->building->progress[idStore[0]];
    this->building->progress.erase(idStore[0]);

    int index = 0;
    for (int j =  this->building->progressTrack.size()-1; j >=0 ; j++){
      std::string t = std::get<1>(this->building->progressTrack[j]);
    if(std::find(idStore.begin(), idStore.end(), t) != idStore.end())
      {
        this->building->progressTrack.erase(this->building->progressTrack.begin() + j);
      }
    }
    this->resource->manpowerInUse-=idStore.size();
    
    // workaround resource deadlock error (caused by deleting the map values within the map value I think)
    std::thread temp([this, idStore]() { 
     this->building->progressAsync.erase(idStore[0]);
    });
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