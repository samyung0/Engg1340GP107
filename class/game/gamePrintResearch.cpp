#include <iostream>
#include <string>
#include <iomanip>

#include "../../io/io.h"
#include "game.h"

void printResearch(int x, int y)
{
  std::vector<std::vector<std::string>> prefix = {{"   "}, {"   "}, {"   "}, {"   "}, {"   "}, {"   "}, {"   "}, {"   "}, {"   "}};
  std::vector<std::string> name = {
      "Farm", "Division of Labour", "Production Line", "Land Doctrine", "Air Doctrine", "Urbanization", "Weapon", "Training", "Recovery"};
  std::vector<std::vector<std::string>> data = {
      {" (40 days) Unlock Level 2 farm", " (40 days) Unlock Level 3 farm"},
      {" (20 days) Unlock Level 2 civilian factory", " (30 days) Unlock Level 3 civilian factory"},
      {" (15 days) Unlock Level 2 military factory", " (30 days) Unlock Level 3 military factory"},
      {" (40 days) +20% damage dealt by each unit of land troop", " (40 days) +30% (total) damage dealt by each unit of land troop"},
      {" (70 days) +40% damage dealt by each unit of plane", " (100 days) +60% (total) damage dealt by each unit of plane"},
      {" (50 days) +10% land available for building", " (70 days) +30% (total) land available for building"},
      {" (20 days) +5% damage dealt by each unit of tank", " (70 days) +10% (total) damage dealt by each unit of tank and plane"},
      {" (30 days) -10% training time required to train any unit of land troop", " (50 days) -20% (total) trianing time required to train any unit of land troop"},
      {" (30 days) Recovery rate increased to 6 everyday", " (50 days) Recovery rate increased to 8 everyday"},
  };

  prefix[x][y].erase(1, 1);
  prefix[x][y].insert(1, color(">", "cyan"));

  std::cout << color("Research", "magenta")
            << std::endl
            << std::endl
            << prefix[0][0] << name[0] << 
}