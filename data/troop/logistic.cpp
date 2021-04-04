#include "troop.h"
void Logistic::takeDamage(double damage)
{
  hp -= damage;
}

void Logistic::increaseHealth(double recovery)
{
  hp += recovery;
}
