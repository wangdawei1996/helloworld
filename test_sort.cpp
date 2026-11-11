#include "sort.hpp"

#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <vector>

namespace {

int failures = 0;

template <typename T>
void expect_equal(const std::vector<T>& actual, const std::vector<T>& expected, const char* test_name) {
    if (actual != expected) {
        std::cerr << "[FAIL] " << test_name << "\n";
        ++failures;
    } else {
        std::cout << "[PASS] " << test_name << "\n";
    }
}

void expect_true(bool condition, const char* test_name) {
    if (!condition) {
        std::cerr << "[FAIL] " << test_name << "\n";
        ++failures;
    } else {
        std::cout << "[PASS] " << test_name << "\n";
    }
}

struct Record {
    int key;
    int order;

    bool operator==(const Record& other) const {
        return key == other.key && order == other.order;
    }
};

void test_quick_sort_basic() {
    std::vector<int> v = {64, 34, 25, 12, 22, 11, 90};
    sorting::quick_sort(v);
    expect_equal(v, {11, 12, 22, 25, 34, 64, 90}, "quick_sort basic ascending");
}

void test_quick_sort_edge_cases() {
    std::vector<int> empty;
    sorting::quick_sort(empty);
    expect_equal(empty, {}, "quick_sort empty");

    std::vector<int> single = {42};
    sorting::quick_sort(single);
    expect_equal(single, {42}, "quick_sort single element");

    std::vector<int> sorted = {1, 2, 3, 4, 5};
    sorting::quick_sort(sorted);
    expect_equal(sorted, {1, 2, 3, 4, 5}, "quick_sort already sorted");

    std::vector<int> reverse = {5, 4, 3, 2, 1};
    sorting::quick_sort(reverse);
    expect_equal(reverse, {1, 2, 3, 4, 5}, "quick_sort reverse sorted");

    std::vector<int> duplicates = {3, -1, 3, 0, -1, 2, 3, 0};
    sorting::quick_sort(duplicates);
    expect_equal(duplicates, {-1, -1, 0, 0, 2, 3, 3, 3}, "quick_sort duplicates and negatives");
}

void test_quick_sort_custom_comparator_and_types() {
    std::vector<int> desc = {4, 1, 7, 3, 9};
    sorting::quick_sort(desc, std::greater<int>{});
    expect_equal(desc, {9, 7, 4, 3, 1}, "quick_sort descending comparator");

    std::vector<std::string> words = {"banana", "apple", "cherry", "date"};
    sorting::quick_sort(words);
    expect_equal(words, {"apple", "banana", "cherry", "date"}, "quick_sort strings");
}

void test_merge_sort_basic_and_stability() {
    std::vector<int> v = {38, 27, 43, 3, 9, 82, 10};
    sorting::merge_sort(v);
    expect_equal(v, {3, 9, 10, 27, 38, 43, 82}, "merge_sort basic ascending");

    std::vector<int> empty;
    sorting::merge_sort(empty);
    expect_equal(empty, {}, "merge_sort empty");

    std::vector<int> desc = {4, 1, 7, 3, 9};
    sorting::merge_sort(desc, std::greater<int>{});
    expect_equal(desc, {9, 7, 4, 3, 1}, "merge_sort descending comparator");

    std::vector<Record> records = {{2, 1}, {1, 1}, {2, 2}, {1, 2}, {2, 3}};
    sorting::merge_sort(records, [](const Record& a, const Record& b) {
        return a.key < b.key;
    });
    std::vector<Record> expected = {{1, 1}, {1, 2}, {2, 1}, {2, 2}, {2, 3}};
    expect_true(records == expected, "merge_sort preserves stability");
}

void test_large_inputs() {
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(-500, 500);

    std::vector<int> base(5000);
    for (int& x : base) {
        x = dist(rng);
    }

    std::vector<int> expected = base;
    std::sort(expected.begin(), expected.end());

    std::vector<int> q = base;
    sorting::quick_sort(q);
    expect_true(q == expected, "quick_sort 5000 random elements");

    std::vector<int> m = base;
    sorting::merge_sort(m);
    expect_true(m == expected, "merge_sort 5000 random elements");

    std::vector<int> all_equal(2000, 7);
    sorting::quick_sort(all_equal);
    expect_true(all_equal == std::vector<int>(2000, 7), "quick_sort 2000 identical elements");
}

}  // namespace

int main() {
    test_quick_sort_basic();
    test_quick_sort_edge_cases();
    test_quick_sort_custom_comparator_and_types();
    test_merge_sort_basic_and_stability();
    test_large_inputs();

    if (failures > 0) {
        std::cerr << failures << " test(s) failed.\n";
        return 1;
    }
    std::cout << "All tests passed.\n";
    return 0;
}
