#include "troop.h"
#include "../../class/damage/damage.h"
void Tank1::giveDamage(double foodS, double equipmentS, double disruption, double attackDebuff, double airSupremacy, Damage &damage)
{
  double strength = ((foodS / food > 1 ? 1 : foodS / food) + (equipmentS / equipment > 1 ? 1 : equipmentS / equipment)) / 2 * 100;
  double debuff = attackDebuff * (1 - speed / 10);

  double softAttackC = softAttack * (strength / 100) * (strength == 100 ? 1.1 : 1) * (1 - disruption / 100) * (1 - debuff / 100) * airSupremacy;
  double hardAttackC = hardAttack * (strength / 100) * (strength == 100 ? 1.1 : 1) * (1 - disruption / 100 / 4) * (1 - debuff / 100) * airSupremacy;

  damage.softAttack += softAttackC;
  damage.hardAttack += hardAttackC;
}

void Tank1::takeDamage(double damage)
{
  hp -= damage;
}

void Tank1::increaseHealth(double recovery)
{
  hp += recovery;
}
