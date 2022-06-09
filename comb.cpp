#include "comb.h"

#include "sort_meters.h"

std::vector<int> comb_sort(std::vector<int> list, double shrink_factor) {
    auto[assignments, comparisons, time] = get_sort_meters();

    time.start();

    // Comb sort gap.
    std::size_t gap = list.size();
    // Flag, which indicates if array is already sorted or not.
    bool sorted = false;

    while (!sorted) {
        // Shrinking gap.
        gap = static_cast<std::size_t>(static_cast<double>(gap) / shrink_factor);

        // Normalizing gap.
        if (gap <= 1) {
            gap = 1;
            // If gap is 1, try to assume that array is sorted. If no swaps will be performed, the loop will end.
            sorted = true;
        }

        // Running bubble pass for sequence with offset i.
        for (std::size_t i = 0; i < gap; ++i) {
            // Flag, indicating if currently some element is in rotation.
            bool inRotation = false;
            // Element that is rotating.
            int rotationItem;

            std::size_t j;
            // Running bubble pass for elements, separated by given gap.
            for (j = i; j + gap < list.size(); j += gap) {
                // Flag, indicating if element should be pushed towards right direction.
                bool shouldMove;

                if (inRotation) {
                    shouldMove = rotationItem > list[j + gap];
                } else {
                    shouldMove = list[j] > list[j + gap];
                }
                ++comparisons;

                if (shouldMove) {
                    // Running new rotation, if currently does not exist one.
                    if (!inRotation) {
                        rotationItem = list[j];
                        ++assignments;

                        inRotation = true;
                    }

                    // Pushing element towards suitable position.
                    list[j] = list[j + gap];
                    ++assignments;

                    // Resetting "sorted" flag - at least one swap performed, array is not sorted yet.
                    sorted = false;
                } else if (inRotation) {
                    // Ending rotation, if there was one.
                    list[j] = rotationItem;
                    ++assignments;
                    inRotation = false;
                }
            }

            // Finalizing last rotation.
            if (inRotation) {
                list[j] = rotationItem;
                ++assignments;
            }
        }
    }

    time.stop();

    return list;
}

std::vector<int> comb_sort_13(std::vector<int> items) {
    return comb_sort(std::move(items), 1.3);
}

std::vector<int> comb_sort_2(std::vector<int> items) {
    return comb_sort(std::move(items), 2);
}

std::vector<int> comb_sort_15(std::vector<int> items) {
    return comb_sort(std::move(items), 1.5);
}