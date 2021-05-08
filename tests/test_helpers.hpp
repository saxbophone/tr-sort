#ifndef COM_SAXBOPHONE_TR_SORT_PRIVATE_TESTS_TEST_HELPERS_HPP
#define COM_SAXBOPHONE_TR_SORT_PRIVATE_TESTS_TEST_HELPERS_HPP

#include <cstddef>
#include <cstdint>
#include <limits>
#include <random>
#include <vector>


using namespace com::saxbophone::tr_sort;

namespace com::saxbophone::tr_sort::PRIVATE::test_helpers {
    // template<std::size_t SIZE>
    // std::array<Byte, SIZE> generate_random_bytes() {
    //     std::array<Byte, SIZE> data;
    //     std::default_random_engine engine;
    //     // N.B: Can't use uint8_t for type as generator doesn't support char types
    //     std::uniform_int_distribution<std::uint16_t> prng(0x0000, 0x00FF);
    //     for (auto& d : data) {
    //         d = (Byte)prng(engine);
    //     }
    //     return data;
    // }

    class PRNG {
    public:
        // default --assume integral types
        template <typename T>
        std::vector<T> generate(std::size_t size) {
            return this->_generate<T, T, std::uniform_int_distribution>(size);
        }

        /*
         * for 8-bit types, almost exactly the same, except generate as larger
         * type due to no stdlib random number distribution accepting them as
         * a type to generate
         */

        template <> std::vector<std::uint8_t> generate<std::uint8_t>(std::size_t size) {
            return this->_generate<std::uint8_t, std::uint16_t, std::uniform_int_distribution>(size);
        }

        template <> std::vector<std::int8_t> generate<std::int8_t>(std::size_t size) {
            return this->_generate<std::int8_t, std::int16_t, std::uniform_int_distribution>(size);
        }

        /*
         * floating-point types are all very similar except that they use the
         * floating-point number distribution
         */

        template <> std::vector<float> generate<float>(std::size_t size) {
            return this->_generate<float, float, std::uniform_real_distribution>(size);
        }

        template <> std::vector<double> generate<double>(std::size_t size) {
            return this->_generate<double, double, std::uniform_real_distribution>(size);
        }

        template <> std::vector<long double> generate<long double>(std::size_t size) {
            // TODO: this really doesn't work well. Pick a better distribution for Reals
            return this->_generate<long double, long double, std::uniform_real_distribution>(size);
        }

    private:
        template <typename T, typename GenType, template<class> class RandomNumberDistribution>
        std::vector<T> _generate(std::size_t size) {
            // create the distribution
            RandomNumberDistribution<GenType> dist(
                std::numeric_limits<T>::lowest(),
                std::numeric_limits<T>::max()
            );
            // pre-allocate and initialise to size
            std::vector<T> data(size);
            // fill vector with lambda calling prng
            std::generate(data.begin(), data.end(), [&](){ return dist(this->engine); });
            return data;
        }

        std::default_random_engine engine;
    };

    PRNG prng;
}

#endif // include guard
