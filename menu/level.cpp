#include <iostream>
#include <string>
#include <iomanip>
#include <cstdlib>

#include "../io/io.h"
#include "../class/menuWrapper/menuWrapper.h"

namespace level
{
  void set(MenuWrapper &gameStats);
  std::string **menuOptions;
  int lastActiveIndex[2] = {0, 0};
}

// set menu options color according to level
// input: gameStats
void level::set(MenuWrapper &gameStats)
{
  menuOptions = new std::string *[4];
  for (int i = 0; i < 4; i++)
    menuOptions[i] = new std::string[3];
  menuOptions[3][0] = "Back";

  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      menuOptions[i][j] = color(std::to_string(i * 3 + j + 1), (gameStats.progress[i * 3 + j][1] ? "green" : (gameStats.progress[i * 3 + j][0] ? "yellow" : "white")));
    }
  }
  menuOptions[lastActiveIndex[0]][lastActiveIndex[1]].insert(0, "> ");
}
void print_levelSelect(int y, int x, MenuWrapper &gameStats)
{
  level::set(gameStats);
  // adjust the selected item
  level::menuOptions[level::lastActiveIndex[0]][level::lastActiveIndex[1]].erase(0, 2);
  level::lastActiveIndex[0] = y;
  level::lastActiveIndex[1] = x;
  level::menuOptions[level::lastActiveIndex[0]][level::lastActiveIndex[1]].insert(0, "> ");

  // prints menu

  // clear screen
  std::cout << "\033[2J\033[1;1H";
  std::cout << "Please select level." << std::endl;
  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
      std::cout << std::setw(15) << level::menuOptions[i][j];
    std::cout << std::endl;
  }
  std::cout << level::menuOptions[3][0] << std::endl;
}