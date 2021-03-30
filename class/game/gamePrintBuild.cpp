#include <iostream>
#include <string>
#include <iomanip>
#include <cmath>
#include <tuple>

#include "../../io/io.h"
#include "game.h"
void Game::printBuild(int x, int y)
{
  // lock lg3 just in case the calculation is slow, becasue of the use of reference to class variables which are subjected to change anytime
  // (copying may slow down performace significantly later into the game, though not tested)
  this->lg3.lock();

  std::vector<std::vector<std::string>> actionPrefix = {

      {"   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ", "   "},
      {"   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ", "   "},
      {"   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ", "   "},
      {"   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ", "   "},
      {"   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ", "   "},
      {"   ", "   ", "   ", "   "},
      {"   ", "   ", "   ", "   ", "   ", "   "},
      {"   ", "   ", "   ", "   ", "   ", "   "},
      {"   ", "   ", "   ", "   ", "   ", "   "},
      {"   ", "   "},
      {"   ", "   "},
      {"   "}};

  std::vector<std::vector<std::string>> action = {

      {"1", "5", "10", "Max", "1", "5", "10", "Max"},
      {"1", "5", "10", "Max", "1", "5", "10", "Max"},
      {"1", "5", "10", "Max", "1", "5", "10", "Max"},
      {"1", "5", "10", "Max", "1", "5", "10", "Max"},
      {"1", "5", "10", "Max", "1", "5", "10", "Max"},
      {"1", "5", "10", "Max"},
      {"1", "All", "1", "All", "1", "All"},
      {"1", "All", "1", "All", "1", "All"},
      {"1", "All", "1", "All", "1", "All"},
      {"1", "All"},
      {"1", "All"},
      {"Back"}};

  // check how many buildings can be built for each type
  double freeLand = this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand;
  double freeManpower = this->resource->manpower - this->resource->manpowerInUse;
  std::vector<int> maxBuild = {
      (int)(freeLand / this->building->farmL[0]),
      (int)(freeLand / this->building->civilianFactoryL[0]),
      (int)(freeLand / this->building->militaryFactoryL[0]),
      (int)(freeLand / this->building->trainingCampL[0]),
      (int)(freeLand / this->building->airportL[0]),
  };
  for (int i = 0; i < maxBuild.size(); i++)
    if (maxBuild[i] > freeManpower)
      maxBuild[i] = freeManpower;
  for (int i = 0; i < 5; i++)
  {
    if (maxBuild[i] < 10)
      action[i][2] = color(action[i][2], "red");
    else
      action[i][2] = underline(action[i][2], "green");
    if (maxBuild[i] < 5)
      action[i][1] = color(action[i][1], "red");
    else
      action[i][1] = underline(action[i][1], "green");
    if (maxBuild[i] < 1)
      action[i][0] = color(action[i][0], "red");
    else
      action[i][0] = underline(action[i][0], "green");

    action[i][3] += "(" + std::to_string(maxBuild[i]) + ")";
    if (maxBuild[i] > 0)
      action[i][3] = underline(action[i][3], "green");
    else
      action[i][3] = color(action[i][3], "red");
  }

  std::vector<int> maxUpgrade = {
      (this->building->farmL[1] > this->building->farmL[0] ? std::min((int)(freeLand / (this->building->farmL[1]- this->building->farmL[0])), this->building->farm[0]) : this->building->farm[0]),
      (this->building->farmL[2] > this->building->farmL[1] ? std::min((int)(freeLand / (this->building->farmL[2]- this->building->farmL[1])), this->building->farm[1]) : this->building->farm[1]),
     (this->building->civilianFactoryL[1] > this->building->civilianFactoryL[0] ? std::min((int)(freeLand / (this->building->civilianFactoryL[1]- this->building->civilianFactoryL[0])), this->building->civilianFactory[0]) : this->building->civilianFactory[0]),
     (this->building->civilianFactoryL[2] > this->building->civilianFactoryL[1] ? std::min((int)(freeLand / (this->building->civilianFactoryL[2]- this->building->civilianFactoryL[1])), this->building->civilianFactory[1]) : this->building->civilianFactory[1]),
     (this->building->militaryFactoryL[1] > this->building->militaryFactoryL[0] ? std::min((int)(freeLand / (this->building->militaryFactoryL[1]- this->building->militaryFactoryL[0])), this->building->militaryFactory[0]) : this->building->militaryFactory[0]),
     (this->building->militaryFactoryL[2] > this->building->militaryFactoryL[1] ? std::min((int)(freeLand / (this->building->militaryFactoryL[2]- this->building->militaryFactoryL[1])), this->building->militaryFactory[1]) : this->building->militaryFactory[1])
  };
  for (int i = 0; i < maxUpgrade.size(); i++)
    if (maxUpgrade[i] > freeManpower)
      maxUpgrade[i] = freeManpower;
  std::vector<bool> canUpgrade = {
      this->building->farmU[1],
      this->building->farmU[2],
      this->building->civilianFactoryU[1],
      this->building->civilianFactoryU[2],
      this->building->militaryFactoryU[1],
      this->building->militaryFactoryU[2]};
  for (int i = 0; i < 5; i++)
  {
    if (maxUpgrade[i] < 10 || !canUpgrade[i])
      action[i][6] = color(action[i][6], "red");
    else
      action[i][6] = underline(action[i][6], "green");
    if (maxUpgrade[i] < 5 || !canUpgrade[i])
      action[i][5] = color(action[i][5], "red");
    else
      action[i][5] = underline(action[i][5], "green");
    if (maxUpgrade[i] < 1 || !canUpgrade[i])
      action[i][4] = color(action[i][4], "red");
    else
      action[i][4] = underline(action[i][4], "green");

    action[i][7] += "(" + std::to_string(maxUpgrade[i]) + ")";
    if (maxUpgrade[i] > 0 && canUpgrade[i])
      action[i][7] = underline(action[i][7], "green");
    else
      action[i][7] = color(action[i][7], "red");
  }
  if (maxUpgrade[5] < 10 || !canUpgrade[5])
    action[5][2] = color(action[5][2], "red");
  else
    action[5][2] = underline(action[5][2], "green");
  if (maxUpgrade[5] < 5 || !canUpgrade[5])
    action[5][1] = color(action[5][1], "red");
  else
    action[5][1] = underline(action[5][1], "green");
  if (maxUpgrade[5] < 1 || !canUpgrade[5])
    action[5][0] = color(action[5][0], "red");
  else
    action[5][0] = underline(action[5][0], "green");

  action[5][3] += "(" + std::to_string(maxUpgrade[5]) + ")";
  if (maxUpgrade[5] > 0 && canUpgrade[5])
    action[5][3] = underline(action[5][3], "green");
  else
    action[5][3] = color(action[5][3], "red");

  for (int i = 0; i < 3; i++)
  {
    action[6][i * 2 + 1] += "(" + std::to_string(this->building->farm[i]) + ")";
    if (this->building->farm[i] > 0)
    {
      action[6][i * 2] = underline(action[6][i * 2], "green");
      action[6][i * 2 + 1] = underline(action[6][i * 2 + 1], "green");
    }
    else
    {
      action[6][i * 2] = color(action[6][i * 2], "red");
      action[6][i * 2 + 1] = color(action[6][i * 2 + 1], "red");
    }
  }
  for (int i = 0; i < 3; i++)
  {
    action[7][i * 2 + 1] += "(" + std::to_string(this->building->civilianFactory[i]) + ")";
    if (this->building->civilianFactory[i] > 0)
    {
      action[7][i * 2] = underline(action[7][i * 2], "green");
      action[7][i * 2 + 1] = underline(action[7][i * 2 + 1], "green");
    }
    else
    {
      action[7][i * 2] = color(action[7][i * 2], "red");
      action[7][i * 2 + 1] = color(action[7][i * 2 + 1], "red");
    }
  }
  for (int i = 0; i < 3; i++)
  {
    action[8][i * 2 + 1] += "(" + std::to_string(this->building->militaryFactory[i]) + ")";
    if (this->building->militaryFactory[i] > 0)
    {
      action[8][i * 2] = underline(action[8][i * 2], "green");
      action[8][i * 2 + 1] = underline(action[8][i * 2 + 1], "green");
    }
    else
    {
      action[8][i * 2] = color(action[8][i * 2], "red");
      action[8][i * 2 + 1] = color(action[8][i * 2 + 1], "red");
    }
  }
  action[9][1] += "(" + std::to_string(this->building->trainingCamp[0]) + ")";
  if (this->building->trainingCamp[0] > 0)
  {
    action[9][0] = underline(action[9][0], "green");
    action[9][1] = underline(action[9][1], "green");
  }
  else
  {
    action[9][0] = color(action[9][0], "red");
    action[9][1] = color(action[9][1], "red");
  }
  action[10][1] += "(" + std::to_string(this->building->airport[0]) + ")";
  if (this->building->airport[0] > 0)
  {
    action[10][0] = underline(action[10][0], "green");
    action[10][1] = underline(action[10][1], "green");
  }
  else
  {
    action[10][0] = color(action[10][0], "red");
    action[10][1] = color(action[10][1], "red");
  }

  action[11][0] = underline(action[11][0], "green");

  std::unordered_map<std::string, std::vector<std::tuple<std::string, std::string, std::string> *>> inProgressCount = {
      {"farm", {}}, {"civliianFactory", {}}, {"militaryFactory", {}}, {"trainingCamp", {}}, {"airport", {}}};

  for (int i = 0; i < this->building->progressTrack.size(); i++)
  {
    inProgressCount[std::get<0>(this->building->progressTrack[i])].push_back(&this->building->progressTrack[i]);
  }

  actionPrefix[x][y].erase(1, 1);
  actionPrefix[x][y].insert(1, color(">", "cyan"));

  std::cout << "\033[2J\033[1;1H";
  std::cout << "(cannot directly build level 2/3 buildings)" << std::endl;
  std::cout << color("Building: ", "magenta")
            << std::endl
            << std::endl;
  std::cout << color("Resources:", "green") << "\n"
            << "Food: " << this->troop->totalFoodRequired << "/" <<  this->resource->food
            << "   Equipment: " << this->resource->equipment << "/" << this->troop->totalEquipmentRequired
            << "   Manpower: " << (this->resource->manpower - this->resource->manpowerInUse) << "/" << this->resource->manpower
            << "   Land: " << (this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand) << "/" << this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand
            << "   Troop: " << this->troop->totalTroops
            << "   Armies: " << this->army->total.size() << "/10"
            << "   Battle Plans: " << this->battlePlan->total.size() << "/10"
            << std::endl
            << std::endl;
  std::cout << std::setw(50 + 11) << color("Build: ", "green")
            << color("Upgrade: ", "green");
  std::cout << "\nFarm:" << std::string(14, ' ') << actionPrefix[0][0] + action[0][0] + actionPrefix[0][1] + action[0][1] + actionPrefix[0][2] + action[0][2] + actionPrefix[0][3] + action[0][3]
            << std::string(10 - std::to_string(maxBuild[0]).length(), ' ')
            << "Farm 1 -> 2:" << std::string(12, ' ') << actionPrefix[0][4] + action[0][4] + actionPrefix[0][5] + action[0][5] + actionPrefix[0][6] + action[0][6] + actionPrefix[0][7] + action[0][7];
  std::cout << "\nCivilian Factory:" << std::string(2, ' ') << actionPrefix[1][0] + action[1][0] + actionPrefix[1][1] + action[1][1] + actionPrefix[1][2] + action[1][2] + actionPrefix[1][3] + action[1][3]
            << std::string(10 - std::to_string(maxBuild[1]).length(), ' ')
            << "Farm 2 -> 3:" << std::string(12, ' ') << actionPrefix[1][4] + action[1][4] + actionPrefix[1][5] + action[1][5] + actionPrefix[1][6] + action[1][6] + actionPrefix[1][7] + action[1][7];
  std::cout << "\nMilitary Factory:" << std::string(2, ' ') << actionPrefix[2][0] + action[2][0] + actionPrefix[2][1] + action[2][1] + actionPrefix[2][2] + action[2][2] + actionPrefix[2][3] + action[2][3]
            << std::string(10 - std::to_string(maxBuild[2]).length(), ' ')
            << "Civilian Factory 1 -> 2:" + actionPrefix[2][4] + action[2][4] + actionPrefix[2][5] + action[2][5] + actionPrefix[2][6] + action[2][6] + actionPrefix[2][7] + action[2][7];
  std::cout << "\nTraining Camp:" << std::string(5, ' ') << actionPrefix[3][0] + action[3][0] + actionPrefix[3][1] + action[3][1] + actionPrefix[3][2] + action[3][2] + actionPrefix[3][3] + action[3][3]
            << std::string(10 - std::to_string(maxBuild[3]).length(), ' ')
            << "Civilian Factory 2 -> 3:" << actionPrefix[3][4] + action[3][4] + actionPrefix[3][5] + action[3][5] + actionPrefix[3][6] + action[3][6] + actionPrefix[3][7] + action[3][7];
  std::cout << "\nAirport:" << std::string(11, ' ') << actionPrefix[4][0] + action[4][0] + actionPrefix[4][1] + action[4][1] + actionPrefix[4][2] + action[4][2] + actionPrefix[4][3] + action[4][3]
            << std::string(10 - std::to_string(maxBuild[4]).length(), ' ')
            << "Military Factory 1 -> 2:" << actionPrefix[4][4] + action[4][4] + actionPrefix[4][5] + action[4][5] + actionPrefix[4][6] + action[4][6] + actionPrefix[4][7] + action[4][7]
            << std::setw(51) << "\n"
            << "Military Factory 2 -> 3:" << actionPrefix[5][0] + action[5][0] + actionPrefix[5][1] + action[5][1] + actionPrefix[5][2] + action[5][2] + actionPrefix[5][3] + action[5][3]
            << std::endl
            << std::endl;
  std::cout << std::setw(50 + 11) << color("Overview: ", "green")
            << color("Remove: ", "green");
  std::cout << "\nFarm:" << std::string(17, ' ') << this->building->farm[0] << ", " << this->building->farm[1] << ", " << this->building->farm[2]
            << std::string(28 - 4 - std::to_string(this->building->farm[0]).length() - std::to_string(this->building->farm[1]).length() - std::to_string(this->building->farm[2]).length(), ' ')
            << "Farm:" << std::string(19, ' ') << actionPrefix[6][0] << action[6][0] << actionPrefix[6][1] << action[6][1] << std::string(5 - std::to_string(this->building->farm[0]).length(), ' ') << "/" << actionPrefix[6][2] << action[6][2] << actionPrefix[6][3] << action[6][3] << std::string(5 - std::to_string(this->building->farm[1]).length(), ' ') << "/" << actionPrefix[6][4] << action[6][4] << actionPrefix[6][5] << action[6][5]
            << "\nCivilian Factory:" << std::string(5, ' ') << this->building->civilianFactory[0] << ", " << this->building->civilianFactory[1] << ", " << this->building->civilianFactory[2]
            << std::string(28 - 4 - std::to_string(this->building->civilianFactory[0]).length() - std::to_string(this->building->civilianFactory[1]).length() - std::to_string(this->building->civilianFactory[2]).length(), ' ')
            << "Civilian Factory:" << std::string(7, ' ') << actionPrefix[7][0] << action[7][0] << actionPrefix[7][1] << action[7][1] << std::string(5 - std::to_string(this->building->civilianFactory[0]).length(), ' ') << "/" << actionPrefix[7][2] << action[7][2] << actionPrefix[7][3] << action[7][3] << std::string(5 - std::to_string(this->building->civilianFactory[1]).length(), ' ') << "/" << actionPrefix[7][4] << action[7][4] << actionPrefix[7][5] << action[7][5]
            << "\nMilitary Factory:" << std::string(5, ' ') << this->building->militaryFactory[0] << ", " << this->building->militaryFactory[1] << ", " << this->building->militaryFactory[2]
            << std::string(28 - 4 - std::to_string(this->building->militaryFactory[0]).length() - std::to_string(this->building->militaryFactory[1]).length() - std::to_string(this->building->militaryFactory[2]).length(), ' ')
            << "Military Factory:" << std::string(7, ' ') << actionPrefix[8][0] << action[8][0] << actionPrefix[8][1] << action[8][1] << std::string(5 - std::to_string(this->building->militaryFactory[0]).length(), ' ') << "/" << actionPrefix[8][2] << action[8][2] << actionPrefix[8][3] << action[8][3] << std::string(5 - std::to_string(this->building->militaryFactory[1]).length(), ' ') << "/" << actionPrefix[8][4] << action[8][4] << actionPrefix[8][5] << action[8][5]
            << "\nTraining Camp:" << std::string(8, ' ') << this->building->trainingCamp[0]
            << std::string(28 - std::to_string(this->building->trainingCamp[0]).length(), ' ')
            << "Training Camp:" << std::string(10, ' ') << actionPrefix[9][0] << action[9][0] << actionPrefix[9][1] << action[9][1]
            << "\nAriport:" << std::string(14, ' ') << this->building->airport[0]
            << std::string(28 - std::to_string(this->building->airport[0]).length(), ' ')
            << "Airport:" << std::string(16, ' ') << actionPrefix[10][0] << action[10][0] << actionPrefix[10][1] << action[10][1]
            << std::endl
            << std::endl;
  std::cout << std::setw(50 + 11) << color("In Progress: ", "green")
            << "\nFarm:" << std::string(17, ' ') << (inProgressCount["farm"].size() == 0 ? "no building in progress" : ([&]() -> std::string {
                 std::string temp = "";
                 for (auto &i : inProgressCount["farm"])
                   temp += std::get<2>(*i) + " (" + std::to_string(this->building->progress[std::get<1>(*i)]->remain) + " days), ";
                 return temp.substr(0, temp.length() - 2);
               }()))
            << "\nCivilian Factory:" << std::string(5, ' ') << (inProgressCount["civilianFactory"].size() == 0 ? "no building in progress" : ([&]() -> std::string {
                 std::string temp = "";
                 for (auto &i : inProgressCount["civilianFactory"])
                   temp += std::get<2>(*i) + " (" + std::to_string(this->building->progress[std::get<1>(*i)]->remain) + " days), ";
                 return temp.substr(0, temp.length() - 2);
               }()))
            << "\nMilitary Factory:" << std::string(5, ' ') << (inProgressCount["militaryFactory"].size() == 0 ? "no building in progress" : ([&]() -> std::string {
                 std::string temp = "";
                 for (auto &i : inProgressCount["militaryFactory"])
                   temp += std::get<2>(*i) + " (" + std::to_string(this->building->progress[std::get<1>(*i)]->remain) + " days), ";
                 return temp.substr(0, temp.length() - 2);
               }()))
            << "\nTraining Camp:" << std::string(8, ' ') << (inProgressCount["trianingCamp"].size() == 0 ? "no building in progress" : ([&]() -> std::string {
                 std::string temp = "";
                 for (auto &i : inProgressCount["trianingCamp"])
                   temp += std::get<2>(*i) + " (" + std::to_string(this->building->progress[std::get<1>(*i)]->remain) + " days), ";
                 return temp.substr(0, temp.length() - 2);
               }()))
            << "\nAirport:" << std::string(14, ' ') << (inProgressCount["airport"].size() == 0 ? "no building in progress" : ([&]() -> std::string {
                 std::string temp = "";
                 for (auto &i : inProgressCount["airport"])
                   temp += std::get<2>(*i) + " (" + std::to_string(this->building->progress[std::get<1>(*i)]->remain) + " days), ";
                 return temp.substr(0, temp.length() - 2);
               }()))
            << std::endl
            << std::endl;
  std::cout << actionPrefix[11][0] << action[11][0] << " (Or spacebar)" << std::endl << std::endl ;
  // unlock after all results are printed because of the same reference thing
  this->lg3.unlock();
}
