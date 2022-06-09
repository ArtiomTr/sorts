#include "heap.h"

#include <iostream>

#include "sort_meters.h"

void siftDown(std::vector<int> &list, int start, int end,
        blab::quantity_meter &assignments, blab::quantity_meter &comparisons) {
    int root = start;

    int child;

    while((child = 2 * root + 1) <= end) {
        int swap = root;

        if(++comparisons, list[swap] < list[child]) {
            swap = child;
        }

        if (child + 1 <= end && (++comparisons, list[swap] < list[child + 1])) {
            swap = child + 1;
        }

        if(swap == root) {
            return;
        } else {
            std::swap(list[root], list[swap]);
            assignments += 3;
            root = swap;
        }
    }
}

void heapify(std::vector<int> &list, blab::quantity_meter &assignments, blab::quantity_meter &comparisons) {
    int start = (list.size() - 1) / 2;

    while(start >= 0) {
        siftDown(list, start, list.size() - 1, assignments, comparisons);

        --start;
    }
}

std::vector<int> heap_sort(std::vector<int> list) {
    auto[assignments, comparisons, time] = get_sort_meters();

    time.start();

    heapify(list, assignments, comparisons);

    int end = list.size() - 1;

    while(end > 0) {
        std::swap(list[end], list[0]);
        assignments += 3;

        --end;

        siftDown(list, 0, end, assignments, comparisons);
    }

    time.stop();

    return list;
}

