#include <array>
#include <vector>

//! [Including tr-sort]
#include <tr-sort.hpp>

//! [Including tr-sort]

//! [Sorting an array]
bool sorting_an_array() {
    std::array<int, 8> input = {1, 492, 7, 2136, 2, 3, 445, -10337,};
    std::array<int, 8> sorted = {-10337, 1, 2, 3, 7, 445, 492, 2136,};

    bool success = com::saxbophone::tr_sort::sort<int>(input);
    return success and input == sorted;
}
//! [Sorting an array]

//! [Sorting a vector]
// bool sorting_a_vector() {
//     std::vector<int> input = {1, 492, 7, 2136, 2, 3, 445, -10337,};
//     std::vector<int> sorted = {-10337, 1, 2, 3, 7, 445, 492, 2136,};

//     bool success = com::saxbophone::tr_sort::sort<int>(input);
//     return success and input == sorted;
// }
//! [Sorting a vector]

//! [Sorting a C array]
// bool sorting_a_c_array() {
//     int input[] = {1, 492, 7, 2136, 2, 3, 445, -10337,};
//     int sorted[] = {-10337, 1, 2, 3, 7, 445, 492, 2136,};

//     bool success = com::saxbophone::tr_sort::sort<int>(input);
//     return success and std::to_array(input) == std::to_array(sorted);
// }
//! [Sorting a C array]

int main() {
    bool success = sorting_an_array() /*and sorting_a_vector() and sorting_a_c_array()*/;
    return success ? 0 : 1;
}
