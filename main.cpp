#include <iostream>
#include <fstream>
#include <random>

// blab library
#include "blab/environment.h"
#include "blab/report.h"
#include "blab/experiment.h"

// sorting algorithms
#include "bubble.h"
#include "cocktail_shaker.h"
#include "comb.h"
#include "selection.h"
#include "insertion.h"
#include "gnome.h"
#include "shell.h"
#include "quick.h"
#include "tree.h"
#include "merge.h"
#include "heap.h"
#include "radix.h"
#include "kibir.h"

#include "sort_meters.h"

template<class T>
std::ostream &operator<<(std::ostream &output, const std::vector<T> &list) {
    output << '{';
    for (T item: list) {
        output << item << ", ";
    }
    output << '}';

    return output;
}

bool is_array_sorted(std::vector<int> arr) {
    for (int i = 0; i < arr.size() - 1; ++i) {
        if (arr[i] > arr[i + 1]) {
            std::cout << arr << std::endl;
            return false;
        }
    }

    return true;
}

template<std::size_t list_size>
std::vector<int> generate_random_list() {
    std::vector<int> list{};
    list.resize(list_size);

    for (int i = 0; i < list.size(); ++i) {
        list[i] = i + 1;
    }

    std::random_device r;
    std::mt19937 random_generator(r());
    std::shuffle(list.begin(), list.end(), random_generator);

    return list;
}

template<std::size_t list_size>
std::vector<int> generate_sorted_list() {
    std::vector<int> list{};
    list.resize(list_size);

    for (int i = 0; i < list.size(); ++i) {
        list[i] = i + 1;
    }

    return list;
}

template<std::size_t list_size>
std::vector<int> generate_reverse_sorted_list() {
    std::vector<int> list{};
    list.resize(list_size);

    for (int i = 0; i < list.size(); ++i) {
        list[i] = (int) list.size() - i;
    }

    return list;
}

template<std::size_t list_size, std::size_t swap_count>
std::vector<int> generate_almost_sorted_list() {
    std::vector<int> list{};
    list.resize(list_size);

    for (std::size_t i = 0; i < list.size(); ++i) {
        list[i] = static_cast<int>(i + 1);
    }

    std::random_device r;
    std::mt19937 random_generator(r());
    std::uniform_int_distribution<std::size_t> distribution(0, list_size - 1);

    for(std::size_t i = 0; i < swap_count; ++i) {
        std::size_t a = distribution(random_generator), b = distribution(random_generator);

        std::swap(list[a], list[b]);
    }

    return list;
}

std::vector<int> quick_bubble(std::vector<int> list) {
    auto[assignments, comparisons, time] = get_sort_meters();

    time.start();

    if(list.size() <= 50) {
        qsort_median_of_three_inner(list, 0, list.size() - 1, assignments, comparisons);

        time.stop();
        return list;
    }

    bool swapped = false;

    // Additional element, for the rotation.
    int rotated_element;
    // Flag, indicating, if element is currently in rotation or not.
    bool in_rotation = false;

    int j;
    for (j = 0; j < list.size() - 1; ++j) {
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

    if(swapped) {
        qsort_median_of_three_inner(list, 0, list.size() - 1, assignments, comparisons);
    }

    time.stop();

    return list;
}

void runAllExperiments() {
    std::array<std::pair<std::string, std::function<std::vector<int>(std::vector<int>)>>, 38> subjects =
            {{
                     {"Bubble sort", bubble_sort_naive},
                     {"Bubble sort (loose)", bubble_sort_loose},
                     {"Bubble sort (shrinking interval)", bubble_sort_shrinking_interval},
                     {"Bubble sort (shrinking interval & early abort)", bubble_sort_early},
                     {"Bubble sort (rotations)", bubble_sort_rotations},
                     {"Cocktail shaker sort", cocktail_shaker_sort},
                     {"Cocktail shaker sort (with index cache)", cocktail_shaker_sort_cached_indexes},
                     {"Cocktail shaker sort (rotations)", cocktail_shaker_sort_rotations},
                     {"Comb sort (shrink factor 1.3)", comb_sort_13},
                     {"Comb sort (shrink factor 1.5)", comb_sort_15},
                     {"Comb sort (shrink factor 2)", comb_sort_2},
                     {"Selection sort", selection_sort},
                     {"Insertion sort", insertion_sort},
                     {"Insertion sort (optimized assignments)", insertion_sort_optimized},
                     {"Gnome sort", gnome_sort},
                     {"Shell sort (Shell, 1959)", shell_sort_original},
                     {"Shell sort (Frank & Lazarus, 1960)", shell_sort_frank_and_lazarus},
                     {"Shell sort (Hibbard, 1963)", shell_sort_hibbard},
                     {"Shell sort (Papernov & Stasevich, 1965)", shell_sort_papernov_stasevich},
                     {"Shell sort (Pratt, 1971)", shell_sort_pratt},
                     {"Shell sort (Knuth, 1973)", shell_sort_knuth},
                     {"Shell sort (Incerpi & Sedgewick, 1985)", shell_sort_incerpi_sedgewick},
                     {"Shell sort (Sedgewick, 1982)", shell_sort_sedgewick_1982},
                     {"Shell sort (Sedgewick, 1986)", shell_sort_sedgewick_1986},
                     {"Shell sort (Gonnet & Baeza-Yates, 1991)", shell_sort_gonnet_baeza_yates},
                     {"Shell sort (Tokuda, 1992)", shell_sort_tokuda},
                     {"Shell sort (Ciura, 2001)", shell_sort_ciura},
                     {"Quick sort (Median of three)", quicksort_median_of_three},
                     {"Tree sort (Unbalanced)", tree_sort_unbalanced},
                     {"Merge sort", merge_sort},
                     {"Heap sort", heap_sort},
                     {"Radix sort (base 10, lsd)", radixSortLSD10},
                     {"Radix sort (base 5, lsd)", radixSortLSD5},
                     {"Radix sort (base 2, lsd)", radixSortLSD2},
                     {"Radix sort (base 10, msd)", radixSortMSD10},
                     {"Radix sort (base 5, msd)", radixSortMSD5},
                     {"Radix sort (base 2, msd)", radixSortMSD2},
                     {"Kibir sort", kibir_sort},
             }};

    using list_generator = std::function<std::vector<int>(void)>;

    std::array<std::pair<std::string, list_generator>, 10> experiments =
            {{
                     {"Random distribution dataset, 10 elements", generate_random_list<10>},
                     {"Random distribution dataset, 100 elements", generate_random_list<100>},
                     {"Random distribution dataset, 1000 elements", generate_random_list<1000>},
                     {"Sorted dataset, 10 elements", generate_sorted_list<10>},
                     {"Sorted dataset, 100 elements", generate_sorted_list<100>},
                     {"Sorted dataset, 1000 elements", generate_sorted_list<1000>},
                     {"Reverse sorted dataset, 10 elements", generate_reverse_sorted_list<10>},
                     {"Reverse sorted dataset, 100 elements", generate_reverse_sorted_list<100>},
                     {"Reverse sorted dataset, 1000 elements", generate_reverse_sorted_list<1000>},
                     {"Almost sorted array, 100 elements", generate_almost_sorted_list<100, 5>}
             }};

    for (const auto&[exp_name, generator]: experiments) {
        blab::experiment<std::vector<int>, std::vector<int>> exp(is_array_sorted, generator());

        for (const auto&[sort_name, sort_fn]: subjects) {
            exp.add_subject(sort_name, sort_fn);
        }

        std::cout << exp_name << '\n';
        std::cout << exp.perform() << '\n';
    }
}
std::vector<int> turbo_insertion_t(std::vector<int> items) {
    auto[assignments, comparisons, time] = get_sort_meters();

    time.start();

    for (int i = 5; i < items.size(); ++i) {
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

    return items;
}

void runDevExperiments() {
    blab::experiment<std::vector<int>, std::vector<int>> exp(is_array_sorted, { 5, 4, 8, 9, 2, 1, 3, 7, 6 });

    exp.add_subject("Quick sort (lomuto)", quicksort_lomuto);

    std::cout << exp.perform() << '\n';
}

void runResearchExperiments() {
    std::array<std::pair<std::string, std::function<std::vector<int>(std::vector<int>)>>, 12> subjects =
            {{
//                     {"Bubble sort", bubble_sort_naive},
//                     {"Bubble sort (loose)", bubble_sort_loose},
//                     {"Bubble sort (shrinking interval)", bubble_sort_shrinking_interval},
//                     {"Bubble sort (shrinking interval & early abort)", bubble_sort_early},
//                     {"Bubble sort (rotations)", bubble_sort_rotations},
//                     {"Cocktail shaker sort", cocktail_shaker_sort},
//                     {"Cocktail shaker sort (with index cache)", cocktail_shaker_sort_cached_indexes},
//                     {"Cocktail shaker sort (rotations)", cocktail_shaker_sort_rotations},
//                     {"Comb sort (shrink factor 1.3)", comb_sort_13},
//                     {"Comb sort (shrink factor 1.5)", comb_sort_15},
//                     {"Comb sort (shrink factor 2)", comb_sort_2},
//                     {"Selection sort", selection_sort},
//                     {"Insertion sort", insertion_sort},
//                     {"Insertion sort (optimized assignments)", insertion_sort_optimized},
//                     {"Gnome sort", gnome_sort},
                     {"Shell sort (Shell, 1959)", shell_sort_original},
                     {"Shell sort (Frank & Lazarus, 1960)", shell_sort_frank_and_lazarus},
                     {"Shell sort (Hibbard, 1963)", shell_sort_hibbard},
                     {"Shell sort (Papernov & Stasevich, 1965)", shell_sort_papernov_stasevich},
                     {"Shell sort (Pratt, 1971)", shell_sort_pratt},
                     {"Shell sort (Knuth, 1973)", shell_sort_knuth},
                     {"Shell sort (Incerpi & Sedgewick, 1985)", shell_sort_incerpi_sedgewick},
                     {"Shell sort (Sedgewick, 1982)", shell_sort_sedgewick_1982},
                     {"Shell sort (Sedgewick, 1986)", shell_sort_sedgewick_1986},
                     {"Shell sort (Gonnet & Baeza-Yates, 1991)", shell_sort_gonnet_baeza_yates},
                     {"Shell sort (Tokuda, 1992)", shell_sort_tokuda},
                     {"Shell sort (Ciura, 2001)", shell_sort_ciura},
//                     {"Quick sort (Median of three)", quicksort_median_of_three},
//                     {"Turbo insertion", turbo_insertion}
//                     {"Tree sort (Unbalanced)", tree_sort_unbalanced},
//                     {"Merge sort", merge_sort},
//                     {"Heap sort", heap_sort}
             }};

    using list_generator = std::function<std::vector<int>(void)>;

    std::array<std::pair<std::string, list_generator>, 9> experiments =
            {{
                     {"Random distribution dataset, 10 elements", generate_random_list<10>},
                     {"Random distribution dataset, 100 elements", generate_random_list<100>},
                     {"Random distribution dataset, 1000 elements", generate_random_list<1000>},
                     {"Sorted dataset, 10 elements", generate_sorted_list<10>},
                     {"Sorted dataset, 100 elements", generate_sorted_list<100>},
                     {"Sorted dataset, 1000 elements", generate_sorted_list<1000>},
                     {"Reverse sorted dataset, 10 elements", generate_reverse_sorted_list<10>},
                     {"Reverse sorted dataset, 100 elements", generate_reverse_sorted_list<100>},
                     {"Reverse sorted dataset, 1000 elements", generate_reverse_sorted_list<1000>},
             }};

    for (const auto&[exp_name, generator]: experiments) {
        blab::experiment<std::vector<int>, std::vector<int>> exp(is_array_sorted, generator());

        for (const auto&[sort_name, sort_fn]: subjects) {
            exp.add_subject(sort_name, sort_fn);
        }

        std::cout << exp_name << '\n';
        std::cout << exp.perform().to_csv() << '\n';
    }
}

int main() {

    runAllExperiments();
    runDevExperiments();
    runResearchExperiments();

    return 0;
}
