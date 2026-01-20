#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <mutex>
#include <shared_mutex>

// 装饰器模式（Decorator）C++ 示例
// --------------------------------
// 本文件以“咖啡加料”为例演示装饰器模式：
// - Beverage         ：抽象饮料，统一接口
// - Espresso/HouseBlend：具体基础饮料
// - BeverageDecorator：抽象装饰器，持有一个 Beverage 指针
// - MilkDecorator/SugarDecorator/WhipDecorator：为饮料动态叠加功能
//
// ==========================
// 线程安全与性能优化
// ==========================
// 1. 线程安全问题：
//    - 装饰器链的并发访问
//    - 多个线程同时添加装饰器
//    - 装饰器内部状态的线程安全
// 2. 性能优化：
//    - 装饰器通常无状态，读操作不需同步
//    - 使用读写锁优化多读场景
//    - 缓存计算结果减少重复计算
//    - 装饰器对象池复用
// 3. C++ 标准版本特性：
//    - C++11: std::shared_ptr, std::mutex
//    - C++14: std::make_unique
//    - C++17: std::shared_mutex, std::optional
//    - C++20: concepts 约束装饰器接口
//    - C++23: std::expected

// 抽象构件：饮料
class Beverage {
public:
    virtual ~Beverage() = default;

    // 获取价格（单位：元，仅为示例）
    virtual double GetCost() const = 0;

    // 获取描述，用于展示当前组合
    virtual std::string GetDescription() const = 0;
};

// 具体构件：浓缩咖啡
class Espresso : public Beverage {
public:
    double GetCost() const override { return 10.0; }
    std::string GetDescription() const override { return "Espresso"; }
};

// 具体构件：综合咖啡
class HouseBlend : public Beverage {
public:
    double GetCost() const override { return 8.0; }
    std::string GetDescription() const override { return "House Blend"; }
};

// 抽象装饰器：持有一个 Beverage，被装饰对象
class BeverageDecorator : public Beverage {
public:
    explicit BeverageDecorator(std::shared_ptr<Beverage> beverage)
        : beverage_(std::move(beverage)) {}

protected:
    std::shared_ptr<Beverage> beverage_;
};

// 具体装饰器：牛奶
class MilkDecorator : public BeverageDecorator {
public:
    using BeverageDecorator::BeverageDecorator;

    double GetCost() const override {
        // 在原有饮料价格基础上增加牛奶费用
        return beverage_->GetCost() + 2.0;
    }

    std::string GetDescription() const override {
        return beverage_->GetDescription() + ", Milk";
    }
};

// 具体装饰器：糖
class SugarDecorator : public BeverageDecorator {
public:
    using BeverageDecorator::BeverageDecorator;

    double GetCost() const override {
        return beverage_->GetCost() + 1.0;
    }

    std::string GetDescription() const override {
        return beverage_->GetDescription() + ", Sugar";
    }
};

// 具体装饰器：奶泡
class WhipDecorator : public BeverageDecorator {
public:
    using BeverageDecorator::BeverageDecorator;

    double GetCost() const override {
        return beverage_->GetCost() + 3.0;
    }

    std::string GetDescription() const override {
        return beverage_->GetDescription() + ", Whip";
    }
};

// 示例 1：为一杯 Espresso 叠加多种配料
inline void RunSimpleDecoratorDemo() {
    std::cout << "--- Simple Decorator Demo ---" << std::endl;

    std::shared_ptr<Beverage> beverage = std::make_shared<Espresso>();
    beverage = std::make_shared<MilkDecorator>(beverage);  // 加牛奶
    beverage = std::make_shared<SugarDecorator>(beverage); // 再加糖

    std::cout << beverage->GetDescription() << " costs " << beverage->GetCost() << " RMB" << std::endl;
}

// 示例 2：不同基础饮料 + 不同装饰组合
inline void RunMultipleBaseDemo() {
    std::cout << "\n--- Multiple Base Decorator Demo ---" << std::endl;

    // HouseBlend + Milk + Sugar
    std::shared_ptr<Beverage> house = std::make_shared<HouseBlend>();
    house = std::make_shared<MilkDecorator>(house);
    house = std::make_shared<SugarDecorator>(house);
    std::cout << house->GetDescription() << " costs " << house->GetCost() << " RMB" << std::endl;

    // Espresso + Whip
    std::shared_ptr<Beverage> espresso = std::make_shared<Espresso>();
    espresso = std::make_shared<WhipDecorator>(espresso);
    std::cout << espresso->GetDescription() << " costs " << espresso->GetCost() << " RMB" << std::endl;
}

// ===== 示例 3：线程安全的装饰器（C++17+） =====
// 为装饰器添加线程安全支持，适用于需要缓存的场景

class IThreadSafeBeverage {
public:
    virtual ~IThreadSafeBeverage() = default;
    virtual double GetCost() const = 0;
    virtual std::string GetDescription() const = 0;
};

class ThreadSafeEspresso : public IThreadSafeBeverage {
public:
    double GetCost() const override { return 10.0; }
    std::string GetDescription() const override { return "Espresso"; }
};

// 线程安全的装饰器基类
class ThreadSafeBeverageDecorator : public IThreadSafeBeverage {
public:
    explicit ThreadSafeBeverageDecorator(std::shared_ptr<IThreadSafeBeverage> beverage)
        : beverage_(std::move(beverage)) {}

protected:
    std::shared_ptr<IThreadSafeBeverage> beverage_;
    mutable std::shared_mutex mutex_; // C++17: 读写锁
};

// 带缓存的线程安全装饰器
class CachedMilkDecorator : public ThreadSafeBeverageDecorator {
public:
    using ThreadSafeBeverageDecorator::ThreadSafeBeverageDecorator;

    double GetCost() const override {
        // 先尝试读锁获取缓存
        {
            std::shared_lock<std::shared_mutex> readLock(mutex_);
            if (cached_) {
                return cachedCost_;
            }
        }

        // 缓存未命中，使用写锁计算并缓存
        std::unique_lock<std::shared_mutex> writeLock(mutex_);
        if (!cached_) { // 双重检查
            cachedCost_ = beverage_->GetCost() + 2.0;
            cached_ = true;
        }
        return cachedCost_;
    }

    std::string GetDescription() const override {
        std::shared_lock<std::shared_mutex> lock(mutex_);
        return beverage_->GetDescription() + ", Milk";
    }

private:
    mutable bool cached_{false};
    mutable double cachedCost_{0.0};
};

class CachedSugarDecorator : public ThreadSafeBeverageDecorator {
public:
    using ThreadSafeBeverageDecorator::ThreadSafeBeverageDecorator;

    double GetCost() const override {
        std::shared_lock<std::shared_mutex> lock(mutex_);
        return beverage_->GetCost() + 1.0;
    }

    std::string GetDescription() const override {
        std::shared_lock<std::shared_mutex> lock(mutex_);
        return beverage_->GetDescription() + ", Sugar";
    }
};

inline void RunThreadSafeDecoratorDemo() {
    std::cout << "\n--- Thread-Safe Decorator Demo ---" << std::endl;

    std::shared_ptr<IThreadSafeBeverage> beverage = std::make_shared<ThreadSafeEspresso>();
    beverage = std::make_shared<CachedMilkDecorator>(beverage);
    beverage = std::make_shared<CachedSugarDecorator>(beverage);

    // 多次调用会命中缓存
    std::cout << beverage->GetDescription() << " costs " << beverage->GetCost() << " RMB" << std::endl;
    std::cout << beverage->GetDescription() << " costs " << beverage->GetCost() << " RMB (cached)" << std::endl;
}

/* C++20 版本：使用 concepts 约束装饰器接口
template<typename T>
concept BeverageType = requires(const T& t) {
    { t.GetCost() } -> std::convertible_to<double>;
    { t.GetDescription() } -> std::convertible_to<std::string>;
};

template<BeverageType Base>
class ConceptDecorator {
public:
    explicit ConceptDecorator(std::shared_ptr<Base> base)
        : base_(std::move(base)) {}
    
    double GetCost() const {
        return base_->GetCost() + extraCost_;
    }
    
    std::string GetDescription() const {
        return base_->GetDescription() + extraDesc_;
    }
    
private:
    std::shared_ptr<Base> base_;
    double extraCost_{0.0};
    std::string extraDesc_;
};
*/
