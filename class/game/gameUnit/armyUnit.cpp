#include <string>
#include <vector>
#include <tuple>
#include <iostream>
#include <cassert>
#include <cmath>
#include <future>

#include "../gameStruct.h"
#include "../../../data/troop/troop.h"

ArmyUnit::ArmyUnit(std::string name_) : name(name_) {}

void ArmyUnit::addTroop(int x, int y, std::string type, data::Troop *troop, data::Resource *resource)
{
  int index = -1;
  for (int i = 0; i < troop->allTroop.size(); i++)
  {
    if (troop->allTroop[i]->state["free"] && troop->allTroop[i]->type == type)
    {
      index = i;
      break;
    }
  }
  assert(index != -1);
  this->removeTroop(x, y, troop, resource);

  troop->allTroop[index]->state["free"] = false;
  troop->allTroop[index]->state["army"] = true;
  troop->helper2[type](0, -1);
  troop->helper2[type](1, 1);

  this->formation[x][y] = troop->allTroop[index];

  this->troopCount++;
  this->totalBaseFoodRequired += troop->allTroop[index]->getFood();
  this->totalBaseEquipmentRequired += troop->allTroop[index]->getEquipment();

  if (type == "logistic")
  {
    this->logisticCount++;
    this->foodReductionPer++;
    this->speedBoostPerLand += 2;
  }

  this->totalFoodRequired = this->totalBaseFoodRequired - this->foodReductionPer * this->troopCount;
  this->totalEquipmentRequired = this->totalBaseEquipmentRequired - this->equipmentReductionPer * this->troopCount;

  // this->foodRatio = std::min(1.0, (std::min(1.0, resource->food / troop->totalFoodRequired) * this->totalBaseFoodRequired) / (this->totalFoodRequired == 0 ? 1 : this->totalFoodRequired));
  // this->equipmentRatio = std::min(1.0, (std::min(1.0, resource->equipment / troop->totalEquipmentRequired) * this->totalBaseEquipmentRequired) / (this->totalEquipmentRequired == 0 ? 1 : this->totalEquipmentRequired));

  // this->subsequentialStrength = (this->foodRatio + this->equipmentRatio)/2;

  troop->allTroop[index]->reference.push_back(true);
  troop->allTroop[index]->isReferenced = true;
}

void ArmyUnit::removeTroop(int x, int y, data::Troop *troop, data::Resource *resource)
{
  if (this->formation[x][y] == NULL)
    return;

  std::string type = this->formation[x][y]->type;

  this->formation[x][y]->state["army"] = false;
  this->formation[x][y]->state["battlePlan"] = false;
  assert(!this->formation[x][y]->state["battle"]);
  this->formation[x][y]->state["free"] = true;
  troop->helper2[type](0, 1);
  troop->helper2[type](1, -1);

  this->troopCount--;
  this->totalBaseFoodRequired -= this->formation[x][y]->getFood();
  this->totalBaseEquipmentRequired -= this->formation[x][y]->getEquipment();

  if (type == "logistic")
  {
    this->logisticCount--;
    this->foodReductionPer--;
    this->speedBoostPerLand -= 2;
  }

  this->totalFoodRequired = this->totalBaseFoodRequired - this->foodReductionPer * this->troopCount;
  this->totalEquipmentRequired = this->totalBaseEquipmentRequired - this->equipmentReductionPer * this->troopCount;

  // this->foodRatio = std::min(1.0, (std::min(1.0, resource->food / troop->totalFoodRequired) * this->totalBaseFoodRequired) / (this->totalFoodRequired == 0 ? 1 : this->totalFoodRequired));
  // this->equipmentRatio = std::min(1.0, (std::min(1.0, resource->equipment / troop->totalEquipmentRequired) * this->totalBaseEquipmentRequired) / (this->totalEquipmentRequired == 0 ? 1 : this->totalEquipmentRequired));

  // this->subsequentialStrength = (this->foodRatio + this->equipmentRatio)/2;

  this->formation[x][y]->reference.pop_back();
  this->formation[x][y]->isReferenced = this->formation[x][y]->reference.size() != 0;
  this->formation[x][y] = NULL;
}

void ArmyUnit::addTroopM(int x, int y, TroopUnit* instance)
{

  this->formation[x][y] = instance;

  this->troopCount++;
  this->totalBaseFoodRequired += instance->getFood();
  this->totalBaseEquipmentRequired += instance->getEquipment();

  if (instance->type == "logistic")
  {
    this->logisticCount++;
    this->foodReductionPer++;
    this->speedBoostPerLand += 2;
  }

  this->totalFoodRequired = this->totalBaseFoodRequired - this->foodReductionPer * this->troopCount;
  this->totalEquipmentRequired = this->totalBaseEquipmentRequired - this->equipmentReductionPer * this->troopCount;

  // this->foodRatio = std::min(1.0, (std::min(1.0, resource->food / troop->totalFoodRequired) * this->totalBaseFoodRequired) / (this->totalFoodRequired == 0 ? 1 : this->totalFoodRequired));
  // this->equipmentRatio = std::min(1.0, (std::min(1.0, resource->equipment / troop->totalEquipmentRequired) * this->totalBaseEquipmentRequired) / (this->totalEquipmentRequired == 0 ? 1 : this->totalEquipmentRequired));

  // this->subsequentialStrength = (this->foodRatio + this->equipmentRatio)/2;

  instance->reference.push_back(true);
  instance->isReferenced = true;
}

void ArmyUnit::removeTroopM(int x, int y)
{
  if (this->formation[x][y] == NULL)
    return;

  std::string type = this->formation[x][y]->type;

  this->troopCount--;
  this->totalBaseFoodRequired -= this->formation[x][y]->getFood();
  this->totalBaseEquipmentRequired -= this->formation[x][y]->getEquipment();

  if (type == "logistic")
  {
    this->logisticCount--;
    this->foodReductionPer--;
    this->speedBoostPerLand -= 2;
  }

  this->totalFoodRequired = this->totalBaseFoodRequired - this->foodReductionPer * this->troopCount;
  this->totalEquipmentRequired = this->totalBaseEquipmentRequired - this->equipmentReductionPer * this->troopCount;

  // this->foodRatio = std::min(1.0, (std::min(1.0, resource->food / troop->totalFoodRequired) * this->totalBaseFoodRequired) / (this->totalFoodRequired == 0 ? 1 : this->totalFoodRequired));
  // this->equipmentRatio = std::min(1.0, (std::min(1.0, resource->equipment / troop->totalEquipmentRequired) * this->totalBaseEquipmentRequired) / (this->totalEquipmentRequired == 0 ? 1 : this->totalEquipmentRequired));

  // this->subsequentialStrength = (this->foodRatio + this->equipmentRatio)/2;

  this->formation[x][y]->reference.pop_back();
  this->formation[x][y]->isReferenced = this->formation[x][y]->reference.size() != 0;
  this->formation[x][y] = NULL;
}