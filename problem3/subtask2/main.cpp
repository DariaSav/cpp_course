#include <memory>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <vector>
#include <mutex>
#include <iostream>
#include <list>
#include <atomic>


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

protected:
    const std::string _name;
    std::atomic<double> _price;
    std::atomic_bool _on_sale;
    std::list<std::weak_ptr<IShop>> _shops;

    void NotifyShops();
};


class IShop {
public:
    IShop(const std::string &name): _name(name) {};
    virtual ~IShop() = default;
    const std::unordered_map<std::string, double> & GetProductOnSale() {
        return _products_on_sale;
    };


    friend void IProduct::Attach(std::weak_ptr <IShop>);
    friend void IProduct::Detach(std::weak_ptr <IShop> shop);
    friend void IProduct::NotifyShops();

protected:
    std::mutex _on_change;

    std::string _name;
    std::unordered_map<std::string, double> _products_on_sale;


    void UpdateProduct(std::shared_ptr <IProduct> prod) {
        std::lock_guard<std::mutex> lock(_on_change);

        if (prod->OnSale()) {
            _products_on_sale.insert_or_assign(prod->GetName(), prod->GetPrice());
        } else {
            _products_on_sale.erase(prod->GetName());
        }
    }

    void DeleteProduct(std::shared_ptr<IProduct> prod) {
        std::lock_guard<std::mutex> lock(_on_change);
         _products_on_sale.erase(prod->GetName());
    }
};


void IProduct::Attach(std::weak_ptr <IShop> shop)  {
    auto shop_ptr = shop.lock();
    if (shop_ptr != nullptr) {
        _shops.push_back(shop);
        shop_ptr->UpdateProduct(this->shared_from_this());
    }
};


void IProduct::Detach(std::weak_ptr <IShop> shop)  {
    auto shop_ptr = shop.lock();
    if (shop_ptr != nullptr) {
        // delete from this shop
        shop_ptr->DeleteProduct(this->shared_from_this());

        // delete from shops list
        for (auto iter=_shops.begin(); iter != _shops.end(); iter++) {
            auto track_shop_ptr = iter->lock();
            if (track_shop_ptr != nullptr) {
                if (shop_ptr == track_shop_ptr) {
                    _shops.erase(iter);
                    break;
                }
            }
        }
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


class ClassicShop: public IShop {
public:
    ClassicShop(std::string name): IShop(name) {};
    void AnnounceSales() {
        std::lock_guard<std::mutex> lock(_on_change);

        if (_products_on_sale.size()) {
            std::cout << "The shop '" << _name << "' sales following goods:  " << std::endl;
            for (auto prod: _products_on_sale) {
                std::cout << "    " << prod.first << ": " << prod.second << "$" << std::endl;
            }
        } else {
            std::cout << "The shop '" << _name << "' don't sale anything now." << std::endl;
        }
    }
};



int main() {
    auto toy_shop_ptr = std::make_shared<ClassicShop>(std::string("Best Toys"));

    auto bear = std::make_shared<TeddyBear>(100, SMALL, "super_cool");

    bear->Attach(toy_shop_ptr);
    toy_shop_ptr->AnnounceSales();

    bear->StartSales();
    toy_shop_ptr->AnnounceSales();

    bear->Detach(toy_shop_ptr);
    toy_shop_ptr->AnnounceSales();

    bear->Attach(toy_shop_ptr);
    toy_shop_ptr->AnnounceSales();

}