#include "gnome.h"
#include "sort_meters.h"

std::vector<int> gnome_sort(std::vector<int> items) {
    auto[assignments, comparisons, time] = get_sort_meters();

    time.start();

    std::size_t position = 0;
    while(position < items.size()) {
        if(position == 0 || (++comparisons, items[position] >= items[position - 1])) {
            position = position + 1;
        } else {
            std::swap(items[position], items[position - 1]);
            assignments += 3;
            position = position - 1;
        }
    }

    time.stop();

    return items;
}
