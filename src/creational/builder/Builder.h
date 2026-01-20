#pragma once

#include <iostream>
#include <memory>
#include <string>

// 建造者模式（Builder）示例
// ---------------------------
// 角色说明：
// - Computer        ：复杂产品对象，包含多个组成部分
// - ComputerBuilder ：抽象建造者，定义构建各部分的抽象接口
// - GamingComputerBuilder / OfficeComputerBuilder：具体建造者，实现不同配置
// - Director        ：指挥者，负责按照既定步骤组织构建过程
//
// 适用场景：构建步骤大致固定，但不同产品在“每一步如何构建”上存在差异。

// 产品：电脑
struct Computer {
    std::string cpu;
    std::string gpu;
    std::string ram;
    std::string storage;

    void Show() const {
        std::cout << "CPU: " << cpu << "\n"
                  << "GPU: " << gpu << "\n"
                  << "RAM: " << ram << "\n"
                  << "Storage: " << storage << std::endl;
    }
};

// 抽象建造者
class ComputerBuilder {
public:
    virtual ~ComputerBuilder() = default;
    virtual void BuildCPU() = 0;
    virtual void BuildGPU() = 0;
    virtual void BuildRAM() = 0;
    virtual void BuildStorage() = 0;
    virtual std::unique_ptr<Computer> GetResult() = 0;
};

// 具体建造者：游戏电脑
class GamingComputerBuilder : public ComputerBuilder {
public:
    GamingComputerBuilder() { computer_ = std::make_unique<Computer>(); }

    void BuildCPU() override { computer_->cpu = "High-end CPU"; }
    void BuildGPU() override { computer_->gpu = "High-end GPU"; }
    void BuildRAM() override { computer_->ram = "32GB"; }
    void BuildStorage() override { computer_->storage = "1TB SSD"; }
    std::unique_ptr<Computer> GetResult() override { return std::move(computer_); }

private:
    std::unique_ptr<Computer> computer_;
};

// 具体建造者：办公电脑
class OfficeComputerBuilder : public ComputerBuilder {
public:
    OfficeComputerBuilder() { computer_ = std::make_unique<Computer>(); }

    void BuildCPU() override { computer_->cpu = "Mid-range CPU"; }
    void BuildGPU() override { computer_->gpu = "Integrated GPU"; }
    void BuildRAM() override { computer_->ram = "16GB"; }
    void BuildStorage() override { computer_->storage = "512GB SSD"; }
    std::unique_ptr<Computer> GetResult() override { return std::move(computer_); }

private:
    std::unique_ptr<Computer> computer_;
};

// 指挥者：封装构建步骤
class Director {
public:
    void Construct(ComputerBuilder& builder) {
        // 构建顺序在这里固定，具体每一步的实现由不同 builder 决定
        builder.BuildCPU();
        builder.BuildGPU();
        builder.BuildRAM();
        builder.BuildStorage();
    }
};

inline void RunBuilderDemo() {
    Director director;

    GamingComputerBuilder gamingBuilder;
    director.Construct(gamingBuilder);
    auto gamingPC = gamingBuilder.GetResult();

    OfficeComputerBuilder officeBuilder;
    director.Construct(officeBuilder);
    auto officePC = officeBuilder.GetResult();

    std::cout << "Gaming PC:\n";
    gamingPC->Show();

    std::cout << "\nOffice PC:\n";
    officePC->Show();
}
