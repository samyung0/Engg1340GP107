#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <vector>

class Block{
  public:
    Block(std::string name_, std::vector<std::vector<std::string>> attackable_):name(name_),attackable(attackable_){}
    std::string name;
    bool captured = false;
    std::vector<std::vector<std::string>> attackable;
};
#endif