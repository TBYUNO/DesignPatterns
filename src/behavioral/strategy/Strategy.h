#pragma once

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <mutex>
#include <shared_mutex>
#include <atomic>

// 策略模式（Strategy）C++ 示例
// -----------------------------
// 示例 1：折扣策略
// 示例 2：排序策略
// 示例 3：线程安全的策略切换（C++17+）
//
// ==========================
// 线程安全与性能优化
// ==========================
// 1. 线程安全问题：
//    - 策略切换过程中的并发访问
//    - 策略对象内部状态的线程安全
// 2. 性能优化：
//    - 策略对象无状态，通常不需要同步
//    - 使用 std::atomic 保存策略指针
//    - 读写锁优化频繁读取场景
//    - 策略对象缓存和复用
// 3. C++ 标准版本特性：
//    - C++11: std::function, lambda, std::atomic
//    - C++14: auto 返回类型
//    - C++17: std::shared_mutex, std::variant
//    - C++20: concepts 约束策略接口
//    - C++23: 模式匹配

// ===== 示例 1：折扣策略 =====

class IDiscountStrategy {
public:
    virtual ~IDiscountStrategy() = default;
    virtual double Calculate(double price) const = 0; // 根据原价计算折后价
};

class NoDiscount : public IDiscountStrategy {
public:
    double Calculate(double price) const override { return price; }
};

class PercentageDiscount : public IDiscountStrategy {
public:
    explicit PercentageDiscount(double percent) : percent_(percent) {}

    double Calculate(double price) const override { return price * (1.0 - percent_); }

private:
    double percent_{}; // 0.1 表示 10% 折扣
};

class ThresholdDiscount : public IDiscountStrategy {
public:
    ThresholdDiscount(double threshold, double minus)
        : threshold_(threshold), minus_(minus) {}

    double Calculate(double price) const override {
        if (price >= threshold_) {
            return price - minus_;
        }
        return price;
    }

private:
    double threshold_{};
    double minus_{};
};

// 上下文：价格计算器
class PriceCalculator {
public:
    explicit PriceCalculator(std::shared_ptr<IDiscountStrategy> strategy)
        : strategy_(std::move(strategy)) {}

    void SetStrategy(std::shared_ptr<IDiscountStrategy> strategy) {
        strategy_ = std::move(strategy);
    }

    double Calculate(double price) const {
        return strategy_ ? strategy_->Calculate(price) : price;
    }

private:
    std::shared_ptr<IDiscountStrategy> strategy_;
};

inline void RunDiscountStrategyDemo() {
    std::cout << "--- Discount Strategy Demo ---" << std::endl;

    PriceCalculator calc(std::make_shared<NoDiscount>());
    double price = 100.0;

    std::cout << "NoDiscount: " << calc.Calculate(price) << std::endl;

    calc.SetStrategy(std::make_shared<PercentageDiscount>(0.2));
    std::cout << "20% off: " << calc.Calculate(price) << std::endl;

    calc.SetStrategy(std::make_shared<ThresholdDiscount>(150.0, 20.0));
    std::cout << "Full 150 minus 20: " << calc.Calculate(price) << std::endl;
}

// ===== 示例 2：排序策略 =====

class ISortStrategy {
public:
    virtual ~ISortStrategy() = default;
    virtual void Sort(std::vector<int>& data) const = 0;
};

class AscSortStrategy : public ISortStrategy {
public:
    void Sort(std::vector<int>& data) const override {
        std::sort(data.begin(), data.end());
    }
};

class DescSortStrategy : public ISortStrategy {
public:
    void Sort(std::vector<int>& data) const override {
        std::sort(data.begin(), data.end(), std::greater<int>());
    }
};

class SortContext {
public:
    explicit SortContext(std::shared_ptr<ISortStrategy> strategy)
        : strategy_(std::move(strategy)) {}

    void SetStrategy(std::shared_ptr<ISortStrategy> strategy) {
        strategy_ = std::move(strategy);
    }

    void Sort(std::vector<int>& data) const {
        if (strategy_) {
            strategy_->Sort(data);
        }
    }

private:
    std::shared_ptr<ISortStrategy> strategy_;
};

inline void PrintVector(const std::vector<int>& data) {
    for (int v : data) {
        std::cout << v << " ";
    }
    std::cout << std::endl;
}

inline void RunSortStrategyDemo() {
    std::cout << "\n--- Sort Strategy Demo ---" << std::endl;

    std::vector<int> data{5, 1, 4, 2, 3};

    SortContext ctx(std::make_shared<AscSortStrategy>());
    ctx.Sort(data);
    std::cout << "Asc: ";
    PrintVector(data);

    ctx.SetStrategy(std::make_shared<DescSortStrategy>());
    ctx.Sort(data);
    std::cout << "Desc: ";
    PrintVector(data);
}

// ===== 示例 3：线程安全的策略切换 =====
// C++17: 使用 std::shared_mutex 实现线程安全的策略管理
// 优点：多线程可并发执行策略，切换策略时互斥

class IThreadSafeStrategy {
public:
    virtual ~IThreadSafeStrategy() = default;
    virtual double Execute(double value) const = 0;
};

class Strategy10PercentOff : public IThreadSafeStrategy {
public:
    double Execute(double value) const override {
        return value * 0.9;
    }
};

class Strategy20PercentOff : public IThreadSafeStrategy {
public:
    double Execute(double value) const override {
        return value * 0.8;
    }
};

class Strategy50PercentOff : public IThreadSafeStrategy {
public:
    double Execute(double value) const override {
        return value * 0.5;
    }
};

// 线程安全的策略上下文
class ThreadSafeStrategyContext {
public:
    explicit ThreadSafeStrategyContext(std::shared_ptr<IThreadSafeStrategy> strategy)
        : strategy_(std::move(strategy)) {}

    // 线程安全的策略切换
    void SetStrategy(std::shared_ptr<IThreadSafeStrategy> strategy) {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        strategy_ = std::move(strategy);
    }

    // 线程安全的策略执行
    double Execute(double value) const {
        std::shared_lock<std::shared_mutex> lock(mutex_);
        return strategy_ ? strategy_->Execute(value) : value;
    }

private:
    mutable std::shared_mutex mutex_; // C++17: 读写锁
    std::shared_ptr<IThreadSafeStrategy> strategy_;
};

inline void RunThreadSafeStrategyDemo() {
    std::cout << "\n--- Thread-Safe Strategy Demo ---" << std::endl;

    ThreadSafeStrategyContext ctx(std::make_shared<Strategy10PercentOff>());
    double price = 100.0;

    std::cout << "10% off: " << ctx.Execute(price) << std::endl;

    ctx.SetStrategy(std::make_shared<Strategy20PercentOff>());
    std::cout << "20% off: " << ctx.Execute(price) << std::endl;

    ctx.SetStrategy(std::make_shared<Strategy50PercentOff>());
    std::cout << "50% off: " << ctx.Execute(price) << std::endl;
}

// ===== 示例 4：无锁策略切换（C++11+）=====
// 使用 weak_ptr + shared_mutex 实现线程安全的策略切换
// 优点：避免 std::atomic<std::shared_ptr<T>> 不支持的问题

class LockFreeStrategyContext {
public:
    explicit LockFreeStrategyContext(std::shared_ptr<IThreadSafeStrategy> strategy) {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        strategy_ = strategy;
    }

    void SetStrategy(std::shared_ptr<IThreadSafeStrategy> strategy) {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        strategy_ = strategy;
    }

    double Execute(double value) const {
        std::shared_lock<std::shared_mutex> lock(mutex_);
        return strategy_ ? strategy_->Execute(value) : value;
    }

private:
    mutable std::shared_mutex mutex_;
    std::shared_ptr<IThreadSafeStrategy> strategy_;
};

inline void RunLockFreeStrategyDemo() {
    std::cout << "\n--- Lock-Free Strategy Demo ---" << std::endl;

    LockFreeStrategyContext ctx(std::make_shared<Strategy10PercentOff>());
    double price = 200.0;

    std::cout << "10% off: " << ctx.Execute(price) << std::endl;

    ctx.SetStrategy(std::make_shared<Strategy50PercentOff>());
    std::cout << "50% off: " << ctx.Execute(price) << std::endl;
}

/* C++20 版本：使用 concepts 约束策略接口
template<typename T>
concept DiscountStrategy = requires(T strategy, double price) {
    { strategy.Execute(price) } -> std::convertible_to<double>;
};

template<DiscountStrategy S>
class ConceptStrategyContext {
public:
    explicit ConceptStrategyContext(S strategy) : strategy_(std::move(strategy)) {}
    
    void SetStrategy(S strategy) {
        std::lock_guard<std::mutex> lock(mutex_);
        strategy_ = std::move(strategy);
    }
    
    double Execute(double value) const {
        std::lock_guard<std::mutex> lock(mutex_);
        return strategy_.Execute(value);
    }
    
private:
    mutable std::mutex mutex_;
    S strategy_;
};
*/