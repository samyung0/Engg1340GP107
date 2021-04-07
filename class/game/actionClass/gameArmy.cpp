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

#include "../game.h"
#include "../../../io/io.h"
#include "../gameUnit.h"

#include "../../../data/troop/troop.h"
#include <tuple>

// the structure is similar to that of game
// reason why it is not done in game is because the number of armies are dynamic and it would be messy to deal with it in game
void Game::gameArmy(int &currentPhase, int prevPhase)
{
  this->lguser.unlock();
  this->army->total["Hisshou"] = new ArmyUnit("Hisshou", {std::make_tuple(0, 0, new Infantry(this->uuid())), std::make_tuple(1, 0, new TankOshimai(this->uuid()))});
  this->army->total["Hisshou2"] = new ArmyUnit("Hisshou2");
  this->army->total["Hisshou2"]->inBattle = true;
  this->army->total["Hisshou3"] = new ArmyUnit("Hisshou3");
  this->army->total["Hisshou4"] = new ArmyUnit("Hisshou4");
  this->army->total["Hisshou5"] = new ArmyUnit("Hisshou5");
  this->army->total["Hisshou6"] = new ArmyUnit("Hisshou6");
  this->army->total["Hisshou7"] = new ArmyUnit("Hisshou7");
  this->army->total["Hisshou8"] = new ArmyUnit("Hisshou8");
  this->army->total["Hisshou9"] = new ArmyUnit("Hisshou9");
  this->army->total["Hisshou10"] = new ArmyUnit("Hisshou10");
  this->army->total["Hisshou71"] = new ArmyUnit("Hisshou71");
  this->army->total["Hisshou82"] = new ArmyUnit("Hisshou82");
  this->army->total["Hisshou93"] = new ArmyUnit("Hisshou93");
  this->army->total["Hisshou104"] = new ArmyUnit("Hisshou104");

  int displayRange[2] = {this->screenHeight - 9, this->screenWidth};

  int armyPhase = 0;
  int armyPhaseSelect[2] = {0, 0};
  int inputMode = 0; // 0 for arrow, 1 for wasd
  std::vector<std::unordered_map<int, std::string>> history;

  std::future<void> phase0;

  bool terminatePhase0 = false;
  std::condition_variable terminatePhase0Cond;
  std::mutex phase0a;
  std::mutex phase0b;

  std::function<void()> loopArmyPhase0;
  std::function<void()> stopLoopArmyPhase0;
  std::function<void()> armyPhase0;

  std::function<void(std::string)> editArmy;
  std::function<void(std::string)> removeArmy;
  std::function<void(std::string)> retreatArmy;

  std::vector<std::function<void()> *> armyPhaseF = {&loopArmyPhase0};

  int moved[2] = {0, 0};

  // defaults to handle input every frame (per fps)
  // bool inputCooldown = false;
  // std::future<void> inputCooldownAsync;

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
    std::vector<std::vector<std::function<void()>>> armyPhaseProgressF = {};
    for (auto &i : this->army->total)
    {
      armyPhaseProgressF.push_back({});
      if (i.second->inBattle)
        armyPhaseProgressF.back().push_back([i, &retreatArmy]() {
          retreatArmy(i.first);
        });
      else
      {
        armyPhaseProgressF.back().push_back([i, &editArmy]() {
          editArmy(i.first);
        });
        armyPhaseProgressF.back().push_back([i, &removeArmy]() {
          removeArmy(i.first);
        });
      }
    }
    std::vector<std::vector<int>> action;

    std::vector<std::string> armyName;
    std::vector<std::string> content = {};

    std::unordered_map<std::string, std::string> typeToDisplay = {
        {"infantry", "Infantry"},
        {"calvary", "Calvary"},
        {"suicideBomber", "Suicide Bomber"},
        {"logistic", "Logistic"},
        {"armoredCar", "Armored Car"},
        {"artillery", "Artillery"},
        {"tank1", "Tank 1"},
        {"tank2", "Tank 2"},
        {"tankOshimai", "Tank O"},
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

    for (auto &i : this->army->total)
    {
      if (!i.second->removed && !i.second->lost)
      {
        action.push_back({});
        content.push_back("");
        content.back() += i.second->name + std::string(10 - i.second->name.length(), ' ') + ": ";

        for (int j = 0; j < 4; j++)
        {
          if (j != 0)
          {
            content.push_back("");
          }
          for (int k = 3; k >= 0; k--)
          {

            if (i.second->formation[k][j] == NULL)
            {
              content.back() += "None" + std::string(20 - 4, ' ');
            }
            else
            {
              content.back() += typeToDisplay[i.second->formation[k][j]->type] + "(";

              // casting down from double to int
              int hp = i.second->formation[k][j]->getHealth();
              int baseHp = typeToHp[i.second->formation[k][j]->type];
              content.back() += std::to_string(hp) + "/" + std::to_string(baseHp) + ")";
              content.back() += std::string(12 + (4 - k) * 20 - content.back().length(), ' ');
            }
          }
        }
        content.push_back("");
        if (i.second->inBattle)
        {
          content[content.size() - 5] += "   Retreat";
          action.back().push_back(93);
        }
        else
        {
          content[content.size() - 5] += "   Edit   Remove";
          action.back().push_back(93);
          action.back().push_back(100);
        }
      }
    }
    action.push_back({0});

    // std::cout << "\033[2J\033[1;1H";
    std::cout << "\033[" << 1 << ";" << 1 << "H";
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
    std::cout << (armyPhaseSelect[0] == 0 ? " > " : "   ") + underline("New Army", "green") << std::endl;

    int displayContent[2] = {(int)content.size(), 108};
    int movableRange[2] = {std::max(displayContent[0] - displayRange[0], 0), std::max(displayContent[1] - displayRange[1], 0)};

    moved[0] = std::min(std::max(0, moved[0]), movableRange[0]);
    moved[1] = std::min(std::max(0, moved[1]), movableRange[1]);

    armyPhaseSelect[0] = (armyPhaseSelect[0] + action.size()) % action.size();
    armyPhaseSelect[1] = (armyPhaseSelect[1] + action[armyPhaseSelect[0]].size()) % action[armyPhaseSelect[0]].size();

    int range[2] = {1, displayContent[0] - 2};

    if (inputMode == 1 && armyPhaseSelect[0] >= range[0] && armyPhaseSelect[0] <= range[1])
    {
      if ((armyPhaseSelect[0] - range[0]) * 5 < moved[0])
        armyPhaseSelect[0] = std::ceil(moved[0] / 5.0) + range[0];
      else if (moved[0] + displayRange[0] < (armyPhaseSelect[0] - range[0]) * 5 + 1)
        armyPhaseSelect[0] = std::ceil((moved[0] + displayRange[0]) / 5.0) - 1 + range[0];

      armyPhaseSelect[0] = (armyPhaseSelect[0] + action.size()) % action.size();
      armyPhaseSelect[1] = (armyPhaseSelect[1] + action[armyPhaseSelect[0]].size()) % action[armyPhaseSelect[0]].size();
    }
    else if (inputMode == 0 && armyPhaseSelect[0] >= range[0] && armyPhaseSelect[0] <= range[1])
    {
      if ((armyPhaseSelect[0] - range[0]) * 5 < moved[0])
        moved[0] = (armyPhaseSelect[0] - range[0]) * 5;
      else if (moved[0] + displayRange[0] < (armyPhaseSelect[0] - range[0]) * 5 + 1)
        moved[0] = (armyPhaseSelect[0] - range[0]) * 5 + 5-displayRange[0];

      moved[0] = std::min(std::max(0, moved[0]), movableRange[0]);
      moved[1] = std::min(std::max(0, moved[1]), movableRange[1]);
    }

    if (armyPhaseSelect[0] != action.size() - 1 && armyPhaseSelect[0] >= range[0] && armyPhaseSelect[0] <= range[1])
      content[(armyPhaseSelect[0]-range[0]) * 5].replace(action[armyPhaseSelect[0]][armyPhaseSelect[1]], 1, ">");

    std::vector<std::string> wrap = {"", ""};

    if (moved[0] != 0)
      wrap[0] = std::string(displayContent[1] / 2 - 11, ' ') + "||scroll up (press w)||";
    if (moved[0] != movableRange[0])
      wrap[1] = std::string(displayContent[1] / 2 - 12, ' ') + "||scroll down (press s)||";

    for (int i = 0; i < content.size(); i++)
    {
      if (i % 5 != 0)
        content[i].insert(0, std::string(12, ' '));
      content[i] += std::string(displayContent[1] - content[i].length(), ' ');
    }

    for (int i = 0; i < wrap.size(); i++)
      wrap[i] += std::string(displayContent[1] - wrap[i].length(), ' ');

    std::cout << wrap[0] << std::endl;
    for (int i = moved[0]; i < std::min(displayRange[0], displayContent[0]) + moved[0]; i++)
    {
      for (int j = moved[1]; j < std::min(displayRange[1], displayContent[1]) + moved[1]; j++)
      {
        std::cout << content[i][j];
      }
      std::cout << std::endl;
    }
    std::cout << wrap[1] << std::endl;
    std::cout << (armyPhaseSelect[0] == action.size() - 1 ? " > " : "   ") << underline("Back", "green") << " (or spacebar)" << std::endl;

    this->lg3.unlock();
  };

  std::cout << "\033[2J\033[1;1H";

  char input;
  (*armyPhaseF[armyPhase])();
  while (1)
  {
    int prevArmyPhase = armyPhase;

    input = getch();

    if (input == '\033')
    {
      getch();

      this->lg3.lock();
      inputMode = 0;
      switch (getch())
      {
      case 'A':
        armyPhaseSelect[0]--;
        break;
      case 'B':
        armyPhaseSelect[0]++;
        break;
      case 'C':
        armyPhaseSelect[1]++;
        break;
      case 'D':
        armyPhaseSelect[1]--;
        break;
      }

      this->lg3.unlock();
    }
    else
    {
      this->lg3.lock();
      inputMode = 1;
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
      this->lg3.unlock();
    }

    // stopLoopArmyPhase0();
  }
}