#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <future>
#include <mutex>
#include <fstream>
#include <cassert>
#include <cstdlib>

#include "gameStruct.h"
#include "game.h"
#include "../../lib/uuid/uuid.hpp"
#include "../../io/io.h"
#include "../../data/troop/troop.h"

// decrement remain by 1 every interval until 0
Progress::Progress(int time, int interval_)
    : remain(time), interval(interval_)
{
}
void Progress::start(std::mutex &lg3)
{
  while (this->remain > 0)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(interval));

    lg3.lock();
    this->remain--;
    lg3.unlock();
  }
}

Game::~Game() {}
Game::Game(std::unordered_map<std::string, int> setting_, const int screenX_, const int screenY_, const int fps_, const std::string path_) : setting(setting_), screenWidth(screenX_), screenHeight(screenY_), fps(fps_), path(path_)
{
  resource = new data::Resource();
  building = new data::Building();
  troop = new data::Troop();
  army = new data::Army();
  research = new data::Research();
  battle = new data::Battle();
  enemies = new data::Enemies();
}

// fetch level data from .dat files
void Game::fetch()
{
  std::cout << "\033[2J\033[1;1H" << std::endl;
  std::fstream in(this->path, std::ios::in);

  std::vector<std::string> indexToTroop = {"infantry", "calvary", "artillery", "logistic", "armoredCar", "tank1", "tank2", "tankOshimai", "cas", "fighter", "bomber"};
  std::unordered_map<std::string, std::function<TroopUnit *()>> troopToInstance = {
      {"infantry", [&]() { return new Infantry(this->uuid()); }},
      {"calvary", [&]() { return new Calvary(this->uuid()); }},
      {"artillery", [&]() { return new Artillery(this->uuid()); }},
      {"logistic", [&]() { return new Logistic(this->uuid()); }},
      {"armoredCar", [&]() { return new ArmoredCar(this->uuid()); }},
      {"tank1", [&]() { return new Tank1(this->uuid()); }},
      {"tank2", [&]() { return new Tank2(this->uuid()); }},
      {"tankOshimai", [&]() { return new TankOshimai(this->uuid()); }},
      {"cas", [&]() { return new Cas(this->uuid()); }},
      {"fighter", [&]() { return new Fighter(this->uuid()); }},
      {"bomber", [&]() { return new Bomber(this->uuid()); }}};

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
    if (operand == "building")
    {
      std::cout << "Making buildings done" << std::endl;
      std::string type = input.substr(index + 1, index2 - index - 1);
      int level = std::atoi(input.substr(index2 + 1, index3 - index2 - 1).c_str());
      int num = std::atoi(input.substr(index3 + 1).c_str());
      this->buildBase(type, 0, this->building->effect[type][level], "", 0, num);
    }
    else if (operand == "enemy")
    {
      std::cout << "Creating countries done" << std::endl;
      std::string modifier = input.substr(index + 1, index2 - index - 1);
      if (modifier == "create")
        this->enemies->totalEnemies.push_back(new Enemy(input.substr(index3 + 1)));
    }
    else if (operand == "map")
    {
      std::cout << "Opening map done" << std::endl;
      std::string country = input.substr(index + 1, index2 - index - 1);

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
            mapA.back().push_back(new Block(this->troop, this->resource, this->battle));
            mapA.back().back()->country = country;

            std::vector<int> sep = {(int)map.find(",")};
            for (int i = 0; i < 10; i++)
              sep.push_back((int)map.find(",", sep.back() + 1));
            for (int i = 0; i < 11; i++)
              assert(sep[i] != std::string::npos);

            std::string name = map.substr(i3 + 1, sep[0] - i3 - 1);
            mapA.back().back()->name = name;

            std::cout << "Block name done" << std::endl;

            for (int i = 0; i < 11; i++)
            {
              int num = std::atoi(map.substr(sep[i] + 1, sep[i + 1] - sep[i] - 1).c_str());
              for (int j = 0; j < num; j++)
                mapA.back().back()->totalFoeTroop.push_back(troopToInstance[indexToTroop[i]]());
            }

            std::cout << "Block troop done" << std::endl;

            int armySep = map.find("$");
            while (armySep != std::string::npos)
            {
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
                mapA.back().back()->totalFoeArmy.back()->addTroopM(0, 0, troopToInstance[indexToTroop[std::atoi(map.substr(brackettemp + 1, sepArmy[0] - brackettemp - 1).c_str())]]());
              for (int i = 0; i < 14; i++)
              {
                brackettemp = map.find(")", sepArmy[i]);
                if (brackettemp + 1 != sepArmy[i + 1])
                  mapA.back().back()->totalFoeArmy.back()->addTroopM(i / 4, i % 4, troopToInstance[indexToTroop[std::atoi(map.substr(brackettemp + 1, sepArmy[i + 1] - brackettemp - 1).c_str())]]());
              }
              brackettemp = map.find(")", sepArmy[14]);
              if (brackettemp + 1 != armyEnd)
                mapA.back().back()->totalFoeArmy.back()->addTroopM(3, 3, troopToInstance[indexToTroop[std::atoi(map.substr(brackettemp + 1, armyEnd - brackettemp - 1).c_str())]]());

              armySep = map.find("$", armyEnd + 1);
            }

            std::cout << "Block army done" << std::endl;

            int rewardStart = map.find("|", sep.back());
            int rewardEnd = map.find("|", rewardStart + 1);

            std::vector<int> rewardSep = {(int)map.find(",", rewardStart)};
            for (int i = 0; i < 4; i++)
              rewardSep.push_back(map.find(",", rewardSep.back()));
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
            int captureEnd = map.find("^", captureStart+1);
            
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

            assert(map[terrainEnd + 1] == ';');
          }

          std::getline(in, map);
        }

        this->enemies->totalEnemies[index]->map = mapA;
        this->enemies->totalEnemies[index]->totalLand = totalLand;

        std::cout << "Block done" << std::endl;
      }
    }
    else if(operand == "time"){
      this->timeLimit = std::atoi(input.substr(index3+1).c_str());

      std::cout << "Set time done" << std::endl;
    }
    else if(operand == "baseLand"){
      this->resource->baseLand = std::atoi(input.substr(index3+1).c_str());

      std::cout << "Set land done" << std::endl;
    }
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  std::cout<<"\033c"<<std::endl;
  this->start();
}

void Game::start()
{
  this->timerThread = new std::thread(&Game::timer, this, this->setting["speed"]);

  char input;
  (this->*this->print[this->gamePhase])(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
  
  // possible data race for gameover
  while (!gameOver)
  {
    int prevGamePhase = this->gamePhase;

    // same method used in menuPhase
    input = getch();

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
      }
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

    this->stopLoopPrintStatus();
    this->stopLoopPrintBuild();
    this->stopLoopPrintResearch();
    this->stopLoopPrintTroop();

    if (this->gamePhase >= 0)
    {
      (this->*this->print[this->gamePhase])(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
    }
    else
    {
      this->lguser.lock();
      (this->*this->action[-this->gamePhase])(this->gamePhase, prevGamePhase);
      this->lguser.unlock();
    }
    
  }

  // timer thread terminates itself and the main while loop
  std::cout<<"\033c"<<std::endl;
  std::cout << "Game ends!" << std::endl;
}