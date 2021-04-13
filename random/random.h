#ifndef RANDOM_H
#define RANDOM_H

// fist get 3 uniform integer distribution to determine the success rate passing into the binomial distribution
// each binomial distribution's succcess rate should be between 0.15 to 0.35 of 1
std::vector<double> randArmy4(std::random_device &rd);
std::vector<double> randArmy3(std::random_device &rd);
std::vector<double> randArmy2(std::random_device &rd);
#endif