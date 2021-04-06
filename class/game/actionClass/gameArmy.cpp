#include <string>
#include <iomanip>
#include <iostream>
#include <vector>
#include <functional>
#include <condition_variable>
#include <mutex>
#include <future>
#include <unordered_map>
#include <cassert>
#include <cmath>
#include <chrono>
#include <unistd.h>

#include "../game.h"
#include "../../../io/io.h"

// the structure is similar to that of game
// reason why it is not done in game is because the number of armies are dynamic and it would be messy to deal with it in game
void Game::gameArmy(int &currentPhase, int prevPhase)
{

  int armyPhase = 0;
  int armyPhaseSelect[2] = {0, 0};
  std::vector<std::unordered_map<int, std::string>> history;

  std::future<void> phase0;

  bool terminatePhase0 = false;
  std::condition_variable terminatePhase0Cond;
  std::mutex phase0a;
  std::mutex phase0b;
  std::mutex movedLock;

  std::function<void()> loopArmyPhase0;
  std::function<void()> stopLoopArmyPhase0;
  std::function<void()> armyPhase0;

  std::function<void(std::string)> editArmy;
  std::function<void(std::string)> removeArmy;
  std::function<void(std::string)> retreatArmy;

  std::vector<std::function<void()> *> armyPhaseF = {&loopArmyPhase0};

  int moved[2] = {0, 0};

  // defaults to handle input every frame (per fps)
  bool inputCooldown = false;
  std::future<void> inputCooldownAsync;

  loopArmyPhase0 = [&]() { phase0 = std::async(std::launch::async, [&]() {
                             terminatePhase0 = false;
                             while (!terminatePhase0)
                             {
                               armyPhase0();
                               std::unique_lock<std::mutex> lock(phase0a);
                               terminatePhase0Cond.wait_for(lock, std::chrono::milliseconds(1000 / 10));
                             }
                           }); };
  stopLoopArmyPhase0 = [&]() {
    if (phase0.valid())
    {
      phase0b.lock();
      terminatePhase0 = true;
      phase0b.unlock();
      terminatePhase0Cond.notify_all();
      phase0.get();
    }
  };

  armyPhase0 = [&]() {
    this->lg3.lock();
    std::vector<std::vector<std::function<void()>>> armyPhaseProgressF = {{NULL}};
    for (auto i : this->army->total)
    {
      armyPhaseProgressF.push_back({});
      armyPhaseProgressF.back().push_back([i, &editArmy]() {
        editArmy(i.first);
      });
      armyPhaseProgressF.back().push_back([i, &removeArmy]() {
        removeArmy(i.first);
      });
      if (i.second->inBattle)
        armyPhaseProgressF.back().push_back([i, &retreatArmy]() {
          retreatArmy(i.first);
        });
    }
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
      armyName.push_back(i.second->name);
      if (!i.second->removed && !i.second->lost)
      {
        prefix.push_back({"   ", "   "});

        std::string edit = (i.second->inBattle ? color("Edit", "red") : underline("Edit", "green"));
        std::string remove = (i.second->inBattle ? color("Remove", "red") : underline("Remove", "green"));

        action.push_back({"Edit", "Remove"});

        if (i.second->inBattle)
          action.back().push_back(underline("Retreat", "green"));

        content.push_back({});
        coutCompensation.push_back({});

        for (int j = 0; j < 4; j++)
        {
          content.back().push_back("");
          coutCompensation.back().push_back(0);
          if (i.second->columnD[j] == NULL)
          {
            content.back()[j] += "None" + std::string(10, ' ');
          }
          else
          {
            content.back()[j] += typeToDisplay[(*i.second->columnD[j])->type] + " (";

            // casting down from double to hp
            int hp = (*i.second->columnD[j])->getHealth();
            int baseHp = typeToHp[(*i.second->columnD[j])->type];
            if (1.0 * hp / baseHp < 0.1)
            {
              content.back()[j] += color(std::to_string(hp), "red");
              coutCompensation.back()[j] += 11;
            }
            else
              content.back()[j] += std::to_string(hp);

            content.back()[j] += "/" + std::to_string(baseHp) + ")";
          }

          if (i.second->columnC[j] == NULL)
          {
            content.back()[j] += "None" + std::string(10, ' ');
          }
          else
          {
            content.back()[j] += typeToDisplay[(*i.second->columnC[j])->type] + " (";

            int hp = (*i.second->columnC[j])->getHealth();
            int baseHp = typeToHp[(*i.second->columnC[j])->type];
            if (1.0 * hp / baseHp < 0.1)
            {
              content.back()[j] += color(std::to_string(hp), "red");
              coutCompensation.back()[j] += 11;
            }
            else
              content.back()[j] += std::to_string(hp);

            content.back()[j] += "/" + std::to_string(baseHp) + ")";
          }

          if (i.second->columnB[j] == NULL)
          {
            content.back()[j] += "None" + std::string(10, ' ');
          }
          else
          {
            content.back()[j] += typeToDisplay[(*i.second->columnB[j])->type] + " (";

            int hp = (*i.second->columnB[j])->getHealth();
            int baseHp = typeToHp[(*i.second->columnB[j])->type];
            if (1.0 * hp / baseHp < 0.1)
            {
              content.back()[j] += color(std::to_string(hp), "red");
              coutCompensation.back()[j] += 11;
            }
            else
              content.back()[j] += std::to_string(hp);

            content.back()[j] += "/" + std::to_string(baseHp) + ")";
          }

          if (i.second->columnA[j] == NULL)
          {
            content.back()[j] += "None" + std::string(10, ' ');
          }
          else
          {
            content.back()[j] += typeToDisplay[(*i.second->columnA[j])->type] + " (";

            int hp = (*i.second->columnA[j])->getHealth();
            int baseHp = typeToHp[(*i.second->columnA[j])->type];
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

    armyPhaseSelect[0] = (armyPhaseSelect[0] + prefix.size()) % prefix.size();
    armyPhaseSelect[1] = (armyPhaseSelect[1] + prefix[armyPhaseSelect[0]].size()) % prefix[armyPhaseSelect[0]].size();

    prefix[armyPhaseSelect[0]][armyPhaseSelect[1]].erase(1, 1);
    prefix[armyPhaseSelect[0]][armyPhaseSelect[1]].insert(1, color(">", "cyan"));

    // std::cout << "\033[2J\033[1;1H";
    std::cout << "\033[" << 1 << ";" << 1 << "H";
    std::cout << color("Army", "magenta") << std::endl
              << std::endl;
    std::cout << color("Resources:", "green") << "\n"
              << "\033[2K"
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

    int displayRange[2] = {this->screenHeight - 5, this->screenWidth};
    int displayContent[2] = {100, 25};
    int movableRange[2] = {std::max(displayContent[0] - displayRange[0],0), std::max(displayContent[1] - displayRange[1],0)};

    movedLock.lock();
    moved[0] = std::min(std::max(0, moved[0]), movableRange[0]);
    moved[1] = std::min(std::max(0, moved[1]), movableRange[1]);
    movedLock.unlock();

    for (int i = moved[0]; i < std::min(displayRange[0],displayContent[0]) + moved[0]; i++)
    {
      for (int j = moved[1]; j < std::min(displayRange[1],displayContent[1]) + moved[1]; j++)
      {
        std::cout << temp[i][j];
      }

      std::cout << std::endl;
    }

    this->lg3.unlock();
  };

  std::cout << "\033[2J\033[1;1H";

  char input;
  (*armyPhaseF[armyPhase])();
  while (1)
  {
    int prevArmyPhase = armyPhase;

    input = getch();
    if (inputCooldown)
      continue;
    else
    {
      inputCooldown = true;
      inputCooldownAsync = std::async(std::launch::async, [&inputCooldown, this]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
        inputCooldown = false;
      });
    }

    if (input == '\033')
    {
      getch();
      switch (getch())
      {
      case 'A':
        this->gamePhaseSelect[0]--;
        break;
      case 'B':
        this->gamePhaseSelect[0]++;
        break;
      case 'C':
        this->gamePhaseSelect[1]++;
        break;
      case 'D':
        this->gamePhaseSelect[1]--;
        break;
      }
    }
    else
    {
      movedLock.lock();
      switch (input)
      {
      case 'w':
        moved[0]--;
        break;
      case 's':
        moved[0]++;
        break;
      case 'd':
        moved[1]++;
        break;
      case 'a':
        moved[1]--;
        break;
      };
      movedLock.unlock();
    }

    // stopLoopArmyPhase0();
  }
}