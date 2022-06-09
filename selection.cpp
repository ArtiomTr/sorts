#include "selection.h"

#include "sort_meters.h"

std::vector<int> selection_sort(std::vector<int> items) {
    auto[assignments, comparisons, time] = get_sort_meters();

    time.start();

    for(std::size_t i = 0; i < items.size(); ++i) {
        std::size_t min_index = i;
        for(std::size_t j = i + 1; j < items.size(); ++j) {
            if(++comparisons, items[j] < items[min_index]) {
                min_index = j;
            }
        }

        if(min_index != i) {
            std::swap(items[i], items[min_index]);
            assignments += 3;
        }
    }

    time.stop();

    return items;
}
