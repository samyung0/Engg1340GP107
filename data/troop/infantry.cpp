#include "troop.h"
#include "../../class/damage/damage.h"
void Infantry::giveDamage(double foodS, double equipmentS, double disruption, double attackDebuff, double airSupremacy, Damage &damage)
{
  double strength = ((foodS / food > 1 ? 1 : foodS / food) + (equipmentS / equipment > 1 ? 1 : equipmentS / equipment)) / 2 * 100;
  double debuff = attackDebuff * (1 - speed / 10);

  double softAttackC = softAttack * (strength / 100) * (strength == 100 ? 1.1 : 1) * (1 - disruption / 100) * (1 - debuff / 100);
  double hardAttackC = hardAttack * (strength / 100) * (strength == 100 ? 1.1 : 1) * (1 - disruption / 100 / 4) * (1 - debuff / 100);

  damage.softAttack += softAttackC;
  damage.hardAttack += hardAttackC;
}

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
