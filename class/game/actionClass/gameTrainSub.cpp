#include <string>
#include <unordered_map>
#include <vector>

#include "../game.h"
#include "../../../data/troop/troop.h"

void Game::trainInfantry(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  if (this->resource->camp - this->resource->campUsed < Infantry::trainingCamp)
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintTroop();
    this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->lg3.unlock();
  this->trainBase(
      "infantry", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new Infantry(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.infantry[0]++;
      },
      Infantry::trainingCamp, 0, Infantry::trainingTime, 1);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::trainCalvary(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  if (this->resource->camp - this->resource->campUsed < Calvary::trainingCamp)
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintTroop();
    this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->lg3.unlock();
  this->trainBase(
      "calvary", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new Calvary(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.calvary[0]++;
      },
      Calvary::trainingCamp, 0, Calvary::trainingTime, 1);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::trainSuicideBomber(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  if (this->resource->camp - this->resource->campUsed < SuicideBomber::trainingCamp)
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintTroop();
    this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->lg3.unlock();
  this->trainBase(
      "suicideBomber", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new SuicideBomber(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.suicideBomber[0]++;
      },
      SuicideBomber::trainingCamp, 0, SuicideBomber::trainingTime, 1);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::trainLogistic(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  if (this->resource->camp - this->resource->campUsed < Logistic::trainingCamp)
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintTroop();
    this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->lg3.unlock();
  this->trainBase(
      "logistic", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new Logistic(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.logistic[0]++;
      },
      Logistic::trainingCamp, 0, Logistic::trainingTime, 1);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::trainArtillery(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  if (this->resource->camp - this->resource->campUsed < Artillery::trainingCamp)
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintTroop();
    this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->lg3.unlock();
  this->trainBase(
      "artillery", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new Artillery(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.artillery[0]++;
      },
      Artillery::trainingCamp, 0, Artillery::trainingTime, 1);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::trainArmoredCar(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  if (this->resource->camp - this->resource->campUsed < ArmoredCar::trainingCamp)
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintTroop();
    this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->lg3.unlock();
  this->trainBase(
      "armoredCar", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new ArmoredCar(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.armoredCar[0]++;
      },
      ArmoredCar::trainingCamp, 0, ArmoredCar::trainingTime, 1);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::trainTank1(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  if (this->resource->camp - this->resource->campUsed < Tank1::trainingCamp)
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintTroop();
    this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->lg3.unlock();
  this->trainBase(
      "tank1", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new Tank1(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.tank1[0]++;
      },
      Tank1::trainingCamp, 0, Tank1::trainingTime, 1);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::trainTank2(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  if (this->resource->camp - this->resource->campUsed < Tank2::trainingCamp)
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintTroop();
    this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->lg3.unlock();
  this->trainBase(
      "tank2", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new Tank2(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.tank2[0]++;
      },
      Tank2::trainingCamp, 0, Tank2::trainingTime, 1);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::trainTankOshimai(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  if (this->resource->camp - this->resource->campUsed < TankOshimai::trainingCamp)
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintTroop();
    this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->lg3.unlock();
  this->trainBase(
      "tankOshimai", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new TankOshimai(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.tankOshimai[0]++;
      },
      TankOshimai::trainingCamp, 0, TankOshimai::trainingTime, 1);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::trainCas(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  if (this->resource->camp - this->resource->campUsed < Cas::trainingCamp || this->resource->airport - this->resource->airportUsed < 1)
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintTroop();
    this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->lg3.unlock();
  this->trainBase(
      "cas", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new Cas(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.cas[0]++;
      },
      Cas::trainingCamp, 1, Cas::trainingTime, 1);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::trainFighter(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  if (this->resource->camp - this->resource->campUsed < Fighter::trainingCamp || this->resource->airport - this->resource->airportUsed < 1)
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintTroop();
    this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->lg3.unlock();
  this->trainBase(
      "fighter", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new Fighter(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.fighter[0]++;
      },
      Fighter::trainingCamp, 1, Fighter::trainingTime, 1);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::trainBomber(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  if (this->resource->camp - this->resource->campUsed < Bomber::trainingCamp || this->resource->airport - this->resource->airportUsed < 1)
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintTroop();
    this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->lg3.unlock();
  this->trainBase(
      "bomber", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new Bomber(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.bomber[0]++;
      },
      Bomber::trainingCamp, 1, Bomber::trainingTime, 1);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::trainKamikaze(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  if (this->resource->camp - this->resource->campUsed < Kamikaze::trainingCamp || this->resource->airport - this->resource->airportUsed < 1)
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintTroop();
    this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->lg3.unlock();
  this->trainBase(
      "kamikaze", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new Kamikaze(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.kamikaze[0]++;
      },
      Kamikaze::trainingCamp, 1, Kamikaze::trainingTime, 1);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::trainInfantry5(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  if (this->resource->camp - this->resource->campUsed < Infantry::trainingCamp * 5)
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintTroop();
    this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->lg3.unlock();

  this->trainBase(
      "infantry", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new Infantry(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.infantry[0]++;
      },
      Infantry::trainingCamp, 0, Infantry::trainingTime, 5);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::trainCalvary5(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  if (this->resource->camp - this->resource->campUsed < Calvary::trainingCamp * 5)
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintTroop();
    this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->lg3.unlock();

  this->trainBase(
      "calvary", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new Calvary(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.calvary[0]++;
      },
      Calvary::trainingCamp, 0, Calvary::trainingTime, 5);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::trainSuicideBomber5(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  if (this->resource->camp - this->resource->campUsed < SuicideBomber::trainingCamp * 5)
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintTroop();
    this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->lg3.unlock();

  this->trainBase(
      "suicideBomber", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new SuicideBomber(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.suicideBomber[0]++;
      },
      SuicideBomber::trainingCamp, 0, SuicideBomber::trainingTime, 5);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::trainLogistic5(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  if (this->resource->camp - this->resource->campUsed < Logistic::trainingCamp * 5)
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintTroop();
    this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->lg3.unlock();

  this->trainBase(
      "logistic", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new Logistic(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.logistic[0]++;
      },
      Logistic::trainingCamp, 0, Logistic::trainingTime, 5);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::trainArtillery5(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  if (this->resource->camp - this->resource->campUsed < Artillery::trainingCamp * 5)
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintTroop();
    this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->lg3.unlock();

  this->trainBase(
      "artillery", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new Artillery(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.artillery[0]++;
      },
      Artillery::trainingCamp, 0, Artillery::trainingTime, 5);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::trainArmoredCar5(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  if (this->resource->camp - this->resource->campUsed < ArmoredCar::trainingCamp * 5)
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintTroop();
    this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->lg3.unlock();

  this->trainBase(
      "armoredCar", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new ArmoredCar(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.armoredCar[0]++;
      },
      ArmoredCar::trainingCamp, 0, ArmoredCar::trainingTime, 5);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::trainTank15(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  if (this->resource->camp - this->resource->campUsed < Tank1::trainingCamp * 5)
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintTroop();
    this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->lg3.unlock();

  this->trainBase(
      "tank1", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new Tank1(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.tank1[0]++;
      },
      Tank1::trainingCamp, 0, Tank1::trainingTime, 5);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::trainTank25(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  if (this->resource->camp - this->resource->campUsed < Tank2::trainingCamp * 5)
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintTroop();
    this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->lg3.unlock();

  this->trainBase(
      "tank2", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new Tank2(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.tank2[0]++;
      },
      Tank2::trainingCamp, 0, Tank2::trainingTime, 5);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::trainTankOshimai5(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  if (this->resource->camp - this->resource->campUsed < TankOshimai::trainingCamp * 5)
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintTroop();
    this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->lg3.unlock();

  this->trainBase(
      "tankOshimai", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new TankOshimai(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.tankOshimai[0]++;
      },
      TankOshimai::trainingCamp, 0, TankOshimai::trainingTime, 5);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::trainCas5(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  if (this->resource->camp - this->resource->campUsed < Cas::trainingCamp * 5 || this->resource->airport - this->resource->airportUsed < 5)
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintTroop();
    this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->lg3.unlock();

  this->trainBase(
      "cas", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new Cas(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.cas[0]++;
      },
      Cas::trainingCamp, 1, Cas::trainingTime, 5);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::trainFighter5(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  if (this->resource->camp - this->resource->campUsed < Fighter::trainingCamp * 5 || this->resource->airport - this->resource->airportUsed < 5)
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintTroop();
    this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->lg3.unlock();

  this->trainBase(
      "fighter", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new Fighter(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.fighter[0]++;
      },
      Fighter::trainingCamp, 1, Fighter::trainingTime, 5);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::trainBomber5(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  if (this->resource->camp - this->resource->campUsed < Bomber::trainingCamp * 5 || this->resource->airport - this->resource->airportUsed < 5)
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintTroop();
    this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->lg3.unlock();

  this->trainBase(
      "bomber", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new Bomber(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.bomber[0]++;
      },
      Bomber::trainingCamp, 1, Bomber::trainingTime, 5);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::trainKamikaze5(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  if (this->resource->camp - this->resource->campUsed < Kamikaze::trainingCamp * 5 || this->resource->airport - this->resource->airportUsed < 5)
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintTroop();
    this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->lg3.unlock();

  this->trainBase(
      "kamikaze", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new Kamikaze(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.kamikaze[0]++;
      },
      Kamikaze::trainingCamp, 1, Kamikaze::trainingTime, 5);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::trainInfantry10(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  if (this->resource->camp - this->resource->campUsed < Infantry::trainingCamp * 5)
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintTroop();
    this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->lg3.unlock();
  this->trainBase(
      "infantry", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new Infantry(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.infantry[0]++;
      },
      Infantry::trainingCamp, 0, Infantry::trainingTime, 10);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::trainCalvary10(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  if (this->resource->camp - this->resource->campUsed < Calvary::trainingCamp * 5)
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintTroop();
    this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->lg3.unlock();
  this->trainBase(
      "calvary", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new Calvary(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.calvary[0]++;
      },
      Calvary::trainingCamp, 0, Calvary::trainingTime, 10);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::trainSuicideBomber10(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  if (this->resource->camp - this->resource->campUsed < SuicideBomber::trainingCamp * 5)
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintTroop();
    this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->lg3.unlock();
  this->trainBase(
      "suicideBomber", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new SuicideBomber(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.suicideBomber[0]++;
      },
      SuicideBomber::trainingCamp, 0, SuicideBomber::trainingTime, 10);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::trainLogistic10(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  if (this->resource->camp - this->resource->campUsed < Logistic::trainingCamp * 5)
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintTroop();
    this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->lg3.unlock();
  this->trainBase(
      "logistic", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new Logistic(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.logistic[0]++;
      },
      Logistic::trainingCamp, 0, Logistic::trainingTime, 10);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::trainArtillery10(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  if (this->resource->camp - this->resource->campUsed < Artillery::trainingCamp * 5)
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintTroop();
    this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->lg3.unlock();
  this->trainBase(
      "artillery", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new Artillery(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.artillery[0]++;
      },
      Artillery::trainingCamp, 0, Artillery::trainingTime, 10);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::trainArmoredCar10(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  if (this->resource->camp - this->resource->campUsed < ArmoredCar::trainingCamp * 5)
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintTroop();
    this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->lg3.unlock();
  this->trainBase(
      "armoredCar", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new ArmoredCar(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.armoredCar[0]++;
      },
      ArmoredCar::trainingCamp, 0, ArmoredCar::trainingTime, 10);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::trainTank110(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  if (this->resource->camp - this->resource->campUsed < Tank1::trainingCamp * 5)
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintTroop();
    this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->lg3.unlock();
  this->trainBase(
      "tank1", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new Tank1(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.tank1[0]++;
      },
      Tank1::trainingCamp, 0, Tank1::trainingTime, 10);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::trainTank210(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  if (this->resource->camp - this->resource->campUsed < Tank2::trainingCamp * 5)
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintTroop();
    this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->lg3.unlock();
  this->trainBase(
      "tank2", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new Tank2(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.tank2[0]++;
      },
      Tank2::trainingCamp, 0, Tank2::trainingTime, 10);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::trainTankOshimai10(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  if (this->resource->camp - this->resource->campUsed < TankOshimai::trainingCamp * 5)
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintTroop();
    this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->lg3.unlock();
  this->trainBase(
      "tankOshimai", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new TankOshimai(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.tankOshimai[0]++;
      },
      TankOshimai::trainingCamp, 0, TankOshimai::trainingTime, 10);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::trainCas10(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  if (this->resource->camp - this->resource->campUsed < Cas::trainingCamp * 5 || this->resource->airport - this->resource->airportUsed < 10)
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintTroop();
    this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->lg3.unlock();
  this->trainBase(
      "cas", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new Cas(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.cas[0]++;
      },
      Cas::trainingCamp, 1, Cas::trainingTime, 10);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::trainFighter10(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  if (this->resource->camp - this->resource->campUsed < Fighter::trainingCamp * 5 || this->resource->airport - this->resource->airportUsed < 10)
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintTroop();
    this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->lg3.unlock();
  this->trainBase(
      "fighter", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new Fighter(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.fighter[0]++;
      },
      Fighter::trainingCamp, 1, Fighter::trainingTime, 10);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::trainBomber10(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  if (this->resource->camp - this->resource->campUsed < Bomber::trainingCamp * 5 || this->resource->airport - this->resource->airportUsed < 10)
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintTroop();
    this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->lg3.unlock();
  this->trainBase(
      "bomber", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new Bomber(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.bomber[0]++;
      },
      Bomber::trainingCamp, 1, Bomber::trainingTime, 10);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::trainKamikaze10(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  if (this->resource->camp - this->resource->campUsed < Kamikaze::trainingCamp * 5 || this->resource->airport - this->resource->airportUsed < 10)
  {
    this->lg3.unlock();
    this->lg2.lock();
    this->stopLoopPrintTroop();
    this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    this->lg2.unlock();
    return;
  }
  this->lg3.unlock();
  this->trainBase(
      "kamikaze", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new Kamikaze(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.kamikaze[0]++;
      },
      Kamikaze::trainingCamp, 1, Kamikaze::trainingTime, 10);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}

void Game::trainInfantrymax(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  int max = (this->resource->camp - this->resource->campUsed) / Infantry::trainingCamp;
  this->lg3.unlock();
  this->trainBase(
      "infantry", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new Infantry(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.infantry[0]++;
      },
      Infantry::trainingCamp, 0, Infantry::trainingTime, max);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}

void Game::trainCalvarymax(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  int max = (this->resource->camp - this->resource->campUsed) / Calvary::trainingCamp;
  this->lg3.unlock();
  this->trainBase(
      "calvary", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new Calvary(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.calvary[0]++;
      },
      Calvary::trainingCamp, 0, Calvary::trainingTime, max);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::trainSuicideBombermax(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  int max = (this->resource->camp - this->resource->campUsed) / SuicideBomber::trainingCamp;
  this->lg3.unlock();
  this->trainBase(
      "suicideBomber", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new SuicideBomber(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.suicideBomber[0]++;
      },
      SuicideBomber::trainingCamp, 0, SuicideBomber::trainingTime, max);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::trainLogisticmax(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  int max = (this->resource->camp - this->resource->campUsed) / Logistic::trainingCamp;
  this->lg3.unlock();
  this->trainBase(
      "logistic", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new Logistic(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.logistic[0]++;
      },
      Logistic::trainingCamp, 0, Logistic::trainingTime, max);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::trainArtillerymax(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  int max = (this->resource->camp - this->resource->campUsed) / Artillery::trainingCamp;
  this->lg3.unlock();
  this->trainBase(
      "artillery", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new Artillery(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.artillery[0]++;
      },
      Artillery::trainingCamp, 0, Artillery::trainingTime, max);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::trainArmoredCarmax(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  int max = (this->resource->camp - this->resource->campUsed) / ArmoredCar::trainingCamp;
  this->lg3.unlock();
  this->trainBase(
      "armoredCar", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new ArmoredCar(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.armoredCar[0]++;
      },
      ArmoredCar::trainingCamp, 0, ArmoredCar::trainingTime, max);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::trainTank1max(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  int max = (this->resource->camp - this->resource->campUsed) / Tank1::trainingCamp;
  this->lg3.unlock();
  this->trainBase(
      "tank1", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new Tank1(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.tank1[0]++;
      },
      Tank1::trainingCamp, 0, Tank1::trainingTime, max);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::trainTank2max(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  int max = (this->resource->camp - this->resource->campUsed) / Tank2::trainingCamp;
  this->lg3.unlock();
  this->trainBase(
      "tank2", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new Tank2(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.tank2[0]++;
      },
      Tank2::trainingCamp, 0, Tank2::trainingTime, max);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::trainTankOshimaimax(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  int max = (this->resource->camp - this->resource->campUsed) / TankOshimai::trainingCamp;
  this->lg3.unlock();
  this->trainBase(
      "tankOshimai", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new TankOshimai(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.tankOshimai[0]++;
      },
      TankOshimai::trainingCamp, 0, TankOshimai::trainingTime, max);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::trainCasmax(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  int max = std::min((this->resource->camp - this->resource->campUsed) / Cas::trainingCamp, this->resource->airport - this->resource->airportUsed);
  this->lg3.unlock();
  this->trainBase(
      "cas", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new Cas(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.cas[0]++;
      },
      Cas::trainingCamp, 1, Cas::trainingTime, max);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::trainFightermax(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  int max = std::min((this->resource->camp - this->resource->campUsed) / Fighter::trainingCamp, this->resource->airport - this->resource->airportUsed);
  this->lg3.unlock();
  this->trainBase(
      "fighter", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new Fighter(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.fighter[0]++;
      },
      Fighter::trainingCamp, 1, Fighter::trainingTime, max);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::trainBombermax(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  int max = std::min((this->resource->camp - this->resource->campUsed) / Bomber::trainingCamp, this->resource->airport - this->resource->airportUsed);
  this->lg3.unlock();
  this->trainBase(
      "bomber", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new Bomber(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.bomber[0]++;
      },
      Bomber::trainingCamp, 1, Bomber::trainingTime, max);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}
void Game::trainKamikazemax(int &gamePhase, int prevGamePhase)
{
  gamePhase = prevGamePhase;

  this->lg3.lock();
  int max = std::min((this->resource->camp - this->resource->campUsed) / Kamikaze::trainingCamp, this->resource->airport - this->resource->airportUsed);
  this->lg3.unlock();
  this->trainBase(
      "kamikaze", [this](data::Resource &resource, data::Troop &troop) {
        troop.allTroop.push_back(new Kamikaze(this->uuid()));
        troop.totalTroops++;
        troop.totalFoodRequired += troop.allTroop.back()->getFood();
        troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
        troop.kamikaze[0]++;
      },
      Kamikaze::trainingCamp, 1, Kamikaze::trainingTime, max);
  this->lg2.lock();
  this->stopLoopPrintTroop();
  this->loopPrintTroop(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  this->lg2.unlock();
}