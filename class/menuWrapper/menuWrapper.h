#include <unordered_map>
#include <string>
#include <vector>

#ifndef MENUWRAPPER_H
#define MENUWRAPPER_H
struct MenuWrapper
{
  MenuWrapper(int levels, int difficulty);
  std::unordered_map<std::string, int> setting;
  std::vector<std::vector<int>> progress;

  // level stats (to be written to in level action, to be read from in game)
  std::string temp;
};
#endif