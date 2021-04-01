#include <iostream>
#include <string>
#include <iomanip>
#include <cstdlib>

#include "../../io/io.h"
#include "game.h"
void Game::printStatus(int x, int y)
{
  std::vector<std::vector<std::string>> actionPrefix = {
    {"   ", "   ", "   ", "   ", "   ", "   "},
    {"   ", "   ", "   ", "   ", "   "}
  };
  actionPrefix[x][y].erase(1, 1);
  actionPrefix[x][y].insert(1, color(">", "cyan"));

  

  // lock lg for day
  this->lg.lock();


  std::cout << "\033[2J\033[1;1H";
  std::cout << color("Day: ", "magenta") << this->day << " (" << std::to_string(this->setting["speed"] / 1000) << "s)"
            << std::endl
            << std::endl;

  this->lg.unlock();
  this->lg3.lock();
  std::cout << color("Resources:", "green") << "\n"
            << "Food: " << this->troop->totalFoodRequired << "/" <<  this->resource->food
            << "   Equipment: " << this->resource->equipment << "/" << this->troop->totalEquipmentRequired
            << "   Manpower: " << (this->resource->manpower - this->resource->manpowerInUse) << "/" << this->resource->manpower
            << "   Land: " << (this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand) << "/" << this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand
            << "   Troop: " << this->troop->totalTroops
            << "   Armies: " << this->army->total.size() << "/10"
            << "   Battle Plans: " << this->battlePlan->total.size() << "/10"
            << "   Camps: " << this->resource->campUsed << "/"<<this->resource->camp
            << "   Airports: " << this->resource->airportUsed << "/"<<this->resource->airport
            << std::endl
            << std::endl
            << std::endl;
  std::cout << std::left << std::setw(40) << "(Level 1 , 2, 3)"
            << "(free, in army, in battle plan, in battle)"
            << std::endl;
  std::cout << std::setw(51) << color("Buildings:", "green")
            << std::setw(51) << color("Troops: ", "green")
            << std::setw(51) << color("Armies", "green")
            << std::setw(51) << color("Battle Plans", "green")
            << std::setw(41) << "\nFarm: " + std::to_string(this->building->farm[0]) + "/" + std::to_string(this->building->farm[1]) + "/" + std::to_string(this->building->farm[2])
            << std::setw(40) << "Infantry: " + std::to_string(this->troop->infantry[0]) + "/" + std::to_string(this->troop->infantry[1]) + "/" + std::to_string(this->troop->infantry[2]) + "/" + std::to_string(this->troop->infantry[3])
            << std::setw(40) << (this->army->total.size() == 0 ? "No army created" : this->army->total.begin()->first)
            << std::setw(40) << (this->battlePlan->total.size() == 0 ? "No battle plan created" : "1")
            << std::setw(41) << "\nCivilian Factory: " + std::to_string(this->building->civilianFactory[0]) + "/" + std::to_string(this->building->civilianFactory[1]) + "/" + std::to_string(this->building->civilianFactory[2])
            << std::setw(40) << "Calvary: " + std::to_string(this->troop->calvary[0]) + "/" + std::to_string(this->troop->calvary[1]) + "/" + std::to_string(this->troop->calvary[2]) + "/" + std::to_string(this->troop->calvary[3])
            << std::setw(40) << (this->army->total.size() > 1 ? (++this->army->total.begin())->first : "")
            << std::setw(40) << (this->battlePlan->total.size() > 1 ? "2" : "")
            << std::setw(41) << "\nMilitary Factory: " + std::to_string(this->building->militaryFactory[0]) + "/" + std::to_string(this->building->militaryFactory[1]) + "/" + std::to_string(this->building->militaryFactory[2])
            << std::setw(40) << "Suicide Bomber: " + std::to_string(this->troop->suicideBomber[0]) + "/" + std::to_string(this->troop->suicideBomber[1]) + "/" + std::to_string(this->troop->suicideBomber[2]) + "/" + std::to_string(this->troop->suicideBomber[3])
            << std::setw(40) << (this->army->total.size() > 2 ? (++++this->army->total.begin())->first : "")
            << std::setw(40) << (this->battlePlan->total.size() > 2 ? "3" : "")
            << std::setw(41) << "\nTraining Camp: " + std::to_string(this->building->trainingCamp[0])
            << std::setw(40) << "Artillery: " + std::to_string(this->troop->artillery[0]) + "/" + std::to_string(this->troop->artillery[1]) + "/" + std::to_string(this->troop->artillery[2]) + "/" + std::to_string(this->troop->artillery[3])
            << std::setw(40) << (this->army->total.size() > 3 ? (++++++this->army->total.begin())->first : "")
            << std::setw(40) << (this->battlePlan->total.size() > 3 ? "4" : "")
            << std::setw(41) << "\nAirport: " + std::to_string(this->building->airport[0])
            << std::setw(40) << "Logistic: " + std::to_string(this->troop->logistic[0]) + "/" + std::to_string(this->troop->logistic[1]) + "/" + std::to_string(this->troop->logistic[2]) + "/" + std::to_string(this->troop->logistic[3])
            << std::setw(40) << (this->army->total.size() > 4 ? (++++++++this->army->total.begin())->first : "")
            << std::setw(40) << (this->battlePlan->total.size() > 4 ? "5" : "")
            << std::setw(41) << "\n"
            << std::setw(40) << "Armored Car: " + std::to_string(this->troop->armoredCar[0]) + "/" + std::to_string(this->troop->armoredCar[1]) + "/" + std::to_string(this->troop->armoredCar[2]) + "/" + std::to_string(this->troop->armoredCar[3])
            << std::setw(40) << (this->army->total.size() > 5 ? (++++++++++this->army->total.begin())->first : "")
            << std::setw(40) << (this->battlePlan->total.size() > 5 ? "6" : "")
            << std::setw(52) << "\n" + color("Researches:", "green")
            << std::setw(40) << "Tank 1: " + std::to_string(this->troop->tank1[0]) + "/" + std::to_string(this->troop->tank1[1]) + "/" + std::to_string(this->troop->tank1[2]) + "/" + std::to_string(this->troop->tank1[3])
            << std::setw(40) << (this->army->total.size() > 6 ? (++++++++++++this->army->total.begin())->first : "")
            << std::setw(40) << (this->battlePlan->total.size() > 6 ? "7" : "")
            << std::setw(41) << "\nFarm: " + this->helper(this->research->farm)
            << std::setw(40) << "Tank 2: " + std::to_string(this->troop->tank2[0]) + "/" + std::to_string(this->troop->tank2[1]) + "/" + std::to_string(this->troop->tank2[2]) + "/" + std::to_string(this->troop->tank2[3])
            << std::setw(40) << (this->army->total.size() > 7 ? (++++++++++++++this->army->total.begin())->first : "")
            << std::setw(40) << (this->battlePlan->total.size() > 7 ? "8" : "")
            << std::setw(41) << "\nDivision of Labour: " + this->helper(this->research->divisionOfLabor)
            << std::setw(40) << "Tank Oshimai: " + std::to_string(this->troop->tankOshimai[0]) + "/" + std::to_string(this->troop->tankOshimai[1]) + "/" + std::to_string(this->troop->tankOshimai[2]) + "/" + std::to_string(this->troop->tankOshimai[3])
            << std::setw(40) << (this->army->total.size() > 8 ? (++++++++++++++++this->army->total.begin())->first : "")
            << std::setw(40) << (this->battlePlan->total.size() > 8 ? "9" : "")
            << std::setw(41) << "\nProduction Line: " + this->helper(this->research->productionLine)
            << std::setw(40) << "Cas: " + std::to_string(this->troop->cas[0]) + "/" + std::to_string(this->troop->cas[1]) + "/" + std::to_string(this->troop->cas[2]) + "/" + std::to_string(this->troop->cas[3])
            << std::setw(40) << (this->army->total.size() > 9 ? (++++++++++++++++++this->army->total.begin())->first : "")
            << std::setw(40) << (this->battlePlan->total.size() > 9 ? "10" : "")
            << std::setw(41) << "\nLand Doctrine: " + this->helper(this->research->landDoctrine)
            << std::setw(40) << "Fighter: " + std::to_string(this->troop->fighter[0]) + "/" + std::to_string(this->troop->fighter[1]) + "/" + std::to_string(this->troop->fighter[2]) + "/" + std::to_string(this->troop->fighter[3])
            << std::setw(41) << "\nAir Doctrine: " + this->helper(this->research->airDoctrine)
            << std::setw(40) << "Bomber: " + std::to_string(this->troop->bomber[0]) + "/" + std::to_string(this->troop->bomber[1]) + "/" + std::to_string(this->troop->bomber[2]) + "/" + std::to_string(this->troop->bomber[3])
            << std::setw(41) << "\nUrbanization: " + this->helper(this->research->urbanization)
            << std::setw(40) << "Kamikaze: " + std::to_string(this->troop->kamikaze[0]) + "/" + std::to_string(this->troop->kamikaze[1]) + "/" + std::to_string(this->troop->kamikaze[2]) + "/" + std::to_string(this->troop->kamikaze[3])
            << std::setw(51) << color("Battling Country: ", "green") + (this->battle->inBattle ? this->battle->countryBattling : "none")
            << std::setw(41) << "\nWeapon: " + this->helper(this->research->weapon)
            << std::setw(40) << ""
            << std::setw(51) << color("Battling Regions: ", "green") + (this->battle->inBattle ? this->helper2(this->battle->regionBattling) : "none")
            << std::setw(41) << "\nTraining: " + this->helper(this->research->training)
            << color("Actions: ", "red")
            << std::setw(41) << "\nRecovery: " + this->helper(this->research->recovery)
            << actionPrefix[0][0] + underline("Build      ", "green") + actionPrefix[0][1] + underline("Research   ", "green") + actionPrefix[0][2] +  underline("Train      ", "green") + actionPrefix[0][3] +  underline("Army       ", "green") + actionPrefix[0][4] +  underline("Battle Plan", "green") + actionPrefix[0][5] +  underline("Battle     ", "magenta")
            << std::endl
            << std::setw(40) << "" << actionPrefix[1][0] + underline("Speed      ", "green") + actionPrefix[1][1] + underline("Pause      ", "green") + actionPrefix[1][2] + underline("Save As    ", "green") + actionPrefix[1][3] + underline("Restart    ", "green") + actionPrefix[1][4] + underline("Quit       ", "green")
            << std::endl;
  this->lg3.unlock();
}
