#ifndef TROOP_H
#define TROOP_H

#include <string>
#include <vector>
#include <unordered_map>
#include "../../class/damage/damage.h"

// structure:
// polymorphism: base: troopunit -> derived: troopunit type
// avoid object slicing by storing troop type as pointers (vector looks like vector<Troop*> ... push_back(new derived()) )
// getting equipment/ food/ hp (only present in derived class but not base class) by getters and setters (dynamic casting is costly)
// no need for virtual destructors because no dynamic memory is used within the derived class (can directly call the base destructor)
// using raw pointers and handling delete myself (TODO: change to shared pointers)

// get: health, equipment, food
// set: equipment, food (health using takeDamage for minus and increaseHealth for plus)
// static members: everything except food, equipment, hp which are subjected to change
class TroopUnit
{
public:
  // Each troop is uniquely identified with a uuid
  TroopUnit(std::string id, std::string type_) : uuid(id), type(type_) {}

  // disruption, air supremacy, land multiplier, air multiplier, tank multiplier, air multiplier 2, accumulate to Damage struct
  virtual void giveDamage(double, double, double, double, double, double, Damage &) = 0;

  // damage received is calculated outside of troop
  virtual void takeDamage(double) = 0;
  virtual void increaseHealth(double) = 0;

  virtual double getHealth() = 0;
  virtual double getFood() = 0;
  virtual double getEquipment() = 0;
  virtual int getSpeed() = 0;
  virtual int getBaseHealth() = 0;
  virtual int getSoftAttack() = 0;
  virtual int getHardAttack() = 0;
  virtual double getAirAttack() = 0;
  virtual int getDisruption() = 0;
  virtual int getConspicuousness() = 0;
  virtual int getDefense() = 0; 
  virtual int getArmor() = 0;
  std::string uuid;
  std::string type;

  // state for identifying it as free/ in battle/ in plan/ in army
  std::unordered_map<std::string, bool> state = {{"free", true}, {"battle", false}, {"battlePlan", false}, {"army", false}};

  // if is getting pointed to by a pointer (only false when the vector array are all falses) (TODO: change to shared pointers)
  // should be replaced by shared pointers
  bool isReferenced = false;
  std::vector<bool> reference;

  double pivotalStrength = 1.0;
  double subsequentialStrength = 1.0;
  double attackDebuff = 0;

  // for battle printing
  bool selected = false;

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
  // const static int baseHp;
};
class Infantry : public TroopUnit
{
public:
  Infantry(std::string id) : TroopUnit(id, "infantry") {}

  // adjusted disruption, air supremacy, mul 1-4, accumulate to Damage struct
  void giveDamage(double, double, double, double, double, double, Damage &);

  // damage received is calculated outside of troop
  void takeDamage(double);
  void increaseHealth(double);

  double getHealth() { return hp; }
  double getFood() { return food; }
  double getEquipment() { return equipment; }
  int getBaseHealth() { return baseHp; }
  int getSoftAttack() { return softAttack; }
  int getHardAttack() { return hardAttack; }
  double getAirAttack() { double strength = 0.6 * this->pivotalStrength + 0.4 *this->subsequentialStrength; return 1.0 * airAttack * strength; }
  int getSpeed() { return speed; }
  int getDisruption() { return disruption; }
  int getConspicuousness() { return conspicuousness; }
  int getDefense() { return defense;}
  int getArmor() { return armor; }
  

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
  const static int baseHp;

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
  void giveDamage(double, double, double, double, double, double, Damage &);
  void takeDamage(double);
  void increaseHealth(double);

  double getHealth() { return hp; }
  double getFood() { return food; }
  double getEquipment() { return equipment; }
  int getBaseHealth() { return baseHp; }
  int getSoftAttack() { return softAttack; }
  int getHardAttack() { return hardAttack; }
  double getAirAttack() { double strength = 0.6 * this->pivotalStrength + 0.4 *this->subsequentialStrength; return 1.0 * airAttack * strength; }
  int getSpeed() { return speed; }
  int getDisruption() { return disruption; }
  int getConspicuousness() { return conspicuousness; }
  int getDefense() { return defense;}
  int getArmor() { return armor; }

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
  const static int baseHp;

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
  void giveDamage(double, double, double, double, double, double, Damage &);

  void takeDamage(double);

  // not used
  void increaseHealth(double){};

  double getHealth() { return hp; }
  double getFood() { return food; }
  double getEquipment() { return equipment; }
  int getBaseHealth() { return baseHp; }
  int getSoftAttack() { return softAttack; }
  int getHardAttack() { return hardAttack; }
  double getAirAttack() { double strength = 0.6 * this->pivotalStrength + 0.4 *this->subsequentialStrength; return 1.0 * airAttack * strength; }
  int getSpeed() { return speed; }
  int getDisruption() { return disruption; }
  int getConspicuousness() { return conspicuousness; }
  int getDefense() { return defense;}
  int getArmor() { return armor; }

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
  const static int baseHp;

  // mutable
private:
  double equipment = 2;
  double food = 1;
  double hp = 0;
};
class Artillery : public TroopUnit
{
public:
  Artillery(std::string id) : TroopUnit(id, "artillery") {}
  void giveDamage(double, double, double, double, double, double, Damage &);
  void takeDamage(double);
  void increaseHealth(double);

  double getHealth() { return hp; }
  double getFood() { return food; }
  double getEquipment() { return equipment; }
  int getBaseHealth() { return baseHp; }
  int getSoftAttack() { return softAttack; }
  int getHardAttack() { return hardAttack; }
  double getAirAttack() { double strength = 0.6 * this->pivotalStrength + 0.4 *this->subsequentialStrength; return 1.0 * airAttack * strength; }
  int getSpeed() { return speed; }
  int getDisruption() { return disruption; }
  int getConspicuousness() { return conspicuousness; }
  int getDefense() { return defense;}
  int getArmor() { return armor; }

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
  const static int baseHp;

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
  void giveDamage(double disruption, double airSupremacy, double mul1, double mul2, double mul3, double mul4, Damage &damage){}

  double getHealth() { return hp; }
  double getFood() { return food; }
  double getEquipment() { return equipment; }
  int getBaseHealth() { return baseHp; }
  int getSoftAttack() { return softAttack; }
  int getHardAttack() { return hardAttack; }
  double getAirAttack() { double strength = 0.6 * this->pivotalStrength + 0.4 *this->subsequentialStrength; return 1.0 * airAttack * strength; }
  int getSpeed() { return speed; }
  int getDisruption() { return disruption; }
  int getConspicuousness() { return conspicuousness; }
  int getDefense() { return defense;}
  int getArmor() { return armor; }

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
  const static int baseHp;

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
  void giveDamage(double, double, double, double, double, double, Damage &);
  void takeDamage(double);
  void increaseHealth(double);

  double getHealth() { return hp; }
  double getFood() { return food; }
  double getEquipment() { return equipment; }
  int getBaseHealth() { return baseHp; }
  int getSoftAttack() { return softAttack; }
  int getHardAttack() { return hardAttack; }
  double getAirAttack() { double strength = 0.6 * this->pivotalStrength + 0.4 *this->subsequentialStrength; return 1.0 * airAttack * strength; }
  int getSpeed() { return speed; }
  int getDisruption() { return disruption; }
  int getConspicuousness() { return conspicuousness; }
  int getDefense() { return defense;}
  int getArmor() { return armor; }

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
  const static int baseHp;

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
  void giveDamage(double, double, double, double, double, double, Damage &);
  void takeDamage(double);
  void increaseHealth(double);

  double getHealth() { return hp; }
  double getFood() { return food; }
  double getEquipment() { return equipment; }
  int getBaseHealth() { return baseHp; }
  int getSoftAttack() { return softAttack; }
  int getHardAttack() { return hardAttack; }
  double getAirAttack() { double strength = 0.6 * this->pivotalStrength + 0.4 *this->subsequentialStrength; return 1.0 * airAttack * strength; }
  int getSpeed() { return speed; }
  int getDisruption() { return disruption; }
  int getConspicuousness() { return conspicuousness; }
  int getDefense() { return defense;}
  int getArmor() { return armor; }

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
  const static int baseHp;

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
  void giveDamage(double, double, double, double, double, double, Damage &);
  void takeDamage(double);
  void increaseHealth(double);

  double getHealth() { return hp; }
  double getFood() { return food; }
  double getEquipment() { return equipment; }
  int getBaseHealth() { return baseHp; }
  int getSoftAttack() { return softAttack; }
  int getHardAttack() { return hardAttack; }
  double getAirAttack() { double strength = 0.6 * this->pivotalStrength + 0.4 *this->subsequentialStrength; return 1.0 * airAttack * strength; }
  int getSpeed() { return speed; }
  int getDisruption() { return disruption; }
  int getConspicuousness() { return conspicuousness; }
  int getDefense() { return defense;}
  int getArmor() { return armor; }

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
  const static int baseHp;

  // mutable
private:
  double equipment = 25;
  double food = 8;
  double hp = 300;
};
class TankOshimai : public TroopUnit
{
public:
  TankOshimai(std::string id) : TroopUnit(id, "tankOshimai") {}
  void giveDamage(double, double, double, double, double, double, Damage &);
  void takeDamage(double);
  void increaseHealth(double);

  double getHealth() { return hp; }
  double getFood() { return food; }
  double getEquipment() { return equipment; }
  int getBaseHealth() { return baseHp; }
  int getSoftAttack() { return softAttack; }
  int getHardAttack() { return hardAttack; }
  double getAirAttack() { double strength = 0.6 * this->pivotalStrength + 0.4 *this->subsequentialStrength; return 1.0 * airAttack * strength; }
  int getSpeed() { return speed; }
  int getDisruption() { return disruption; }
  int getConspicuousness() { return conspicuousness; }
  int getDefense() { return defense;}
  int getArmor() { return armor; }

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
  const static int baseHp;

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
  void giveDamage(double, double, double, double, double, double, Damage &);
  void takeDamage(double);
  void increaseHealth(double);

  double getHealth() { return hp; }
  double getFood() { return food; }
  double getEquipment() { return equipment; }
  int getBaseHealth() { return baseHp; }
  int getSoftAttack() { return softAttack; }
  int getHardAttack() { return hardAttack; }
  double getAirAttack() { double strength = 0.6 * this->pivotalStrength + 0.4 *this->subsequentialStrength; return 1.0 * airAttack * strength; }
  int getSpeed() { return speed; }
  int getDisruption() { return disruption; }
  int getConspicuousness() { return conspicuousness; }
  int getDefense() { return defense;}
  int getArmor() { return armor; }

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
  const static int baseHp;

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
  void giveDamage(double, double, double, double, double, double, Damage &);
  void takeDamage(double);
  void increaseHealth(double);

  double getHealth() { return hp; }
  double getFood() { return food; }
  double getEquipment() { return equipment; }
  int getBaseHealth() { return baseHp; }
  int getSoftAttack() { return softAttack; }
  int getHardAttack() { return hardAttack; }
  double getAirAttack() { double strength = 0.6 * this->pivotalStrength + 0.4 *this->subsequentialStrength; return 1.0 * airAttack * strength; }
  int getSpeed() { return speed; }
  int getDisruption() { return disruption; }
  int getConspicuousness() { return conspicuousness; }
  int getDefense() { return defense;}
  int getArmor() { return armor; }

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
  const static int baseHp;

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
  void giveDamage(double, double, double, double, double, double, Damage &);
  void takeDamage(double);
  void increaseHealth(double);

  double getHealth() { return hp; }
  double getFood() { return food; }
  double getEquipment() { return equipment; }
  int getBaseHealth() { return baseHp; }
  int getSoftAttack() { return softAttack; }
  int getHardAttack() { return hardAttack; }
  double getAirAttack() { double strength = 0.6 * this->pivotalStrength + 0.4 *this->subsequentialStrength; return 1.0 * airAttack * strength; }
  int getSpeed() { return speed; }
  int getDisruption() { return disruption; }
  int getConspicuousness() { return conspicuousness; }
  int getDefense() { return defense;}
  int getArmor() { return armor; }

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
  const static int baseHp;

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
  void giveDamage(double, double, double, double, double, double, Damage &);

  void takeDamage(double);
  // not used
  void increaseHealth(double){};

  double getHealth() { return hp; }
  double getFood() { return food; }
  double getEquipment() { return equipment; }
  int getBaseHealth() { return baseHp; }
  int getSoftAttack() { return softAttack; }
  int getHardAttack() { return hardAttack; }
  double getAirAttack() { double strength = 0.6 * this->pivotalStrength + 0.4 *this->subsequentialStrength; return 1.0 * airAttack * strength; }
  int getSpeed() { return speed; }
  int getDisruption() { return disruption; }
  int getConspicuousness() { return conspicuousness; }
  int getDefense() { return defense;}
  int getArmor() { return armor; }

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
  const static int baseHp;

  // mutable
private:
  double equipment = 10;
  double food = 1;
  double hp = 0;
};

#endif