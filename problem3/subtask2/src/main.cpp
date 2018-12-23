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