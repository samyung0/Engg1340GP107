#include <iostream>
#include <string>
#include <iomanip>

#include "../class/menuWrapper/menuWrapper.h"
#include "../io/io.h"
namespace setting
{
  void set(MenuWrapper &gameStats);
  std::string menuOptions[2][2] = {{"On", "Off"}, {"Back"}};
  int lastActiveIndex[2] = {0, 0};
}

// set menu options color according to settings
// input: gameStats
void setting::set(MenuWrapper &gameStats)
{
  for (auto i : gameStats.setting)
  {
    if (i.first == "showStatusPeriodically")
    {
      menuOptions[0][0] = color("On", (i.second ? "green" : "white"));
      menuOptions[0][1] = color("Off", (i.second ? "white" : "green"));
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

  // "clears" screen by printing out many blank lines
  std::cout << std::string(100, '\n') << std::endl;
  std::cout << "Settings." << std::endl;
  std::cout << "Show status periodically";
  std::cout << std::setw(50) << setting::menuOptions[0][0];
  std::cout << std::setw(17) << setting::menuOptions[0][1] << std::endl;
  std::cout << '\n'
            << setting::menuOptions[1][0] << std::endl;
}