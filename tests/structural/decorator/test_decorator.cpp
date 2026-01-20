#include "../../../src/structural/decorator/Decorator.h"
#include <gtest/gtest.h>

// 装饰器模式测试套件

// 测试基础饮料Espresso
TEST(DecoratorTest, Espresso_BasicFunctionality) {
    Espresso espresso;
    
    EXPECT_EQ(espresso.GetCost(), 10.0);
    EXPECT_EQ(espresso.GetDescription(), "Espresso");
}

// 测试基础饮料HouseBlend
TEST(DecoratorTest, HouseBlend_BasicFunctionality) {
    HouseBlend houseBlend;
    
    EXPECT_EQ(houseBlend.GetCost(), 8.0);
    EXPECT_EQ(houseBlend.GetDescription(), "House Blend");
}

// 测试单层装饰：Espresso + Milk
TEST(DecoratorTest, SingleDecorator_MilkOnEspresso) {
    std::shared_ptr<Beverage> beverage = std::make_shared<Espresso>();
    beverage = std::make_shared<MilkDecorator>(beverage);
    
    EXPECT_EQ(beverage->GetCost(), 12.0); // 10 + 2
    EXPECT_EQ(beverage->GetDescription(), "Espresso, Milk");
}

// 测试多层装饰：Espresso + Milk + Sugar
TEST(DecoratorTest, MultipleDecorators_MilkAndSugar) {
    std::shared_ptr<Beverage> beverage = std::make_shared<Espresso>();
    beverage = std::make_shared<MilkDecorator>(beverage);
    beverage = std::make_shared<SugarDecorator>(beverage);
    
    EXPECT_EQ(beverage->GetCost(), 13.0); // 10 + 2 + 1
    EXPECT_EQ(beverage->GetDescription(), "Espresso, Milk, Sugar");
}

// 测试多层装饰：Espresso + Milk + Sugar + Whip
TEST(DecoratorTest, TripleDecorators) {
    std::shared_ptr<Beverage> beverage = std::make_shared<Espresso>();
    beverage = std::make_shared<MilkDecorator>(beverage);
    beverage = std::make_shared<SugarDecorator>(beverage);
    beverage = std::make_shared<WhipDecorator>(beverage);
    
    EXPECT_EQ(beverage->GetCost(), 16.0); // 10 + 2 + 1 + 3
    EXPECT_EQ(beverage->GetDescription(), "Espresso, Milk, Sugar, Whip");
}

// 测试不同基础饮料的装饰
TEST(DecoratorTest, DifferentBaseBeverages) {
    // HouseBlend + Milk
    std::shared_ptr<Beverage> house = std::make_shared<HouseBlend>();
    house = std::make_shared<MilkDecorator>(house);
    
    EXPECT_EQ(house->GetCost(), 10.0); // 8 + 2
    EXPECT_EQ(house->GetDescription(), "House Blend, Milk");
    
    // Espresso + Whip
    std::shared_ptr<Beverage> espresso = std::make_shared<Espresso>();
    espresso = std::make_shared<WhipDecorator>(espresso);
    
    EXPECT_EQ(espresso->GetCost(), 13.0); // 10 + 3
    EXPECT_EQ(espresso->GetDescription(), "Espresso, Whip");
}

// 测试RunSimpleDecoratorDemo
TEST(DecoratorTest, RunSimpleDecoratorDemo) {
    EXPECT_NO_THROW(RunSimpleDecoratorDemo());
}

// 测试RunMultipleBaseDemo
TEST(DecoratorTest, RunMultipleBaseDemo) {
    EXPECT_NO_THROW(RunMultipleBaseDemo());
}

// 测试线程安全的装饰器
TEST(DecoratorTest, ThreadSafeDecorator_BasicFunctionality) {
    std::shared_ptr<IThreadSafeBeverage> beverage = std::make_shared<ThreadSafeEspresso>();
    
    EXPECT_EQ(beverage->GetCost(), 10.0);
    EXPECT_EQ(beverage->GetDescription(), "Espresso");
}

// 测试带缓存的装饰器
TEST(DecoratorTest, CachedDecorator_Caching) {
    std::shared_ptr<IThreadSafeBeverage> beverage = std::make_shared<ThreadSafeEspresso>();
    beverage = std::make_shared<CachedMilkDecorator>(beverage);
    
    // 第一次调用
    double cost1 = beverage->GetCost();
    EXPECT_EQ(cost1, 12.0);
    
    // 第二次调用应该使用缓存
    double cost2 = beverage->GetCost();
    EXPECT_EQ(cost2, 12.0);
}

// 测试RunThreadSafeDecoratorDemo
TEST(DecoratorTest, RunThreadSafeDecoratorDemo) {
    EXPECT_NO_THROW(RunThreadSafeDecoratorDemo());
}

// 测试装饰器链的组合
TEST(DecoratorTest, DecoratorChain_Combinations) {
    // 测试不同的装饰顺序
    std::shared_ptr<Beverage> bev1 = std::make_shared<Espresso>();
    bev1 = std::make_shared<SugarDecorator>(bev1);
    bev1 = std::make_shared<MilkDecorator>(bev1);
    EXPECT_EQ(bev1->GetCost(), 13.0); // 10 + 1 + 2
    
    std::shared_ptr<Beverage> bev2 = std::make_shared<Espresso>();
    bev2 = std::make_shared<MilkDecorator>(bev2);
    bev2 = std::make_shared<SugarDecorator>(bev2);
    EXPECT_EQ(bev2->GetCost(), 13.0); // 10 + 2 + 1
    
    // 价格相同但描述顺序不同
    EXPECT_NE(bev1->GetDescription(), bev2->GetDescription());
}

// 测试重复装饰
TEST(DecoratorTest, RepeatedDecorators) {
    std::shared_ptr<Beverage> beverage = std::make_shared<Espresso>();
    beverage = std::make_shared<MilkDecorator>(beverage);
    beverage = std::make_shared<MilkDecorator>(beverage); // 双倍牛奶
    
    EXPECT_EQ(beverage->GetCost(), 14.0); // 10 + 2 + 2
    EXPECT_EQ(beverage->GetDescription(), "Espresso, Milk, Milk");
}
