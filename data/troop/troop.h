#ifndef TROOP_H
#define TROOP_H

#include <string>
#include "../../class/damage/damage.h"
class Troop
{
public:
  // Each troop is uniquely identified with a uuid
  Troop(std::string id) : uuid(id) {}

  // food supplied, equipment supplied, disruption, attack debuff, air supremacy, accumulate to Damage struct
  virtual void giveDamage(double, double, double, double, double, Damage &) = 0;

  // damage received is calculated outside of troop
  virtual void takeDamage(double) = 0;
  virtual double getHealth() = 0;
  virtual void increaseHealth(double) = 0;

  std::string getId() { return uuid; }

  std::string uuid;

  // reference (all data)
  // const static int trainingCamp;
  // const static int trainingTime;
  // const static int softAttack;
  // const static int hardAttack;
  // const static int defense;
  // const static int speed;
  // const static int disruption;
  // const static int airAttack;
  // const static int armor;
  // const static int conspicuousness;
  // double equipment;
  // double food;
  // double hp;

};
class Infantry : public Troop
{
public:
  Infantry(std::string id) : Troop(id) {}

  // food supplied, equipment supplied, disruption, attack debuff, air supremacy, accumulate to Damage struct
  void giveDamage(double, double, double, double, double, Damage &) = 0;

  // damage received is calculated outside of troop
  void takeDamage(double) = 0;
  double getHealth() = 0;
  void increaseHealth(double) = 0;
  
  //permanent
  const static int trainingCamp;
  const static int trainingTime;
  const static int softAttack;
  const static int hardAttack;
  const static int defense;
  const static int speed;
  const static int disruption;
  const static int airAttack;
  const static int armor;
  const static int conspicuousness;

  // mutable
  double equipment = 2;
  double food = 2;
  double hp = 100;
};
class Calvary : public Troop
{

public:
  Calvary(std::string id) : Troop(id) {}
  void giveDamage(double, double, double, double, double, Damage &) = 0;
  void takeDamage(double) = 0;
  double getHealth() = 0;
  void increaseHealth(double) = 0;
  
  //permanent
  const static int trainingCamp;
  const static int trainingTime;
  const static int softAttack;
  const static int hardAttack;
  const static int defense;
  const static int speed;
  const static int disruption;
  const static int airAttack;
  const static int armor;
  const static int conspicuousness;

  // mutable
  double equipment = 2;
  double food = 2;
  double hp = 150;
};
class SuicideBomber : public Troop
{
public:
  SuicideBomber(std::string id) : Troop(id) {}
  void giveDamage(double, double, double, double, double, Damage &) = 0;
  double getHealth() = 0;
  
  //permanent
  const static int trainingCamp;
  const static int trainingTime;
  const static int softAttack;
  const static int hardAttack;
  const static int defense;
  const static int speed;
  const static int disruption;
  const static int airAttack;
  const static int armor;
  const static int conspicuousness;

  // mutable
  double equipment = 2;
  double food = 1;
  double hp = 0;
};
class Artillery : public Troop
{
public:
  Artillery(std::string id) : Troop(id) {}
  void giveDamage(double, double, double, double, double, Damage &) = 0;
  void takeDamage(double) = 0;
  double getHealth() = 0;
  void increaseHealth(double) = 0;
  
  //permanent
  const static int trainingCamp;
  const static int trainingTime;
  const static int softAttack;
  const static int hardAttack;
  const static int defense;
  const static int speed;
  const static int disruption;
  const static int airAttack;
  const static int armor;
  const static int conspicuousness;

  // mutable
  double equipment = 2;
  double food = 1;
  double hp = 170;
};
class Logistic : public Troop
{
public:
  Logistic(std::string id) : Troop(id) {}
  void takeDamage(double) = 0;
  double getHealth() = 0;
  void increaseHealth(double) = 0;
  
  //permanent
  const static int trainingCamp;
  const static int trainingTime;
  const static int softAttack;
  const static int hardAttack;
  const static int defense;
  const static int speed;
  const static int disruption;
  const static int airAttack;
  const static int armor;
  const static int conspicuousness;

  // mutable
  double equipment = 2;
  double food = 2;
  double hp = 50;
};
class ArmoredCar : public Troop
{
public:
  ArmoredCar(std::string id) : Troop(id) {}
  void giveDamage(double, double, double, double, double, Damage &) = 0;
  void takeDamage(double) = 0;
  double getHealth() = 0;
  void increaseHealth(double) = 0;
  
  //permanent
  const static int trainingCamp;
  const static int trainingTime;
  const static int softAttack;
  const static int hardAttack;
  const static int defense;
  const static int speed;
  const static int disruption;
  const static int airAttack;
  const static int armor;
  const static int conspicuousness;

  // mutable
  double equipment = 10;
  double food = 6;
  double hp = 160;
};
class Tank1 : public Troop
{
public:
  Tank1(std::string id) : Troop(id) {}
  void giveDamage(double, double, double, double, double, Damage &) = 0;
  void takeDamage(double) = 0;
  double getHealth() = 0;
  void increaseHealth(double) = 0;
  
  //permanent
  const static int trainingCamp;
  const static int trainingTime;
  const static int softAttack;
  const static int hardAttack;
  const static int defense;
  const static int speed;
  const static int disruption;
  const static int airAttack;
  const static int armor;
  const static int conspicuousness;

  // mutable
  double equipment = 15;
  double food = 6;
  double hp = 230;
};
class Tank2 : public Troop
{
public:
  Tank2(std::string id) : Troop(id) {}
  void giveDamage(double, double, double, double, double, Damage &) = 0;
  void takeDamage(double) = 0;
  double getHealth() = 0;
  void increaseHealth(double) = 0;
  
  //permanent
  const static int trainingCamp;
  const static int trainingTime;
  const static int softAttack;
  const static int hardAttack;
  const static int defense;
  const static int speed;
  const static int disruption;
  const static int airAttack;
  const static int armor;
  const static int conspicuousness;

  // mutable
  double equipment = 25;
  double food = 8;
  double hp = 30;
};
class TankOshimai : public Troop
{
public:
  TankOshimai(std::string id) : Troop(id) {}
  void giveDamage(double, double, double, double, double, Damage &) = 0;
  void takeDamage(double) = 0;
  double getHealth() = 0;
  void increaseHealth(double) = 0;
  
  //permanent
  const static int trainingCamp;
  const static int trainingTime;
  const static int softAttack;
  const static int hardAttack;
  const static int defense;
  const static int speed;
  const static int disruption;
  const static int airAttack;
  const static int armor;
  const static int conspicuousness;

  // mutable
  double equipment = 50;
  double food = 25;
  double hp = 1000;
};
class Cas : public Troop
{
public:
  Cas(std::string id) : Troop(id) {}
  void giveDamage(double, double, double, double, double, Damage &) = 0;
  void takeDamage(double) = 0;
  double getHealth() = 0;
  void increaseHealth(double) = 0;
  
  //permanent
  const static int trainingCamp;
  const static int trainingTime;
  const static int softAttack;
  const static int hardAttack;
  const static int defense;
  const static int speed;
  const static int disruption;
  const static int airAttack;
  const static int armor;
  const static int conspicuousness;

  // mutable
  double equipment = 10;
  double food = 1;
  double hp = 50;
};
class Fighter : public Troop
{
public:
  Fighter(std::string id) : Troop(id) {}
  void giveDamage(double, double, double, double, double, Damage &) = 0;
  void takeDamage(double) = 0;
  double getHealth() = 0;
  void increaseHealth(double) = 0;
  
  //permanent
  const static int trainingCamp;
  const static int trainingTime;
  const static int softAttack;
  const static int hardAttack;
  const static int defense;
  const static int speed;
  const static int disruption;
  const static int airAttack;
  const static int armor;
  const static int conspicuousness;

  // mutable
  double equipment = 10;
  double food = 1;
  double hp = 130;
};
class Bomber : public Troop
{
public:
  Bomber(std::string id) : Troop(id) {}
  void giveDamage(double, double, double, double, double, Damage &) = 0;
  void takeDamage(double) = 0;
  double getHealth() = 0;
  void increaseHealth(double) = 0;
  
  //permanent
  const static int trainingCamp;
  const static int trainingTime;
  const static int softAttack;
  const static int hardAttack;
  const static int defense;
  const static int speed;
  const static int disruption;
  const static int airAttack;
  const static int armor;
  const static int conspicuousness;

  // mutable
  double equipment = 15;
  double food = 2;
  double hp = 80;
};
class Kamikaze : public Troop
{
public:
  Kamikaze(std::string id) : Troop(id) {}
  void giveDamage(double, double, double, double, double, Damage &) = 0;
  double getHealth() = 0;
  
  //permanent
  const static int trainingCamp;
  const static int trainingTime;
  const static int softAttack;
  const static int hardAttack;
  const static int defense;
  const static int speed;
  const static int disruption;
  const static int airAttack;
  const static int armor;
  const static int conspicuousness;

  // mutable
  double equipment = 10;
  double food = 1;
  double hp = 0;
};
#endif