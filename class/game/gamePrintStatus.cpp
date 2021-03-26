#include <iostream>
#include <string>
#include <iomanip>

#include "../../io/io.h"
#include "game.h"
void Game::printStatus()
{
  this->lg.lock();
  std::cout << std::string(100, '\n') << std::endl;
  
  std::cout << color("Day: ", "magenta") << this->day << " (" << std::to_string(this->setting["speed"]/1000) << "s)"
            << std::endl
            << std::endl;
  std::cout << color("Resources:", "green") << "\n"
            << "Food: " << this->resource.food << "/" << this->totalFoodRequired
            << "   Equipment: " << this->resource.equipment << "/" << this->totalEquipmentRequired
            << "   Manpower: " << (this->resource.manpower - this->resource.manpowerInUse) << "/" << this->resource.manpower
            << "   Land: " << (this->resource.baseLand * this->resource.baseLandMul + this->resource.capturedLand - this->resource.usedLand) << "/" << this->resource.baseLand * this->resource.baseLandMul + this->resource.capturedLand
            << "   Troop: " << this->totalTroops
            << std::endl
            << std::endl
            << std::endl;
  std::cout << std::left << std::setw(40) << "(Level 1 , (2), (3))" << "(total, in army, in battle plan, in battle)"
            << std::endl;
  std::cout << std::setw(51) << color("Buildings:", "green")
            << color("Troops: ", "green")
            << std::setw(41) << "\nFarm: " + std::to_string(this->building.farm[0]) + "/" + std::to_string(this->building.farm[1]) + "/" + std::to_string(this->building.farm[2])
            << "Infantry: " << this->troop.infantry[0] << "/" << this->troop.infantry[1] << "/" << this->troop.infantry[2] << "/" << this->troop.infantry[3]
            << std::setw(41) << "\nCivilian Factory: " + std::to_string(this->building.civilianFactory[0]) + "/" + std::to_string(this->building.civilianFactory[1]) + "/" + std::to_string(this->building.civilianFactory[2])
            << "Calvary: " << this->troop.calvary[0] << "/" << this->troop.calvary[1] << "/" << this->troop.calvary[2] << "/" << this->troop.calvary[3]
            << std::setw(41) << "\nMilitary Factory: " + std::to_string(this->building.militaryFactory[0]) + "/" + std::to_string(this->building.militaryFactory[1]) + "/" + std::to_string(this->building.militaryFactory[2])
            << "Suicide Bomber: " << this->troop.suicideBomber[0] << "/" << this->troop.suicideBomber[1] << "/" << this->troop.suicideBomber[2] << "/" << this->troop.suicideBomber[3]
            << std::setw(41) << "\nTraining Camp: " + std::to_string(this->building.trainingCamp[0])
            << "Artillery: " << this->troop.artillery[0] << "/" << this->troop.artillery[1] << "/" << this->troop.artillery[2] << "/" << this->troop.artillery[3]
            << std::setw(41) << "\nAirport: " + std::to_string(this->building.airport[0])
            << "Logistic: " << this->troop.logistic[0] << "/" << this->troop.logistic[1] << "/" << this->troop.logistic[2] << "/" << this->troop.logistic[3]
            << std::setw(41) << "\n"
            << "Armored Car: " << this->troop.armoredCar[0] << "/" << this->troop.armoredCar[1] << "/" << this->troop.armoredCar[2] << "/" << this->troop.armoredCar[3]
            << std::setw(52) << "\n" + color("Researches:", "green")
            << "Tank 1: " << this->troop.tank1[0] << "/" << this->troop.tank1[1] << "/" << this->troop.tank1[2] << "/" << this->troop.tank1[3]
            << std::setw(41) << "\nFarm: " + this->helper(this->research.farm)
            << "Tank 2: " << this->troop.tank2[0] << "/" << this->troop.tank2[1] << "/" << this->troop.tank2[2] << "/" << this->troop.tank2[3]
            << std::setw(41) << "\nDivision of Labour: " + this->helper(this->research.divisionOfLabor)
            << "Cas: " << this->troop.cas[0] << "/" << this->troop.cas[1] << "/" << this->troop.cas[2] << "/" << this->troop.cas[3]
            << std::setw(41) << "\nProduction Line: " + this->helper(this->research.productionLine)
            << "Fighter: " << this->troop.fighter[0] << "/" << this->troop.fighter[1] << "/" << this->troop.fighter[2] << "/" << this->troop.fighter[3]
            << std::setw(41) << "\nLand Doctrine: " + this->helper(this->research.landDoctrine)
            << "Bomber: " << this->troop.bomber[0] << "/" << this->troop.bomber[1] << "/" << this->troop.bomber[2] << "/" << this->troop.bomber[3]
            << std::setw(41) << "\nAir Doctrine: " + this->helper(this->research.airDoctrine)
            << "Kamikaze: " << this->troop.kamikaze[0] << "/" << this->troop.kamikaze[1] << "/" << this->troop.kamikaze[2] << "/" << this->troop.kamikaze[3]
            << std::setw(41) << "\nUrbanization: " + this->helper(this->research.urbanization)
            << std::setw(41) << "\nWeapon: " + this->helper(this->research.weapon)
            << color("Actions: ", "red")
            << std::setw(41) << "\nTraining: " + this->helper(this->research.training)
            << std::setw(41) << "\nRecovery: " + this->helper(this->research.recovery)
            << std::endl
            << std::endl;

  this->lg.unlock();
}
