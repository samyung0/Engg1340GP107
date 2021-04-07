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
#include <regex>
#include <cctype>
#include <sstream>

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

  this->lg3.lock();
  this->army->total["Hisshou"] = new ArmyUnit("Hisshou", {std::make_tuple(0, 0, new Infantry(this->uuid())), std::make_tuple(1, 0, new TankOshimai(this->uuid()))});
  this->army->total["Hisshou2"] = new ArmyUnit("Hisshou2");
  this->army->total["Hisshou2"]->inBattle = true;
  this->army->total["Hisshou3"] = new ArmyUnit("Hisshou3");
  this->lg3.unlock();

  int displayRange[2] = {this->screenHeight - 9, this->screenWidth};

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
  std::vector<std::string> posToTroop = {"infantry", "calvary", "suicideBomber", "artillery", "logistic", "armoredCar", "tank1", "tank2", "tankOshimai", "cas", "fighter", "bomber", "kamikaze"};

  int armyPhase = 0;
  int armyPhaseSelect[2] = {0, 0};
  int inputMode = 0;     // 0 for arrow, 1 for wasd
  bool pickMode = false; // used in phase 2 when selecting troops
  int pickTroop[2] = {0, 0};
  int pickTroopPos = 0;
  std::string armySelected;

  std::future<void> phase0;
  std::future<void> phase2;

  bool terminatePhase0 = false;
  std::condition_variable terminatePhase0Cond;
  std::mutex phase0a;
  std::mutex phase0b;
  bool terminatePhase2 = false;
  std::condition_variable terminatePhase2Cond;
  std::mutex phase2a;
  std::mutex phase2b;

  std::function<void()> loopArmyPhase0;
  std::function<void()> stopLoopArmyPhase0;
  std::function<void()> armyPhase0;

  std::function<void()> armyPhase1;

  std::function<void()> loopArmyPhase2;
  std::function<void()> stopLoopArmyPhase2;
  std::function<void()> armyPhase2;

  std::function<void(std::string)> removeArmy;
  std::function<void(std::string)> retreatArmy;
  std::function<void(int *, std::string)> posTroop;
  std::function<void(int *)> removeTroop;

  std::vector<std::function<void()> *> armyPhaseF = {&loopArmyPhase0, &armyPhase1, &loopArmyPhase2};
  std::vector<std::vector<std::function<void()>>> actionPhase0 = {};

  int moved[2] = {0, 0};

  std::vector<std::vector<std::string>> prefixPhase2Troop = {
      {"   ", "   "},
      {"   "},
      {"   "},
      {"   "},
      {"   "},
      {"   "},
      {"   "},
      {"   "},
      {"   "},
      {"   "},
      {"   "},
      {"   "},
      {"   "},
      {"   "}};

  std::vector<std::vector<std::string>> prefixPhase2Pick = {
      {"   ", "   ", "   ", "   "},
      {"   ", "   ", "   ", "   "},
      {"   ", "   ", "   ", "   "},
      {"   ", "   ", "   ", "   "},
      {"   "}};

      std::vector<std::vector<std::function<void()>>> actionPhase2Pick = {
      {[&]() {posTroop(armyPhaseSelect,posToTroop[0]);pickMode=false; }, [&]() {removeTroop(armyPhaseSelect);pickMode=false; }},
      {[&]() {posTroop(armyPhaseSelect,posToTroop[1]);pickMode=false; }},
      {[&]() {posTroop(armyPhaseSelect,posToTroop[2]);pickMode=false; }},
      {[&]() {posTroop(armyPhaseSelect,posToTroop[3]);pickMode=false; }},
      {[&]() {posTroop(armyPhaseSelect,posToTroop[4]);pickMode=false; }},
      {[&]() {posTroop(armyPhaseSelect,posToTroop[5]);pickMode=false; }},
      {[&]() {posTroop(armyPhaseSelect,posToTroop[6]);pickMode=false; }},
      {[&]() {posTroop(armyPhaseSelect,posToTroop[7]);pickMode=false; }},
      {[&]() {posTroop(armyPhaseSelect,posToTroop[8]);pickMode=false; }},
      {[&]() {posTroop(armyPhaseSelect,posToTroop[9]);pickMode=false; }},
      {[&]() {posTroop(armyPhaseSelect,posToTroop[10]);pickMode=false; }},
      {[&]() {posTroop(armyPhaseSelect,posToTroop[11]);pickMode=false; }},
      {[&]() {posTroop(armyPhaseSelect,posToTroop[12]);pickMode=false; }},
      {[&]() {pickTroop[0] = 0;
          pickTroop[1] = 0;pickMode=false; }}};

  std::vector<std::vector<std::function<void()>>> actionPhase2Troop = {
      {[&]() { pickMode = true; }, [&]() { pickMode = true; }, [&]() { pickMode = true; }, [&]() { pickMode = true; }},
      {[&]() { pickMode = true; }, [&]() { pickMode = true; }, [&]() { pickMode = true; }, [&]() { pickMode = true; }},
      {[&]() { pickMode = true; }, [&]() { pickMode = true; }, [&]() { pickMode = true; }, [&]() { pickMode = true; }},
      {[&]() { pickMode = true; }, [&]() { pickMode = true; }, [&]() { pickMode = true; }, [&]() { pickMode = true; }},
      {[&]() {armyPhaseSelect[0] = 0;
          armyPhaseSelect[1] = 0;
          pickTroop[0] = 0;
          pickTroop[1] = 0;
          pickMode=false;
          armySelected = "";
          armyPhase = 0; }}
          };

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
  loopArmyPhase2 = [&]() { phase2 = std::async(std::launch::async, [&]() {
                             terminatePhase2 = false;
                             while (!terminatePhase2)
                             {
                               armyPhase2();
                               std::unique_lock<std::mutex> lock(phase2a);
                               terminatePhase2Cond.wait_for(lock, std::chrono::milliseconds(1000 / 10));
                             }
                           }); };
  stopLoopArmyPhase2 = [&]() {
    if (phase2.valid())
    {
      phase2b.lock();
      terminatePhase2 = true;
      phase2b.unlock();
      terminatePhase2Cond.notify_all();
      phase2.get();
    }
  };

  armyPhase0 = [&]() {
    this->lg3.lock();
    actionPhase0 = {{[&]() {
      armyPhaseSelect[0] = 0;
      armyPhaseSelect[1] = 0;
      moved[0] = 0;
      moved[0] = 0;
      armyPhase = 1;
    }}};
    
    for (auto &i : this->army->total)
    {
      actionPhase0.push_back({});
      if (i.second->inBattle)
        actionPhase0.back().push_back([i, &retreatArmy]() {
          retreatArmy(i.first);
        });
      else
      {
        actionPhase0.back().push_back([&, i]() {
          armyPhaseSelect[0] = 0;
          armyPhaseSelect[1] = 0;
          moved[0] = 0;
          moved[0] = 0;
          armySelected = i.first;
          armyPhase = 2;
        });
        actionPhase0.back().push_back([&, i]() {
          armyPhaseSelect[0] = 0;
          armyPhaseSelect[1] = 0;
          moved[0] = 0;
          moved[0] = 0;
          removeArmy(i.first);
        });
      }
    }
    actionPhase0.push_back({[&]() {
      armyPhase = -999;
    }});


    std::vector<std::vector<int>> placement = {{1}};

    std::vector<std::string> armyName;
    std::vector<std::string> content = {};

    for (auto &i : this->army->total)
    {
      if (!i.second->removed && !i.second->lost)
      {
        placement.push_back({});
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
          placement.back().push_back(93);
        }
        else
        {
          content[content.size() - 5] += "   Edit   Remove";
          placement.back().push_back(93);
          placement.back().push_back(100);
        }
      }
    }
    placement.push_back({1});

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

    armyPhaseSelect[0] = (armyPhaseSelect[0] + placement.size()) % placement.size();
    armyPhaseSelect[1] = (armyPhaseSelect[1] + placement[armyPhaseSelect[0]].size()) % placement[armyPhaseSelect[0]].size();

    int range[2] = {1, displayContent[0] - 2};

    if (inputMode == 1 && armyPhaseSelect[0] >= range[0] && armyPhaseSelect[0] <= range[1])
    {
      if ((armyPhaseSelect[0] - range[0]) * 5 < moved[0])
        armyPhaseSelect[0] = std::ceil(moved[0] / 5.0) + range[0];
      else if (moved[0] + displayRange[0] < (armyPhaseSelect[0] - range[0]) * 5 + 1)
        armyPhaseSelect[0] = std::ceil((moved[0] + displayRange[0]) / 5.0) - 1 + range[0];

      armyPhaseSelect[0] = (armyPhaseSelect[0] + placement.size()) % placement.size();
      armyPhaseSelect[1] = (armyPhaseSelect[1] + placement[armyPhaseSelect[0]].size()) % placement[armyPhaseSelect[0]].size();
    }
    else if (inputMode == 0 && armyPhaseSelect[0] >= range[0] && armyPhaseSelect[0] <= range[1])
    {
      if ((armyPhaseSelect[0] - range[0]) * 5 < moved[0])
        moved[0] = (armyPhaseSelect[0] - range[0]) * 5;
      else if (moved[0] + displayRange[0] < (armyPhaseSelect[0] - range[0]) * 5 + 1)
        moved[0] = (armyPhaseSelect[0] - range[0]) * 5 + 5 - displayRange[0];

      moved[0] = std::min(std::max(0, moved[0]), movableRange[0]);
      moved[1] = std::min(std::max(0, moved[1]), movableRange[1]);
    }

    if (armyPhaseSelect[0] != placement.size() - 1 && armyPhaseSelect[0] >= range[0] && armyPhaseSelect[0] <= range[1])
      content[(armyPhaseSelect[0] - range[0]) * 5].replace(placement[armyPhaseSelect[0]][armyPhaseSelect[1]], 1, ">");

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
    std::cout << (armyPhaseSelect[0] == placement.size() - 1 ? " > " : "   ") << underline("Back", "green") << " (or spacebar)" << std::endl;

    this->lg3.unlock();
  };

  armyPhase1 = [&]() {
    bool unique = false;

    std::cout << "\033[" << 1 << ";" << 1 << "H";
    std::cout << color("Enter an unique army name (max. 10 characters, only letters, numbers and underscore):", "green") << std::endl;

    int c = 0;
    std::string input = "";
    while (1)
    {
      std::string temp;
      getline(std::cin, temp);

      this->lg3.lock();
      if (!std::regex_match(temp, std::regex("^[a-zA-Z0-9_]+$")))
      {
        std::cout << "\033[1A\033[K";
        std::cout << std::endl
                  << std::endl;
        std::cout << "Only letters and numbers are allowed!";
        std::cout << "\033[2;" + std::to_string(input.size() + 1) + "f";
      }
      else if (this->army->total.count(input) != 0)
      {
        std::cout << "\033[1A\033[K";
        std::cout << std::endl
                  << std::endl;
        std::cout << "Army name already exists!";
        std::cout << "\033[2;" + std::to_string(input.size() + 1) + "f";
      }
      else
      {
        input = temp;
        this->lg3.unlock();
        break;
      };

      this->lg3.unlock();
    }

    this->lg3.lock();
    this->army->total[input] = new ArmyUnit(input);
    this->lg3.unlock();
    armySelected = input;

    armyPhase = 2;

    std::cout << "\033[2J\033[1;1H";
    (*armyPhaseF[armyPhase])();
  };

  armyPhase2 = [&]() {
    this->lg3.lock();
    assert(this->army->total.count(armySelected) != 0);

    std::stringstream food;
    food << std::fixed << std::setprecision(1) << this->army->total[armySelected]->totalFoodRequired;
    std::stringstream equipment;
    equipment << std::fixed << std::setprecision(1) << this->army->total[armySelected]->totalEquipmentRequired;


    std::cout << "\033[2J\033[1;1H";
    std::cout << color("Editing Army ("+armySelected + ")", "magenta") << std::endl
              << std::endl;

    std::cout << color("Overview:", "green") << std::endl
              <<"Battle Plan: " + (this->army->total[armySelected]->battlePlanAssigned ? this->army->total[armySelected]->battlePlanName : "none")
              << "   Battling Region: " + (this->army->total[armySelected]->inBattle ? this->army->total[armySelected]->battleRegion.first + ", " + this->army->total[armySelected]->battleRegion.second : "none")
              << "   Total food: " + food.str()
              << "   Total equipment: " + equipment.str() << std::endl
              << std::endl;
    this->lg3.unlock();
  };

  char input;
  std::cout << "\033[2J\033[1;1H";
  (*armyPhaseF[armyPhase])();
  while (1)
  {
    int prevArmyPhase = armyPhase;

    input = getch();

    if (input == '\033')
    {
      getch();

      this->lg3.lock();

      if (pickMode)
      {
        switch (getch())
        {
        case 'A':
          pickTroop[0]--;
          break;
        case 'B':
          pickTroop[0]++;
          break;
        }
      }
      else
      {
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
      }

      this->lg3.unlock();
    }
    else if (input == ' ')
      armyPhase = -999;
    else if (input == '\n')
    {
      if(armyPhase == 0)actionPhase0[armyPhaseSelect[0]][armyPhaseSelect[1]]();
      else if(armyPhase == 2){
        if(pickMode) actionPhase2Troop[pickTroop[0]][pickTroop[1]]();
        else actionPhase2Pick[armyPhaseSelect[0]][armyPhaseSelect[1]]();
      }
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

    stopLoopArmyPhase0();
    stopLoopArmyPhase2();

    std::cout << "\033[2J\033[1;1H";
    if (armyPhase == -999)
    {
      gamePhase = prevPhase;

      this->gamePhaseSelect[0] = 0;
      this->gamePhaseSelect[1] = 0;
      (this->*this->print[this->gamePhase])(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
      break;
    }
    else
    {
      (*armyPhaseF[armyPhase])();
    }
  }
}