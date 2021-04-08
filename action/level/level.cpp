#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>

#include "../../io/io.h"
#include "../../class/menuWrapper/menuWrapper.h"

namespace levelf
{
  std::vector<std::vector<std::string>> print = {{"> Easy", "Hard"}, {"Back"}};
  int activeIndex[2] = {0, 0};
}

// base function to be called by sub functions, load game stats into gamestats and update menuphase
// input game level
void levelBasef(int &menuPhase, int prevMenuPhase, MenuWrapper &gameStats, std::string index)
{
  while (1)
  {
    // clear screen
    std::cout << "\033[2J\033[1;1H";
    std::cout << "Please choose the difficulty." << std::endl
              << std::endl;
    std::cout << std::setw(10) << levelf::print[0][0] << std::setw(10) << levelf::print[0][1] << std::endl
              << std::endl;
    std::cout << levelf::print[1][0] << std::endl;

    // detecing arrow keys
    char input;

    input = getch();
    if (input == '\033')
    {
      getch();

      levelf::print[levelf::activeIndex[0]][levelf::activeIndex[1]] = levelf::print[levelf::activeIndex[0]][levelf::activeIndex[1]].erase(0, 2);
      switch (getch())
      {
      case 'A':
        levelf::activeIndex[0] = (levelf::activeIndex[0] + -1 + levelf::print.size()) % levelf::print.size();
        levelf::activeIndex[1] = (levelf::activeIndex[1]) % levelf::print[levelf::activeIndex[1]].size();
        break;
      case 'B':
        levelf::activeIndex[0] = (levelf::activeIndex[0] + 1) % levelf::print.size();
        levelf::activeIndex[1] = (levelf::activeIndex[1]) % levelf::print[levelf::activeIndex[1]].size();
        break;
      case 'C':
        levelf::activeIndex[0] = (levelf::activeIndex[0]) % levelf::print.size();
        levelf::activeIndex[1] = (levelf::activeIndex[1] + 1) % levelf::print[levelf::activeIndex[1]].size();
        break;
      case 'D':
        levelf::activeIndex[0] = (levelf::activeIndex[0]) % levelf::print.size();
        levelf::activeIndex[1] = (levelf::activeIndex[1] + -1 + levelf::print[levelf::activeIndex[1]].size()) % levelf::print[levelf::activeIndex[1]].size();
        break;
      }
      levelf::print[levelf::activeIndex[0]][levelf::activeIndex[1]] = levelf::print[levelf::activeIndex[0]][levelf::activeIndex[1]].insert(0, "> ");
    }
    // progress menu phase
    else if (input == '\n')
    {
      if (levelf::activeIndex[0] == 1)
      {
        menuPhase = prevMenuPhase;
        return;
      }
      else
      {
        gameStats.levelpath = "data/levels/" + index + (levelf::activeIndex[1] ? "b" : "a") + ".dat";
        // phase 999 calls game to start in main
        menuPhase = 999;
        return;
      }
    }
  }
}

void level1f(int &menuPhase, int prevMenuPhase, MenuWrapper &gameStats)
{
  levelBasef(menuPhase, prevMenuPhase, gameStats, "1");
}

void level2f(int &menuPhase, int prevMenuPhase, MenuWrapper &gameStats)
{
  levelBasef(menuPhase, prevMenuPhase, gameStats, "2");
}
void level3f(int &menuPhase, int prevMenuPhase, MenuWrapper &gameStats)
{
  levelBasef(menuPhase, prevMenuPhase, gameStats, "3");
}
void level4f(int &menuPhase, int prevMenuPhase, MenuWrapper &gameStats)
{
  levelBasef(menuPhase, prevMenuPhase, gameStats, "4");
}
void level5f(int &menuPhase, int prevMenuPhase, MenuWrapper &gameStats)
{
  levelBasef(menuPhase, prevMenuPhase, gameStats, "5");
}
void level6f(int &menuPhase, int prevMenuPhase, MenuWrapper &gameStats)
{
  levelBasef(menuPhase, prevMenuPhase, gameStats, "6");
}
void level7f(int &menuPhase, int prevMenuPhase, MenuWrapper &gameStats)
{
  levelBasef(menuPhase, prevMenuPhase, gameStats, "7");
}
void level8f(int &menuPhase, int prevMenuPhase, MenuWrapper &gameStats)
{
  levelBasef(menuPhase, prevMenuPhase, gameStats, "8");
}
void level9f(int &menuPhase, int prevMenuPhase, MenuWrapper &gameStats)
{
  levelBasef(menuPhase, prevMenuPhase, gameStats, "9");
}