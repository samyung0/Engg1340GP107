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
#include "action/load/load.h"
#include "action/randomSelect/randomSelect.h"

#include "class/game/game.h"
#include "class/menuWrapper/menuWrapper.h"

int TOTAL_LEVELS = 9;
int TOTAL_DIFFICULTY = 2;
int SCREENX = 160;
int SCREENY = 38;
int FPS = 10;

int init(MenuWrapper &gameStats, std::string &error);

int main()
{
  // reset terminal
  std::cout << "\033c";
  std::cout << "\033[2J\033[1;1H";

  // map to index of functions for displaying menus
  std::vector<std::vector<std::vector<int>>> map{
      {{1}, {2}, {-1}, {-999}},
      {{3}, {4}, {-16}, {0}},
      {{-17, -18, -19}, {-20, -21, -22, -23, -24, -25}, {0}},
      {{-2, -3, -4}, {-5, -6, -7}, {-8, -9, -10}, {1}},
      {{-11, -12, -13, -14, -15}}};

  // functions for displaying menus
  // input y and x indices for currently selected element, pass settings by reference
  std::vector<void (*)(int, int, MenuWrapper &)> select{&print_menuSelect, &print_playSelect, &print_settingSelect, &print_levelSelect, &print_randomSelect};

  // functions for actions
  // passing menuPhase and settings by reference
  std::vector<void (*)(int &menuPhase, int prevMenuPhase, MenuWrapper &)> action = {
      NULL,
      &statf,
      &level1f,
      &level2f,
      &level3f,
      &level4f,
      &level5f,
      &level6f,
      &level7f,
      &level8f,
      &level9f,
      &randomSelect1,
      &randomSelect2,
      &randomSelect3,
      &randomSelect4,
      &randomSelect5,
      &load,
      &setting1af,
      &setting1bf,
      &setting1cf,
      &setting2af,
      &setting2bf,
      &setting2cf,
      &setting2df,
      &setting2ef,
      &setting2ff};

  // game settings
  MenuWrapper gameStats(TOTAL_LEVELS, TOTAL_DIFFICULTY, SCREENX, SCREENY, FPS);

  // init: read in settings to setting map, read in level progress to progress
  // return 0 if no error, 1 if error in opening files
  std::string error;
  int hasError = init(gameStats, error);
  if (hasError)
  {
    std::cout << "An error occured:\n"
              << error << " Try recloing the repo." << std::endl;
    return 0;
  }

  // vector of pointers to objects of all games played and playing (allow the use of dynamic memory)
  std::vector<Game *> allGames;

  //  std::future<void> temp = std::async(std::launch::async, [&]() {while(1){clean_stdin();std::this_thread::sleep_for(std::chrono::milliseconds(500));} });

  char input;
  int menuPhase = 0;
  int menuPhaseSelect[2] = {0, 0};
  bool restartGame = false;

  std::string path;

  (*select[menuPhase])(menuPhaseSelect[0], menuPhaseSelect[1], gameStats);
  while (1)
  {
    int prevMenuPhase = menuPhase;
    if (!restartGame)
    {
      input = getch();

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
            restartGame = false;
            allGames.push_back(new Game(gameStats.setting, gameStats.width, gameStats.height, gameStats.fps, gameStats.levelpath));

            // start game
            std::future<int> game;
            if (gameStats.difficulty == -1)
              game = std::async(std::launch::async, &Game::fetch, std::ref(allGames.back()));
            else
              game = std::async(std::launch::async, &Game::fetchRandom, std::ref(allGames.back()), gameStats.difficulty, gameStats.setting["completeness"]);
            gameStats.difficulty = -1;
            int r = game.get();

            delete allGames.back();
            allGames.pop_back();
            if (r == 0)
            {
              // normal exit
              menuPhase = 0;
            }
            else if (r == -999)
            {
              // restart game
              restartGame = true;
              menuPhase = -999;
              continue;
            }
          }
          (*select[menuPhase])(menuPhaseSelect[0], menuPhaseSelect[1], gameStats);
        }
      }
    }
    else
    {
      restartGame = false;
      allGames.push_back(new Game(gameStats.setting, gameStats.width, gameStats.height, gameStats.fps, gameStats.levelpath));

      // start game
      std::future<int> game;
      if (gameStats.difficulty == -1)
        game = std::async(std::launch::async, &Game::fetch, std::ref(allGames.back()));
      else
        game = std::async(std::launch::async, &Game::fetchRandom, std::ref(allGames.back()), gameStats.difficulty, gameStats.setting["completeness"]);
      gameStats.difficulty = -1;
      int r = game.get();

      delete allGames.back();
      allGames.pop_back();
      if (r == 0)
      {
        // normal exit
        menuPhase = 0;
        (*select[menuPhase])(menuPhaseSelect[0], menuPhaseSelect[1], gameStats);
      }
      else if (r == -999)
      {
        // restart game
        restartGame = true;
        menuPhase = -999;
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
    if (input == "")
      continue;
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