#ifndef GAME_H
#define GAME_H
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <mutex>
#include <thread>
#include <chrono>
#include <future>
#include <condition_variable>
#include <functional>
#include <cmath>

#include "gameStruct.h"
#include "../../lib/uuid/uuid.hpp"

class Game
{
public:
  ~Game();
  Game(const std::unordered_map<std::string, int> &);
  void start();

private:
  // similar to how menu phase works, we progress the phase number according to use actions
  std::vector<std::vector<std::vector<int>>> map{

      // building, researching, troops training, army editing, battle planning, battling
      {{1, 2, 3, -162, -163, -164},

       // set speed, pause (action), save as (action), restart (action), quit (action)
       {7, -1, -2, -3, -4}},

      // build (1-4), upgrade (5-8)
      // delete (1-2), upgraed (3 - 6)
      {
          {-5, -6, -7, -8, -9, -10, -11, -12},
          {-14, -15, -16, -17, -18, -19, -20, -21},
          {-23, -24, -25, -26, -27, -28, -29, -30},
          {-32, -33, -34, -35, -36, -37, -38, -39},
          {-41, -42, -43, -44, -45, -46, -47, -48},
          {-49, -50, -51, -52},
          {-53, -54, -55, -56, -57, -58},
          {-59, -60, -61, -62, -63, -64},
          {-65, -66, -67, -68, -69, -70},
          {-71, -72},
          {-73, -74},
          {0}},
      {// researches
       {-75},
       {-76},
       {-77},
       {-78},
       {-79},
       {-80},
       {-81},
       {-82},
       {-83},
       {0}},
      {// train troops (1, 5, max), remove troops (1, all)
       {-84, -85, -86, -87, -88, -89},
       {-90, -91, -92, -93, -94, -95},
       {-96, -97, -98, -99, -100, -101},
       {-102, -103, -104, -105, -106, -107},
       {-108, -109, -110, -111, -112, -113},
       {-114, -115, -116, -117, -118, -119},
       {-120, -121, -122, -123, -124, -125},
       {-126, -127, -128, -129, -130, -131},
       {-132, -133, -134, -135, -136, -137},
       {-138, -139, -140, -141, -142, -143},
       {-144, -145, -146, -147, -148, -149},
       {-150, -151, -152, -153, -154, -155},
       {-156, -157, -158, -159, -160, -161},
       {0}}};
  int gamePhase = 0;
  std::vector<int> prevGamePhase = {};
  int gamePhaseSelect[2] = {0, 0};

  // function to be executed according to gamePhase (printing)
  // input: x and y value
  std::vector<void (Game::*)(int, int)> print = {
      &Game::loopPrintStatus, &Game::loopPrintBuild, &Game::loopPrintResearch, &Game::loopPrintTroop};
  // , &Game::printResearch, &Game::printTroopTrain, &Game::printArmyEdit, &Game::printBattlePlan, &Game::printBattle};

  // function to be executed according to gamePhase (modifying)
  // length: 162 (index 0: NULL)
  std::vector<void (Game::*)(int &, int)> action = {
      NULL, NULL, NULL, NULL, NULL,
      &Game::buildfarm1, &Game::buildfarm5, &Game::buildfarm10, &Game::buildfarmmax, &Game::upgradefarm1, &Game::upgradefarm5, &Game::upgradefarm10, &Game::upgradefarmmax,
      NULL,
      &Game::buildcivilianFactory1, &Game::buildcivilianFactory5, &Game::buildcivilianFactory10, &Game::buildcivilianFactorymax, &Game::upgradefarm21, &Game::upgradefarm25, &Game::upgradefarm210, &Game::upgradefarm2max,
      NULL,
      &Game::buildmilitaryFactory1, &Game::buildmilitaryFactory5, &Game::buildmilitaryFactory10, &Game::buildmilitaryFactorymax, &Game::upgradecivilianFactory1, &Game::upgradecivilianFactory5, &Game::upgradecivilianFactory10, &Game::upgradecivilianFactorymax,
      NULL,
      &Game::buildtrainingCamp1, &Game::buildtrainingCamp5, &Game::buildtrainingCamp10, &Game::buildtrainingCampmax, &Game::upgradecivilianFactory21, &Game::upgradecivilianFactory25, &Game::upgradecivilianFactory210, &Game::upgradecivilianFactory2max,
      NULL,
      &Game::buildairport1, &Game::buildairport5, &Game::buildairport10, &Game::buildairportmax, &Game::upgrademilitaryFactory1, &Game::upgrademilitaryFactory5, &Game::upgrademilitaryFactory10, &Game::upgrademilitaryFactorymax,
      &Game::upgrademilitaryFactory21, &Game::upgrademilitaryFactory25, &Game::upgrademilitaryFactory210, &Game::upgrademilitaryFactory2max,
      &Game::removefarm1, &Game::removefarmmax, &Game::removefarmLV21, &Game::removefarmLV2max, &Game::removefarmLV31, &Game::removefarmLV3max,
      &Game::removecivilianFactory1, &Game::removecivilianFactorymax, &Game::removecivilianFactoryLV21, &Game::removecivilianFactoryLV2max, &Game::removecivilianFactoryLV31, &Game::removecivilianFactoryLV3max,
      &Game::removemilitaryFactory1, &Game::removemilitaryFactorymax, &Game::removemilitaryFactoryLV21, &Game::removemilitaryFactoryLV2max, &Game::removemilitaryFactoryLV31, &Game::removemilitaryFactoryLV3max,
      &Game::removetrainingCamp1, &Game::removetrainingCampmax,
      &Game::removeairport1, &Game::removeairportmax,
      &Game::researchFarm,
      &Game::researchDivisionOfLabor,
      &Game::researchProductionLine,
      &Game::researchLandDoctrine,
      &Game::researchAirDoctrine,
      &Game::researchUrbanization,
      &Game::researchWeapon,
      &Game::researchTraining,
      &Game::researchRecovery,
      &Game::trainInfantry, &Game::trainInfantry5, &Game::trainInfantry10, &Game::trainInfantrymax, &Game::removeInfantry, &Game::removeInfantrymax,
      &Game::trainCalvary, &Game::trainCalvary5, &Game::trainCalvary10, &Game::trainCalvarymax, &Game::removeCalvary, &Game::removeCalvarymax,
      &Game::trainSuicideBomber, &Game::trainSuicideBomber5, &Game::trainSuicideBomber10, &Game::trainSuicideBombermax, &Game::removeSuicideBomber, &Game::removeSuicideBombermax,
      &Game::trainArtillery, &Game::trainArtillery5, &Game::trainArtillery10, &Game::trainArtillerymax, &Game::removeArtillery, &Game::removeArtillerymax,
      &Game::trainLogistic, &Game::trainLogistic5, &Game::trainLogistic10, &Game::trainLogisticmax, &Game::removeLogistic, &Game::removeLogisticmax,
      &Game::trainArmoredCar, &Game::trainArmoredCar5, &Game::trainArmoredCar10, &Game::trainArmoredCarmax, &Game::removeArmoredCar, &Game::removeArmoredCarmax,
      &Game::trainTank1, &Game::trainTank15, &Game::trainTank110, &Game::trainTank1max, &Game::removeTank1, &Game::removeTank1max,
      &Game::trainTank2, &Game::trainTank25, &Game::trainTank210, &Game::trainTank2max, &Game::removeTank2, &Game::removeTank2max,
      &Game::trainTankOshimai, &Game::trainTankOshimai5, &Game::trainTankOshimai10, &Game::trainTankOshimaimax, &Game::removeTankOshimai, &Game::removeTankOshimaimax,
      &Game::trainCas, &Game::trainCas5, &Game::trainCas10, &Game::trainCasmax, &Game::removeCas, &Game::removeCasmax,
      &Game::trainFighter, &Game::trainFighter5, &Game::trainFighter10, &Game::trainFightermax, &Game::removeFighter, &Game::removeFightermax,
      &Game::trainBomber, &Game::trainBomber5, &Game::trainBomber10, &Game::trainBombermax, &Game::removeBomber, &Game::removeBombermax,
      &Game::trainKamikaze, &Game::trainKamikaze5, &Game::trainKamikaze10, &Game::trainKamikazemax, &Game::removeKamikaze, &Game::removeKamikazemax};

  // separate timer thread to increment time only
  std::thread *timerThread;
  std::future<void> loopPrintStatusThread;
  std::future<void> loopPrintBuildThread;
  std::future<void> loopPrintResearchThread;
  std::future<void> loopPrintTroopThread;

  // default to printing the base status every second
  // approach: call async to allocate non-blocking thread to tempLoopPrintStatus
  //           -> tempLoopPrintStatus repeatedly waits 1 second for a conditional variable to notify instead of doing thread sleep (cannot wake thread when slept)
  //           -> async thread is terminated per user action (to update the x and y index value)
  //           -> terminated by first setting terminatePrint to false (stops while loop), then notifying the conditional variable (breaks wait_for)
  void loopPrintStatus(int x, int y)
  {

    // tempLoopPrintStatus is required so that I do not need to pass in a lambda capturing *this*
    // the class function called within the lambda (printStatus) does not have a reference to *this* even if it is captured inside lambda
    this->loopPrintStatusThread = std::async(std::launch::async, &Game::tempLoopPrintStatus, this, x, y);
  };
  void stopLoopPrintStatus()
  {
    if (this->loopPrintStatusThread.valid())
    {
      this->lgcv1a.lock();
      this->terminatePrint = true;
      this->lgcv1a.unlock();
      terminatePrintCV.notify_all();
      this->loopPrintStatusThread.get();
    }
  }
  void tempLoopPrintStatus(int x, int y)
  {
    this->terminatePrint = false;
    while (!terminatePrint)
    {
      this->printStatus(x, y);
      std::unique_lock<std::mutex> lock(this->lgcv1b);
      terminatePrintCV.wait_for(lock, std::chrono::milliseconds(1000/10));
    }
  }
  void loopPrintBuild(int x, int y)
  {
    this->loopPrintBuildThread = std::async(std::launch::async, &Game::tempLoopPrintBuild, this, x, y);
  };
  void stopLoopPrintBuild()
  {
    if (this->loopPrintBuildThread.valid())
    {
      this->lgcv2a.lock();
      this->terminateBuild = true;
      this->lgcv2a.unlock();
      terminateBuildCV.notify_all();
      this->loopPrintBuildThread.get();
    }
  }
  void tempLoopPrintBuild(int x, int y)
  {
    this->terminateBuild = false;
    while (!terminateBuild)
    {
      this->printBuild(x, y);
      std::unique_lock<std::mutex> lock(this->lgcv2b);
      terminateBuildCV.wait_for(lock, std::chrono::milliseconds(1000/10));
    }
  }
  void loopPrintResearch(int x, int y)
  {
    this->loopPrintResearchThread = std::async(std::launch::async, &Game::tempLoopPrintResearch, this, x, y);
  };
  void stopLoopPrintResearch()
  {
    if (this->loopPrintResearchThread.valid())
    {
      this->lgcv3a.lock();
      this->terminateResearch = true;
      this->lgcv3a.unlock();
      terminateResearchCV.notify_all();
      this->loopPrintResearchThread.get();
    }
  }
  void tempLoopPrintResearch(int x, int y)
  {
    this->terminateResearch = false;
    while (!terminateResearch)
    {
      this->printResearch(x, y);
      std::unique_lock<std::mutex> lock(this->lgcv3b);
      terminateResearchCV.wait_for(lock, std::chrono::milliseconds(1000/10));
    }
  }
  void loopPrintTroop(int x, int y)
  {
    this->loopPrintTroopThread = std::async(std::launch::async, &Game::tempLoopPrintTroop, this, x, y);
  };
  void stopLoopPrintTroop()
  {
    if (this->loopPrintTroopThread.valid())
    {
      this->lgcv4a.lock();
      this->terminateTroop = true;
      this->lgcv4a.unlock();
      terminateTroopCV.notify_all();
      this->loopPrintTroopThread.get();
    }
  }
  void tempLoopPrintTroop(int x, int y)
  {
    this->terminateTroop = false;
    while (!terminateTroop)
    {
      this->printTroop(x, y);
      std::unique_lock<std::mutex> lock(this->lgcv4b);
      terminateTroopCV.wait_for(lock, std::chrono::milliseconds(1000/10));
    }
  }

  void printStatus(int x, int y);
  void printBuild(int x, int y);
  void printResearch(int x, int y);
  void printTroop(int x, int y);
  void gameArmy(int &currentPhase, int prevPhase);
  void printBattlePlan(int x, int y);
  void printBattle(int x, int y);
  void printSetSpeed(int x, int y);

  void buildBase(std::string, int, std::function<void(data::Resource &)> &, std::string, double, int);
  void buildBase(std::string, int, std::function<void()>, std::string, double, int);
  // each of these parent build function:
  // check if enough resources to build, calls buildbase, and start the printing build loop
  void buildfarm1(int &currentPhase, int prevPhase);
  void buildfarm5(int &currentPhase, int prevPhase);
  void buildfarm10(int &currentPhase, int prevPhase);
  void buildfarmmax(int &currentPhase, int prevPhase);
  void buildcivilianFactory1(int &currentPhase, int prevPhase);
  void buildcivilianFactory5(int &currentPhase, int prevPhase);
  void buildcivilianFactory10(int &currentPhase, int prevPhase);
  void buildcivilianFactorymax(int &currentPhase, int prevPhase);
  void buildmilitaryFactory1(int &currentPhase, int prevPhase);
  void buildmilitaryFactory5(int &currentPhase, int prevPhase);
  void buildmilitaryFactory10(int &currentPhase, int prevPhase);
  void buildmilitaryFactorymax(int &currentPhase, int prevPhase);
  void buildtrainingCamp1(int &currentPhase, int prevPhase);
  void buildtrainingCamp5(int &currentPhase, int prevPhase);
  void buildtrainingCamp10(int &currentPhase, int prevPhase);
  void buildtrainingCampmax(int &currentPhase, int prevPhase);
  void buildairport1(int &currentPhase, int prevPhase);
  void buildairport5(int &currentPhase, int prevPhase);
  void buildairport10(int &currentPhase, int prevPhase);
  void buildairportmax(int &currentPhase, int prevPhase);

  // each of these parent build function:
  // removes selected type of building with specified amount and add back specified amount of land
  void removefarm1(int &currentPhase, int prevPhase);
  void removefarmmax(int &currentPhase, int prevPhase);
  void removefarmLV21(int &currentPhase, int prevPhase);
  void removefarmLV2max(int &currentPhase, int prevPhase);
  void removefarmLV31(int &currentPhase, int prevPhase);
  void removefarmLV3max(int &currentPhase, int prevPhase);
  void removecivilianFactory1(int &currentPhase, int prevPhase);
  void removecivilianFactorymax(int &currentPhase, int prevPhase);
  void removecivilianFactoryLV21(int &currentPhase, int prevPhase);
  void removecivilianFactoryLV2max(int &currentPhase, int prevPhase);
  void removecivilianFactoryLV31(int &currentPhase, int prevPhase);
  void removecivilianFactoryLV3max(int &currentPhase, int prevPhase);
  void removemilitaryFactory1(int &currentPhase, int prevPhase);
  void removemilitaryFactorymax(int &currentPhase, int prevPhase);
  void removemilitaryFactoryLV21(int &currentPhase, int prevPhase);
  void removemilitaryFactoryLV2max(int &currentPhase, int prevPhase);
  void removemilitaryFactoryLV31(int &currentPhase, int prevPhase);
  void removemilitaryFactoryLV3max(int &currentPhase, int prevPhase);
  void removetrainingCamp1(int &currentPhase, int prevPhase);
  void removetrainingCampmax(int &currentPhase, int prevPhase);
  void removeairport1(int &currentPhase, int prevPhase);
  void removeairportmax(int &currentPhase, int prevPhase);

  void researchBase(std::string, std::function<void(data::Resource &, data::Building &, data::Troop &, data::Army &, data::BattlePlan &, data::Battle &)> &, int);
  // researches the particular area
  void researchFarm(int &currentPhase, int prevPhase);
  void researchDivisionOfLabor(int &currentPhase, int prevPhase);
  void researchProductionLine(int &currentPhase, int prevPhase);
  void researchLandDoctrine(int &currentPhase, int prevPhase);
  void researchAirDoctrine(int &currentPhase, int prevPhase);
  void researchUrbanization(int &currentPhase, int prevPhase);
  void researchWeapon(int &currentPhase, int prevPhase);
  void researchTraining(int &currentPhase, int prevPhase);
  void researchRecovery(int &currentPhase, int prevPhase);

  // these upgrade functions first call build function, then remove
  // cannot swap the order because removing it first may lead to negative numbers in manpower etc.
  void upgradefarm1(int &currentPhase, int prevPhase);
  void upgradefarm5(int &currentPhase, int prevPhase);
  void upgradefarm10(int &currentPhase, int prevPhase);
  void upgradefarmmax(int &currentPhase, int prevPhase);
  void upgradefarm21(int &currentPhase, int prevPhase);
  void upgradefarm25(int &currentPhase, int prevPhase);
  void upgradefarm210(int &currentPhase, int prevPhase);
  void upgradefarm2max(int &currentPhase, int prevPhase);
  void upgradecivilianFactory1(int &currentPhase, int prevPhase);
  void upgradecivilianFactory5(int &currentPhase, int prevPhase);
  void upgradecivilianFactory10(int &currentPhase, int prevPhase);
  void upgradecivilianFactorymax(int &currentPhase, int prevPhase);
  void upgradecivilianFactory21(int &currentPhase, int prevPhase);
  void upgradecivilianFactory25(int &currentPhase, int prevPhase);
  void upgradecivilianFactory210(int &currentPhase, int prevPhase);
  void upgradecivilianFactory2max(int &currentPhase, int prevPhase);
  void upgrademilitaryFactory1(int &currentPhase, int prevPhase);
  void upgrademilitaryFactory5(int &currentPhase, int prevPhase);
  void upgrademilitaryFactory10(int &currentPhase, int prevPhase);
  void upgrademilitaryFactorymax(int &currentPhase, int prevPhase);
  void upgrademilitaryFactory21(int &currentPhase, int prevPhase);
  void upgrademilitaryFactory25(int &currentPhase, int prevPhase);
  void upgrademilitaryFactory210(int &currentPhase, int prevPhase);
  void upgrademilitaryFactory2max(int &currentPhase, int prevPhase);

  // train troops
  void trainBase(std::string, std::function<void(data::Resource &, data::Troop &)>, int, int, int, int);
  void trainInfantry(int &, int);
  void trainCalvary(int &, int);
  void trainSuicideBomber(int &, int);
  void trainLogistic(int &, int);
  void trainArtillery(int &, int);
  void trainArmoredCar(int &, int);
  void trainTank1(int &, int);
  void trainTank2(int &, int);
  void trainTankOshimai(int &, int);
  void trainCas(int &, int);
  void trainFighter(int &, int);
  void trainBomber(int &, int);
  void trainKamikaze(int &, int);
  void trainInfantry5(int &, int);
  void trainCalvary5(int &, int);
  void trainSuicideBomber5(int &, int);
  void trainLogistic5(int &, int);
  void trainArtillery5(int &, int);
  void trainArmoredCar5(int &, int);
  void trainTank15(int &, int);
  void trainTank25(int &, int);
  void trainTankOshimai5(int &, int);
  void trainCas5(int &, int);
  void trainFighter5(int &, int);
  void trainBomber5(int &, int);
  void trainKamikaze5(int &, int);
  void trainInfantry10(int &, int);
  void trainCalvary10(int &, int);
  void trainSuicideBomber10(int &, int);
  void trainLogistic10(int &, int);
  void trainArtillery10(int &, int);
  void trainArmoredCar10(int &, int);
  void trainTank110(int &, int);
  void trainTank210(int &, int);
  void trainTankOshimai10(int &, int);
  void trainCas10(int &, int);
  void trainFighter10(int &, int);
  void trainBomber10(int &, int);
  void trainKamikaze10(int &, int);
  void trainInfantrymax(int &, int);
  void trainCalvarymax(int &, int);
  void trainSuicideBombermax(int &, int);
  void trainLogisticmax(int &, int);
  void trainArtillerymax(int &, int);
  void trainArmoredCarmax(int &, int);
  void trainTank1max(int &, int);
  void trainTank2max(int &, int);
  void trainTankOshimaimax(int &, int);
  void trainCasmax(int &, int);
  void trainFightermax(int &, int);
  void trainBombermax(int &, int);
  void trainKamikazemax(int &, int);
  void removeInfantry(int &, int);
  void removeCalvary(int &, int);
  void removeSuicideBomber(int &, int);
  void removeLogistic(int &, int);
  void removeArtillery(int &, int);
  void removeArmoredCar(int &, int);
  void removeTank1(int &, int);
  void removeTank2(int &, int);
  void removeTankOshimai(int &, int);
  void removeCas(int &, int);
  void removeFighter(int &, int);
  void removeBomber(int &, int);
  void removeKamikaze(int &, int);
  void removeInfantrymax(int &, int);
  void removeCalvarymax(int &, int);
  void removeSuicideBombermax(int &, int);
  void removeLogisticmax(int &, int);
  void removeArtillerymax(int &, int);
  void removeArmoredCarmax(int &, int);
  void removeTank1max(int &, int);
  void removeTank2max(int &, int);
  void removeTankOshimaimax(int &, int);
  void removeCasmax(int &, int);
  void removeFightermax(int &, int);
  void removeBombermax(int &, int);
  void removeKamikazemax(int &, int);

  void pause();
  void saveAs();
  void restart();
  void quit();

  // Note timer only progress day variable, nothing else
  // training troop/ building have their own async loops
  // reason is because the user can start a progress at anytime so it owuld be impossible to use a single loop for all progresses

  // NOTE: KNOWN VISUAL BUG, sometimes when starting multiple progresses at the same time, some would show n days while some show n+1 days
  // reason: data racing
  void timer(int time)
  {
    while (!this->terminate)
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(time));

      this->lg.lock();
      this->day++;
      this->lg.unlock();
    }
  }
  bool terminate = false;
  bool terminatePrint = false;
  bool terminateBuild = false;
  bool terminateResearch = false;
  bool terminateTroop = false;
  std::condition_variable terminatePrintCV;
  std::condition_variable terminateBuildCV;
  std::condition_variable terminateResearchCV;
  std::condition_variable terminateTroopCV;

  int day = 1;
  std::vector<Progress *> progress;

  // key: speed
  std::unordered_map<std::string, int> setting;

  data::Resource *resource;
  data::Building *building;
  data::Troop *troop;
  data::Army *army;
  data::BattlePlan *battlePlan;
  data::Research *research;
  data::Battle *battle;
  data::Enemies *enemies;

  // for day
  std::mutex lg;
  // for print cv
  std::mutex lgcv1a;
  std::mutex lgcv1b;
  // for build cv
  std::mutex lgcv2a;
  std::mutex lgcv2b;
  // for research cv
  std::mutex lgcv3a;
  std::mutex lgcv3b;
  // for troop cv
  std::mutex lgcv4a;
  std::mutex lgcv4b;
  // for loop thread
  std::mutex lg2;
  // for any mutation of data
  std::mutex lg3;
  // user action lock (prevent spamming)
  std::mutex lguser;

  std::function<std::string()> uuid = [&]() -> std::string { sole::uuid A = sole::uuid1(); return A.str(); };

  // format researches when printing, return string
  std::string helper(std::vector<bool> &level)
  {
    return (level[2] ? "advanced" : level[1] ? "intermediate"
                                             : "rudimentary");
  }

  // format battling regions when printing, return string
  std::string helper2(std::vector<std::string> &region)
  {
    std::string out;
    for (auto i : region)
      out += i + " ";
    return out.substr(0, out.size() - 1);
  }
};
#endif