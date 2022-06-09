#include "insertion.h"

#include "sort_meters.h"

#include <iostream>

std::vector<int> insertion_sort(std::vector<int> items) {
    auto[assignments, comparisons, time] = get_sort_meters();

    time.start();
    for (std::size_t i = 1; i < items.size(); ++i) {
        for (std::size_t j = i; j > 0 && (++comparisons, items[j - 1] > items[j]); --j) {
            std::swap(items[j], items[j - 1]);
            assignments += 3;
        }
    }
    time.stop();

    return items;
}

std::vector<int> insertion_sort_optimized(std::vector<int> items) {
    auto[assignments, comparisons, time] = get_sort_meters();

    time.start();

    // Collecting sorted list on the left side.
    for (std::size_t i = 1; i < items.size(); ++i) {
        // Flag for rotation detection. Required for additional optimization, to avoid rotations, when element is
        //     already in the right place.
        bool rotating = false;
        // Item, that is being rotated.
        int rotation_item;

        // Searching position for the new element.
        std::size_t j;
        for (j = i; j > 0; --j) {
            ++comparisons;
            // If currently rotating item, then compare with it, otherwise with neighbour item.
            if (rotating ? items[j - 1] < rotation_item : items[j - 1] < items[j]) {
                break;
            }

            // Begin rotation, if not rotating already.
            if (!rotating) {
                rotation_item = items[j];
                ++assignments;
                rotating = true;
            }

            // Push items towards right positions.
            items[j] = items[j - 1];
            ++assignments;
        }

        // If some element was in rotation, finish that rotation.
        if (rotating) {
            items[j] = rotation_item;
            ++assignments;
        }
    }

    time.stop();

    return items;
}

template<class T>
std::ostream &operator<<(std::ostream &output, const std::vector<T> &list) {
    for (T item: list) {
        output << item << " ";
    }

    return output;
}
std::vector<int> turbo_insertion(std::vector<int> items) {
    auto[assignments, comparisons, time] = get_sort_meters();

    time.start();

    for (int i = 1; i < items.size(); ++i) {
        int min = 0, max = i - 1;

        while(min <= max) {
            int middle = (min + max) / 2;
            ++comparisons;
            if(items[middle] > items[i]) {
                max = middle - 1;
            } else {
                min = middle + 1;
            }
        }

        int newPosition = min > max ? min : max;

        if(i != newPosition) {
            int item = items[i];
            ++assignments;
            for(int j = i; j > newPosition; --j) {
                items[j] = items[j - 1];
                ++assignments;
            }
            items[newPosition] = item;
            ++assignments;
        }
    }

    time.stop();

    std::cout << items << std::endl;

    return items;
}