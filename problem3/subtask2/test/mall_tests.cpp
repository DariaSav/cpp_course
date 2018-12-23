#include <memory>
#include <string>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <mall.h>


//===========================Simple operation on IProduct======================


TEST(IProductSimpleOp, InitialValues) {
    IProduct prod = IProduct("prod1", 1.23);

    ASSERT_STREQ("prod1", prod.GetUniqueName().c_str());
    EXPECT_DOUBLE_EQ(1.23, prod.GetPrice());
    EXPECT_FALSE(prod.OnSale());
}


TEST(IProductSimpleOp, ChangePrice) {
    IProduct prod = IProduct("prod1", 1);

    EXPECT_DOUBLE_EQ(1, prod.GetPrice());
    prod.ChangePrice(2.345);
    EXPECT_DOUBLE_EQ(2.345, prod.GetPrice());

    prod.ChangePrice(6.7);
    EXPECT_DOUBLE_EQ(6.7, prod.GetPrice());
}


TEST(IProductSimpleOp, ChangeOnSale) {
    IProduct prod = IProduct("prod1", 1);

    EXPECT_FALSE(prod.OnSale());
    prod.StartSales();
    EXPECT_TRUE(prod.OnSale());
    prod.StopSales();
    EXPECT_FALSE(prod.OnSale());
}


TEST(IProductSimpleOp, Attach) {
    IProduct prod = IProduct("prod1", 1);
    auto shop = std::make_shared<IShop>("shop1");

    prod.Attach(shop);
}


TEST(IProductSimpleOp, Detach) {
    IProduct prod = IProduct("prod1", 1);
    auto shop = std::make_shared<IShop>("shop1");

    prod.Detach(shop);
}


TEST(IProductSimpleOp, AttachDetach) {
    IProduct prod = IProduct("prod1", 1);
    auto shop = std::make_shared<IShop>("shop1");

    prod.Attach(shop);
    prod.Detach(shop);

    prod.Attach(shop);
    prod.Detach(shop);

    prod.Attach(shop);
    prod.Attach(shop);
    prod.Detach(shop);
}


//===========================Simple operation on IShop=========================


TEST(IShopSimpleOp, InitialValues) {
    auto shop = std::make_shared<IShop>("shop1");
    ASSERT_STREQ("shop1", shop->GetName().c_str());
    ASSERT_EQ(0, shop->GetProductOnSale().size());
}


//========================IProduct-IShop interaction===========================


TEST(IProductIShopInteraction, OneProduct) {
    auto shop = std::make_shared<IShop>("shop1");
    IProduct prod = IProduct("prod1", 1);

    ASSERT_EQ(0, shop->GetProductOnSale().size());

    prod.Attach(shop);
    ASSERT_EQ(0, shop->GetProductOnSale().size());

    prod.StartSales();
    auto prod_on_sales = shop->GetProductOnSale();
    ASSERT_EQ(1, prod_on_sales.size());
    ASSERT_TRUE(prod_on_sales.find(prod.GetUniqueName()) != prod_on_sales.end());
    EXPECT_DOUBLE_EQ(prod.GetPrice(), prod_on_sales[prod.GetUniqueName()]);

    prod.Detach(shop);
    ASSERT_EQ(0, shop->GetProductOnSale().size());

    prod.Attach(shop);
    ASSERT_EQ(1, shop->GetProductOnSale().size());

    prod.StopSales();
    ASSERT_EQ(0, shop->GetProductOnSale().size());
}


TEST(IProductIShopInteraction, AttachAfterStartSales) {
    auto shop = std::make_shared<IShop>("shop1");
    IProduct prod = IProduct("prod1", 1);

    ASSERT_EQ(0, shop->GetProductOnSale().size());

    prod.StartSales();
    prod.Attach(shop);

    const auto &prod_on_sales = shop->GetProductOnSale();
    ASSERT_EQ(1, prod_on_sales.size());
    ASSERT_TRUE(prod_on_sales.find(prod.GetUniqueName()) != prod_on_sales.end());
    EXPECT_DOUBLE_EQ(1, prod_on_sales.at(prod.GetUniqueName()));
}



TEST(IProductIShopInteraction, ChangePrice) {
    auto shop = std::make_shared<IShop>("shop1");
    IProduct prod = IProduct("prod1", 1);

    prod.Attach(shop);
    prod.StartSales();

    EXPECT_DOUBLE_EQ(1, shop->GetProductOnSale().at(prod.GetUniqueName()));

    prod.ChangePrice(2);
    EXPECT_DOUBLE_EQ(2, shop->GetProductOnSale().at(prod.GetUniqueName()));

    prod.ChangePrice(3);
    EXPECT_DOUBLE_EQ(3, shop->GetProductOnSale().at(prod.GetUniqueName()));
}


TEST(IProductIShopInteraction, RemovingShop) {
    auto shop = std::make_shared<IShop>("shop");
    IProduct prod = IProduct("prod", 1);

    prod.Attach(shop);
    prod.StartSales();

    shop.reset();

    prod.ChangePrice(2);
    prod.StopSales();
}


TEST(IProductIShopInteraction, SeveralProduct) {
    auto shop = std::make_shared<IShop>("shop");

    // one product
    IProduct teddy_bear = IProduct("teddy_bear", 10);
    teddy_bear.Attach(shop);
    teddy_bear.StartSales();

    ASSERT_EQ(1, shop->GetProductOnSale().size());
    EXPECT_DOUBLE_EQ(10, shop->GetProductOnSale().at(teddy_bear.GetUniqueName()));

    // two products

    IProduct doll = IProduct("doll", 5);
    doll.StartSales();
    doll.Attach(shop);

    const auto &prods = shop->GetProductOnSale();
    ASSERT_EQ(2, prods.size());
    EXPECT_DOUBLE_EQ(10, prods.at(teddy_bear.GetUniqueName()));
    EXPECT_DOUBLE_EQ(5, prods.at(doll.GetUniqueName()));

    // three products

    IProduct car = IProduct("car", 7);
    car.StartSales();
    car.Attach(shop);

    doll.ChangePrice(8);

    const auto &prods_2 = shop->GetProductOnSale();
    ASSERT_EQ(3, prods_2.size());
    EXPECT_DOUBLE_EQ(10, prods_2.at(teddy_bear.GetUniqueName()));
    EXPECT_DOUBLE_EQ(8, prods_2.at(doll.GetUniqueName()));
    EXPECT_DOUBLE_EQ(7, prods_2.at(car.GetUniqueName()));

    // one products

    teddy_bear.ChangePrice(15);
    doll.StopSales();
    car.Detach(shop);

    ASSERT_EQ(1, shop->GetProductOnSale().size());
    EXPECT_DOUBLE_EQ(15, shop->GetProductOnSale().at(teddy_bear.GetUniqueName()));
}


TEST(IProductIShopInteraction, SeveralProductSeveralShops) {
    auto best_shop = std::make_shared<IShop>("best toys");
    auto super_shop = std::make_shared<IShop>("super toys");
    auto cool_shop = std::make_shared<IShop>("cool toys");

    // one product
    IProduct teddy_bear = IProduct("teddy_bear", 10);
    teddy_bear.Attach(best_shop);
    teddy_bear.StartSales();

    ASSERT_EQ(1, best_shop->GetProductOnSale().size());
    ASSERT_EQ(0, super_shop->GetProductOnSale().size());
    ASSERT_EQ(0, cool_shop->GetProductOnSale().size());


    teddy_bear.Attach(super_shop);
    ASSERT_EQ(1, best_shop->GetProductOnSale().size());
    ASSERT_EQ(1, super_shop->GetProductOnSale().size());
    ASSERT_EQ(0, cool_shop->GetProductOnSale().size());

    teddy_bear.ChangePrice(15);
    teddy_bear.Attach(cool_shop);
    teddy_bear.Detach(super_shop);

    ASSERT_EQ(1, best_shop->GetProductOnSale().size());
    ASSERT_EQ(0, super_shop->GetProductOnSale().size());
    ASSERT_EQ(1, cool_shop->GetProductOnSale().size());
    EXPECT_DOUBLE_EQ(15, best_shop->GetProductOnSale().at(teddy_bear.GetUniqueName()));
    EXPECT_DOUBLE_EQ(15, cool_shop->GetProductOnSale().at(teddy_bear.GetUniqueName()));

    IProduct doll = IProduct("doll", 5);
    doll.Attach(best_shop);
    doll.StartSales();
    teddy_bear.StopSales();

    ASSERT_EQ(1, best_shop->GetProductOnSale().size());
    ASSERT_EQ(0, super_shop->GetProductOnSale().size());
    ASSERT_EQ(0, cool_shop->GetProductOnSale().size());
    EXPECT_DOUBLE_EQ(5, best_shop->GetProductOnSale().at(doll.GetUniqueName()));

}
