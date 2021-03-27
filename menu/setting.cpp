#include <iostream>
#include <string>
#include <iomanip>

#include "../class/menuWrapper/menuWrapper.h"
#include "../io/io.h"
namespace setting
{
  void set(MenuWrapper &gameStats);
  std::string menuOptions[2][2] = {{"Normal", "Fast"}, {"Back"}};
  int lastActiveIndex[2] = {0, 0};
}

// set menu options color according to settings
// input: gameStats
void setting::set(MenuWrapper &gameStats)
{
  for (auto i : gameStats.setting)
  {
    if (i.first == "speed")
    {
      menuOptions[0][0] = color("Normal", (i.second == 1000 ? "green" : "white"));
      menuOptions[0][1] = color("Fast", (i.second == 500 ? "green" : "white"));
    }
  }
  menuOptions[lastActiveIndex[0]][lastActiveIndex[1]].insert(0, "> ");
}
void print_settingSelect(int y, int x, MenuWrapper &gameStats)
{

  setting::set(gameStats);

  // adjust the selected item
  setting::menuOptions[setting::lastActiveIndex[0]][setting::lastActiveIndex[1]].erase(0, 2);
  setting::lastActiveIndex[0] = y;
  setting::lastActiveIndex[1] = x;
  setting::menuOptions[setting::lastActiveIndex[0]][setting::lastActiveIndex[1]].insert(0, "> ");

  // prints menu

  // clear screen
  std::cout << "\033[2J\033[1;1H";
  std::cout << "Settings." << std::endl;
  std::cout << "Game Speed";
  std::cout << std::setw(50) << setting::menuOptions[0][0];
  std::cout << std::setw(18) << setting::menuOptions[0][1] << std::endl;
  std::cout << '\n'
            << setting::menuOptions[1][0] << std::endl;
}