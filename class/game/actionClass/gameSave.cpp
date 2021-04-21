#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <ctime>

#include "../game.h"

void Game::save(int &gamePhase, int prevPhase)
{
  this->lg3.lock();

  time_t t = std::time(NULL);
  tm *ttime = localtime(&t);
  std::string path = std::string("SAVEGAME/") + std::to_string(1900 + ttime->tm_year) + "-" + std::to_string(1 + ttime->tm_mon) + "-" + std::to_string(ttime->tm_mday) + "-" + std::to_string(ttime->tm_hour) + "-" + std::to_string(1 + ttime->tm_min) + "-" + std::to_string(1 + ttime->tm_sec);
  std::ofstream file(path, std::ios::out | std::ios::trunc);

  std::cout << "\033[2J\033[1;1H";
  std::cout << "ゲームを一時停止されている" << std::endl;
  this->paused = true;

  std::cout << "----- 注意：訓練や建物や戦闘のプログレスは全て保存されない、結果と影響だけは保存される -----" << std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  std::cout << std::endl
            << std::endl;
  
  if(this->successAction)
    file << "success:" << this->success[0] << "-" << this->success[1] << "=" << this->success[2] << std::endl;
  
  std::cout << "データの保存..." << std::endl;

  file << "time:" << this->timeAcc << "-" << this->day << "=" << this->timeLimit << std::endl;
  std::cout << "時間の保存完了" << std::endl;

  std::unordered_map<std::string, int> count = {
      {"infantry", 0},
      {"calvary", 0},
      {"suicideBomber", 0},
      {"artillery", 0},
      {"logistic", 0},
      {"armoredCar", 0},
      {"tank1", 0},
      {"tank2", 0},
      {"tankOshimai", 0},
      {"cas", 0},
      {"fighter", 0},
      {"bomber", 0},
      {"kamikaze", 0}};
  for (auto i : this->troop->allTroop)
    count[i->type]++;
  for (auto i : count)
    file << "troop:" << i.first << "-=" << i.second << std::endl;

  std::unordered_map<std::string, int> indexToTroop = {
      {"infantry", 0},
      {"calvary", 1},
      {"artillery", 2},
      {"logistic", 3},
      {"armoredCar", 4},
      {"tank1", 5},
      {"tank2", 6},
      {"tankOshimai", 7},
      {"cas", 8},
      {"fighter", 9},
      {"bomber", 10}};
  std::unordered_map<std::string, int> indexToTroop2 = {
      {"infantry", 0},
      {"calvary", 1},
      {"suicideBomber", 2},
      {"artillery", 3},
      {"logistic", 4},
      {"armoredCar", 5},
      {"tank1", 6},
      {"tank2", 7},
      {"tankOshimai", 8},
      {"cas", 9},
      {"fighter", 10},
      {"bomber", 11},
      {"kamikaze", 12}};

  for (auto i : this->army->total)
  {
    file << "army:" << i.first << "-=$";
    std::string temp;
    for (int j = 0; j < 4; j++)
      for (int k = 0; k < 4; k++)
        if (i.second->formation[j][k] != NULL)
          temp += std::string("(") + std::to_string(j) + ":" + std::to_string(k) + ")" + std::to_string(indexToTroop2[i.second->formation[j][k]->type]) + ",";
        else
          temp += std::string("(") + std::to_string(j) + ":" + std::to_string(k) + "),";
    file << temp.substr(0, temp.length() - 1) << "$" << std::endl;
  }
  std::cout << "軍隊と兵士の保存完了" << std::endl;

  file << "set:food-=" << this->resource->food << std::endl;
  std::cout << "食物量の保存完了" << std::endl;
  file << "set:equipment-=" << this->resource->equipment << std::endl;
  std::cout << "装備量の保存完了" << std::endl;
  file << "set:manpower-=" << this->resource->manpower << std::endl;
  std::cout << "労働力人口の保存完了" << std::endl;
  file << "set:baseLand-=" << this->resource->baseLand << std::endl;
  std::cout << "元土地数の保存完了" << std::endl;
  file << "set:usedLand-=" << this->resource->usedLand << std::endl;
  std::cout << "使っている土地数の保存完了" << std::endl;
  file << "set:capturedLand-=" << this->resource->capturedLand << std::endl;
  std::cout << "捕獲した土地数の保存完了" << std::endl;
  file << "set:camp-=" << this->resource->camp << std::endl;
  std::cout << "軍隊キャンプ数の保存完了" << std::endl;
  file << "set:airport1-=" << this->resource->airport << std::endl;
  std::cout << "空港数の保存完了" << std::endl;
  file << "set:baseLandMul-=" << this->resource->baseLandMul << std::endl;
  std::cout << "マルチプライヤー一つ目の保存完了" << std::endl;
  file << "set:baseLandTroopMul-=" << this->resource->baseLandTroopMul << std::endl;
  std::cout << "マルチプライヤー２つ目の保存完了" << std::endl;
  file << "set:baseAirTroopMul-=" << this->resource->baseAirTroopMul << std::endl;
  std::cout << "マルチプライヤー３つ目の保存完了" << std::endl;
  file << "set:baseTankMul-=" << this->resource->baseTankMul << std::endl;
  std::cout << "マルチプライヤー４つ目の保存完了" << std::endl;
  file << "set:baseAirToopMul2-=" << this->resource->baseAirToopMul2 << std::endl;
  std::cout << "マルチプライヤー５つ目の保存完了" << std::endl;
  file << "set:baseTrainingTimeMul-=" << this->resource->baseTrainingTimeMul << std::endl;
  std::cout << "マルチプライヤー６つ目の保存完了" << std::endl;
  file << "set:baseRecovery-=" << this->resource->baseRecovery << std::endl;
  std::cout << "基本回復力の保存完了" << std::endl;
  file << "set:baseRecoveryDiff-=" << this->resource->baseRecoveryDiff << std::endl;
  std::cout << "回復力修飾子の保存完了" << std::endl;
  file << "set:farm-=" << this->building->farm[0] << "," << this->building->farm[1] << "," << this->building->farm[2] << std::endl;
  std::cout << "農地数の保存完了" << std::endl;
  file << "set:civilianFactory-=" << this->building->civilianFactory[0] << "," << this->building->civilianFactory[1] << "," << this->building->civilianFactory[2] << std::endl;
  std::cout << "工場数の保存完了" << std::endl;
  file << "set:militaryFactory-=" << this->building->militaryFactory[0] << "," << this->building->militaryFactory[1] << "," << this->building->militaryFactory[2] << std::endl;
  std::cout << "工廠数の保存完了" << std::endl;
  file << "set:trainingCamp-=" << this->building->trainingCamp[0] << std::endl;
  std::cout << "軍隊キャンプ数の保存完了（建築物）" << std::endl;
  file << "set:airport2-=" << this->building->airport[0] << std::endl;
  std::cout << "空港数の保存完了（建築物）" << std::endl;
  file << "set:farmR-=" << this->research->farm[0] << "," << this->research->farm[1] << "," << this->research->farm[2] << std::endl;
  std::cout << "農地の研究段階の保存完了" << std::endl;
  file << "set:divisionOfLaborR-=" << this->research->divisionOfLabor[0] << "," << this->research->divisionOfLabor[1] << "," << this->research->divisionOfLabor[2] << std::endl;
  std::cout << "工場効率の研究段階の保存完了" << std::endl;
  file << "set:productionLineR-=" << this->research->productionLine[0] << "," << this->research->productionLine[1] << "," << this->research->productionLine[2] << std::endl;
  std::cout << "工廠効率の研究段階の保存完了" << std::endl;
  file << "set:landDoctrineR-=" << this->research->landDoctrine[0] << "," << this->research->landDoctrine[1] << "," << this->research->landDoctrine[2] << std::endl;
  std::cout << "陸上戦略の研究段階の保存完了" << std::endl;
  file << "set:airDoctrineR-=" << this->research->airDoctrine[0] << "," << this->research->airDoctrine[1] << "," << this->research->airDoctrine[2] << std::endl;
  std::cout << "航空戦略の研究段階の保存完了" << std::endl;
  file << "set:urbanizationR-=" << this->research->urbanization[0] << "," << this->research->urbanization[1] << "," << this->research->urbanization[2] << std::endl;
  std::cout << "使える土地を拡大する研究段階の保存完了" << std::endl;
  file << "set:trainingR-=" << this->research->training[0] << "," << this->research->training[1] << "," << this->research->training[2] << std::endl;
  std::cout << "訓練時間を減少する研究段階の保存完了" << std::endl;
  file << "set:weaponR-=" << this->research->weapon[0] << "," << this->research->weapon[1] << "," << this->research->weapon[2] << std::endl;
  std::cout << "攻撃を増加する研究段階の保存完了" << std::endl;
  file << "set:recoveryR-=" << this->research->recovery[0] << "," << this->research->recovery[1] << "," << this->research->recovery[2] << std::endl;
  std::cout << "回復力を増加する研究段階の保存完了" << std::endl;

  std::cout << "敵の詳細の保存..." << std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  for (auto i : this->enemies->totalEnemies)
  {
    file << "enemy:create-" << i->capitulated << "=" << i->name << std::endl;
    file << "map:" << i->name << "-start=" << std::endl;
    for (int j = 0; j < i->map.size(); j++)
      for (int k = 0; k < i->map[j].size(); k++)
      {
        file << j << "," << k << ":";
        if (i->map[j][k] == NULL)
          file << ";" << std::endl;
        else
        {
          file << i->map[j][k]->name << "," << i->map[j][k]->foeCount["infantry"] << "," << i->map[j][k]->foeCount["calvary"] << "," << i->map[j][k]->foeCount["artillery"] << "," << i->map[j][k]->foeCount["logistic"] << "," << i->map[j][k]->foeCount["armoredCar"] << "," << i->map[j][k]->foeCount["tank1"] << "," << i->map[j][k]->foeCount["tank2"] << "," << i->map[j][k]->foeCount["tankOshimai"] << "," << i->map[j][k]->foeCount["cas"] << "," << i->map[j][k]->foeCount["fighter"] << "," << i->map[j][k]->foeCount["bomber"];
          std::cout << "ブロックの兵士の保存完了" << std::endl;

          assert(i->map[j][k]->foeCount["army"] == i->map[j][k]->totalFoeArmy.size());
          for (auto x : i->map[j][k]->totalFoeArmy)
          {
            file << "$";
            std::string temp;
            for (int y = 0; y < 4; y++)
              for (int z = 0; z < 4; z++)
                if (x->formation[y][z] != NULL)
                  temp += std::string("(") + std::to_string(y) + ":" + std::to_string(z) + ")" + std::to_string(indexToTroop[x->formation[y][z]->type]) + ",";
                else
                  temp += std::string("(") + std::to_string(y) + ":" + std::to_string(z) + "),";
            file << temp.substr(0, temp.length() - 1) << "$";
          }
          std::cout << "ブロックの軍隊の保存完了" << std::endl;

          file << "|" << i->map[j][k]->acquirable["farm"] << "," << i->map[j][k]->acquirable["civilianFactory"] << "," << i->map[j][k]->acquirable["militaryFactory"] << "," << i->map[j][k]->acquirable["trainingCamp"] << "," << i->map[j][k]->acquirable["airport"] << "," << i->map[j][k]->acquirable["land"] << "|";
          std::cout << "ブロックの得られる戦利品の保存完了" << std::endl;

          file << "^";
          std::string temp;
          for (auto x : i->map[j][k]->attackable)
          {
            temp += std::to_string(x.first) + "," + std::to_string(x.second) + ",";
          }
          file << temp.substr(0, temp.length() - 1) << "^";
          std::cout << "ブロックを攻撃仕方の保存完了" << std::endl;

          file << "*";
          temp = "";
          for (auto x : i->map[j][k]->encircled)
          {
            temp += std::to_string(x.first) + "," + std::to_string(x.second) + ",";
          }
          file << temp.substr(0, temp.length() - 1) << "*";
          std::cout << "ブロックを囲み方の保存完了" << std::endl;

          file << "(" << i->map[j][k]->terrain << ")";
          std::cout << "ブロックの地勢の保存完了" << std::endl;

          file << "&" << i->map[j][k]->captured << "," << i->map[j][k]->isAttackable << "," << i->map[j][k]->isEncircled << "&;" << std::endl;
          std::cout << "ブロックのステータスの保存完了" << std::endl;
          std::cout << "ブロック " << j << " " << k << " 完了" << std::endl;
        }
      }
    file << "end" << std::endl;
    std::cout << "敵 " << i->name << " の保存完了" << std::endl;
  }

  std::cout << "全て完了" << std::endl;
  std::cout << "間もなくメイン画面に戻る..." << std::endl;

  file.close();
  std::this_thread::sleep_for(std::chrono::milliseconds(2000));

  gamePhase = prevPhase;
  this->paused = false;
  this->lg3.unlock();

  this->gamePhaseSelect[0] = 0;
  this->gamePhaseSelect[1] = 0;
  (this->*this->print[this->gamePhase])(this->gamePhaseSelect[0], this->gamePhaseSelect[1]);
}