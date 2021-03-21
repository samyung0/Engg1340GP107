#include <iostream>
#include <string>
#include <vector>
#include <thread>

#include "io/io.h"

#include "menu/menu.h"

#include "game/game.h"

int main()
{
  // store all games played
  std::vector<Game> allGames;

  // menu phase
  char input;
  int arrowKey;
  print_menuSelect(0);
  while (1)
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
        print_menuSelect(-1);
        break;
      case 'B':
        print_menuSelect(1);
        break;
      }
    }
  }
}