#include <string>
#ifndef IO_H
#define IO_H
char getch();
char getche();
void reset();
void clean_stdin();
std::string color(const std::string text, const std::string color);
std::string underline(const std::string text, const std::string color);
#endif