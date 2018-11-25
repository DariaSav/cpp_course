#ifndef CPP_COURSE_GENERATORS_H
#define CPP_COURSE_GENERATORS_H

#include <vector>
#include <any>
#include <typeinfo>
#include <random>
#include <stdexcept>
#include <chrono>
#include <numeric>
#include <cmath>


class TRandomNumberGenerator {
public:
    virtual ~TRandomNumberGenerator() = default;
    virtual double Generate() const = 0;
};


class TPoissonGenerator: public TRandomNumberGenerator {
public:
    TPoissonGenerator(const std::vector<std::any> &params) {
        if (params.size() != 1) {
            throw std::bad_cast();
        }

        double lambda = std::any_cast<double>(params[0]);

        if (lambda <= 0) {
            throw std::invalid_argument("Lambda of poisson distribution must be > 0");
        }

        distribution = std::poisson_distribution<int>(lambda);
        generator = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());
    }

    double Generate() const {
        return distribution(generator);
    }

    ~TPoissonGenerator() {}
private:
    mutable std::poisson_distribution<int> distribution;
    mutable std::default_random_engine generator;
};


class TFiniteGenerator: public TRandomNumberGenerator {
public:
    TFiniteGenerator(const std::vector<std::any> &params) {
        if (params.size() != 2) {
            throw std::bad_cast();
        }
        values = std::any_cast<std::vector<double>>(params[0]);
        const std::vector<double> &probs = std::any_cast<std::vector<double>>(params[1]);
        if (values.size() !=  probs.size()) {
            throw std::bad_cast();
        }

        double sum_probs = 0;
        for (auto& p : probs) {
            if (p < 0 or p > 1) {
                throw std::invalid_argument("All probabilities of finite distribution must be >= 0 and <= 1");
            }
            sum_probs += p;
        }

        if (std::abs(sum_probs - 1) > 1e-6) {
            throw std::invalid_argument("All probabilities of finite distribution must be summed to 1");
        }

        distribution = std::discrete_distribution<int>(probs.begin(), probs.end());
        generator = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());
    }

    double Generate() const {
        return values[distribution(generator)];
    }

    ~TFiniteGenerator() {}

private:
    mutable std::discrete_distribution<int> distribution;
    mutable std::default_random_engine generator;
    std::vector<double> values;
};


class TBernoulliGenerator: public TRandomNumberGenerator {
public:
    TBernoulliGenerator(const std::vector<std::any> &params) {
        if (params.size() != 1) {
            throw std::bad_cast();
        }
        double prob = std::any_cast<double>(params[0]);

        if (prob < 0 or prob > 1) {
            throw std::invalid_argument("Probability value of bernoulli distribution must be >= 0, <= 1");
        }

        distribution = std::bernoulli_distribution(prob);
        generator = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());
    }

    double Generate() const {
        return distribution(generator);
    }

    ~TBernoulliGenerator() {}
private:
    mutable std::bernoulli_distribution distribution;
    mutable std::default_random_engine generator;
};


class TGeometricGenerator: public TRandomNumberGenerator {
public:
    TGeometricGenerator(const std::vector<std::any> &params) {
        if (params.size() != 1) {
            throw std::bad_cast();
        }
        double prob = std::any_cast<double>(params[0]);

        if (prob < 0 or prob > 1) {
            throw std::invalid_argument("Probability value of geometric distribution must be >= 0, <= 1");
        }

        distribution = std::geometric_distribution<int>(prob);
        generator = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());
    }

    double Generate() const {
        return distribution(generator);
    }

    ~TGeometricGenerator() {}
private:
    mutable std::geometric_distribution<int> distribution;
    mutable std::default_random_engine generator;
};

#endif //CPP_COURSE_GENERATORS_H
