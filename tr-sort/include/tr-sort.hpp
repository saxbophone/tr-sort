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

#include <cstddef>
#include <span>


namespace com::saxbophone::tr_sort {
    /**
     * @tparam T data typed stored in the iterable to sort
     * @tparam Extent size of iterable (leave default for dynamic size)
     * @tparam Real data type used by the algorithm for real numbers
     * @param data iterable to sort
     * @returns data stable-sorted by Transposition Sort
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
        return true; // XXX: debug only
    }
}

#endif // include guard
