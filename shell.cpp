#include "shell.h"

#include <functional>
#include <cmath>
#include <iostream>
#include <set>

#include "sort_meters.h"

std::vector<int> shell_sort(
        std::vector<int> items,
        const std::function<std::size_t(std::size_t k, std::size_t n)> &gap_generator) {
    auto[assignments, comparisons, time] = get_sort_meters();

    time.start();

    std::size_t gap, k = 0;

    do {
        // Calculating new gap.
        gap = gap_generator(k, items.size());
        // Increasing gap index counter.
        ++k;

        // Running insertion sort for each subsequence.
        for (std::size_t offset = 0; offset < gap; ++offset) {
            // Collecting sorted list on the left side.
            for (std::size_t i = offset + gap; i < items.size(); i += gap) {
                // Flag for rotation detection. Required for additional optimization, to avoid rotations, when element is
                //     already in the right place.
                bool isRotating = false;
                // Item, that is being rotated.
                int rotationItem;

                // Searching position for the new element.
                std::size_t j;
                for (j = i; j >= gap; j -= gap) {
                    ++comparisons;
                    // If currently rotating item, then compare with it, otherwise with neighbour item.
                    if (isRotating ? items[j - gap] < rotationItem : items[j - gap] < items[j]) {
                        break;
                    }

                    // Begin rotation, if not rotating already.
                    if (!isRotating) {
                        rotationItem = items[j];
                        ++assignments;
                        isRotating = true;
                    }

                    // Push items towards right positions.
                    items[j] = items[j - gap];
                    ++assignments;
                }

                // If some element was in rotation, finish that rotation.
                if (isRotating) {
                    items[j] = rotationItem;
                    ++assignments;
                }
            }
        }
    } while (gap > 1);

    time.stop();

    return items;
}

void printList(const std::vector<int> &list, const std::set<int> &indexesToHighlight) {
    for (int i = 0; i < list.size(); ++i) {
        if (indexesToHighlight.contains(i)) {
            std::cout << '[' << list[i] << "] ";
        } else {
            std::cout << list[i] << ' ';
        }
    }
    std::cout << std::endl;
}

template<class F, class S>
std::ostream &operator<<(std::ostream &output, const std::pair<F, S> p) {
    output << '{' << p.first << "; " << p.second << '}';

    return output;
}

template<class T>
std::ostream &operator<<(std::ostream &output, const std::vector<T> &list) {
    for (T item: list) {
        output << item << " ";
    }

    return output;
}

void binary_insertion_sequence_sort(const std::vector<std::pair<int, int>> &sequences, std::vector<int> &items) {

}

std::vector<int> shell_sort_old(
        std::vector<int> items,
        const std::function<std::size_t(std::size_t k, std::size_t n)> &gap_generator) {
    auto[assignments, comparisons, time] = get_sort_meters();

    time.start();

    std::vector<std::pair<int, int>> subsequences;

    subsequences.emplace_back(0, 0);

    for (int i = 0; i < items.size(); ++i) {
        ++comparisons;
        if(items[i + 1] < items[i]) {
            subsequences.back().second = i;
            subsequences.emplace_back(i + 1, i + 1);
        }
    }

    subsequences.back().second = items.size() - 1;

    std::cout << subsequences << std::endl;

    std::size_t gap, k = 0;

    do {
        // Calculating new gap.
        gap = gap_generator(k, subsequences.size());
        // Increasing gap index counter.
        ++k;

        // Running insertion sort for each subsequence.
        for (std::size_t offset = 0; offset < gap; ++offset) {
            // Collecting sorted list on the left side.
            for (std::size_t i = offset; i < subsequences.size(); i += gap) {
                int min = 0, max = static_cast<int>(i) - 1;

                while (min <= max) {
                    int middle = (min + max) / 2;
                    ++comparisons;
                    if (items[middle * gap + offset] > items[i * gap + offset]) {
                        max = middle - 1;
                    } else {
                        min = middle + 1;
                    }
                }

                int newPosition = min > max ? min : max;

                if (i > newPosition) {
                    int item = items[i * gap + offset];
                    ++assignments;
                    for (int j = static_cast<int>(i); j > newPosition; --j) {
                        items[j * gap + offset] = items[(j - 1) * gap + offset];
                        ++assignments;
                    }
                    items[newPosition * gap + offset] = item;
                    ++assignments;
                }
            }
        }
    } while (gap > 1);

    time.stop();

    std::cout << items << std::endl;

    return items;
}

std::vector<int> shell_sort_original(std::vector<int> items) {
    return shell_sort(std::move(items), [](std::size_t k, std::size_t n) -> std::size_t {
        std::size_t gap = std::max(n / (std::size_t) std::pow(2, k), (std::size_t) 1);

        return gap;
    });
}

std::vector<int> shell_sort_frank_and_lazarus(std::vector<int> items) {
    return shell_sort(std::move(items), [](std::size_t k, std::size_t n) -> std::size_t {
        std::size_t gap = n / (std::size_t) std::pow(2, k + 1);

        return 2 * gap + 1;
    });
}

std::vector<int> shell_sort_hibbard(std::vector<int> items) {
    std::vector<std::size_t> computed_gaps{};

    std::size_t gap = 0, k = 0;
    while (gap < items.size()) {
        gap = (std::size_t) std::pow(2, ++k) - 1;
        computed_gaps.push_back(gap);
    }

    return shell_sort(std::move(items), [&computed_gaps](std::size_t k, std::size_t n) -> std::size_t {
        return computed_gaps[computed_gaps.size() - k - 1];
    });
}

std::vector<int> shell_sort_papernov_stasevich(std::vector<int> items) {
    std::vector<std::size_t> computed_gaps{1};

    std::size_t gap = 0, k = 0;
    while (gap < items.size()) {
        gap = (std::size_t) std::pow(2, ++k) - 1;
        computed_gaps.push_back(gap);
    }

    return shell_sort(std::move(items), [&computed_gaps](std::size_t k, std::size_t n) -> std::size_t {
        return computed_gaps[computed_gaps.size() - k - 1];
    });
}

// Algorithm taken from https://www.geeksforgeeks.org/p-smooth-numbers-in-given-ranges/
std::size_t max_prime_divisor(std::size_t number) {
    std::size_t mpd = 1;

    if (number == 1) {
        return mpd;
    }

    while (number % 2 == 0) {
        mpd = 2;
        number /= 2;
    }

    std::size_t last_check_value = (std::size_t) std::sqrt(number) + 1;
    for (std::size_t i = 3; i < last_check_value; i += 2) {
        while (number % i == 0) {
            mpd = i;
            number /= i;
        }
    }

    mpd = std::max(mpd, number);

    return mpd;
}

std::vector<int> shell_sort_pratt(std::vector<int> items) {
    std::vector<std::size_t> computed_gaps{1};

    for (std::size_t gap = 2; gap < items.size(); ++gap) {
        if (max_prime_divisor(gap) <= 3) {
            computed_gaps.push_back(gap);
        }
    }

    return shell_sort(std::move(items), [&computed_gaps](std::size_t k, std::size_t n) -> std::size_t {
        return computed_gaps[computed_gaps.size() - k - 1];
    });
}

std::vector<int> shell_sort_knuth(std::vector<int> items) {
    std::vector<std::size_t> computed_gaps{};

    std::size_t gap = 0, k = 0;
    std::size_t max_gap = std::ceil((double) items.size() / 3.0);
    while (gap < items.size()) {
        gap = std::min(((std::size_t) std::pow(3, ++k) - 1) / 2, max_gap);
        computed_gaps.push_back(gap);
        if (gap == max_gap) {
            break;
        }
    }

    return shell_sort(std::move(items), [&computed_gaps](std::size_t k, std::size_t n) -> std::size_t {
        return computed_gaps[computed_gaps.size() - k - 1];
    });
}

std::size_t gcd(std::size_t a, std::size_t b) {
    if (a < b) {
        std::swap(a, b);
    }

    while (b != 0) {
        std::size_t tmp = b;
        b = a % b;
        a = tmp;
    }

    return a;
}

std::vector<int> shell_sort_incerpi_sedgewick(std::vector<int> items) {
    std::vector<std::size_t> computed_gaps{};

    std::vector<std::size_t> multiplicands{3};
    std::size_t gap = 3, k = 1;
    while (gap < items.size()) {
        auto r = (std::size_t) std::sqrt(2.0 * (double) k + std::sqrt(2 * k));

        if (r > multiplicands.size()) {
            for (std::size_t q = multiplicands.size(); q < r; ++q) {
                std::size_t new_member = std::numeric_limits<std::size_t>::max();

                for (std::size_t p = 0; p < q; ++p) {
                    auto n = (std::size_t) std::ceil(std::pow(5.0 / 2.0, q + 1));

                    while (gcd(n, multiplicands[p]) != 1)
                        ++n;

                    new_member = std::min(new_member, n);
                }

                multiplicands.push_back(new_member);
            }
        }

        std::size_t excluded_value = 0.5 * (std::pow(r, 2.0) + r) - k;

        gap = 1;
        for (std::size_t i = 0; i < r; ++i) {
            if (i != excluded_value) {
                gap *= multiplicands[i];
            }
        }
        computed_gaps.push_back(gap);
        ++k;
    }

    return shell_sort(std::move(items), [&computed_gaps](std::size_t k, std::size_t n) -> std::size_t {
        return computed_gaps[computed_gaps.size() - k - 1];
    });
}

std::vector<int> shell_sort_sedgewick_1982(std::vector<int> items) {
    std::vector<std::size_t> computed_gaps{1};

    std::size_t gap = 0, k = 1;
    while (gap < items.size()) {
        gap = (std::size_t) (std::pow(4, k) + 3 * std::pow(2, k - 1) + 1);
        ++k;
        if (gap < items.size())
            computed_gaps.push_back(gap);
    }

    return shell_sort(std::move(items), [&computed_gaps](std::size_t k, std::size_t n) -> std::size_t {
        return computed_gaps[computed_gaps.size() - k - 1];
    });
}

std::vector<int> shell_sort_sedgewick_1986(std::vector<int> items) {
    std::vector<std::size_t> computed_gaps{};

    std::size_t gap = 0, k = 0;
    while (gap < items.size()) {
        if (k % 2 == 0) {
            gap = (std::size_t) (9 * (std::pow(2, k) - std::pow(2, k / 2)) + 1);
        } else {
            gap = (std::size_t) (8 * (std::pow(2, k)) - 6 * std::pow(2, (k + 1) / 2) + 1);
        }

        ++k;

        if (gap < items.size()) {
            computed_gaps.push_back(gap);
        }
    }

    return shell_sort(std::move(items), [&computed_gaps](std::size_t k, std::size_t n) -> std::size_t {
        return computed_gaps[computed_gaps.size() - k - 1];
    });
}

std::vector<int> shell_sort_gonnet_baeza_yates(std::vector<int> items) {
    std::vector<std::size_t> computed_gaps{items.size()};

    std::size_t gap = items.size(), k = 1;
    while (gap != 1) {
        gap = std::max<std::size_t>((5 * gap - 1) / 11, 1);
        computed_gaps.push_back(gap);
    }

    return shell_sort(std::move(items), [&computed_gaps](std::size_t k, std::size_t n) -> std::size_t {
        return computed_gaps[k];
    });
}

std::vector<int> shell_sort_tokuda(std::vector<int> items) {
    std::vector<std::size_t> computed_gaps{1};

    std::size_t gap = 0, k = 1;
    while (gap < items.size()) {
        gap = (std::size_t) std::max<double>(std::ceil(0.2 * (9.0 * std::pow(2.25, k - 1) - 4.0)), 1.0);
        ++k;
        if (gap < items.size() && gap != 1)
            computed_gaps.push_back(gap);
    }

    return shell_sort(std::move(items), [&computed_gaps](std::size_t k, std::size_t n) -> std::size_t {
        return computed_gaps[computed_gaps.size() - k - 1];
    });
}

std::vector<int> shell_sort_ciura(std::vector<int> items) {
    std::vector<std::size_t> gaps{701, 301, 132, 57, 23, 10, 4, 1};

    return shell_sort(std::move(items), [&gaps](std::size_t k, std::size_t n) -> std::size_t {
        return gaps[k];
    });
}