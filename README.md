# Transpose Sort
A novel sorting algorithm I've invented

This sorting algorithm works by calculating roughly where each item of the input array should go. Consequently, it only works on data types for which a conversion to scalar type (integer or real) can be provided.

Currently, sorting only works on types which are intrinsically scalar in their own right, however there are plans to add a new feature which allows for a user-provided conversion function to be specified, opening up the potential to be able to sort user-made arbitrary precision types, as well as maybe even sequence types such as strings (if the user can provide a conversion function to convert these to scalar).

### Quick usage demonstration

```cpp
#include <vector>

#include <tr-sort.hpp>


using namespace com::saxbophone;

int main() {
    std::vector<int> input = {
        -100, 212, -313, 40, 16, 32, 64, -907,
    };
    bool success = tr_sort::sort<int>({input});
    return success ? 0 : -1;
}
```

### Further Reading

- [tr_sort::sort()](@ref com::saxbophone::tr_sort::sort) API reference
