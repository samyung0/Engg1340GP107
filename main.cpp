#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <functional>
#include <unordered_map>
#include <fstream>
#include <cstdlib>
#include <sstream>

#include "io/io.h"
#include "menu/menu.h"
#include "action/stat/stat.h"
#include "action/setting/setting.h"
#include "action/level/level.h"

#include "class/game/game.h"
#include "class/menuWrapper/menuWrapper.h"

#define TOTAL_LEVELS 9
#define TOTAL_DIFFICULTY 2

// TODO
// add speed settings (action/setting/setting, menu/setting)

int init(MenuWrapper &gameStats, std::string &error);

int main()
{
  // functions for displaying menus
  // input y and x indices for currently selected element
  std::vector<void (*)(int, int, MenuWrapper &)> select{&print_menuSelect, &print_playSelect, &print_settingSelect, &print_levelSelect};

  // map to index of functions for displaying menus
  // positive: menu phase
  // negative: action phase
  std::vector<std::vector<std::vector<int>>> map{
      // play, settings, show version -> action 1, quit program -> action 999 (special case)
      {{1}, {2}, {-1}, {-999}},
      // (play): level, random game, -> action 16, go back to phase 0
      {{3}, {4}, {-16}, {0}},
      // (settings): on 30s show status -> action 17, off -> action 18, back to phase 0
      {{-17, -18}, {0}},
      // (levels) -> action 2 to 10 // 9 levels, go back to phase 1
      {{-2, -3, -4}, {-5, -6, -7}, {-8, -9, -10}, {1}},
      // (random games) -> action 11 to 15 // 5 levels of difficulty
      {{-11, -12, -13, -14, -15}}};

  // functions for actions
  // passing menuPhase and gameStats by reference
  // modify the value during the action
  std::vector<void (*)(int &menuPhase, int prevMenuPhase, MenuWrapper &gameStats)> action = {NULL, &statf, &level1f, &level2f, &level3f, &level4f, &level5f, &level6f, &level7f, &level8f, &level9f, NULL, NULL, NULL, NULL, NULL, NULL, &setting1af, &setting1bf};

  // data storage in a struct
  MenuWrapper gameStats(TOTAL_LEVELS, TOTAL_DIFFICULTY);

  // init: read in settings to setting map, read in level progress to progress
  // return 0 if no error, 1 if error in opening files
  std::string error;
  int hasError = init(gameStats, error);
  if (hasError)
  {
    std::cout << "An error occured:\n"
              << error << std::endl;
    return 0;
  }

  // vector of pointers to objects of all games played and playing (allow the use of dynamic memory)
  std::vector<Game *> allGames;

  char input;
  int menuPhase = 0;
  int menuPhaseSelect[2] = {0, 0};

  (*select[menuPhase])(menuPhaseSelect[0], menuPhaseSelect[1], gameStats);
  while (1)
  {
    input = getch();
    int prevMenuPhase = menuPhase;

    // detection for arrowkeys
    // https://stackoverflow.com/questions/10463201/getch-and-arrow-codes
    if (input == '\033')
    {
      getch();
      switch (getch())
      {
      case 'A':
        menuPhaseSelect[0] = (menuPhaseSelect[0] + -1 + map[menuPhase].size()) % map[menuPhase].size();
        menuPhaseSelect[1] = (menuPhaseSelect[1]) % map[menuPhase][menuPhaseSelect[0]].size();
        break;
      case 'B':
        menuPhaseSelect[0] = (menuPhaseSelect[0] + 1) % map[menuPhase].size();
        menuPhaseSelect[1] = (menuPhaseSelect[1]) % map[menuPhase][menuPhaseSelect[0]].size();
        break;
      case 'C':
        menuPhaseSelect[0] = (menuPhaseSelect[0]) % map[menuPhase].size();
        menuPhaseSelect[1] = (menuPhaseSelect[1] + 1) % map[menuPhase][menuPhaseSelect[0]].size();
        break;
      case 'D':
        menuPhaseSelect[0] = (menuPhaseSelect[0]) % map[menuPhase].size();
        menuPhaseSelect[1] = (menuPhaseSelect[1] + -1 + map[menuPhase][menuPhaseSelect[0]].size()) % map[menuPhase][menuPhaseSelect[0]].size();
        break;
      }
    }
    // progress menu phase
    else if (input == '\n')
    {
      menuPhase = map[menuPhase][menuPhaseSelect[0]][menuPhaseSelect[1]];
      menuPhaseSelect[0] = 0;
      menuPhaseSelect[1] = 0;
    }

    // menu phase
    if (menuPhase >= 0)
      (*select[menuPhase])(menuPhaseSelect[0], menuPhaseSelect[1], gameStats);
    else
    // action phase
    {
      if (menuPhase == -999)
      {
        /////////////////////
        // TODO: check for unsaved games
        //////////////////////

        break;
      }
      else
      {
        // allocate thread to action
        // using ref to call by reference to thread
        std::thread actionThread((*action[-menuPhase]), std::ref(menuPhase), prevMenuPhase, std::ref(gameStats));
        actionThread.join();

        if (menuPhase == 999)
        {
          std::cout << "in game" << std::endl;
          allGames.push_back(new Game(gameStats));

          // start game
          std::thread actionThread(&Game::start, *allGames.back());
          actionThread.join();

          // DOES NOT DELETE GAME when still unsaved
          menuPhase = prevMenuPhase;
          std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
        (*select[menuPhase])(menuPhaseSelect[0], menuPhaseSelect[1], gameStats);
      }
    }
  }
}

int init(MenuWrapper &gameStats, std::string &error)
{
  // read settings
  std::fstream settingStream("save/setting.txt", std::ios::in);

  if (settingStream.fail())
  {
    error = "Cannot open setting.txt!";
    return 1;
  }

  std::string input;
  while (std::getline(settingStream, input))
  {
    int val;
    std::string index = input.substr(0, input.find('='));
    val = atoi(input.substr(input.find('=') + 1).c_str());
    gameStats.setting[index] = val;
  }

  settingStream.close();

  // read progress
  std::fstream progressStream("save/progress.txt", std::ios::in);

  if (progressStream.fail())
  {
    error = "Cannot open progress.txt!";
    return 1;
  }

  int lineCount = 0;
  while (std::getline(progressStream, input))
  {
    std::stringstream parse(input);
    int val1, val2;
    parse >> val1 >> val2;
    gameStats.progress[lineCount][0] = val1;
    gameStats.progress[lineCount][1] = val2;
    lineCount++;
  }

  progressStream.close();

  return 0;
}