#include <string>
#include <cassert>

#include "../game.h"
#include "../../../data/troop/troop.h"
#include <iostream>

// note since troops that are not free cannot be deleted
// and also all free troops of the same type should have the same food/ equipment
// so we can just loop through the vector and remove any same type of troop

void Game::removeInfantry(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();

  if (this->troop->infantry[0] > 0)
  {

    this->troop->infantry[0]--;
    this->troop->totalTroops--;

    int index = -1;

    for (int i = 0; i < this->troop->allTroop.size(); i++)
    {

      if (this->troop->allTroop[i]->type == "infantry" && !this->troop->allTroop[i]->isReferenced)
      {
        index = i;
        break;
      }
    }

    assert(index != -1);

    this->troop->totalEquipmentRequired -= this->troop->allTroop[index]->getEquipment();
    this->troop->totalFoodRequired -= this->troop->allTroop[index]->getFood();

    delete this->troop->allTroop[index];
    this->troop->allTroop.erase(this->troop->allTroop.begin() + index);
  }
  this->lg3.unlock();

  this->lg2.lock();

  this->stopLoopPrintTroop();

  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);

  this->lg2.unlock();
}




void Game::removeCalvary(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();

  if (this->troop->calvary[0] > 0)
  {

    this->troop->calvary[0]--;

    this->troop->totalTroops--;

    int index = -1;



    for (int i = 0; i < this->troop->allTroop.size(); i++)
    {
      if (this->troop->allTroop[i]->type == "calvary" && !this->troop->allTroop[i]->isReferenced)
      {
        index = i;
        break;
      }
    }
    assert(index != -1);

    this->troop->totalEquipmentRequired -= this->troop->allTroop[index]->getEquipment();

    this->troop->totalFoodRequired -= this->troop->allTroop[index]->getFood();

    delete this->troop->allTroop[index];

    this->troop->allTroop.erase(this->troop->allTroop.begin() + index);
  }






  this->lg3.unlock();
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}













void Game::removeSuicideBomber(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();

  if (this->troop->suicideBomber[0] > 0)
  {

    this->troop->suicideBomber[0]--;
    this->troop->totalTroops--;

    int index = -1;
    for (int i = 0; i < this->troop->allTroop.size(); i++)
    {
      if (this->troop->allTroop[i]->type == "suicideBomber" && !this->troop->allTroop[i]->isReferenced)
      {
        index = i;
        break;
      }
    }
    assert(index != -1);

    this->troop->totalEquipmentRequired -= this->troop->allTroop[index]->getEquipment();
    this->troop->totalFoodRequired -= this->troop->allTroop[index]->getFood();

    delete this->troop->allTroop[index];
    this->troop->allTroop.erase(this->troop->allTroop.begin() + index);
  }

  this->lg3.unlock();

  this->lg2.lock();

  this->stopLoopPrintTroop();

  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);

  this->lg2.unlock();
}













void Game::removeLogistic(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();

  if (this->troop->logistic[0] > 0)
  {

    this->troop->logistic[0]--;
    this->troop->totalTroops--;

    int index = -1;
    for (int i = 0; i < this->troop->allTroop.size(); i++)
    {
      if (this->troop->allTroop[i]->type == "logistic" && !this->troop->allTroop[i]->isReferenced)
      {
        index = i;
        break;
      }
    }
    assert(index != -1);

    this->troop->totalEquipmentRequired -= this->troop->allTroop[index]->getEquipment();
    this->troop->totalFoodRequired -= this->troop->allTroop[index]->getFood();

    delete this->troop->allTroop[index];
    this->troop->allTroop.erase(this->troop->allTroop.begin() + index);

  }

  this->lg3.unlock();

  this->lg2.lock();

  this->stopLoopPrintTroop();

  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);

  this->lg2.unlock();
}















void Game::removeArtillery(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();

  if (this->troop->artillery[0] > 0)
  {

    this->troop->artillery[0]--;
    this->troop->totalTroops--;

    int index = -1;
    for (int i = 0; i < this->troop->allTroop.size(); i++)
    {
      if (this->troop->allTroop[i]->type == "artillery" && !this->troop->allTroop[i]->isReferenced)
      {
        index = i;
        break;
      }
    }
    assert(index != -1);

    this->troop->totalEquipmentRequired -= this->troop->allTroop[index]->getEquipment();
    this->troop->totalFoodRequired -= this->troop->allTroop[index]->getFood();

    delete this->troop->allTroop[index];
    this->troop->allTroop.erase(this->troop->allTroop.begin() + index);
  }

  this->lg3.unlock();

  this->lg2.lock();

  this->stopLoopPrintTroop();

  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);

  this->lg2.unlock();
}















void Game::removeArmoredCar(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();

  if (this->troop->armoredCar[0] > 0)
  {

    this->troop->armoredCar[0]--;
    this->troop->totalTroops--;

    int index = -1;
    for (int i = 0; i < this->troop->allTroop.size(); i++)
    {
      if (this->troop->allTroop[i]->type == "armoredCar" && !this->troop->allTroop[i]->isReferenced)
      {
        index = i;
        break;
      }
    }

    assert(index != -1);

    this->troop->totalEquipmentRequired -= this->troop->allTroop[index]->getEquipment();
    this->troop->totalFoodRequired -= this->troop->allTroop[index]->getFood();

    delete this->troop->allTroop[index];
    this->troop->allTroop.erase(this->troop->allTroop.begin() + index);
  }

  this->lg3.unlock();

  this->lg2.lock();

  this->stopLoopPrintTroop();

  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);

  this->lg2.unlock();
}















void Game::removeTank1(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();

  if (this->troop->tank1[0] > 0)
  {

    this->troop->tank1[0]--;
    this->troop->totalTroops--;

    int index = -1;
    for (int i = 0; i < this->troop->allTroop.size(); i++)
    {
      if (this->troop->allTroop[i]->type == "tank1" && !this->troop->allTroop[i]->isReferenced)
      {
        index = i;
        break;
      }
    }

    assert(index != -1);

    this->troop->totalEquipmentRequired -= this->troop->allTroop[index]->getEquipment();
    this->troop->totalFoodRequired -= this->troop->allTroop[index]->getFood();

    delete this->troop->allTroop[index];
    this->troop->allTroop.erase(this->troop->allTroop.begin() + index);
  }

  this->lg3.unlock();

  this->lg2.lock();

  this->stopLoopPrintTroop();

  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);

  this->lg2.unlock();
}

















void Game::removeTank2(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();

  if (this->troop->tank2[0] > 0)
  {

    this->troop->tank2[0]--;
    this->troop->totalTroops--;

    int index = -1;
    for (int i = 0; i < this->troop->allTroop.size(); i++)
    {
      if (this->troop->allTroop[i]->type == "tank2" && !this->troop->allTroop[i]->isReferenced)
      {
        index = i;
        break;
      }
    }
    assert(index != -1);

    this->troop->totalEquipmentRequired -= this->troop->allTroop[index]->getEquipment();
    this->troop->totalFoodRequired -= this->troop->allTroop[index]->getFood();

    delete this->troop->allTroop[index];
    this->troop->allTroop.erase(this->troop->allTroop.begin() + index);
  }

  this->lg3.unlock();

  this->lg2.lock();

  this->stopLoopPrintTroop();

  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);

  this->lg2.unlock();
}














void Game::removeTankOshimai(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();

  if (this->troop->tankOshimai[0] > 0)
  {

    this->troop->tankOshimai[0]--;
    this->troop->totalTroops--;

    int index = -1;
    for (int i = 0; i < this->troop->allTroop.size(); i++)
    {
      if (this->troop->allTroop[i]->type == "tankOshimai" && !this->troop->allTroop[i]->isReferenced)
      {
        index = i;
        break;
      }
    }

    assert(index != -1);

    this->troop->totalEquipmentRequired -= this->troop->allTroop[index]->getEquipment();
    this->troop->totalFoodRequired -= this->troop->allTroop[index]->getFood();

    delete this->troop->allTroop[index];
    this->troop->allTroop.erase(this->troop->allTroop.begin() + index);
  }

  this->lg3.unlock();

  this->lg2.lock();

  this->stopLoopPrintTroop();

  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);

  this->lg2.unlock();
}















void Game::removeCas(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();

  if (this->troop->cas[0] > 0)
  {

    this->troop->cas[0]--;
    this->troop->totalTroops--;
    this->resource->airportUsed--;

    int index = -1;
    for (int i = 0; i < this->troop->allTroop.size(); i++)
    {
      if (this->troop->allTroop[i]->type == "cas" && !this->troop->allTroop[i]->isReferenced)
      {
        index = i;
        break;
      }
    }
    assert(index != -1);

    this->troop->totalEquipmentRequired -= this->troop->allTroop[index]->getEquipment();
    this->troop->totalFoodRequired -= this->troop->allTroop[index]->getFood();

    delete this->troop->allTroop[index];
    this->troop->allTroop.erase(this->troop->allTroop.begin() + index);
  }

  this->lg3.unlock();

  this->lg2.lock();

  this->stopLoopPrintTroop();

  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);

  this->lg2.unlock();
}











void Game::removeFighter(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();

  if (this->troop->fighter[0] > 0)
  {

    this->troop->fighter[0]--;
    this->troop->totalTroops--;
    this->resource->airportUsed--;

    int index = -1;

    for (int i = 0; i < this->troop->allTroop.size(); i++)
    {
      if (this->troop->allTroop[i]->type == "fighter" && !this->troop->allTroop[i]->isReferenced)
      {
        index = i;
        break;
      }
    }

    assert(index != -1);

    this->troop->totalEquipmentRequired -= this->troop->allTroop[index]->getEquipment();
    this->troop->totalFoodRequired -= this->troop->allTroop[index]->getFood();

    delete this->troop->allTroop[index];
    this->troop->allTroop.erase(this->troop->allTroop.begin() + index);
  }

  this->lg3.unlock();

  this->lg2.lock();

  this->stopLoopPrintTroop();

  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);

  this->lg2.unlock();
}













void Game::removeBomber(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();

  if (this->troop->bomber[0] > 0)
  {

    this->troop->bomber[0]--;
    this->troop->totalTroops--;
    this->resource->airportUsed--;

    int index = -1;
    for (int i = 0; i < this->troop->allTroop.size(); i++)
    {
      if (this->troop->allTroop[i]->type == "bomber" && !this->troop->allTroop[i]->isReferenced)
      {
        index = i;
        break;
      }
    }
    assert(index != -1);

    this->troop->totalEquipmentRequired -= this->troop->allTroop[index]->getEquipment();
    this->troop->totalFoodRequired -= this->troop->allTroop[index]->getFood();

    delete this->troop->allTroop[index];
    this->troop->allTroop.erase(this->troop->allTroop.begin() + index);
  }

  this->lg3.unlock();

  this->lg2.lock();

  this->stopLoopPrintTroop();

  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);

  this->lg2.unlock();

}














void Game::removeKamikaze(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();

  if (this->troop->kamikaze[0] > 0)
  {

    this->troop->kamikaze[0]--;
    this->troop->totalTroops--;
    this->resource->airportUsed--;

    int index = -1;

    for (int i = 0; i < this->troop->allTroop.size(); i++)
    {
      if (this->troop->allTroop[i]->type == "kamikaze" && !this->troop->allTroop[i]->isReferenced)
      {
        index = i;
        break;
      }
    }

    assert(index != -1);

    this->troop->totalEquipmentRequired -= this->troop->allTroop[index]->getEquipment();
    this->troop->totalFoodRequired -= this->troop->allTroop[index]->getFood();

    delete this->troop->allTroop[index];
    this->troop->allTroop.erase(this->troop->allTroop.begin() + index);
  }

  this->lg3.unlock();

  this->lg2.lock();

  this->stopLoopPrintTroop();

  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);

  this->lg2.unlock();
}














void Game::removeInfantrymax(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();

  if (this->troop->infantry[0] > 0)
  {

    std::vector<int> indexStore;

    for (int i = 0; i < this->troop->allTroop.size(); i++)
    {
      if (this->troop->allTroop[i]->type == "infantry" && !this->troop->allTroop[i]->isReferenced)
      {
        indexStore.push_back(i);
      }
    }

    assert(indexStore.size() == this->troop->infantry[0]);

    this->troop->totalTroops -= this->troop->infantry[0];
    this->troop->infantry[0] = 0;

    for (int i = this->troop->allTroop.size() - 1; i >= 0; i--)
    {
      if (this->troop->allTroop[i]->type == "infantry" && !this->troop->allTroop[i]->isReferenced)
      {
        this->troop->totalEquipmentRequired -= this->troop->allTroop[i]->getEquipment();
        this->troop->totalFoodRequired -= this->troop->allTroop[i]->getFood();

        delete this->troop->allTroop[i];
        this->troop->allTroop.erase(this->troop->allTroop.begin() + i);
      }
    }
  }

  this->lg3.unlock();

  this->lg2.lock();

  this->stopLoopPrintTroop();

  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);

  this->lg2.unlock();
}













void Game::removeCalvarymax(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();

  if (this->troop->calvary[0] > 0)
  {

    std::vector<int> indexStore;

    for (int i = 0; i < this->troop->allTroop.size(); i++)
    {
      if (this->troop->allTroop[i]->type == "calvary" && !this->troop->allTroop[i]->isReferenced)
      {
        indexStore.push_back(i);
      }
    }
    assert(indexStore.size() == this->troop->calvary[0]);

    this->troop->totalTroops -= this->troop->calvary[0];

    this->troop->calvary[0] = 0;

    for (int i = this->troop->allTroop.size() - 1; i >= 0; i--)
    {
      if (this->troop->allTroop[i]->type == "calvary" && !this->troop->allTroop[i]->isReferenced)
      {
        this->troop->totalEquipmentRequired -= this->troop->allTroop[i]->getEquipment();
        this->troop->totalFoodRequired -= this->troop->allTroop[i]->getFood();

        delete this->troop->allTroop[i];
        this->troop->allTroop.erase(this->troop->allTroop.begin() + i);
      }
    }
  }

  this->lg3.unlock();

  this->lg2.lock();

  this->stopLoopPrintTroop();

  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);

  this->lg2.unlock();
}














void Game::removeSuicideBombermax(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();

  if (this->troop->suicideBomber[0] > 0)
  {

    std::vector<int> indexStore;

    for (int i = 0; i < this->troop->allTroop.size(); i++)
    {
      if (this->troop->allTroop[i]->type == "suicideBomber" && !this->troop->allTroop[i]->isReferenced)
      {
        indexStore.push_back(i);
      }
    }
    assert(indexStore.size() == this->troop->suicideBomber[0]);

    this->troop->totalTroops -= this->troop->suicideBomber[0];
    this->troop->suicideBomber[0] = 0;

    for (int i = this->troop->allTroop.size() - 1; i >= 0; i--)
    {
      if (this->troop->allTroop[i]->type == "suicideBomber" && !this->troop->allTroop[i]->isReferenced)
      {
        this->troop->totalEquipmentRequired -= this->troop->allTroop[i]->getEquipment();
        this->troop->totalFoodRequired -= this->troop->allTroop[i]->getFood();

        delete this->troop->allTroop[i];
        this->troop->allTroop.erase(this->troop->allTroop.begin() + i);
      }
    }
  }

  this->lg3.unlock();

  this->lg2.lock();

  this->stopLoopPrintTroop();

  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);

  this->lg2.unlock();
}














void Game::removeLogisticmax(int &gamePhase, int prevGamePhase)
{

  gamePhase = prevGamePhase;

  this->lg3.lock();

  if (this->troop->logistic[0] > 0)
  {

    std::vector<int> indexStore;

    for (int i = 0; i < this->troop->allTroop.size(); i++)
    {
      if (this->troop->allTroop[i]->type == "logistic" && !this->troop->allTroop[i]->isReferenced)
      {
        indexStore.push_back(i);
      }
    }

    assert(indexStore.size() == this->troop->logistic[0]);

    this->troop->totalTroops -= this->troop->logistic[0];
    this->troop->logistic[0] = 0;

    for (int i = this->troop->allTroop.size() - 1; i >= 0; i--)
    {
      if (this->troop->allTroop[i]->type == "logistic" && !this->troop->allTroop[i]->isReferenced)
      {
        this->troop->totalEquipmentRequired -= this->troop->allTroop[i]->getEquipment();
        this->troop->totalFoodRequired -= this->troop->allTroop[i]->getFood();

        delete this->troop->allTroop[i];
        this->troop->allTroop.erase(this->troop->allTroop.begin() + i);
      }
    }
  }

  this->lg3.unlock();

  this->lg2.lock();

  this->stopLoopPrintTroop();

  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);

  this->lg2.unlock();

}















void Game::removeArtillerymax(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();

  if (this->troop->artillery[0] > 0)
  {

    std::vector<int> indexStore;

    for (int i = 0; i < this->troop->allTroop.size(); i++)
    {
      if (this->troop->allTroop[i]->type == "artillery" && !this->troop->allTroop[i]->isReferenced)
      {
        indexStore.push_back(i);
      }
    }

    assert(indexStore.size() == this->troop->artillery[0]);

    this->troop->totalTroops -= this->troop->artillery[0];
    this->troop->artillery[0] = 0;

    for (int i = this->troop->allTroop.size() - 1; i >= 0; i--)
    {
      if (this->troop->allTroop[i]->type == "artillery" && !this->troop->allTroop[i]->isReferenced)
      {
        this->troop->totalEquipmentRequired -= this->troop->allTroop[i]->getEquipment();
        this->troop->totalFoodRequired -= this->troop->allTroop[i]->getFood();

        delete this->troop->allTroop[i];
        this->troop->allTroop.erase(this->troop->allTroop.begin() + i);
      }
    }
  }

  this->lg3.unlock();

  this->lg2.lock();

  this->stopLoopPrintTroop();

  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);

  this->lg2.unlock();
}













void Game::removeArmoredCarmax(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();

  if (this->troop->armoredCar[0] > 0)
  {

    std::vector<int> indexStore;

    for (int i = 0; i < this->troop->allTroop.size(); i++)
    {
      if (this->troop->allTroop[i]->type == "armoredCar" && !this->troop->allTroop[i]->isReferenced)
      {
        indexStore.push_back(i);
      }
    }

    assert(indexStore.size() == this->troop->armoredCar[0]);

    this->troop->totalTroops -= this->troop->armoredCar[0];
    this->troop->armoredCar[0] = 0;

    for (int i = this->troop->allTroop.size() - 1; i >= 0; i--)
    {
      if (this->troop->allTroop[i]->type == "armoredCar" && !this->troop->allTroop[i]->isReferenced)
      {
        this->troop->totalEquipmentRequired -= this->troop->allTroop[i]->getEquipment();
        this->troop->totalFoodRequired -= this->troop->allTroop[i]->getFood();

        delete this->troop->allTroop[i];
        this->troop->allTroop.erase(this->troop->allTroop.begin() + i);
      }
    }
  }

  this->lg3.unlock();

  this->lg2.lock();

  this->stopLoopPrintTroop();

  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);

  this->lg2.unlock();
}












void Game::removeTank1max(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();

  if (this->troop->tank1[0] > 0)
  {

    std::vector<int> indexStore;

    for (int i = 0; i < this->troop->allTroop.size(); i++)
    {
      if (this->troop->allTroop[i]->type == "tank1" && !this->troop->allTroop[i]->isReferenced)
      {
        indexStore.push_back(i);
      }
    }

    assert(indexStore.size() == this->troop->tank1[0]);

    this->troop->totalTroops -= this->troop->tank1[0];
    this->troop->tank1[0] = 0;

    for (int i = this->troop->allTroop.size() - 1; i >= 0; i--)
    {
      if (this->troop->allTroop[i]->type == "tank1" && !this->troop->allTroop[i]->isReferenced)
      {
        this->troop->totalEquipmentRequired -= this->troop->allTroop[i]->getEquipment();
        this->troop->totalFoodRequired -= this->troop->allTroop[i]->getFood();

        delete this->troop->allTroop[i];
        this->troop->allTroop.erase(this->troop->allTroop.begin() + i);
      }
    }
  }

  this->lg3.unlock();

  this->lg2.lock();

  this->stopLoopPrintTroop();

  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);

  this->lg2.unlock();
}













void Game::removeTank2max(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();

  if (this->troop->tank2[0] > 0)
  {

    std::vector<int> indexStore;

    for (int i = 0; i < this->troop->allTroop.size(); i++)
    {
      if (this->troop->allTroop[i]->type == "tank2" && !this->troop->allTroop[i]->isReferenced)
      {
        indexStore.push_back(i);
      }
    }

    assert(indexStore.size() == this->troop->tank2[0]);

    this->troop->totalTroops -= this->troop->tank2[0];
    this->troop->tank2[0] = 0;

    for (int i = this->troop->allTroop.size() - 1; i >= 0; i--)
    {
      if (this->troop->allTroop[i]->type == "tank2" && !this->troop->allTroop[i]->isReferenced)
      {
        this->troop->totalEquipmentRequired -= this->troop->allTroop[i]->getEquipment();
        this->troop->totalFoodRequired -= this->troop->allTroop[i]->getFood();

        delete this->troop->allTroop[i];
        this->troop->allTroop.erase(this->troop->allTroop.begin() + i);
      }
    }
  }

  this->lg3.unlock();

  this->lg2.lock();

  this->stopLoopPrintTroop();

  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);

  this->lg2.unlock();
}















void Game::removeTankOshimaimax(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();

  if (this->troop->tankOshimai[0] > 0)
  {

    std::vector<int> indexStore;

    for (int i = 0; i < this->troop->allTroop.size(); i++)
    {
      if (this->troop->allTroop[i]->type == "tankOshimai" && !this->troop->allTroop[i]->isReferenced)
      {
        indexStore.push_back(i);
      }
    }

    assert(indexStore.size() == this->troop->tankOshimai[0]);

    this->troop->totalTroops -= this->troop->tankOshimai[0];
    this->troop->tankOshimai[0] = 0;

    for (int i = this->troop->allTroop.size() - 1; i >= 0; i--)
    {
      if (this->troop->allTroop[i]->type == "tankOshimai" && !this->troop->allTroop[i]->isReferenced)
      {
        this->troop->totalEquipmentRequired -= this->troop->allTroop[i]->getEquipment();
        this->troop->totalFoodRequired -= this->troop->allTroop[i]->getFood();

        delete this->troop->allTroop[i];
        this->troop->allTroop.erase(this->troop->allTroop.begin() + i);
      }
    }
  }

  this->lg3.unlock();

  this->lg2.lock();

  this->stopLoopPrintTroop();

  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);

  this->lg2.unlock();
}















void Game::removeCasmax(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();

  if (this->troop->cas[0] > 0)
  {

    std::vector<int> indexStore;

    for (int i = 0; i < this->troop->allTroop.size(); i++)
    {
      if (this->troop->allTroop[i]->type == "cas" && !this->troop->allTroop[i]->isReferenced)
      {
        indexStore.push_back(i);
      }
    }

    assert(indexStore.size() == this->troop->cas[0]);

    this->troop->totalTroops -= this->troop->cas[0];
    this->troop->cas[0] = 0;

    for (int i = this->troop->allTroop.size() - 1; i >= 0; i--)
    {
      if (this->troop->allTroop[i]->type == "cas" && !this->troop->allTroop[i]->isReferenced)
      {
        this->troop->totalEquipmentRequired -= this->troop->allTroop[i]->getEquipment();
        this->troop->totalFoodRequired -= this->troop->allTroop[i]->getFood();

        delete this->troop->allTroop[i];
        this->troop->allTroop.erase(this->troop->allTroop.begin() + i);
      }
    }
  }

  this->lg3.unlock();

  this->lg2.lock();

  this->stopLoopPrintTroop();

  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);

  this->lg2.unlock();
}













void Game::removeFightermax(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();

  if (this->troop->fighter[0] > 0)
  {

    std::vector<int> indexStore;

    for (int i = 0; i < this->troop->allTroop.size(); i++)
    {
      if (this->troop->allTroop[i]->type == "fighter" && !this->troop->allTroop[i]->isReferenced)
      {
        indexStore.push_back(i);
      }
    }

    assert(indexStore.size() == this->troop->fighter[0]);

    this->troop->totalTroops -= this->troop->fighter[0];
    this->troop->fighter[0] = 0;

    for (int i = this->troop->allTroop.size() - 1; i >= 0; i--)
    {
      if (this->troop->allTroop[i]->type == "fighter" && !this->troop->allTroop[i]->isReferenced)
      {
        this->troop->totalEquipmentRequired -= this->troop->allTroop[i]->getEquipment();
        this->troop->totalFoodRequired -= this->troop->allTroop[i]->getFood();

        delete this->troop->allTroop[i];
        this->troop->allTroop.erase(this->troop->allTroop.begin() + i);
      }
    }
  }

  this->lg3.unlock();

  this->lg2.lock();

  this->stopLoopPrintTroop();

  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);

  this->lg2.unlock();
}














void Game::removeBombermax(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();

  if (this->troop->bomber[0] > 0)
  {

    std::vector<int> indexStore;

    for (int i = 0; i < this->troop->allTroop.size(); i++)
    {
      if (this->troop->allTroop[i]->type == "bomber" && !this->troop->allTroop[i]->isReferenced)
      {
        indexStore.push_back(i);
      }
    }

    assert(indexStore.size() == this->troop->bomber[0]);

    this->troop->totalTroops -= this->troop->bomber[0];
    this->troop->bomber[0] = 0;

    for (int i = this->troop->allTroop.size() - 1; i >= 0; i--)
    {
      if (this->troop->allTroop[i]->type == "bomber" && !this->troop->allTroop[i]->isReferenced)
      {
        this->troop->totalEquipmentRequired -= this->troop->allTroop[i]->getEquipment();
        this->troop->totalFoodRequired -= this->troop->allTroop[i]->getFood();

        delete this->troop->allTroop[i];
        this->troop->allTroop.erase(this->troop->allTroop.begin() + i);
      }
    }
  }

  this->lg3.unlock();

  this->lg2.lock();

  this->stopLoopPrintTroop();

  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);

  this->lg2.unlock();
}












void Game::removeKamikazemax(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();

  if (this->troop->kamikaze[0] > 0)
  {

    std::vector<int> indexStore;

    for (int i = 0; i < this->troop->allTroop.size(); i++)
    {
      if (this->troop->allTroop[i]->type == "kamikaze" && !this->troop->allTroop[i]->isReferenced)
      {
        indexStore.push_back(i);
      }
    }

    assert(indexStore.size() == this->troop->kamikaze[0]);

    this->troop->totalTroops -= this->troop->kamikaze[0];
    this->troop->kamikaze[0] = 0;

    for (int i = this->troop->allTroop.size() - 1; i >= 0; i--)
    {
      if (this->troop->allTroop[i]->type == "kamikaze" && !this->troop->allTroop[i]->isReferenced)
      {
        this->troop->totalEquipmentRequired -= this->troop->allTroop[i]->getEquipment();
        this->troop->totalFoodRequired -= this->troop->allTroop[i]->getFood();

        delete this->troop->allTroop[i];
        this->troop->allTroop.erase(this->troop->allTroop.begin() + i);
      }
    }
  }

  this->lg3.unlock();

  this->lg2.lock();

  this->stopLoopPrintTroop();

  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);

  this->lg2.unlock();
}
















