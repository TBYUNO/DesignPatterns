#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

// 组合模式（Composite）C++ 示例
// ------------------------------
// 本文件以“文件系统目录树”为例演示组合模式：
// - Component：抽象节点，统一文件与目录的接口；
// - File     ：叶子节点，表示文件；
// - Directory：组合节点，表示目录，可包含若干子节点。

// 抽象构件：统一的文件系统节点接口
class Component {
public:
    explicit Component(std::string name) : name_(std::move(name)) {}
    virtual ~Component() = default;

    // 打印节点信息（包含层级缩进）
    virtual void Operation(int indent = 0) const = 0;

    // 计算节点大小（单位：KB，仅为演示，实际项目可用字节等）
    virtual std::size_t GetSize() const = 0;

    // 组合相关操作：默认对叶子节点无效，由 Directory 重写
    virtual void Add(std::shared_ptr<Component> /*child*/) {}

protected:
    std::string name_;

    static void PrintIndent(int indent) {
        for (int i = 0; i < indent; ++i) {
            std::cout << "  ";
        }
    }
};

// 叶子节点：文件
class File : public Component {
public:
    File(const std::string& name, std::size_t sizeKB)
        : Component(name), sizeKB_(sizeKB) {}

    void Operation(int indent = 0) const override {
        PrintIndent(indent);
        std::cout << "- " << name_ << " (file, " << sizeKB_ << " KB)" << std::endl;
    }

    std::size_t GetSize() const override { return sizeKB_; }

private:
    std::size_t sizeKB_{};
};

// 组合节点：目录
class Directory : public Component {
public:
    explicit Directory(const std::string& name)
        : Component(name) {}

    void Operation(int indent = 0) const override {
        PrintIndent(indent);
        std::cout << "+ " << name_ << " (dir)" << std::endl;
        for (const auto& child : children_) {
            child->Operation(indent + 1);
        }
    }

    std::size_t GetSize() const override {
        std::size_t total = 0;
        for (const auto& child : children_) {
            total += child->GetSize();
        }
        return total;
    }

    void Add(std::shared_ptr<Component> child) override {
        children_.push_back(std::move(child));
    }

private:
    std::vector<std::shared_ptr<Component>> children_;
};

// 构建一棵示例目录树
inline std::shared_ptr<Directory> BuildSampleFileTree() {
    auto root = std::make_shared<Directory>("root");

    auto dirSrc = std::make_shared<Directory>("src");
    dirSrc->Add(std::make_shared<File>("main.cpp", 4));
    dirSrc->Add(std::make_shared<File>("util.cpp", 2));

    auto dirInclude = std::make_shared<Directory>("include");
    dirInclude->Add(std::make_shared<File>("util.h", 1));

    auto dirDocs = std::make_shared<Directory>("docs");
    dirDocs->Add(std::make_shared<File>("readme.md", 1));

    root->Add(dirSrc);
    root->Add(dirInclude);
    root->Add(dirDocs);

    return root;
}

// 示例 1：打印目录树结构
inline void RunCompositePrintDemo() {
    std::cout << "--- Composite Print Demo ---" << std::endl;
    auto root = BuildSampleFileTree();
    root->Operation();
}

// 示例 2：计算整个目录的总大小
inline void RunCompositeSizeDemo() {
    std::cout << "\n--- Composite Size Demo ---" << std::endl;
    auto root = BuildSampleFileTree();
    std::cout << "Total size: " << root->GetSize() << " KB" << std::endl;
}
