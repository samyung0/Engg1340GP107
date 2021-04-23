#include <iostream>
#include <string>
#include <iomanip>

#include "../class/menuWrapper/menuWrapper.h"
#include "../io/io.h"

void print_settingSelect(int y, int x, MenuWrapper &gameStats)
{
  void set(MenuWrapper & gameStats);
  std::string menuOptions[3][6] = {{"Fast", "Normal", "Slow"}, {"0.5", "1", "2", "3", "4", "5"}, {"Back"}};
  int lastActiveIndex[2] = {0, 0};
  for (auto i : gameStats.setting)
  {
    if (i.first == "speed")
    {
      menuOptions[0][0] = color("Fast", (i.second == 500 ? "green" : "white"));
      menuOptions[0][1] = color("Normal", (i.second == 1000 ? "green" : "white"));
      menuOptions[0][2] = color("Slow", (i.second == 2000 ? "green" : "white"));
    }else if(i.first == "completeness"){
      menuOptions[1][0] = color("0.5", (i.second == -1 ? "green" : "white"));
      menuOptions[1][1] = color("1", (i.second == 1 ? "green" : "white"));
      menuOptions[1][2] = color("2", (i.second == 2 ? "green" : "white"));
      menuOptions[1][3] = color("3", (i.second == 3 ? "green" : "white"));
      menuOptions[1][4] = color("4", (i.second == 4 ? "green" : "white"));
      menuOptions[1][5] = color("5", (i.second == 5 ? "green" : "white"));
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
  std::cout << std::endl;
  std::cout << "Map generation completeness";
  std::cout << std::string(13, ' ');
  std::cout << menuOptions[1][0];
  std::cout << std::string(5, ' ');
  std::cout << menuOptions[1][1];
  std::cout << std::string(5, ' ');
  std::cout << menuOptions[1][2];
  std::cout << std::string(5, ' ');
  std::cout << menuOptions[1][3];
  std::cout << std::string(5, ' ');
  std::cout << menuOptions[1][4];
  std::cout << std::string(5, ' ');
  std::cout << menuOptions[1][5];
  std::cout << "\n\n"
            << menuOptions[2][0] << std::endl;
}