#include <iostream>

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
    std::size_t test_exponent = GENERATE(range(1u, 4u));
    std::size_t array_length = (std::size_t)std::pow(10u, test_exponent);
    for (int i = 0; i < 1000; i++) {
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
        // bool equal = tr_sorted_data == stable_sorted_data;
        REQUIRE(tr_sorted_data == stable_sorted_data);
        // REQUIRE(equal);
    }
}

TEMPLATE_TEST_CASE(
    "tr_sort::sort() does a stable sort on input arrays containing only extreme finite values and infinities", "",
    float, double, long double
) {
    const TestType VALUES[] = {
        // finite:
        std::numeric_limits<TestType>::lowest(),
        std::numeric_limits<TestType>::max(),
        // infinite:
        -std::numeric_limits<TestType>::infinity(),
        +std::numeric_limits<TestType>::infinity()
    };
    std::size_t array_size = 10;
    std::random_device rd;
    std::default_random_engine engine(rd());
    std::uniform_int_distribution<std::size_t> picker(0, 3);
    for (int i = 0; i < 1000; i++) {
        // generate random data
        std::vector<TestType> unsorted_data(array_size);
        std::generate(
            unsorted_data.begin(),
            unsorted_data.end(),
            [&](){ return VALUES[picker(engine)]; }
        );
        std::vector<TestType> tr_sorted_data = unsorted_data;
        std::vector<TestType> stable_sorted_data = unsorted_data;
        // std::cout << "sort({";
        // for (auto datum : unsorted_data) {
        //     std::cout << datum << ", ";
        // }
        // std::cout << "});" << std::endl;
        // sort with tr-sort and stdlib stable_sort
        REQUIRE(tr_sort::sort<TestType>({tr_sorted_data}));
        // stable_sort returns void so no need to test return value
        std::stable_sort(stable_sorted_data.begin(), stable_sorted_data.end());
        // results should be the same
        REQUIRE(tr_sorted_data == stable_sorted_data);
    }
}
