#include "menuWrapper.h"
#include <vector>

MenuWrapper::MenuWrapper(int levels, int difficulty)
{
  for (int i = 0; i < levels; i++)
  {
    std::vector<int> t;
    for (int j = 0; j < difficulty; j++)
      t.push_back(0);
    progress.push_back(t);
  }
}