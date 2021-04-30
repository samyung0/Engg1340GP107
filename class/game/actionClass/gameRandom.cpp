#include <iostream>
#include <vector>
#include <unordered_map>
#include <random>
#include <utility>
#include <array>
#include <functional>
#include <new>
#include <ctime>
#include <iterator>
#include <algorithm>
#include <chrono>
#include <thread>
#include <map>
#include <cassert>
#include <new>

#include "../game.h"
#include "../../../data/troop/troop.h"
#include "../gameStruct.h"

int Game::fetchRandom(int difficulty, int completeness)
{
  std::cout << "\033[2J\033[1;1H" << std::endl;
  std::cout << "ランダムデータを生成されている" << std::endl;

  srand(time(0));
  this->timeAcc = 0;
  this->day = 1;
  this->timeLimit = 99999;
  std::cout << "時間設定された" << std::endl;

  this->resource->baseLand = 3000;
  this->resource->usedLand = 1;
  this->resource->manpower = 1;
    this->building->civilianFactory[0] = 1;

  std::cout << "初期の建築物数設定された" << std::endl;

  std::vector<std::string> terrain = {
      "plain", "hill", "river", "mountain", "snowland", "desert"};
  
  std::vector<std::string> indexToType = {
      "infantry", "calvary", "artillery", "armoredCar", "tank1", "tank2", "tankOshimai", "cas", "fighter", "bomber"};

  std::vector<std::function<TroopUnit *()>> troopToInstance = {
      [&]() { return new Infantry(this->uuid()); },
      [&]() { return new Calvary(this->uuid()); },
      [&]() { return new Artillery(this->uuid()); },
      [&]() { return new ArmoredCar(this->uuid()); },
      [&]() { return new Tank1(this->uuid()); },
      [&]() { return new Tank2(this->uuid()); },
      [&]() { return new TankOshimai(this->uuid()); },
      [&]() { return new Cas(this->uuid()); },
      [&]() { return new Fighter(this->uuid()); },
      [&]() { return new Bomber(this->uuid()); }};

  std::vector<int> noHardLandTroop = {0, 1};

  std::vector<int> hardLandTroop = {2, 3, 4, 5, 6};

  std::vector<int> armorLandTroop = {3, 4, 5, 6};

  std::vector<int> airTroop = {7, 8, 9};

  std::vector<std::array<int, 10>> weight = {
      {1, 1, 1, 4, 5, 3, 0, 5, 5, 1},
      {3, 3, 1, 4, 5, 3, 0, 5, 5, 1},
      {1, 5, 3, 4, 5, 3, 0, 5, 5, 1},
      {0, 0, 1, 4, 5, 3, 0, 5, 5, 1},
      {0, 0, 0, 0, 5, 3, 0, 4, 5, 2},
      {5, 1, 1, 0, 5, 3, 0, 4, 5, 1},
      {1, 0, 0, 0, 3, 5, 2, 2, 4, 1},
      {0, 1, 1, 0, 3, 5, 2, 2, 4, 1},
      {0, 0, 0, 0, 3, 5, 2, 2, 4, 1},
      {0, 0, 0, 0, 0, 5, 2, 2, 4, 1},
      {0, 0, 0, 0, 0, 0, 1, 2, 4, 1},
  };

  double minimumLand = 0.5;

  std::vector<std::string> name = {
      "ShirakamiFubuki", "AkaiHaato", "MinatoAqua", "NekomataOkayu", "UsadaPekora", "shroud"};

  std::vector<std::vector<double>> weightD = {
      {0.2375, 0.2375, 0.2375, 0.05, 0, 0, 0, 0, 0, 0, 0},
      {0.1625, 0.1625, 0.1625, 0.05, 0.3, 0, 0, 0, 0, 0, 0},
      {0.14286, 0.14286, 0.14286, 0.14286, 0.14286, 0.14286, 0.14286, 0, 0, 0, 0},
      {0.090909, 0.090909, 0.090909, 0.090909, 0.090909, 0.090909, 0.090909, 0.090909, 0.090909, 0.090909, 0},
      {0, 0, 0, 0, 0.141428, 0.141428, 0.141428, 0.141428, 0.141428, 0.141428, 0.01},
  };
  // soft attack and hard attack do not include that from planes

  std::vector<std::array<int, 3>> damageMidLineD = {
      {120, 80, 25},
      {200, 140, 40},
      {400, 300, 70},
      {700, 900, 200},
      {1200, 1800, 250},
  };
  // 0: decrease value, 1: increase value
  // binomial

  std::vector<std::vector<std::pair<double, double>>> damageDeviationD = {
      {
          {0.4, 0.4},
          {0.3, 0.3},
          {0.2, 0.2},
          {0.1, 0.1},
          {0, 0},
      },
      {
          {0.4, 0.4},
          {0.3, 0.3},
          {0.2, 0.2},
          {0.1, 0.1},
          {0, 0},
      },
      {
          {0.4, 0.5},
          {0.3, 0.4},
          {0.2, 0.3},
          {0.1, 0.15},
          {0, 0},
      },
      {
          {0.4, 0.6},
          {0.3, 0.4},
          {0.2, 0.3},
          {0.1, 0.15},
          {0, 0},
      },
      {
          {0.2, 0.7},
          {0.2, 0.4},
          {0.1, 0.3},
          {0, 0.15},
          {0, 0},
      },
  };
  // percentage to increase

  std::vector<double> damageDDeviationD = {
      0.5, 0.6, 0.7, 0.7, 0.75};

  std::vector<std::pair<int, int>> armySizeD = {
      {4, 8},
      {6, 10},
      {8, 12},
      {10, 14},
      {14, 16},
  };

  std::vector<std::pair<int, int>> countryD = {
      {1, 2}, {1, 3}, {3, 4}, {3, 5}, {6, 6}};
  // column, row, column, row

  std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> countrySizeD = {
      {{6, 6}, {7, 7}}, {{7, 7}, {9, 9}}, {{7, 7}, {10, 10}}, {{8, 8}, {11, 11}}, {{10, 10}, {15, 15}}};

  // binomial

  std::vector<double> makeArmyD = {
      0.4, 0.5, 0.7, 0.9, 1.};

  std::random_device rd;

  std::mt19937 gen(rd());

  std::uniform_int_distribution<> int1(countryD[difficulty].first, countryD[difficulty].second);

  std::uniform_int_distribution<> int2;

  std::binomial_distribution<> b1;

  std::discrete_distribution<> dint1;

  std::discrete_distribution<> dint2;

  std::discrete_distribution<> map;

  if(completeness == -1) map = std::discrete_distribution<>({1, 0.5});

  else map = std::discrete_distribution<>({1, (double) completeness});

  int n = int1(gen);

  std::cout << n << "つの国が生成される" << std::endl;

  std::vector<double> weightSub = weightD[difficulty];

  for (auto &i : weightSub)
    i *= 1000000;
  std::pair<int, int> armySize = armySizeD[difficulty];


  for (int z = 0; z < n; z++)
  {
    int1 = std::uniform_int_distribution<>(0, name.size() - 1);

    int temp = int1(gen);

    std::string nameChosen = name[temp];

    name.erase(name.begin() + temp);

    this->enemies->totalEnemies.push_back(new Enemy(nameChosen, this->enemies->defeated));
    std::cout << "ようこそ、" << nameChosen << "、この残酷で無慈悲な世界へ" << std::endl;

    int1 = std::uniform_int_distribution<>(countrySizeD[difficulty].first.first, countrySizeD[difficulty].second.first);
    int2 = std::uniform_int_distribution<>(countrySizeD[difficulty].first.second, countrySizeD[difficulty].second.second);
    int dimension[2] = {int1(gen), int2(gen)};
    std::cout << dimension[0] << "ｘ" << dimension[1] << " マップが開かさせている" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    Enemy *ptr = this->enemies->totalEnemies.back();
    int makeTotal = dimension[0] * dimension[1] * minimumLand;
    int makePossible = dimension[0] * dimension[1];
    int made = 0;
    std::vector<std::vector<std::pair<std::pair<int, int>, std::vector<std::pair<int, int>>>>> connect;
    std::vector<int> columnProgression;
    std::map<std::pair<int, int>, bool> bottomRight = {};


    for (int i = 0; i < dimension[1]; i++)
      columnProgression.push_back(65);

    std::vector<std::vector<Block *>> mapA;


    for (int i = 0; i < dimension[0]; i++)
    {
      connect.push_back({});
      mapA.push_back({});
      bool emptyRow = true;


      for (int j = 0; j < dimension[1]; j++)
      {
        bool make;

        if (makeTotal - made == makePossible || (emptyRow && j == dimension[1] - 1) || bottomRight.count(std::make_pair(i - 1, j)) > 0 || (i == dimension[0] - 1 && bottomRight.count(std::make_pair(i, j - 1)) > 0))
          make = true;

        else
          make = map(gen);


        if (make)
        {
          std::cout << "ブロック " << i << "," << j << " は作らせている" << std::endl;
          emptyRow = false;

          mapA.back().push_back(new Block(this->troop, this->resource, this->battle, this->enemies->totalEnemies.back()->capturedLand, this->enemies->totalEnemies.back()->capitulated, this->enemies->totalEnemies.back()->battlingRegions, this->enemies->totalEnemies.back()->totalLand, this->enemies->totalEnemies.back()->defeated, mapA.size() - 1, mapA.back().size()));
          connect.back().push_back(std::make_pair(std::make_pair(i, j), std::vector<std::pair<int, int>>()));

          std::array<int, 3> damageToReach = damageMidLineD[std::min(z, (int)damageMidLineD.size() - 1)];
          std::array<int, 3> damage = {0, 0, 0};

          std::vector<std::pair<double, double>> damageDeviation = damageDeviationD[std::min(z, (int)damageDeviationD.size() - 1)];
          double damageDDeviation = damageDDeviationD[std::min(z, (int)damageMidLineD.size() - 1)];
          bool increase = rand() % 100 < damageDDeviation * 100;
          b1 = std::binomial_distribution<>(damageDeviation.size() - 1, 0.8);

          for (int i = 0; i < 3; i++)

            if (increase)
              damageToReach[i] *= 1 + damageDeviation[b1(gen)].second;

            else
              damageToReach[i] *= 1 - damageDeviation[b1(gen)].first;

          std::cout << "敵総合ダメージ: " << damageToReach[0] << ", " << damageToReach[1] << ", " << damageToReach[2] << std::endl;

          std::array<std::vector<TroopUnit *>, 10> troop = {std::vector<TroopUnit *>(), std::vector<TroopUnit *>(), std::vector<TroopUnit *>(), std::vector<TroopUnit *>(), std::vector<TroopUnit *>(), std::vector<TroopUnit *>(), std::vector<TroopUnit *>(), std::vector<TroopUnit *>(), std::vector<TroopUnit *>(), std::vector<TroopUnit *>()};

          dint1 = std::discrete_distribution<>(weightSub.begin(), weightSub.end());

          int templateIndex = dint1(gen);

          std::cout << "テンプレート: " << templateIndex << std::endl;

          if (templateIndex == 10)
          {
            std::cout << "運が味方してくれるように (。・ω・。)" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
          }

          std::array<int, 10> troopC = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
          int totalTroopC = 0;

          dint1 = std::discrete_distribution<>(std::next(weight[templateIndex].begin(), hardLandTroop[0]), std::next(weight[templateIndex].begin(), hardLandTroop.back() + 1));

          while (damage[1] < damageToReach[1])
          {
            int type = dint1(gen) + hardLandTroop[0];
            std::cout << "兵士種類: " << indexToType[type] << std::endl;
            troop[type].push_back(troopToInstance[type]());

            mapA.back().back()->totalFoe++;
            mapA.back().back()->foeCount[indexToType[type]]++;
            damage[0] += troop[type].back()->getSoftAttack();
            damage[1] += troop[type].back()->getHardAttack();
            damage[2] += troop[type].back()->getAirAttack();
            troopC[type]++;
            totalTroopC++;
          }

          dint1 = std::discrete_distribution<>(std::next(weight[templateIndex].begin(), noHardLandTroop[0]), std::next(weight[templateIndex].begin(), noHardLandTroop.back() + 1));

          while (damage[0] < damageToReach[0])
          {
            int type = dint1(gen) + noHardLandTroop[0];
            std::cout << "兵士種類: " << indexToType[type] << std::endl;
            troop[type].push_back(troopToInstance[type]());

            mapA.back().back()->totalFoe++;
            mapA.back().back()->foeCount[indexToType[type]]++;
            damage[0] += troop[type].back()->getSoftAttack();
            damage[1] += troop[type].back()->getHardAttack();
            damage[2] += troop[type].back()->getAirAttack();
            troopC[type]++;
            totalTroopC++;
          }

          dint1 = std::discrete_distribution<>(std::next(weight[templateIndex].begin(), airTroop[0]), std::next(weight[templateIndex].begin(), airTroop.back() + 1));

          while (damage[2] < damageToReach[2])
          {
            int type = dint1(gen) + airTroop[0];
            std::cout << "兵士種類: " << indexToType[type] << std::endl;
            troop[type].push_back(troopToInstance[type]());

            mapA.back().back()->totalFoe++;
            mapA.back().back()->foeCount[indexToType[type]]++;
            damage[0] += troop[type].back()->getSoftAttack();
            damage[1] += troop[type].back()->getHardAttack();
            damage[2] += troop[type].back()->getAirAttack();
            troopC[type]++;
            totalTroopC++;
          }

          dint1 = std::discrete_distribution<>(makeArmyD[difficulty], 1 - makeArmyD[difficulty]);

          while (totalTroopC > 0)
          {
            if (!dint1(gen))
            {
              mapA.back().back()->foeCount["army"]++;
              mapA.back().back()->totalFoeArmy.push_back(new ArmyUnit(this->uuid()));
              int1 = std::uniform_int_distribution<>(armySize.first, armySize.second);
              int size = int1(gen);

              for (int i = 0; i < 4; i++)
              {
                std::vector<double> prob;
                std::vector<int> typeRemain = {};
                for (int i = 0; i < troop.size(); i++)
                  if (troop[i].size() > 0)
                    typeRemain.push_back(i);

                if (typeRemain.size() == 0)
                  break;

                for (auto i : typeRemain)
                  if (std::find(armorLandTroop.begin(), armorLandTroop.end(), i) != armorLandTroop.end())
                    prob.push_back(1);

                  else
                    prob.push_back(1 / typeRemain.size());

                dint2 = std::discrete_distribution<>(prob.begin(), prob.end());

                int type = typeRemain[dint2(gen)];
                mapA.back().back()->totalFoeArmy.back()->addTroopM(0, i, troop[type].back());
                troop[type].pop_back();
                totalTroopC--;
              }


              for (int i = 0; i < 4; i++)
              {
                std::vector<double> prob;
                std::vector<int> typeRemain = {};

                for (int i = 0; i < troop.size(); i++)
                  if (troop[i].size() > 0)
                    typeRemain.push_back(i);


                if (typeRemain.size() == 0)
                  break;

                for (auto i : typeRemain)

                  if (std::find(armorLandTroop.begin(), armorLandTroop.end(), i) != armorLandTroop.end())
                    prob.push_back(1 / typeRemain.size() * 1.3);

                  else
                    prob.push_back(1 / typeRemain.size());
                dint2 = std::discrete_distribution<>(prob.begin(), prob.end());

                int type = typeRemain[dint2(gen)];
                mapA.back().back()->totalFoeArmy.back()->addTroopM(1, i, troop[type].back());
                troop[type].pop_back();
                totalTroopC--;
              }


              for (int i = 0; i < 4; i++)
              {
                std::vector<double> prob;
                std::vector<int> typeRemain = {};

                for (int i = 0; i < troop.size(); i++)
                  if (troop[i].size() > 0)
                    typeRemain.push_back(i);

                if (typeRemain.size() == 0)
                  break;

                for (auto i : typeRemain)
                  if (std::find(armorLandTroop.begin(), armorLandTroop.end(), i) == armorLandTroop.end())
                    prob.push_back(1 / typeRemain.size() * 1.3);

                  else
                    prob.push_back(1 / typeRemain.size());

                dint2 = std::discrete_distribution<>(prob.begin(), prob.end());

                int type = typeRemain[dint2(gen)];
                mapA.back().back()->totalFoeArmy.back()->addTroopM(2, i, troop[type].back());
                troop[type].pop_back();
                totalTroopC--;
              }

              for (int i = 0; i < 4; i++)
              {
                std::vector<double> prob;
                std::vector<int> typeRemain = {};

                for (int i = 0; i < troop.size(); i++)
                  if (troop[i].size() > 0)
                    typeRemain.push_back(i);

                if (typeRemain.size() == 0)
                  break;

                for (auto i : typeRemain)
                  if (std::find(armorLandTroop.begin(), armorLandTroop.end(), i) == armorLandTroop.end())
                    prob.push_back(1);

                  else
                    prob.push_back(1 / typeRemain.size());
                dint2 = std::discrete_distribution<>(prob.begin(), prob.end());

                int type = typeRemain[dint2(gen)];
                mapA.back().back()->totalFoeArmy.back()->addTroopM(3, i, troop[type].back());
                troop[type].pop_back();
                totalTroopC--;
              }
            }


            else
            {
              std::vector<double> prob;
              std::vector<int> typeRemain = {};


              for (int i = 0; i < troop.size(); i++)
                if (troop[i].size() > 0)
                  typeRemain.push_back(i);
              for (auto i : typeRemain)
                prob.push_back(1 / typeRemain.size());
              dint2 = std::discrete_distribution<>(prob.begin(), prob.end());

              int type = typeRemain[dint2(gen)];
              mapA.back().back()->totalFoeTroop.push_back(troop[type].back());
              troop[type].pop_back();
              totalTroopC--;
            }
          }
          std::cout << "ブロックの兵士は準備された" << std::endl;

          int1 = std::uniform_int_distribution<>(columnProgression[j], 65 + i);
          int progressTo = int1(gen);
          columnProgression[j] = progressTo + 1;
          std::string name = (char)progressTo + std::to_string(j + 1);
          mapA.back().back()->name = name;
          std::cout << "ブロックの名はつけられた" << std::endl;

          int1 = std::uniform_int_distribution<>(0, terrain.size() - 1);
          std::string terrainName = terrain[int1(gen)];
          mapA.back().back()->terrain = terrainName;
          std::cout << "ブロックの地勢はランダムされた" << std::endl;

          if (j > 0 && mapA[i][j - 1] != NULL)
          {
            connect[i][j - 1].second.push_back(std::make_pair(i, j));
            connect[i][j].second.push_back(connect[i][j - 1].first);
          }

          if (i > 0 && mapA[i - 1][j] != NULL)
          {
            connect[i - 1][j].second.push_back(std::make_pair(i, j));
            connect[i][j].second.push_back(connect[i - 1][j].first);
          }

          if (progressTo == 65)
            mapA.back().back()->isAttackable = true;

          if (connect[i][j].second.size() == 0)
            bottomRight[std::make_pair(i, j)] = true;
          else
          {
            std::map<std::pair<int, int>, bool> visited;
            std::vector<std::pair<int, int>> node = {std::make_pair(i, j)};
            visited[std::make_pair(i, j)] = true;


            while (node.size() > 0)
            {
              for (auto i : connect[node.front().first][node.front().second].second)
              {
                assert(mapA[i.first][i.second] != NULL);
                if (visited.count(i) == 0)
                {
                  if (bottomRight.count(i) > 0)
                    bottomRight.erase(i);
                  node.push_back(i);
                  visited[i] = true;
                }
              }
              node.erase(node.begin());
            }
            bottomRight[std::make_pair(i, j)] = true;
          }

          made++;
          std::cout << "ブロック完了" << std::endl;
        }

        else
        {
          connect.back().push_back(std::make_pair(std::make_pair(-1, -1), std::vector<std::pair<int, int>>()));
          mapA.back().push_back(NULL);
        }
        makePossible--;
      }
    }

    for (int i = 0; i < dimension[0]; i++)
      for (int j = 0; j < dimension[1]; j++)
        if (mapA[i][j] != NULL)
          for (auto k : connect[i][j].second)
          {
            mapA[i][j]->attackable.push_back(k);
            mapA[i][j]->encircled.push_back(k);
          }

    std::cout << "全てのブロックの攻撃と囲みの仕方はセットされた" << std::endl;

    ptr->map = mapA;
    ptr->totalLand = made;
    std::cout << nameChosen << "　国の準備が完了" << std::endl;
  }

  std::cout << "全て完了。。。はず" << std::endl;

  std::cout << "ゲームを楽しんでいただければ幸いです（クラッシュしなかったらだな、笑）" << std::endl;

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  return this->start();
}
























