/**
 * This file forms part of tr-sort, a novel sorting algorithm
 *
 * Copyright Joshua Saxby 2021. All rights reserved.
 *
 * This is closed-source software and may not be produced in either part or in
 * full, in either source or binary form, without the express written consent
 * of the copyright holder.
 *
 */

#include <tr-sort.hpp>


namespace com::saxbophone::tr_sort {
    template bool sort<std::uint8_t>(std::span<std::uint8_t> data);
    template bool sort<std::int8_t>(std::span<std::int8_t> data);
    template bool sort<std::uint16_t>(std::span<std::uint16_t> data);
    template bool sort<std::int16_t>(std::span<std::int16_t> data);
    template bool sort<std::uint32_t>(std::span<std::uint32_t> data);
    template bool sort<std::int32_t>(std::span<std::int32_t> data);
    template bool sort<std::uint64_t>(std::span<std::uint64_t> data);
    template bool sort<std::int64_t>(std::span<std::int64_t> data);
    template bool sort<float>(std::span<float> data);
    template bool sort<double>(std::span<double> data);
    template bool sort<long double>(std::span<long double> data);
}
