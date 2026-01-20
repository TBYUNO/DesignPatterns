#include "../../../src/creational/factory_method/FactoryMethod.h"
#include <gtest/gtest.h>
#include <thread>
#include <mutex>
#include <vector>
#include <memory>

// 工厂方法模式测试套件

// 测试具体工厂A创建产品A
TEST(FactoryMethodTest, ConcreteCreatorA_CreatesProductA) {
    ConcreteCreatorA creatorA;
    auto product = creatorA.CreateProduct();
    
    ASSERT_NE(product, nullptr);
    EXPECT_NO_THROW(product->Use());
}

// 测试具体工厂B创建产品B
TEST(FactoryMethodTest, ConcreteCreatorB_CreatesProductB) {
    ConcreteCreatorB creatorB;
    auto product = creatorB.CreateProduct();
    
    ASSERT_NE(product, nullptr);
    EXPECT_NO_THROW(product->Use());
}

// 测试工厂的业务操作
TEST(FactoryMethodTest, Creator_AnOperation) {
    ConcreteCreatorA creatorA;
    EXPECT_NO_THROW(creatorA.AnOperation());
    
    ConcreteCreatorB creatorB;
    EXPECT_NO_THROW(creatorB.AnOperation());
}

// 测试工厂多态性
TEST(FactoryMethodTest, FactoryPolymorphism) {
    std::unique_ptr<Creator> creator;
    
    // 使用工厂A
    creator = std::make_unique<ConcreteCreatorA>();
    auto productA = creator->CreateProduct();
    ASSERT_NE(productA, nullptr);
    
    // 切换到工厂B
    creator = std::make_unique<ConcreteCreatorB>();
    auto productB = creator->CreateProduct();
    ASSERT_NE(productB, nullptr);
}

// 测试产品注册表 - 注册功能
TEST(FactoryMethodTest, ProductRegistry_RegisterAndCreate) {
    InitProductRegistry();
    
    auto productA = ProductRegistry::Create("A");
    ASSERT_NE(productA, nullptr);
    
    EXPECT_NO_THROW(productA->Use());
    
    auto productB = ProductRegistry::Create("B");
    ASSERT_NE(productB, nullptr);
    EXPECT_NO_THROW(productB->Use());
}

// 测试产品注册表 - 不存在的产品
TEST(FactoryMethodTest, ProductRegistry_NonExistentProduct) {
    InitProductRegistry();
    
    auto product = ProductRegistry::Create("NonExistent");
    EXPECT_EQ(product, nullptr);
}

// 测试产品注册表 - 重复创建
TEST(FactoryMethodTest, ProductRegistry_MultipleCreations) {
    InitProductRegistry();
    
    auto product1 = ProductRegistry::Create("A");
    auto product2 = ProductRegistry::Create("A");
    
    ASSERT_NE(product1, nullptr);
    ASSERT_NE(product2, nullptr);
    // 两个产品应该是不同的实例
    EXPECT_NE(product1.get(), product2.get());
}

// 测试产品注册表的线程安全性
TEST(FactoryMethodTest, ProductRegistry_ThreadSafety) {
    InitProductRegistry();
    
    std::vector<std::thread> threads;
    std::vector<std::unique_ptr<Product>> products(10);
    
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([i, &products]() {
            products[i] = ProductRegistry::Create(i % 2 == 0 ? "A" : "B");
        });
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    // 验证所有产品都创建成功
    for (const auto& product : products) {
        EXPECT_NE(product, nullptr);
    }
}