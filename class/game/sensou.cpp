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
// troop withdraw page: sorted in ascending order of health, then alphanumerical order of battling region

// note the actual damage calculation and battle progression happens in the timer thread in game.cpp and in the BattleUnit class in gameStruct.cpp
// this cpp file is merely a representation and interface of the battle
void Game::sensou(int &gamePhase, int prevPhase)
{
  int currentCountry = 0;
  // 0: map mode, 1: send troops, 2: withdraw troops, 3: view battle
  int mode = 0;

  int blockSize[2] = {4, 6};

  // io
  int scroll[2] = {0, 0};
  int phase[2] = {0, 0};
  int enter = false;

  std::vector<std::function<void()>> printMode;
  std::function<void(TroopUnit *, std::string, std::string)> sendTroop;
  std::function<void(ArmyUnit *, std::string, std::string)> sendArmy;
  std::function<void(TroopUnit *, std::string, std::string)> retreatTroop;
  std::function<void(ArmyUnit *, std::string, std::string)> retreatArmy;

  std::function<void()> loopPrint;
  std::function<void()> stopPrint;

  std::future<void> printFuture;
  std::condition_variable printCond;
  std::mutex printa;
  std::mutex printb;
  bool terminatePrint = false;

  int battleCycle = 0;
  const int iconInterval = 500;
  int iconLocation = 0;

  printMode.push_back([&]() {
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

    phase[0] = (phase[0] + ptr->map.size()) % ptr->map.size();
    std::unordered_map<int, int> hasBlock;
    for (int i = 0; i < ptr->map[phase[0]].size(); i++)
    {
      if (ptr->map[phase[0]][i] != NULL)
        hasBlock[i] = 1;
    }
    // find closest block
    if (hasBlock.count(phase[1]) == 0)
    {
      int diff = 1;
      bool found = false;
      while (!found)
      {
        if (hasBlock.count(phase[1] + diff) != 0)
        {
          phase[1] = phase[1] + diff;
          found = true;
        }
        else if (hasBlock.count(phase[1] - diff) != 0)
        {
          phase[1] = phase[1] - diff;
          found = true;
        }
        diff++;
      }
    }

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
          if (i == phase[0] && j == phase[1])
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

          if (i == phase[0] && j == phase[1])
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
          if (!iconLocation)
            row3 += iconType + " ";
          else
            row3 += ' ' + iconType;
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

    bool inBattle = false;
    std::string name;
    for (auto i : this->enemies->totalEnemies)
      if (i->battlingRegions > 0)
      {
        inBattle = true;
        name = i->name;
        break;
      }

    int maxLength2 = 13;
    std::vector<std::string> render2;
    if (!inBattle || name == ptr->name)
      render2.push_back("Send Troop" + std::string(3, ' '));
    if (ptr->map[phase[0]][phase[1]]->battling)
    {
      render2.push_back("Retreat Troop");
      render2.push_back("View Battle" + std::string(2, ' '));
    }

    std::vector<std::string> render3;
    render3.push_back("Rewards");
    render3.push_back("Farm: " + std::to_string(ptr->map[phase[0]][phase[1]]->acquirable["farm"]));
    render3.push_back("Civilian Factory: " + std::to_string(ptr->map[phase[0]][phase[1]]->acquirable["civilianFactory"]));
    render3.push_back("Military Factory: " + std::to_string(ptr->map[phase[0]][phase[1]]->acquirable["militaryFactory"]));
    render3.push_back("Training Camp: " + std::to_string(ptr->map[phase[0]][phase[1]]->acquirable["trainingCamp"]));
    render3.push_back("Airport: " + std::to_string(ptr->map[phase[0]][phase[1]]->acquirable["airport"]));
    render3.push_back("Land: " + std::to_string(ptr->map[phase[0]][phase[1]]->acquirable["land"]));
    render3.push_back("");
    render3.push_back("");
    render3.push_back("Terrain");
    render3.push_back(ptr->map[phase[0]][phase[1]]->terrain);
    int maxLength3 = 0;
    for (auto i : render3)
      if (i.length() > maxLength3)
        maxLength3 = i.length();
    for (int i = 0; i < render3.size(); i++)
      render3[i] += std::string(maxLength3 - render3[i].length(), ' ');

    std::vector<std::string> render4;
    render4.push_back("Enemies troops");
    render4.push_back("Infantry: " + std::to_string(ptr->map[phase[0]][phase[1]]->foeCount["infantry"]));
    render4.push_back("Calvary: " + std::to_string(ptr->map[phase[0]][phase[1]]->foeCount["calvary"]));
    render4.push_back("Logistic: " + std::to_string(ptr->map[phase[0]][phase[1]]->foeCount["logistic"]));
    render4.push_back("Armored Car: " + std::to_string(ptr->map[phase[0]][phase[1]]->foeCount["armoredCar"]));
    render4.push_back("Tank 1: " + std::to_string(ptr->map[phase[0]][phase[1]]->foeCount["tank1"]));
    render4.push_back("Tank 2: " + std::to_string(ptr->map[phase[0]][phase[1]]->foeCount["tank2"]));
    render4.push_back("Tank Oshimai: " + std::to_string(ptr->map[phase[0]][phase[1]]->foeCount["tankOshimai"]));
    render4.push_back("Cas: " + std::to_string(ptr->map[phase[0]][phase[1]]->foeCount["cas"]));
    render4.push_back("Fighter: " + std::to_string(ptr->map[phase[0]][phase[1]]->foeCount["fighter"]));
    render4.push_back("Bomber: " + std::to_string(ptr->map[phase[0]][phase[1]]->foeCount["bomber"]));
    render4.push_back("Armies: " + std::to_string(ptr->map[phase[0]][phase[1]]->foeCount["army"]));
    int maxLength4 = 0;
    for (auto i : render4)
      if (i.length() > maxLength4)
        maxLength4 = i.length();
    for (int i = 0; i < render4.size(); i++)
      render4[i] += std::string(maxLength4 - render4[i].length(), ' ');

    int scrollable[2] = {std::max(0, (int)render.size() - screen[0]), std::max(0, (int)render[0].size() - screen[1])};
    scroll[0] = std::min(std::max(scroll[0], 0), scrollable[0]);
    scroll[1] = std::min(std::max(scroll[1], 0), scrollable[1]);

    std::cout << "\033[1;1H";
    std::cout << color("Battle", "magenta") << std::endl;

    this->lg.lock();
    std::stringstream speed;
    speed << std::fixed << std::setprecision(1) << this->setting["speed"] / 1000.0;
    std::cout << color("Day: ", "green") << this->day << "/" << this->timeLimit << " (" << speed.str() << "s)" << std::endl
              << std::endl;
    this->lg.unlock();

    std::cout << "Country: " << this->enemies->totalEnemies[currentCountry]->name
              << " (" << (this->enemies->totalEnemies[currentCountry]->capitulated ? "defeated" : "not defeated") << ")"
              << "   Battling Regions: " << this->enemies->totalEnemies[currentCountry]->battlingRegions
              << std::endl;

    std::cout << "Change speed: e   Next country: q (total: " << this->enemies->totalEnemies.size() << ")     (wasd to move map if too large)" << std::endl
              << std::endl;

    for (int i = 0; i < 12; i++)
    {
      std::cout << render[i+scroll[0]].substr(scroll[1],screen[1]) << "        "
                << (render2.size() > i ? render2[i] : std::string(maxLength2, ' ')) << "        "
                << (render3.size() > i ? render3[i] : std::string(maxLength3, ' ')) << "        "
                << (render4.size() > i ? render4[i] : std::string(maxLength4, ' ')) << std::endl;
    }
    for (int i = 12; i < screen[0]; i++)
      std::cout << render[i+scroll[0]].substr(scroll[1],screen[1]) << std::endl;

    this->lg3.unlock();
  });

  loopPrint = [&]() {
    printFuture = std::async(std::launch::async, [&]() {
      terminatePrint = false;
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
  printMode[mode]();
  char input;
  while (1)
  {
    input = getch();
    if (this->gameOver)
    {
      stopPrint();
      break;
    }

    stopPrint();

    if (input == '\033')
    {
      getch();
      switch (getch())
      {
      case 'A':
        phase[0]--;
        break;
      case 'B':
        phase[0]++;
        break;
      case 'C':
        phase[1]++;
        break;
      case 'D':
        phase[1]--;
        break;
      }
    }
    else if (input == '\n')
      enter = true;
    else if (input == 'w')
      scroll[0]--;
    else if (input == 'a')
      scroll[1]--;
    else if (input == 's')
      scroll[0]++;
    else if (input == 'd')
      scroll[1]++;

    std::cout << "\033[2J\033[1;1H";
    loopPrint();
  }
}