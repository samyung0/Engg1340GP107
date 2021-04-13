#include "troop.h"
#include "../../class/damage/damage.h"
void SuicideBomber::giveDamage(double disruption, double airSupremacy, double mul1, double mul2, double mul3, double mul4, Damage &damage)
{
  double strength = 0.6 * this->pivotalStrength + 0.4 *this->subsequentialStrength;

  double softAttackC = softAttack * (strength) * (strength == 1 ? 1.1 : 1) * (1 - disruption) * (1 - this->attackDebuff) * airSupremacy * mul1;
  double hardAttackC = hardAttack * (strength) * (strength == 1 ? 1.1 : 1) * (1 - disruption / 4) * (1 - this->attackDebuff) * airSupremacy * mul1;

  damage.softAttack += softAttackC;
  damage.hardAttack += hardAttackC;

  this->takeDamage(999);
}

void SuicideBomber::takeDamage(double damage){
  hp -= damage;
}