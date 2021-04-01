#include <string>
#include <iostream>

#include "../game.h"
void Game::upgradefarm1(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;

  this->lg3.lock();
  double equivLand = this->building->farmL[1] - this->building->farmL[0];
  if ((this->resource->manpower - this->resource->manpowerInUse <= 0) || (this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand < equivLand || !this->research->farm[1] || this->building->farm[0] < 1))
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->building->farm[0]--;
  this->lg3.unlock();

  this->buildBase(
      "farm", this->building->farmT[1],
      [&]() {this->building->effect["farm"][1](*this->resource); this->building->reverseEffect["farm"][0](*this->resource); },
      "farm (1 -> 2)", equivLand);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::upgradefarm5(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;

  this->lg3.lock();
  double equivLand = this->building->farmL[1] - this->building->farmL[0];
  if ((this->resource->manpower - this->resource->manpowerInUse <= 4) || (this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand < equivLand * 5 || !this->research->farm[1] || this->building->farm[0] < 5))
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->building->farm[0] -= 5;
  this->lg3.unlock();

  for (int i = 0; i < 5; i++)
    this->buildBase(
        "farm", this->building->farmT[1],
        [&]() {this->building->effect["farm"][1](*this->resource); this->building->reverseEffect["farm"][0](*this->resource); },
        "farm (1 -> 2)", equivLand);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::upgradefarm10(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;

  this->lg3.lock();
  double equivLand = this->building->farmL[1] - this->building->farmL[0];
  if ((this->resource->manpower - this->resource->manpowerInUse <= 9) || (this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand < equivLand * 10 || !this->research->farm[1] || this->building->farm[0] < 10))
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->building->farm[0] -= 10;
  this->lg3.unlock();

  for (int i = 0; i < 10; i++)
    this->buildBase(
        "farm", this->building->farmT[1],
        [&]() {this->building->effect["farm"][1](*this->resource); this->building->reverseEffect["farm"][0](*this->resource); },
        "farm (1 -> 2)", equivLand);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::upgradefarmmax(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;

  this->lg3.lock();
  double equivLand = this->building->farmL[1] - this->building->farmL[0];
  double freeLand = this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand;
  int max = equivLand == 0 ? std::min(this->building->farm[0], this->resource->manpower - this->resource->manpowerInUse) : std::min(std::min((int)(freeLand / equivLand), this->resource->manpower - this->resource->manpowerInUse), this->building->farm[0]);
  if (max == 0 || !this->research->farm[1])
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->building->farm[0] -= max;
  this->lg3.unlock();
  for (int i = 0; i < max; i++)
    this->buildBase(
        "farm", this->building->farmT[1],
        [&]() {this->building->effect["farm"][1](*this->resource); this->building->reverseEffect["farm"][0](*this->resource); },
        "farm (1 -> 2)", equivLand);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::upgradefarm21(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;

  this->lg3.lock();
  double equivLand = this->building->farmL[2] - this->building->farmL[1];
  if ((this->resource->manpower - this->resource->manpowerInUse <= 0) || (this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand < equivLand || !this->research->farm[2] || this->building->farm[1] < 1))
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->building->farm[1]--;
  this->lg3.unlock();
  this->buildBase(
      "farm", this->building->farmT[2],
      [&]() {this->building->effect["farm"][2](*this->resource); this->building->reverseEffect["farm"][1](*this->resource); },
      "farm (2 -> 3)", equivLand);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::upgradefarm25(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;

  this->lg3.lock();
  double equivLand = this->building->farmL[2] - this->building->farmL[1];
  if ((this->resource->manpower - this->resource->manpowerInUse <= 4) || (this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand < equivLand * 5 || !this->research->farm[2] || this->building->farm[1] < 5))
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->building->farm[1] -= 5;
  this->lg3.unlock();
  for (int i = 0; i < 5; i++)
    this->buildBase(
        "farm", this->building->farmT[2],
        [&]() {this->building->effect["farm"][2](*this->resource); this->building->reverseEffect["farm"][1](*this->resource); },
        "farm (2 -> 3)", equivLand);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::upgradefarm210(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;

  this->lg3.lock();
  double equivLand = this->building->farmL[2] - this->building->farmL[1];
  if ((this->resource->manpower - this->resource->manpowerInUse <= 9) || (this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand < equivLand * 10 || !this->research->farm[2] || this->building->farm[1] < 10))
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->building->farm[1] -= 10;
  this->lg3.unlock();
  for (int i = 0; i < 10; i++)
    this->buildBase(
        "farm", this->building->farmT[2],
        [&]() {this->building->effect["farm"][2](*this->resource); this->building->reverseEffect["farm"][1](*this->resource); },
        "farm (2 -> 3)", equivLand);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::upgradefarm2max(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;

  this->lg3.lock();
  double equivLand = this->building->farmL[2] - this->building->farmL[1];
  double freeLand = this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand;
  int max = equivLand == 0 ? std::min(this->building->farm[1], this->resource->manpower - this->resource->manpowerInUse) : std::min(std::min((int)(freeLand / equivLand), this->resource->manpower - this->resource->manpowerInUse), this->building->farm[1]);
  if (max == 0 || !this->research->farm[2])
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  std::cout << max << std::endl;
  this->building->farm[1] -= max;
  this->lg3.unlock();
  for (int i = 0; i < max; i++)
    this->buildBase(
        "farm", this->building->farmT[2],
        [&]() {this->building->effect["farm"][2](*this->resource); this->building->reverseEffect["farm"][1](*this->resource); },
        "farm (2 -> 3)", equivLand);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::upgradecivilianFactory1(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;

  this->lg3.lock();
  double equivLand = this->building->civilianFactoryL[1] - this->building->civilianFactoryL[0];
  if ((this->resource->manpower - this->resource->manpowerInUse <= 0) || (this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand < equivLand || !this->research->divisionOfLabor[1] || this->building->civilianFactory[0] < 1))
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->building->civilianFactory[0]--;
  this->lg3.unlock();
  this->buildBase(
      "civilianFactory", this->building->civilianFactoryT[1],
      [&]() {this->building->effect["civilianFactory"][1](*this->resource); this->building->reverseEffect["civilianFactory"][0](*this->resource); },
      "civ (1 -> 2)", equivLand);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::upgradecivilianFactory5(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;

  this->lg3.lock();
  double equivLand = this->building->civilianFactoryL[1] - this->building->civilianFactoryL[0];
  if ((this->resource->manpower - this->resource->manpowerInUse <= 4) || (this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand < equivLand * 5 || !this->research->divisionOfLabor[1] || this->building->civilianFactory[0] < 5))
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->building->civilianFactory[0] -= 5;
  this->lg3.unlock();
  for (int i = 0; i < 5; i++)
    this->buildBase(
        "civilianFactory", this->building->civilianFactoryT[1],
        [&]() {this->building->effect["civilianFactory"][1](*this->resource); this->building->reverseEffect["civilianFactory"][0](*this->resource); },
        "civ (1 -> 2)", equivLand);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::upgradecivilianFactory10(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;

  this->lg3.lock();
  double equivLand = this->building->civilianFactoryL[1] - this->building->civilianFactoryL[0];
  if ((this->resource->manpower - this->resource->manpowerInUse <= 9) || (this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand < equivLand * 10 || !this->research->divisionOfLabor[1] || this->building->civilianFactory[0] < 10))
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->building->civilianFactory[0] -= 10;
  this->lg3.unlock();
  for (int i = 0; i < 10; i++)
    this->buildBase(
        "civilianFactory", this->building->civilianFactoryT[1],
        [&]() {this->building->effect["civilianFactory"][1](*this->resource); this->building->reverseEffect["civilianFactory"][0](*this->resource); },
        "civ (1 -> 2)", equivLand);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::upgradecivilianFactorymax(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;

  this->lg3.lock();
  double equivLand = this->building->civilianFactoryL[1] - this->building->civilianFactoryL[0];
  double freeLand = this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand;
  int max = equivLand == 0 ? std::min(this->building->civilianFactory[0], this->resource->manpower - this->resource->manpowerInUse) : std::min(std::min((int)(freeLand / equivLand), this->resource->manpower - this->resource->manpowerInUse), this->building->civilianFactory[0]);
  if (max == 0 || !this->research->divisionOfLabor[1])
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->building->civilianFactory[0] -= max;
  this->lg3.unlock();
  for (int i = 0; i < max; i++)
    this->buildBase(
        "civilianFactory", this->building->civilianFactoryT[1],
        [&]() {this->building->effect["civilianFactory"][1](*this->resource); this->building->reverseEffect["civilianFactory"][0](*this->resource); },
        "civ (1 -> 2)", equivLand);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::upgradecivilianFactory21(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;

  this->lg3.lock();
  double equivLand = this->building->civilianFactoryL[2] - this->building->civilianFactoryL[1];
  if ((this->resource->manpower - this->resource->manpowerInUse <= 0) || (this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand < equivLand || !this->research->divisionOfLabor[2] || this->building->civilianFactory[1] < 1))
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->building->civilianFactory[1]--;
  this->lg3.unlock();
  this->buildBase(
      "civilianFactory", this->building->civilianFactoryT[2],
      [&]() {this->building->effect["civilianFactory"][2](*this->resource); this->building->reverseEffect["civilianFactory"][1](*this->resource); },
      "civ (2 -> 3)", equivLand);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::upgradecivilianFactory25(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;

  this->lg3.lock();
  double equivLand = this->building->civilianFactoryL[2] - this->building->civilianFactoryL[1];
  if ((this->resource->manpower - this->resource->manpowerInUse <= 4) || (this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand < equivLand * 5 || !this->research->divisionOfLabor[2] || this->building->civilianFactory[1] < 5))
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->building->civilianFactory[1] -= 5;
  this->lg3.unlock();
  for (int i = 0; i < 5; i++)
    this->buildBase(
        "civilianFactory", this->building->civilianFactoryT[2],
        [&]() {this->building->effect["civilianFactory"][2](*this->resource); this->building->reverseEffect["civilianFactory"][1](*this->resource); },
        "civ (2 -> 3)", equivLand);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::upgradecivilianFactory210(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;

  this->lg3.lock();
  double equivLand = this->building->civilianFactoryL[2] - this->building->civilianFactoryL[1];
  if ((this->resource->manpower - this->resource->manpowerInUse <= 9) || (this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand < equivLand * 10 || !this->research->divisionOfLabor[2] || this->building->civilianFactory[1] < 10))
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->building->civilianFactory[1] -= 10;
  this->lg3.unlock();
  for (int i = 0; i < 10; i++)
    this->buildBase(
        "civilianFactory", this->building->civilianFactoryT[2],
        [&]() {this->building->effect["civilianFactory"][2](*this->resource); this->building->reverseEffect["civilianFactory"][1](*this->resource); },
        "civ (2 -> 3)", equivLand);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::upgradecivilianFactory2max(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;

  this->lg3.lock();
  double equivLand = this->building->civilianFactoryL[2] - this->building->civilianFactoryL[1];
  double freeLand = this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand;
  int max = equivLand == 0 ? std::min(this->building->civilianFactory[1], this->resource->manpower - this->resource->manpowerInUse) : std::min(std::min((int)(freeLand / equivLand), this->resource->manpower - this->resource->manpowerInUse), this->building->civilianFactory[1]);
  if (max == 0 || !this->research->divisionOfLabor[2])
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->building->civilianFactory[1] -= max;
  this->lg3.unlock();
  for (int i = 0; i < max; i++)
    this->buildBase(
        "civilianFactory", this->building->civilianFactoryT[2],
        [&]() {this->building->effect["civilianFactory"][2](*this->resource); this->building->reverseEffect["civilianFactory"][1](*this->resource); },
        "civ (2 -> 3)", equivLand);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::upgrademilitaryFactory1(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;

  this->lg3.lock();
  double equivLand = this->building->militaryFactoryL[1] - this->building->militaryFactoryL[0];
  if ((this->resource->manpower - this->resource->manpowerInUse <= 0) || (this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand < equivLand || !this->research->productionLine[1] || this->building->militaryFactory[0] < 1))
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->building->militaryFactory[0]--;
  this->lg3.unlock();
  this->buildBase(
      "militaryFactory", this->building->militaryFactoryT[1],
      [&]() {this->building->effect["militaryFactory"][1](*this->resource); this->building->reverseEffect["militaryFactory"][0](*this->resource); },
      "mil (1 -> 2)", equivLand);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::upgrademilitaryFactory5(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;

  this->lg3.lock();
  double equivLand = this->building->militaryFactoryL[1] - this->building->militaryFactoryL[0];
  if ((this->resource->manpower - this->resource->manpowerInUse <= 4) || (this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand < equivLand * 5 || !this->research->productionLine[1] || this->building->militaryFactory[0] < 5))
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->building->militaryFactory[0] -= 5;
  this->lg3.unlock();
  for (int i = 0; i < 5; i++)
    this->buildBase(
        "militaryFactory", this->building->militaryFactoryT[1],
        [&]() {this->building->effect["militaryFactory"][1](*this->resource); this->building->reverseEffect["militaryFactory"][0](*this->resource); },
        "mil (1 -> 2)", equivLand);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::upgrademilitaryFactory10(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;

  this->lg3.lock();
  double equivLand = this->building->militaryFactoryL[1] - this->building->militaryFactoryL[0];
  if ((this->resource->manpower - this->resource->manpowerInUse <= 9) || (this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand < equivLand * 10 || !this->research->productionLine[1] || this->building->militaryFactory[0] < 10))
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->building->militaryFactory[0] -= 10;
  this->lg3.unlock();
  for (int i = 0; i < 10; i++)
    this->buildBase(
        "militaryFactory", this->building->militaryFactoryT[1],
        [&]() {this->building->effect["militaryFactory"][1](*this->resource); this->building->reverseEffect["militaryFactory"][0](*this->resource); },
        "mil (1 -> 2)", equivLand);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::upgrademilitaryFactorymax(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;

  this->lg3.lock();
  double equivLand = this->building->militaryFactoryL[1] - this->building->militaryFactoryL[0];
  double freeLand = this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand;
  int max = equivLand == 0 ? std::min(this->building->militaryFactory[0], this->resource->manpower - this->resource->manpowerInUse) : std::min(std::min((int)(freeLand / equivLand), this->resource->manpower - this->resource->manpowerInUse), this->building->militaryFactory[0]);
  if (max == 0 || !this->research->productionLine[0])
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->building->militaryFactory[0] -= max;
  this->lg3.unlock();
  for (int i = 0; i < max; i++)
    this->buildBase(
        "militaryFactory", this->building->militaryFactoryT[1],
        [&]() {this->building->effect["militaryFactory"][1](*this->resource); this->building->reverseEffect["militaryFactory"][0](*this->resource); },
        "mil (1 -> 2)", equivLand);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::upgrademilitaryFactory21(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;

  this->lg3.lock();
  double equivLand = this->building->militaryFactoryL[2] - this->building->militaryFactoryL[1];
  if ((this->resource->manpower - this->resource->manpowerInUse <= 0) || (this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand < equivLand || !this->research->productionLine[2] || this->building->militaryFactory[1] < 1))
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->building->militaryFactory[1]--;
  this->lg3.unlock();
  this->buildBase(
      "militaryFactory", this->building->militaryFactoryT[2],
      [&]() {this->building->effect["militaryFactory"][2](*this->resource); this->building->reverseEffect["militaryFactory"][1](*this->resource); },
      "mil (2 -> 3)", equivLand);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::upgrademilitaryFactory25(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;

  this->lg3.lock();
  double equivLand = this->building->militaryFactoryL[2] - this->building->militaryFactoryL[1];
  if ((this->resource->manpower - this->resource->manpowerInUse <= 4) || (this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand < equivLand * 5 || !this->research->productionLine[2] || this->building->militaryFactory[1] < 5))
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->building->militaryFactory[1] -= 5;
  this->lg3.unlock();
  for (int i = 0; i < 5; i++)
    this->buildBase(
        "militaryFactory", this->building->militaryFactoryT[2],
        [&]() {this->building->effect["militaryFactory"][2](*this->resource); this->building->reverseEffect["militaryFactory"][1](*this->resource); },
        "mil (2 -> 3)", equivLand);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::upgrademilitaryFactory210(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;

  this->lg3.lock();
  double equivLand = this->building->militaryFactoryL[2] - this->building->militaryFactoryL[1];
  if ((this->resource->manpower - this->resource->manpowerInUse <= 9) || (this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand < equivLand * 10 || !this->research->productionLine[2] || this->building->militaryFactory[1] < 10))
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->building->militaryFactory[1] -= 10;
  this->lg3.unlock();
  for (int i = 0; i < 10; i++)
    this->buildBase(
        "militaryFactory", this->building->militaryFactoryT[2],
        [&]() {this->building->effect["militaryFactory"][2](*this->resource); this->building->reverseEffect["militaryFactory"][1](*this->resource); },
        "mil (2 -> 3)", equivLand);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::upgrademilitaryFactory2max(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;

  this->lg3.lock();
  double equivLand = this->building->militaryFactoryL[2] - this->building->militaryFactoryL[1];
  double freeLand = this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand;
  int max = equivLand == 0 ? std::min(this->building->militaryFactory[1], this->resource->manpower - this->resource->manpowerInUse) : std::min(std::min((int)(freeLand / equivLand), this->resource->manpower - this->resource->manpowerInUse), this->building->militaryFactory[1]);
  if (max == 0 || !this->research->productionLine[2])
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintBuild();
    this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->building->militaryFactory[1] -= max;
  this->lg3.unlock();
  for (int i = 0; i < max; i++)
    this->buildBase(
        "militaryFactory", this->building->militaryFactoryT[2],
        [&]() {this->building->effect["militaryFactory"][2](*this->resource); this->building->reverseEffect["militaryFactory"][1](*this->resource); },
        "mil (2 -> 3)", equivLand);
  this->lg2.lock();
  this->stopLoopPrintBuild();
  this->loopPrintBuild(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}