#include "Singletons.h"

// 本示例的 main 函数只负责演示不同单例实现的使用方式
// 具体实现细节都在 Singletons.h 中，便于在工程中复用和对比。
int main() {
    // 1. 懒汉式（不加锁）—— 在单线程环境下可以正常工作，但多线程下可能产生多个实例
    LazySingletonUnsafe* s1 = LazySingletonUnsafe::Instance();
    LazySingletonUnsafe* s2 = LazySingletonUnsafe::Instance();
    s1->Log("第一次调用 LazySingletonUnsafe");
    s2->Log("第二次调用 LazySingletonUnsafe（同一实例）");
    std::cout << "LazySingletonUnsafe address: " << s1 << " , " << s2 << "\n\n";

    // 2. 懒汉式 + 互斥量—— 每次获取实例都要加锁，保证线程安全
    LazySingletonMutex* m1 = LazySingletonMutex::Instance();
    LazySingletonMutex* m2 = LazySingletonMutex::Instance();
    m1->Log("第一次调用 LazySingletonMutex");
    m2->Log("第二次调用 LazySingletonMutex（同一实例）");
    std::cout << "LazySingletonMutex address: " << m1 << " , " << m2 << "\n\n";

    // 3. 饿汉式单例—— 程序启动时就已构造好实例
    HungrySingleton& h1 = HungrySingleton::Instance();
    HungrySingleton& h2 = HungrySingleton::Instance();
    h1.Log("第一次调用 HungrySingleton");
    h2.Log("第二次调用 HungrySingleton（同一实例）");
    std::cout << "HungrySingleton address: " << &h1 << " , " << &h2 << "\n\n";

    // 4. Meyers Singleton—— C++11 推荐写法
    MeyersSingleton& ms1 = MeyersSingleton::Instance();
    MeyersSingleton& ms2 = MeyersSingleton::Instance();
    ms1.Log("第一次调用 MeyersSingleton");
    ms2.Log("第二次调用 MeyersSingleton（同一实例）");
    std::cout << "MeyersSingleton address: " << &ms1 << " , " << &ms2 << "\n";

    return 0;
}
