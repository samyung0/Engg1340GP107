#include <string>
#include <unordered_map>
#include <vector>

#include "../game.h"
#include "../gameStruct.h"
#include "../../../data/troop/troop.h"

// visit Game::buildBase for documentation
void Game::trainBase(std::string type, std::function<void(data::Resource &, data::Troop &)> callBack, int camp, int airport, int time){
  this->lg3.lock();

  this->resource->campUsed+= camp;
  this->resource->airportUsed += airport;
  std::string id = this->uuid();
  this->troop->progressTrack.push_back(std::make_tuple(type, id));

  this->troop->progressAsync[id] = std::async(std::launch::async, [this, id, time, callBack, type, camp]() {
    this->troop->progress[id] = new Progress(time, this->setting["speed"]);
    this->lg3.unlock();
    this->troop->progress[id]->start(this->lg3);
    this->lg3.lock();
    callBack(*this->resource, *this->troop);
    delete this->troop->progress[id];
    this->troop->progress.erase(id);
    int index = 0;
    for (int j = 0; j < this->troop->progressTrack.size(); j++)
      if (std::get<1>(this->troop->progressTrack[j]) == id)
      {
        index = j;
        break;
      }
    this->troop->progressTrack.erase(this->troop->progressTrack.begin() + index);
    this->resource->campUsed-= camp;

    std::thread temp([this, id]() { this->troop->progressAsync.erase(id);});
    temp.detach();

    this->lg3.unlock();
  });
  
}