/**
 * @file
 * This file forms part of tr-sort, a novel sorting algorithm
 *
 * @author Joshua Saxby <joshua.a.saxby@gmail.com>
 * @date 2021-05-08
 *
 * @copyright Joshua Saxby 2021. All rights reserved.
 *
 * @copyright
 * This is closed-source software and may not be produced in either part or in
 * full, in either source or binary form, without the express written consent
 * of the copyright holder.
 *
 */

#ifndef COM_SAXBOPHONE_TR_SORT_HPP
#define COM_SAXBOPHONE_TR_SORT_HPP

#include <cmath>       // ceil, nextafter, pow
#include <cstddef>
#include <span>
#include <type_traits>
#include <vector>


namespace com::saxbophone::tr_sort {
    /**
     * @brief Stable sorts the input dating using the Transposition Sort sorting
     * algorithm.
     * @tparam T data typed stored in the iterable to sort
     * @tparam Extent size of iterable (leave default for dynamic size)
     * @tparam Real data type used by the algorithm for real numbers
     * @param data iterable to sort
     * @returns `true` when the sort was successful
     * @returns `false` when the sort was unsuccessful
     * @todo Add template param or function param for conversion lambda function
     * to be used for casting objects of type T to some other Scalar type. This
     * may or may not need an additional template type param to determine which
     * data type is to be used for the result value of such conversions, when
     * the type T is non-scalar (assuming we don't just default to using Real in
     * that case). Need to check the current implementation of the algorithm to
     * know how to proceed in this regard.
     */
    template <
        typename T,
        std::size_t Extent = std::dynamic_extent,
        typename Real = long double
    >
    bool sort(std::span<T, Extent> data) {
        // don't sort data of length {0..1}
        if (data.size() < 2) {
            return true;
        }
        // gather stats on first pass of data
        std::size_t size = data.size();
        T min, max;
        if constexpr (std::is_floating_point<T>::value) {
            min = +std::numeric_limits<T>::infinity();
            max = -std::numeric_limits<T>::infinity();
        } else {
            min = std::numeric_limits<T>::max();
            max = std::numeric_limits<T>::lowest();
        }
        // Real mean = 0.0;
        // Real mid = 0.0;
        // must be Real because otherwise gives out-of-range values for signed types
        Real range = 0;
        T previous = data[0];
        bool already_sorted = true;
        for (auto datum : data) {
            // if (not std::isfinite(datum)) {
            //     continue; // skip non-finite values
            // }
            // mean += datum;
            if (datum < min) {
                min = datum;
            }
            // NOTE: >= to preserve stable sorting, so last max value gets put at end
            /*
             * Not actually an issue for this implementation because it only deals
             * with primitives, however if extended to handle general-purpose types
             * (using some casting), this will matter.
             */
            if (datum >= max) {
                max = datum;
            }
            if (datum < previous) {
                already_sorted = false;
            }
            previous = datum;
        }
        // short cut for already sorted
        if (already_sorted) {
            return true;
        }
        // short cut for size 2
        if (data.size() == 2) {
            data[0] = min;
            data[data.size() - 1] = max;
        }
        if constexpr (std::is_floating_point<T>::value) {
            if (min == -std::numeric_limits<T>::infinity()) {
                min = std::numeric_limits<T>::lowest();
            }
            if (max == +std::numeric_limits<T>::infinity()) {
                max = std::numeric_limits<T>::max();
            }
        }
        // mean /= size;
        // mid = (min + max) / 2.0;
        range = (Real)max - (Real)min;
        // temporary storage for sorting -- vector of sub-vectors to store partial sorts
        std::vector<std::vector<T>> sorts(data.size());
        for (auto n : data) {
            // calculated sort position
            Real raw_pos = std::ceil((((Real)n - min) / range) * (size - 1));
            std::size_t pos = (std::size_t)raw_pos;
            if (raw_pos < 0) {
                pos = 0;
            } else if (raw_pos > (sorts.size() - 1)) {
                pos = sorts.size() - 1;
            }
            sorts[pos].push_back(n);
        }
        // pull data out of sorted buckets, recursively sorting each before pulling
        std::size_t i = 0;
        for (auto& bucket : sorts) {
            // recursively sort any sort buckets that are larger than 1
            if (bucket.size() > 1) {
                sort<T, std::dynamic_extent, Real>(bucket);
            }
            for (T datum : bucket) {
                data[i] = datum;
                i++;
            }
        }
        return true;
    }
}

#endif // include guard
