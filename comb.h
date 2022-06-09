#ifndef PRAT6_COMB_H
#define PRAT6_COMB_H

#include <vector>

// Comb sort implementation, with shrink factor 1.3. Taken from https://en.wikipedia.org/wiki/Comb_sort
std::vector<int> comb_sort_13(std::vector<int> items);
// Comb sort implementation, shrink factor 2.
std::vector<int> comb_sort_2(std::vector<int> items);
// Comb sort implementation, shrink factor 1.5.
std::vector<int> comb_sort_15(std::vector<int> items);

#endif //PRAT6_COMB_H
