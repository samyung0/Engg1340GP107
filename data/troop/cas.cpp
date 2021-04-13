#include <cmath>
#include "troop.h"
#include "../../class/damage/damage.h"
void Cas::giveDamage(double disruption, double airSupremacy, double mul1, double mul2, double mul3, double mul4, Damage &damage)
{
  double strength = 0.6 * this->pivotalStrength + 0.4 *this->subsequentialStrength;

  double softAttackC = softAttack * (strength) * (strength == 1 ? 1.1 : 1) * (1 - disruption) * (1 - this->attackDebuff) * airSupremacy * (mul2 + mul4 - 1);
  double hardAttackC = hardAttack * (strength) * (strength == 1 ? 1.1 : 1) * (1 - disruption / 4) * (1 - this->attackDebuff) * airSupremacy * (mul2 + mul4 - 1);

  damage.softAttack += softAttackC;
  damage.hardAttack += hardAttackC;
}

void Cas::takeDamage(double damage)
{
  hp -= damage;
  pivotalStrength = std::pow(1.2 - std::exp(-1.5 * (hp / Cas::baseHp) + std::log(0.2) + 1.5), 1 - (hp / Cas::baseHp));
}

void Cas::increaseHealth(double recovery)
{
  hp += recovery;
  pivotalStrength = std::pow(1.2 - std::exp(-1.5 * (hp / Cas::baseHp) + std::log(0.2) + 1.5), 1 - (hp / Cas::baseHp));
}
