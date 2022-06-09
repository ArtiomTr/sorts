#include "radix.h"
#include "sort_meters.h"

#include <vector>
#include <cmath>

std::vector<int> radixSortLSD(std::vector<int> &list, int base) {
    auto[assignments, comparisons, time] = get_sort_meters();

    time.start();

    int maxValue = std::numeric_limits<int>::min();

    for(int item : list) {
        ++comparisons;
        if(item > maxValue) {
            maxValue = item;
            ++assignments;
        }
    }

    int digitCount = std::ceil(std::log(maxValue) / std::log(base));

    for(int digit = 0; digit <= digitCount; ++digit) {
        std::vector<int> histogram[base];

        int digitMask = std::pow(base, digit);

        for(int item : list) {
            histogram[(item / digitMask) % base].push_back(item);
            ++assignments;
        }

        int lastIndex = 0;
        for(int i = 0; i < base; ++i) {
            for(int j = 0; j < histogram[i].size(); ++j) {
                list[lastIndex + j] = histogram[i][j];
                ++assignments;
            }
            lastIndex += histogram[i].size();
        }
    }

    time.stop();

    return list;
}

std::vector<int> radixSortLSD10(std::vector<int> list) {
    return radixSortLSD(list, 10);
}

std::vector<int> radixSortLSD5(std::vector<int> list) {
    return radixSortLSD(list, 5);
}

std::vector<int> radixSortLSD2(std::vector<int> list) {
    return radixSortLSD(list, 2);
}

std::vector<int> radixSortMSD(std::vector<int> &list, int base) {
    auto[assignments, comparisons, time] = get_sort_meters();

    time.start();

    int maxValue = std::numeric_limits<int>::min();

    for(int item : list) {
        ++comparisons;
        if(item > maxValue) {
            maxValue = item;
            ++assignments;
        }
    }

    int digitCount = std::ceil(std::log(maxValue) / std::log(base));

    for(int digit = 0; digit <= digitCount; ++digit) {
        std::vector<int> histogram[base];

        int digitMask = std::pow(base, digitCount - digit);

        for(int item : list) {
            histogram[(item / digitMask) % base].push_back(item);
            ++assignments;
        }

        int lastIndex = 0;
        for(int i = 0; i < base; ++i) {
            for(int j = 0; j < histogram[i].size(); ++j) {
                list[lastIndex + j] = histogram[i][j];
                ++assignments;
            }
            lastIndex += histogram[i].size();
        }
    }

    time.stop();

    return list;
}

std::vector<int> radixSortMSD10(std::vector<int> list) {
    return radixSortLSD(list, 10);
}

std::vector<int> radixSortMSD5(std::vector<int> list) {
    return radixSortLSD(list, 5);
}

std::vector<int> radixSortMSD2(std::vector<int> list) {
    return radixSortLSD(list, 2);
}