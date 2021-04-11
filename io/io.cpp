#include <termios.h>
#include <iostream>
#include <unistd.h>

struct termios prev, current;

// modify terminal so we can capture user's input even without entering
void init(bool echo = false)
{
  tcgetattr(0, &prev);
  current = prev;

  // set no buffer
  current.c_lflag &= ~ICANON;

  // no echo
  if (!echo)
    current.c_lflag &= ~ECHO;
  tcsetattr(0, TCSANOW, &current);
}

void reset()
{
  tcsetattr(0, TCSANOW, &prev);
}

char getch()
{
  char ch;
  init();
  ch = std::getchar();
  reset();
  return ch;
}

char getche()
{
  char ch;
  init(true);
  ch = std::getchar();
  reset();
  return ch;
}