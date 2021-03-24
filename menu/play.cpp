#include <iostream>
#include <string>

#include "../class/menuWrapper/menuWrapper.h"
namespace play
{
  std::string menuOptions[4] = {"> Levels", "Random Game", "Load Game", "Back"};
  int lastActiveIndex = 0;
}
void print_playSelect(int y, int x, MenuWrapper& _)
{

  // adjust the selected item
  play::menuOptions[play::lastActiveIndex].erase(0, 2);
  play::lastActiveIndex = y;
  play::menuOptions[play::lastActiveIndex].insert(0, "> ");

  // prints menu

  // "clears" screen by printing out many blank lines
  std::cout << std::string(100, '\n') << std::endl;
  std::cout << "Please select game type." << std::endl;
  for (int i = 0; i < 4; i++)
    std::cout << play::menuOptions[i] << std::endl;
}