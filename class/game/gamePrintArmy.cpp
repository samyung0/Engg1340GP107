#include <string>
#include <iomanip>
#include <iostream>
#include <vector>

#include "game.h"
#include "../../io/io.h"

void Game::printArmy(int x, int y)
{
  this->lg3low.lock();
  this->lg3high.lock();
  this->lg3.lock();
  this->lg3high.unlock();

  std::vector<std::vector<std::string>> prefix;
  std::vector<std::vector<std::string>> action;

  std::vector<std::vector<std::string>> content;
  std::vector<std::vector<int>> coutCompensation;

  std::unordered_map<std::string, std::string> typeToDisplay = {
    {"infantry", "Infantry"},
    {"calvary", "Calvary"},
    {"suicideBomber", "Suicide Bomber"},
    {"logistic", "Logistic"},
    {"armoredCar", "Armored Car"},
    {"artillery", "Artillery"},
    {"tank1", "Tank 1"},
    {"tank2", "Tank 2"},
    {"tankOshimai", "Tank Oshimai"},
    {"cas", "Cas"},
    {"fighter", "Fighter"},
    {"bomber", "Bomber"},
    {"kamikaze", "Kamikaze"}
  };

  for (auto i: this->army->total)
  {
    if (!this->army->total[i.first]->removed && !this->army->total[i.first]->lost)
    {
      prefix.push_back({"   ", "   "});
      action.push_back({"Edit", "Remove"});

      content.push_back({""});
      if(this->army->total[i.first]->columnA[0] == NULL){
        content.back().back()+= "None" + std::string(10, ' ');
      }else {
        content.back().back()+= typeToDisplay[(*this->army->total[i.first]->columnA[0])->type] + " (";

        // casting down from double to hp
        int hp = (*this->army->total[i.first]->columnA[0])->getHealth();
        if((*this->army->total[i.first]->columnA[0])->getHealth() == 0)
      }
    }
  }

  prefix.push_back({"   "});
  action.push_back({"Back"});

  prefix[x][y].erase(1, 1);
  prefix[x][y].insert(1, color(">", "cyan"));

  this->lg3.unlock();
  this->lg3low.unlock();
}