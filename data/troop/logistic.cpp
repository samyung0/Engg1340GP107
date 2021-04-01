#include "troop.h"
void Logistic::takeDamage(double damage)
{
  hp -= damage;
}

double Logistic::getHealth()
{
  return hp;
}

void Logistic::increaseHealth(double recovery)
{
  hp += recovery;
}
