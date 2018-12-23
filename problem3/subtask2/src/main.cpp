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



class TeddyBear: public IProduct {
public:
    TeddyBear(double price): IProduct("TeddyBear", price) {};
};


class AnnouncingShop: public IShop {
public:
    AnnouncingShop(std::string name): IShop(name) {};

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
    };

    void UpdateProduct(const std::string &prod_name, bool on_sale, double price) override {
        IShop::UpdateProduct(prod_name, on_sale, price);
    }
    void DeleteProduct(const std::string &prod_name) override {
        IShop::DeleteProduct(prod_name);
    }
};


int main() {
//    auto toy_shop_ptr = std::make_shared<ClassicShop>(std::string("Best Toys"));
//
//    auto bear = std::make_shared<TeddyBear>(100, SMALL, "super_cool");
//
//    bear->Attach(toy_shop_ptr);
//    toy_shop_ptr->AnnounceSales();
//
//    bear->StartSales();
//    toy_shop_ptr->AnnounceSales();
//
//    bear->Detach(toy_shop_ptr);
//    toy_shop_ptr->AnnounceSales();
//
//    bear->Attach(toy_shop_ptr);
//    toy_shop_ptr->AnnounceSales();

}