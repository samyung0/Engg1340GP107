#include <unordered_map>
#include <string>
#include <vector>

#ifndef MENUWRAPPER_H
#define MENUWRAPPER_H
struct MenuWrapper
{
  MenuWrapper(int, int, int, int, int);
  std::unordered_map<std::string, int> setting;
  std::vector<std::vector<int>> progress;
  int width = 0;
  int height = 0;
  int fps;
};
#endif