#ifndef TROOP_H
#define TROOP_H

#include <string>
#include "../../class/damage/damage.h"
class Troop
{
public:

  // Each troop is uniquely identified with a uuid
  Troop(std::string id):uuid(id){}

  // food supplied, equipment supplied, disruption, attack debuff, air supremacy, accumulate to Damage struct
  virtual void giveDamage(double, double, double, double, double, Damage &) = 0;

  // damage received is calculated outside of troop
  virtual void takeDamage(double) = 0;
  virtual double getHealth() = 0;
  virtual void increaseHealth(double) = 0;

  std::string getId() { return uuid; }

  protected:
    std::string uuid;
};
class Infantry : public Troop
{
public:

  Infantry(std::string id):Troop(id){}

  // food supplied, equipment supplied, disruption, attack debuff, air supremacy, accumulate to Damage struct
  void giveDamage(double, double, double, double, double, Damage &) = 0;

  // damage received is calculated outside of troop
  void takeDamage(double) = 0;
  double getHealth() = 0;
  void increaseHealth(double) = 0;

private:
  // permanent
  int trainingCamp = 1;
  int trainingTime = 10;
  int softAttack = 10;
  int hardAttack = 2;
  int defense = 25;
  int speed = 5;
  int equipment = 2;
  int food = 2;
  int disruption=0;
  int airAttack=0;
  int armor=0;
  // mutable
  int hp=100;
};
#endif