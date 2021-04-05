#include <cmath>
#include "troop.h"
#include "../../class/damage/damage.h"
void Fighter::giveDamage(double foodS, double equipmentS, double disruption, double attackDebuff, double airSupremacy, Damage &damage)
{
  double strength = ((foodS / food > 1 ? 1 : foodS / food) + (equipmentS / equipment > 1 ? 1 : equipmentS / equipment)) / 2 * 100;
  double debuff = attackDebuff * (1 - speed / 10);

  double softAttackC = softAttack * (strength / 100) * (strength == 100 ? 1.1 : 1) * (1 - disruption / 100) * (1 - debuff / 100) * airSupremacy;
  double hardAttackC = hardAttack * (strength / 100) * (strength == 100 ? 1.1 : 1) * (1 - disruption / 100 / 4) * (1 - debuff / 100) * airSupremacy;

  damage.softAttack += softAttackC;
  damage.hardAttack += hardAttackC;
}

void Fighter::takeDamage(double damage)
{
  hp -= damage;
  pivotalStrength = std::pow(1.2 - std::exp(-1.5 * (hp / Fighter::baseHp) + std::log(0.2) + 1.5), 1 - (hp / Fighter::baseHp));
}

void Fighter::increaseHealth(double recovery)
{
  hp += recovery;
  pivotalStrength = std::pow(1.2 - std::exp(-1.5 * (hp / Fighter::baseHp) + std::log(0.2) + 1.5), 1 - (hp / Fighter::baseHp));
}
