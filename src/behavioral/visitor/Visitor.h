#pragma once

#include <iostream>
#include <memory>
#include <vector>

// 访问者模式（Visitor）C++ 示例
// -----------------------------
// 元素结构 + 两种访问者：打印访问者 / 求和访问者

class ConcreteElementA;
class ConcreteElementB;

// 访问者接口
class Visitor {
public:
    virtual ~Visitor() = default;
    virtual void Visit(ConcreteElementA& element) = 0;
    virtual void Visit(ConcreteElementB& element) = 0;
};

// 元素接口
class Element {
public:
    virtual ~Element() = default;
    virtual void Accept(Visitor& visitor) = 0;
};

// 具体元素 A
class ConcreteElementA : public Element {
public:
    explicit ConcreteElementA(int value) : value_(value) {}

    void Accept(Visitor& visitor) override {
        visitor.Visit(*this);
    }

    int GetValue() const { return value_; }

private:
    int value_;
};

// 具体元素 B
class ConcreteElementB : public Element {
public:
    explicit ConcreteElementB(int value) : value_(value) {}

    void Accept(Visitor& visitor) override {
        visitor.Visit(*this);
    }

    int GetValue() const { return value_; }

private:
    int value_;
};

// 具体访问者：打印元素信息
class PrintVisitor : public Visitor {
public:
    void Visit(ConcreteElementA& element) override {
        std::cout << "ConcreteElementA with value = " << element.GetValue() << std::endl;
    }

    void Visit(ConcreteElementB& element) override {
        std::cout << "ConcreteElementB with value = " << element.GetValue() << std::endl;
    }
};

// 具体访问者：求和
class SumVisitor : public Visitor {
public:
    void Visit(ConcreteElementA& element) override {
        sum_ += element.GetValue();
    }

    void Visit(ConcreteElementB& element) override {
        sum_ += element.GetValue();
    }

    int GetSum() const { return sum_; }

private:
    int sum_{0};
};

// 对象结构：保存一组元素
class ObjectStructure {
public:
    void Add(std::shared_ptr<Element> element) {
        elements_.push_back(std::move(element));
    }

    void Accept(Visitor& visitor) {
        for (auto& e : elements_) {
            e->Accept(visitor);
        }
    }

private:
    std::vector<std::shared_ptr<Element>> elements_;
};

inline void RunVisitorDemo() {
    std::cout << "--- Visitor Demo ---" << std::endl;

    ObjectStructure obj;
    obj.Add(std::make_shared<ConcreteElementA>(10));
    obj.Add(std::make_shared<ConcreteElementB>(20));
    obj.Add(std::make_shared<ConcreteElementA>(5));

    PrintVisitor printVisitor;
    obj.Accept(printVisitor);

    SumVisitor sumVisitor;
    obj.Accept(sumVisitor);
    std::cout << "Sum of all element values = " << sumVisitor.GetSum() << std::endl;
}
