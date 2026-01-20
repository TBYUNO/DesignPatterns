#pragma once

#include <iostream>
#include <memory>
#include <string>

// 代理模式（Proxy）C++ 示例
// --------------------------
// 示例 1：虚拟代理（Virtual Proxy）—— 延迟加载大资源
// 示例 2：日志代理（Logging Proxy）—— 为业务接口添加日志

// ===== 示例 1：虚拟代理 =====

// 抽象主题：图片接口
class Image {
public:
    virtual ~Image() = default;
    virtual void Display() = 0;
};

// 真实主题：真正的图片对象，模拟加载开销
class RealImage : public Image {
public:
    explicit RealImage(const std::string& filename)
        : filename_(filename) {
        // 构造时模拟“加载大图片”的耗时操作
        std::cout << "RealImage: loading image from '" << filename_ << "'..." << std::endl;
    }

    void Display() override {
        std::cout << "RealImage: display '" << filename_ << "'" << std::endl;
    }

private:
    std::string filename_;
};

// 代理主题：延迟在第一次 Display 时才真正创建 RealImage
class ProxyImage : public Image {
public:
    explicit ProxyImage(std::string filename)
        : filename_(std::move(filename)) {}

    void Display() override {
        // 第一次使用时才创建 RealImage
        if (!realImage_) {
            realImage_ = std::make_unique<RealImage>(filename_);
        }
        realImage_->Display();
    }

private:
    std::string filename_;
    std::unique_ptr<RealImage> realImage_;
};

// 演示虚拟代理
inline void RunVirtualProxyDemo() {
    std::cout << "--- Virtual Proxy Demo ---" << std::endl;

    ProxyImage image("big_picture.png");

    std::cout << "First display:" << std::endl;
    image.Display(); // 此时才真正加载图片

    std::cout << "\nSecond display:" << std::endl;
    image.Display(); // 直接使用已加载的 RealImage
}

// ===== 示例 2：日志代理 =====

// 抽象服务接口
class Service {
public:
    virtual ~Service() = default;
    virtual void DoWork(const std::string& task) = 0;
};

// 真实服务
class RealService : public Service {
public:
    void DoWork(const std::string& task) override {
        std::cout << "RealService: doing task '" << task << "'" << std::endl;
    }
};

// 日志代理：在调用真实服务前后输出日志
class LoggingServiceProxy : public Service {
public:
    explicit LoggingServiceProxy(std::shared_ptr<Service> real)
        : real_(std::move(real)) {}

    void DoWork(const std::string& task) override {
        std::cout << "[LOG] About to do task: " << task << std::endl;
        real_->DoWork(task);
        std::cout << "[LOG] Finished task: " << task << std::endl;
    }

private:
    std::shared_ptr<Service> real_;
};

inline void RunLoggingProxyDemo() {
    std::cout << "\n--- Logging Proxy Demo ---" << std::endl;

    auto real = std::make_shared<RealService>();
    LoggingServiceProxy proxy(real);

    Service& svc = proxy; // 客户端只依赖 Service 接口
    svc.DoWork("generate-report");
}
