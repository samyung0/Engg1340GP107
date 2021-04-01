#include "troop.h"
void Infantry::takeDamage(double damage)
{
  hp -= damage;
}

double Infantry::getHealth()
{
  return hp;
}

void Infantry::increaseHealth(double recovery)
{
  hp += recovery;
}
