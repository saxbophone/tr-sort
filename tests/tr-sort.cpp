#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <vector>

#include <catch2/catch.hpp>

#include <tr-sort.hpp>

#include "test_helpers.hpp"


using namespace com::saxbophone;                                 // tr_sort
using namespace com::saxbophone::tr_sort::PRIVATE::test_helpers; // prng

TEMPLATE_TEST_CASE(
    "tr_sort::sort() does a stable sort on input arrays of varying size", "",
    std::uint8_t, std::int8_t, std::uint16_t, std::int16_t, std::uint32_t,
    std::int32_t, std::uint64_t, std::int64_t, float, double, long double
) {
    std::size_t test_exponent = GENERATE(range(2u, 8u));
    std::size_t array_length = std::pow(10u, test_exponent);
    // PARAM: SIZE (100)
    std::vector<TestType> unsorted_data = prng.generate<TestType>(array_length);
    // create two copies to work-in-place
    std::vector<TestType> tr_sorted_data = unsorted_data;
    std::vector<TestType> stable_sorted_data = unsorted_data;
    // sort with tr-sort and stdlib stable_sort
    REQUIRE(tr_sort::sort<TestType>({tr_sorted_data}));
    // stable_sort returns void so no need to test return value
    std::stable_sort(stable_sorted_data.begin(), stable_sorted_data.end());
    // verify both sorted arrays are equal
    bool equal = tr_sorted_data == stable_sorted_data;
    REQUIRE(equal);
}
