#include <string>
#include <iostream>

#include "../game.h"
void Game::buildfarm1(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  this->lg3.lock();
  if ((this->resource->manpower - this->resource->manpowerInUse <= 0) || (this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand < this->building->farmL[0]))
  {
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    this->lg3.unlock();
    return;
  }
  this->lg3.unlock();
  this->buildBase("farm", this->building->farmT[0], this->building->effect["farm"][0], "farm", this->building->farmL[0],1);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::buildfarm5(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  this->lg3.lock();
  if ((this->resource->manpower - this->resource->manpowerInUse <= 4) || (this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand < this->building->farmL[0] * 5))
  {
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    this->lg3.unlock();
    return;
  }
  this->lg3.unlock();
    this->buildBase("farm", this->building->farmT[0], this->building->effect["farm"][0], "farm", this->building->farmL[0],5);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::buildfarm10(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  this->lg3.lock();
  if ((this->resource->manpower - this->resource->manpowerInUse <= 9) || (this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand < this->building->farmL[0] * 10))
  {
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    this->lg3.unlock();
    return;
  }
  this->lg3.unlock();

    this->buildBase("farm", this->building->farmT[0], this->building->effect["farm"][0], "farm", this->building->farmL[0],10);
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
    this->buildBase("farm", this->building->farmT[0], this->building->effect["farm"][0], "farm", this->building->farmL[0],max);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::buildcivilianFactory1(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  this->lg3.lock();
  if ((this->resource->manpower - this->resource->manpowerInUse <= 0) || (this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand < this->building->civilianFactoryL[0]))
  {
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    this->lg3.unlock();
    return;
  }
  this->lg3.unlock();
  this->buildBase("civilianFactory", this->building->civilianFactoryT[0], this->building->effect["civilianFactory"][0], "civ", this->building->civilianFactoryL[0],1);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::buildcivilianFactory5(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  this->lg3.lock();
  if ((this->resource->manpower - this->resource->manpowerInUse <= 4) || (this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand < this->building->civilianFactoryL[0] * 5))
  {
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    this->lg3.unlock();
    return;
  }
  this->lg3.unlock();
    this->buildBase("civilianFactory", this->building->civilianFactoryT[0], this->building->effect["civilianFactory"][0], "civ", this->building->civilianFactoryL[0],5);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::buildcivilianFactory10(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  this->lg3.lock();
  if ((this->resource->manpower - this->resource->manpowerInUse <= 9) || (this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand < this->building->civilianFactoryL[0] * 10))
  {
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    this->lg3.unlock();
    return;
  }
  this->lg3.unlock();

    this->buildBase("civilianFactory", this->building->civilianFactoryT[0], this->building->effect["civilianFactory"][0], "civ", this->building->civilianFactoryL[0],10);
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
    this->buildBase("civilianFactory", this->building->civilianFactoryT[0], this->building->effect["civilianFactory"][0], "civ", this->building->civilianFactoryL[0],max);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::buildmilitaryFactory1(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  this->lg3.lock();
  if ((this->resource->manpower - this->resource->manpowerInUse <= 0) || (this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand < this->building->militaryFactoryL[0]))
  {
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    this->lg3.unlock();
    return;
  }
  this->lg3.unlock();
  this->buildBase("militaryFactory", this->building->militaryFactoryT[0], this->building->effect["militaryFactory"][0], "mil", this->building->militaryFactoryL[0],1);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::buildmilitaryFactory5(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  this->lg3.lock();
  if ((this->resource->manpower - this->resource->manpowerInUse <= 4) || (this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand < this->building->militaryFactoryL[0] * 5))
  {
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    this->lg3.unlock();
    return;
  }
  this->lg3.unlock();
    this->buildBase("militaryFactory", this->building->militaryFactoryT[0], this->building->effect["militaryFactory"][0], "mil", this->building->militaryFactoryL[0],5);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::buildmilitaryFactory10(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  this->lg3.lock();
  if ((this->resource->manpower - this->resource->manpowerInUse <= 9) || (this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand < this->building->militaryFactoryL[0] * 10))
  {
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    this->lg3.unlock();
    return;
  }
  this->lg3.unlock();

    this->buildBase("militaryFactory", this->building->militaryFactoryT[0], this->building->effect["militaryFactory"][0], "mil", this->building->militaryFactoryL[0],10);
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
    this->buildBase("militaryFactory", this->building->militaryFactoryT[0], this->building->effect["militaryFactory"][0], "mil", this->building->militaryFactoryL[0],max);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}

void Game::buildtrainingCamp1(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  this->lg3.lock();
  if ((this->resource->manpower - this->resource->manpowerInUse <= 0) || (this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand < this->building->trainingCampL[0]))
  {
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    this->lg3.unlock();
    return;
  }
  this->lg3.unlock();
  this->buildBase("trainingCamp", this->building->trainingCampT[0], this->building->effect["trainingCamp"][0], "camp", this->building->trainingCampL[0],1);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::buildtrainingCamp5(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  this->lg3.lock();
  if ((this->resource->manpower - this->resource->manpowerInUse <= 4) || (this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand < this->building->trainingCampL[0] * 5))
  {
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    this->lg3.unlock();
    return;
  }
  this->lg3.unlock();
    this->buildBase("trainingCamp", this->building->trainingCampT[0], this->building->effect["trainingCamp"][0], "camp", this->building->trainingCampL[0],5);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::buildtrainingCamp10(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  this->lg3.lock();
  if ((this->resource->manpower - this->resource->manpowerInUse <= 9) || (this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand < this->building->trainingCampL[0] * 10))
  {
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    this->lg3.unlock();
    return;
  }
  this->lg3.unlock();

    this->buildBase("trainingCamp", this->building->trainingCampT[0], this->building->effect["trainingCamp"][0], "camp", this->building->trainingCampL[0],10);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::buildtrainingCampmax(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  this->lg3.lock();
  double freeLand = this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand;
  int max = std::min((int)(freeLand / this->building->trainingCampL[0]), this->resource->manpower - this->resource->manpowerInUse);
  this->lg3.unlock();
  if (max == 0)
  {
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
    this->buildBase("trainingCamp", this->building->trainingCampT[0], this->building->effect["trainingCamp"][0], "camp", this->building->trainingCampL[0],max);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}

void Game::buildairport1(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  this->lg3.lock();
  if ((this->resource->manpower - this->resource->manpowerInUse <= 0) || (this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand < this->building->airportL[0]))
  {
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    this->lg3.unlock();
    return;
  }
  this->lg3.unlock();
  this->buildBase("airport", this->building->airportT[0], this->building->effect["airport"][0], "airport", this->building->airportL[0],1);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::buildairport5(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  this->lg3.lock();
  if ((this->resource->manpower - this->resource->manpowerInUse <= 4) || (this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand < this->building->airportL[0] * 5))
  {
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    this->lg3.unlock();
    return;
  }
  this->lg3.unlock();
    this->buildBase("airport", this->building->airportT[0], this->building->effect["airport"][0], "airport", this->building->airportL[0],5);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::buildairport10(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  this->lg3.lock();
  if ((this->resource->manpower - this->resource->manpowerInUse <= 9) || (this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand < this->building->airportL[0] * 10))
  {
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    this->lg3.unlock();
    return;
  }
  this->lg3.unlock();

    this->buildBase("airport", this->building->airportT[0], this->building->effect["airport"][0], "airport", this->building->airportL[0],10);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::buildairportmax(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;
  this->lg3.lock();
  double freeLand = this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand;
  int max = std::min((int)(freeLand / this->building->airportL[0]), this->resource->manpower - this->resource->manpowerInUse);
  this->lg3.unlock();
  if (max == 0)
  {
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
    this->buildBase("airport", this->building->airportT[0], this->building->effect["airport"][0], "airport", this->building->airportL[0],max);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
