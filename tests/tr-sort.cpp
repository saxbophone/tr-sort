#include <algorithm>
#include <vector>

#include <catch2/catch.hpp>

#include <tr-sort.hpp>

#include "test_helpers.hpp"


using namespace com::saxbophone;                                 // tr_sort
using namespace com::saxbophone::tr_sort::PRIVATE::test_helpers; // prng

TEST_CASE("Base test case for sort()") {
    // PARAM: TYPE (int)
    // PARAM: SIZE (100)
    std::vector<int> unsorted_data = prng.generate<int>(100);
    // create two copies to work-in-place
    std::vector<int> tr_sorted_data = unsorted_data;
    std::vector<int> stable_sorted_data = unsorted_data;
    // sort with tr-sort and stdlib stable_sort
    REQUIRE(tr_sort::sort<int>({tr_sorted_data}));
    // stable_sort returns void so no need to test return value
    std::stable_sort(stable_sorted_data.begin(), stable_sorted_data.end());
    // verify both sorted arrays are equal
    REQUIRE(tr_sorted_data == stable_sorted_data);
}
