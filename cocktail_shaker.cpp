#include "cocktail_shaker.h"

#include "sort_meters.h"

std::vector<int> cocktail_shaker_sort(std::vector<int> input) {
    auto[assignments, comparisons, time] = get_sort_meters();

    time.start();

    bool swapped;
    do {
        swapped = false;
        for (std::size_t i = 0; i < input.size() - 1; ++i) {
            if (++comparisons, input[i] > input[i + 1]) {
                std::swap(input[i], input[i + 1]);
                assignments += 3;
                swapped = true;
            }
        }

        if (!swapped) {
            break;
        }

        swapped = false;

        for (std::size_t i = input.size() - 1; i > 0; --i) {
            if (++comparisons, input[i - 1] > input[i]) {
                std::swap(input[i - 1], input[i]);
                assignments += 3;
                swapped = true;
            }
        }
    } while (swapped);

    time.stop();

    return input;
}

std::vector<int> cocktail_shaker_sort_cached_indexes(std::vector<int> input) {
    auto[assignments, comparisons, time] = get_sort_meters();

    time.start();

    std::size_t begin_index = 0,
            end_index = input.size() - 2;

    while (begin_index <= end_index) {
        std::size_t new_begin_index = end_index,
                new_end_index = begin_index;

        for (std::size_t i = begin_index; i <= end_index; ++i) {
            if (++comparisons, input[i] > input[i + 1]) {
                std::swap(input[i], input[i + 1]);
                assignments += 3;
                new_end_index = i;
            }
        }

        if(new_end_index == 0) {
            end_index = 0;
        } else {
            end_index = new_end_index - 1;
        }

        for (std::size_t i = end_index + 1; i > begin_index; --i) {
            if (++comparisons, input[i - 1] > input[i]) {
                std::swap(input[i - 1], input[i]);
                assignments += 3;
                new_begin_index = i - 1;
            }
        }
        begin_index = new_begin_index + 1;
    };

    time.stop();

    return input;
}

// Implementation of cocktail shaker sort. Based on implementation from
//      https://en.wikipedia.org/wiki/Cocktail_shaker_sort. Improved with assignment optimizations (rotations).
std::vector<int> cocktail_shaker_sort_rotations(std::vector<int> list) {
    auto[assignments, comparisons, time] = get_sort_meters();

    time.start();

    // First element, waiting for check.
    int begin_index = 0;
    // Last element, waiting for check. (-1 because elements are being checked using "forwards" strategy, where element
    //                                    i is always compared to the element i + 1).
    int end_index = (int) list.size() - 2;

    while (begin_index <= end_index) {
        // New calculated array bounds, that are unsorted.
        int new_begin_index = end_index,
                new_end_index = begin_index;

        // Additional element, for the rotation.
        int rotated_element;
        // Flag, indicating, if element is currently in rotation or not.
        bool in_rotation = false;

        // Index of last element, that was checked.
        int i;
        // Running "bubble pass" (iteration from bubble sort).
        for (i = begin_index; i <= end_index; ++i) {
            // Flag, indicating if element is not on the right place.
            bool should_move;

            // Compare with rotated_element, if currently is in rotation.
            if (in_rotation) {
                should_move = rotated_element > list[i + 1];
                ++comparisons;
            } else {
                should_move = list[i] > list[i + 1];
                ++comparisons;
            }

            // Element is not in right place, should move it.
            if (should_move) {
                // Begin new rotation, if is not currently rotating.
                if (!in_rotation) {
                    rotated_element = list[i];
                    ++assignments;
                    in_rotation = true;
                }

                // Push element towards right position.
                list[i] = list[i + 1];
                ++assignments;

                // Move end index towards beginning. Because new_begin_index is being altered on each push, only last
                //     value will count. This is true because no pushes will be performed on the sorted part of the
                //     array.
                new_end_index = i;
            } else if (in_rotation) {
                // If some element was in rotation, end this rotation.
                list[i] = rotated_element;
                ++assignments;
                in_rotation = false;
            }
        }

        // Finish last rotation.
        if(in_rotation) {
            list[i] = rotated_element;
            ++assignments;
        }

        // Update end index. End index is being moved towards beginning, because last element is already sorted.
        //     (last element will always be sorted, because of "forwards" strategy, where each i'th element is compared
        //      to the i+1 element).
        end_index = new_end_index - 1;

        // Reset flag.
        in_rotation = false;

        // Running "bubble pass", but in reversed order.
        for (i = end_index; i >= begin_index; --i) {
            // Flag, indicating if element is not on the right place.
            bool should_move;

            // Compare with rotated_element, if currently is in rotation.
            if (in_rotation) {
                // Because we're running backwards, rotated element is different, comparing to "direct" bubble pass:
                //     it is not i'th, but i + 1 element. This is need because the index of rotated element, will be
                //     always greater, than current element (backward rotation).
                should_move = list[i] > rotated_element;
                ++comparisons;
            } else {
                should_move = list[i] > list[i + 1];
                ++comparisons;
            }

            if (should_move) {
                // Begin new rotation, if is not currently rotating.
                if (!in_rotation) {
                    rotated_element = list[i + 1];
                    in_rotation = true;
                    ++assignments;
                }

                // Pushing element towards right position.
                list[i + 1] = list[i];
                ++assignments;

                // Moving begin index towards the ending of array.
                new_begin_index = i;
            } else if(in_rotation) {
                // End rotation if it exists.
                list[i + 1] = rotated_element;
                ++assignments;

                in_rotation = false;
            }
        }

        // End last rotation.
        if(in_rotation) {
            list[begin_index] = rotated_element;
            ++assignments;
        }

        // Update begin index to new position. Move it towards ending by 1, because current element was already sorted.
        begin_index = new_begin_index + 1;
    }

    time.stop();

    return list;
}