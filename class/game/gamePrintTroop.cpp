#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <unordered_map>

#include "../../io/io.h"
#include "game.h"
#include "../../data/troop/troop.h"

void Game::printTroop(int x, int y)
{
  this->lg3.lock();

  int freeCamp = this->resource->camp - this->resource->campUsed;
  std::vector<int> maxTrain = {
      freeCamp / Infantry::trainingCamp,
      freeCamp / Calvary::trainingCamp,
      freeCamp / SuicideBomber::trainingCamp,
      freeCamp / Artillery::trainingCamp,
      freeCamp / Logistic::trainingCamp,
      freeCamp / ArmoredCar::trainingCamp,
      freeCamp / Tank1::trainingCamp,
      freeCamp / Tank2::trainingCamp,
      freeCamp / TankOshimai::trainingCamp,
      freeCamp / Cas::trainingCamp,
      freeCamp / Fighter::trainingCamp,
      freeCamp / Bomber::trainingCamp,
      freeCamp / Kamikaze::trainingCamp};
  std::vector<int> maxRemove = {
      this->troop->infantry[0],
      this->troop->calvary[0],
      this->troop->suicideBomber[0],
      this->troop->artillery[0],
      this->troop->logistic[0],
      this->troop->armoredCar[0],
      this->troop->tank1[0],
      this->troop->tank2[0],
      this->troop->tankOshimai[0],
      this->troop->cas[0],
      this->troop->fighter[0],
      this->troop->bomber[0],
      this->troop->kamikaze[0]};

  std::unordered_map<std::string, int> trainingC = {};
  std::unordered_map<std::string, std::string> training = {};
  for (int i = 0; i < this->troop->progressTrack.size(); i++)
  {
    std::string type = std::get<0>(this->troop->progressTrack[i]);
    std::string id = std::get<1>(this->troop->progressTrack[i]);
    if (trainingC.count(type) == 0)
    {
      trainingC[type] = 1;
      training[type] = " (" + std::to_string(this->troop->progress[id]->remain) + ", ";
    }
    else
    {
      trainingC[type]++;
      training[type] += std::to_string(this->troop->progress[id]->remain) + ", ";
    }
  }
  for (auto i : trainingC)
  {
    training[i.first] = std::to_string(i.second) + training[i.first].substr(0, training[i.first].size() - 2) + " days)";
  }

  std::vector<std::vector<std::string>> actionPrefix = {

      {"   ", "   ", "   ", "   ", "   ", "   "},
      {"   ", "   ", "   ", "   ", "   ", "   "},
      {"   ", "   ", "   ", "   ", "   ", "   "},
      {"   ", "   ", "   ", "   ", "   ", "   "},
      {"   ", "   ", "   ", "   ", "   ", "   "},
      {"   ", "   ", "   ", "   ", "   ", "   "},
      {"   ", "   ", "   ", "   ", "   ", "   "},
      {"   ", "   ", "   ", "   ", "   ", "   "},
      {"   ", "   ", "   ", "   ", "   ", "   "},
      {"   ", "   ", "   ", "   ", "   ", "   "},
      {"   ", "   ", "   ", "   ", "   ", "   "},
      {"   ", "   ", "   ", "   ", "   ", "   "},
      {"   ", "   ", "   ", "   ", "   ", "   "},
      {"   "}};

  std::vector<std::vector<std::string>> action = {

      {"1", "5", "10", "Max", "1", "All"},
      {"1", "5", "10", "Max", "1", "All"},
      {"1", "5", "10", "Max", "1", "All"},
      {"1", "5", "10", "Max", "1", "All"},
      {"1", "5", "10", "Max", "1", "All"},
      {"1", "5", "10", "Max", "1", "All"},
      {"1", "5", "10", "Max", "1", "All"},
      {"1", "5", "10", "Max", "1", "All"},
      {"1", "5", "10", "Max", "1", "All"},
      {"1", "5", "10", "Max", "1", "All"},
      {"1", "5", "10", "Max", "1", "All"},
      {"1", "5", "10", "Max", "1", "All"},
      {"1", "5", "10", "Max", "1", "All"},
      {"Back"}};

  for (int i = 0; i < action.size() - 1; i++)
    action[i][3] += " (" + std::to_string(maxTrain[i]) + ") ";
  for (int i = 0; i < action.size() - 1; i++)
    action[i][5] += " (" + std::to_string(maxRemove[i]) + ") ";
  for (int i = 0; i < action.size() - 1; i++)
  {
    if (maxTrain[i] < 10)
      action[i][2] = color(action[i][2], "red");
    else
      action[i][2] = underline(action[i][2], "green");
    if (maxTrain[i] < 5)
      action[i][1] = color(action[i][1], "red");
    else
      action[i][1] = underline(action[i][1], "green");
    if (maxTrain[i] < 1)
    {
      action[i][0] = color(action[i][0], "red");
      action[i][3] = color(action[i][3], "red");
    }
    else
    {
      action[i][0] = underline(action[i][0], "green");
      action[i][3] = underline(action[i][3], "green");
    }
    if (maxRemove[i] > 0)
    {
      action[i][4] = underline(action[i][4], "green");
      action[i][5] = underline(action[i][5], "green");
    }
    else
    {
      action[i][4] = color(action[i][4], "red");
      action[i][5] = color(action[i][5], "red");
    }
  }
  action[action.size() - 1][0] = underline(action[action.size() - 1][0], "green");

  actionPrefix[x][y].erase(1, 1);
  actionPrefix[x][y].insert(1, color(">", "cyan"));

  std::cout << "\033[2J\033[1;1H";
  std::cout << std::left
            << color("Troop Training: ", "magenta")
            << std::endl
            << std::endl;
  std::cout << color("Resources:", "green") << "\n"
            << "Food: " << this->troop->totalFoodRequired << "/" << this->resource->food
            << "   Equipment: " << this->resource->equipment << "/" << this->troop->totalEquipmentRequired
            << "   Manpower: " << (this->resource->manpower - this->resource->manpowerInUse) << "/" << this->resource->manpower
            << "   Land: " << (this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand) << "/" << this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand
            << "   Troop: " << this->troop->totalTroops
            << "   Armies: " << this->army->total.size() << "/10"
            << "   Battle Plans: " << this->battlePlan->total.size() << "/10"
            << std::endl
            << std::endl;

  std::cout << std::setw(50 + 11) << color("Train: ", "green")
            << std::setw(43 + 11) << color("Remove: ", "green")
            << color("Training: ", "green")
            << std::endl;
  std::cout << std::setw((x == 0 && y < 4 ? 105 : 94)) << "Infantry:" + std::string(15 - 8, ' ') + actionPrefix[0][0] + action[0][0] + actionPrefix[0][1] + action[0][1] + actionPrefix[0][2] + action[0][2] + actionPrefix[0][3] + action[0][3]
            << std::setw((x == 0 && y > 3 ? 76: 65)) << "Infantry:" + std::string(15 - 8, ' ') + actionPrefix[0][4] + action[0][4] + actionPrefix[0][5] + action[0][5]
            << "Infantry:" + std::string(18 - 8, ' ') + (training.count("infantry") == 0 ? "0" : training["infantry"]) << std::endl;
  std::cout << std::setw((x == 1 && y < 4 ? 105 : 94)) << "Calvary:" + std::string(15 - 7, ' ') + actionPrefix[1][0] + action[1][0] + actionPrefix[1][1] + action[1][1] + actionPrefix[1][2] + action[1][2] + actionPrefix[1][3] + action[1][3]
            << std::setw((x == 1 && y > 3 ? 76: 65)) << "Calvary:" + std::string(15 - 7, ' ') + actionPrefix[1][4] + action[1][4] + actionPrefix[1][5] + action[1][5]
            << "Calvary:" + std::string(18 - 7, ' ') + (training.count("calvary") == 0 ? "0" : training["calvary"]) << std::endl;
  std::cout << std::setw((x == 2 && y < 4 ? 105 : 94)) << "Suicide Bomber:" + std::string(15 - 14, ' ') + actionPrefix[2][0] + action[2][0] + actionPrefix[2][1] + action[2][1] + actionPrefix[2][2] + action[2][2] + actionPrefix[2][3] + action[2][3]
            << std::setw((x == 2 && y > 3 ? 76: 65)) << "Suicide Bomber:" + std::string(15 - 14, ' ') + actionPrefix[2][4] + action[2][4] + actionPrefix[2][5] + action[2][5]
            << "Suicide Bomber:" + std::string(18 - 14, ' ') + (training.count("suicideBomber") == 0 ? "0" : training["suicideBomber"]) << std::endl;
  std::cout << std::setw((x == 3 && y < 4 ? 105 : 94)) << "Artillery:" + std::string(15 - 9, ' ') + actionPrefix[3][0] + action[3][0] + actionPrefix[3][1] + action[3][1] + actionPrefix[3][2] + action[3][2] + actionPrefix[3][3] + action[3][3]
            << std::setw((x == 3 && y > 3 ? 76: 65)) << "Artillery:" + std::string(15 - 9, ' ') + actionPrefix[3][4] + action[3][4] + actionPrefix[3][5] + action[3][5]
            << "Artillery:" + std::string(18 - 9, ' ') + (training.count("artillery") == 0 ? "0" : training["artillery"]) << std::endl;
  std::cout << std::setw((x == 4 && y < 4 ? 105 : 94)) << "Logistic:" + std::string(15 - 8, ' ') + actionPrefix[4][0] + action[4][0] + actionPrefix[4][1] + action[4][1] + actionPrefix[4][2] + action[4][2] + actionPrefix[4][3] + action[4][3]
            << std::setw((x == 4 && y > 3 ? 76: 65)) << "Logistic:" + std::string(15 - 8, ' ') + actionPrefix[4][4] + action[4][4] + actionPrefix[4][5] + action[4][5]
            << "Logistic:" + std::string(18 - 8, ' ') + (training.count("logistic") == 0 ? "0" : training["logistic"]) << std::endl;
  std::cout << std::setw((x == 5 && y < 4 ? 105 : 94)) << "Armored Car:" + std::string(15 - 11, ' ') + actionPrefix[5][0] + action[5][0] + actionPrefix[5][1] + action[5][1] + actionPrefix[5][2] + action[5][2] + actionPrefix[5][3] + action[5][3]
            << std::setw((x == 5 && y > 3 ? 76: 65)) << "Armored Car:" + std::string(15 - 11, ' ') + actionPrefix[5][4] + action[5][4] + actionPrefix[5][5] + action[5][5]
            << "Armored Car:" + std::string(18 - 11, ' ') + (training.count("armoredCar") == 0 ? "0" : training["armoredCar"]) << std::endl;
  std::cout << std::setw((x == 6 && y < 4 ? 105 : 94)) << "Tank 1:" + std::string(15 - 6, ' ') + actionPrefix[6][0] + action[6][0] + actionPrefix[6][1] + action[6][1] + actionPrefix[6][2] + action[6][2] + actionPrefix[6][3] + action[6][3]
            << std::setw((x == 6 && y > 3 ? 76: 65)) << "Tank 1:" + std::string(15 - 6, ' ') + actionPrefix[6][4] + action[6][4] + actionPrefix[6][5] + action[6][5]
            << "Tank 1:" + std::string(18 - 6, ' ') + (training.count("tank1") == 0 ? "0" : training["tank1"]) << std::endl;
  std::cout << std::setw((x == 7 && y < 4 ? 105 : 94)) << "Tank 2:" + std::string(15 - 6, ' ') + actionPrefix[7][0] + action[7][0] + actionPrefix[7][1] + action[7][1] + actionPrefix[7][2] + action[7][2] + actionPrefix[7][3] + action[7][3]
            << std::setw((x == 7 && y > 3 ? 76: 65)) << "Tank 2:" + std::string(15 - 6, ' ') + actionPrefix[7][4] + action[7][4] + actionPrefix[7][5] + action[7][5]
            << "Tank 2:" + std::string(18 - 6, ' ') + (training.count("tank2") == 0 ? "0" : training["tank2"]) << std::endl;
  std::cout << std::setw((x == 8 && y < 4 ? 105 : 94)) << "Tank Oshimai:" + std::string(15 - 12, ' ') + actionPrefix[8][0] + action[8][0] + actionPrefix[8][1] + action[8][1] + actionPrefix[8][2] + action[8][2] + actionPrefix[8][3] + action[8][3]
            << std::setw((x == 8 && y > 3 ? 76: 65)) << "Tank Oshimai:" + std::string(15 - 12, ' ') + actionPrefix[8][4] + action[8][4] + actionPrefix[8][5] + action[8][5]
            << "Tank Oshimai:" + std::string(18 - 12, ' ') + (training.count("tankOshimai") == 0 ? "0" : training["tankOshimai"]) << std::endl;
  std::cout << std::setw((x == 9 && y < 4 ? 105 : 94)) << "Cas:" + std::string(15 - 3, ' ') + actionPrefix[9][0] + action[9][0] + actionPrefix[9][1] + action[9][1] + actionPrefix[9][2] + action[9][2] + actionPrefix[9][3] + action[9][3]
            << std::setw((x == 9 && y > 3 ? 76: 65)) << "Cas:" + std::string(15 - 3, ' ') + actionPrefix[9][4] + action[9][4] + actionPrefix[9][5] + action[9][5]
            << "Cas:" + std::string(18 - 3, ' ') + (training.count("cas") == 0 ? "0" : training["cas"]) << std::endl;
  std::cout << std::setw((x == 10 && y < 4 ? 105 : 94)) << "Fighter:" + std::string(15 - 7, ' ') + actionPrefix[10][0] + action[10][0] + actionPrefix[10][1] + action[10][1] + actionPrefix[10][2] + action[10][2] + actionPrefix[10][3] + action[10][3]
            << std::setw((x == 10 && y > 3 ? 76: 65)) << "Fighter:" + std::string(15 - 7, ' ') + actionPrefix[10][4] + action[10][4] + actionPrefix[10][5] + action[10][5]
            << "Fighter:" + std::string(18 - 7, ' ') + (training.count("fighter") == 0 ? "0" : training["fighter"]) << std::endl;
  std::cout << std::setw((x == 11 && y < 4 ? 105 : 94)) << "Bomber:" + std::string(15 - 6, ' ') + actionPrefix[11][0] + action[11][0] + actionPrefix[11][1] + action[11][1] + actionPrefix[11][2] + action[11][2] + actionPrefix[11][3] + action[11][3]
            << std::setw((x == 11 && y > 3 ? 76: 65)) << "Bomber:" + std::string(15 - 6, ' ') + actionPrefix[11][4] + action[11][4] + actionPrefix[11][5] + action[11][5]
            << "Bomber:" + std::string(18 - 6, ' ') + (training.count("bomber") == 0 ? "0" : training["bomber"]) << std::endl;
  std::cout << std::setw((x == 12 && y < 4 ? 105 : 94)) << "Kamikaze:" + std::string(15 - 8, ' ') + actionPrefix[12][0] + action[12][0] + actionPrefix[12][1] + action[12][1] + actionPrefix[12][2] + action[12][2] + actionPrefix[12][3] + action[12][3]
            << std::setw((x == 12 && y > 3 ? 76: 65)) << "Kamikaze:" + std::string(15 - 8, ' ') + actionPrefix[12][4] + action[12][4] + actionPrefix[12][5] + action[12][5]
            << "Kamikaze:" + std::string(18 - 8, ' ') + (training.count("kamikaze") == 0 ? "0" : training["kamikaze"]) << std::endl
            << std::endl;
  std::cout << std::setw(55) << "(free, in army, in battle plan, in battle)"
            << actionPrefix[action.size() - 1][0] + action[action.size() - 1][0] + " (or spacebar)"
            << std::endl;
  std::cout << color("Overview:", "green") << std::endl
            << std::right
            << "Infantry:" + std::string(15 - 8, ' ') << std::setw(4) << this->troop->infantry[0] << "/" << std::setw(4) << this->troop->infantry[1] << "/" << std::setw(4) << this->troop->infantry[2] << "/" << std::setw(4) << this->troop->infantry[3] << "   (" << std::setw(5) << (this->troop->infantry[0] + this->troop->infantry[1] + this->troop->infantry[2] + this->troop->infantry[3]) << " total)" << std::endl
            << "Calvary:" + std::string(15 - 7, ' ') << std::setw(4) << this->troop->calvary[0] << "/" << std::setw(4) << this->troop->calvary[1] << "/" << std::setw(4) << this->troop->calvary[2] << "/" << std::setw(4) << this->troop->calvary[3] << "   (" << std::setw(5) << (this->troop->calvary[0] + this->troop->calvary[1] + this->troop->calvary[2] + this->troop->calvary[3]) << " total)" << std::endl
            << "Suicide Bomber:" + std::string(15 - 14, ' ') << std::setw(4) << this->troop->suicideBomber[0] << "/" << std::setw(4) << this->troop->suicideBomber[1] << "/" << std::setw(4) << this->troop->suicideBomber[2] << "/" << std::setw(4) << this->troop->suicideBomber[3] << "   (" << std::setw(5) << (this->troop->suicideBomber[0] + this->troop->suicideBomber[1] + this->troop->suicideBomber[2] + this->troop->suicideBomber[3]) << " total)" << std::endl
            << "Artillery:" + std::string(15 - 9, ' ') << std::setw(4) << this->troop->artillery[0] << "/" << std::setw(4) << this->troop->artillery[1] << "/" << std::setw(4) << this->troop->artillery[2] << "/" << std::setw(4) << this->troop->artillery[3] << "   (" << std::setw(5) << (this->troop->artillery[0] + this->troop->artillery[1] + this->troop->artillery[2] + this->troop->artillery[3]) << " total)" << std::endl
            << "Logistic:" + std::string(15 - 8, ' ') << std::setw(4) << this->troop->logistic[0] << "/" << std::setw(4) << this->troop->logistic[1] << "/" << std::setw(4) << this->troop->logistic[2] << "/" << std::setw(4) << this->troop->logistic[3] << "   (" << std::setw(5) << (this->troop->logistic[0] + this->troop->logistic[1] + this->troop->logistic[2] + this->troop->logistic[3]) << " total)" << std::endl
            << "Armored Car:" + std::string(15 - 11, ' ') << std::setw(4) << this->troop->armoredCar[0] << "/" << std::setw(4) << this->troop->armoredCar[1] << "/" << std::setw(4) << this->troop->armoredCar[2] << "/" << std::setw(4) << this->troop->armoredCar[3] << "   (" << std::setw(5) << (this->troop->armoredCar[0] + this->troop->armoredCar[1] + this->troop->armoredCar[2] + this->troop->armoredCar[3]) << " total)" << std::endl
            << "Tank 1:" + std::string(15 - 6, ' ') << std::setw(4) << this->troop->tank1[0] << "/" << std::setw(4) << this->troop->tank1[1] << "/" << std::setw(4) << this->troop->tank1[2] << "/" << std::setw(4) << this->troop->tank1[3] << "   (" << std::setw(5) << (this->troop->tank1[0] + this->troop->tank1[1] + this->troop->tank1[2] + this->troop->tank1[3]) << " total)" << std::endl
            << "Tank 2:" + std::string(15 - 6, ' ') << std::setw(4) << this->troop->tank2[0] << "/" << std::setw(4) << this->troop->tank2[1] << "/" << std::setw(4) << this->troop->tank2[2] << "/" << std::setw(4) << this->troop->tank2[3] << "   (" << std::setw(5) << (this->troop->tank2[0] + this->troop->tank2[1] + this->troop->tank2[2] + this->troop->tank2[3]) << " total)" << std::endl
            << "Tank Oshimai:" + std::string(15 - 12, ' ') << std::setw(4) << this->troop->tankOshimai[0] << "/" << std::setw(4) << this->troop->tankOshimai[1] << "/" << std::setw(4) << this->troop->tankOshimai[2] << "/" << std::setw(4) << this->troop->tankOshimai[3] << "   (" << std::setw(5) << (this->troop->tankOshimai[0] + this->troop->tankOshimai[1] + this->troop->tankOshimai[2] + this->troop->tankOshimai[3]) << " total)" << std::endl
            << "Cas:" + std::string(15 - 3, ' ') << std::setw(4) << this->troop->cas[0] << "/" << std::setw(4) << this->troop->cas[1] << "/" << std::setw(4) << this->troop->cas[2] << "/" << std::setw(4) << this->troop->cas[3] << "   (" << std::setw(5) << (this->troop->cas[0] + this->troop->cas[1] + this->troop->cas[2] + this->troop->cas[3]) << " total)" << std::endl
            << "Fighter:" + std::string(15 - 7, ' ') << std::setw(4) << this->troop->fighter[0] << "/" << std::setw(4) << this->troop->fighter[1] << "/" << std::setw(4) << this->troop->fighter[2] << "/" << std::setw(4) << this->troop->fighter[3] << "   (" << std::setw(5) << (this->troop->fighter[0] + this->troop->fighter[1] + this->troop->fighter[2] + this->troop->fighter[3]) << " total)" << std::endl
            << "Bomber:" + std::string(15 - 6, ' ') << std::setw(4) << this->troop->bomber[0] << "/" << std::setw(4) << this->troop->bomber[1] << "/" << std::setw(4) << this->troop->bomber[2] << "/" << std::setw(4) << this->troop->bomber[3] << "   (" << std::setw(5) << (this->troop->bomber[0] + this->troop->bomber[1] + this->troop->bomber[2] + this->troop->bomber[3]) << " total)" << std::endl
            << "Kamikaze:" + std::string(15 - 8, ' ') << std::setw(4) << this->troop->kamikaze[0] << "/" << std::setw(4) << this->troop->kamikaze[1] << "/" << std::setw(4) << this->troop->kamikaze[2] << "/" << std::setw(4) << this->troop->kamikaze[3] << "   (" << std::setw(5) << (this->troop->kamikaze[0] + this->troop->kamikaze[1] + this->troop->kamikaze[2] + this->troop->kamikaze[3]) << " total)"
            << std::endl;
  this->lg3.unlock();
}