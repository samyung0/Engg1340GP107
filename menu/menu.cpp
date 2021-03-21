#include <iostream>
#include <string>
namespace menu{
  std::string menuOptions[3] = {"> Play", "Settings", "Beginner Guide"};
  int lastActiveIndex = 0;
}
void print_menuSelect(int diff){

  // adjust the selected item
  menu::menuOptions[menu::lastActiveIndex].erase(0, 2);
  menu::lastActiveIndex = (menu::lastActiveIndex + diff + 3) % 3;
  menu::menuOptions[menu::lastActiveIndex].insert(0, "> ");

  // prints menu

  // "clears" screen by printing out many blank lines
  std::cout << std::string(100, '\n') << std::endl;
  std::cout << "Welcome to Hearts of Gold." << std::endl;
  for(int i=0;i<3;i++) std::cout << menu::menuOptions[i] << std::endl;
}