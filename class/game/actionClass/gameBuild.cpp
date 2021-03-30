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

void Game::buildBase(std::string type, int time, std::function<void(data::Resource &)> &callBack, std::string desc, double land, int number)
{
  this->resource->manpowerInUse += number;
  this->resource->usedLand += land;

  std::cout << "before" << std::endl;

  for (int i = 0; i < number; i++)
  {
    std::cout << "inside for" << std::endl;
    this->lg3.lock();
    std::string id = this->uuid();
    this->building->progressTrack.push_back({type, id, desc});
    // not sure why but this async function blocks the parent build function
    // so if I put loopPrintBuild in the parent function, if wont be run until the progress loop is finished
    this->building->progressAsync[id] = std::async(std::launch::async, [&, id]() {
      std::cout << "inside async" << std::endl;
      this->building->progress[id] = new Progress(time, this->setting["speed"]);
      this->lg3.unlock();
      this->building->progress[id]->start(this->lg3);
      this->lg3.lock();
      callBack(*this->resource);
      std::cout << id << std::endl;
      this->building->progressAsync.erase(id);
      delete this->building->progress[id];
      this->building->progress.erase(id);
      int index = 0;
      for (int i = 0; i < this->building->progressTrack.size(); i++)
        if (std::get<2>(this->building->progressTrack[i]) == id)
        {
          index = i;
          break;
        }
      this->building->progressTrack.erase(this->building->progressTrack.begin() + index);

      this->resource->manpowerInUse--;
      this->lg3.unlock();
    });
  }

  std::cout << "after" << std::endl;

  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
}