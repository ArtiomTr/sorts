#ifndef PRAT6_COCKTAIL_SHAKER_H
#define PRAT6_COCKTAIL_SHAKER_H

#include <vector>

// Simplest implementation of cocktail shaker sort. Taken from https://en.wikipedia.org/wiki/Cocktail_shaker_sort
std::vector<int> cocktail_shaker_sort(std::vector<int> input);
// Cocktail shaker sort, with index caches. Taken from https://en.wikipedia.org/wiki/Cocktail_shaker_sort
std::vector<int> cocktail_shaker_sort_cached_indexes(std::vector<int> input);
// Same as cocktail_shaker_sort_cached_indexes, improved with assignment optimizations (rotations).
std::vector<int> cocktail_shaker_sort_rotations(std::vector<int> input);

#endif //PRAT6_COCKTAIL_SHAKER_H
