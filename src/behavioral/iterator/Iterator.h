#pragma once

#include <iostream>
#include <memory>
#include <vector>

// 迭代器模式（Iterator）C++ 示例
// ------------------------------
// 示例：自定义容器 IntCollection + 前向迭代器

// 迭代器接口
class IIterator {
public:
    virtual ~IIterator() = default;
    virtual bool HasNext() const = 0;
    virtual int& Next() = 0;            // 返回当前元素引用并前进
    virtual int& Current() = 0;         // 返回当前元素引用
};

// 聚合：自定义整型集合
class IntCollection {
public:
    void Add(int value) { data_.push_back(value); }

    std::size_t Size() const { return data_.size(); }

    int& At(std::size_t index) { return data_.at(index); }
    const int& At(std::size_t index) const { return data_.at(index); }

private:
    std::vector<int> data_;

    // 迭代器需要访问内部实现
    friend class IntIterator;
};

// 具体迭代器
class IntIterator : public IIterator {
public:
    explicit IntIterator(IntCollection& collection)
        : collection_(collection), index_(0) {}

    bool HasNext() const override {
        return index_ < collection_.Size();
    }

    int& Next() override {
        return collection_.At(index_++);
    }

    int& Current() override {
        return collection_.At(index_);
    }

private:
    IntCollection& collection_;
    std::size_t index_;
};

inline void RunIteratorDemo() {
    std::cout << "--- Iterator Demo ---" << std::endl;

    IntCollection coll;
    coll.Add(1);
    coll.Add(2);
    coll.Add(3);

    IntIterator it(coll);

    // 遍历并将每个元素加 10
    while (it.HasNext()) {
        int& value = it.Next();
        std::cout << "Before: " << value;
        value += 10;
        std::cout << ", After: " << value << std::endl;
    }
}
