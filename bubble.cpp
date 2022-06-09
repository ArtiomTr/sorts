#include "sort_meters.h"

#include <vector>

std::vector<int> bubble_sort_naive(std::vector<int> items) {
    auto[assignments, comparisons, time] = get_sort_meters();

    time.start();

    for (int i = 0; i < items.size(); ++i) {
        for (int j = 0; j < items.size() - 1; ++j) {
            if (++comparisons, items[j + 1] < items[j]) {
                std::swap(items[j + 1], items[j]);
                assignments += 3;
            }
        }
    }

    time.stop();

    return items;
}

std::vector<int> bubble_sort_loose(std::vector<int> items) {
    auto[assignments, comparisons, time] = get_sort_meters();

    time.start();

    bool swapped;
    do {
        swapped = false;
        for (int i = 1; i < items.size(); ++i) {
            if (++comparisons, items[i - 1] > items[i]) {
                std::swap(items[i - 1], items[i]);
                assignments += 3;
                swapped = true;
            }
        }
    } while (swapped);

    time.stop();

    return items;
}

std::vector<int> bubble_sort_shrinking_interval(std::vector<int> items) {
    auto[assignments, comparisons, time] = get_sort_meters();

    time.start();

    for (int i = 1; i < items.size(); ++i) {
        for (int j = 0; j < items.size() - i; ++j) {
            if (++comparisons, items[j + 1] < items[j]) {
                std::swap(items[j + 1], items[j]);
                assignments += 3;
            }
        }
    }

    time.stop();

    return items;
}

std::vector<int> bubble_sort_early(std::vector<int> items) {
    auto[assignments, comparisons, time] = get_sort_meters();

    time.start();

    bool swapped;
    for (int i = 1; i < items.size(); ++i) {
        swapped = false;
        for (int j = 0; j < items.size() - i; ++j) {
            if (++comparisons, items[j] > items[j + 1]) {
                std::swap(items[j], items[j + 1]);
                assignments += 3;
                swapped = true;
            }
        }

        if (!swapped) {
            break;
        }
    }

    time.stop();

    return items;
}

std::vector<int> bubble_sort_rotations(std::vector<int> list) {
    auto[assignments, comparisons, time] = get_sort_meters();

    time.start();

    // Flag, indicating if array is already fully sorted.
    bool swapped;
    for (int i = 1; i < list.size(); ++i) {
        swapped = false;

        // Additional element, for the rotation.
        int rotated_element;
        // Flag, indicating, if element is currently in rotation or not.
        bool in_rotation = false;

        int j;
        for (j = 0; j < list.size() - i; ++j) {
            // Flag, indicating if element is not on the right place.
            bool should_move;

            // If currently rotating some element.
            if (in_rotation) {
                should_move = rotated_element > list[j + 1];
                ++comparisons;
            } else {
                should_move = list[j] > list[j + 1];
                ++comparisons;
            }

            // Element is not in right place, should move it.
            if (should_move) {
                // If currently nothing is rotated.
                if (!in_rotation) {
                    // Begin rotation.
                    rotated_element = list[j];
                    ++assignments;

                    in_rotation = true;
                }

                // Move element.
                list[j] = list[j + 1];
                ++assignments;

                swapped = true;
            } else if (in_rotation) {
                // If currently rotating some element, then end rotation.
                list[j] = rotated_element;
                ++assignments;
                in_rotation = false;
            }
        }

        // Finalize last rotation.
        if (in_rotation) {
            list[j] = rotated_element;
            ++assignments;
        }

        if (!swapped) {
            break;
        }
    }

    time.stop();

    return list;
}
