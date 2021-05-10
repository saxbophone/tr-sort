#ifndef COM_SAXBOPHONE_TR_SORT_PRIVATE_TESTS_TEST_HELPERS_HPP
#define COM_SAXBOPHONE_TR_SORT_PRIVATE_TESTS_TEST_HELPERS_HPP

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <random>
#include <vector>

#include <iostream>
#include <typeinfo>


using namespace com::saxbophone::tr_sort;

namespace com::saxbophone::tr_sort::PRIVATE::test_helpers {
    class PRNG {
    public:
        PRNG() : _engine(std::random_device()()) {}

        // default --assume integral types
        template <typename T>
        std::vector<T> generate(std::size_t size) {
            return this->_generate<
                T,
                T,
                std::uniform_int_distribution,
                std::binomial_distribution,
                std::negative_binomial_distribution,
                std::poisson_distribution
            >(size);
        }

    private:
        // for randomly choosing one from a number of RandomNumberDistributions
        template <
            typename T,
            typename GenType,
            template<class> class... RandomNumberDistributions
        >
        class RND {
        };

        template <
            typename T,
            typename GenType,
            template<class> class RandomNumberDistribution,
            template<class> class... RandomNumberDistributions
        >
        class RND<
            T,
            GenType,
            RandomNumberDistribution,
            RandomNumberDistributions...
        > {
        private:
            template <typename OT, typename GT>
            class RNDMaker {
            public:
                template <template<class> class Dist>
                static Dist<GT> create_rnd(std::default_random_engine& engine) {
                    return Dist<GT>();
                }

                // distribution-specific overrides
                template <>
                static std::uniform_int_distribution<GT> create_rnd(std::default_random_engine& engine) {
                    return std::uniform_int_distribution<GT>(
                        std::numeric_limits<OT>::min(),
                        std::numeric_limits<OT>::max()
                    );
                }

                template <>
                static std::binomial_distribution<GT> create_rnd(std::default_random_engine& engine) {
                    std::uniform_real_distribution<double> probability(0.0, 1.0);
                    return std::binomial_distribution<GT>(
                        std::numeric_limits<OT>::max(),
                        probability(engine)
                    );
                }

                template <>
                static std::negative_binomial_distribution<GT> create_rnd(std::default_random_engine& engine) {
                    std::uniform_real_distribution<double> probability(0.0, 1.0);
                    return std::negative_binomial_distribution<GT>(
                        std::numeric_limits<OT>::max(),
                        probability(engine)
                    );
                }

                template <>
                static std::poisson_distribution<GT> create_rnd(std::default_random_engine& engine) {
                    std::uniform_real_distribution<double> probability(
                        std::numeric_limits<OT>::min(),
                        std::numeric_limits<OT>::max()
                    );
                    return std::poisson_distribution<GT>(probability(engine));
                }

                template <>
                static std::discrete_distribution<GT> create_rnd(std::default_random_engine& engine) {
                    std::uniform_real_distribution<double> probability(0.0, 1.0);
                    // build a table of weights for each possible outcome
                    std::vector<double> weights;
                    std::generate_n(
                        std::back_inserter(weights),
                        std::numeric_limits<OT>::max(),
                        [&](){ return probability(engine); }
                    );
                    return std::discrete_distribution<GT>(weights.begin(), weights.end());
                }
            };

        public:
            static std::vector<T> generate(
                std::default_random_engine& engine,
                std::size_t size,
                int chosen_rnd=-1
            ) {
                // only need to choose if chosen == -1
                int chosen = chosen_rnd;
                if (chosen == -1) {
                    std::size_t rnds = sizeof...(RandomNumberDistributions) + 1;
                    chosen = RND::_roll(engine, rnds);
                }
                // if it's 0, use this one, otherwise, go further down the chain
                if (chosen == 0) {
                    // create the distribution
                    RandomNumberDistribution<GenType> dist = RNDMaker<T, GenType>::template create_rnd<RandomNumberDistribution>(engine);
                    // pre-allocate and initialise to size
                    std::vector<T> data(size);
                    // fill vector with lambda calling prng
                    std::generate(data.begin(), data.end(), [&](){ return (T)dist(engine); });
                    return data;
                } else {
                    return RND<T, GenType, RandomNumberDistributions...>::generate(engine, size, chosen - 1);
                }
            }
        private:
            static std::size_t _roll(std::default_random_engine& engine, std::size_t choices) {
                std::uniform_int_distribution<std::size_t> dice(0, choices - 1);
                return dice(engine);
            }
        };

        template <
            typename T,
            typename GenType
        >
        class RND<T, GenType> {
        public:
            static std::vector<T> generate(
                std::default_random_engine&,
                std::size_t,
                int=-1
            ) {
                return {};
            }
        };

        template <typename T, typename GenType, template<class> class... RandomNumberDistributions>
        std::vector<T> _generate(std::size_t size) {
            return RND<T, GenType, RandomNumberDistributions...>::generate(this->_engine, size);
        }

        std::default_random_engine _engine;
    };

    /*
     * for 8-bit types, almost exactly the same, except generate as larger
     * type due to no stdlib random number distribution accepting them as
     * a type to generate
     */

    template <> std::vector<std::uint8_t> PRNG::generate<std::uint8_t>(std::size_t size) {
        return this->_generate<
            std::uint8_t,
            std::uint16_t,
            std::uniform_int_distribution,
            std::binomial_distribution,
            std::negative_binomial_distribution,
            std::poisson_distribution,
            std::discrete_distribution
        >(size);
    }

    template <> std::vector<std::int8_t> PRNG::generate<std::int8_t>(std::size_t size) {
        return this->_generate<
            std::int8_t,
            std::int16_t,
            std::uniform_int_distribution,
            std::binomial_distribution,
            std::negative_binomial_distribution,
            std::poisson_distribution,
            std::discrete_distribution
        >(size);
    }

    /*
     * 16-bit types are also allowed to use the discrete distribution because
     * the number of possible values is not so big that we cannot store a set
     * of weights for them
     */

    template <> std::vector<std::uint16_t> PRNG::generate<std::uint16_t>(std::size_t size) {
        return this->_generate<
            std::uint16_t,
            std::uint16_t,
            std::uniform_int_distribution,
            std::binomial_distribution,
            std::negative_binomial_distribution,
            std::poisson_distribution,
            std::discrete_distribution
        >(size);
    }

    template <> std::vector<std::int16_t> PRNG::generate<std::int16_t>(std::size_t size) {
        return this->_generate<
            std::int16_t,
            std::int16_t,
            std::uniform_int_distribution,
            std::binomial_distribution,
            std::negative_binomial_distribution,
            std::poisson_distribution,
            std::discrete_distribution
        >(size);
    }

    /*
     * floating-point types are all very similar except that they use the
     * floating-point number distribution
     */

    template <> std::vector<float> PRNG::generate<float>(std::size_t size) {
        return this->_generate<
            float,
            float,
            std::uniform_real_distribution,
            std::exponential_distribution,
            std::gamma_distribution,
            std::weibull_distribution,
            std::extreme_value_distribution,
            std::normal_distribution,
            std::lognormal_distribution,
            std::chi_squared_distribution,
            std::cauchy_distribution,
            std::fisher_f_distribution,
            std::student_t_distribution,
            std::piecewise_constant_distribution,
            std::piecewise_linear_distribution
        >(size);
    }

    template <> std::vector<double> PRNG::generate<double>(std::size_t size) {
        return this->_generate<
            double,
            double,
            std::uniform_real_distribution,
            std::exponential_distribution,
            std::gamma_distribution,
            std::weibull_distribution,
            std::extreme_value_distribution,
            std::normal_distribution,
            std::lognormal_distribution,
            std::chi_squared_distribution,
            std::cauchy_distribution,
            std::fisher_f_distribution,
            std::student_t_distribution,
            std::piecewise_constant_distribution,
            std::piecewise_linear_distribution
        >(size);
    }

    template <> std::vector<long double> PRNG::generate<long double>(std::size_t size) {
        // TODO: this really doesn't work well. Pick a better distribution for Reals
        return this->_generate<
            long double,
            long double,
            std::uniform_real_distribution,
            std::exponential_distribution,
            std::gamma_distribution,
            std::weibull_distribution,
            std::extreme_value_distribution,
            std::normal_distribution,
            std::lognormal_distribution,
            std::chi_squared_distribution,
            std::cauchy_distribution,
            std::fisher_f_distribution,
            std::student_t_distribution,
            std::piecewise_constant_distribution,
            std::piecewise_linear_distribution
        >(size);
    }

    PRNG prng;
}

#endif // include guard
