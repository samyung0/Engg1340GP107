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
  double hp=100;
};
class Calvary : public Troop
{
private:
  // permanent
  int trainingCamp = 1;
  int trainingTime = 10;
  int softAttack = 8;
  int hardAttack = 1;
  int defense = 20;
  int speed = 10;
  int equipment = 2;
  int food = 2;
  int disruption=0;
  int airAttack=0;
  int armor=0;
  // mutable
  double hp=150;
};
class SuicideBomber : public Troop
{
private:
  // permanent
  int trainingCamp = 2;
  int trainingTime = 50;
  int softAttack = 300;
  int hardAttack = 200;
  int defense = 0;
  int speed = 30;
  int equipment = 2;
  int food = 1;
  int disruption=1;
  int airAttack=0;
  int armor=0;
  // mutable
  double hp=-999;
};
class Artillery : public Troop
{
private:
  // permanent
  int trainingCamp = 2;
  int trainingTime = 20;
  int softAttack = 3;
  int hardAttack = 12;
  int defense = 10;
  int speed = 2;
  int equipment = 2;
  int food = 1;
  int disruption=0;
  int airAttack=0;
  int armor=0;
  // mutable
  double hp=170;
};
class Logistic : public Troop
{
private:
  // permanent
  int trainingCamp = 1;
  int trainingTime = 20;
  int softAttack = 0;
  int hardAttack = 0;
  int defense = 15;
  int speed = 2;
  int equipment = 2;
  int food = 2;
  int disruption=0;
  int airAttack=0;
  int armor=0;
  // mutable
  double hp=50;
};
class ArmoredCar : public Troop
{
private:
  // permanent
  int trainingCamp = 2;
  int trainingTime = 30;
  int softAttack = 18;
  int hardAttack = 10;
  int defense = 30;
  int speed = 15;
  int equipment = 10;
  int food = 6;
  int disruption=0;
  int airAttack=0;
  int armor=15;
  // mutable
  double hp=160;
};
class Tank1 : public Troop
{
private:
  // permanent
  int trainingCamp = 3;
  int trainingTime = 40;
  int softAttack = 12;
  int hardAttack = 20;
  int defense = 10;
  int speed = 6;
  int equipment = 15;
  int food = 6;
  int disruption=0;
  int airAttack=0;
  int armor=25;
  // mutable
  double hp=230;
};
class Tank2 : public Troop
{
private:
  // permanent
  int trainingCamp = 5;
  int trainingTime = 60;
  int softAttack = 12;
  int hardAttack = 50;
  int defense = 12;
  int speed = 8;
  int equipment = 25;
  int food = 8;
  int disruption=0;
  int airAttack=0;
  int armor=35;
  // mutable
  double hp=30;
};
class TankOshimai : public Troop
{
private:
  // permanent
  int trainingCamp = 20;
  int trainingTime = 200;
  int softAttack = 80;
  int hardAttack = 150;
  int defense = 40;
  int speed = 1;
  int equipment = 50;
  int food = 25;
  int disruption=2;
  int airAttack=0;
  int armor=80;
  // mutable
  double hp=1000;
};
#endif