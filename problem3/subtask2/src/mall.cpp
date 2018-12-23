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