#ifndef CPP_COURSE_SHOP_H
#define CPP_COURSE_SHOP_H

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


class IProduct {
public:
    IProduct(const std::string &unique_name, double price)
        : _unique_name(unique_name), _price(price), _on_sale(false) {}

    virtual void Attach(std::weak_ptr<IShop> shop);
    virtual void Detach(std::weak_ptr<IShop> shop);

    std::string GetUniqueName() const { return _unique_name; };
    double GetPrice() const { return _price; };
    bool OnSale() const { return  _on_sale; }

    virtual void StartSales() { _on_sale = true; NotifyShops(); }

    virtual void StopSales() { _on_sale = false; NotifyShops(); }

    virtual void ChangePrice(double price) { _price = price; NotifyShops(); }

    virtual ~IProduct() = default;

    friend IShop;

protected:
    const std::string _unique_name;
    std::atomic<double> _price;
    std::atomic_bool _on_sale;
    std::list<std::weak_ptr<IShop>> _shops;

    void NotifyShops();
};


class IShop {
public:
    IShop(const std::string &name): _name(name) {};
    virtual ~IShop() = default;

    std::string GetName() const { return _name; }

    const std::unordered_map<std::string, double> & GetProductOnSale() const;

    friend void IProduct::Attach(std::weak_ptr <IShop>);
    friend void IProduct::Detach(std::weak_ptr <IShop> shop);
    friend void IProduct::NotifyShops();

protected:
    mutable std::mutex _on_change;

    const std::string _name;
    std::unordered_map<std::string, double> _products_on_sale;

    virtual void UpdateProduct(const std::string &prod_name, bool on_sale, double price);
    virtual void DeleteProduct(const std::string &prod_name);
};


#endif //CPP_COURSE_SHOP_H
