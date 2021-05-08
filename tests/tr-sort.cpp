#include <catch2/catch.hpp>

#include <tr-sort.hpp>

#include "test_helpers.hpp"

using namespace com::saxbophone::tr_sort;
using namespace com::saxbophone::tr_sort::PRIVATE::test_helpers;

// dummy code
TEST_CASE("Library works") {
    std::vector<long double> data = prng.generate<long double>(1024);
    REQUIRE(sort<long double>({data}));
}
