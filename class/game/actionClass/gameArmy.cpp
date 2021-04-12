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
#include "../gameStruct.h"
#include "../../../data/troop/troop.h"

// the structure is similar to that of game
// reason why it is not done in game is because the number of armies are dynamic and it would be messy to deal with it in game

// flow:
// armyphase 0: prints all armies with the ability to scroll
// phase 1: create new army
// phase 2: edit army

// scrolling feature is implemented by repainting the terminal
// I have a fixed size vector that contains the same amount of chars to be printed everytime (may change after user action, but the terminal will be cleared after any user action and army will not disappear on its own)
// every frame I will output the char vector and print cover the old one
void Game::gameArmy(int &currentPhase, int prevPhase)
{
  this->lguser.unlock();

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

  std::function<void(ArmyUnit *)> removeArmy;
  std::function<void(ArmyUnit *)> retreatArmy;
  std::function<void(int *, std::string)> posTroop;
  std::function<void(int *)> removeTroop;

  std::vector<std::function<void()> *> armyPhaseF = {&loopArmyPhase0, &armyPhase1, &loopArmyPhase2};
  std::vector<std::vector<std::function<void()>>> actionPhase0 = {};

  int moved[2] = {0, 0};

  std::vector<std::vector<std::function<void()>>> actionPhase2Pick = {
      {[&]() {if(armyPhaseSelect[0] > 3) return;posTroop(armyPhaseSelect,posToTroop[0]);pickMode=false; }, [&]() {if(armyPhaseSelect[0] > 3) return;removeTroop(armyPhaseSelect);pickMode=false; }},
      {[&]() {if(armyPhaseSelect[0] > 3) return;posTroop(armyPhaseSelect,posToTroop[1]);pickMode=false; }},
      {[&]() {if(armyPhaseSelect[0] > 3) return;posTroop(armyPhaseSelect,posToTroop[2]);pickMode=false; }},
      {[&]() {if(armyPhaseSelect[0] > 3) return;posTroop(armyPhaseSelect,posToTroop[3]);pickMode=false; }},
      {[&]() {if(armyPhaseSelect[0] > 3) return;posTroop(armyPhaseSelect,posToTroop[4]);pickMode=false; }},
      {[&]() {if(armyPhaseSelect[0] > 3) return;posTroop(armyPhaseSelect,posToTroop[5]);pickMode=false; }},
      {[&]() {if(armyPhaseSelect[0] > 3) return;posTroop(armyPhaseSelect,posToTroop[6]);pickMode=false; }},
      {[&]() {if(armyPhaseSelect[0] > 3) return;posTroop(armyPhaseSelect,posToTroop[7]);pickMode=false; }},
      {[&]() {if(armyPhaseSelect[0] > 3) return;posTroop(armyPhaseSelect,posToTroop[8]);pickMode=false; }},
      {[&]() {if(armyPhaseSelect[0] > 3) return;posTroop(armyPhaseSelect,posToTroop[9]);pickMode=false; }},
      {[&]() {if(armyPhaseSelect[0] > 3) return;posTroop(armyPhaseSelect,posToTroop[10]);pickMode=false; }},
      {[&]() {if(armyPhaseSelect[0] > 3) return;posTroop(armyPhaseSelect,posToTroop[11]);pickMode=false; }},
      {[&]() {if(armyPhaseSelect[0] > 3) return;posTroop(armyPhaseSelect,posToTroop[12]);pickMode=false; }},
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
          armyPhase = 0; }}};

  loopArmyPhase0 = [&]() { phase0 = std::async(std::launch::async, [&]() {
                             terminatePhase0 = false;
                             while (!terminatePhase0 && !this->gameOver)
                             {
                               armyPhase0();
                               std::unique_lock<std::mutex> lock(phase0a);
                               terminatePhase0Cond.wait_for(lock, std::chrono::milliseconds(1000 / this->fps));
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
                             while (!terminatePhase2 && !this->gameOver)
                             {
                               armyPhase2();
                               std::unique_lock<std::mutex> lock(phase2a);
                               terminatePhase2Cond.wait_for(lock, std::chrono::milliseconds(1000 / this->fps));
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
      if (this->army->total.size() == 10)
        return;
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
          retreatArmy(i.second);
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
          removeArmy(i.second);
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
            content.back() += "None" + std::string(25 - 4, ' ');
          }
          else
          {
            content.back() += typeToDisplay[i.second->formation[k][j]->type] + "(";

            // casting down from double to int
            int hp = i.second->formation[k][j]->getHealth();
            int baseHp = typeToHp[i.second->formation[k][j]->type];
            content.back() += std::to_string(hp) + "/" + std::to_string(baseHp) + ")";
            content.back() += std::string((j == 0 ? 12 : 0) + (4 - k) * 25 - content.back().length(), ' ');
          }
        }
      }
      content.push_back("");
      if (i.second->inBattle)
      {
        content[content.size() - 5] += "   Retreat";
        placement.back().push_back(12 + 25 * 4 + 1);
      }
      else
      {
        content[content.size() - 5] += "   Edit   Remove";
        placement.back().push_back(12 + 25 * 4 + 1);
        placement.back().push_back(12 + 25 * 4 + 8);
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
              << "   Camps: " << this->resource->campUsed << "/" << this->resource->camp
              << "   Airports: " << this->resource->airportUsed << "/" << this->resource->airport
              << std::endl
              << std::endl;
    std::cout << (armyPhaseSelect[0] == 0 ? " > " : "   ") + underline("New Army", "green") << std::endl;

    int displayContent[2] = {(int)content.size(), 12 + 25 * 4 + 16};
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
        std::cout << "\033[K"
                  << "Only letters and numbers are allowed!";
        std::cout << "\033[2;1f";
      }
      else if (this->army->total.count(temp) != 0)
      {
        std::cout << "\033[1A\033[K";
        std::cout << std::endl
                  << std::endl;
        std::cout << "\033[K"
                  << "Army name already exists!";
        std::cout << "\033[2;1f";
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

    std::stringstream food;
    food << std::fixed << std::setprecision(1) << this->army->total[armySelected]->totalFoodRequired;
    std::stringstream equipment;
    equipment << std::fixed << std::setprecision(1) << this->army->total[armySelected]->totalEquipmentRequired;

    std::cout << "\033[2J\033[1;1H";
    std::cout << color("Editing Army (" + armySelected + ")", "magenta") << std::endl
              << std::endl;

    std::cout << color("Overview:", "green") << std::endl
              << "Battling Region: " + (this->army->total[armySelected]->inBattle ? this->army->total[armySelected]->battleRegion.first + ", " + this->army->total[armySelected]->battleRegion.second : "none")
              << "   Total food: " + food.str()
              << "   Total equipment: " + equipment.str() << std::endl
              << std::endl;

    std::cout << std::setw(25 + 11) << color("   Column D", "green")
              << std::setw(25 + 11) << color("   Column C", "green")
              << std::setw(25 + 11) << color("   Column B", "green")
              << std::setw(25 + 11) << color("   Column A", "green")
              << std::string(15, ' ') + color("   Troop Type", "green") << std::endl;

    if (pickMode)
    {
      pickTroop[0] = (pickTroop[0] + prefixPhase2Troop.size()) % prefixPhase2Troop.size();
      pickTroop[1] = (pickTroop[1] + prefixPhase2Troop[pickTroop[0]].size()) % prefixPhase2Troop[pickTroop[0]].size();
      prefixPhase2Troop[pickTroop[0]][pickTroop[1]].replace(1, 1, ">");
    }
    else
    {
      armyPhaseSelect[0] = (armyPhaseSelect[0] + prefixPhase2Pick.size()) % prefixPhase2Pick.size();
      armyPhaseSelect[1] = (armyPhaseSelect[1] + prefixPhase2Pick[armyPhaseSelect[0]].size()) % prefixPhase2Pick[armyPhaseSelect[0]].size();

      prefixPhase2Pick[armyPhaseSelect[0]][armyPhaseSelect[1]].replace(1, 1, ">");
    }

    for (int i = 0; i < 4; i++)
    {
      for (int j = 3; j >= 0; j--)
      {
        if (this->army->total[armySelected]->formation[j][i] == NULL)
          std::cout << std::setw(25) << prefixPhase2Pick[i][3-j] + "None";
        else
          std::cout << std::setw(25) << prefixPhase2Pick[i][3-j] + typeToDisplay[this->army->total[armySelected]->formation[j][i]->type] + " (" + std::to_string((int)this->army->total[armySelected]->formation[j][i]->getHealth()) + "/" + std::to_string(typeToHp[this->army->total[armySelected]->formation[j][i]->type]) + ")";
      }
      std::cout << std::string(15, ' ') << std::setw(18) << prefixPhase2Troop[i][0] + typeToDisplay[posToTroop[i]] << " (" + std::to_string(this->troop->helper[posToTroop[i]](0)) + ")" + " (shortcut: " + std::to_string(i + 1) + ")";
      if (i == 0)
        std::cout << prefixPhase2Troop[0][1] << underline("Remove", "green") << " (shortcut: 0)";
      std::cout << std::endl;
    }
    std::cout << std::string(25 * 4 + 15, ' ') << std::setw(18) << prefixPhase2Troop[4][0] + typeToDisplay[posToTroop[4]] << " (" + std::to_string(this->troop->helper[posToTroop[4]](0)) + ")"
              << " (shortcut: 4)" << std::endl;
    std::cout << std::setw(25 * 4 + 11 + 15) << prefixPhase2Pick[4][0] + underline("Back", "green") + " (Or spacebar)" << std::setw(18) << prefixPhase2Troop[5][0] + typeToDisplay[posToTroop[5]] << " (" + std::to_string(this->troop->helper[posToTroop[5]](0)) + ")"
              << " (shortcut: 5)" << std::endl;
    for (int i = 6; i < 9; i++)
      std::cout << std::string(25 * 4 + 15, ' ') << std::setw(18) << prefixPhase2Troop[i][0] + typeToDisplay[posToTroop[i]] << " (" + std::to_string(this->troop->helper[posToTroop[i]](0)) + ")"
                << " (shortcut: " + std::to_string(i + 1) + ")" << std::endl;
    std::cout << std::string(25 * 4 + 15, ' ') << std::setw(18) << prefixPhase2Troop[9][0] + typeToDisplay[posToTroop[9]] << " (" + std::to_string(this->troop->helper[posToTroop[9]](0)) + ")"
              << " (shortcut: q)" << std::endl;
    std::cout << std::string(25 * 4 + 15, ' ') << std::setw(18) << prefixPhase2Troop[10][0] + typeToDisplay[posToTroop[10]] << " (" + std::to_string(this->troop->helper[posToTroop[10]](0)) + ")"
              << " (shortcut: w)" << std::endl;
    std::cout << std::string(25 * 4 + 15, ' ') << std::setw(18) << prefixPhase2Troop[11][0] + typeToDisplay[posToTroop[11]] << " (" + std::to_string(this->troop->helper[posToTroop[11]](0)) + ")"
              << " (shortcut: e)" << std::endl;
    std::cout << std::string(25 * 4 + 15, ' ') << std::setw(18) << prefixPhase2Troop[12][0] + typeToDisplay[posToTroop[12]] << " (" + std::to_string(this->troop->helper[posToTroop[12]](0)) + ")"
              << " (shortcut: r)" << std::endl;

    std::cout << std::endl;
    std::cout << std::string(23 * 4 + 15, ' ') << prefixPhase2Troop[13][0] << underline("Back to Army", "green") << std::endl;
    this->lg3.unlock();
  };

  posTroop = [&](int *pos, std::string type) {
    this->lg3.lock();
    if (this->troop->helper[type](0) == 0)
    {
      this->lg3.unlock();
      return;
    }
    if (type == "logistic" && this->army->total[armySelected]->logisticCount == 2)
    {
      this->lg3.unlock();
      return;
    }
    this->army->total[armySelected]->addTroop(3-pos[1],pos[0], type, this->troop, this->resource);
    this->lg3.unlock();
  };
  removeTroop = [&](int *pos) {
    this->lg3.lock();
    this->army->total[armySelected]->removeTroop(3-pos[1],pos[0], this->troop, this->resource);
    this->lg3.unlock();
  };

  removeArmy = [&](ArmyUnit *army) {
    for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++)
        army->removeTroop(i, j, this->troop, this->resource);
    std::string name = army->name;
    delete army;
    this->army->total.erase(name);
  };

  retreatArmy = [&](ArmyUnit *army) {
    int index = -1;
    int index2[2] = {-1, -1};

    this->lg3.lock();

    for (int i = 0; i < this->enemies->totalEnemies.size(); i++)
      if (this->enemies->totalEnemies[i]->name == army->battleRegion.first)
      {
        index = i;
        break;
      }

    assert(index != -1);

    for (int i = 0; i < this->enemies->totalEnemies[index]->map.size(); i++)
    {
      for (int j = 0; j < this->enemies->totalEnemies[index]->map[i].size(); j++)
      {
        if (this->enemies->totalEnemies[index]->map[i][j]->name == army->battleRegion.second)
        {
          index2[0] = i;
          index2[1] = j;
        }
      }
    }

    assert(index2[0] != -1);

    this->enemies->totalEnemies[index]->map[index2[0]][index2[1]]->retreat(army);

    this->lg3.unlock();
  };

  char input;
  std::cout << "\033[2J\033[1;1H";
  (*armyPhaseF[armyPhase])();
  while (1)
  {
    input = getch();
    if (this->gameOver)
    {
      stopLoopArmyPhase0();
      stopLoopArmyPhase2();
      break;
    }

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
        case 'C':
          pickTroop[1]++;
          break;
        case 'D':
          pickTroop[1]--;
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
    {
      if (armyPhase == 0)
        armyPhase = -999;
      else if (armyPhase == 2)
        armyPhase = 0;
    }

    else if (input == '\n')
    {
      if (armyPhase == 0)
        actionPhase0[armyPhaseSelect[0]][armyPhaseSelect[1]]();
      else if (armyPhase == 2)
      {
        if (pickMode)
          actionPhase2Pick[pickTroop[0]][pickTroop[1]]();
        else
          actionPhase2Troop[armyPhaseSelect[0]][armyPhaseSelect[1]]();
      }
    } // reset terminal
    else if (input == '/')
      std::cout << "\033c" << std::endl;
    else
    {

      if (armyPhase == 0)
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
      else if (armyPhase == 2)
      {
        switch (input)
        {
        case '0':
          actionPhase2Pick[0][1]();
          break;
        case '1':
          actionPhase2Pick[0][0]();
          break;
        case '2':
          actionPhase2Pick[1][0]();
          break;
        case '3':
          actionPhase2Pick[2][0]();
          break;
        case '4':
          actionPhase2Pick[3][0]();
          break;
        case '5':
          actionPhase2Pick[4][0]();
          break;
        case '6':
          actionPhase2Pick[5][0]();
          break;
        case '7':
          actionPhase2Pick[6][0]();
          break;
        case '8':
          actionPhase2Pick[7][0]();
          break;
        case '9':
          actionPhase2Pick[8][0]();
          break;
        case 'q':
          actionPhase2Pick[9][0]();
          break;
        case 'w':
          actionPhase2Pick[10][0]();
          break;
        case 'e':
          actionPhase2Pick[11][0]();
          break;
        case 'r':
          actionPhase2Pick[12][0]();
          break;
        };
      }
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