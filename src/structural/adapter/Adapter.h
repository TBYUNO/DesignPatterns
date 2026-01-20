#pragma once

#include <iostream>
#include <memory>
#include <string>

// 适配器模式（Adapter）C++ 示例
// ------------------------------
// 本文件演示两种典型适配方式：
// 1）类适配器（ClassAdapter）：通过多重继承实现适配；
// 2）对象适配器（ObjectAdapter）：通过组合持有被适配对象实现适配；
//
// 场景：已有一个 OldPrinter，只支持 OldPrint(const char*) 接口，
//      而新系统要求统一使用 ITarget::Print(const std::string&) 接口。

// 目标接口：客户端期望使用的统一打印接口
class ITarget {
public:
    virtual ~ITarget() = default;

    // 对外统一的打印接口，使用 std::string
    virtual void Print(const std::string& text) = 0;
};

// 被适配者：旧打印类，只支持 const char* 的接口，接口形式不符合新系统要求
class OldPrinter {
public:
    void OldPrint(const char* text) {
        std::cout << "[OldPrinter] " << text << std::endl;
    }
};

// 1. 类适配器：通过继承 OldPrinter + 实现 ITarget
//    - 适配器本身既是 Target，又是 Adaptee 的“子类”
class ClassAdapter : public ITarget, public OldPrinter {
public:
    void Print(const std::string& text) override {
        // 在这里进行接口转换：std::string -> const char*
        OldPrint(text.c_str());
    }
};

// 2. 对象适配器：通过组合持有一个 OldPrinter 实例
class ObjectAdapter : public ITarget {
public:
    // 可以在构造函数中注入已有的 OldPrinter
    explicit ObjectAdapter(std::shared_ptr<OldPrinter> printer)
        : printer_(std::move(printer)) {}

    void Print(const std::string& text) override {
        if (printer_) {
            printer_->OldPrint(text.c_str());
        }
    }

private:
    std::shared_ptr<OldPrinter> printer_;
};

// 演示：使用类适配器
inline void RunClassAdapterDemo() {
    std::cout << "--- ClassAdapter Demo ---" << std::endl;
    ClassAdapter adapter;
    ITarget& target = adapter; // 客户端只依赖 ITarget
    target.Print("Hello from ClassAdapter");
}

// 演示：使用对象适配器
inline void RunObjectAdapterDemo() {
    std::cout << "--- ObjectAdapter Demo ---" << std::endl;
    auto oldPrinter = std::make_shared<OldPrinter>();
    ObjectAdapter adapter(oldPrinter);
    ITarget& target = adapter;
    target.Print("Hello from ObjectAdapter");
}

// 综合演示入口
inline void RunAdapterDemo() {
    RunClassAdapterDemo();
    std::cout << std::endl;
    RunObjectAdapterDemo();
}
