#ifndef PRAT6_QUICK_H
#define PRAT6_QUICK_H

#include <vector>
#include <blab/meter.h>

std::vector<int> quicksort_median_of_three(std::vector<int>);
void qsort_median_of_three_inner(std::vector<int> &list, int begin, int end, blab::quantity_meter &assignments, blab::quantity_meter &comparisons);

std::vector<int> quicksort_lomuto(std::vector<int> list);

std::vector<int> quicksort_hoare(std::vector<int> list);

#endif //PRAT6_QUICK_H
