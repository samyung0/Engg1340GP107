#include <iostream>
#include <string>
#include <iomanip>

#include "../class/menuWrapper/menuWrapper.h"
#include "../io/io.h"

void print_settingSelect(int y, int x, MenuWrapper &gameStats)
{
  void set(MenuWrapper & gameStats);
  std::string menuOptions[2][3] = {{"Fast", "Normal", "Slow"}, {"Back"}};
  int lastActiveIndex[2] = {0, 0};
  for (auto i : gameStats.setting)
  {
    if (i.first == "speed")
    {
      menuOptions[0][0] = color("Fast", (i.second == 500 ? "green" : "white"));
      menuOptions[0][1] = color("Normal", (i.second == 1000 ? "green" : "white"));
      menuOptions[0][2] = color("Slow", (i.second == 2000 ? "green" : "white"));
    }
  }

  // adjust the selected item
  lastActiveIndex[0] = y;
  lastActiveIndex[1] = x;
  menuOptions[lastActiveIndex[0]][lastActiveIndex[1]].insert(0, " > ");

  // prints menu

  // clear screen
  std::cout << "\033[2J\033[1;1H";
  std::cout << "Settings" << std::endl;
  std::cout << "Game Speed";
  std::cout << std::string(30, ' ');
  std::cout << menuOptions[0][0];
  std::cout << std::string(5, ' ');
  std::cout << menuOptions[0][1];
  std::cout << std::string(5, ' ');
  std::cout << menuOptions[0][2] << std::endl;
  std::cout << '\n'
            << menuOptions[1][0] << std::endl;
}