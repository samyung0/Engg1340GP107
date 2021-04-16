#include <cmath>
#include "troop.h"
#include "../../class/damage/damage.h"
void Fighter::giveDamage(double disruption, double airSupremacy, double mul1, double mul2, double mul3, double mul4, Damage &damage)
{
  double strength = 0.6 * this->pivotalStrength + 0.4 *this->subsequentialStrength;

  double softAttackC = softAttack * (strength) * (strength == 1 ? 1.1 : 1) * (1 - disruption) * (1 - this->attackDebuff) * airSupremacy * (mul2 + mul4 - 1);
  double hardAttackC = hardAttack * (strength) * (strength == 1 ? 1.1 : 1) * (1 - disruption / 4) * (1 - this->attackDebuff) * airSupremacy * (mul2 + mul4 - 1);

  damage.softAttack += softAttackC;
  damage.hardAttack += hardAttackC;
}

void Fighter::takeDamage(double damage)
{
  hp -= damage;
}

void Fighter::increaseHealth(double recovery)
{
  hp += recovery;
  hp = std::min((double) this->baseHp, hp);
}
