#include "../../../src/creational/singleton/Singletons.h"  // 更新头文件路径
#include "gtest/gtest.h"
#include <thread>
#include <mutex>
#include <vector>

// 测试懒汉式单例（不安全版本）
TEST(SingletonTest, LazySingletonUnsafe) {
    auto* instance1 = LazySingletonUnsafe::Instance();
    auto* instance2 = LazySingletonUnsafe::Instance();
    
    EXPECT_EQ(instance1, instance2);  // 两个实例应该是同一个
    
    instance1->Log("Test message from instance1");
    instance2->Log("Test message from instance2");
}

// 测试懒汉式单例（互斥量保护版本）
TEST(SingletonTest, LazySingletonMutex) {
    auto* instance1 = LazySingletonMutex::Instance();
    auto* instance2 = LazySingletonMutex::Instance();
    
    EXPECT_EQ(instance1, instance2);  // 两个实例应该是同一个
    
    instance1->Log("Test message from instance1");
    instance2->Log("Test message from instance2");
}

// 测试懒汉式单例（双重检查锁定版本）
TEST(SingletonTest, LazySingletonDCL) {
    auto* instance1 = LazySingletonDCL::Instance();
    auto* instance2 = LazySingletonDCL::Instance();
    
    EXPECT_EQ(instance1, instance2);  // 两个实例应该是同一个
    
    instance1->Log("Test message from instance1");
    instance2->Log("Test message from instance2");
}

// 测试懒汉式单例（call_once版本）
TEST(SingletonTest, LazySingletonCallOnce) {
    auto& instance1 = LazySingletonCallOnce::Instance();
    auto& instance2 = LazySingletonCallOnce::Instance();
    
    EXPECT_EQ(&instance1, &instance2);  // 两个实例应该是同一个
    
    instance1.Log("Test message from instance1");
    instance2.Log("Test message from instance2");
}

// 测试饿汉式单例
TEST(SingletonTest, HungrySingleton) {
    auto& instance1 = HungrySingleton::Instance();
    auto& instance2 = HungrySingleton::Instance();
    
    EXPECT_EQ(&instance1, &instance2);  // 两个实例应该是同一个
    
    instance1.Log("Test message from instance1");
    instance2.Log("Test message from instance2");
}

// 测试Meyers单例（推荐写法）
TEST(SingletonTest, MeyersSingleton) {
    auto& instance1 = MeyersSingleton::Instance();
    auto& instance2 = MeyersSingleton::Instance();
    
    EXPECT_EQ(&instance1, &instance2);  // 两个实例应该是同一个
    
    instance1.Log("Test message from instance1");
    instance2.Log("Test message from instance2");
}

// 测试原子操作单例
TEST(SingletonTest, AtomicSingleton) {
    auto instance1 = AtomicSingleton::Instance();
    auto instance2 = AtomicSingleton::Instance();
    
    EXPECT_EQ(instance1.get(), instance2.get());  // 两个实例应该是同一个
    
    instance1->Log("Test message from instance1");
    instance2->Log("Test message from instance2");
}

// 线程安全测试
TEST(SingletonTest, ThreadSafety) {
    const int num_threads = 10;
    std::vector<std::thread> threads;
    std::vector<MeyersSingleton*> instances;
    std::mutex mtx;

    // 启动多个线程获取单例实例
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&instances, &mtx]() {
            auto& instance = MeyersSingleton::Instance();
            std::lock_guard<std::mutex> lock(mtx);
            instances.push_back(&instance);
        });
    }

    // 等待所有线程完成
    for (auto& t : threads) {
        t.join();
    }

    // 验证所有线程获取的实例都是同一个
    ASSERT_FALSE(instances.empty());
    auto* base_instance = instances[0];
    for (auto* instance : instances) {
        EXPECT_EQ(instance, base_instance);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}