#include "quick.h"
#include <iostream>

#include "sort_meters.h"

// Insertion sort, for the medians of quick sort. Sort values using iterators (actually, accepts *pointers* to values,
//     and sorts not the array of these pointers, but the values of these pointers).
// Optimized with rotations, instead of swaps.
void sort_medians(std::vector<std::vector<int>::iterator> &items, blab::quantity_meter &assignments, blab::quantity_meter &comparisons) {
    for (size_t i = 1; i < items.size(); ++i) {
        // Boolean, for skipping rotation, if element is already on the right place.
        bool rotating = false;
        // Item, that is being rotated.
        int rotation_item;

        size_t j;
        for (j = i; j > 0; --j) {
            ++comparisons;
            // Comparison that takes into account rotation.
            if (rotating ? *(items[j - 1]) < rotation_item : *(items[j - 1]) < *(items[j])) {
                break;
            }

            // Begin rotation, if element is not in the rotation.
            if (!rotating) {
                rotation_item = *(items[j]);
                ++assignments;
                rotating = true;
            }

            *(items[j]) = *(items[j - 1]);
            ++assignments;
        }

        // End last rotation.
        if (rotating) {
            *(items[j]) = rotation_item;
            ++assignments;
        }
    }
}

// Algorithm, for partitioning array. Uses "median-of-three" as pivot choosing algorithm.
int median_of_three_partition(std::vector<int> &list, int begin_index, int end_index, blab::quantity_meter &assignments, blab::quantity_meter &comparisons) {

    // Calculating pivot index - it will be always element at the middle of array.
    int pivot_index = (begin_index + end_index) / 2;
    // Picking up candidates to be a median.
    std::vector<std::vector<int>::iterator> median_candidates{(list.begin() + begin_index),
                                                              (list.begin() + pivot_index),
                                                              (list.begin() + end_index)};
    // Sorting medians (in-place).
    sort_medians(median_candidates, assignments, comparisons);

    // If array contains 3 elements, no additional actions required - array already sorted after choosing pivot.
    if (end_index - begin_index == 2) {
        return pivot_index;
    }

    // Shrinking interval, because first and last elements are already in right place, after picking pivot.
    ++begin_index;
    --end_index;

    // Optimization: moving endings of intervals only towards their beginnings.
    int begin_index_stop = pivot_index;
    int end_index_stop = pivot_index;

    // Optimization - pivot value never changes, so it could be re-used to optimize assignment count, during rotations.
    int pivot;
    // Flag, indicating, if pivot value was set or not.
    bool is_pivot_set = false;

    while (true) {
        // Shrinking interval from the beginning.
        while (begin_index < begin_index_stop && (++comparisons, list[begin_index] < list[pivot_index]))
            ++begin_index;

        // Shrinking interval from the end_index.
        while (end_index > end_index_stop && (++comparisons, list[end_index] > list[pivot_index]))
            --end_index;

        // Exit condition - begin_index / end_index points intersected.
        if (begin_index >= begin_index_stop && end_index <= end_index_stop) {
            break;
        }

        if (begin_index < begin_index_stop && end_index > end_index_stop) {
            // If two elements are not in right place - swap them.
            std::swap(list[begin_index], list[end_index]);
            ++begin_index;
            --end_index;
            assignments += 3;
        } else if (begin_index < begin_index_stop && begin_index + 1 == pivot_index) {
            // If only left element is not in right place, and it is next to the pivot (no extra space needed) - simply
            //     swap pivot and element.
            std::swap(list[begin_index], list[pivot_index]);
            assignments += 3;
            --pivot_index;

            // We can leave now - no additional swaps required.
            break;
        } else if (end_index > end_index_stop && end_index - 1 == pivot_index) {
            // Same logic as above, for the right part of the array.
            std::swap(list[end_index], list[pivot_index]);
            assignments += 3;
            ++pivot_index;

            break;
        } else if (begin_index != begin_index_stop) {
            // Perform rotation, because extra space required.

            // Setting pivot, if was not set up previously.
            if (!is_pivot_set) {
                is_pivot_set = true;
                pivot = list[pivot_index];
                ++assignments;
            }
            list[pivot_index] = list[begin_index];
            list[begin_index] = list[pivot_index - 1];
            list[pivot_index - 1] = pivot;

            assignments += 3;
            // Pivot moved to the left.
            --pivot_index;
            // Reducing searching interval, to avoid unnecessary comparisons.
            --begin_index_stop;
        } else {
            // Same logic as above, for the right part of the array.

            if (!is_pivot_set) {
                is_pivot_set = true;
                pivot = list[pivot_index];
                ++assignments;
            }
            list[pivot_index] = list[end_index];
            list[end_index] = list[pivot_index + 1];
            list[pivot_index + 1] = pivot;

            assignments += 3;
            ++pivot_index;
            ++end_index_stop;
        }
    }

    return pivot_index;
}

// Quick sort implementation, with "median-of-three" pivot choosing algorithm.
// Private function. Should not be called directly - only from "quickSort" function.
void qsort_median_of_three_inner(std::vector<int> &list, int begin, int end, blab::quantity_meter &assignments, blab::quantity_meter &comparisons) {
    if (end <= begin) { // Already sorted list.
        return;
    } else if (end - begin == 1) { // Two elements to be sorted.
        ++comparisons;
        if (list[begin] > list[end]) {
            std::swap(list[begin], list[end]);
            assignments += 3;
        }

        return;
    }

    // Doing partitioning. Returns index of pivot element.
    int pivot_index = median_of_three_partition(list, begin, end, assignments, comparisons);

    // Sorting left part of the array.
    qsort_median_of_three_inner(list, begin, pivot_index - 1, assignments, comparisons);
    // Sorting right part of the array.
    qsort_median_of_three_inner(list, pivot_index + 1, end, assignments, comparisons);
}

// Quick sort's public API.
std::vector<int> quicksort_median_of_three(std::vector<int> list) {
    auto [assignments, comparisons, time] = get_sort_meters();

    time.start();
    // Calling quick sort with median-of-three pivot choosing algorithm.
    qsort_median_of_three_inner(list, 0, static_cast<int>(list.size()) - 1, assignments, comparisons);

    time.stop();

    return list;
}

void qsort_lomuto_inner(std::vector<int> &list, int begin, int end, blab::quantity_meter &assignments, blab::quantity_meter &comparisons) {
    if(begin >= end || begin < 0) {
        return;
    }

    if(end - begin == 1) {
        ++comparisons;
        if(list[begin] > list[end]) {
            assignments += 3;
            std::swap(list[begin], list[end]);
        }

        return;
    }

    int pivot = begin - 1;
    int pivotElement = list[end];

    for(int j = begin; j < end; ++j) {
        if(list[j] <= pivotElement) {
            ++pivot;

            std::swap(list[pivot], list[j]);
            assignments += 3;
        }
    }

    ++pivot;
    std::swap(list[pivot], list[end]);
    assignments += 3;

    qsort_lomuto_inner(list, begin, pivot - 1, assignments, comparisons);
    qsort_lomuto_inner(list, pivot + 1, end, assignments, comparisons);
}

std::vector<int> quicksort_lomuto(std::vector<int> list) {
    auto [assignments, comparisons, time] = get_sort_meters();

    time.start();

    qsort_lomuto_inner(list, 0, static_cast<int>(list.size()) - 1, assignments, comparisons);

    time.stop();

    return list;
}

void qsort_hoare_inner(std::vector<int> &list, int begin, int end, blab::quantity_meter &assignments, blab::quantity_meter &comparisons) {
    if(begin >= end || begin < 0 || end < 0) {
        return;
    }

    if(end - begin == 1) {
        ++comparisons;
        if(list[begin] > list[end]) {
            assignments += 3;
            std::swap(list[begin], list[end]);
        }

        return;
    }

    int pivotValue = list[end];
    int i = begin - 1;
    int pivot = end + 1;

    while(true) {
        do {
            ++i;
        } while(list[i] < pivotValue);

        do {
            --pivot;
        } while(list[pivot] > pivotValue);

        if(i >= pivotValue) {
            break;
        }

        std::swap(list[i], list[pivot]);
        assignments += 3;
    }

    qsort_hoare_inner(list, begin, pivot, assignments, comparisons);
    qsort_hoare_inner(list, pivot + 1, end, assignments, comparisons);
}

std::vector<int> quicksort_hoare(std::vector<int> list) {
    auto [assignments, comparisons, time] = get_sort_meters();

    time.start();
    qsort_hoare_inner(list, 0, static_cast<int>(list.size()) - 1, assignments, comparisons);

    time.stop();

    return list;
}