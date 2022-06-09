#ifndef PRAT6_INSERTION_H
#define PRAT6_INSERTION_H

#include <vector>

// Implementation of insertion sort. Taken from https://en.wikipedia.org/wiki/Insertion_sort
std::vector<int> insertion_sort(std::vector<int> items);
// Slightly optimized insertion sort, reduced swap count. Taken from https://en.wikipedia.org/wiki/Insertion_sort
std::vector<int> insertion_sort_optimized(std::vector<int> items);
// Insertion sort with binary search.
std::vector<int> turbo_insertion(std::vector<int> items);

#endif //PRAT6_INSERTION_H
