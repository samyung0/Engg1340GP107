#include <iostream>
#include <string>
#include <iomanip>

#include "../../io/io.h"
#include "game.h"

void Game::printResearch(int x, int y)
{
  this->lg3.lock();
    std::vector<std::string> prefix = {"   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ", "   "};
    std::vector<std::string> name = {
        "Farm", "Division of Labour", "Production Line", "Land Doctrine", "Air Doctrine", "Urbanization", "Weapon", "Training", "Recovery"};
    std::vector<std::string> suffix = {"", "", "", "", "", "", "", "", ""};
    std::vector<std::vector<std::string>> data = {
        {" (40 days) Unlock Level 2 farm", " (40 days) Unlock Level 3 farm"},
        {" (20 days) Unlock Level 2 civilian factory", " (30 days) Unlock Level 3 civilian factory"},
        {" (15 days) Unlock Level 2 military factory", " (120 days) Unlock Level 3 military factory"},
        {" (40 days) +20% damage dealt by each unit of land troop", " (40 days) +30% (total) damage dealt by each unit of land troop"},
        {" (70 days) +40% damage dealt by each unit of plane", " (100 days) +60% (total) damage dealt by each unit of plane"},
        {" (50 days) +10% land available for building", " (70 days) +30% (total) land available for building"},
        {" (20 days) +5% damage dealt by each unit of tank", " (70 days) +10% (total) damage dealt by each unit of tank and plane"},
        {" (30 days) -10% training time required to train any unit of land troop", " (50 days) -20% (total) trianing time required to train any unit of land troop"},
        {" (30 days) Recovery rate increased to 6 everyday", " (50 days) Recovery rate increased to 8 everyday"},
    };
    std::vector<int> dataMatch = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::vector<bool> shouldPrint = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    if (!this->research->farm[1])
    {
        suffix[0] = " (intermediate)";
    }
    else if (!this->research->farm[2])
    {
        suffix[0] = " (advanced)";
        dataMatch[0] = 1;
    }
    else
        shouldPrint[0] = 0;
    if (!this->research->divisionOfLabor[1])
    {
        suffix[1] = " (intermediate)";
    }
    else if (!this->research->divisionOfLabor[2])
    {
        suffix[1] = " (advanced)";
        dataMatch[2] = 1;
    }
    else
        shouldPrint[1] = 0;
    if (!this->research->productionLine[1])
    {
        suffix[2] = " (intermediate)";
    }
    else if (!this->research->productionLine[2])
    {
        suffix[2] = " (advanced)";
        dataMatch[2] = 1;
    }
    else
        shouldPrint[2] = 0;
    if (!this->research->landDoctrine[1])
    {
        suffix[3] = " (intermediate)";
    }
    else if (!this->research->landDoctrine[2])
    {
        suffix[3] = " (advanced)";
        dataMatch[3] = 1;
    }
    else
        shouldPrint[3] = 0;
    if (!this->research->airDoctrine[1])
    {
        suffix[4] = " (intermediate)";
    }
    else if (!this->research->airDoctrine[2])
    {
        suffix[4] = " (advanced)";
        dataMatch[4] = 1;
    }
    else
        shouldPrint[4] = 0;
    if (!this->research->urbanization[1])
    {
        suffix[5] = " (intermediate)";
    }
    else if (!this->research->urbanization[2])
    {
        suffix[5] = " (advanced)";
        dataMatch[5] = 1;
    }
    else
        shouldPrint[5] = 0;
    if (!this->research->weapon[1])
    {
        suffix[6] = " (intermediate)";
    }
    else if (!this->research->weapon[2])
    {
        suffix[6] = " (advanced)";
        dataMatch[6] = 1;
    }
    else
        shouldPrint[6] = 0;
    if (!this->research->training[1])
    {
        suffix[7] = " (intermediate)";
    }
    else if (!this->research->training[2])
    {
        suffix[7] = " (advanced)";
        dataMatch[7] = 1;
    }
    else
        shouldPrint[7] = 0;
    if (!this->research->recovery[1])
    {
        suffix[8] = " (intermediate)";
    }
    else if (!this->research->recovery[2])
    {
        suffix[8] = " (advanced)";
        dataMatch[8] = 1;
    }
    else
        shouldPrint[8] = 0;

    for (int i = 0; i < name.size(); i++)
        if (shouldPrint[i] && std::get<1>(this->research->progressTrack) == "")
            name[i] = underline(name[i] + suffix[i], "green");
        else
            name[i] = color(name[i] + suffix[i], "red");

    prefix[x].erase(1, 1);
    prefix[x].insert(1, color(">", "cyan"));

    std::vector<std::string> printingList = {};
    for (int i = 0; i < name.size(); i++)
    {
        std::string body = name[i];
        body += std::string(45 - body.length(), ' ');
        if (shouldPrint[i])
        {

            printingList.push_back(prefix[i] + body + data[i][dataMatch[i]]);
        }
        else
            printingList.push_back(prefix[i] + body + " (Max)");
    }

    bool hasProgress = std::get<1>(this->research->progressTrack) != "";
    std::string progressDesc = "";
    int progressTime = 0;
    if (hasProgress)
    {
        progressDesc = std::get<0>(this->research->progressTrack);
        progressTime = this->research->progress[std::get<1>(this->research->progressTrack)]->remain;
    }

    std::cout << "\033[2J\033[1;1H";
    std::cout << color("Research", "magenta")
              << std::endl
              << std::endl;
    std::cout << color("Resources:", "green") << "\n"
            << "Food: " << this->troop->totalFoodRequired << "/" <<  this->resource->food
            << "   Equipment: " << this->troop->totalEquipmentRequired << "/" << this->resource->equipment
            << "   Manpower: " << (this->resource->manpower - this->resource->manpowerInUse) << "/" << this->resource->manpower
            << "   Land: " << (this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand - this->resource->usedLand) << "/" << this->resource->baseLand * this->resource->baseLandMul + this->resource->capturedLand
            << "   Troop: " << this->troop->totalTroops
            << "   Armies: " << this->army->total.size() << "/10"
            << "   Camps: " << this->resource->campUsed << "/"<<this->resource->camp
            << "   Airports: " << this->resource->airportUsed << "/"<<this->resource->airport
              << std::endl
              << std::endl;
    std::cout << std::setw(130) << color("Upgrade", "green") << color("Overview", "green") << std::endl
              << std::setw((x == 0 ? 141 : 130)) << printingList[0] << "Farm: " << this->helper(this->research->farm) << std::endl
              << std::setw((x == 1 ? 141 : 130)) << printingList[1] << "Division of Labour: " << this->helper(this->research->divisionOfLabor) << std::endl
              << std::setw((x == 2 ? 141 : 130)) << printingList[2] << "Production Line: " << this->helper(this->research->productionLine) << std::endl
              << std::setw((x == 3 ? 141 : 130)) << printingList[3] << "Land Doctrine: " << this->helper(this->research->landDoctrine) << std::endl
              << std::setw((x == 4 ? 141 : 130)) << printingList[4] << "Air Doctrine: " << this->helper(this->research->airDoctrine) << std::endl
              << std::setw((x == 5 ? 141 : 130)) << printingList[5] << "Urbanization: " << this->helper(this->research->urbanization) << std::endl
              << std::setw((x == 6 ? 141 : 130)) << printingList[6] << "Weapon: " << this->helper(this->research->weapon) << std::endl
              << std::setw((x == 7 ? 141 : 130)) << printingList[7] << "Training: " << this->helper(this->research->training) << std::endl
              << std::setw((x == 8 ? 141 : 130)) << printingList[8] << "Recovery: " << this->helper(this->research->recovery) << std::endl
              << std::endl
              << color("In Progress", "green") << std::endl
              << (hasProgress ? (progressDesc + " (" + std::to_string(progressTime) + ")") : "no research in progress") << std::endl
              << std::endl
              << prefix[9] << underline("Return", "green") << " (Or spacebar)" << std::endl
              << std::endl;
    this->lg3.unlock();
}