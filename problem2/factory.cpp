#include <vector>
#include <map>
#include <string>
#include <any>
#include <memory>
#include <stdexcept>

#include "factory.h"
#include "generators.h"

class TRandomNumberGeneratorFactory::TImpl {
    class ICreator {
    public:
        virtual ~ICreator(){}
        virtual std::unique_ptr<TRandomNumberGenerator> Create(const std::vector<std::any> &params) const = 0;
    };
    using TCreatorPtr = std::shared_ptr<ICreator>;
    using TRegisteredCreators = std::map<std::string, TCreatorPtr>;
    TRegisteredCreators RegisteredCreators;

public:
    template <class TParticularGenerator>
    class TCreator : public ICreator {
        std::unique_ptr<TRandomNumberGenerator> Create(const std::vector<std::any> &params) const override {
            return std::make_unique<TParticularGenerator>(params);
        }
    };

    TImpl() { RegisterAll();}

    template <typename TParticularGenerator>
    void RegisterCreator(const std::string& name) {
        RegisteredCreators[name] = std::make_shared<TCreator<TParticularGenerator>>();
    }

    void RegisterAll() {
        RegisterCreator<TPoissonGenerator>("poisson");
        RegisterCreator<TFiniteGenerator>("finite");
        RegisterCreator<TBernoulliGenerator>("bernoulli");
        RegisterCreator<TGeometricGenerator>("geometric");
    }

    std::unique_ptr<TRandomNumberGenerator> CreateObject(const std::string& n, const std::vector<std::any> &params) const {
        auto creator = RegisteredCreators.find(n);
        if (creator == RegisteredCreators.end()) {
            return nullptr;
        }
        try {
            return creator->second->Create(params);
        } catch (std::invalid_argument &err) {
            return nullptr;
        }
    }

    std::vector<std::string> GetAvailableObjects () const {
        std::vector<std::string> result;
        for (const auto& creatorPair : RegisteredCreators) {
            result.push_back(creatorPair.first);
        }
        return result;
    }
};

std::unique_ptr<TRandomNumberGenerator> TRandomNumberGeneratorFactory::Create(const std::string& name, const std::vector<std::any> &params) const {
    return Impl->CreateObject(name, params);
}

TRandomNumberGeneratorFactory::TRandomNumberGeneratorFactory() : Impl(std::make_unique<TRandomNumberGeneratorFactory::TImpl>()) {}

TRandomNumberGeneratorFactory::~TRandomNumberGeneratorFactory(){}

std::vector<std::string> TRandomNumberGeneratorFactory::GetAvailableObjects() const {
    return Impl->GetAvailableObjects();
}
