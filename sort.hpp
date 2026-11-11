#ifndef SORT_HPP
#define SORT_HPP

#include <cstddef>
#include <functional>
#include <utility>
#include <vector>

namespace sorting {

namespace detail {

template <typename T, typename Compare>
void quick_sort_impl(std::vector<T>& arr, std::ptrdiff_t low, std::ptrdiff_t high, Compare& comp) {
    if (low >= high) {
        return;
    }

    std::ptrdiff_t mid = low + (high - low) / 2;
    if (comp(arr[mid], arr[low])) {
        std::swap(arr[low], arr[mid]);
    }
    if (comp(arr[high], arr[low])) {
        std::swap(arr[low], arr[high]);
    }
    if (comp(arr[high], arr[mid])) {
        std::swap(arr[mid], arr[high]);
    }
    T pivot = arr[mid];

    // Three-way partition handles duplicate keys without degrading to O(n^2).
    std::ptrdiff_t lt = low;
    std::ptrdiff_t i = low;
    std::ptrdiff_t gt = high;
    while (i <= gt) {
        if (comp(arr[i], pivot)) {
            std::swap(arr[lt], arr[i]);
            ++lt;
            ++i;
        } else if (comp(pivot, arr[i])) {
            std::swap(arr[i], arr[gt]);
            --gt;
        } else {
            ++i;
        }
    }

    quick_sort_impl(arr, low, lt - 1, comp);
    quick_sort_impl(arr, gt + 1, high, comp);
}

template <typename T, typename Compare>
void merge_sort_impl(std::vector<T>& arr, std::vector<T>& buffer, std::size_t left, std::size_t right, Compare& comp) {
    if (right - left <= 1) {
        return;
    }

    std::size_t mid = left + (right - left) / 2;
    merge_sort_impl(arr, buffer, left, mid, comp);
    merge_sort_impl(arr, buffer, mid, right, comp);

    std::size_t i = left;
    std::size_t j = mid;
    std::size_t k = left;
    while (i < mid && j < right) {
        if (!comp(arr[j], arr[i])) {
            buffer[k++] = std::move(arr[i++]);
        } else {
            buffer[k++] = std::move(arr[j++]);
        }
    }
    while (i < mid) {
        buffer[k++] = std::move(arr[i++]);
    }
    while (j < right) {
        buffer[k++] = std::move(arr[j++]);
    }
    for (std::size_t idx = left; idx < right; ++idx) {
        arr[idx] = std::move(buffer[idx]);
    }
}

}  // namespace detail

template <typename T, typename Compare = std::less<T>>
void quick_sort(std::vector<T>& arr, Compare comp = Compare{}) {
    if (arr.size() < 2) {
        return;
    }
    detail::quick_sort_impl(arr, 0, static_cast<std::ptrdiff_t>(arr.size() - 1), comp);
}

template <typename T, typename Compare = std::less<T>>
void merge_sort(std::vector<T>& arr, Compare comp = Compare{}) {
    if (arr.size() < 2) {
        return;
    }
    std::vector<T> buffer(arr.size());
    detail::merge_sort_impl(arr, buffer, 0, arr.size(), comp);
}

}  // namespace sorting

#endif  // SORT_HPP
