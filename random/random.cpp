#include <random>
#include <vector>
#include "./random.h"

// fist get 3 uniform integer distribution to determine the success rate passing into the binomial distribution
// each binomial distribution's succcess rate should be between 0.15 to 0.35 of 1
std::vector<double> randArmy4(std::random_device& rd)
{
  double remain = 1;
  std::mt19937 gen(rd());

  std::uniform_int_distribution<> dis(15, 35);
  std::uniform_int_distribution<> dis2(20, 46);
  std::uniform_int_distribution<> dis3(30, 70);

  std::binomial_distribution<int> distribution(1000,dis(gen)/100.0);
  double num1 = distribution(gen)/1000.0;
  remain -= num1;
  std::binomial_distribution<int> distribution2((int) (remain * 1000),dis2(gen)/100.0);
  double num2 = distribution2(gen)/1000.0;
  remain -= num2;
  std::binomial_distribution<int> distribution3((int) (remain * 1000),dis3(gen)/100.0);
  double num3 = distribution3(gen)/1000.0;
  remain -= num3;

  std::vector<double> out = {num1, num2, num3, remain};
  return out;
}
std::vector<double> randArmy3(std::random_device& rd)
{
   double remain = 1;
  std::mt19937 gen(rd());

  std::uniform_int_distribution<> dis(18, 48);
  std::uniform_int_distribution<> dis2(27, 72);

  std::binomial_distribution<int> distribution(1000,dis(gen)/100.0);
  double num1 = distribution(gen)/1000.0;
  remain -= num1;
  std::binomial_distribution<int> distribution2((int) (remain * 1000),dis2(gen)/100.0);
  double num2 = distribution2(gen)/1000.0;
  remain -= num2;

  std::vector<double> out = {num1, num2, remain};
  return out;
}
std::vector<double> randArmy2(std::random_device& rd)
{
  double remain = 1;
  std::mt19937 gen(rd());

  std::uniform_int_distribution<> dis(30, 70);

  std::binomial_distribution<int> distribution(1000,dis(gen)/100.0);
  double num1 = distribution(gen)/1000.0;
  remain -= num1;

  std::vector<double> out = {num1, remain};
  return out;
}