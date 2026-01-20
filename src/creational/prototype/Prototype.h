#pragma once

#include <iostream>
#include <memory>
#include <string>

// 原型模式（Prototype）示例
// ---------------------------
// 角色说明：
// - Prototype         ：抽象原型，声明 Clone 接口
// - ConcretePrototype ：具体原型，实现深拷贝逻辑
//
// 适用场景：
// - 创建对象成本较高（如复杂初始化、深层对象图等）；
// - 需要频繁创建“大致相同但略有差异”的对象。

// 抽象原型
class Prototype {
public:
    virtual ~Prototype() = default;

    // 克隆接口：返回一个新的堆对象，由调用者负责管理（这里用智能指针）
    virtual std::unique_ptr<Prototype> Clone() const = 0;
    virtual void Show() const = 0;
};

// 具体原型
class ConcretePrototype : public Prototype {
public:
    ConcretePrototype(std::string name, int value)
        : name_(std::move(name)), value_(value) {}

    std::unique_ptr<Prototype> Clone() const override {
        // 此处为“深拷贝”：拷贝所有必要的成员
        return std::make_unique<ConcretePrototype>(*this);
    }

    void Show() const override {
        std::cout << "ConcretePrototype{name=" << name_ << ", value=" << value_ << "}" << std::endl;
    }

private:
    std::string name_;
    int value_;
};

inline void RunPrototypeDemo() {
    // 创建一个原型对象
    std::unique_ptr<Prototype> p1 = std::make_unique<ConcretePrototype>("origin", 42);
    p1->Show();

    // 通过克隆创建新对象，而不是重新构造
    auto p2 = p1->Clone();
    p2->Show();
}
