#include "../../../src/behavioral/observer/Observer.h"
#include <gtest/gtest.h>
#include <thread>
#include <mutex>
#include <vector>

// 观察者模式测试套件

// 测试具体主题 - 添加观察者
TEST(ObserverTest, Subject_AddObserver) {
    Subject subject;
    auto observer = std::make_shared<ConcreteObserver>("Observer1");
    
    EXPECT_NO_THROW(subject.Attach(observer));
}

// 测试具体主题 - 设置状态
TEST(ObserverTest, Subject_SetState) {
    Subject subject;
    
    EXPECT_NO_THROW(subject.SetState(42));
    EXPECT_EQ(subject.GetState(), 42);
}

// 测试具体主题 - 通知观察者
TEST(ObserverTest, Subject_NotifyObservers) {
    Subject subject;
    auto observer1 = std::make_shared<ConcreteObserver>("Observer1");
    auto observer2 = std::make_shared<ConcreteObserver>("Observer2");
    
    subject.Attach(observer1);
    subject.Attach(observer2);
    
    // 更改状态以触发通知
    subject.SetState(100);
    
    EXPECT_EQ(subject.GetState(), 100);
}

// 测试观察者接收通知
TEST(ObserverTest, Observer_ReceiveNotification) {
    Subject subject;
    auto observer = std::make_shared<ConcreteObserver>("TestObserver");
    
    subject.Attach(observer);
    subject.SetState(55);
    
    // 验证可以正常通知
    EXPECT_NO_THROW(observer->Update(subject));
}

// 测试主题状态更改
TEST(ObserverTest, Subject_StateChange) {
    Subject subject;
    
    EXPECT_NO_THROW(subject.SetState(10));
    EXPECT_EQ(subject.GetState(), 10);
    
    EXPECT_NO_THROW(subject.SetState(20));
    EXPECT_EQ(subject.GetState(), 20);
}

// 测试推模型的观察者
TEST(ObserverTest, PushModel_Observer) {
    PushSubject subject;
    auto observer1 = std::make_shared<ConcretePushObserver>("PushObserver1");
    auto observer2 = std::make_shared<ConcretePushObserver>("PushObserver2");
    
    subject.Attach(observer1);
    subject.Attach(observer2);
    
    EXPECT_NO_THROW(subject.ChangeState(300));
}

// 测试线程安全的观察者模式
TEST(ObserverTest, ThreadSafeObserver_AddAndRemove) {
    ThreadSafeSubject subject;
    auto observer1 = std::make_shared<ThreadSafeObserver>("SafeObserver1");
    auto observer2 = std::make_shared<ThreadSafeObserver>("SafeObserver2");
    
    EXPECT_NO_THROW(observer1->SubscribeTo(subject));
    EXPECT_NO_THROW(observer2->SubscribeTo(subject));
    
    EXPECT_NO_THROW(subject.ChangeState(1000));
}

// 测试线程安全观察者的状态获取
TEST(ObserverTest, ThreadSafeObserver_GetState) {
    ThreadSafeSubject subject;
    
    subject.ChangeState(500);
    EXPECT_EQ(subject.GetState(), 500);
}

// 测试线程安全观察者清理过期观察者
TEST(ObserverTest, ThreadSafeObserver_RemoveExpired) {
    ThreadSafeSubject subject;
    auto observer = std::make_shared<ThreadSafeObserver>("ExpireObserver");
    
    observer->SubscribeTo(subject);
    subject.ChangeState(100);
    
    EXPECT_NO_THROW(subject.RemoveExpiredObservers());
}

// 测试RunPullObserverDemo
TEST(ObserverTest, RunPullObserverDemo) {
    EXPECT_NO_THROW(RunPullObserverDemo());
}

// 测试RunPushObserverDemo
TEST(ObserverTest, RunPushObserverDemo) {
    EXPECT_NO_THROW(RunPushObserverDemo());
}

// 测试RunThreadSafeObserverDemo
TEST(ObserverTest, RunThreadSafeObserverDemo) {
    EXPECT_NO_THROW(RunThreadSafeObserverDemo());
}

// 测试多线程访问观察者模式
TEST(ObserverTest, MultiThread_AccessObserver) {
    auto subject = std::make_shared<Subject>();
    
    // 启动多个线程来操作主题
    std::vector<std::thread> threads;
    const int numThreads = 3;
    
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back([subject, i]() {
            auto observer = std::make_shared<ConcreteObserver>("ThreadObserver" + std::to_string(i));
            subject->Attach(observer);
            subject->SetState(10 * (i + 1));
        });
    }
    
    // 等待所有线程完成
    for (auto& t : threads) {
        t.join();
    }
    
    // 验证最终状态
    EXPECT_GE(subject->GetState(), 10);
}

// 测试观察者在接收通知时的行为
TEST(ObserverTest, Observer_BehaviorDuringUpdate) {
    Subject subject;
    auto observer = std::make_shared<ConcreteObserver>("UpdateObserver");
    
    subject.Attach(observer);
    subject.SetState(999);
    
    // 观察者更新时不应该抛出异常
    EXPECT_NO_THROW(observer->Update(subject));
}