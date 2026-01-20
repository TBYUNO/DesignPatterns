#pragma once

#include <iostream>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <functional>

// 工厂方法模式（Factory Method）示例
// ----------------------------------
// 角色说明：
// - Product           ：抽象产品，定义产品对外统一接口
// - ConcreteProductA/B：具体产品，实现不同的业务行为
// - Creator           ：抽象工厂，声明工厂方法 CreateProduct
// - ConcreteCreatorA/B：具体工厂，决定实例化哪一种具体产品
//
// 与简单工厂相比，工厂方法将“创建什么产品”的逻辑下放到子类，
// 当新增产品时，只需要新增对应的具体工厂类即可，更符合开闭原则。
//
// =========================
// 线程安全与性能优化
// =========================
// 1. 线程安全：工厂方法本身是无状态的，通常不需要额外的线程同步
// 2. 性能优化：
//    - 使用对象池复用对象
//    - 缓存工厂实例（工厂单例化）
//    - 使用类型注册表替代多态（C++11+）
// 3. C++ 标准版本特性：
//    - C++11: std::unique_ptr, std::make_unique (C++14), lambda
//    - C++14: auto 返回类型推导
//    - C++17: std::optional 用于可选产品
//    - C++20: concepts 约束工厂接口
//    - C++23: 模式匹配优化

// 抽象产品
class Product {
public:
    virtual ~Product() = default;

    // 抽象业务接口：不同产品有不同实现
    virtual void Use() = 0;
};

// 具体产品 A
class ConcreteProductA : public Product {
public:
    void Use() override {
        std::cout << "Use ConcreteProductA" << std::endl;
    }
};

// 具体产品 B
class ConcreteProductB : public Product {
public:
    void Use() override {
        std::cout << "Use ConcreteProductB" << std::endl;
    }
};

// 抽象工厂（工厂方法所在的基类）
class Creator {
public:
    virtual ~Creator() = default;

    // 工厂方法：由子类决定具体创建哪种产品
    virtual std::unique_ptr<Product> CreateProduct() const = 0;

    // 通用业务逻辑：依赖抽象产品，不关心具体产品类型
    void AnOperation() const {
        auto product = CreateProduct();
        product->Use();
    }
};

// 具体工厂 A：创建 ConcreteProductA
class ConcreteCreatorA : public Creator {
public:
    std::unique_ptr<Product> CreateProduct() const override {
        return std::make_unique<ConcreteProductA>();
    }
};

// 具体工厂 B：创建 ConcreteProductB
class ConcreteCreatorB : public Creator {
public:
    std::unique_ptr<Product> CreateProduct() const override {
        return std::make_unique<ConcreteProductB>();
    }
};

// 演示函数：客户端只依赖抽象 Creator 和 Product
inline void RunFactoryMethodDemo() {
    std::unique_ptr<Creator> creatorA = std::make_unique<ConcreteCreatorA>();
    std::unique_ptr<Creator> creatorB = std::make_unique<ConcreteCreatorB>();

    std::cout << "FactoryMethod: use creatorA" << std::endl;
    creatorA->AnOperation();

    std::cout << "FactoryMethod: use creatorB" << std::endl;
    creatorB->AnOperation();
}

// ====================================
// 性能优化：工厂注册表模式（C++11+）
// ====================================
// 使用函数指针或 lambda 注册工厂，避免为每个产品创建工厂类
// 优点：灵活、减少类层次、支持动态注册

class ProductRegistry {
public:
    using FactoryFunction = std::function<std::unique_ptr<Product>()>;

    // 注册产品工厂函数
    static void Register(const std::string& type, FactoryFunction factory) {
        std::lock_guard<std::mutex> lock(mutex_);
        registry_[type] = std::move(factory);
    }

    // 根据类型创建产品
    static std::unique_ptr<Product> Create(const std::string& type) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = registry_.find(type);
        if (it != registry_.end()) {
            return it->second();
        }
        return nullptr;
    }

private:
    // C++17: inline static 简化静态成员定义
    inline static std::unordered_map<std::string, FactoryFunction> registry_;
    inline static std::mutex mutex_; // 线程安全保护
};

// 注册示例（可在程序初始化时执行）
inline void InitProductRegistry() {
    // C++11: lambda 表达式注册工厂
    ProductRegistry::Register("A", []() -> std::unique_ptr<Product> {
        return std::make_unique<ConcreteProductA>();
    });
    
    ProductRegistry::Register("B", []() -> std::unique_ptr<Product> {
        return std::make_unique<ConcreteProductB>();
    });
}

// 使用注册表模式
inline void RunRegistryDemo() {
    std::cout << "\n--- Factory Registry Demo ---" << std::endl;
    
    InitProductRegistry();
    
    auto productA = ProductRegistry::Create("A");
    if (productA) productA->Use();
    
    auto productB = ProductRegistry::Create("B");
    if (productB) productB->Use();
}

/* C++20 版本：使用 concepts 约束工厂接口
template<typename T>
concept ProductFactory = requires(T t) {
    { t.CreateProduct() } -> std::convertible_to<std::unique_ptr<Product>>;
};

template<ProductFactory Factory>
void ProcessFactory(const Factory& factory) {
    auto product = factory.CreateProduct();
    product->Use();
}
*/
