#ifndef PRAT6_BUBBLE_H
#define PRAT6_BUBBLE_H

#include <vector>

// No optimizations - most common implementation.
std::vector<int> bubble_sort_naive(std::vector<int> items);
// Does not have fixed count of iterations - iterates, while at least one element is swapped.
std::vector<int> bubble_sort_loose(std::vector<int> items);
// Assumes that last N elements of array are already sorted, after N'th iteration.
std::vector<int> bubble_sort_shrinking_interval(std::vector<int> items);
// Same as shrinking_interval bubble sort, but with one improvement - aborts when no swaps performed on iteration.
std::vector<int> bubble_sort_early(std::vector<int> items);
// Same as bubble_sort_early but with optimized assignments (rotations).
std::vector<int> bubble_sort_rotations(std::vector<int> items);

#endif //PRAT6_BUBBLE_H
