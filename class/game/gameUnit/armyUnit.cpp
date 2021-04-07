#include <string>
#include <vector>
#include <tuple>
#include <iostream>

#include "../gameUnit.h"

#include "../../../data/troop/troop.h"

ArmyUnit::ArmyUnit(std::string name_):name(name_){}

ArmyUnit::ArmyUnit(std::string name_, std::vector<std::tuple<int, int,TroopUnit*>> fillTroop):name(name_){
  for(int i=0;i<fillTroop.size();i++)
    this->formation[std::get<0>(fillTroop[i])][std::get<1>(fillTroop[i])] = std::get<2>(fillTroop[i]);
    
}