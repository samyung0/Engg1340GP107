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

  std::vector<std::string> armyName;
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
      {"kamikaze", "Kamikaze"}};

  std::unordered_map<std::string, int> typeToHp = {
      {"infantry", Infantry::baseHp},
      {"calvary", Calvary::baseHp},
      {"suicideBomber", SuicideBomber::baseHp},
      {"logistic", Logistic::baseHp},
      {"armoredCar", ArmoredCar::baseHp},
      {"artillery", Artillery::baseHp},
      {"tank1", Tank1::baseHp},
      {"tank2", Tank2::baseHp},
      {"tankOshimai", TankOshimai::baseHp},
      {"cas", Cas::baseHp},
      {"fighter", Fighter::baseHp},
      {"bomber", Bomber::baseHp},
      {"kamikaze", Kamikaze::baseHp}};

  for (auto i : this->army->total)
  {
    armyName.push_back(this->army->total[i.first]->name);
    if (!this->army->total[i.first]->removed && !this->army->total[i.first]->lost)
    {
      prefix.push_back({"   ", "   "});
      action.push_back({"Edit", "Remove"});

      content.push_back({});
      coutCompensation.push_back({});

      for (int j = 0; j < 4; j++)
      {
        content.back().push_back("");
        coutCompensation.back().push_back(0);
        if (this->army->total[i.first]->columnD[j] == NULL)
        {
          content.back()[j] += "None" + std::string(10, ' ');
        }
        else
        {
          content.back()[j] += typeToDisplay[(*this->army->total[i.first]->columnD[j])->type] + " (";

          // casting down from double to hp
          int hp = (*this->army->total[i.first]->columnD[j])->getHealth();
          int baseHp = typeToHp[(*this->army->total[i.first]->columnD[j])->type];
          if (1.0 * hp / baseHp < 0.1)
          {
            content.back()[j] += color(std::to_string(hp), "red");
            coutCompensation.back()[j] += 11;
          }
          else
            content.back()[j] += std::to_string(hp);

          content.back()[j] += "/" + std::to_string(baseHp) + ")";
        }

        if (this->army->total[i.first]->columnC[j] == NULL)
        {
          content.back()[j] += "None" + std::string(10, ' ');
        }
        else
        {
          content.back()[j] += typeToDisplay[(*this->army->total[i.first]->columnC[j])->type] + " (";

          int hp = (*this->army->total[i.first]->columnC[j])->getHealth();
          int baseHp = typeToHp[(*this->army->total[i.first]->columnC[j])->type];
          if (1.0 * hp / baseHp < 0.1)
          {
            content.back()[j] += color(std::to_string(hp), "red");
            coutCompensation.back()[j] += 11;
          }
          else
            content.back()[j] += std::to_string(hp);

          content.back()[j] += "/" + std::to_string(baseHp) + ")";
        }

        if (this->army->total[i.first]->columnB[j] == NULL)
        {
          content.back()[j] += "None" + std::string(10, ' ');
        }
        else
        {
          content.back()[j] += typeToDisplay[(*this->army->total[i.first]->columnB[j])->type] + " (";

          int hp = (*this->army->total[i.first]->columnB[j])->getHealth();
          int baseHp = typeToHp[(*this->army->total[i.first]->columnB[j])->type];
          if (1.0 * hp / baseHp < 0.1)
          {
            content.back()[j] += color(std::to_string(hp), "red");
            coutCompensation.back()[j] += 11;
          }
          else
            content.back()[j] += std::to_string(hp);

          content.back()[j] += "/" + std::to_string(baseHp) + ")";
        }

        if (this->army->total[i.first]->columnA[j] == NULL)
        {
          content.back()[j] += "None" + std::string(10, ' ');
        }
        else
        {
          content.back()[j] += typeToDisplay[(*this->army->total[i.first]->columnA[j])->type] + " (";

          int hp = (*this->army->total[i.first]->columnA[j])->getHealth();
          int baseHp = typeToHp[(*this->army->total[i.first]->columnA[j])->type];
          if (1.0 * hp / baseHp < 0.1)
          {
            content.back()[j] += color(std::to_string(hp), "red");
            coutCompensation.back()[j] += 11;
          }
          else
            content.back()[j] += std::to_string(hp);

          content.back()[j] += "/" + std::to_string(baseHp) + ")";
        }
      }
    }
  }

  prefix.push_back({"   "});
  action.push_back({"Back"});

  prefix[x][y].erase(1, 1);
  prefix[x][y].insert(1, color(">", "cyan"));

  std::cout << "\033[2J\033[1;1H";
  std::cout << color("Army", "magenta") << std::endl
            << std::endl;
  std::cout << color("Resources:", "green") << "\n"
            << "Food: " << this->troop->totalFoodRequired << "/" << this->resource->food
            << "   Equipment: " << this->troop->totalEquipmentRequired << "/" << this->resource->equipment
            << "   Manpower: " << (this->resource->manpower - this->resource->manpowerInUse) << "/" << this->resource->manpower
            << "   Land: " << (this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand) << "/" << this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand
            << "   Troop: " << this->troop->totalTroops
            << "   Armies: " << this->army->total.size() << "/10"
            << "   Battle Plans: " << this->battlePlan->total.size() << "/10"
            << "   Camps: " << this->resource->campUsed << "/" << this->resource->camp
            << "   Airports: " << this->resource->airportUsed << "/" << this->resource->airport
            << std::endl
            << std::endl;

  this->lg3.unlock();
  this->lg3low.unlock();
}