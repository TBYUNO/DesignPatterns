#pragma once

#include <iostream>
#include <memory>

// 抽象工厂模式（Abstract Factory）示例
// -------------------------------------
// 角色说明：
// - Button   / Checkbox ：抽象产品，定义一族产品中的不同种类
// - WindowsButton / MacButton 等：具体产品，实现不同平台/风格
// - GUIFactory       ：抽象工厂，声明创建一族产品的接口
// - WindowsFactory / MacFactory：具体工厂，实现一整套产品的创建逻辑
//
// 客户端只依赖抽象工厂和抽象产品，通过替换具体工厂即可切换“产品族”，
// 非常适合跨平台 UI、主题切换等场景。

// 抽象产品：按钮
class Button {
public:
    virtual ~Button() = default;
    virtual void Paint() = 0; // 绘制按钮
};

// 抽象产品：复选框
class Checkbox {
public:
    virtual ~Checkbox() = default;
    virtual void Paint() = 0; // 绘制复选框
};

// 具体产品：Windows 风格按钮
class WindowsButton : public Button {
public:
    void Paint() override {
        std::cout << "Render Windows Button" << std::endl;
    }
};

// 具体产品：Windows 风格复选框
class WindowsCheckbox : public Checkbox {
public:
    void Paint() override {
        std::cout << "Render Windows Checkbox" << std::endl;
    }
};

// 具体产品：Mac 风格按钮
class MacButton : public Button {
public:
    void Paint() override {
        std::cout << "Render Mac Button" << std::endl;
    }
};

// 具体产品：Mac 风格复选框
class MacCheckbox : public Checkbox {
public:
    void Paint() override {
        std::cout << "Render Mac Checkbox" << std::endl;
    }
};

// 抽象工厂：负责创建一整套 UI 控件
class GUIFactory {
public:
    virtual ~GUIFactory() = default;
    virtual std::unique_ptr<Button> CreateButton() const = 0;
    virtual std::unique_ptr<Checkbox> CreateCheckbox() const = 0;
};

// 具体工厂：Windows 工厂
class WindowsFactory : public GUIFactory {
public:
    std::unique_ptr<Button> CreateButton() const override {
        return std::make_unique<WindowsButton>();
    }

    std::unique_ptr<Checkbox> CreateCheckbox() const override {
        return std::make_unique<WindowsCheckbox>();
    }
};

// 具体工厂：Mac 工厂
class MacFactory : public GUIFactory {
public:
    std::unique_ptr<Button> CreateButton() const override {
        return std::make_unique<MacButton>();
    }

    std::unique_ptr<Checkbox> CreateCheckbox() const override {
        return std::make_unique<MacCheckbox>();
    }
};

// 客户端演示函数：只依赖抽象工厂与抽象产品
inline void RenderUI(const GUIFactory& factory) {
    auto button = factory.CreateButton();
    auto checkbox = factory.CreateCheckbox();
    button->Paint();
    checkbox->Paint();
}

inline void RunAbstractFactoryDemo() {
    WindowsFactory winFactory;
    MacFactory macFactory;

    std::cout << "Use WindowsFactory:" << std::endl;
    RenderUI(winFactory);

    std::cout << "\nUse MacFactory:" << std::endl;
    RenderUI(macFactory);
}
