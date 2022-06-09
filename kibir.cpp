//
// Created by artio on 13.04.2022.
//

#include "kibir.h"
#include "sort_meters.h"

#include <stdexcept>
#include <iostream>
#include <cmath>

struct Interval {
    bool reorder;
    std::size_t begin;
    std::size_t end;
};

inline int get_normal_index(const std::vector<Interval> &intervals, int pos) {
    int acc = 0;
    for (Interval interval: intervals) {
        int intervalWidth = interval.end - interval.begin + 1;
        if (acc + intervalWidth > pos) {
            return interval.begin + (pos - acc);
        }

        acc += intervalWidth;
    }

//    std::cout << intervals << "\n" << pos << std::endl;
    throw std::logic_error("f*ck");
}

int binary_search_with_gap(int item,
                           int &initialMin,
                           int initialMax,
                           const std::vector<int> &list,
                           const std::vector<Interval> &intervals,
                           int skipIndex,
                           blab::quantity_meter &comparisons) {
    int min = initialMin;
    int max = initialMax;

//    std::cout << list << "\n" << skipIndex << "\n" << intervals << std::endl;
    while (max - min - (max >= skipIndex && min <= skipIndex) >= 0) {
        int middle = (max - (max >= skipIndex && min <= skipIndex) + min) / 2;

        if (middle >= skipIndex && max >= skipIndex && min <= skipIndex) {
            ++middle;
        }

//        std::cout << min << " " << middle << " " << max << std::endl;

        if(middle > max || middle < min) {
            break;
        }

        int normalIndex = get_normal_index(intervals, middle);

        ++comparisons;
        if (item > list[normalIndex]) {
            min = middle + 1;
        } else {
            max = middle - 1;
        }
    }
    int currentIndex = std::max(min, max);

    if(currentIndex > skipIndex) {
        --currentIndex;
    }

    initialMin = currentIndex + 1;

    return currentIndex;
}

void bis_pass(std::vector<int> &items, std::vector<Interval> &current_list, const std::vector<Interval> &insert,
              blab::quantity_meter &assignments, blab::quantity_meter &comparisons) {
    int initial_min = 0, initial_max, list_size = 0;

    for(Interval interval : current_list) {
        list_size += static_cast<int>(interval.end - interval.begin + 1);
    }

    initial_max = list_size;

    for(Interval insert_interval : insert) {
        Interval new_interval{false, insert_interval.begin, insert_interval.begin};
        Interval *inserted_interval = nullptr;
        int index = 0;
        for(int i = 0; i < current_list.size(); ++i) {
            if(insert_interval.end < current_list[i].begin) {
                current_list.insert(current_list.begin() + i, new_interval);
                inserted_interval = &current_list[i];
                break;
            } else {
                index += static_cast<int>(current_list[i].end - current_list[i].begin + 1);
            }
        }

        if(inserted_interval == nullptr) {
            current_list.push_back(new_interval);
            inserted_interval = &current_list.back();
        }

        for(std::size_t i = insert_interval.begin; i <= insert_interval.end; ++i) {
            inserted_interval->end = i;

            int position = binary_search_with_gap(items[i], initial_min, initial_max, items, current_list, index, comparisons);

            if(position != index) {
                int temp = items[i];
                ++assignments;

                if(position > index) {
                    for(std::size_t j = index; j < position; ++j) {
                        int a_normal_pos = get_normal_index(current_list, j);
                        int b_normal_pos = get_normal_index(current_list, j + 1);
                        items[a_normal_pos] = items[b_normal_pos];
                        ++assignments;
                    }
                } else {
                    for(std::size_t j = index; j > position; --j) {
                        int a_normal_pos = get_normal_index(current_list, j);
                        int b_normal_pos = get_normal_index(current_list, j - 1);
                        items[a_normal_pos] = items[b_normal_pos];
                        ++assignments;
                    }
                }

                items[get_normal_index(current_list, position)] = temp;
                ++assignments;
            }

            ++index;
            ++initial_max;
        }
    }
}

std::vector<int> kibir_sort(std::vector<int> items) {
    std::vector<int> itemCopy = items;
    auto[assignments,comparisons,time] = get_sort_meters();

    time.start();

    std::vector<Interval> sequences;

    sequences.push_back({false, 0, 0});
    for (std::size_t i = 0; i < items.size() - 1; ++i) {
//        ++comparisons;
        if (items[i] > items[i + 1]) {
            Interval &last = sequences.back();
            last.end = i;

            if(last.end - last.begin == 0) {
                if(sequences.size() > 1 && sequences[sequences.size() - 2].reorder) {
                    sequences[sequences.size() - 2].end = i;
                    last.begin = i + 1;
                } else {
                    last.reorder = true;
                    sequences.push_back({false, i + 1, 0});
                }
            } else {
                sequences.push_back({false, i + 1, 0});
            }
        }
    }

    Interval &last = sequences.back();
    last.end = items.size() - 1;

    if(last.end - last.begin == 0 && sequences.size() > 1 && sequences[sequences.size() - 2].reorder) {
        sequences[sequences.size() - 2].end = items.size() - 1;
        sequences.erase(sequences.end() - 1);
    }

    std::cout << sequences.size() << std::endl;

    for(Interval interval : sequences) {
        if(interval.reorder) {
            for(std::size_t i = interval.begin, j = interval.end; i < j; ++i, --j) {
                std::swap(items[i], items[j]);
                assignments += 3;
            }
        }
    }

    std::vector<std::vector<Interval>> intervals;

    for(Interval interval : sequences) {
        intervals.push_back({interval});
    }

    int gap = static_cast<int>(std::pow(2, std::floor(std::log2(intervals.size())) + 1));

    while(gap > 1) {
        gap /= 2;

        if(gap < 1) {
            gap = 1;
        }

        for(int offset = 0; offset < gap && offset + gap < intervals.size(); ++offset) {
            bis_pass(items, intervals[offset], intervals[offset + gap], assignments, comparisons);
        }
    }

    time.stop();

    return items;
}