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
