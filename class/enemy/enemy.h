#include <string>
#include <vector>
#include "../block/block.h"

#ifndef ENEMY_H
#define ENEMY_H

class Enemy{
  public:
    Enemy(std::string name_, std::vector<std::vector<Block*>> map_):name(name_),map(map_){}
    std::string name;
    bool capitulated = false;
    std::vector<std::vector<Block*>> map;
};
#endif