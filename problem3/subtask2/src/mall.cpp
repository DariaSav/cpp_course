#include <memory>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <vector>
#include <mutex>
#include <iostream>
#include <list>
#include <atomic>

#include "mall.h"


void IProduct::Attach(std::weak_ptr <IShop> shop)  {
    auto shop_ptr = shop.lock();
    if (shop_ptr != nullptr) {
        _shops.push_back(shop);
        shop_ptr->UpdateProduct(_unique_name, _on_sale, _price);
    }
};


void IProduct::Detach(std::weak_ptr <IShop> shop)  {
    auto shop_ptr = shop.lock();
    if (shop_ptr != nullptr) {
        // delete from this shop
        shop_ptr->DeleteProduct(_unique_name);

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
            shop_ptr->UpdateProduct(_unique_name, _on_sale, _price);
        }
    }
};


const std::unordered_map<std::string, double> & IShop::GetProductOnSale() const {
    std::lock_guard<std::mutex> lock(_on_change);
    return _products_on_sale;
};


void IShop::UpdateProduct(const std::string &prod_name, bool on_sale, double price) {
    std::lock_guard<std::mutex> lock(_on_change);

    if (on_sale) {
        _products_on_sale.insert_or_assign(prod_name, price);
    } else {
        _products_on_sale.erase(prod_name);
    }
}


void  IShop::DeleteProduct(const std::string &prod_name) {
    std::lock_guard<std::mutex> lock(_on_change);
    _products_on_sale.erase(prod_name);
}
