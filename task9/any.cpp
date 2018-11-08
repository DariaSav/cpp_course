#include <memory>
#include <iostream>
#include <string>
#include <stdexcept>


class Any {
public:
    template <typename T>
    Any (T value) {
        _pvalue = std::static_pointer_cast<Base>(std::make_shared<Derived<T>>(value));
    }

    template <typename T>
    T get() {
        std::shared_ptr<Derived<T>> p = std::dynamic_pointer_cast<Derived<T>>(_pvalue);
        if (p == nullptr) {
            throw std::logic_error("Contain other type.");
        } else {
            return p->value;
        }
    }

private:
    class Base {
    public:
    virtual ~Base() = default;
    };

    template <typename T>
    class Derived: public Base {
    public:
        Derived(T value): value(value) {};
        T value;
    };

    std::shared_ptr<Base> _pvalue;
};


int main(int argc, char **argv) {
    Any a(5);
    std::cout << a.get<int>() << std::endl;

    Any b(std::string("something"));
    std::cout << b.get<std::string>() << std::endl;

    a.get<std::string>();
}