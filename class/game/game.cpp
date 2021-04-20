#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <future>
#include <mutex>
#include <fstream>
#include <cassert>
#include <cstdlib>
#include <cmath>

#include "gameStruct.h"
#include "game.h"
#include "../../lib/uuid/uuid.hpp"
#include "../../io/io.h"
#include "../../data/troop/troop.h"

// decrement remain by 1 every interval until 0
Progress::Progress(int time, int &interval_, int fps_, bool &paused_)
    : remain(time), milliRemain(time * 1000), interval(interval_), fps(fps_), paused(paused_)
{
}
Progress::Progress(double time, int &interval_, int fps_, bool &paused_)
    : milliRemain(time), interval(interval_), fps(fps_), paused(paused_)
{
  this->remain = std::max((int)std::ceil(this->milliRemain / 1000), 0);
}
void Progress::start(std::mutex &lg3)
{
  while (this->milliRemain > 0)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000 / this->fps));

    if (this->paused)
      continue;
    this->milliRemain -= 1000 * 1000 / this->fps / this->interval;
    lg3.lock();
    this->remain = std::max((int)std::ceil(1.0 * this->milliRemain / 1000), 0);
    lg3.unlock();
  }
}

Game::~Game()
{
}
Game::Game(std::unordered_map<std::string, int> setting_, const int screenX_, const int screenY_, const int fps_, const std::string path_) : setting(setting_), screenWidth(screenX_), screenHeight(screenY_), fps(fps_), path(path_)
{
  this->resource = new data::Resource();
  this->building = new data::Building();
  this->troop = new data::Troop();
  this->army = new data::Army();
  this->research = new data::Research();
  this->battle = new data::Battle();
  this->enemies = new data::Enemies();
}

void Game::endGame(bool restart)
{
  this->lguser.lock();
  this->lg3.lock();
  this->lg.lock();
  this->stopLoopPrintStatus();
  this->stopLoopPrintBuild();
  this->stopLoopPrintResearch();
  this->stopLoopPrintTroop();
  for (auto i : this->enemies->totalEnemies)
  {
    for (auto j : i->map)
    {
      for (auto k : j)
      {
        if (k != NULL)
        {
          k->lg.lock();
          for (auto m : k->battle)
          {
            delete m;
            m = NULL;
          }
          for (auto m : k->totalFoeTroop)
            if (m != NULL)
            {
              delete m;
              m = NULL;
            }
          for (auto m : k->totalFoeArmy)
          {
            if (m != NULL)
            {
              for (auto n : m->formation)
                for (auto p : n)
                  if (p != NULL)
                  {
                    delete p;
                    p = NULL;
                  }
              delete m;
              m = NULL;
            }
          }
          k->lg.unlock();
        }
        delete k;
        k = NULL;
      }
    }
    delete i;
    i = NULL;
  }
  for (auto i : this->troop->allTroop)
  {
    if (i != NULL)
    {
      delete i;
      i = NULL;
    }
  }
  for (auto i : this->army->total)
  {
    if (i.second != NULL)
    {
      delete i.second;
      i.second = NULL;
    }
  }
  for (auto i : this->building->progress)
    if (i.second != NULL)
    {
      delete i.second;
      i.second = NULL;
    }
  for (auto i : this->troop->progress)
    if (i.second != NULL)
    {
      delete i.second;
      i.second = NULL;
    }
  for (auto i : this->research->progress)
    if (i.second != NULL)
    {
      delete i.second;
      i.second = NULL;
    }

  bool levelPassed = true || this->enemies->totalEnemies.size() == this->enemies->defeated;

  delete this->resource;
  delete this->building;
  delete this->troop;
  delete this->army;
  delete this->research;
  delete this->battle;
  delete this->enemies;
  this->lg.unlock();
  this->lg3.unlock();
  this->lguser.unlock();

  if (restart)
    return;

  std::cout << "\033c" << std::endl;
  std::cout << "Game ended. Press spacebar to continue..." << std::endl
            << std::endl;

  if (this->successAction && levelPassed)
  {
    std::ifstream file("save/progress.txt", std::ios::in);

    std::vector<std::string> content;
    if (file.fail())
    {
      std::cout << "Cannot save progress to save/progress.txt! Try cloning the repo again." << std::endl;
    }
    else
    {
      std::string temp;
      while (std::getline(file, temp))
        content.push_back(temp);
      content[this->success[0]][this->success[1]] = std::to_string(this->success[2])[0];
      file.close();

      std::ofstream out("save/progress.txt", std::ios::out | std::ios::trunc);
      for (int i = 0; i < content.size() - 1; i++)
        out << content[i] << std::endl;
      out << content.back();
      out.close();
    }
  }
}

void Game::speed(int &gamePhase, int prevPhase)
{
  this->stopTimer();
  this->timeChosen = (this->timeChosen + 1) % this->timeRange.size();
  this->setting["speed"] = this->timeRange[this->timeChosen];
  this->timer(this->setting["speed"]);
  gamePhase = prevPhase;
  (this->*this->print[this->gamePhase])(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
}

void Game::pause(int &gamePhase, int prevPhase)
{
  this->paused = !this->paused;
  gamePhase = prevPhase;
  (this->*this->print[this->gamePhase])(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
}

void Game::restart(int &gamePhase, int prevPhase)
{
  this->stopTimer();
  this->gameOver = true;
  this->endGame(true);
  gamePhase = -999;
}

void Game::quit(int &gamePhase, int prevPhase)
{
  this->stopTimer();
  this->gameOver = true;
  this->endGame(true);
  gamePhase = -998;
}

// fetch level data from .dat files
int Game::fetch()
{
  std::cout << "\033[2J\033[1;1H" << std::endl;
  std::fstream in(this->path, std::ios::in);

  std::vector<std::string> indexToTroop = {"infantry", "calvary", "artillery", "logistic", "armoredCar", "tank1", "tank2", "tankOshimai", "cas", "fighter", "bomber"};
  std::vector<std::string> indexToTroop2 = {"infantry", "calvary", "suicideBomber", "artillery", "logistic", "armoredCar", "tank1", "tank2", "tankOshimai", "cas", "fighter", "bomber", "kamikaze"};
  std::unordered_map<std::string, std::function<TroopUnit *()>> troopToInstance = {
      {"infantry", [&]() { return new Infantry(this->uuid()); }},
      {"calvary", [&]() { return new Calvary(this->uuid()); }},
      {"suicideBomber", [&]() { return new SuicideBomber(this->uuid()); }},
      {"artillery", [&]() { return new Artillery(this->uuid()); }},
      {"logistic", [&]() { return new Logistic(this->uuid()); }},
      {"armoredCar", [&]() { return new ArmoredCar(this->uuid()); }},
      {"tank1", [&]() { return new Tank1(this->uuid()); }},
      {"tank2", [&]() { return new Tank2(this->uuid()); }},
      {"tankOshimai", [&]() { return new TankOshimai(this->uuid()); }},
      {"cas", [&]() { return new Cas(this->uuid()); }},
      {"fighter", [&]() { return new Fighter(this->uuid()); }},
      {"bomber", [&]() { return new Bomber(this->uuid()); }},
      {"kamikaze", [&]() { return new Kamikaze(this->uuid()); }}};
  std::unordered_map<std::string, int> troopToAirport = {
      {"infantry", 0},
      {"calvary", 0},
      {"suicideBomber", 0},
      {"artillery", 0},
      {"logistic", 0},
      {"armoredCar", 0},
      {"tank1", 0},
      {"tank2", 0},
      {"tankOshimai", 0},
      {"cas", 1},
      {"fighter", 1},
      {"bomber", 1},
      {"kamikaze", 1}};

  std::string input;
  while (std::getline(in, input))
  {
    int index = input.find(":");
    int index2 = input.find("-");
    int index3 = input.find("=");
    assert(index != std::string::npos);
    assert(index2 != std::string::npos);
    assert(index3 != std::string::npos);

    std::string operand = input.substr(0, index);
    if (operand == "set")
    {
      std::string operand2 = input.substr(index + 1, index2 - index - 1);
      if (operand2 == "food")
        this->resource->food = std::stod(input.substr(index3 + 1).c_str());
      else if (operand2 == "equipment")
        this->resource->equipment = std::stod(input.substr(index3 + 1).c_str());
      else if (operand2 == "manpower")
        this->resource->manpower = std::atoi(input.substr(index3 + 1).c_str());
      else if (operand2 == "baseLand")
        this->resource->baseLand = std::stod(input.substr(index3 + 1).c_str());
      else if (operand2 == "usedLand")
        this->resource->usedLand = std::stod(input.substr(index3 + 1).c_str());
      else if (operand2 == "capturedLand")
        this->resource->capturedLand = std::stod(input.substr(index3 + 1).c_str());
      else if (operand2 == "camp")
        this->resource->camp = std::atoi(input.substr(index3 + 1).c_str());
      else if (operand2 == "airport1")
        this->resource->airport = std::atoi(input.substr(index3 + 1).c_str());
      else if (operand2 == "baseLandMul")
        this->resource->baseLandMul = std::stod(input.substr(index3 + 1));
      else if (operand2 == "baseLandTroopMul")
        this->resource->baseLandTroopMul = std::stod(input.substr(index3 + 1));
      else if (operand2 == "baseAirTroopMul")
        this->resource->baseAirTroopMul = std::stod(input.substr(index3 + 1));
      else if (operand2 == "baseTankMul")
        this->resource->baseTankMul = std::stod(input.substr(index3 + 1));
      else if (operand2 == "baseAirToopMul2")
        this->resource->baseAirToopMul2 = std::stod(input.substr(index3 + 1));
      else if (operand2 == "baseTrainingTimeMul")
        this->resource->baseTrainingTimeMul = std::stod(input.substr(index3 + 1));
      else if (operand2 == "baseRecovery")
        this->resource->baseRecovery = std::stod(input.substr(index3 + 1));
      else if (operand2 == "baseRecoveryDiff")
        this->resource->baseRecoveryDiff = std::stod(input.substr(index3 + 1));
      else if (operand2 == "farm")
      {
        int sep1 = input.find(',', index3);
        int sep2 = input.find(',', sep1 + 1);
        assert(sep1 != std::string::npos);
        assert(sep2 != std::string::npos);
        this->building->farm[0] = std::atoi(input.substr(index3 + 1, sep1 - index3 - 1).c_str());
        this->building->farm[1] = std::atoi(input.substr(sep1 + 1, sep2 - sep1 - 1).c_str());
        this->building->farm[2] = std::atoi(input.substr(sep2 + 1).c_str());
      }
      else if (operand2 == "militaryFactory")
      {
        int sep1 = input.find(',', index3);
        int sep2 = input.find(',', sep1 + 1);
        assert(sep1 != std::string::npos);
        assert(sep2 != std::string::npos);
        this->building->militaryFactory[0] = std::atoi(input.substr(index3 + 1, sep1 - index3 - 1).c_str());
        this->building->militaryFactory[1] = std::atoi(input.substr(sep1 + 1, sep2 - sep1 - 1).c_str());
        this->building->militaryFactory[2] = std::atoi(input.substr(sep2 + 1).c_str());
      }
      else if (operand2 == "civilianFactory")
      {
        int sep1 = input.find(',', index3);
        int sep2 = input.find(',', sep1 + 1);
        assert(sep1 != std::string::npos);
        assert(sep2 != std::string::npos);
        this->building->civilianFactory[0] = std::atoi(input.substr(index3 + 1, sep1 - index3 - 1).c_str());
        this->building->civilianFactory[1] = std::atoi(input.substr(sep1 + 1, sep2 - sep1 - 1).c_str());
        this->building->civilianFactory[2] = std::atoi(input.substr(sep2 + 1).c_str());
      }
      else if (operand2 == "trainingCamp")
        this->building->trainingCamp[0] = std::atoi(input.substr(index3 + 1).c_str());
      else if (operand2 == "airport2")
        this->building->airport[0] = std::atoi(input.substr(index3 + 1).c_str());
      else if (operand2 == "farmU")
      {
        int sep1 = input.find(',', index3);
        int sep2 = input.find(',', sep1 + 1);
        assert(sep1 != std::string::npos);
        assert(sep2 != std::string::npos);
        this->building->farmU[0] = std::atoi(input.substr(index3 + 1, sep1 - index3 - 1).c_str());
        this->building->farmU[1] = std::atoi(input.substr(sep1 + 1, sep2 - sep1 - 1).c_str());
        this->building->farmU[2] = std::atoi(input.substr(sep2 + 1).c_str());
      }
      else if (operand2 == "civilianFactoryU")
      {
        int sep1 = input.find(',', index3);
        int sep2 = input.find(',', sep1 + 1);
        assert(sep1 != std::string::npos);
        assert(sep2 != std::string::npos);
        this->building->civilianFactoryU[0] = std::atoi(input.substr(index3 + 1, sep1 - index3 - 1).c_str());
        this->building->civilianFactoryU[1] = std::atoi(input.substr(sep1 + 1, sep2 - sep1 - 1).c_str());
        this->building->civilianFactoryU[2] = std::atoi(input.substr(sep2 + 1).c_str());
      }
      else if (operand2 == "militaryFactoryU")
      {
        int sep1 = input.find(',', index3);
        int sep2 = input.find(',', sep1 + 1);
        assert(sep1 != std::string::npos);
        assert(sep2 != std::string::npos);
        this->building->militaryFactoryU[0] = std::atoi(input.substr(index3 + 1, sep1 - index3 - 1).c_str());
        this->building->militaryFactoryU[1] = std::atoi(input.substr(sep1 + 1, sep2 - sep1 - 1).c_str());
        this->building->militaryFactoryU[2] = std::atoi(input.substr(sep2 + 1).c_str());
      }
      else if (operand2 == "farmR")
      {
        int sep1 = input.find(',', index3);
        int sep2 = input.find(',', sep1 + 1);
        assert(sep1 != std::string::npos);
        assert(sep2 != std::string::npos);
        this->research->farm[0] = std::atoi(input.substr(index3 + 1, sep1 - index3 - 1).c_str());
        this->research->farm[1] = std::atoi(input.substr(sep1 + 1, sep2 - sep1 - 1).c_str());
        this->research->farm[2] = std::atoi(input.substr(sep2 + 1).c_str());
      }
      else if (operand2 == "divisionOfLaborR")
      {
        int sep1 = input.find(',', index3);
        int sep2 = input.find(',', sep1 + 1);
        assert(sep1 != std::string::npos);
        assert(sep2 != std::string::npos);
        this->research->divisionOfLabor[0] = std::atoi(input.substr(index3 + 1, sep1 - index3 - 1).c_str());
        this->research->divisionOfLabor[1] = std::atoi(input.substr(sep1 + 1, sep2 - sep1 - 1).c_str());
        this->research->divisionOfLabor[2] = std::atoi(input.substr(sep2 + 1).c_str());
      }
      else if (operand2 == "productionLineR")
      {
        int sep1 = input.find(',', index3);
        int sep2 = input.find(',', sep1 + 1);
        assert(sep1 != std::string::npos);
        assert(sep2 != std::string::npos);
        this->research->productionLine[0] = std::atoi(input.substr(index3 + 1, sep1 - index3 - 1).c_str());
        this->research->productionLine[1] = std::atoi(input.substr(sep1 + 1, sep2 - sep1 - 1).c_str());
        this->research->productionLine[2] = std::atoi(input.substr(sep2 + 1).c_str());
      }
      else if (operand2 == "landDoctrineR")
      {
        int sep1 = input.find(',', index3);
        int sep2 = input.find(',', sep1 + 1);
        assert(sep1 != std::string::npos);
        assert(sep2 != std::string::npos);
        this->research->landDoctrine[0] = std::atoi(input.substr(index3 + 1, sep1 - index3 - 1).c_str());
        this->research->landDoctrine[1] = std::atoi(input.substr(sep1 + 1, sep2 - sep1 - 1).c_str());
        this->research->landDoctrine[2] = std::atoi(input.substr(sep2 + 1).c_str());
      }
      else if (operand2 == "airDoctrineR")
      {
        int sep1 = input.find(',', index3);
        int sep2 = input.find(',', sep1 + 1);
        assert(sep1 != std::string::npos);
        assert(sep2 != std::string::npos);
        this->research->airDoctrine[0] = std::atoi(input.substr(index3 + 1, sep1 - index3 - 1).c_str());
        this->research->airDoctrine[1] = std::atoi(input.substr(sep1 + 1, sep2 - sep1 - 1).c_str());
        this->research->airDoctrine[2] = std::atoi(input.substr(sep2 + 1).c_str());
      }
      else if (operand2 == "urbanizationR")
      {
        int sep1 = input.find(',', index3);
        int sep2 = input.find(',', sep1 + 1);
        assert(sep1 != std::string::npos);
        assert(sep2 != std::string::npos);
        this->research->urbanization[0] = std::atoi(input.substr(index3 + 1, sep1 - index3 - 1).c_str());
        this->research->urbanization[1] = std::atoi(input.substr(sep1 + 1, sep2 - sep1 - 1).c_str());
        this->research->urbanization[2] = std::atoi(input.substr(sep2 + 1).c_str());
      }
      else if (operand2 == "weaponR")
      {
        int sep1 = input.find(',', index3);
        int sep2 = input.find(',', sep1 + 1);
        assert(sep1 != std::string::npos);
        assert(sep2 != std::string::npos);
        this->research->weapon[0] = std::atoi(input.substr(index3 + 1, sep1 - index3 - 1).c_str());
        this->research->weapon[1] = std::atoi(input.substr(sep1 + 1, sep2 - sep1 - 1).c_str());
        this->research->weapon[2] = std::atoi(input.substr(sep2 + 1).c_str());
      }
      else if (operand2 == "trainingR")
      {
        int sep1 = input.find(',', index3);
        int sep2 = input.find(',', sep1 + 1);
        assert(sep1 != std::string::npos);
        assert(sep2 != std::string::npos);
        this->research->training[0] = std::atoi(input.substr(index3 + 1, sep1 - index3 - 1).c_str());
        this->research->training[1] = std::atoi(input.substr(sep1 + 1, sep2 - sep1 - 1).c_str());
        this->research->training[2] = std::atoi(input.substr(sep2 + 1).c_str());
      }
      else if (operand2 == "recoveryR")
      {
        int sep1 = input.find(',', index3);
        int sep2 = input.find(',', sep1 + 1);
        assert(sep1 != std::string::npos);
        assert(sep2 != std::string::npos);
        this->research->recovery[0] = std::atoi(input.substr(index3 + 1, sep1 - index3 - 1).c_str());
        this->research->recovery[1] = std::atoi(input.substr(sep1 + 1, sep2 - sep1 - 1).c_str());
        this->research->recovery[2] = std::atoi(input.substr(sep2 + 1).c_str());
      }
      std::cout << "Setting " << operand2 << " done" << std::endl;
    }
    else if (operand == "success")
    {
      this->successAction = true;
      this->success.push_back(std::atoi(input.substr(index + 1, index2 - index - 1).c_str()));
      this->success.push_back(std::atoi(input.substr(index2 + 1, index3 - index2 - 1).c_str()));
      this->success.push_back(std::atoi(input.substr(index3 + 1).c_str()));
    }
    else if (operand == "army")
    {
      std::string name = input.substr(index + 1, index2 - index - 1);
      ArmyUnit *temp = new ArmyUnit(name);
      int armySep = input.find("$");
      int armyEnd = input.find("$", armySep + 1);

      std::vector<int> sepArmy = {(int)input.find(",", armySep)};
      for (int i = 0; i < 14; i++)
        sepArmy.push_back((int)input.find(",", sepArmy.back() + 1));
      for (int i = 0; i < 15; i++)
        assert(sepArmy[i] != std::string::npos);

      int brackettemp = input.find(")", armySep);
      if (brackettemp + 1 != sepArmy[0])
      {
        temp->addTroop(0, 0, indexToTroop2[std::atoi(input.substr(brackettemp + 1, sepArmy[0] - brackettemp - 1).c_str())], this->troop, this->resource);
      }
      for (int i = 0; i < 14; i++)
      {
        brackettemp = input.find(")", sepArmy[i]);
        if (brackettemp + 1 != sepArmy[i + 1])
        {
          temp->addTroop((i + 1) / 4, (i + 1) % 4, indexToTroop2[std::atoi(input.substr(brackettemp + 1, sepArmy[i + 1] - brackettemp - 1).c_str())], this->troop, this->resource);
        }
      }
      brackettemp = input.find(")", sepArmy[14]);
      if (brackettemp + 1 != armyEnd)
      {
        temp->addTroop(3, 3, indexToTroop2[std::atoi(input.substr(brackettemp + 1, armyEnd - brackettemp - 1).c_str())], this->troop, this->resource);
      }
      this->army->total[name] = temp;
      std::cout << "Making army done" << std::endl;
    }
    else if (operand == "troop")
    {
      std::cout << this->uuid() << std::endl;
      std::cout << this->uuid() << std::endl;
      std::cout << this->uuid() << std::endl;
      std::cout << this->uuid() << std::endl;
      std::cout << this->uuid() << std::endl;
      std::cout << this->uuid() << std::endl;
      std::cout << this->uuid() << std::endl;
      std::string type = input.substr(index + 1, index2 - index - 1);
      int num = std::atoi(input.substr(index3 + 1).c_str());
      for (int i = 0; i < num; i++)
      {
        // auto ptr = troopToInstance[type]();
        this->trainBase(
            type, [&troopToInstance, type](data::Resource &resource, data::Troop &troop) {
              troop.allTroop.push_back(troopToInstance[type]());
              troop.totalTroops++;
              troop.totalFoodRequired += troop.allTroop.back()->getFood();
              troop.totalEquipmentRequired += troop.allTroop.back()->getEquipment();
              troop.helper2[type](0, 1);
            },
            0, troopToAirport[type], 0, 1);
      }
      std::cout << "Creating " << type << " done" << std::endl;
      assert(0);
    }
    else if (operand == "enemy")
    {
      std::string modifier = input.substr(index + 1, index2 - index - 1);
      if (modifier == "create")
        this->enemies->totalEnemies.push_back(new Enemy(input.substr(index3 + 1), this->enemies->defeated));
      bool defeated = std::atoi(input.substr(index2 + 1, index3 - index2 - 1).c_str());
      if (defeated)
      {
        this->enemies->defeated++;
        this->enemies->totalEnemies.back()->capitulated = true;
      }
      std::cout << "Creating countries done" << std::endl;
    }
    else if (operand == "map")
    {
      std::string country = input.substr(index + 1, index2 - index - 1);
      std::cout << "Opening map done" << std::endl;

      int index = -1;
      for (int i = 0; i < this->enemies->totalEnemies.size(); i++)
        if (this->enemies->totalEnemies[i]->name == country)
        {
          index = i;
          break;
        }
      assert(index != -1);

      if (input.substr(index2 + 1, index3 - index2 - 1) == "start")
      {
        std::vector<std::vector<Block *>> mapA;
        int totalLand = 0;

        std::string map;
        std::getline(in, map);
        while (map != "end")
        {
          int i1 = map.find(",");
          int i2 = map.find(":");
          int i3 = map.find(";");
          assert(i1 != std::string::npos);
          assert(i2 != std::string::npos);
          assert(i3 != std::string::npos);

          int x = std::atoi(map.substr(0, i1).c_str());
          int y = std::atoi(map.substr(i1 + 1, i2 - i1 - 1).c_str());

          std::cout << "Instantiating block: " << x << " " << y << std::endl;

          if (mapA.size() < x + 1)
            mapA.push_back({});
          if (i2 + 1 == i3)
            mapA.back().push_back(NULL);
          else
          {
            totalLand++;
            std::cout << "Map size: " << mapA.size() << " " << mapA.back().size() << std::endl;
            mapA.back().push_back(new Block(this->troop, this->resource, this->battle, this->enemies->totalEnemies.back()->capturedLand, this->enemies->totalEnemies.back()->capitulated, this->enemies->totalEnemies.back()->battlingRegions, this->enemies->totalEnemies[index]->totalLand, this->enemies->totalEnemies[index]->defeated, mapA.size() - 1, mapA.back().size()));
            mapA.back().back()->country = country;

            std::vector<int> sep = {(int)map.find(",", i2)};
            for (int i = 0; i < 10; i++)
              sep.push_back((int)map.find(",", sep.back() + 1));
            for (int i = 0; i < 11; i++)
              assert(sep[i] != std::string::npos);

            std::string name = map.substr(i2 + 1, sep[0] - i2 - 1);
            mapA.back().back()->name = name;

            std::cout << "Block name done" << std::endl;

            for (int i = 0; i < 11; i++)
            {
              int num = std::atoi(map.substr(sep[i] + 1, sep[i + 1] - sep[i] - 1).c_str());
              mapA.back().back()->foeCount[indexToTroop[i]] += num;
              mapA.back().back()->totalFoe += num;
              for (int j = 0; j < num; j++)
                mapA.back().back()->totalFoeTroop.push_back(troopToInstance[indexToTroop[i]]());
            }

            std::cout << "Block troop done" << std::endl;

            int armySep = map.find("$");
            while (armySep != std::string::npos)
            {
              mapA.back().back()->foeCount["army"]++;
              std::string uid = this->uuid();
              mapA.back().back()->totalFoeArmy.push_back(new ArmyUnit(uid));

              int armyEnd = map.find("$", armySep + 1);

              std::vector<int> sepArmy = {(int)map.find(",", armySep)};
              for (int i = 0; i < 14; i++)
                sepArmy.push_back((int)map.find(",", sepArmy.back() + 1));
              for (int i = 0; i < 15; i++)
                assert(sepArmy[i] != std::string::npos);
              int brackettemp = map.find(")", armySep);
              if (brackettemp + 1 != sepArmy[0])
              {
                mapA.back().back()->totalFoe++;
                std::string type = indexToTroop[std::atoi(map.substr(brackettemp + 1, sepArmy[0] - brackettemp - 1).c_str())];
                mapA.back().back()->totalFoeArmy.back()->addTroopM(0, 0, troopToInstance[type]());
                mapA.back().back()->foeCount[type]++;
              }
              for (int i = 0; i < 14; i++)
              {
                brackettemp = map.find(")", sepArmy[i]);
                if (brackettemp + 1 != sepArmy[i + 1])
                {
                  mapA.back().back()->totalFoe++;
                  std::string type = indexToTroop[std::atoi(map.substr(brackettemp + 1, sepArmy[i + 1] - brackettemp - 1).c_str())];
                  mapA.back().back()->totalFoeArmy.back()->addTroopM((i + 1) / 4, (i + 1) % 4, troopToInstance[type]());
                  mapA.back().back()->foeCount[type]++;
                }
              }
              brackettemp = map.find(")", sepArmy[14]);
              if (brackettemp + 1 != armyEnd)
              {
                mapA.back().back()->totalFoe++;
                std::string type = indexToTroop[std::atoi(map.substr(brackettemp + 1, armyEnd - brackettemp - 1).c_str())];
                mapA.back().back()->totalFoeArmy.back()->addTroopM(3, 3, troopToInstance[type]());
                mapA.back().back()->foeCount[type]++;
              }
              armySep = map.find("$", armyEnd + 1);
            }

            std::cout << "Block army done" << std::endl;

            int rewardStart = map.find("|", sep.back());
            int rewardEnd = map.find("|", rewardStart + 1);

            assert(rewardStart != std::string::npos);
            assert(rewardEnd != std::string::npos);

            std::vector<int> rewardSep = {(int)map.find(",", rewardStart)};
            for (int i = 0; i < 4; i++)
              rewardSep.push_back(map.find(",", rewardSep.back() + 1));
            for (int i = 0; i < 5; i++)
              assert(rewardSep[i] != std::string::npos);

            mapA.back().back()->acquirable["farm"] = std::atoi(map.substr(rewardStart + 1, rewardSep[0] - rewardStart - 1).c_str());
            mapA.back().back()->acquirable["civilianFactory"] = std::atoi(map.substr(rewardSep[0] + 1, rewardSep[1] - rewardSep[0] - 1).c_str());
            mapA.back().back()->acquirable["militaryFactory"] = std::atoi(map.substr(rewardSep[1] + 1, rewardSep[2] - rewardSep[1] - 1).c_str());
            mapA.back().back()->acquirable["trainingCamp"] = std::atoi(map.substr(rewardSep[2] + 1, rewardSep[3] - rewardSep[2] - 1).c_str());
            mapA.back().back()->acquirable["airport"] = std::atoi(map.substr(rewardSep[3] + 1, rewardSep[4] - rewardSep[3] - 1).c_str());
            mapA.back().back()->acquirable["land"] = std::atoi(map.substr(rewardSep[4] + 1, rewardEnd - rewardSep[4] - 1).c_str());

            std::cout << "Block acquirable done" << std::endl;

            int captureStart = map.find("^", rewardEnd);
            int captureEnd = map.find("^", captureStart + 1);

            if (captureStart + 1 != captureEnd)
            {
              std::string sub = map.substr(captureStart + 1, captureEnd - captureStart - 1);
              std::vector<int> coord;
              int init = sub.find(",");
              coord.push_back(std::atoi(sub.substr(0, init).c_str()));
              int temp = sub.find(",", init + 1);
              while (temp != std::string::npos)
              {
                coord.push_back(std::atoi(sub.substr(init + 1, temp - init - 1).c_str()));
                init = temp;
                temp = sub.find(",", init + 1);
              }
              coord.push_back(std::atoi(sub.substr(init + 1).c_str()));
              assert(coord.size() % 2 == 0);

              for (int i = 0; i < coord.size() / 2; i++)
                mapA.back().back()->attackable.push_back(std::make_pair(coord[i * 2], coord[i * 2 + 1]));
            }

            std::cout << "Block capturable done" << std::endl;

            int encircleStart = map.find("*", captureEnd);
            int encircleEnd = map.find("*", encircleStart + 1);
            if (encircleStart + 1 != encircleEnd)
            {
              std::string sub = map.substr(encircleStart + 1, encircleEnd - encircleStart - 1);
              std::vector<int> coord;
              int init = sub.find(",");
              coord.push_back(std::atoi(sub.substr(0, init).c_str()));
              int temp = sub.find(",", init + 1);
              while (temp != std::string::npos)
              {
                coord.push_back(std::atoi(sub.substr(init + 1, temp - init - 1).c_str()));
                init = temp;
                temp = sub.find(",", init + 1);
              }
              coord.push_back(std::atoi(sub.substr(init + 1).c_str()));
              assert(coord.size() % 2 == 0);

              for (int i = 0; i < coord.size() / 2; i++)
                mapA.back().back()->encircled.push_back(std::make_pair(coord[i * 2], coord[i * 2 + 1]));
            }

            std::cout << "Block encirclement done" << std::endl;

            int terrainStart = map.find("(", encircleEnd);
            int terrainEnd = map.find(")", terrainStart);
            mapA.back().back()->terrain = map.substr(terrainStart + 1, terrainEnd - terrainStart - 1);
            std::cout << "Block terrain done" << std::endl;

            int capturedStart = map.find("&", terrainEnd);
            int sep1 = map.find(",", capturedStart);
            int sep2 = map.find(",", sep1 + 1);
            int capturedEnd = map.find("&", sep2);
            bool captured = std::atoi(map.substr(capturedStart + 1, sep1 - capturedStart - 1).c_str());
            bool attackable = std::atoi(map.substr(sep1 + 1, sep2 - sep1 - 1).c_str());
            bool encircled = std::atoi(map.substr(sep2 + 1, capturedEnd - sep2 - 1).c_str());
            if (captured)
            {
              this->enemies->totalEnemies[index]->capturedLand++;
              mapA.back().back()->captured = true;
            }
            mapA.back().back()->isAttackable = attackable;
            mapA.back().back()->isEncircled = encircled;
            std::cout << "Block terrain done" << std::endl;

            assert(map[capturedEnd + 1] == ';');
          }

          std::cout << "Block done" << std::endl;
          std::getline(in, map);
        }

        for (auto i : mapA)
          for (auto j : i)
            if (j != NULL)
              j->map = mapA;

        this->enemies->totalEnemies[index]->map = mapA;
        this->enemies->totalEnemies[index]->totalLand = totalLand;
        std::cout << "Map done" << std::endl;
        for (auto i : this->enemies->totalEnemies)
          std::cout << i->name << " "
                    << "Size: " << i->map.size() << " " << i->map[0].size() << std::endl;
      }
    }
    else if (operand == "time")
    {
      this->timeAcc = std::atoi(input.substr(index + 1, index2 - index - 1).c_str());
      this->day = std::atoi(input.substr(index2 + 1, index3 - index2 - 1).c_str());
      this->timeLimit = std::atoi(input.substr(index3 + 1).c_str());

      std::cout << "Set time done" << std::endl;
    }
  }

  in.close();

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  std::cout << "\033c" << std::endl;
  return this->start();
}

int Game::start()
{
  this->timeChosen = timeRange[0] == this->setting["time"] ? 0 : timeRange[1] == this->setting["time"] ? 1
                                                                                                       : 2;
  this->timer(this->setting["speed"]);

  char input;
  (this->*this->print[this->gamePhase])(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);

  // possible data race for gameover
  while (1)
  {
    // clean_stdin();
    int prevGamePhase = this->gamePhase;

    // same method used in menuPhase

    if (gameOver)
    {
      break;
    }
    input = getch();
    if (gameOver)
    {
      while (input != ' ')
        input = getch();
      break;
    }
    this->lguser.lock();
    this->stopLoopPrintStatus();
    this->stopLoopPrintBuild();
    this->stopLoopPrintResearch();
    this->stopLoopPrintTroop();
    this->lguser.unlock();

    if (input == '\033')
    {
      getch();
      switch (getch())
      {
      case 'A':
        this->gamePhaseSelect[0] = (this->gamePhaseSelect[0] + -1 + this->map[this->gamePhase].size()) % this->map[this->gamePhase].size();
        this->gamePhaseSelect[1] = (this->gamePhaseSelect[1]) % this->map[this->gamePhase][this->gamePhaseSelect[0]].size();
        break;
      case 'B':
        this->gamePhaseSelect[0] = (this->gamePhaseSelect[0] + 1) % this->map[this->gamePhase].size();
        this->gamePhaseSelect[1] = (this->gamePhaseSelect[1]) % this->map[this->gamePhase][this->gamePhaseSelect[0]].size();
        break;
      case 'C':
        this->gamePhaseSelect[0] = (this->gamePhaseSelect[0]) % this->map[this->gamePhase].size();
        this->gamePhaseSelect[1] = (this->gamePhaseSelect[1] + 1) % this->map[this->gamePhase][this->gamePhaseSelect[0]].size();
        break;
      case 'D':
        this->gamePhaseSelect[0] = (this->gamePhaseSelect[0]) % this->map[this->gamePhase].size();
        this->gamePhaseSelect[1] = (this->gamePhaseSelect[1] + -1 + this->map[this->gamePhase][this->gamePhaseSelect[0]].size()) % this->map[this->gamePhase][this->gamePhaseSelect[0]].size();
        break;
      case '1':
        getch();
        getch();
        getch();
      }
    }
    else if (input == 'p')
    {
      this->paused = !this->paused;
    }
    else if (input == 'q')
    {
      this->stopTimer();
      timeChosen = (timeChosen + 1) % this->timeRange.size();
      this->setting["speed"] = this->timeRange[this->timeChosen];
      this->timer(this->setting["speed"]);
    }
    else if (input == '1')
    {
      this->gamePhase = 1;
      this->prevGamePhase.push_back(prevGamePhase);
      this->gamePhaseSelect[0] = 0;
      this->gamePhaseSelect[1] = 0;
    }
    else if (input == '2')
    {
      this->gamePhase = 2;
      this->prevGamePhase.push_back(prevGamePhase);
      this->gamePhaseSelect[0] = 0;
      this->gamePhaseSelect[1] = 0;
    }
    else if (input == '3')
    {
      this->gamePhase = 3;
      this->prevGamePhase.push_back(prevGamePhase);
      this->gamePhaseSelect[0] = 0;
      this->gamePhaseSelect[1] = 0;
    }
    else if (input == '4')
    {
      this->gamePhase = -162;
    }
    else if (input == '5')
    {
      this->gamePhase = -163;
    }
    else if (input == 's')
    {
      this->gamePhase = -2;
    }
    // progress game phase
    else if (input == '\n')
    {
      this->gamePhase = this->map[this->gamePhase][this->gamePhaseSelect[0]][this->gamePhaseSelect[1]];
      if (gamePhase > 0)
      {
        this->prevGamePhase.push_back(prevGamePhase);
        this->gamePhaseSelect[0] = 0;
        this->gamePhaseSelect[1] = 0;
      }
      else if (gamePhase == 0)
      {
        this->gamePhaseSelect[0] = 0;
        this->gamePhaseSelect[1] = 0;
      }
    }
    // default shortcut key for back
    else if (input == ' ')
    {
      if (this->prevGamePhase.size() > 0)
      {
        this->gamePhase = this->prevGamePhase.back();
        this->prevGamePhase.pop_back();
        this->gamePhaseSelect[0] = 0;
        this->gamePhaseSelect[1] = 0;
      }
    }
    this->lguser.lock();
    if (this->gamePhase >= 0)
    {
      (this->*this->print[this->gamePhase])(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    }
    else
    {
      this->lguser.unlock();
      (this->*this->action[-this->gamePhase])(this->gamePhase, prevGamePhase);
      if (this->gamePhase == -999)
      {
        // restart game
        return -999;
      }
      else if (this->gamePhase == -998)
      {
        // quit game
        return 0;
      }
    }
    // clean_stdin();
  }

  return 0;
}