#ifndef TROOP_H
#define TROOP_H

#include <string>
#include <unordered_map>
#include "../../class/damage/damage.h"

// structure:
// polymorphism: base: troopunit -> derived: troopunit type
// avoid object slicing by storing troop type as pointers (vector looks like vector<Troop*> ... push_back(new derived()) )
// getting equipment/ food/ hp (only present in derived class but not base class) by getters and setters (dynamic casting is costly)
// no need for virtual destructors because no dynamic memory is used within the derived class (can directly call the base destructor)
// using raw pointers and handling delete myself

// get: health, equipment, food
// set: equipment, food (health using takeDamage for minus and increaseHealth for plus)
// static members: everything except food, equipment, hp which are subjected to change
class TroopUnit
{
public:
  // Each troop is uniquely identified with a uuid
  TroopUnit(std::string id, std::string type_) : uuid(id), type(type_) {}

  // food supplied, equipment supplied, disruption, attack debuff, air supremacy, accumulate to Damage struct
  virtual void giveDamage(double, double, double, double, double, Damage &) = 0;

  // damage received is calculated outside of troop
  virtual void takeDamage(double) = 0;
  virtual void increaseHealth(double) = 0;

  virtual void setFood(double) = 0;
  virtual void setEquipment(double) = 0;
  virtual double getHealth() = 0;
  virtual double getFood() = 0;
  virtual double getEquipment() = 0;
  std::string uuid;
  std::string type;

  // state for identifying it as free/ in battle/ in plan/ in army
  std::unordered_map<std::string, bool> state = {{"free", true}, {"battle", false}, {"battlePlan", false}, {"army", false}};

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
class Infantry : public TroopUnit
{
public:
  Infantry(std::string id) : TroopUnit(id, "infantry") {}

  // food supplied, equipment supplied, disruption, attack debuff, air supremacy, accumulate to Damage struct
  void giveDamage(double, double, double, double, double, Damage &);

  // damage received is calculated outside of troop
  void takeDamage(double);
  void increaseHealth(double);

  void setFood(double val) { food = val; }
  void setEquipment(double val) { equipment = val; }
  double getHealth() { return hp; }
  double getFood() { return food; }
  double getEquipment() { return equipment; }

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
private:
  double equipment = 2;
  double food = 2;
  double hp = 100;
};
class Calvary : public TroopUnit
{

public:
  Calvary(std::string id) : TroopUnit(id, "calvary") {}
  void giveDamage(double, double, double, double, double, Damage &);
  void takeDamage(double);
  void increaseHealth(double);

  void setFood(double val) { food = val; }
  void setEquipment(double val) { equipment = val; }
  double getHealth() { return hp; }
  double getFood() { return food; }
  double getEquipment() { return equipment; }

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
private:
  double equipment = 2;
  double food = 2;
  double hp = 150;
};
class SuicideBomber : public TroopUnit
{
public:
  SuicideBomber(std::string id) : TroopUnit(id, "suicideBomber") {}
  void giveDamage(double, double, double, double, double, Damage &);

  // not used
  void takeDamage(double){};
  void increaseHealth(double){};

  void setFood(double val) { food = val; }
  void setEquipment(double val) { equipment = val; }
  double getHealth() { return hp; }
  double getFood() { return food; }
  double getEquipment() { return equipment; }

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
private:
  double equipment = 2;
  double food = 1;
  double hp;
};
class Artillery : public TroopUnit
{
public:
  Artillery(std::string id) : TroopUnit(id, "artillery") {}
  void giveDamage(double, double, double, double, double, Damage &);
  void takeDamage(double);
  void increaseHealth(double);

  void setFood(double val) { food = val; }
  void setEquipment(double val) { equipment = val; }
  double getHealth() { return hp; }
  double getFood() { return food; }
  double getEquipment() { return equipment; }

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
private:
  double equipment = 2;
  double food = 1;
  double hp = 170;
};
class Logistic : public TroopUnit
{
public:
  Logistic(std::string id) : TroopUnit(id, "logistic") {}
  void takeDamage(double);
  void increaseHealth(double);

  // not used
  void giveDamage(double, double, double, double, double, Damage &){};

  void setFood(double val) { food = val; }
  void setEquipment(double val) { equipment = val; }
  double getHealth() { return hp; }
  double getFood() { return food; }
  double getEquipment() { return equipment; }

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
private:
  double equipment = 2;
  double food = 2;
  double hp = 50;
};
class ArmoredCar : public TroopUnit
{
public:
  ArmoredCar(std::string id) : TroopUnit(id, "armoredCar") {}
  void giveDamage(double, double, double, double, double, Damage &);
  void takeDamage(double);
  void increaseHealth(double);

  void setFood(double val) { food = val; }
  void setEquipment(double val) { equipment = val; }
  double getHealth() { return hp; }
  double getFood() { return food; }
  double getEquipment() { return equipment; }

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
private:
  double equipment = 10;
  double food = 6;
  double hp = 160;
};
class Tank1 : public TroopUnit
{
public:
  Tank1(std::string id) : TroopUnit(id, "tank1") {}
  void giveDamage(double, double, double, double, double, Damage &);
  void takeDamage(double);
  void increaseHealth(double);

  void setFood(double val) { food = val; }
  void setEquipment(double val) { equipment = val; }
  double getHealth() { return hp; }
  double getFood() { return food; }
  double getEquipment() { return equipment; }

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
private:
  double equipment = 15;
  double food = 6;
  double hp = 230;
};
class Tank2 : public TroopUnit
{
public:
  Tank2(std::string id) : TroopUnit(id, "tank2") {}
  void giveDamage(double, double, double, double, double, Damage &);
  void takeDamage(double);
  void increaseHealth(double);

  void setFood(double val) { food = val; }
  void setEquipment(double val) { equipment = val; }
  double getHealth() { return hp; }
  double getFood() { return food; }
  double getEquipment() { return equipment; }

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
private:
  double equipment = 25;
  double food = 8;
  double hp = 30;
};
class TankOshimai : public TroopUnit
{
public:
  TankOshimai(std::string id) : TroopUnit(id, "tankOshimai") {}
  void giveDamage(double, double, double, double, double, Damage &);
  void takeDamage(double);
  void increaseHealth(double);

  void setFood(double val) { food = val; }
  void setEquipment(double val) { equipment = val; }
  double getHealth() { return hp; }
  double getFood() { return food; }
  double getEquipment() { return equipment; }

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
private:
  double equipment = 50;
  double food = 25;
  double hp = 1000;
};
class Cas : public TroopUnit
{
public:
  Cas(std::string id) : TroopUnit(id, "cas") {}
  void giveDamage(double, double, double, double, double, Damage &);
  void takeDamage(double);
  void increaseHealth(double);

  void setFood(double val) { food = val; }
  void setEquipment(double val) { equipment = val; }
  double getHealth() { return hp; }
  double getFood() { return food; }
  double getEquipment() { return equipment; }

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
private:
  double equipment = 10;
  double food = 1;
  double hp = 50;
};
class Fighter : public TroopUnit
{
public:
  Fighter(std::string id) : TroopUnit(id, "fighter") {}
  void giveDamage(double, double, double, double, double, Damage &);
  void takeDamage(double);
  void increaseHealth(double);

  void setFood(double val) { food = val; }
  void setEquipment(double val) { equipment = val; }
  double getHealth() { return hp; }
  double getFood() { return food; }
  double getEquipment() { return equipment; }

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
private:
  double equipment = 10;
  double food = 1;
  double hp = 130;
};
class Bomber : public TroopUnit
{
public:
  Bomber(std::string id) : TroopUnit(id, "bomber") {}
  void giveDamage(double, double, double, double, double, Damage &);
  void takeDamage(double);
  void increaseHealth(double);

  void setFood(double val) { food = val; }
  void setEquipment(double val) { equipment = val; }
  double getHealth() { return hp; }
  double getFood() { return food; }
  double getEquipment() { return equipment; }

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
private:
  double equipment = 15;
  double food = 2;
  double hp = 80;
};
class Kamikaze : public TroopUnit
{
public:
  Kamikaze(std::string id) : TroopUnit(id, "kamikaze") {}
  void giveDamage(double, double, double, double, double, Damage &);

  // not used
  void takeDamage(double){};
  void increaseHealth(double){};

  void setFood(double val) { food = val; }
  void setEquipment(double val) { equipment = val; }
  double getHealth() { return hp; }
  double getFood() { return food; }
  double getEquipment() { return equipment; }

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
private:
  double equipment = 10;
  double food = 1;
  double hp;
};
#endif