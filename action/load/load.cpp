#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <dirent.h>

#include "../../io/io.h"
#include "../../class/menuWrapper/menuWrapper.h"

// base function to be called by sub functions, load game stats into gamestats and update menuphase
// input game level
void load(int &menuPhase, int prevMenuPhase, MenuWrapper &gameStats)
{
    std::vector<std::string> path = {""};
    std::vector<std::string> print = {std::string(" > " + underline("Back", "green")) + " (or spacebar)"};
    int activeIndex[2] = {0, 0};

  // code from https://stackoverflow.com/questions/306533/how-do-i-get-a-list-of-files-in-a-directory-in-c
  DIR *dpdf;
  struct dirent *epdf;
  try
  {
    dpdf = opendir("./SAVEGAME");
    if (dpdf != NULL)
    {
      readdir(dpdf);
      readdir(dpdf);
      while (epdf = readdir(dpdf))
      {
        path.push_back(std::string(epdf->d_name));
        print.push_back("   " + std::string(epdf->d_name));
      }
    }
    closedir(dpdf);
  }
  catch (...)
  {
    std::cout << "Cannot open SAVEGAME folder! Try recloning the repo!" << std::endl;
    return;
  };

  while (1)
  {
    // clear screen
    std::cout << "\033[2J\033[1;1H";
    std::cout << "Saved files" << std::endl
              << std::endl;
    for (auto i : print)
      std::cout << i << std::endl;

    // detecing arrow keys
    char input;

    input = getch();
    if (input == '\033')
    {
      print[activeIndex[0]] = print[activeIndex[0]].replace(1, 1, " ");
      getch();
      switch (getch())
      {
      case 'A':
        activeIndex[0] = (activeIndex[0] + -1 + print.size()) % print.size();
        break;
      case 'B':
        activeIndex[0] = (activeIndex[0] + 1) % print.size();
        break;
      }
      print[activeIndex[0]] = print[activeIndex[0]].replace(1, 1, ">");
    }
    // progress menu phase
    else if (input == '\n')
    {
      if (activeIndex[0] == 0)
      {
        menuPhase = prevMenuPhase;
        return;
      }
      else
      {
        gameStats.levelpath = "SAVEGAME/" + path[activeIndex[0]];
        // phase 999 calls game to start in main
        menuPhase = 999;
        return;
      }
    }
    else if (input == ' ')
    {
      menuPhase = prevMenuPhase;
      return;
    }
  }
}