#include "../../../src/behavioral/strategy/Strategy.h"
#include <gtest/gtest.h>

// 策略模式测试套件

// 测试折扣策略 - 无折扣
TEST(StrategyTest, NoDiscount_Calculate) {
    PriceCalculator calc(std::make_shared<NoDiscount>());
    EXPECT_EQ(calc.Calculate(100.0), 100.0);
}

// 测试折扣策略 - 百分比折扣
TEST(StrategyTest, PercentageDiscount_Calculate) {
    PriceCalculator calc(std::make_shared<PercentageDiscount>(0.2));
    EXPECT_EQ(calc.Calculate(100.0), 80.0); // 20% off
}

// 测试折扣策略 - 门槛折扣（未达到）
TEST(StrategyTest, ThresholdDiscount_BelowThreshold) {
    PriceCalculator calc(std::make_shared<ThresholdDiscount>(150.0, 20.0));
    EXPECT_EQ(calc.Calculate(100.0), 100.0);
}

// 测试折扣策略 - 门槛折扣（达到）
TEST(StrategyTest, ThresholdDiscount_AboveThreshold) {
    PriceCalculator calc(std::make_shared<ThresholdDiscount>(150.0, 20.0));
    EXPECT_EQ(calc.Calculate(200.0), 180.0);
}

// 测试策略切换
TEST(StrategyTest, PriceCalculator_SwitchStrategy) {
    PriceCalculator calc(std::make_shared<NoDiscount>());
    EXPECT_EQ(calc.Calculate(100.0), 100.0);
    
    calc.SetStrategy(std::make_shared<PercentageDiscount>(0.1));
    EXPECT_EQ(calc.Calculate(100.0), 90.0);
}

// 测试RunDiscountStrategyDemo
TEST(StrategyTest, RunDiscountStrategyDemo) {
    EXPECT_NO_THROW(RunDiscountStrategyDemo());
}

// 测试排序策略 - 升序
TEST(StrategyTest, AscSort_Sort) {
    std::vector<int> data{5, 2, 8, 1, 9};
    SortContext ctx(std::make_shared<AscSortStrategy>());
    
    ctx.Sort(data);
    EXPECT_EQ(data, std::vector<int>({1, 2, 5, 8, 9}));
}

// 测试排序策略 - 降序
TEST(StrategyTest, DescSort_Sort) {
    std::vector<int> data{5, 2, 8, 1, 9};
    SortContext ctx(std::make_shared<DescSortStrategy>());
    
    ctx.Sort(data);
    EXPECT_EQ(data, std::vector<int>({9, 8, 5, 2, 1}));
}

// 测试排序策略切换
TEST(StrategyTest, SortContext_SwitchStrategy) {
    std::vector<int> data{3, 1, 2};
    SortContext ctx(std::make_shared<AscSortStrategy>());
    
    ctx.Sort(data);
    EXPECT_EQ(data, std::vector<int>({1, 2, 3}));
    
    ctx.SetStrategy(std::make_shared<DescSortStrategy>());
    ctx.Sort(data);
    EXPECT_EQ(data, std::vector<int>({3, 2, 1}));
}

// 测试RunSortStrategyDemo
TEST(StrategyTest, RunSortStrategyDemo) {
    EXPECT_NO_THROW(RunSortStrategyDemo());
}

// 测试线程安全策略 - 10% off
TEST(StrategyTest, ThreadSafe_Strategy10PercentOff) {
    ThreadSafeStrategyContext ctx(std::make_shared<Strategy10PercentOff>());
    EXPECT_EQ(ctx.Execute(100.0), 90.0);
}

// 测试线程安全策略 - 20% off
TEST(StrategyTest, ThreadSafe_Strategy20PercentOff) {
    ThreadSafeStrategyContext ctx(std::make_shared<Strategy20PercentOff>());
    EXPECT_EQ(ctx.Execute(100.0), 80.0);
}

// 测试线程安全策略 - 50% off
TEST(StrategyTest, ThreadSafe_Strategy50PercentOff) {
    ThreadSafeStrategyContext ctx(std::make_shared<Strategy50PercentOff>());
    EXPECT_EQ(ctx.Execute(100.0), 50.0);
}

// 测试线程安全策略切换
TEST(StrategyTest, ThreadSafe_SwitchStrategy) {
    ThreadSafeStrategyContext ctx(std::make_shared<Strategy10PercentOff>());
    EXPECT_EQ(ctx.Execute(100.0), 90.0);
    
    ctx.SetStrategy(std::make_shared<Strategy20PercentOff>());
    EXPECT_EQ(ctx.Execute(100.0), 80.0);
}

// 测试RunThreadSafeStrategyDemo
TEST(StrategyTest, RunThreadSafeStrategyDemo) {
    EXPECT_NO_THROW(RunThreadSafeStrategyDemo());
}

// 测试无锁策略
TEST(StrategyTest, LockFree_Execute) {
    LockFreeStrategyContext ctx(std::make_shared<Strategy10PercentOff>());
    EXPECT_EQ(ctx.Execute(200.0), 180.0);
}

// 测试无锁策略切换
TEST(StrategyTest, LockFree_SwitchStrategy) {
    LockFreeStrategyContext ctx(std::make_shared<Strategy10PercentOff>());
    EXPECT_EQ(ctx.Execute(200.0), 180.0);
    
    ctx.SetStrategy(std::make_shared<Strategy50PercentOff>());
    EXPECT_EQ(ctx.Execute(200.0), 100.0);
}

// 测试RunLockFreeStrategyDemo
TEST(StrategyTest, RunLockFreeStrategyDemo) {
    EXPECT_NO_THROW(RunLockFreeStrategyDemo());
}
