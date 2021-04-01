#include <string>
#include <cmath>

#include "../game.h"

void Game::removefarm1(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  if (this->building->farm[0] == 0)
    return;
  this->lg3.lock();
  this->building->farm[0]--;
  this->building->reverseEffect["farm"][0](*this->resource);
  this->resource->usedLand -= this->building->farmL[0];
  this->lg3.unlock();
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::removefarmmax(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  if (this->building->farm[0] == 0)
    return;
  this->lg3.lock();
  int num = this->building->farm[0];
  this->building->farm[0]-=num;
  for (int i = 0; i < num; i++)
    this->building->reverseEffect["farm"][0](*this->resource);
  this->resource->usedLand -= this->building->farmL[0] * num;
  this->lg3.unlock();
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::removefarmLV21(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  if (this->building->farm[1] == 0)
    return;
  this->lg3.lock();
  this->building->farm[1]--;
  this->building->reverseEffect["farm"][1](*this->resource);
  this->resource->usedLand -= this->building->farmL[1];
  this->lg3.unlock();
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::removefarmLV2max(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  if (this->building->farm[1] == 0)
    return;
  this->lg3.lock();
  int num = this->building->farm[1];
  this->building->farm[1]-=num;
  for (int i = 0; i < num; i++)
    this->building->reverseEffect["farm"][1](*this->resource);
  this->resource->usedLand -= this->building->farmL[1] * num;
  this->lg3.unlock();
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::removefarmLV31(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  if (this->building->farm[2] == 0)
    return;
  this->lg3.lock();
  this->building->farm[2]--;
  this->building->reverseEffect["farm"][2](*this->resource);
  this->resource->usedLand -= this->building->farmL[2];
  this->lg3.unlock();
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::removefarmLV3max(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  if (this->building->farm[2] == 0)
    return;
  this->lg3.lock();
  int num = this->building->farm[2];
  this->building->farm[2]-=num;
  for (int i = 0; i < num; i++)
    this->building->reverseEffect["farm"][2](*this->resource);
  this->resource->usedLand -= this->building->farmL[2] * num;
  this->lg3.unlock();
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::removecivilianFactory1(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  this->lg3.lock();
  if (std::min(this->resource->manpower - this->resource->manpowerInUse, this->building->civilianFactory[0]) == 0)
  {
    this->lg3.unlock();
    return;
  }
this->building->civilianFactory[0]--;
  this->building->reverseEffect["civilianFactory"][0](*this->resource);
  this->resource->usedLand -= this->building->civilianFactoryL[0];
  this->lg3.unlock();
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::removecivilianFactorymax(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  this->lg3.lock();
  int num = std::min(this->resource->manpower - this->resource->manpowerInUse, this->building->civilianFactory[0]);
  if (num == 0)
  {
    this->lg3.unlock();
    return;
  }
  this->building->civilianFactory[0]-=num;
  for (int i = 0; i < num; i++)
    this->building->reverseEffect["civilianFactory"][0](*this->resource);
  this->resource->usedLand -= this->building->civilianFactoryL[0] * num;
  this->lg3.unlock();
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::removecivilianFactoryLV21(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  this->lg3.lock();
  if (std::min(this->resource->manpower - this->resource->manpowerInUse, this->building->civilianFactory[1]) == 0)
  {
    this->lg3.unlock();
    return;
  }
  this->building->civilianFactory[1]--;
  this->building->reverseEffect["civilianFactory"][1](*this->resource);
  this->resource->usedLand -= this->building->civilianFactoryL[1];
  this->lg3.unlock();
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::removecivilianFactoryLV2max(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  this->lg3.lock();
  int num = std::min(this->resource->manpower - this->resource->manpowerInUse, this->building->civilianFactory[1]);
  if (num == 0)
  {
    this->lg3.unlock();
    return;
  }
  this->building->civilianFactory[1]-=num;
  for (int i = 0; i < num; i++)
    this->building->reverseEffect["civilianFactory"][1](*this->resource);
  this->resource->usedLand -= this->building->civilianFactoryL[1] * num;
  this->lg3.unlock();
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::removecivilianFactoryLV31(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  this->lg3.lock();
  if (std::min(this->resource->manpower - this->resource->manpowerInUse, this->building->civilianFactory[2]) == 0)
  {
    this->lg3.unlock();
    return;
  }
  this->building->civilianFactory[2]--;
  this->building->reverseEffect["civilianFactory"][2](*this->resource);
  this->resource->usedLand -= this->building->civilianFactoryL[2];
  this->lg3.unlock();
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::removecivilianFactoryLV3max(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;

  this->lg3.lock();
  int num = std::min(this->resource->manpower - this->resource->manpowerInUse, this->building->civilianFactory[2]);
  if (this->building->civilianFactory[2] == 0)
  {
    this->lg3.unlock();
    return;
  }
  this->building->civilianFactory[2]-=num;
  for (int i = 0; i < num; i++)
    this->building->reverseEffect["civilianFactory"][2](*this->resource);
  this->resource->usedLand -= this->building->civilianFactoryL[2] * num;
  this->lg3.unlock();
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::removemilitaryFactory1(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  if (this->building->militaryFactory[0] == 0)
    return;
  this->lg3.lock();
  this->building->militaryFactory[0]--;
  this->building->reverseEffect["militaryFactory"][0](*this->resource);
  this->resource->usedLand -= this->building->militaryFactoryL[0];
  this->lg3.unlock();
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::removemilitaryFactorymax(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  if (this->building->militaryFactory[0] == 0)
    return;
  this->lg3.lock();
  int num = this->building->militaryFactory[0];
  this->building->militaryFactory[0]-=num;
  for (int i = 0; i < num; i++)
    this->building->reverseEffect["militaryFactory"][0](*this->resource);
  this->resource->usedLand -= this->building->militaryFactoryL[0] * num;
  this->lg3.unlock();
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::removemilitaryFactoryLV21(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  if (this->building->militaryFactory[1] == 0)
    return;
  this->lg3.lock();
  this->building->militaryFactory[1]--;
  this->building->reverseEffect["militaryFactory"][1](*this->resource);
  this->resource->usedLand -= this->building->militaryFactoryL[1];
  this->lg3.unlock();
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::removemilitaryFactoryLV2max(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  if (this->building->militaryFactory[1] == 0)
    return;
  this->lg3.lock();
  int num = this->building->militaryFactory[1];
  this->building->militaryFactory[1]-=num;
  for (int i = 0; i < num; i++)
    this->building->reverseEffect["militaryFactory"][1](*this->resource);
  this->resource->usedLand -= this->building->militaryFactoryL[1] * num;
  this->lg3.unlock();
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::removemilitaryFactoryLV31(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  if (this->building->militaryFactory[2] == 0)
    return;
  this->lg3.lock();
  this->building->militaryFactory[2]--;
  this->building->reverseEffect["militaryFactory"][2](*this->resource);
  this->resource->usedLand -= this->building->militaryFactoryL[2];
  this->lg3.unlock();
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::removemilitaryFactoryLV3max(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  if (this->building->militaryFactory[2] == 0)
    return;
  this->lg3.lock();
  int num = this->building->militaryFactory[2];
  this->building->militaryFactory[2]-=num;
  for (int i = 0; i < num; i++)
    this->building->reverseEffect["militaryFactory"][2](*this->resource);
  this->resource->usedLand -= this->building->militaryFactoryL[2] * num;
  this->lg3.unlock();
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::removetrainingCamp1(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  this->lg3.lock();
  if (std::min(this->resource->camp - this->resource->campUsed, this->building->trainingCamp[0]) == 0)
  {
    this->lg3.unlock();
    return;
  }
this->building->trainingCamp[0]--;
  this->building->reverseEffect["trainingCamp"][0](*this->resource);
  this->resource->usedLand -= this->building->trainingCampL[0];
  this->lg3.unlock();
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::removetrainingCampmax(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  this->lg3.lock();
  int num = std::min(this->resource->camp - this->resource->campUsed, this->building->trainingCamp[0]);
  if (this->building->trainingCamp[0] == 0)
  {
    this->lg3.unlock();
    return;
  }
  this->building->trainingCamp[0]-=num;
  for (int i = 0; i < num; i++)
    this->building->reverseEffect["trainingCamp"][0](*this->resource);
  this->resource->usedLand -= this->building->trainingCampL[0] * num;
  this->lg3.unlock();
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::removeairport1(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  this->lg3.lock();
  if (std::min(this->resource->airport - this->resource->airportUsed, this->building->airport[0]) == 0)
  {
    this->lg3.unlock();
    return;
  }
this->building->airport[0]--;
  this->building->reverseEffect["airport"][0](*this->resource);
  this->resource->usedLand -= this->building->airportL[0];
  this->lg3.unlock();
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::removeairportmax(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  this->lg3.lock();
  int num = std::min(this->resource->airport - this->resource->airportUsed, this->building->airport[0]);
  if (this->building->airport[0] == 0)
  {
    this->lg3.unlock();
    return;
  }
  this->building->airport[0]-=num;
  for (int i = 0; i < num; i++)
    this->building->reverseEffect["airport"][0](*this->resource);
  this->resource->usedLand -= this->building->airportL[0] * num;
  this->lg3.unlock();
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}