#pragma once

#include <iostream>
#include <mutex>
#include <string>
#include <atomic>
#include <memory>
#include <shared_mutex>

// ===========================
// 单例模式的多种典型实现
// ===========================
// 为了对比不同实现方式，本文件中给出：
// 1）LazySingletonUnsafe     ：懒汉式，不加锁（多线程下不安全）
// 2）LazySingletonMutex      ：懒汉式，使用互斥量加锁，线程安全但有一定开销
// 3）LazySingletonDCL        ：懒汉式，双重检查锁定（DCL），C++11/14/17 优化版本
// 4）LazySingletonCallOnce   ：懒汉式，使用 std::call_once，线程安全（C++11+）
// 5）HungrySingleton          ：饿汉式，程序启动时就创建实例，线程天然安全
// 6）MeyersSingleton          ：C++11 推荐写法，函数内局部静态变量，线程安全
// 7）AtomicSingleton          ：使用原子操作的高性能单例（C++11+）
//
// 所有类都使用中文注释详细说明其设计意图和实现要点，方便对比学习。
//
// ===========================
// 线程安全与性能优化说明
// ===========================
// 1. 线程安全：单例模式在多线程环境下必须保证只创建一个实例
// 2. 性能优化：
//    - 避免每次访问都加锁（使用DCL、原子操作、局部静态变量）
//    - 考虑缓存行伪共享问题（使用 alignas）
//    - 智能指针替代裸指针，自动管理生命周期
// 3. 内存序：使用 std::atomic 时需要考虑 memory_order，防止指令重排
//
// ===========================
// C++ 标准版本说明
// ===========================
// C++11：引入 std::mutex, std::atomic, std::call_once，局部静态变量线程安全
// C++14：改进 std::make_unique
// C++17：内联变量（inline static）简化静态成员初始化，改进 std::shared_mutex
// C++20：std::atomic 改进，constinit 关键字
// C++23：进一步优化原子操作性能

// 1. 懒汉式（不加锁）—— 多线程环境下不安全
class LazySingletonUnsafe {
public:
    // 获取唯一实例的全局访问点
    static LazySingletonUnsafe* Instance() {
        // 第一次调用时才创建实例（延迟加载）
        if (instance_ == nullptr) {
            instance_ = new LazySingletonUnsafe();
        }
        return instance_;
    }

    void Log(const std::string& message) {
        std::cout << "[LazyUnsafe] " << message << std::endl;
    }

    // 禁止拷贝和赋值，防止产生多个实例
    LazySingletonUnsafe(const LazySingletonUnsafe&) = delete;
    LazySingletonUnsafe& operator=(const LazySingletonUnsafe&) = delete;

private:
    LazySingletonUnsafe() { std::cout << "LazySingletonUnsafe constructed" << std::endl; }
    ~LazySingletonUnsafe() = default;

    // 使用裸指针只是为了演示传统实现方式
    static LazySingletonUnsafe* instance_;
};

// 静态成员指针初始化
inline LazySingletonUnsafe* LazySingletonUnsafe::instance_ = nullptr;

// 2. 懒汉式（互斥量保护）—— 线程安全，但每次获取都要加锁
// 性能问题：每次访问都加锁，适合访问频率不高的场景
class LazySingletonMutex {
public:
    static LazySingletonMutex* Instance() {
        // 通过互斥量实现线程安全的延迟初始化
        std::lock_guard<std::mutex> lock(mutex_);
        if (instance_ == nullptr) {
            instance_ = new LazySingletonMutex();
        }
        return instance_;
    }

    void Log(const std::string& message) {
        std::cout << "[LazyMutex] " << message << std::endl;
    }

    LazySingletonMutex(const LazySingletonMutex&) = delete;
    LazySingletonMutex& operator=(const LazySingletonMutex&) = delete;

private:
    LazySingletonMutex() { std::cout << "LazySingletonMutex constructed" << std::endl; }
    ~LazySingletonMutex() = default;

    static LazySingletonMutex* instance_;
    static std::mutex mutex_;
};

// C++17: 使用 inline 简化静态成员定义
inline LazySingletonMutex* LazySingletonMutex::instance_ = nullptr;
inline std::mutex LazySingletonMutex::mutex_;

// 3. 懒汉式（双重检查锁定 DCL）—— 性能优化版本
// C++11/14/17: 使用 std::atomic 和 memory_order 保证线程安全
// 优点：只在首次创建时加锁，之后无锁访问
// 注意：必须使用 std::atomic 和正确的 memory_order，否则存在指令重排风险
class LazySingletonDCL {
public:
    static LazySingletonDCL* Instance() {
        // 第一次检查：无锁读取，快速路径
        LazySingletonDCL* tmp = instance_.load(std::memory_order_acquire);
        if (tmp == nullptr) {
            std::lock_guard<std::mutex> lock(mutex_);
            // 第二次检查：确保只创建一次
            tmp = instance_.load(std::memory_order_relaxed);
            if (tmp == nullptr) {
                tmp = new LazySingletonDCL();
                instance_.store(tmp, std::memory_order_release);
            }
        }
        return tmp;
    }

    void Log(const std::string& message) {
        std::cout << "[LazyDCL] " << message << std::endl;
    }

    LazySingletonDCL(const LazySingletonDCL&) = delete;
    LazySingletonDCL& operator=(const LazySingletonDCL&) = delete;

private:
    LazySingletonDCL() { std::cout << "LazySingletonDCL constructed" << std::endl; }
    ~LazySingletonDCL() = default;

    // C++11: 使用 std::atomic 防止指令重排
    static std::atomic<LazySingletonDCL*> instance_;
    static std::mutex mutex_;
};

inline std::atomic<LazySingletonDCL*> LazySingletonDCL::instance_{nullptr};
inline std::mutex LazySingletonDCL::mutex_;

// 4. 懒汉式（std::call_once）—— C++11 标准库提供的线程安全初始化
// 优点：代码简洁，标准库保证线程安全，性能优于简单加锁
class LazySingletonCallOnce {
public:
    static LazySingletonCallOnce& Instance() {
        // std::call_once 保证 Init 只执行一次
        std::call_once(initFlag_, &LazySingletonCallOnce::Init);
        return *instance_;
    }

    void Log(const std::string& message) {
        std::cout << "[LazyCallOnce] " << message << std::endl;
    }

    LazySingletonCallOnce(const LazySingletonCallOnce&) = delete;
    LazySingletonCallOnce& operator=(const LazySingletonCallOnce&) = delete;

private:
    LazySingletonCallOnce() { std::cout << "LazySingletonCallOnce constructed" << std::endl; }
    ~LazySingletonCallOnce() = default;

    static void Init() {
        instance_ = new LazySingletonCallOnce();
    }

    static LazySingletonCallOnce* instance_;
    static std::once_flag initFlag_;
};

inline LazySingletonCallOnce* LazySingletonCallOnce::instance_ = nullptr;
inline std::once_flag LazySingletonCallOnce::initFlag_;

// 5. 饿汉式单例 —— 在程序启动时就完成实例初始化
// 优点：实现简单，天然线程安全，访问无锁性能最优
// 缺点：如果实例很大且不一定用到，会浪费资源
// 适用场景：几乎确定会使用的全局对象
class HungrySingleton {
public:
    static HungrySingleton& Instance() {
        return instance_;
    }

    void Log(const std::string& message) {
        std::cout << "[Hungry] " << message << std::endl;
    }

    HungrySingleton(const HungrySingleton&) = delete;
    HungrySingleton& operator=(const HungrySingleton&) = delete;

private:
    HungrySingleton() { std::cout << "HungrySingleton constructed" << std::endl; }
    ~HungrySingleton() = default;

    // C++17: inline static 简化静态成员定义
    // 静态对象在程序启动时构造，天然线程安全
    static HungrySingleton instance_;
};

// C++17 之前需要在类外定义
inline HungrySingleton HungrySingleton::instance_;

/* C++17+ 可以直接在类内使用 inline static:
class HungrySingletonCpp17 {
public:
    static HungrySingletonCpp17& Instance() { return instance_; }
private:
    inline static HungrySingletonCpp17 instance_;
};
*/

// 6. Meyers Singleton（推荐写法）
// 使用函数内局部静态变量，C++11 标准保证其初始化是线程安全的
// C++11: 标准保证局部静态变量初始化的线程安全性（magic static）
// 优点：延迟初始化 + 线程安全 + 代码简洁 + 自动析构
// 性能：首次访问有轻微开销（编译器生成的线程安全检查），之后无开销
// 这是现代 C++ 中最推荐的单例实现方式
class MeyersSingleton {
public:
    static MeyersSingleton& Instance() {
        // C++11: magic static - 编译器保证线程安全的初始化
        // 相当于编译器自动生成了类似 std::call_once 的机制
        static MeyersSingleton instance; // 第一次调用时构造，之后复用
        return instance;
    }

    void Log(const std::string& message) {
        std::cout << "[Meyers] " << message << std::endl;
    }

    MeyersSingleton(const MeyersSingleton&) = delete;
    MeyersSingleton& operator=(const MeyersSingleton&) = delete;

private:
    MeyersSingleton() { std::cout << "MeyersSingleton constructed" << std::endl; }
    ~MeyersSingleton() = default;
};

// 7. 原子操作单例（高性能版本）
// C++11+: 使用互斥量保护的原子操作单例
// 优点：线程安全，性能较好
// 注意：std::atomic<std::shared_ptr<T>> 在某些平台上可能不被支持，
// 因为 shared_ptr 不是 trivially copyable 类型
class AtomicSingleton {
public:
    static std::shared_ptr<AtomicSingleton> Instance() {
        // 使用互斥量保护的双重检查锁定模式
        std::shared_ptr<AtomicSingleton> tmp = nullptr;
        {
            std::shared_lock<std::shared_mutex> lock(read_mutex_);
            tmp = instance_.lock();  // 使用 weak_ptr::lock() 获取 shared_ptr
        }
        
        if (tmp == nullptr) {
            std::lock_guard<std::mutex> lock(write_mutex_);
            tmp = instance_.lock();
            if (tmp == nullptr) {
                // 使用私有静态函数创建实例
                tmp = CreateInstance();
                instance_ = tmp;  // 存储为 weak_ptr
            }
        }
        return tmp;
    }

    void Log(const std::string& message) {
        std::cout << "[Atomic] " << message << std::endl;
    }

    AtomicSingleton(const AtomicSingleton&) = delete;
    AtomicSingleton& operator=(const AtomicSingleton&) = delete;

private:
    // 私有构造函数和析构函数
    AtomicSingleton() { std::cout << "AtomicSingleton constructed" << std::endl; }
    ~AtomicSingleton() = default;

    // 私有静态函数，用于创建实例，使用默认删除器
    static std::shared_ptr<AtomicSingleton> CreateInstance() {
        return std::shared_ptr<AtomicSingleton>(new AtomicSingleton(), [](AtomicSingleton* p) { delete p; });
    }

    static std::weak_ptr<AtomicSingleton> instance_;  // 使用 weak_ptr 存储实例
    static std::shared_mutex read_mutex_;             // 读取时使用的读写锁
    static std::mutex write_mutex_;                   // 写入时使用的互斥量
};

// C++17: inline 简化定义
inline std::weak_ptr<AtomicSingleton> AtomicSingleton::instance_;
inline std::shared_mutex AtomicSingleton::read_mutex_;
inline std::mutex AtomicSingleton::write_mutex_;

/* C++20 版本可以使用 std::atomic<std::shared_ptr> 的特化：
// C++20: std::atomic<std::shared_ptr> 提供了更好的性能
class AtomicSingletonCpp20 {
public:
    static std::shared_ptr<AtomicSingletonCpp20> Instance() {
        auto tmp = instance_.load();
        if (tmp == nullptr) {
            std::call_once(flag_, []() {
                instance_.store(std::make_shared<AtomicSingletonCpp20>());
            });
            tmp = instance_.load();
        }
        return tmp;
    }
private:
    inline static std::atomic<std::shared_ptr<AtomicSingletonCpp20>> instance_;
    inline static std::once_flag flag_;
};
*/
