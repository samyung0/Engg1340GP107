#include <iostream>
#include <string>

#include "../class/menuWrapper/menuWrapper.h"
namespace menu
{
  std::string menuOptions[4] = {"> Play", "Settings", "Stats", "Quit"};
  int lastActiveIndex = 0;
}
void print_menuSelect(int y, int x, MenuWrapper& _)
{

  // adjust the selected item
  menu::menuOptions[menu::lastActiveIndex].erase(0, 2);
  menu::lastActiveIndex = y;
  menu::menuOptions[menu::lastActiveIndex].insert(0, "> ");

  // prints menu

  // clear screen
  std::cout << "\033[2J\033[1;1H";
  std::cout << "Welcome to Hearts of Gold." << std::endl;
  for (int i = 0; i < 4; i++)
    std::cout << menu::menuOptions[i] << std::endl;
  std::cout << std::endl;
  std::cout << "Please enlarge the screen until both title and the last line are shown:"
            << "\n|"
            << "\n|"
            << "\n|"
            << "\n|"
            << "\n|"
            << "\n|"
            << "\n|"
            << "\n|"
            << "\n|"
            << "\n|"
            << "\n|"
            << "\n|"
            << "\n|"
            << "\n|"
            << "\n|"
            << "\n|"
            << "\n|"
            << "\n|"
            << "\n|"
            << "\n|"
            << "\nEnd"
            << std::endl;
}