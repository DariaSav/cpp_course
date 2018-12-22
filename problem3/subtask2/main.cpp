#include <memory>


class IShop;

class IProduct {
public:
    virtual void Attach(std::shared_ptr<IShop>) = 0;
    virtual void Detach(std::shared_ptr<IShop>) = 0;
    virtual double GetPrice() = 0;
    virtual void StartSales() = 0;
    virtual void StopSales() = 0;
    virtual void ChangePrice(double price) = 0;
    virtual ~IProduct() = default;

};


class IShop {
public:
    virtual ~IShop() = default;
    virtual Update (std::IProduct)


    friend void IProduct::Attach(std::shared_ptr <IShop>);
private:
    virtual void Track(std::shared_ptr<IProduct>) = 0;
};

class TypicalShop: public



class TypicalProduct: public IProduct, public std::enable_shared_from_this<TypicalProduct> {
public:
    void Attach(std::shared_ptr<IShop> shop) {
        shop->Track(this->shared_from_this());
    }
};

int main() {

}