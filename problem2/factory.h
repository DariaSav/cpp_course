#ifndef CPP_COURSE_FACTORY_H
#define CPP_COURSE_FACTORY_H

#include <vector>
#include <string>
#include <any>
#include <memory>

#include "generators.h"


class TRandomNumberGeneratorFactory {
public:
    TRandomNumberGeneratorFactory();
    ~TRandomNumberGeneratorFactory();

    std::unique_ptr<TRandomNumberGenerator> Create(const std::string& name, const std::vector<std::any> &params) const;
    std::vector<std::string> GetAvailableObjects() const;
private:
    class TImpl;
    std::unique_ptr<const TImpl> Impl;
};


#endif //CPP_COURSE_FACTORY_H
