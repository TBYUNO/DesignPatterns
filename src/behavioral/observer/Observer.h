#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <mutex>
#include <shared_mutex>
#include <algorithm>

// 观察者模式（Observer）C++ 示例
// ------------------------------
// 示例 1：拉模型（Observer 从 Subject 拉取数据）
// 示例 2：推模型（Subject 主动推送数据）
// 示例 3：线程安全版本（C++17+）
//
// ==========================
// 线程安全与性能优化
// ==========================
// 1. 线程安全问题：
//    - 观察者列表的并发访问（添加/删除/遍历）
//    - 通知过程中观察者自我注销导致迭代器失效
//    - 死锁风险（观察者在 Update 中反向调用 Subject）
// 2. 性能优化：
//    - 使用读写锁（std::shared_mutex）优化多读场景
//    - 异步通知（事件队列）
//    - weak_ptr 避免循环引用
//    - 批量通知减少锁开销
// 3. C++ 标准版本特性：
//    - C++11: std::mutex, std::weak_ptr, lambda
//    - C++14: auto 返回类型
//    - C++17: std::shared_mutex (读写锁)
//    - C++20: std::jthread, 协程
//    - C++23: std::expected 用于错误处理

// ===== 示例 1：拉模型 =====

class Subject; // 前向声明

class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void Update(const Subject& subject) = 0; // 拉模型：从 subject 获取数据
};

class Subject {
public:
    void Attach(std::shared_ptr<IObserver> obs) { observers_.push_back(obs); }

    void SetState(int state) {
        state_ = state;
        Notify();
    }

    int GetState() const { return state_; }

    void Notify() const {
        for (const auto& obs : observers_) {
            obs->Update(*this);
        }
    }

private:
    int state_{};
    std::vector<std::shared_ptr<IObserver>> observers_;
};

class ConcreteObserver : public IObserver {
public:
    explicit ConcreteObserver(std::string name) : name_(std::move(name)) {}

    void Update(const Subject& subject) override {
        std::cout << name_ << " observed new state: " << subject.GetState() << std::endl;
    }

private:
    std::string name_;
};

inline void RunPullObserverDemo() {
    std::cout << "--- Pull Observer Demo ---" << std::endl;

    Subject subject;
    auto obs1 = std::make_shared<ConcreteObserver>("Observer1");
    auto obs2 = std::make_shared<ConcreteObserver>("Observer2");

    subject.Attach(obs1);
    subject.Attach(obs2);

    subject.SetState(10);
    subject.SetState(20);
}

// ===== 示例 2：推模型 =====

class IPushObserver {
public:
    virtual ~IPushObserver() = default;
    virtual void Update(int newState) = 0; // 推模型：直接接收状态
};

class PushSubject {
public:
    void Attach(std::shared_ptr<IPushObserver> obs) { observers_.push_back(obs); }

    void ChangeState(int newState) {
        state_ = newState;
        Notify();
    }

private:
    void Notify() const {
        for (const auto& obs : observers_) {
            obs->Update(state_);
        }
    }

    int state_{};
    std::vector<std::shared_ptr<IPushObserver>> observers_;
};

class ConcretePushObserver : public IPushObserver {
public:
    explicit ConcretePushObserver(std::string name) : name_(std::move(name)) {}

    void Update(int newState) override {
        std::cout << name_ << " got pushed state: " << newState << std::endl;
    }

private:
    std::string name_;
};

inline void RunPushObserverDemo() {
    std::cout << "\n--- Push Observer Demo ---" << std::endl;

    PushSubject subject;
    auto obs1 = std::make_shared<ConcretePushObserver>("PushObserver1");
    auto obs2 = std::make_shared<ConcretePushObserver>("PushObserver2");

    subject.Attach(obs1);
    subject.Attach(obs2);

    subject.ChangeState(100);
    subject.ChangeState(200);
}

// ===== 示例 3：线程安全版本 =====
// C++17: 使用 std::shared_mutex 实现读写锁
// 优点：多个观察者可以并发读取，提高性能

class IThreadSafeObserver {
public:
    virtual ~IThreadSafeObserver() = default;
    virtual void Update(int state) = 0;
};

class ThreadSafeSubject {
public:
    // 添加观察者（使用 weak_ptr 避免循环引用）
    void Attach(std::weak_ptr<IThreadSafeObserver> obs) {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        observers_.push_back(std::move(obs));
    }

    // 移除已过期的观察者
    void RemoveExpiredObservers() {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        observers_.erase(
            std::remove_if(observers_.begin(), observers_.end(),
                [](const std::weak_ptr<IThreadSafeObserver>& wp) {
                    return wp.expired();
                }),
            observers_.end()
        );
    }

    void ChangeState(int newState) {
        state_ = newState;
        Notify();
    }

    int GetState() const {
        std::shared_lock<std::shared_mutex> lock(mutex_);
        return state_;
    }

private:
    void Notify() {
        // 复制一份观察者列表，避免通知时长时间持有锁
        std::vector<std::weak_ptr<IThreadSafeObserver>> observersCopy;
        {
            std::shared_lock<std::shared_mutex> lock(mutex_);
            observersCopy = observers_;
        }

        // 在锁外通知观察者，减少锁竞争，避免死锁
        for (auto& wp : observersCopy) {
            if (auto obs = wp.lock()) {
                obs->Update(state_);
            }
        }
    }

    int state_{0};
    mutable std::shared_mutex mutex_; // C++17: 读写锁
    std::vector<std::weak_ptr<IThreadSafeObserver>> observers_;
};

class ThreadSafeObserver : public IThreadSafeObserver,
                           public std::enable_shared_from_this<ThreadSafeObserver> {
public:
    explicit ThreadSafeObserver(std::string name) : name_(std::move(name)) {}

    void Update(int newState) override {
        std::lock_guard<std::mutex> lock(mutex_);
        std::cout << name_ << " received state update: " << newState << std::endl;
    }

    // 注册到主题
    void SubscribeTo(ThreadSafeSubject& subject) {
        subject.Attach(weak_from_this());
    }

private:
    std::string name_;
    std::mutex mutex_; // 保护观察者内部状态
};

inline void RunThreadSafeObserverDemo() {
    std::cout << "\n--- Thread-Safe Observer Demo ---" << std::endl;

    ThreadSafeSubject subject;
    
    auto obs1 = std::make_shared<ThreadSafeObserver>("SafeObserver1");
    auto obs2 = std::make_shared<ThreadSafeObserver>("SafeObserver2");

    obs1->SubscribeTo(subject);
    obs2->SubscribeTo(subject);

    subject.ChangeState(1000);
    subject.ChangeState(2000);
    
    // 清理过期观察者
    subject.RemoveExpiredObservers();
}

/* C++20 版本：使用协程实现异步观察者
#include <coroutine>
#include <queue>

class AsyncSubject {
public:
    struct Awaitable {
        AsyncSubject* subject;
        
        bool await_ready() const noexcept { return false; }
        void await_suspend(std::coroutine_handle<> handle) {
            subject->RegisterWaiter(handle);
        }
        int await_resume() const noexcept { return subject->state_; }
    };
    
    Awaitable WaitForChange() { return Awaitable{this}; }
    
    void ChangeState(int newState) {
        state_ = newState;
        NotifyWaiters();
    }
    
private:
    void RegisterWaiter(std::coroutine_handle<> handle) {
        waiters_.push(handle);
    }
    
    void NotifyWaiters() {
        while (!waiters_.empty()) {
            auto handle = waiters_.front();
            waiters_.pop();
            handle.resume();
        }
    }
    
    int state_{0};
    std::queue<std::coroutine_handle<>> waiters_;
};
*/
