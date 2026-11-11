#include "sort.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

namespace {

template <typename T>
void print_vector(const std::string& label, const std::vector<T>& arr) {
    std::cout << label << ": [";
    for (std::size_t i = 0; i < arr.size(); ++i) {
        if (i > 0) {
            std::cout << ", ";
        }
        std::cout << arr[i];
    }
    std::cout << "]\n";
}

struct Student {
    std::string name;
    int score;
};

}  // namespace

int main() {
    std::vector<int> nums1 = {64, 34, 25, 12, 22, 11, 90};
    print_vector("Original (quick_sort)", nums1);
    sorting::quick_sort(nums1);
    print_vector("Ascending (quick_sort)", nums1);

    sorting::quick_sort(nums1, std::greater<int>{});
    print_vector("Descending (quick_sort)", nums1);

    std::vector<int> nums2 = {38, 27, 43, 3, 9, 82, 10};
    sorting::merge_sort(nums2);
    print_vector("Ascending (merge_sort)", nums2);

    std::vector<Student> students = {
        {"Alice", 92},
        {"Bob", 85},
        {"Charlie", 92},
        {"Diana", 98},
    };
    std::sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
        if (a.score != b.score) {
            return a.score > b.score;
        }
        return a.name < b.name;
    });

    std::cout << "Custom struct sort (std::sort):\n";
    for (const auto& s : students) {
        std::cout << "  " << s.name << ": " << s.score << "\n";
    }

    return 0;
}
