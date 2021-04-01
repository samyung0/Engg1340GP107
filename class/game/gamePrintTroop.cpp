#include <iostream>
#include <string>
#include <iomanip>
#include <vector>

#include "../../io/io.h"
#include "game.h"
#include "../../data/troop/troop.h"

void Game::printTroop(int x, int y)
{
  this->lg3.lock();

  int freeCamp = this->resource->camp - this->resource->campUsed;
  std::vector<int> maxTrain = {
      freeCamp / Infantry::trainingCamp,
      freeCamp / Calvary::trainingCamp,
      freeCamp / SuicideBomber::trainingCamp,
      freeCamp / Artillery::trainingCamp,
      freeCamp / Logistic::trainingCamp,
      freeCamp / ArmoredCar::trainingCamp,
      freeCamp / Tank1::trainingCamp,
      freeCamp / Tank2::trainingCamp,
      freeCamp / TankOshimai::trainingCamp,
      freeCamp / Cas::trainingCamp,
      freeCamp / Fighter::trainingCamp,
      freeCamp / Bomber::trainingCamp,
      freeCamp / Kamikaze::trainingCamp};
  std::vector<int> maxRemove = {
      this->troop->infantry[0],
      this->troop->calvary[0],
      this->troop->suicideBomber[0],
      this->troop->artillery[0],
      this->troop->logistic[0],
      this->troop->armoredCar[0],
      this->troop->tank1[0],
      this->troop->tank2[0],
      this->troop->tankOshimai[0],
      this->troop->cas[0],
      this->troop->fighter[0],
      this->troop->bomber[0],
      this->troop->kamikaze[0]};
}