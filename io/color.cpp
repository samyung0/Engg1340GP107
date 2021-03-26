#include <iostream>
#include <string>

// format text to give colors
// input text and target color as strings
// output formatted text as string
std::string color(const std::string& text, const std::string& color)
{
  if (color == "red")
    return "\033[1;31m" + text + "\033[0m";
  else if (color == "green")
    return "\033[1;32m" + text + "\033[0m";
  else if (color == "magenta")
    return "\033[1;35m" + text + "\033[0m";
  else if (color == "yellow")
    return "\033[1;33m" + text + "\033[0m";
  else 
    // default white
    // does not just return text because we need to align character count
    return "\033[1;37m" + text + "\033[0m";
}