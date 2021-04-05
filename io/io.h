#include <string>
#ifndef IO_H
#define IO_H
char getch();
void reset();
std::string color(const std::string text, const std::string color);
std::string underline(const std::string text, const std::string color);
#endif