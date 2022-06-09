#include "merge.h"

#include "sort_meters.h"

void merge(int begin,
           int merge_width,
           std::vector<int> &list,
           std::vector<int> &auxiliary,
           blab::quantity_meter &assignments,
           blab::quantity_meter &comparisons) {
    int target_size = merge_width / 2;
    int first_index_begin = begin, second_index_begin = begin + target_size,
            first_index_end = std::min(begin + target_size, (int) list.size()),
            second_index_end = std::min(begin + merge_width, (int) list.size());

    int first_index = first_index_begin, second_index = second_index_begin;
    while (true) {
        int current_index = begin + (first_index - first_index_begin) + (second_index - second_index_begin);

        if (first_index < first_index_end && second_index < second_index_end) {
            ++comparisons;
            if (list[first_index] < list[second_index]) {
                auxiliary[current_index] = list[first_index];
                ++first_index;
            } else {
                auxiliary[current_index] = list[second_index];
                ++second_index;
            }
            ++assignments;
        } else if (first_index < first_index_end) {
            auxiliary[current_index] = list[first_index];
            ++first_index;
            ++assignments;
        } else if (second_index < second_index_end) {
            auxiliary[current_index] = list[second_index];
            ++second_index;
            ++assignments;
        } else {
            break;
        }
    }
}

std::vector<int> merge_sort(std::vector<int> list) {
    auto[assignments, comparisons, time] = get_sort_meters();

    time.start();

    std::vector<int> list2{};
    list2.resize(list.size());

    std::vector<int> &target = list, &auxiliary = list2;

    int merge_size = 1;
    while (merge_size < list.size()) {
        merge_size *= 2;
        for (int iteration = 0; iteration <= list.size() / merge_size; ++iteration) {
            merge(iteration * merge_size, merge_size, target, auxiliary, assignments, comparisons);
        }
        std::swap(target, auxiliary);
    }

    if(target != list) {
        for(int i = 0; i < target.size(); ++i) {
            list[i] = target[i];
            ++assignments;
        }
    }

    time.stop();

    return target;
}