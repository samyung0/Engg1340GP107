#include <iostream>
#include <string>

#include "../class/menuWrapper/menuWrapper.h"
namespace randomSelect
{
  std::string menuOptions[5] = {"> 1", "2", "3", "4", "5"};
  int lastActiveIndex = 0;
}
void print_randomSelect(int y, int x, MenuWrapper& _)
{

  // adjust the selected item
  randomSelect::menuOptions[randomSelect::lastActiveIndex].erase(0, 2);
  randomSelect::lastActiveIndex = x;
  randomSelect::menuOptions[randomSelect::lastActiveIndex].insert(0, "> ");

  // prints menu

  // clear screen
  std::cout << "\033[2J\033[1;1H";
  std::cout << "Please select difficulty" << std::endl << std::endl;
  for (int i = 0; i < 5; i++)
    std::cout << randomSelect::menuOptions[i] << "   ";
  std::cout << std::endl;
}