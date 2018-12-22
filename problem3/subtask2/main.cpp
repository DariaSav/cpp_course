#include <memory>
#include <unordered_set>
#include <unordered_map>
#include <vector>


class IShop;


class IProduct: public std::enable_shared_from_this<IProduct>  {
public:
    IProduct(const std::string &name, double price): _name(name), _price(price), _on_sale(false) {}

    void Attach(std::weak_ptr<IShop> shop);
    void Detach(std::weak_ptr<IShop> shop);

    std::string GetName() const { return _name; };
    double GetPrice() const { return _price; };
    bool OnSale() const { return  _on_sale; }

    void StartSales() {
        _on_sale = true;
        NotifyShops();
    }

    void StopSales() {
        _on_sale = false;
        NotifyShops();
    }

    void ChangePrice(double price) {
        _price = price;
        NotifyShops();
    }

    virtual ~IProduct() = default;

    friend IShop;
private:
    std::string _name;
    double _price;
    bool _on_sale;
    std::vector<std::weak_ptr<IShop>> _shops;

    void NotifyShops();
};


class IShop {
public:
    IShop(const std::string &name): _name(name) {};
    virtual ~IShop() = default;

    friend void IProduct::Attach(std::weak_ptr <IShop>);
    friend void IProduct::Detach(std::weak_ptr <IShop> shop);
    friend void IProduct::NotifyShops();

private:
    std::string _name;
    std::unordered_set<std::shared_ptr<IProduct>> _products;
    std::unordered_map<std::string, double> _products_on_sale;

    enum shopAction {START_SALE, STOP_SALE, CHANGE_PRICE};

    void UpdateProduct(std::shared_ptr<IProduct> prod) {
        if (prod->OnSale()) {
            _products_on_sale.insert_or_assign(prod->GetName(), prod->GetPrice());
//            Announcement("")
        } else {
            _products_on_sale.erase(prod->GetName());
        }
    }

    void AddProduct(std::shared_ptr<IProduct> prod) {
        _products.insert(prod);
        UpdateProduct(prod);
    }

    void DeleteProduct(std::shared_ptr<IProduct> prod) {
        _products.erase(prod);
        UpdateProduct(prod);
    }

//    void Announcement(shopAction action, std::shared_ptr<IProduct> prod) {
//        std::cout << "The shop '" << _name << "' " <<
//    }


};


void IProduct::Attach(std::weak_ptr <IShop> shop)  {
    auto shop_ptr = shop.lock();
    if (shop_ptr != nullptr) {
        shop_ptr->AddProduct(this->shared_from_this());
    }
};

void IProduct::Detach(std::weak_ptr <IShop> shop)  {
    auto shop_ptr = shop.lock();
    if (shop_ptr != nullptr) {
        shop_ptr->DeleteProduct(this->shared_from_this());
    }
};

void IProduct::NotifyShops() {
    for (auto shop: _shops) {
        auto shop_ptr = shop.lock();
        if (shop_ptr != nullptr) {
            shop_ptr->UpdateProduct(this->shared_from_this());
        }
    }
};


enum ToySize { SMALL, MIDDLE, BIG };


class TeddyBear: public IProduct {
public:
    TeddyBear(double price, ToySize size, const std::string &seria)
        : IProduct("TeddyBear", price), _size(size), _seria(seria) {};
private:
    ToySize _size;
    std::string _seria;
};




int main() {
    auto toy_shop_ptr = std::make_shared<IShop>(std::string("Best Toys"));

    TeddyBear bear(100, SMALL, "super_cool");

    bear.Attach(toy_shop_ptr);
    bear.Detach(toy_shop_ptr);

}