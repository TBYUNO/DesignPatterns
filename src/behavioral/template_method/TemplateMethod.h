#pragma once

#include <iostream>
#include <memory>
#include <string>

// 模板方法模式（Template Method）C++ 示例
// -------------------------------------
// 示例 1：制作饮料（茶 / 咖啡）
// 示例 2：数据处理流程

// ===== 示例 1：饮料制作模板 =====

class CaffeineBeverage {
public:
    virtual ~CaffeineBeverage() = default;

    // 模板方法：定义固定的制作流程
    void PrepareRecipe() {
        BoilWater();
        Brew();
        PourInCup();
        if (CustomerWantsCondiments()) {
            AddCondiments();
        }
    }

protected:
    void BoilWater() { std::cout << "Boiling water" << std::endl; }
    void PourInCup() { std::cout << "Pouring into cup" << std::endl; }

    virtual void Brew() = 0;          // 冲泡（由子类实现）
    virtual void AddCondiments() = 0; // 加调料（由子类实现）

    // 钩子方法：子类可以覆盖决定是否添加调料
    virtual bool CustomerWantsCondiments() { return true; }
};

class Tea : public CaffeineBeverage {
protected:
    void Brew() override {
        std::cout << "Steeping the tea" << std::endl;
    }

    void AddCondiments() override {
        std::cout << "Adding lemon" << std::endl;
    }
};

class Coffee : public CaffeineBeverage {
protected:
    void Brew() override {
        std::cout << "Dripping coffee through filter" << std::endl;
    }

    void AddCondiments() override {
        std::cout << "Adding sugar and milk" << std::endl;
    }
};

inline void RunBeverageTemplateDemo() {
    std::cout << "--- Beverage Template Demo ---" << std::endl;

    Tea tea;
    Coffee coffee;

    std::cout << "\nMake tea:" << std::endl;
    tea.PrepareRecipe();

    std::cout << "\nMake coffee:" << std::endl;
    coffee.PrepareRecipe();
}

// ===== 示例 2：数据处理模板 =====

class DataProcessor {
public:
    virtual ~DataProcessor() = default;

    void Process() {
        ReadData();
        TransformData();
        SaveResult();
    }

protected:
    virtual void ReadData() = 0;
    virtual void TransformData() = 0;
    virtual void SaveResult() = 0;
};

class CsvToJsonProcessor : public DataProcessor {
protected:
    void ReadData() override {
        std::cout << "Reading CSV data" << std::endl;
    }

    void TransformData() override {
        std::cout << "Transforming CSV to JSON" << std::endl;
    }

    void SaveResult() override {
        std::cout << "Saving JSON file" << std::endl;
    }
};

class LogFilterProcessor : public DataProcessor {
protected:
    void ReadData() override {
        std::cout << "Reading raw log entries" << std::endl;
    }

    void TransformData() override {
        std::cout << "Filtering errors and warnings" << std::endl;
    }

    void SaveResult() override {
        std::cout << "Saving filtered log" << std::endl;
    }
};

inline void RunDataProcessTemplateDemo() {
    std::cout << "\n--- Data Process Template Demo ---" << std::endl;

    CsvToJsonProcessor csv2json;
    csv2json.Process();

    LogFilterProcessor logFilter;
    logFilter.Process();
}
