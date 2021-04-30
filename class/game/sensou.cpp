#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <iomanip>
#include <functional>
#include <utility>
#include <cmath>
#include <condition_variable>
#include <mutex>
#include <chrono>
#include <iterator>
#include <thread>
#include <algorithm>
#include <cassert>
#include <map>
#include <climits>

#include "game.h"
#include "gameStruct.h"
#include "../../data/troop/troop.h"
#include "../../io/io.h"

// flow: print the page for current mode for every frame

//       wait for user actions (switch country, send troops, retreat troops)
//       naturally occuring: when country capitulates
//       naturally occuring: when all user's troops died

//       battle: calc all stats for both side -> user troops give damage -> user troops take damage -> clean up troops with no health
//       when adding in troops, they will join the battle on the next day

// printing technique: repaint every character, no screen clearing
// default page render: 4 renders, each corresponding to the 4 columns (map, actions, rewards, enemy troops)
// troop withdraw page: sorted in ascending order of health, then alphanumerical order of battling region

// note the actual damage calculation and battle progression happens in the timer thread in game.cpp and in the BattleUnit class in gameStruct.cpp
// this cpp file is merely a representation and interface of the battle
void Game::sensou(int &gamePhase, int prevPhase)
{

  int currentCountry = 0;

  for (int i = 0; i < this->enemies->totalEnemies.size(); i++)
    if (!this->enemies->totalEnemies[i]->capitulated && this->enemies->totalEnemies[i]->capturedLand > 0)
    {
      currentCountry = i;
      break;
    }
  // 0: map mode, 0: block action (0b), 1: add troop, 1: add troop detail (2b), 3: retreat all troops, 2: view battle

  int mode = 0;

  int subMode = 0;

  int blockSize[2] = {4, 6};

  // io

  int prePhase0[2] = {0, 0};

  int scroll0[2] = {0, 0};

  int phase0[2] = {0, 0};

  int subPhase0[2] = {0, 0};

  int subPhase0Mode = 0;

  int scroll1[2] = {0, 0};

  int phase1[2] = {0, 0};

  int subPhase1[2] = {0, 0};

  int subPhase1Mode = 0;

  int scroll2[2] = {0, 0};

  int subPhase2Mode = 0;

  std::string subPhase1Type;

  TroopUnit *subPhase1TroopDetail;

  bool enter = false;

  std::vector<std::function<void()>> printMode;

  std::function<void()> sendAll;

  std::function<void()> retreatAll;

  std::function<void(std::string)> troopIreru;

  std::function<void(ArmyUnit *)> armyIreru;

  std::function<void(std::string)> troopNuku;

  std::function<void(ArmyUnit *)> armyNuku;

  std::vector<TroopUnit *> selectedTroop;

  std::vector<ArmyUnit *> selectedArmy;

  std::function<void()> deselectAll;

  std::function<void()> loopPrint;
  std::function<void()> stopPrint;

  std::future<void> printFuture;

  std::condition_variable printCond;

  std::mutex printa;

  std::mutex printb;

  std::mutex user;

  bool terminatePrint = false;


  int battleCycle = 0;

  const int iconInterval = 300;

  int iconLocation = 0;

  std::unordered_map<std::string, int> typeToHealth = {
      {"infantry", Infantry::baseHp},
      {"calvary", Calvary::baseHp},
      {"suicideBomber", 0},
      {"artillery", Artillery::baseHp},
      {"logistic", Logistic::baseHp},
      {"armoredCar", ArmoredCar::baseHp},
      {"tank1", Tank1::baseHp},
      {"tank2", Tank2::baseHp},
      {"tankOshimai", TankOshimai::baseHp},
      {"cas", Cas::baseHp},
      {"fighter", Fighter::baseHp},
      {"bomber", Bomber::baseHp},
      {"kamikaze", 0}};

  std::vector<std::string> indexToTroop = {"infantry", "calvary", "suicideBomber", "artillery", "logistic", "armoredCar", "tank1", "tank2", "tankOshimai", "cas", "fighter", "bomber", "kamikaze"};

  std::unordered_map<std::string, std::string> typeToDisplay = {
      {"infantry", "Infantry"},
      {"calvary", "Calvary"},
      {"suicideBomber", "Suicide Bomber"},
      {"artillery", "Artillery"},
      {"logistic", "Logistic"},
      {"armoredCar", "Armored Car"},
      {"tank1", "Tank 1"},
      {"tank2", "Tank 2"},
      {"tankOshimai", "Tank Oshimai"},
      {"cas", "Cas"},
      {"fighter", "Fighter"},
      {"bomber", "Bomber"},
      {"kamikaze", "Kamikaze"}};

  std::map<std::string, bool> selectedArmyMap;

  std::unordered_map<std::string, int> selectedTroopMap = {
      {"infantry", 0},
      {"calvary", 0},
      {"suicideBomber", 0},
      {"artillery", 0},
      {"logistic", 0},
      {"armoredCar", 0},
      {"tank1", 0},
      {"tank2", 0},
      {"tankOshimai", 0},
      {"cas", 0},
      {"fighter", 0},
      {"bomber", 0},
      {"kamikaze", 0}};

  retreatAll = [&]() {
    Block *ptr = this->enemies->totalEnemies[currentCountry]->map[phase0[0]][phase0[1]];
    ptr->retreatAll(this->battle);
  };

  deselectAll = [&]() {
    for (auto i : selectedTroopMap)
      for (int j = 0; j < i.second; j++)
        troopNuku(i.first);

    for (int i = selectedArmy.size() - 1; i >= 0; i--)
      armyNuku(selectedArmy[i]);
  };

  sendAll = [&]() {
    user.lock();

    Enemy *ptr = this->enemies->totalEnemies[currentCountry];
    for (auto i : selectedTroop)
      if (!ptr->map[phase0[0]][phase0[1]]->captured)
        ptr->map[phase0[0]][phase0[1]]->reinforce(
            user, [&]() { this->endGame(); }, this->gameOver, this->enemies->totalEnemies.size(), i, this->resource, this->building, this->battle, [&](std::string type, int time, std::function<void(data::Resource &)> &callBack, std::string desc, double land, int amount) { this->buildBase(type, time, callBack, desc, land, amount); });

    for (auto i : selectedArmy)
      if (!ptr->map[phase0[0]][phase0[1]]->captured)
        ptr->map[phase0[0]][phase0[1]]->reinforce(
            user, [&]() { this->endGame(); }, this->gameOver, this->enemies->totalEnemies.size(), i, this->resource, this->building, this->battle, [&](std::string type, int time, std::function<void(data::Resource &)> &callBack, std::string desc, double land, int amount) { this->buildBase(type, time, callBack, desc, land, amount); });

    deselectAll();

    mode = 0;

    if (this->gameOver)
    {
      user.unlock();
      return;
    }

    std::cout << "\033[2J\033[1;1H" << std::endl;
    // avoid resource deadlock error
    std::thread temp([&]() {
      stopPrint();
      loopPrint();
    });
    temp.detach();
  };

  troopIreru = [&](std::string type) {
    std::vector<int> indices;
    for (int i = 0; i < this->troop->allTroop.size(); i++)
    {
      if (this->troop->allTroop[i]->type == type && this->troop->allTroop[i]->state["free"] && !this->troop->allTroop[i]->selected)
      {
        indices.push_back(i);
      }
    }
    assert(indices.size() != 0);


    std::sort(indices.begin(), indices.end(), [this](int a, int b) -> bool {
      return this->troop->allTroop[a]->getHealth() > this->troop->allTroop[b]->getHealth();
    });


    this->troop->allTroop[indices[0]]->selected = true;
    selectedTroop.push_back(this->troop->allTroop[indices[0]]);
    selectedTroopMap[type]++;
  };

  armyIreru = [&](ArmyUnit *guntai) {
    assert(guntai != NULL);

    selectedArmy.push_back(guntai);

    selectedArmyMap[guntai->name] = true;
  };

  troopNuku = [&](std::string type) {

    std::vector<int> indices;

    for (int i = 0; i < selectedTroop.size(); i++)
    {
      if (selectedTroop[i]->type == type)
      {
        indices.push_back(i);
      }
    }

    if (indices.size() == 0)
      return;

    std::sort(indices.begin(), indices.end(), [&selectedTroop](int a, int b) -> bool {
      return selectedTroop[a]->getHealth() < selectedTroop[b]->getHealth();
    });

    selectedTroop[indices[0]]->selected = false;
    selectedTroopMap[selectedTroop[indices[0]]->type]--;
    selectedTroop.erase(selectedTroop.begin() + indices[0]);
  };

  armyNuku = [&](ArmyUnit *guntai) {
    assert(guntai != NULL);


    int index = -1;

    for (int i = 0; i < selectedArmy.size(); i++)
      if (selectedArmy[i] == guntai)
      {
        index = i;
        break;
      }

    assert(index != -1);

    selectedArmy.erase(selectedArmy.begin() + index);
    selectedArmyMap.erase(guntai->name);
  };

  printMode.push_back([&]() {
    if (enter)
    {
      enter = false;
      if (subMode == 1)
      {
        if (subPhase0[0] == 0)
          mode = 1;

        else if (subPhase0[0] == 2)
          mode = 2;

        else
          retreatAll();
        subMode = 0;

        user.lock();

        std::cout << "\033[2J\033[1;1H" << std::endl;
        // avoid resource deadlock error
        std::thread temp([&]() {
          stopPrint();
          loopPrint();
        });
        temp.detach();

        return;
      }
      else
      {
        Enemy *ptr = this->enemies->totalEnemies[currentCountry];
        if ((!this->battle->inBattle || this->battle->countryBattling == ptr->name) && !ptr->map[phase0[0]][phase0[1]]->captured && ptr->map[phase0[0]][phase0[1]]->isAttackable && !ptr->capitulated)
          subMode = 1;
      }
    }

    this->lg3.lock();

    int screen[2] = {25, 48};

    std::vector<std::string> render;
    Enemy *ptr = this->enemies->totalEnemies[currentCountry];

    battleCycle += 1000 / this->fps;

    if (battleCycle >= iconInterval)
    {
      battleCycle = 0;
      iconLocation = !iconLocation;
    }

    phase0[0] = (phase0[0] + ptr->map.size()) % ptr->map.size();

    std::unordered_map<int, int> hasBlock;

    int start = -1;

    int end = -1;

    int scrollable[2] = {std::max(0, (int)(ptr->map.size() * 3 + 1 - screen[0])), std::max(0, (int)(ptr->map[0].size() * 6 - screen[1]))};

    scroll0[0] = std::min(std::max(scroll0[0], 0), scrollable[0]);

    scroll0[1] = std::min(std::max(scroll0[1], 0), scrollable[1]);

    if (subPhase0Mode == 1)
    {
      if (scroll0[0] > phase0[0] * 3)
        phase0[0] = scroll0[0] / 3 + 1;

      if (scroll0[0] + screen[0] <= phase0[0] * 3 + 1)
        phase0[0] = (scroll0[0] + screen[0]) / 3 - 1;

      if (scroll0[1] > phase0[1] * 6)
        phase0[1] = scroll0[1] / 6 + 1;

      if (scroll0[1] + screen[1] <= phase0[1] * 6 + 1)
        phase0[1] = (scroll0[1] + screen[1]) / 6 - 1;
      phase0[0] = (phase0[0] + ptr->map.size()) % ptr->map.size();
    }

    else if (subPhase0Mode == 0)
    {

      if (scroll0[0] > phase0[0] * 3)
        scroll0[0] = phase0[0] * 3;

      if (scroll0[0] + screen[0] <= phase0[0] * 3 + 1)
        scroll0[0] = phase0[0] * 3 - screen[0] + 3;
      scroll0[0] = std::min(std::max(scroll0[0], 0), scrollable[0]);
      scroll0[1] = std::min(std::max(scroll0[1], 0), scrollable[1]);
    }

    for (int i = 0; i < ptr->map[phase0[0]].size(); i++)
    {
      if (ptr->map[phase0[0]][i] != NULL)
      {
        hasBlock[i] = 1;
        if (start == -1)
          start = i;

        end = i;
      }
    }

    if (phase0[0] == prePhase0[0])
    {
      int diff = 0;

      if (phase0[1] > prePhase0[1])
      {
        while (hasBlock.count(phase0[1] + diff) == 0 && phase0[1] + diff < ptr->map[phase0[0]].size())
          diff++;

        if (phase0[1] + diff == ptr->map[phase0[0]].size())
          phase0[1] = start;

        else
          phase0[1] += diff;
      }

      else if (phase0[1] < prePhase0[1])
      {
        while (hasBlock.count(phase0[1] - diff) == 0 && phase0[1] - diff >= 0)
          diff++;

        if (phase0[1] - diff == -1)
          phase0[1] = end;

        else
          phase0[1] -= diff;
      }

      else
      {
        if (hasBlock.count(phase0[1]) == 0)
        {
          int diff = 1;
          bool found = false;
          while (!found)
          {
            if (hasBlock.count(phase0[1] + diff) != 0)
            {
              phase0[1] = phase0[1] + diff;
              found = true;
            }

            else if (hasBlock.count(phase0[1] - diff) != 0)
            {
              phase0[1] = phase0[1] - diff;
              found = true;
            }

            diff++;
          }
        }
      }
    }


    else
    {
      // find closest block
      if (hasBlock.count(phase0[1]) == 0)
      {
        int diff = 1;
        bool found = false;

        while (!found)
        {
          if (hasBlock.count(phase0[1] + diff) != 0)
          {
            phase0[1] = phase0[1] + diff;
            found = true;
          }

          else if (hasBlock.count(phase0[1] - diff) != 0)
          {
            phase0[1] = phase0[1] - diff;
            found = true;
          }
          diff++;
        }
      }
    }




    if (subPhase0Mode == 0)
    {
      if (scroll0[1] > phase0[1] * 6)
        scroll0[1] = phase0[1] * 6;
      if (scroll0[1] + screen[1] <= phase0[1] * 6 + 1)
        scroll0[1] = phase0[1] * 6 - screen[1] + 6;
      scroll0[0] = std::min(std::max(scroll0[0], 0), scrollable[0]);
      scroll0[1] = std::min(std::max(scroll0[1], 0), scrollable[1]);
    }

    subPhase0Mode = -1;

    prePhase0[0] = phase0[0];

    prePhase0[1] = phase0[1];

    for (int i = 0; i < ptr->map.size(); i++)
    {
      std::string row1 = "";
      std::string row2 = "";
      std::string row3 = "";

      for (int j = 0; j < ptr->map[i].size(); j++)
      {
        if (ptr->map[i][j] == NULL)
        {
          if (i != 0 && ptr->map[i - 1][j] != NULL)
            row1 += std::string(blockSize[1], '-');

          else
            row1 += std::string(blockSize[1], ' ');
          row2 += std::string(blockSize[1], ' ');
          row3 += std::string(blockSize[1], ' ');
        }

        else
        {
          if (i == 0 || ptr->map[i - 1][j] == NULL)
            row1 += std::string(blockSize[1], '-');

          else
            row1 += std::string(blockSize[1], ' ');

          if (j == 0 || ptr->map[i][j - 1] == NULL)
          {
            row2 += '|';
            row3 += '|';
          }

          else
          {
            row2 += ' ';
            row3 += ' ';
          }

          if (i == phase0[0] && j == phase0[1] && subMode == 0)
            row2 += ">";

          else
            row2 += " ";

          if (ptr->map[i][j]->name.length() < 3)
            row2 += ' ';
          row2 += ptr->map[i][j]->name;

          if (j == ptr->map[i].size() - 1 || ptr->map[i][j + 1] == NULL)
            row2 += '|';

          else
            row2 += ' ';

          if (i == phase0[0] && j == phase0[1] && subMode == 0)
            row3 += "> ";

          else
            row3 += "  ";

          std::string iconType = " ";

          if (ptr->map[i][j]->battling)
          {
            iconType = ".";
            bool heavilyInjured = false;

            for (auto i : ptr->map[i][j]->battle.back()->mikata->totalArmy)
              if (i->casualtyPercentage >= 0.7)
              {
                heavilyInjured = true;
                break;
              }

            if (heavilyInjured)
              iconType = "!";
          }

          if (ptr->map[i][j]->captured)
          {
            row3 += "xx";
          }

          else
          {
            if (!iconLocation)
              row3 += iconType + " ";

            else
              row3 += ' ' + iconType;
          }

          if (j == ptr->map[i].size() - 1 || ptr->map[i][j + 1] == NULL)
            row3 += '|';

          else
            row3 += ' ';
        }
      }
      assert(row1.length() == blockSize[1] * ptr->map[i].size());
      assert(row2.length() == blockSize[1] * ptr->map[i].size());
      assert(row3.length() == blockSize[1] * ptr->map[i].size());
      render.push_back(row1);
      render.push_back(row2);
      render.push_back(row3);
    }

    // a layer unnderneath to compensate for the bottom border of the last row
    std::string row1 = "";
    for (int j = 0; j < ptr->map.back().size(); j++)
    {

      if (ptr->map.back()[j] != NULL)
        row1 += std::string(blockSize[1], '-');

      else
        row1 += std::string(blockSize[1], ' ');
    }

    assert(row1.length() == blockSize[1] * ptr->map.back().size());

    render.push_back(row1);

    for (int i = 0; i < render.size(); i++)
      if (render[i].size() < screen[1])
        render[i] += std::string(screen[1] - render[i].length(), ' ');

    for (int i = render.size(); i < screen[0]; i++)
      render.push_back(std::string(screen[1], ' '));

    int maxLength2 = 20;

    std::vector<std::string> render2;

    if ((!this->battle->inBattle || this->battle->countryBattling == ptr->name) && !ptr->map[phase0[0]][phase0[1]]->captured && ptr->map[phase0[0]][phase0[1]]->isAttackable && !ptr->capitulated)
      render2.push_back("   Send Troop (z)   ");

    if (ptr->map[phase0[0]][phase0[1]]->battling)
    {
      render2.push_back("   Retreat Troop (r)");
      render2.push_back("   View Battle (v)  ");
    }

    if (render2.size() > 0)
    {
      subPhase0[0] = (subPhase0[0] + render2.size()) % render2.size();
      subPhase0[1] = 0;
      if (subMode == 1)
        render2[subPhase0[0]].replace(1, 1, ">");
    }

    std::vector<std::string> render3;

    render3.push_back("Rewards");

    render3.push_back("Farm: " + std::to_string(ptr->map[phase0[0]][phase0[1]]->acquirable["farm"]));

    render3.push_back("Civilian Factory: " + std::to_string(ptr->map[phase0[0]][phase0[1]]->acquirable["civilianFactory"]));

    render3.push_back("Military Factory: " + std::to_string(ptr->map[phase0[0]][phase0[1]]->acquirable["militaryFactory"]));

    render3.push_back("Training Camp: " + std::to_string(ptr->map[phase0[0]][phase0[1]]->acquirable["trainingCamp"]));

    render3.push_back("Airport: " + std::to_string(ptr->map[phase0[0]][phase0[1]]->acquirable["airport"]));

    render3.push_back("Land: " + std::to_string(ptr->map[phase0[0]][phase0[1]]->acquirable["land"]));

    render3.push_back("");

    render3.push_back("");

    render3.push_back("Terrain");

    render3.push_back(ptr->map[phase0[0]][phase0[1]]->terrain);

    int maxLength3 = 0;

    for (auto i : render3)
      if (i.length() > maxLength3)
        maxLength3 = i.length();

    for (int i = 0; i < render3.size(); i++)
      render3[i] += std::string(maxLength3 - render3[i].length(), ' ');

    std::vector<std::string> render4;
    int soft = 0, hard = 0, air = 0;

    for (auto &i : ptr->map[phase0[0]][phase0[1]]->totalFoeTroop)
    {
      soft += i->getSoftAttack();
      hard += i->getHardAttack();
      air += i->getAirAttack();
    }

    for (auto &i : ptr->map[phase0[0]][phase0[1]]->totalFoeArmy)
      for (auto &j : i->formation)
        for (auto &k : j)
          if (k != NULL)
          {
            soft += k->getSoftAttack();
            hard += k->getHardAttack();
            air += k->getAirAttack();
          }
    render4.push_back("Enemies troops");

    render4.push_back("Infantry: " + std::to_string(ptr->map[phase0[0]][phase0[1]]->foeCount["infantry"]));

    render4.push_back("Calvary: " + std::to_string(ptr->map[phase0[0]][phase0[1]]->foeCount["calvary"]));

    render4.push_back("Artillery: " + std::to_string(ptr->map[phase0[0]][phase0[1]]->foeCount["artillery"]));

    render4.push_back("Logistic: " + std::to_string(ptr->map[phase0[0]][phase0[1]]->foeCount["logistic"]));

    render4.push_back("Armored Car: " + std::to_string(ptr->map[phase0[0]][phase0[1]]->foeCount["armoredCar"]));

    render4.push_back("Tank 1: " + std::to_string(ptr->map[phase0[0]][phase0[1]]->foeCount["tank1"]));

    render4.push_back("Tank 2: " + std::to_string(ptr->map[phase0[0]][phase0[1]]->foeCount["tank2"]));

    render4.push_back("Tank Oshimai: " + std::to_string(ptr->map[phase0[0]][phase0[1]]->foeCount["tankOshimai"]));

    render4.push_back("Cas: " + std::to_string(ptr->map[phase0[0]][phase0[1]]->foeCount["cas"]));

    render4.push_back("Fighter: " + std::to_string(ptr->map[phase0[0]][phase0[1]]->foeCount["fighter"]));

    render4.push_back("Bomber: " + std::to_string(ptr->map[phase0[0]][phase0[1]]->foeCount["bomber"]));

    render4.push_back("Armies: " + std::to_string(ptr->map[phase0[0]][phase0[1]]->foeCount["army"]));

    render4.push_back("");

    render4.push_back("");

    render4.push_back("Total base attack:");

    render4.push_back("Soft: " + std::to_string(soft));

    render4.push_back("Hard: " + std::to_string(hard));

    render4.push_back("Air: " + std::to_string(air));



    int maxLength4 = 0;
    for (auto i : render4)
      if (i.length() > maxLength4)
        maxLength4 = i.length();

    for (int i = 0; i < render4.size(); i++)
      render4[i] += std::string(maxLength4 - render4[i].length(), ' ');

    std::cout << "\033[1;1H";

    std::cout << color("Battle", "magenta") << std::endl
              << std::endl;

    this->lg.lock();

    std::stringstream speed;

    speed << std::fixed << std::setprecision(1) << this->setting["speed"] / 1000.0;

    std::cout << color("Day: ", "green") << this->day << "/" << this->timeLimit << " (" << speed.str() << "s)     " << (this->paused ? color("PAUSED", "red") : "") << std::endl
              << std::endl;

    this->lg.unlock();

    std::cout << "Country: " << this->enemies->totalEnemies[currentCountry]->name
              << " (" << (this->enemies->totalEnemies[currentCountry]->capitulated ? "    defeated" : "not defeated") << ")"
              << " (" << currentCountry + 1 << "/" << this->enemies->totalEnemies.size() << ")"
              << "   Battling Regions: " << this->enemies->totalEnemies[currentCountry]->battlingRegions
              << "   Captured: " << this->enemies->totalEnemies[currentCountry]->capturedLand << "/" << this->enemies->totalEnemies[currentCountry]->totalLand
              << std::endl;

    std::cout << "Change speed: q     Pause: p     Next country: e (total: " << this->enemies->totalEnemies.size() << ")     Move map: wasd     Back: spacebar " << std::endl
              << std::endl
              << std::endl;

    for (int i = 0; i < 19; i++)
    {
      std::cout << render[i + scroll0[0]].substr(scroll0[1], screen[1]) << "        "
                << (render2.size() > i ? render2[i] : std::string(maxLength2, ' ')) << "        "
                << (render3.size() > i ? render3[i] : std::string(maxLength3, ' ')) << "        "
                << (render4.size() > i ? render4[i] : std::string(maxLength4, ' ')) << std::endl;
    }

    for (int i = 19; i < screen[0]; i++)
      std::cout << render[i + scroll0[0]].substr(scroll0[1], screen[1]) << std::endl;

    this->lg3.unlock();
  });

  printMode.push_back([&]() {
    std::unordered_map<std::string, int> fullHealth = {
        {"infantry", 0},
        {"calvary", 0},
        {"artillery", 0},
        {"logistic", 0},
        {"armoredCar", 0},
        {"tank1", 0},
        {"tank2", 0},
        {"tankOshimai", 0},
        {"cas", 0},
        {"fighter", 0},
        {"bomber", 0}};

    if (enter)
    {
      enter = false;

      if (subMode == 1)
      {
        if (subPhase1TroopDetail != NULL)
        {
          if (subPhase1TroopDetail->selected)
          {
            subPhase1TroopDetail->selected = false;
            int index = -1;
            for (int i = 0; i < selectedTroop.size(); i++)
              if (selectedTroop[i] == subPhase1TroopDetail)
              {
                index = i;
                break;
              }
            assert(index != -1);
            selectedTroop.erase(selectedTroop.begin() + index);
            selectedTroopMap[subPhase1Type]--;
          }
          else
          {
            subPhase1TroopDetail->selected = true;
            selectedTroop.push_back(subPhase1TroopDetail);
            selectedTroopMap[subPhase1Type]++;
          }
        }
      }

      else
      {
        if (phase1[0] >= 0 && phase1[0] <= 12)
        {
          if (phase1[1] == 0 && this->troop->helper[indexToTroop[phase1[0]]](0) > 0 && selectedTroopMap[indexToTroop[phase1[0]]] < this->troop->helper[indexToTroop[phase1[0]]](0))
            troopIreru(indexToTroop[phase1[0]]);

          else if (phase1[1] == 1)
            troopNuku(indexToTroop[phase1[0]]);

          else if (phase1[1] == 2)
          {
            for (int i = selectedTroopMap[indexToTroop[phase1[0]]]; i < this->troop->helper[indexToTroop[phase1[0]]](0); i++)
              troopIreru(indexToTroop[phase1[0]]);
          }

          else if (phase1[1] == 3)
          {
            subMode = 1;
            subPhase1Type = indexToTroop[phase1[0]];
          }

          else if (phase1[1] == 4)
          {
            auto current = std::next(this->army->total.begin(), phase1[0]);
            bool found = false;

            for (auto i : selectedArmy)
            {
              if (i == current->second)
              {
                armyNuku(current->second);
                found = true;
                break;
              }
            }

            if (!found)
              armyIreru(current->second);
          }
        }

        else if (phase1[0] == 13)
        {
          if (phase1[1] == 0)
          {
            sendAll();
            return;
          }

          else
            deselectAll();
        }

        else
        {
          user.lock();
          std::cout << "\033[2J\033[1;1H" << std::endl;
          deselectAll();
          mode = 0;
          // avoid resource deadlock error
          std::thread temp([&]() {
            stopPrint();
            loopPrint();
          });
          temp.detach();
          return;
        }
      }
    }

    this->lg3.lock();

    std::cout << "\033[2J\033[1;1H";

    if (subMode == 0)
    {
      std::vector<std::vector<std::string>> prefix = {
          {"   ", "   ", "   ", "   "},
          {"   ", "   ", "   ", "   "},
          {"   ", "   ", "   ", "   "},
          {"   ", "   ", "   ", "   "},
          {"   ", "   ", "   ", "   "},
          {"   ", "   ", "   ", "   "},
          {"   ", "   ", "   ", "   "},
          {"   ", "   ", "   ", "   "},
          {"   ", "   ", "   ", "   "},
          {"   ", "   ", "   ", "   "},
          {"   ", "   ", "   ", "   "},
          {"   ", "   ", "   ", "   "},
          {"   ", "   ", "   ", "   "},
          {"   ", "   "},
          {"   "}};

      std::vector<std::map<std::string, ArmyUnit *>::iterator> ptr = {this->army->total.begin()};

      for (int i = 0; i < this->army->total.size(); i++)
      {
        if (!ptr.back()->second->inBattle)
          prefix[i].push_back("   ");
        ptr.push_back(std::next(ptr.back(), 1));
      }

      phase1[0] = (phase1[0] + prefix.size()) % prefix.size();
      phase1[1] = (phase1[1] + prefix[phase1[0]].size()) % prefix[phase1[0]].size();
      prefix[phase1[0]][phase1[1]].replace(1, 1, color(">", "cyan"));

      for (int i = 0; i < this->troop->allTroop.size(); i++)
      {
        if (this->troop->allTroop[i]->type != "suicideBomber" && this->troop->allTroop[i]->type != "kamikaze" && this->troop->allTroop[i]->getHealth() == typeToHealth[this->troop->allTroop[i]->type])
          fullHealth[this->troop->allTroop[i]->type]++;
      }

      std::cout << color("Send Troops", "magenta") << std::endl
                << std::endl;

      std::cout << std::setw(75 + 11) << color("Troop", "green") + " (full health, free, selected)" << color("Army", "green") << std::endl;

      std::cout << std::setw(30) << "Infantry:       " + std::to_string(fullHealth["infantry"]) + "/" + std::to_string(this->troop->helper["infantry"](0)) + "/" + std::to_string(selectedTroopMap["infantry"])
                << std::setw(75 + 11 * 4 - 30 + (phase1[1] < 4 && phase1[0] == 0 ? 11 : 0)) << prefix[0][0] + underline("+1", "green") + " (1)" + prefix[0][1] + underline("-1", "green") + prefix[0][2] + underline("All", "green") + prefix[0][3] + underline("Details", "green")
                << (this->army->total.size() > 0 ? (ptr[0]->first + " (" + std::to_string(ptr[0]->second->troopCount) + "/16)" + (ptr[0]->second->inBattle ? "" : prefix[0][4] + (selectedArmyMap.count(ptr[0]->first) == 0 ? underline("select", "green") : underline("deselect", "green")))) : "") << std::endl
                << std::setw(30) << "Calvary:        " + std::to_string(fullHealth["calvary"]) + "/" + std::to_string(this->troop->helper["calvary"](0)) + "/" + std::to_string(selectedTroopMap["calvary"])
                << std::setw(75 + 11 * 4 - 30 + (phase1[1] < 4 && phase1[0] == 1 ? 11 : 0)) << prefix[1][0] + underline("+1", "green") + " (2)" + prefix[1][1] + underline("-1", "green") + prefix[1][2] + underline("All", "green") + prefix[1][3] + underline("Details", "green")
                << (this->army->total.size() > 1 ? (ptr[1]->first + " (" + std::to_string(ptr[1]->second->troopCount) + "/16)" + (ptr[1]->second->inBattle ? "" : prefix[1][4] + (selectedArmyMap.count(ptr[1]->first) == 0 ? underline("select", "green") : underline("deselect", "green")))) : "") << std::endl
                << std::setw(30) << "Suicide Bomber: " + std::string("-/") + std::to_string(this->troop->helper["suicideBomber"](0)) + "/" + std::to_string(selectedTroopMap["suicideBomber"])
                << std::setw(75 + 11 * 4 - 30 + (phase1[1] < 4 && phase1[0] == 2 ? 11 : 0)) << prefix[2][0] + underline("+1", "green") + " (3)" + prefix[2][1] + underline("-1", "green") + prefix[2][2] + underline("All", "green") + prefix[2][3] + underline("Details", "green")
                << (this->army->total.size() > 2 ? (ptr[2]->first + " (" + std::to_string(ptr[2]->second->troopCount) + "/16)" + (ptr[2]->second->inBattle ? "" : prefix[2][4] + (selectedArmyMap.count(ptr[2]->first) == 0 ? underline("select", "green") : underline("deselect", "green")))) : "") << std::endl
                << std::setw(30) << "Artillery:      " + std::to_string(fullHealth["artillery"]) + "/" + std::to_string(this->troop->helper["artillery"](0)) + "/" + std::to_string(selectedTroopMap["artillery"])
                << std::setw(75 + 11 * 4 - 30 + (phase1[1] < 4 && phase1[0] == 3 ? 11 : 0)) << prefix[3][0] + underline("+1", "green") + " (4)" + prefix[3][1] + underline("-1", "green") + prefix[3][2] + underline("All", "green") + prefix[3][3] + underline("Details", "green")
                << (this->army->total.size() > 3 ? (ptr[3]->first + " (" + std::to_string(ptr[3]->second->troopCount) + "/16)" + (ptr[3]->second->inBattle ? "" : prefix[3][4] + (selectedArmyMap.count(ptr[3]->first) == 0 ? underline("select", "green") : underline("deselect", "green")))) : "") << std::endl
                << std::setw(30) << "Logistic:       " + std::to_string(fullHealth["logistic"]) + "/" + std::to_string(this->troop->helper["logistic"](0)) + "/" + std::to_string(selectedTroopMap["logistic"])
                << std::setw(75 + 11 * 4 - 30 + (phase1[1] < 4 && phase1[0] == 4 ? 11 : 0)) << prefix[4][0] + underline("+1", "green") + " (5)" + prefix[4][1] + underline("-1", "green") + prefix[4][2] + underline("All", "green") + prefix[4][3] + underline("Details", "green")
                << (this->army->total.size() > 4 ? (ptr[4]->first + " (" + std::to_string(ptr[4]->second->troopCount) + "/16)" + (ptr[4]->second->inBattle ? "" : prefix[4][4] + (selectedArmyMap.count(ptr[4]->first) == 0 ? underline("select", "green") : underline("deselect", "green")))) : "") << std::endl
                << std::setw(30) << "Armored Car:    " + std::to_string(fullHealth["armoredCar"]) + "/" + std::to_string(this->troop->helper["armoredCar"](0)) + "/" + std::to_string(selectedTroopMap["armoredCar"])
                << std::setw(75 + 11 * 4 - 30 + (phase1[1] < 4 && phase1[0] == 5 ? 11 : 0)) << prefix[5][0] + underline("+1", "green") + " (6)" + prefix[5][1] + underline("-1", "green") + prefix[5][2] + underline("All", "green") + prefix[5][3] + underline("Details", "green")
                << (this->army->total.size() > 5 ? (ptr[5]->first + " (" + std::to_string(ptr[5]->second->troopCount) + "/16)" + (ptr[5]->second->inBattle ? "" : prefix[5][4] + (selectedArmyMap.count(ptr[5]->first) == 0 ? underline("select", "green") : underline("deselect", "green")))) : "") << std::endl
                << std::setw(30) << "Tank 1:         " + std::to_string(fullHealth["tank1"]) + "/" + std::to_string(this->troop->helper["tank1"](0)) + "/" + std::to_string(selectedTroopMap["tank1"])
                << std::setw(75 + 11 * 4 - 30 + (phase1[1] < 4 && phase1[0] == 6 ? 11 : 0)) << prefix[6][0] + underline("+1", "green") + " (7)" + prefix[6][1] + underline("-1", "green") + prefix[6][2] + underline("All", "green") + prefix[6][3] + underline("Details", "green")
                << (this->army->total.size() > 6 ? (ptr[6]->first + " (" + std::to_string(ptr[6]->second->troopCount) + "/16)" + (ptr[6]->second->inBattle ? "" : prefix[6][4] + (selectedArmyMap.count(ptr[6]->first) == 0 ? underline("select", "green") : underline("deselect", "green")))) : "") << std::endl
                << std::setw(30) << "Tank 2:         " + std::to_string(fullHealth["tank2"]) + "/" + std::to_string(this->troop->helper["tank2"](0)) + "/" + std::to_string(selectedTroopMap["tank2"])
                << std::setw(75 + 11 * 4 - 30 + (phase1[1] < 4 && phase1[0] == 7 ? 11 : 0)) << prefix[7][0] + underline("+1", "green") + " (8)" + prefix[7][1] + underline("-1", "green") + prefix[7][2] + underline("All", "green") + prefix[7][3] + underline("Details", "green")
                << (this->army->total.size() > 7 ? (ptr[7]->first + " (" + std::to_string(ptr[7]->second->troopCount) + "/16)" + (ptr[7]->second->inBattle ? "" : prefix[7][4] + (selectedArmyMap.count(ptr[7]->first) == 0 ? underline("select", "green") : underline("deselect", "green")))) : "") << std::endl
                << std::setw(30) << "Tank Oshimai:   " + std::to_string(fullHealth["tankOshimai"]) + "/" + std::to_string(this->troop->helper["tankOshimai"](0)) + "/" + std::to_string(selectedTroopMap["tankOshimai"])
                << std::setw(75 + 11 * 4 - 30 + (phase1[1] < 4 && phase1[0] == 8 ? 11 : 0)) << prefix[8][0] + underline("+1", "green") + " (9)" + prefix[8][1] + underline("-1", "green") + prefix[8][2] + underline("All", "green") + prefix[8][3] + underline("Details", "green")
                << (this->army->total.size() > 8 ? (ptr[8]->first + " (" + std::to_string(ptr[8]->second->troopCount) + "/16)" + (ptr[8]->second->inBattle ? "" : prefix[8][4] + (selectedArmyMap.count(ptr[8]->first) == 0 ? underline("select", "green") : underline("deselect", "green")))) : "") << std::endl
                << std::setw(30) << "Cas:            " + std::to_string(fullHealth["cas"]) + "/" + std::to_string(this->troop->helper["cas"](0)) + "/" + std::to_string(selectedTroopMap["cas"])
                << std::setw(75 + 11 * 4 - 30 + (phase1[1] < 4 && phase1[0] == 9 ? 11 : 0)) << prefix[9][0] + underline("+1", "green") + " (-)" + prefix[9][1] + underline("-1", "green") + prefix[9][2] + underline("All", "green") + prefix[9][3] + underline("Details", "green")
                << (this->army->total.size() > 9 ? (ptr[9]->first + " (" + std::to_string(ptr[9]->second->troopCount) + "/16)" + (ptr[9]->second->inBattle ? "" : prefix[9][4] + (selectedArmyMap.count(ptr[9]->first) == 0 ? underline("select", "green") : underline("deselect", "green")))) : "") << std::endl
                << std::setw(30) << "Fighter:        " + std::to_string(fullHealth["fighter"]) + "/" + std::to_string(this->troop->helper["fighter"](0)) + "/" + std::to_string(selectedTroopMap["fighter"])
                << std::setw(75 + 11 * 4 - 30) << prefix[10][0] + underline("+1", "green") + " (-)" + prefix[10][1] + underline("-1", "green") + prefix[10][2] + underline("All", "green") + prefix[10][3] + underline("Details", "green") << std::endl
                << std::setw(30) << "Bomber:         " + std::to_string(fullHealth["bomber"]) + "/" + std::to_string(this->troop->helper["bomber"](0)) + "/" + std::to_string(selectedTroopMap["bomber"])
                << std::setw(75 + 11 * 4 - 30) << prefix[11][0] + underline("+1", "green") + " (-)" + prefix[11][1] + underline("-1", "green") + prefix[11][2] + underline("All", "green") + prefix[11][3] + underline("Details", "green") << std::endl
                << std::setw(30) << "Kamikaze:       " + std::string("-/") + std::to_string(this->troop->helper["kamikaze"](0)) + "/" + std::to_string(selectedTroopMap["kamikaze"])
                << std::setw(75 + 11 * 4 - 30) << prefix[12][0] + underline("+1", "green") + " (-)" + prefix[12][1] + underline("-1", "green") + prefix[12][2] + underline("All", "green") + prefix[12][3] + underline("Details", "green") << std::endl
                << std::endl;

      std::cout << std::setw(30 + 11) << color("Troops selected: ", "green") + std::to_string(selectedTroop.size()) << color(" Armies selected: ", "green") << selectedArmy.size() << std::endl
                << std::endl;

      std::cout << prefix[13][0] << underline("Confirm", "green") << " (z)" << prefix[13][1] << underline("Deselect all", "green") << " (x)" << std::endl
                << prefix[14][0] << underline("Back", "green") << " (spacebar)" << std::endl;
    }

    else if (subMode == 1)
    {
      std::vector<TroopUnit *> sort;

      for (auto i : this->troop->allTroop)
        if (i->type == subPhase1Type)
          sort.push_back(i);

      std::sort(sort.begin(), sort.end(), [](TroopUnit *a, TroopUnit *b) -> bool {
        return a->getHealth() < b->getHealth();
      });

      if (sort.size() > 0)
      {
        subPhase1[0] = (subPhase1[0] + (int)std::ceil(sort.size() / 4.0)) % (int)std::ceil(sort.size() / 4.0);
        subPhase1[1] = (subPhase1[1] + std::min((int)(sort.size() - subPhase1[0] * 4), 4)) % std::min((int)(sort.size() - subPhase1[0] * 4), 4);
      }

      else
      {
        subPhase1[0] = 0;
        subPhase1[1] = 0;
      }

      int screenY = 20;

      std::vector<std::string> render;

      for (int i = 0; i < sort.size(); i++)
      {
        if (i % 4 == 0)
          render.push_back("");
        render.back() += std::string((i / 4 == subPhase1[0] && i % 4 == subPhase1[1] ? " > " : "   ")) + (sort[i]->selected ? "   ++" : "     ") + typeToDisplay[subPhase1Type] + " (" + std::to_string((int)sort[i]->getHealth()) + "/" + std::to_string(typeToHealth[subPhase1Type]) + ")        ";
      }

      int maxX = 0;

      for (int i = 0; i < render.size(); i++)
        if (render[i].length() > maxX)
          maxX = render[i].length();

      for (int i = 0; i < render.size(); i++)
        if (render[i].length() < maxX)
          render[i] += std::string(maxX - render[i].length(), ' ');

      int scrollableY = std::max(0, (int)(render.size() - screenY));

      scroll1[1] = 0;

      scroll1[0] = std::min(std::max(0, scroll1[0]), scrollableY);

      if (subPhase1Mode == 0)
      {
        if (subPhase1[0] < scroll1[0])
          scroll1[0] = subPhase1[0];

        else if (subPhase1[0] >= scroll1[0] + screenY)
          scroll1[0] = subPhase1[0] - screenY + 1;

        scroll1[0] = std::min(std::max(0, scroll1[0]), scrollableY);
      }

      else if (subPhase1Mode == 1)
      {
        if (subPhase1[0] < scroll1[0])
          subPhase1[0] = scroll1[0];

        else if (subPhase1[0] >= scroll1[0] + screenY)
          subPhase1[0] = scroll1[0] + screenY - 1;
      }

      if (sort.size() > 0)
        subPhase1TroopDetail = sort[subPhase1[0] * 4 + subPhase1[1]];

      std::cout << color("Details of " + typeToDisplay[subPhase1Type], "green") << std::endl
                << "(enter to select/deselect) (spacebar to return)" << std::endl
                << std::endl;

      if (scroll1[0] != 0)
        std::cout << std::string(maxX / 2 - 8, ' ') + "||Scroll up (w)||" + std::string(maxX / 2 - 8, ' ') << std::endl;

      else
        std::cout << std::endl;

      for (int i = scroll1[0]; i < std::min(scroll1[0] + screenY, (int)render.size()); i++)
        std::cout << render[i] << std::endl;

      if (scroll1[0] != scrollableY)
        std::cout << std::string(maxX / 2 - 8, ' ') + "||Scroll down (s)||" + std::string(maxX / 2 - 8, ' ') << std::endl;

      else
        std::cout << std::endl;
    }

    this->lg3.unlock();
  });

  printMode.push_back([&]() {
    Block *ptr = this->enemies->totalEnemies[currentCountry]->map[phase0[0]][phase0[1]];

    if (!ptr->battling)
    {
      user.lock();

      std::cout << "\033[2J\033[1;1H" << std::endl;

      mode = 0;

      subPhase2Mode = 0;
      // avoid resource deadlock error

      std::thread temp([&]() {
        stopPrint();
        loopPrint();

      });

      temp.detach();
      return;
    }

    if (subPhase2Mode == 0)
    {

      int fillX = 150;
      int screenY = 27;

      std::vector<std::string> render;

      BattleUnit *ptr2 = ptr->battle.back();

      ptr2->lg.lock();


      render.push_back(std::string(fillX / 2 - 2, ' ') + "Stats");

      render.back() += std::string((int)(fillX - render.back().length()), ' ');

      int deathFd = (int)std::round(1.0 * ptr2->totalFriendlyDeathCount / ptr2->totalFriendly * 20);

      int deathFoe = (int)std::round(1.0 * ptr2->totalFoeDeathCount / ptr->totalFoe * 20);

      render.push_back("Casualty Rate: " + std::string(20 - deathFd, '+') + std::string(deathFd, '-'));

      std::string temp = std::string(deathFoe, '-') + std::string(20 - deathFoe, '+') + " :Casualty Rate";

      render.back() += std::string((int)(fillX - render.back().length() - temp.length()), ' ') + temp;

      render.push_back("Soft Attack: " + std::to_string((int)ptr2->totalSoftAttack));

      temp = std::to_string((int)ptr2->totalFoeSoftAttack) + " :Soft Attack";

      render.back() += std::string((int)(fillX - render.back().length() - temp.length()), ' ') + temp;

      render.push_back("Hard Attack: " + std::to_string((int)ptr2->totalHardAttack));

      temp = std::to_string((int)ptr2->totalFoeHardAttack) + " :Hard Attack";

      render.back() += std::string((int)(fillX - render.back().length() - temp.length()), ' ') + temp;

      render.push_back("Air Attack: " + std::to_string((int)ptr2->totalAirAttack));

      temp = std::to_string((int)ptr2->totalFoeAirAttack) + " :Air Attack";

      render.back() += std::string((int)(fillX - render.back().length() - temp.length()), ' ') + temp;



      render.push_back(std::string(fillX / 2 - 2, ' ') + "Troops");

      render.back() += std::string((int)(fillX - render.back().length()), ' ');

      render.push_back("Infantry: " + std::to_string(ptr2->friendCount["infantry"]));

      temp = std::to_string(ptr->foeCount["infantry"]) + " :Infantry";

      render.back() += std::string((int)(fillX - render.back().length() - temp.length()), ' ') + temp;

      render.push_back("Calvary: " + std::to_string(ptr2->friendCount["Calvary"]));

      temp = std::to_string(ptr->foeCount["Calvary"]) + " :Calvary";

      render.back() += std::string((int)(fillX - render.back().length() - temp.length()), ' ') + temp;

      render.push_back("Suicide Bomber: " + std::to_string(ptr2->friendCount["suicideBomber"]));

      temp = "0 :Suicide Bomber";

      render.back() += std::string((int)(fillX - render.back().length() - temp.length()), ' ') + temp;

      render.push_back("Artillery: " + std::to_string(ptr2->friendCount["artillery"]));

      temp = std::to_string(ptr->foeCount["artillery"]) + " :Artillery";

      render.back() += std::string((int)(fillX - render.back().length() - temp.length()), ' ') + temp;

      render.push_back("Logistic: " + std::to_string(ptr2->friendCount["logistic"]));

      temp = std::to_string(ptr->foeCount["logistic"]) + " :Logistic";

      render.back() += std::string((int)(fillX - render.back().length() - temp.length()), ' ') + temp;

      render.push_back("Armored Car: " + std::to_string(ptr2->friendCount["armoredCar"]));

      temp = std::to_string(ptr->foeCount["armoredCar"]) + " :Armored Car";

      render.back() += std::string((int)(fillX - render.back().length() - temp.length()), ' ') + temp;

      render.push_back("Tank 1: " + std::to_string(ptr2->friendCount["tank1"]));

      temp = std::to_string(ptr->foeCount["tank1"]) + " :Tank 1";

      render.back() += std::string((int)(fillX - render.back().length() - temp.length()), ' ') + temp;

      render.push_back("Tank 2: " + std::to_string(ptr2->friendCount["tank2"]));

      temp = std::to_string(ptr->foeCount["tank2"]) + " :Tank 2";

      render.back() += std::string((int)(fillX - render.back().length() - temp.length()), ' ') + temp;

      render.push_back("Tank Oshimai: " + std::to_string(ptr2->friendCount["tankOshimai"]));

      temp = std::to_string(ptr->foeCount["tankOshimai"]) + " :Tank Oshimai";
      render.back() += std::string((int)(fillX - render.back().length() - temp.length()), ' ') + temp;

      render.push_back("Cas: " + std::to_string(ptr2->friendCount["cas"]));

      temp = std::to_string(ptr->foeCount["cas"]) + " :Cas";

      render.back() += std::string((int)(fillX - render.back().length() - temp.length()), ' ') + temp;

      render.push_back("Fighter: " + std::to_string(ptr2->friendCount["fighter"]));

      temp = std::to_string(ptr->foeCount["fighter"]) + " :Fighter";

      render.back() += std::string((int)(fillX - render.back().length() - temp.length()), ' ') + temp;

      render.push_back("Bomber: " + std::to_string(ptr2->friendCount["bomber"]));

      temp = std::to_string(ptr->foeCount["bomber"]) + " :Bomber";

      render.back() += std::string((int)(fillX - render.back().length() - temp.length()), ' ') + temp;

      render.push_back("Kamikaze: " + std::to_string(ptr2->friendCount["kamikaze"]));

      temp = "0 :Kamikaze";

      render.back() += std::string((int)(fillX - render.back().length() - temp.length()), ' ') + temp;

      for (int i = 0; i < render.size(); i++)
        assert(render[i].length() == fillX);

      render.push_back(std::string(fillX / 2 - 2, ' ') + "Armies");

      render.back() += std::string((int)(fillX - render.back().length()), ' ');

      int troopLength = 16;
      int start = render.size();

      for (auto i : ptr2->mikata->totalArmy)
      {
        render.push_back("D" + std::string(troopLength - 1, ' ') + "C" + std::string(troopLength - 1, ' ') + "B" + std::string(troopLength - 1, ' ') + "A" + std::string(troopLength - 1, ' '));
        render.back() += std::string((int)(fillX / 2 - render.back().length()), ' ');

        for (int j = 0; j < 4; j++)
        {
          render.push_back("");

          for (int k = 3; k >= 0; k--)
          {
            if (i->formation[k][j] != NULL)
              temp = typeToDisplay[i->formation[k][j]->type];

            else
              temp = "None";

            render.back() += temp + std::string(troopLength - temp.length(), ' ');
          }

          render.back() += std::string((int)(fillX / 2 - render.back().length()), ' ');

          render.push_back("");

          for (int k = 3; k >= 0; k--)
          {
            if (i->formation[k][j] != NULL)
              temp = "(" + std::to_string((int)i->formation[k][j]->getHealth()) + "/" + std::to_string(i->formation[k][j]->getBaseHealth()) + ")";

            else
              temp = "";

            render.back() += temp + std::string(troopLength - temp.length(), ' ');
          }
          render.back() += std::string((int)(fillX / 2 - render.back().length()), ' ');
        }

        render.push_back(std::string(fillX / 2, ' '));
      }

      int tempCount = 0;
      std::string temp2 = "";

      for (auto i : ptr2->foe->totalArmy)
      {
        temp = std::string(troopLength - 1, ' ') + "A" + std::string(troopLength - 1, ' ') + "B" + std::string(troopLength - 1, ' ') + "C" + std::string(troopLength - 1, ' ') + "D";

        if (render.size() <= start + tempCount * 10)
        {
          for (int i = 0; i < 10; i++)
            render.push_back(std::string(fillX / 2, ' '));
        }
        render[start + tempCount * 10] += std::string((int)(fillX / 2 - temp.length()), ' ') + temp;
        for (int j = 0; j < 4; j++)
        {
          for (int k = 0; k < 4; k++)
          {
            if (i->formation[k][j] != NULL)
              temp = typeToDisplay[i->formation[k][j]->type];
            else
              temp = "None";
            temp2 += std::string(troopLength - temp.length(), ' ') + temp;
          }
          render[start + tempCount * 10 + j * 2 + 1] += std::string((int)(fillX / 2 - temp2.length()), ' ') + temp2;
          temp2 = std::string(troopLength * 4, ' ');
          render[start + tempCount * 10 + j * 2 + 2] += std::string((int)(fillX / 2 - temp2.length()), ' ') + temp2;
          temp2 = "";
        }
        render[start + tempCount * 10 + 9] += std::string(fillX / 2, ' ');
        tempCount++;
      }

      for (int i = 0; i < render.size(); i++)
        if (render[i].length() < fillX)
          render[i] += std::string(fillX - render[i].length(), ' ');

      int scrollableY = std::max(0, (int)(render.size() - screenY));
      scroll2[1] = 0;
      scroll2[0] = std::min(std::max(0, scroll2[0]), scrollableY);

      std::cout << "\033[1;1H";
      std::cout << color("View Battle", "magenta") << std::endl
                << "Change Speed: q     Pause: p    Retreat all: r     Battle log: l     Back: spacebar"
                << std::endl
                << std::endl;

      this->lg.lock();
      std::stringstream speed;
      speed << std::fixed << std::setprecision(1) << this->setting["speed"] / 1000.0;
      std::cout << color("Day: ", "green") << this->day << "/" << this->timeLimit << " (" << speed.str() << "s)     " << (this->paused ? color("PAUSED", "red") : "") << std::endl
                << std::endl;
      this->lg.unlock();

      if (scroll2[0] != 0)
        std::cout << std::string(fillX / 2 - 7, ' ') + "||Scroll up (w)||" + std::string(fillX / 2 - 8, ' ') << std::endl;
      else
        std::cout << std::string(fillX, ' ') << std::endl;
      for (int i = scroll2[0]; i < std::min((int)render.size(), scroll2[0] + screenY); i++)
        std::cout << render[i] << std::endl;
      if (scroll2[0] != scrollableY)
        std::cout << std::string(fillX / 2 - 8, ' ') + "||Scroll down (s)||" + std::string(fillX / 2 - 9, ' ') << std::endl;
      else
        std::cout << std::string(fillX, ' ') << std::endl;
      ptr2->lg.unlock();
    }
    else
    {
      std::cout << "\033[2J\033[1;1H";
      std::cout << color("View Battle", "magenta") << std::endl
                << "Change Speed: q     Pause: p     Back: spacebar"
                << std::endl
                << std::endl;

      this->lg.lock();
      std::stringstream speed;
      speed << std::fixed << std::setprecision(1) << this->setting["speed"] / 1000.0;
      std::cout << color("Day: ", "green") << this->day << "/" << this->timeLimit << " (" << speed.str() << "s)     " << (this->paused ? color("PAUSED", "red") : "") << std::endl
                << std::endl;
      this->lg.unlock();
      BattleUnit *ptr = this->enemies->totalEnemies[currentCountry]->map[phase0[0]][phase0[1]]->battle.back();
      for (int i = std::max(0, (int)ptr->log.size() - 26); i < ptr->log.size(); i++)
        std::cout << ptr->log[i] << std::endl;
    }
  });

  loopPrint = [&]() {
    printFuture = std::async(std::launch::async, [&]() {
      terminatePrint = false;
      user.unlock();
      while (!terminatePrint && !this->gameOver)
      {
        printMode[mode]();
        std::unique_lock<std::mutex> lock(printa);
        printCond.wait_for(lock, std::chrono::milliseconds(1000 / this->fps));
      }
    });
  };

  stopPrint = [&]() {
    if (printFuture.valid())
    {
      terminatePrint = true;
      printCond.notify_all();
      printFuture.get();
    }
  };
  std::cout << "\033[2J\033[1;1H";
  user.lock();
  loopPrint();
  char input;
  // std::future<void> temp = std::async(std::launch::async, [&]() {while(1){clean_stdin();std::this_thread::sleep_for(std::chrono::milliseconds(500));} });
  while (1)
  {
    if (this->gameOver)
    {
      this->stopTimer();
      this->endGame();
      while (input != ' ')
        input = getch();
      break;
    }
    input = getch();
    if (this->gameOver)
    {
      this->stopTimer();
      this->endGame();
      while (input != ' ')
        input = getch();
      break;
    }

    user.lock();
    stopPrint();
    user.unlock();

    if (input == '\033')
    {
      getch();
      switch (getch())
      {
      case 'A':
        if (mode == 0)
        {
          subPhase0Mode = 0;
          if (subMode == 0)
            phase0[0]--;
          else
            subPhase0[0]--;
        }
        else if (mode == 1)
        {
          subPhase1Mode = 0;
          if (subMode == 0)
            phase1[0]--;
          else
            subPhase1[0]--;
        }
        break;
      case 'B':
        if (mode == 0)
        {
          subPhase0Mode = 0;
          if (subMode == 0)
            phase0[0]++;
          else
            subPhase0[0]++;
        }
        else if (mode == 1)
        {
          subPhase1Mode = 0;
          if (subMode == 0)
            phase1[0]++;
          else
            subPhase1[0]++;
        }
        break;
      case 'C':
        if (mode == 0)
        {
          subPhase0Mode = 0;
          if (subMode == 0)
            phase0[1]++;
          else
            subPhase0[1]++;
        }
        else if (mode == 1)
        {
          subPhase1Mode = 0;
          if (subMode == 0)
            phase1[1]++;
          else
            subPhase1[1]++;
        }
        break;
      case 'D':
        if (mode == 0)
        {
          subPhase0Mode = 0;
          if (subMode == 0)
            phase0[1]--;
          else
            subPhase0[1]--;
        }
        else if (mode == 1)
        {
          subPhase1Mode = 0;
          if (subMode == 0)
            phase1[1]--;
          else
            subPhase1[1]--;
        }
        break;
      case '1':
        getch();
        getch();
        getch();
      }
    }
    else if (input == '\n')
    {

      enter = true;
    }
    else if (input == 'w')
    {

      if (mode == 0)
      {
        subPhase0Mode = 1;
        scroll0[0]--;
      }
      else if (mode == 1)
      {
        subPhase1Mode = 1;
        scroll1[0]--;
      }
      else if (mode == 2)
        scroll2[0]--;
    }
    else if (input == 'a')
    {

      if (mode == 0)
      {
        subPhase0Mode = 1;
        scroll0[1]--;
      }
      else if (mode == 1)
      {
        subPhase1Mode = 1;
        scroll1[1]--;
      }
      else if (mode == 2)
        scroll2[1]--;
    }
    else if (input == 's')
    {

      if (mode == 0)
      {
        scroll0[0]++;
        subPhase0Mode = 1;
      }
      else if (mode == 1)
      {
        subPhase1Mode = 1;
        scroll1[0]++;
      }
      else if (mode == 2)
        scroll2[0]++;
    }
    else if (input == 'd')
    {

      if (mode == 0)
      {
        subPhase0Mode = 1;
        scroll0[1]++;
      }
      else if (mode == 1)
      {
        subPhase1Mode = 1;
        scroll1[1]++;
      }
      else if (mode == 2)
        scroll2[1]++;
    }
    else if (input == 'q')
    {
      this->stopTimer();
      timeChosen = (timeChosen + 1) % this->timeRange.size();
      this->setting["speed"] = this->timeRange[this->timeChosen];
      this->timer(this->setting["speed"]);
    }
    else if (input == 'z')
    {

      if (mode == 0)
      {
        Enemy *ptr = this->enemies->totalEnemies[currentCountry];
        if ((!this->battle->inBattle || this->battle->countryBattling == ptr->name) && !ptr->map[phase0[0]][phase0[1]]->captured && ptr->map[phase0[0]][phase0[1]]->isAttackable && !ptr->capitulated)
        {
          mode = 1;
          subMode = 0;
        }
      }
      else if (mode == 1 && subMode == 0)
      {
        sendAll();
        continue;
      }
    }
    else if (input == 'r')
    {

      if (mode == 0)
        retreatAll();
      else if (mode == 1 && subMode == 0)
      {
        deselectAll();
      }
      else if (mode == 2)
        retreatAll();
    }
    else if (input == 'v')
    {

      if (mode == 0)
      {
        if (this->enemies->totalEnemies[currentCountry]->map[phase0[0]][phase0[1]]->battling)
        {
          mode = 2;
          subMode = 0;
        }
      }
    }
    else if (input == 'e')
    {

      if (mode == 0)
      {
        currentCountry = (currentCountry + 1) % this->enemies->totalEnemies.size();
        phase0[0] = 0;
        phase0[0] = 0;
        subMode = 0;
      }
    }
    else if (input == 'p')
    {

      this->paused = !this->paused;
    }
    else if (input == 'l')
    {

      if (mode == 2)
      {
        if (subPhase2Mode == 0)
          subPhase2Mode = 1;
      }
    }
    else if (input == ' ')
    {

      if (mode == 0)
      {
        if (subMode == 0)
        {
          gamePhase = prevPhase;
          this->gamePhaseSelect[0] = 0;
          this->gamePhaseSelect[1] = 0;
          (this->*this->print[this->gamePhase])(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
          break;
        }
        else
          subMode = 0;
      }
      else if (mode == 1)
      {
        if (subMode == 0)
        {
          deselectAll();
          mode = 0;
        }
        else
          subMode = 0;
      }
      else if (mode == 2)
      {
        if (subPhase2Mode == 0)
          mode = 0;
        else
          subPhase2Mode = 0;
      }
    }
    else if (input == '1')
    {
      if (mode == 1)
        troopIreru(indexToTroop[0]);
    }
    else if (input == '2')
    {
      if (mode == 1)
        troopIreru(indexToTroop[1]);
    }
    else if (input == '3')
    {
      if (mode == 1)
        troopIreru(indexToTroop[2]);
    }
    else if (input == '4')
    {
      if (mode == 1)
        troopIreru(indexToTroop[3]);
    }
    else if (input == '5')
    {
      if (mode == 1)
        troopIreru(indexToTroop[4]);
    }
    else if (input == '6')
    {
      if (mode == 1)
        troopIreru(indexToTroop[5]);
    }
    else if (input == '7')
    {
      if (mode == 1)
        troopIreru(indexToTroop[6]);
    }
    else if (input == '8')
    {
      if (mode == 1)
        troopIreru(indexToTroop[7]);
    }
    else if (input == '9')
    {
      if (mode == 1)
        troopIreru(indexToTroop[8]);
    }
    user.lock();
    std::cout << "\033[2J\033[1;1H";
    loopPrint();
  }
}