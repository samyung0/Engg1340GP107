#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>

#include "../game.h"
#include "../gameStruct.h"
#include "../../../data/troop/troop.h"

// visit Game::buildBase for documentation
void Game::trainBase(std::string type, std::function<void(data::Resource &, data::Troop &)> callBack, int camp, int airport, int time, int amount)
{
  this->lg3.lock();

  this->resource->campUsed += camp * amount;
  this->resource->airportUsed += airport * amount;

  std::vector<std::string> idStore;
  for (int i = 0; i < amount; i++)
  {
    idStore.push_back(this->uuid());
    this->troop->progressTrack.push_back(std::make_tuple(type, idStore.back()));
  }

  this->troop->progressAsync[idStore[0]] = std::async(std::launch::async, [this, idStore, time, callBack, type, camp]() {
    this->troop->progress[idStore[0]] = new Progress(time, this->setting["speed"], this->fps, this->paused);
    for (int i = 1; i < idStore.size(); i++)
      this->troop->progress[idStore[i]] = this->troop->progress[idStore[0]];

    this->lg3.unlock();
    this->troop->progress[idStore[0]]->start(this->lg3);
    this->lg3.lock();

    for (int i = 0; i < idStore.size(); i++)
      callBack(*this->resource, *this->troop);

    delete this->troop->progress[idStore[0]];
    for (int i = 0; i < idStore.size(); i++)
      this->troop->progress.erase(idStore[i]);

    for (int j = this->troop->progressTrack.size() - 1; j >= 0; j--)
    {
      if (std::find(idStore.begin(), idStore.end(), std::get<1>(this->troop->progressTrack[j])) != idStore.end())
      {
        this->troop->progressTrack.erase(this->troop->progressTrack.begin() + j);
      }
    }
    this->resource->campUsed -= camp * idStore.size();

    std::thread temp([this, idStore]() { this->troop->progressAsync.erase(idStore[0]); });
    temp.detach();

    this->lg3.unlock();
  });
}