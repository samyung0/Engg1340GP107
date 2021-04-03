#include <iostream>
#include <fstream>
#include <string>
#include <functional>

#include "../../io/io.h"
#include "../../class/menuWrapper/menuWrapper.h"

// base function to be called by sub functions, updates the setting file
// input index and val to be updated to
void settingBasef(int &menuPhase, int prevMenuPhase, MenuWrapper &gameStats, std::string index, int val)
{
  std::fstream settingStream("save/setting.txt", std::ios::out | std::ios::trunc);
  gameStats.setting[index] = val;

  for (auto i = gameStats.setting.begin(); i != gameStats.setting.end(); i++)
    settingStream << (i->first + "=" + std::to_string(i->second)) << std::endl;

  settingStream.close();
  menuPhase = prevMenuPhase;
  return;
}

void setting1af(int &menuPhase, int prevMenuPhase, MenuWrapper &gameStats)
{
  settingBasef(menuPhase, prevMenuPhase, gameStats, "speed", 1000);
}

void setting1bf(int &menuPhase, int prevMenuPhase, MenuWrapper &gameStats)
{
  settingBasef(menuPhase, prevMenuPhase, gameStats, "speed", 500);
}