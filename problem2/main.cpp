#include <cassert>
#include <cmath>
#include <memory>
#include <string>
#include <stdexcept>
#include <iostream>
#include "factory.h"

#define ASSERT(x) \
    if ( !(x) ) { \
        throw std::logic_error(std::string("Assert failed: ") + #x + " In: " + std::string(__FILE__) + ":" + std::to_string(__LINE__)); \
    }


double compute_distribution_mean(const std::unique_ptr<TRandomNumberGenerator> &distribution, unsigned points_count=1000000) {
    double mean = 0;
    for(int n = 1; n <= points_count; n++) {
        mean += (distribution->Generate() - mean) / n;
    }
    return mean;
}


bool check_mean(const std::unique_ptr<TRandomNumberGenerator> &distribution, double true_mean_value) {
    return (std::abs(compute_distribution_mean(distribution) - true_mean_value) < 0.01);
}


int main(int argc, char **argv) {
    TRandomNumberGeneratorFactory factory;

    ASSERT(check_mean(factory.Create("poisson", {0.01}), 0.01));
    ASSERT(check_mean(factory.Create("poisson", {0.5}), 0.5));
    ASSERT(check_mean(factory.Create("poisson", {1.0}), 1.0));
    ASSERT(check_mean(factory.Create("poisson", {3.0}), 3.0));

    ASSERT(check_mean(factory.Create("bernoulli", {0.0}), 0));
    ASSERT(check_mean(factory.Create("bernoulli", {1.0}), 1));
    ASSERT(check_mean(factory.Create("bernoulli", {0.5}), 0.5));
    ASSERT(check_mean(factory.Create("bernoulli", {0.7}), 0.7));

    ASSERT(check_mean(factory.Create("geometric", {1.0}), 0));
    ASSERT(check_mean(factory.Create("geometric", {0.5}), 1));
    ASSERT(check_mean(factory.Create("geometric", {0.7}), 0.3 / 0.7));
    ASSERT(check_mean(factory.Create("geometric", {0.99}), 0.01 / 0.99));

    ASSERT(check_mean(factory.Create("finite", {std::vector<double>{1}, std::vector<double>{1}}), 1));
    ASSERT(check_mean(factory.Create("finite", {std::vector<double>{1, 2, 3, 4},
                                                std::vector<double>{0.25, 0.25, 0.25, 0.25}}),
                      2.5));
    ASSERT(check_mean(factory.Create("finite", {std::vector<double>{1, 2, 3, 4},
                                                std::vector<double>{0, 0, 0, 1}}),
                      4));

    ASSERT(check_mean(factory.Create("finite", {std::vector<double>{1, 2, 3, 4},
                                                std::vector<double>{0, 0.5, 0, 0.5}}),
                      3));

}