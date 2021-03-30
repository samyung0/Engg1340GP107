#include <string>

#include "../game.h"
void Game::buildfarm1(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  this->lg3.lock();
  if ((this->resource->manpower - this->resource->manpowerInUse <= 0) || (this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand < this->building->farmL[0]))
  {
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    this->lg3.unlock();
    return;
  }
  this->lg3.unlock();
  this->buildBase("farm", this->building->farmT[0], this->building->effect["farm"][0], "farm", this->building->farmL[0]);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::buildfarm5(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  this->lg3.lock();
  if ((this->resource->manpower - this->resource->manpowerInUse <= 4) || (this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand < this->building->farmL[0] * 5))
  {
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    this->lg3.unlock();
    return;
  }
  this->lg3.unlock();
  for (int i = 0; i < 5; i++)
    this->buildBase("farm", this->building->farmT[0], this->building->effect["farm"][0], "farm", this->building->farmL[0]);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::buildfarm10(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  this->lg3.lock();
  if ((this->resource->manpower - this->resource->manpowerInUse <= 9) || (this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand < this->building->farmL[0] * 10))
  {
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    this->lg3.unlock();
    return;
  }
  this->lg3.unlock();
  for (int i = 0; i < 10; i++)
    this->buildBase("farm", this->building->farmT[0], this->building->effect["farm"][0], "farm", this->building->farmL[0]);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::buildfarmmax(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  this->lg3.lock();
  double freeLand = this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand;
  int max = std::min((int)(freeLand / this->building->farmL[0]), this->resource->manpower - this->resource->manpowerInUse);
  this->lg3.unlock();
  if (max == 0)
  {
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  for (int i = 0; i < max; i++)
    this->buildBase("farm", this->building->farmT[0], this->building->effect["farm"][0], "farm", this->building->farmL[0]);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::buildcivilianFactory1(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  this->lg3.lock();
  if ((this->resource->manpower - this->resource->manpowerInUse <= 0) || (this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand < this->building->civilianFactoryL[0]))
  {
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    this->lg3.unlock();
    return;
  }
  this->lg3.unlock();
  this->buildBase("civilianFactory", this->building->civilianFactoryT[0], this->building->effect["civilianFactory"][0], "civ", this->building->civilianFactoryL[0]);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::buildcivilianFactory5(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  this->lg3.lock();
  if ((this->resource->manpower - this->resource->manpowerInUse <= 4) || (this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand < this->building->civilianFactoryL[0] * 5))
  {
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    this->lg3.unlock();
    return;
  }
  this->lg3.unlock();
  for (int i = 0; i < 5; i++)
    this->buildBase("civilianFactory", this->building->civilianFactoryT[0], this->building->effect["civilianFactory"][0], "civ", this->building->civilianFactoryL[0]);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::buildcivilianFactory10(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  this->lg3.lock();
  if ((this->resource->manpower - this->resource->manpowerInUse <= 9) || (this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand < this->building->civilianFactoryL[0] * 10))
  {
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    this->lg3.unlock();
    return;
  }
  this->lg3.unlock();
  for (int i = 0; i < 10; i++)
    this->buildBase("civilianFactory", this->building->civilianFactoryT[0], this->building->effect["civilianFactory"][0], "civ", this->building->civilianFactoryL[0]);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::buildcivilianFactorymax(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  this->lg3.lock();
  double freeLand = this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand;
  int max = std::min((int)(freeLand / this->building->civilianFactoryL[0]), this->resource->manpower - this->resource->manpowerInUse);
  this->lg3.unlock();
  if (max == 0)
  {
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  for (int i = 0; i < max; i++)
    this->buildBase("civilianFactory", this->building->civilianFactoryT[0], this->building->effect["civilianFactory"][0], "civ", this->building->civilianFactoryL[0]);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::buildmilitaryFactory1(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  this->lg3.lock();
  if ((this->resource->manpower - this->resource->manpowerInUse <= 0) || (this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand < this->building->militaryFactoryL[0]))
  {
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    this->lg3.unlock();
    return;
  }
  this->lg3.unlock();
  this->buildBase("militaryFactory", this->building->militaryFactoryT[0], this->building->effect["militaryFactory"][0], "mil", this->building->militaryFactoryL[0]);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::buildmilitaryFactory5(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  this->lg3.lock();
  if ((this->resource->manpower - this->resource->manpowerInUse <= 4) || (this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand < this->building->militaryFactoryL[0] * 5))
  {
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    this->lg3.unlock();
    return;
  }
  this->lg3.unlock();
  for (int i = 0; i < 5; i++)
    this->buildBase("militaryFactory", this->building->militaryFactoryT[0], this->building->effect["militaryFactory"][0], "mil", this->building->militaryFactoryL[0]);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::buildmilitaryFactory10(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  this->lg3.lock();
  if ((this->resource->manpower - this->resource->manpowerInUse <= 9) || (this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand < this->building->militaryFactoryL[0] * 10))
  {
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    this->lg3.unlock();
    return;
  }
  this->lg3.unlock();
  for (int i = 0; i < 10; i++)
    this->buildBase("militaryFactory", this->building->militaryFactoryT[0], this->building->effect["militaryFactory"][0], "mil", this->building->militaryFactoryL[0]);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::buildmilitaryFactorymax(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  this->lg3.lock();
  double freeLand = this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand;
  int max = std::min((int)(freeLand / this->building->militaryFactoryL[0]), this->resource->manpower - this->resource->manpowerInUse);
  this->lg3.unlock();
  if (max == 0)
  {
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  for (int i = 0; i < max; i++)
    this->buildBase("militaryFactory", this->building->militaryFactoryT[0], this->building->effect["militaryFactory"][0], "mil", this->building->militaryFactoryL[0]);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
