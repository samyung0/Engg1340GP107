#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>

#include "../../io/io.h"
#include "../../class/menuWrapper/menuWrapper.h"
void statf(int &menuPhase, int prevMenuPhase, MenuWrapper& _)
{
  // read stat from file
  std::fstream v("stat", std::ios::in);
  std::string a;

  std::cout << "\033[2J\033[1;1H";
  while(std::getline(v, a)) std::cout << a << std::endl;

  v.close();

  // pause for 1.5 second
  std::this_thread::sleep_for(std::chrono::milliseconds(1500));

  menuPhase = prevMenuPhase;
  return;
}