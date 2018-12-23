#include <memory>
#include <mutex>
#include <iostream>
#include <cmath>
#include <thread>
#include <cstdlib>

#include "mall.h"


class TeddyBear: public IProduct {
public:
    TeddyBear(double price): IProduct("TeddyBear", price) {};
};


class RedTruck: public IProduct {
public:
    RedTruck(double price): IProduct("RedTruck", price) {};
};


class Barbi: public IProduct {
public:
    Barbi(double price): IProduct("Barbi", price) {};
};



class AnnouncingShop: public IShop {
    mutable std::mutex _on_print;
public:
    AnnouncingShop(std::string name): IShop(name) {};

    void AnnounceSales() {
        std::lock_guard<std::mutex> lock(_on_change);
        std::lock_guard<std::mutex> lock2(_on_print);

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
        if (on_sale) {
            if (_products_on_sale.find(prod_name) == _products_on_sale.end()) {
                std::lock_guard<std::mutex> lock(_on_print);
                std::cout << "The shop '" << _name << "' started sale '"
                          << prod_name << "' by price " << price << std::endl;
            } else {
                double old_price = _products_on_sale.at(prod_name);
                if (std::abs(old_price - price) < 1e-6) {
                    std::lock_guard<std::mutex> lock(_on_print);
                    std::cout << "The shop '" << _name << "' changed price '"
                              << prod_name << "' from "
                              << " to " << price << std::endl;
                }
            }
        } else {
            std::lock_guard<std::mutex> lock(_on_print);
            if (_products_on_sale.find(prod_name) != _products_on_sale.end()) {
                std::cout << "The shop '" << _name << "' stopped sale '"
                          << prod_name << std::endl;
            }
        }
        IShop::UpdateProduct(prod_name, on_sale, price);
    }
    void DeleteProduct(const std::string &prod_name) override {
        if (_products_on_sale.find(prod_name) != _products_on_sale.end()) {
            std::lock_guard<std::mutex> lock(_on_print);
            std::cout << "The shop '" << _name << "' stopped sale '"
                      << prod_name << std::endl;
        }

        IShop::DeleteProduct(prod_name);
    }

    ~AnnouncingShop() {
        std::cout << "The shop '" << _name << "' closed :( '" << std::endl;
    }
};


int main() {
    auto best_shop_ptr = std::make_shared<AnnouncingShop>(std::string("Best Toys"));
    auto boys_shop_ptr = std::make_shared<AnnouncingShop>(std::string("Super Cool Toys For Boys"));
    auto wonderland_ptr = std::make_shared<AnnouncingShop>(std::string("Wonderland"));



    std::thread product_thread([&]() {
        TeddyBear bear(15);
        bear.StartSales();
        bear.Attach(best_shop_ptr);
        bear.Attach(boys_shop_ptr);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        RedTruck truck(13);
        truck.StartSales();
        truck.Attach(wonderland_ptr);
        truck.Attach(best_shop_ptr);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        bear.Detach(boys_shop_ptr);
        truck.Detach(wonderland_ptr);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        truck.ChangePrice(12.99);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        bear.ChangePrice(16);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        Barbi barbi(45);
        barbi.Attach(best_shop_ptr);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    });

    std::thread shop_thread([&]() {
        for(int i = 0; i < 5; i++) {
            best_shop_ptr->AnnounceSales();
            boys_shop_ptr->AnnounceSales();
            wonderland_ptr->AnnounceSales();
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        best_shop_ptr.reset();
        boys_shop_ptr->AnnounceSales();
        wonderland_ptr->AnnounceSales();

        boys_shop_ptr.reset();
        wonderland_ptr->AnnounceSales();
    });

    product_thread.join();
    shop_thread.join();
}