#include <string>
#include <tuple>

#include "../game.h"

void Game::researchFarm(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;

  this->lg3.lock();
  if (std::get<1>(this->research->progressTrack) != "")
  {
    this->lg2.lock();

    this->stopLoopPrintResearch();

    this->loopPrintResearch(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);

    this->lg2.unlock();

    this->lg3.unlock();
    return;
  }

  this->lg3.unlock();

  if (!this->research->farm[1])
    this->researchBase("Farm (intermediate)", this->research->effect["farm"][0], this->research->farmT[0]);


  else if (!this->research->farm[2])
    this->researchBase("Farm (advanced)", this->research->effect["farm"][1], this->research->farmT[1]);
  this->lg2.lock();
  this->stopLoopPrintResearch();
  this->loopPrintResearch(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}









void Game::researchDivisionOfLabor(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;

  this->lg3.lock();

  if (std::get<1>(this->research->progressTrack) != "")
  {
    this->lg2.lock();
    this->stopLoopPrintResearch();
    this->loopPrintResearch(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    this->lg3.unlock();
    return;
  }

  this->lg3.unlock();

  if (!this->research->divisionOfLabor[1])
    this->researchBase("Division of Labor (intermediate)", this->research->effect["divisionOfLabor"][0], this->research->divisionOfLaborT[0]);

  else if (!this->research->divisionOfLabor[2])
    this->researchBase("Division of Labor (advanced)", this->research->effect["divisionOfLabor"][1], this->research->divisionOfLaborT[1]);
  this->lg2.lock();
  this->stopLoopPrintResearch();
  this->loopPrintResearch(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}












void Game::researchProductionLine(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;

  this->lg3.lock();

  if (std::get<1>(this->research->progressTrack) != "")
  {
    this->lg2.lock();
    this->stopLoopPrintResearch();
    this->loopPrintResearch(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    this->lg3.unlock();
    return;
  }

  this->lg3.unlock();

  if (!this->research->productionLine[1])
    this->researchBase("Production Line (intermediate)", this->research->effect["productionLine"][0], this->research->productionLineT[0]);

  else if (!this->research->productionLine[2])
    this->researchBase("Production Line (advanced)", this->research->effect["productionLine"][1], this->research->productionLineT[1]);

  this->lg2.lock();

  this->stopLoopPrintResearch();

  this->loopPrintResearch(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);

  this->lg2.unlock();
}













void Game::researchLandDoctrine(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;

  this->lg3.lock();

  if (std::get<1>(this->research->progressTrack) != "")
  {

    this->lg2.lock();

    this->stopLoopPrintResearch();

    this->loopPrintResearch(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);

    this->lg2.unlock();

    this->lg3.unlock();

    return;
  }

  this->lg3.unlock();

  if (!this->research->landDoctrine[1])
    this->researchBase("Land Doctrine (intermediate)", this->research->effect["landDoctrine"][0], this->research->landDoctrineT[0]);

  else if (!this->research->landDoctrine[2])
    this->researchBase("Land Doctrine (advanced)", this->research->effect["landDoctrine"][1], this->research->landDoctrineT[1]);
  this->lg2.lock();
  this->stopLoopPrintResearch();
  this->loopPrintResearch(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}












void Game::researchAirDoctrine(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;

  this->lg3.lock();

  if (std::get<1>(this->research->progressTrack) != "")
  {
    this->lg2.lock();
    this->stopLoopPrintResearch();
    this->loopPrintResearch(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    this->lg3.unlock();
    return;
  }

  this->lg3.unlock();

  if (!this->research->airDoctrine[1])
    this->researchBase("Air Doctrine (intermediate)", this->research->effect["airDoctrine"][0], this->research->airDoctrineT[0]);

  else if (!this->research->airDoctrine[2])
    this->researchBase("Air Doctrine (advanced)", this->research->effect["airDoctrine"][1], this->research->airDoctrineT[1]);
  this->lg2.lock();
  this->stopLoopPrintResearch();
  this->loopPrintResearch(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}










void Game::researchUrbanization(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;

  this->lg3.lock();

  if (std::get<1>(this->research->progressTrack) != "")
  {
    this->lg2.lock();
    this->stopLoopPrintResearch();
    this->loopPrintResearch(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    this->lg3.unlock();
    return;
  }

  this->lg3.unlock();

  if (!this->research->urbanization[1])
    this->researchBase("Urbanization (intermediate)", this->research->effect["urbanization"][0], this->research->urbanizationT[0]);

  else if (!this->research->urbanization[2])
    this->researchBase("Urbanization (advanced)", this->research->effect["urbanization"][1], this->research->urbanizationT[1]);
  this->lg2.lock();
  this->stopLoopPrintResearch();
  this->loopPrintResearch(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}












void Game::researchWeapon(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;

  this->lg3.lock();

  if (std::get<1>(this->research->progressTrack) != "")
  {
    this->lg2.lock();
    this->stopLoopPrintResearch();
    this->loopPrintResearch(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    this->lg3.unlock();
    return;
  }

  this->lg3.unlock();

  if (!this->research->weapon[1])
    this->researchBase("Weapon (intermediate)", this->research->effect["weapon"][0], this->research->weaponT[0]);

  else if (!this->research->weapon[2])
    this->researchBase("Weapon (advanced)", this->research->effect["weapon"][1], this->research->weaponT[1]);
  this->lg2.lock();
  this->stopLoopPrintResearch();
  this->loopPrintResearch(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}












void Game::researchTraining(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;

  this->lg3.lock();

  if (std::get<1>(this->research->progressTrack) != "")
  {
    this->lg2.lock();
    this->stopLoopPrintResearch();
    this->loopPrintResearch(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    this->lg3.unlock();
    return;
  }

  this->lg3.unlock();

  if (!this->research->training[1])
    this->researchBase("Training (intermediate)", this->research->effect["training"][0], this->research->trainingT[0]);

  else if (!this->research->training[2])
    this->researchBase("Training (advanced)", this->research->effect["training"][1], this->research->trainingT[1]);
  this->lg2.lock();
  this->stopLoopPrintResearch();
  this->loopPrintResearch(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}












void Game::researchRecovery(int &currentPhase, int prevPhase)
{
  currentPhase = prevPhase;

  this->lg3.lock();
  if (std::get<1>(this->research->progressTrack) != "")
  {
    this->lg2.lock();
    this->stopLoopPrintResearch();
    this->loopPrintResearch(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    this->lg3.unlock();
    return;
  }

  this->lg3.unlock();

  if (!this->research->recovery[1])
    this->researchBase("Recovery (intermediate)", this->research->effect["recovery"][0], this->research->recoveryT[0]);

  else if (!this->research->recovery[2])
    this->researchBase("Recovery (advanced)", this->research->effect["recovery"][1], this->research->recoveryT[1]);
  this->lg2.lock();
  this->stopLoopPrintResearch();
  this->loopPrintResearch(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}


















